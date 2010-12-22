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

// Spells Toravon
#define SPELL_FREEZING_GROUND   RAID_MODE(72090,72104)  // don't know cd... using 20 secs.
#define SPELL_FROZEN_ORB        RAID_MODE(72091,72095)
#define SPELL_WHITEOUT          RAID_MODE(72034,72096)  // Every 38 sec. cast. (after SPELL_FROZEN_ORB)
#define SPELL_FROZEN_MALLET     71993

// Spells Frost Warder
#define SPELL_FROST_BLAST   RAID_MODE(72123,72124)      // don't know cd... using 20 secs.
#define SPELL_FROZEN_MALLET_2 72122

// Spell Frozen Orb
#define SPELL_FROZEN_ORB_DMG    72081   // priodic dmg aura
#define SPELL_FROZEN_ORB_AURA   72067   // make visible

// Spell Frozen Orb Stalker
#define SPELL_FROZEN_ORB_SUMMON 72093   // summon orb

// Events boss
#define EVENT_FREEZING_GROUND   1
#define EVENT_FROZEN_ORB        2
#define EVENT_WHITEOUT          3

// Events mob
#define EVENT_FROST_BLAST   4

// Mob Frozen Orb
#define MOB_FROZEN_ORB          38456    // 1 in 10 mode and 3 in 25 mode
#define MOB_FROZEN_ORB_STALKER  38461   // 1 of them is spawned for each player through SPELL_FROZEN_ORB!

class boss_toravon : public CreatureScript
{
    public:
        boss_toravon(): CreatureScript("boss_toravon") {}

    struct boss_toravonAI : public ScriptedAI
    {
        boss_toravonAI(Creature *c) : ScriptedAI(c)
        {
            pInstance = c->GetInstanceScript();

            if (getDifficulty() == RAID_DIFFICULTY_25MAN_NORMAL)
                num_orbs = 3;
            else
                num_orbs = 1;
        }

        InstanceScript *pInstance;
        EventMap events;
        uint32 spawntimer;
        uint32 checktimer;
        uint8 num_orbs;

        void Reset()
        {
            // Only for orbs because the stalker despawns automatic
            std::list<Creature*> OrbList;
            GetCreatureListWithEntryInGrid(OrbList, me, MOB_FROZEN_ORB, 150.0f);
            for (std::list<Creature*>::iterator iter = OrbList.begin(); iter != OrbList.end(); ++iter)
            {
                if (Creature* pOrb = *iter)
                    pOrb->ForcedDespawn();
            }

            events.Reset();

            CheckForVoA();

            checktimer = 10000;
            spawntimer = 0;

            if (pInstance)
                pInstance->SetData(DATA_TORAVON_EVENT, NOT_STARTED);
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
            if (pInstance)
            {
                pInstance->SetData(DATA_TORAVON_EVENT, DONE);
                pInstance->SaveToDB();
            }
        }

        void EnterCombat(Unit *who)
        {
            DoZoneInCombat();

            DoCast(me, SPELL_FROZEN_MALLET);

            events.ScheduleEvent(EVENT_FROZEN_ORB, 11000);
            events.ScheduleEvent(EVENT_WHITEOUT, 13000);
            events.ScheduleEvent(EVENT_FREEZING_GROUND, 15000);

            if (pInstance)
                pInstance->SetData(DATA_TORAVON_EVENT, IN_PROGRESS);
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

            if (spawntimer && spawntimer <= diff)
            {
                uint8 cnt = 0;
                std::list<Creature*> StalkerList;
                GetCreatureListWithEntryInGrid(StalkerList, me, MOB_FROZEN_ORB_STALKER, 50.0f);
                for (std::list<Creature*>::iterator iter = StalkerList.begin(); iter != StalkerList.end(); ++iter)
                {
                    if (cnt < num_orbs)
                    {
                        if (Creature* pStalker = *iter)
                            pStalker->AI()->DoCast(pStalker, SPELL_FROZEN_ORB_SUMMON);

                        ++cnt;
                    }
                    else
                        break;
                }
                spawntimer = 0;
            }
            else
                if (spawntimer)
                    spawntimer -= diff;

            if (me->HasUnitState(UNIT_STAT_CASTING))
                return;

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch(eventId)
                {
                    case EVENT_FROZEN_ORB:
                        DoCast(me, SPELL_FROZEN_ORB);
                        events.ScheduleEvent(EVENT_FROZEN_ORB, 38000);
                        spawntimer = 2200; // Because the casttime of SPELL_FROZEN_ORB is 2 secs.
                        return;
                    case EVENT_WHITEOUT:
                        DoCast(me, SPELL_WHITEOUT);
                        events.ScheduleEvent(EVENT_WHITEOUT, 38000);
                        return;
                    case EVENT_FREEZING_GROUND:
                        if (Unit *pTarget = SelectUnit(SELECT_TARGET_RANDOM, 0))
                            DoCast(pTarget, SPELL_FREEZING_GROUND);
                        events.ScheduleEvent(EVENT_FREEZING_GROUND, 20000);
                        return;
                }
            }
            DoMeleeAttackIfReady();
        }
    };
    
    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new boss_toravonAI (pCreature);
    };
};

