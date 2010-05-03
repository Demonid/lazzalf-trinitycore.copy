/*
 * Copyright (C) 2008 - 2009 Trinity <http://www.trinitycore.org/>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */
 
 /* ScriptData
SDName: Hodir
SDAuthor: PrinceCreed
SD%Complete: 80
SDComments: TODO: NPC helper scripts and Achievements
EndScriptData */

#include "ScriptedPch.h"
#include "ulduar.h"

// Hodir spells
#define SPELL_FROZEN_BLOWS                      RAID_MODE(62478, 63512)
#define SPELL_FLASH_FREEZE                      61968
#define SPELL_BITING_COLD                       48094 //62038 from Keristrasza
#define SPELL_FREEZE                            62469
#define SPELL_ICICLE                            62234
#define SPELL_ICICLE_SNOWDRIFT                  62462
#define SPELL_BLOCK_OF_ICE                      61990
#define SPELL_FROZEN_KILL                       62226
#define SPELL_ICICLE_FALL                       69428
#define SPELL_FALL_DAMAGE                       62236
#define SPELL_FALL_SNOWDRIFT                    62460
#define SPELL_BERSERK                           47008

// NPCs
#define NPC_FLASH_FREEZE                        32938
#define NPC_ICICLE_TARGET                       33174

enum Events
{
    EVENT_NONE,
    EVENT_FREEZE,
    EVENT_FLASH_CAST,
    EVENT_FLASH_EFFECT,
    EVENT_ICICLE,
    EVENT_BLOWS,
    EVENT_BERSERK
};

enum Yells
{
    SAY_AGGRO                                   = -1603210,
    SAY_SLAY_1                                  = -1603211,
    SAY_SLAY_2                                  = -1603212,
    SAY_FLASH_FREEZE                            = -1603213,
    SAY_STALACTITE                              = -1603214,
    SAY_DEATH                                   = -1603215,
    SAY_BERSERK                                 = -1603216,
    SAY_YS_HELP                                 = -1603217,
    SAY_HARD_MODE_MISSED                        = -1603218,
};

#define EMOTE_FREEZE      "Hodir begins to cast Flash Freeze!"
#define EMOTE_BLOWS       "Hodir gains Frozen Blows!"

struct mob_flash_freezeAI : public ScriptedAI
{
    mob_flash_freezeAI(Creature *c) : ScriptedAI(c)
    {
        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE | UNIT_FLAG_PACIFIED);
        me->SetReactState(REACT_PASSIVE);
        FrozenTargetGUID = 0;
    }

    uint64 FrozenTargetGUID;

    void SetPrisoner(Unit* uPrisoner)
    {
        FrozenTargetGUID = uPrisoner->GetGUID();
    }

    void Reset(){ FrozenTargetGUID = 0; }
};

CreatureAI* GetAI_mob_flash_freeze(Creature* pCreature)
{
    return new mob_flash_freezeAI(pCreature);
}

struct boss_hodir_AI : public BossAI
{
    boss_hodir_AI(Creature *pCreature) : BossAI(pCreature, BOSS_HODIR)
    {
        pInstance = pCreature->GetInstanceData();
    }
    
    ScriptedInstance* pInstance;
        
    void Reset()
    {
        _Reset();
    }

    void EnterCombat(Unit* who)
    {
        _EnterCombat();
        DoScriptText(SAY_AGGRO,me);
        DoCast(me, SPELL_BITING_COLD);
        events.ScheduleEvent(EVENT_ICICLE, 2000);
        events.ScheduleEvent(EVENT_FREEZE, 25000);
        events.ScheduleEvent(EVENT_BLOWS, urand(60000, 65000));
        events.ScheduleEvent(EVENT_FLASH_CAST, 50000);
        events.ScheduleEvent(EVENT_BERSERK, 480000);
    }
    void KilledUnit(Unit* victim)
    {
        DoScriptText(RAND(SAY_SLAY_1, SAY_SLAY_2), me);
    }

    void JustDied(Unit *victim)
    {
        _JustDied();
        DoScriptText(SAY_DEATH, me);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!UpdateVictim() || !CheckInRoom())
            return;

        events.Update(diff);

        if (me->hasUnitState(UNIT_STAT_CASTING))
            return;

