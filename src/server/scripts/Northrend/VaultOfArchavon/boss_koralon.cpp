/*
 * Copyright (C) 2008-2010 TrinityCore <http://www.trinitycore.org/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "ScriptPCH.h"
#include "vault_of_archavon.h"

enum Events
{
    EVENT_NONE,

    // Koralon
    EVENT_BURNING_BREATH,
    EVENT_BURNING_FURY,
    EVENT_FLAME_CINDER_A,
    EVENT_METEOR_FISTS_A,
    EVENT_METEOR_FISTS_B,

    // Flame Warder
    EVENT_FW_LAVA_BIRST,
    EVENT_FW_METEOR_FISTS_A,
    EVENT_FW_METEOR_FISTS_B,
};

enum Spells
{
    // Spells Koralon
    //SPELL_BURNING_BREATH                        = 66665,
    SPELL_BURNING_BREATH                        = 63677, //Hack    
    //SPELL_BURNING_BREATH_H                      = 67328,
    SPELL_BURNING_BREATH_H                      = 64533, //Hack
    SPELL_BURNING_FURY                          = 66721,
    SPELL_FLAME_CINDER_A                        = 66684,
    SPELL_FLAME_CINDER_A_H                      = 67332,
    SPELL_FLAME_CINDER_B                        = 66681, // don't know the real relation to SPELL_FLAME_CINDER_A atm.
    SPELL_METEOR_FISTS_A                        = 66725,
    SPELL_METEOR_FISTS_A_H                      = 66765,
    SPELL_METEOR_FISTS_B                        = 67333,
    SPELL_METEOR_FISTS_B_H                      = 68161,

    // Spells Flame Warder
    SPELL_FW_LAVA_BIRST                         = 66813,
    SPELL_FW_LAVA_BIRST_H                       = 67330,
    SPELL_FW_METEOR_FISTS_A                     = 66808,
    SPELL_FW_METEOR_FISTS_A_H                   = 66809,
    SPELL_FW_METEOR_FISTS_B                     = 67331,
    SPELL_FW_METEOR_FISTS_B_H                   = 68160,
};

class boss_koralon : public CreatureScript
{
    public:
        boss_koralon(): CreatureScript("boss_koralon") {}

    struct boss_koralonAI : public ScriptedAI
    {
        boss_koralonAI(Creature *c) : ScriptedAI(c)
        {
            pInstance = c->GetInstanceScript();
        }

        InstanceScript *pInstance;
        EventMap events;
        uint32 checktimer;
        uint8 WatchersCount;

        void Reset()
        {
            events.Reset();

            CheckForVoA();

            checktimer = 10000;
            WatchersCount = 0;

            if (pInstance)
                pInstance->SetData(DATA_KORALON_EVENT, NOT_STARTED);
        }

        void CheckForVoA()
        {
            if (!sOutdoorPvPMgr->CanBeAttacked(me))
            {
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE|UNIT_FLAG_NON_ATTACKABLE|UNIT_FLAG_DISABLE_MOVE);
                me->SetReactState(REACT_PASSIVE);
            }
            else
            {
                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE|UNIT_FLAG_NON_ATTACKABLE|UNIT_FLAG_DISABLE_MOVE);
                me->SetReactState(REACT_AGGRESSIVE);
            }
        }

        void KilledUnit(Unit* Victim) {}

        void JustDied(Unit* /*Killer*/)
        {
            if (Creature* Emalon = Unit::GetCreature(*me, pInstance ? pInstance->GetData64(DATA_EMALON) : 0))
                if (Emalon->isAlive()) 
                    WatchersCount++;
                
            if (Creature* Archavon = Unit::GetCreature(*me, pInstance ? pInstance->GetData64(DATA_ARCHAVON) : 0))
                if (Archavon->isAlive())
                    WatchersCount++;

            if (pInstance)
            {
                if (WatchersCount == 2)
                    pInstance->SetData(DATA_EWF_START, ACHI_START);                

                pInstance->SetData(DATA_EWF_COUNT, ACHI_INCREASE);
                pInstance->SetData(DATA_KORALON_EVENT, DONE);
                pInstance->SaveToDB();
            }
        }

        void EnterCombat(Unit *who)
        {
            DoZoneInCombat();

            DoCast(me, SPELL_BURNING_FURY);

            events.ScheduleEvent(EVENT_BURNING_FURY, 20000);    // timer ok
            events.ScheduleEvent(EVENT_BURNING_BREATH, 15000);  // 1st after 15sec, then every 45sec
            events.ScheduleEvent(EVENT_METEOR_FISTS_A, 30000);  // 1st after 30sec, then every 45sec
            events.ScheduleEvent(EVENT_FLAME_CINDER_A, 5000);  // timer ok - TODO: check for RANDOM target

            if (pInstance)
                pInstance->SetData(DATA_KORALON_EVENT, IN_PROGRESS);
        }

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
            {
                if (checktimer <= diff)
                {
                    CheckForVoA();
                    checktimer = 10000;
                } else checktimer -= diff;

                return;
            }

            events.Update(diff);

            if (me->HasUnitState(UNIT_STAT_CASTING))
                return;

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch(eventId)
                {
                    case EVENT_BURNING_FURY:
                        DoCast(me, SPELL_BURNING_FURY);
                        events.ScheduleEvent(EVENT_BURNING_FURY, 20000);
                        return;
                    case EVENT_BURNING_BREATH:
                        DoCast(me, RAID_MODE(SPELL_BURNING_BREATH,SPELL_BURNING_BREATH_H));
                        events.ScheduleEvent(EVENT_BURNING_BREATH, 45000);
                        return;
                    case EVENT_METEOR_FISTS_A:
                        DoCast(me, RAID_MODE(SPELL_METEOR_FISTS_A,SPELL_METEOR_FISTS_A_H));
                        events.ScheduleEvent(EVENT_METEOR_FISTS_B, 1500);
                        return;
                    case EVENT_METEOR_FISTS_B:
                        DoCast(me, RAID_MODE(SPELL_METEOR_FISTS_B,SPELL_METEOR_FISTS_B_H));
                        events.ScheduleEvent(EVENT_METEOR_FISTS_A, 45000);
                        return;
                    case EVENT_FLAME_CINDER_A:
                        if (Unit* pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0, 100, true))
                            DoCast(pTarget, RAID_MODE(SPELL_FLAME_CINDER_A,SPELL_FLAME_CINDER_A_H));
                        events.ScheduleEvent(EVENT_FLAME_CINDER_A, 5000);
                        return;
                }
            }
            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new boss_koralonAI (pCreature);
    };
};