/*######
##  Mob Frost Warder
######*/

class mob_frost_warder : public CreatureScript
{
    public:
        mob_frost_warder(): CreatureScript("mob_frost_warder") {}

    struct mob_frost_warderAI : public ScriptedAI
    {
        mob_frost_warderAI(Creature *c) : ScriptedAI(c) {}

        EventMap events;

        void Reset()
        {
            events.Reset();
        }

        void EnterCombat(Unit *who)
        {
            DoZoneInCombat();

            DoCast(me, SPELL_FROZEN_MALLET_2);

            events.ScheduleEvent(EVENT_FROST_BLAST, 5000);
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
                    case EVENT_FROST_BLAST:
                        DoCast(me->getVictim(), SPELL_FROST_BLAST);
                        events.ScheduleEvent(EVENT_FROST_BLAST, 20000);
                        return;
                }
            }
            DoMeleeAttackIfReady();
        }
    };
    
    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new mob_frost_warderAI (pCreature);
    };
};

/*######
##  Mob Frozen Orb
######*/

class mob_frozen_orb : public CreatureScript
{
    public:
        mob_frozen_orb(): CreatureScript("mob_frozen_orb") {}

    struct mob_frozen_orbAI : public ScriptedAI
    {
        mob_frozen_orbAI(Creature *c) : ScriptedAI(c)
        {
            pInstance = c->GetInstanceScript();
            done = false;
        }

        bool done;
        uint32 killtimer;
        InstanceScript *pInstance;

        void Reset() {}

        void EnterCombat(Unit *who)
        {
            DoZoneInCombat();
        }

        void UpdateAI(const uint32 diff)
        {
            if (!done)
            {
                DoCast(me, SPELL_FROZEN_ORB_AURA, true);
                DoCast(me, SPELL_FROZEN_ORB_DMG, true);
                done = true;
            }

            if (!UpdateVictim() && pInstance)
            {
                Unit* pToravon = me->GetMap()->GetCreature(pInstance->GetData64(DATA_TORAVON));
                if (pToravon)
                {
                    Unit* pTarget = pToravon->SelectNearbyTarget(10);
                    if (pTarget)
                        me->AI()->AttackStart(pTarget);
                }
            }
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new mob_frozen_orbAI (pCreature);
    };
};

/*###### 
##  Mob Frozen Orb Stalker
######*/

class mob_frozen_orb_stalker : public CreatureScript
{
    public:
        mob_frozen_orb_stalker(): CreatureScript("mob_frozen_orb_stalker") {}

    struct mob_frozen_orb_stalkerAI : public Scripted_NoMovementAI
    {
        mob_frozen_orb_stalkerAI(Creature* c) : Scripted_NoMovementAI(c)
        {
            c->SetVisible(false);
            c->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE|UNIT_FLAG_NON_ATTACKABLE|UNIT_FLAG_DISABLE_MOVE);
            c->SetReactState(REACT_PASSIVE);
        }

        void UpdateAI(const uint32 diff) {}
    };

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new mob_frozen_orb_stalkerAI (pCreature);
    };
};

void AddSC_boss_toravon()
{
    new boss_toravon();
    new mob_frost_warder();
    new mob_frozen_orb();
    new mob_frozen_orb_stalker();
}