        while(uint32 eventId = events.ExecuteEvent())
        {
            switch(eventId)
            {
                case EVENT_FREEZE:
                    DoCastAOE(SPELL_FREEZE);
                    events.ScheduleEvent(EVENT_FREEZE, urand(30000, 35000));
                    break;
                case EVENT_ICICLE:
                    if (Unit *pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0, 100, true))
                        if (pTarget->isAlive())
                            DoCast(pTarget, SPELL_ICICLE);
                    events.ScheduleEvent(EVENT_ICICLE, 2000);
                    break;
                case EVENT_FLASH_CAST:
                    DoScriptText(SAY_FLASH_FREEZE, me);
                    me->MonsterTextEmote(EMOTE_FREEZE, 0, true);
                    for (uint32 i = 0; i < 2; ++i)
                        if (Unit *pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0, 100, true))
                            if (pTarget->isAlive())
                                pTarget->CastSpell(pTarget, SPELL_ICICLE_SNOWDRIFT, true);
                    DoCast(SPELL_FLASH_FREEZE);
                    events.ScheduleEvent(EVENT_FLASH_CAST, 50000);
                    events.ScheduleEvent(EVENT_FLASH_EFFECT, 9000);
                    break;
                case EVENT_FLASH_EFFECT:
                    FlashFreeze();
                    //DoCast(62148);
                    events.CancelEvent(EVENT_FLASH_EFFECT);
                    break;
                case EVENT_BLOWS:
                    me->MonsterTextEmote(EMOTE_BLOWS, 0, true);
                    DoCast(me, SPELL_FROZEN_BLOWS);
                    events.ScheduleEvent(EVENT_BLOWS, urand(60000, 65000));
                    break;
                case EVENT_BERSERK:
                    DoCast(me, SPELL_BERSERK, true);
                    DoScriptText(SAY_BERSERK, me);
                    events.CancelEvent(EVENT_BERSERK);
                    break;
            }
        }

        DoMeleeAttackIfReady();
    }
    
    void FlashFreeze()
    {
        DoZoneInCombat();
        std::list<HostileReference*> ThreatList = me->getThreatManager().getThreatList();
        for (std::list<HostileReference*>::const_iterator itr = ThreatList.begin(); itr != ThreatList.end(); ++itr)
        {
            Unit *pTarget = Unit::GetUnit(*me, (*itr)->getUnitGuid());
            
            if (pTarget->GetTypeId() != TYPEID_PLAYER)
                continue;

            if (pTarget->HasAura(SPELL_BLOCK_OF_ICE))
            {
                DoCast(pTarget, SPELL_FROZEN_KILL);
                continue;
            }
            else
            {
                if (GetClosestCreatureWithEntry(pTarget, NPC_ICICLE_TARGET, 5.0f))
                    continue;
                    
                else if (Creature *pIceBlock = me->SummonCreature(NPC_FLASH_FREEZE, pTarget->GetPositionX(), pTarget->GetPositionY(), pTarget->GetPositionZ(), 0, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 90000))
                {
                        CAST_AI(mob_flash_freezeAI, pIceBlock->AI())->SetPrisoner(pTarget);
                        pIceBlock->CastSpell(pTarget, SPELL_BLOCK_OF_ICE, true);
                }
            }
        }
    }
};

CreatureAI* GetAI_boss_hodir(Creature* pCreature)
{
    return new boss_hodir_AI (pCreature);
}

struct mob_icicleAI : public ScriptedAI
{
    mob_icicleAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE | UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_PACIFIED);
        me->SetReactState(REACT_PASSIVE);
    }
    
    int32 IcicleTimer;
    
    void UpdateAI(const uint32 diff)
    {
        if (IcicleTimer <= diff)
        {
            DoCast(me, SPELL_FALL_DAMAGE);
            DoCast(me, SPELL_ICICLE_FALL);
            IcicleTimer = 10000;
        } else IcicleTimer -= diff;
    }
    
    void Reset()
    {
        IcicleTimer = 2000;
    }
};

CreatureAI* GetAI_mob_icicle(Creature* pCreature)
{
    return new mob_icicleAI(pCreature);
}

struct mob_icicle_snowdriftAI : public ScriptedAI
{
    mob_icicle_snowdriftAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE | UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_PACIFIED);
        me->SetReactState(REACT_PASSIVE);
    }
    
    int32 IcicleTimer;
    
    void UpdateAI(const uint32 diff)
    {
        if (IcicleTimer <= diff)
        {
            DoCast(me, SPELL_FALL_SNOWDRIFT);
            DoCast(me, SPELL_ICICLE_FALL);
            IcicleTimer = 10000;
        } else IcicleTimer -= diff;
    }
    
    void Reset()
    {
        IcicleTimer = 2000;
    }
};

CreatureAI* GetAI_mob_icicle_snowdrift(Creature* pCreature)
{
    return new mob_icicle_snowdriftAI(pCreature);
}


void AddSC_boss_hodir()
{
    Script *newscript;
    
    newscript = new Script;
    newscript->Name = "boss_hodir";
    newscript->GetAI = &GetAI_boss_hodir;
    newscript->RegisterSelf();
    
    newscript = new Script;
    newscript->Name = "mob_flash_freeze";
    newscript->GetAI = &GetAI_mob_flash_freeze;
    newscript->RegisterSelf();
    
    newscript = new Script;
    newscript->Name = "mob_icicle";
    newscript->GetAI = &GetAI_mob_icicle;
    newscript->RegisterSelf();
    
    newscript = new Script;
    newscript->Name = "mob_icicle_snowdrift";
    newscript->GetAI = &GetAI_mob_icicle_snowdrift;
    newscript->RegisterSelf();

}