/*######
##  Mob Flame Warder
######*/

class mob_flame_warder : public CreatureScript
{
    public:
        mob_flame_warder(): CreatureScript("mob_flame_warder") {}

    struct mob_flame_warderAI : public ScriptedAI
    {
        mob_flame_warderAI(Creature *c) : ScriptedAI(c) {}

        EventMap events;

        void Reset()
        {
            events.Reset();
        }

        void EnterCombat(Unit *who)
        {
            DoZoneInCombat();

            events.ScheduleEvent(EVENT_FW_LAVA_BIRST, 5000);
            events.ScheduleEvent(EVENT_FW_METEOR_FISTS_A, 10000);
        }

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch(eventId)
                {
                    case EVENT_FW_LAVA_BIRST:
                        DoCast(me->getVictim(), RAID_MODE(SPELL_FW_LAVA_BIRST,SPELL_FW_LAVA_BIRST_H));
                        events.ScheduleEvent(EVENT_FW_LAVA_BIRST, 15000);
                        return;
                    case EVENT_FW_METEOR_FISTS_A:
                        DoCast(me, RAID_MODE(SPELL_FW_METEOR_FISTS_A,SPELL_FW_METEOR_FISTS_A_H));
                        events.ScheduleEvent(EVENT_FW_METEOR_FISTS_B, 1500);
                        return;
                    case EVENT_FW_METEOR_FISTS_B:
                        DoCast(me, RAID_MODE(SPELL_FW_METEOR_FISTS_B,SPELL_FW_METEOR_FISTS_B_H));
                        events.ScheduleEvent(EVENT_FW_METEOR_FISTS_A, 20000);
                        return;
                }
            }
            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new mob_flame_warderAI (pCreature);
    };
};

void AddSC_boss_koralon()
{
    new boss_koralon();
    new mob_flame_warder();
}
