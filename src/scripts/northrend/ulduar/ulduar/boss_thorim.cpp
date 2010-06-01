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
SDName: Thorim
SDAuthor: PrinceCreed
SD%Complete: 85
SDComments: Lightning Charge not works, TODO: Achievements and Hard Mode
EndScriptData */

#include "ScriptedPch.h"
#include "ulduar.h"

// Thorim Spells
enum Spells
{
    SPELL_SHEAT_OF_LIGHTNING                    = 62276,
    SPELL_STORMHAMMER                           = 62042,
    SPELL_DEAFENING_THUNDER                     = 62470,
    SPELL_CHARGE_ORB                            = 62016,
    SPELL_SUMMON_LIGHTNING_ORB                  = 62391,
    SPELL_TOUCH_OF_DOMINION                     = 62565,
    SPELL_CHAIN_LIGHTNING_10                    = 62131,
    SPELL_CHAIN_LIGHTNING_25                    = 64390,
    SPELL_LIGHTNING_CHARGE                      = 62279,
    SPELL_LIGHTNING_RELEASE                     = 62466,
    SPELL_UNBALANCING_STRIKE                    = 62130,
    SPELL_BERSERK                               = 62560
};

enum Phases
{
    PHASE_NULL = 0,
    PHASE_1,
    PHASE_2
};

enum Events
{
    EVENT_NONE,
    EVENT_STORMHAMMER,
    EVENT_CHARGE_ORB,
    EVENT_SUMMON_ADDS,
    EVENT_BERSERK,
    EVENT_UNBALANCING_STRIKE,
    EVENT_CHAIN_LIGHTNING,
    EVENT_TRANSFER_ENERGY,
    EVENT_RELEASE_ENERGY
};

enum Yells
{
    SAY_AGGRO_1                                 = -1603270,
    SAY_AGGRO_2                                 = -1603271,
    SAY_SPECIAL_1                               = -1603272,
    SAY_SPECIAL_2                               = -1603273,
    SAY_SPECIAL_3                               = -1603274,
    SAY_JUMPDOWN                                = -1603275,
    SAY_SLAY_1                                  = -1603276,
    SAY_SLAY_2                                  = -1603277,
    SAY_BERSERK                                 = -1603278,
    SAY_WIPE                                    = -1603279,
    SAY_DEATH                                   = -1603280,
    SAY_END_NORMAL_1                            = -1603281,
    SAY_END_NORMAL_2                            = -1603282,
    SAY_END_NORMAL_3                            = -1603283,
    SAY_END_HARD_1                              = -1603284,
    SAY_END_HARD_2                              = -1603285,
    SAY_END_HARD_3                              = -1603286,
    SAY_YS_HELP                                 = -1603287
};

#define EMOTE_BARRIER      "Runic Colossus surrounds itself with a crackling Runic Barrier!"
#define EMOTE_MIGHT        "Ancient Rune Giant fortifies nearby allies with runic might!"

// Thorim Pre-Phase Adds
enum PreAdds
{
    BEHEMOTH,
    MERCENARY_CAPTAIN,
    MERCENARY_SOLDIER,
    DARK_RUNE_ACOLYTE
};

const uint32 PRE_PHASE_ADD[]          = {32882, 32908, 32885, 33110};
#define SPELL_PRE_PRIMARY(i)            RAID_MODE(SPELL_PRE_PRIMARY_N[i],SPELL_PRE_PRIMARY_H[i])
const uint32 SPELL_PRE_PRIMARY_N[]    = {62315, 62317, 62318, 62333};
const uint32 SPELL_PRE_PRIMARY_H[]    = {62415, 62317, 62318, 62441};
#define SPELL_PRE_SECONDARY(i)          RAID_MODE(SPELL_PRE_SECONDARY_N[i],SPELL_PRE_SECONDARY_H[i])
const uint32 SPELL_PRE_SECONDARY_N[]  = {62316, 62444, 16496, 62334};
const uint32 SPELL_PRE_SECONDARY_H[]  = {62417, 62444, 16496, 62442};
#define SPELL_HOLY_SMITE                RAID_MODE(62335, 62443)

#define INCREASE_PREADDS_COUNT                     1
#define ACTION_CHANGE_PHASE                        2

// Thorim Arena Phase Adds
enum ArenaAdds
{
    DARK_RUNE_CHAMPION,
    DARK_RUNE_COMMONER,
    DARK_RUNE_EVOKER,
    DARK_RUNE_WARBRINGER,
    IRON_RING_GUARD,
    IRON_HONOR_GUARD
};

const uint32 ARENA_PHASE_ADD[]          = {32876, 32904, 32878, 32877, 32874, 32875};
#define SPELL_ARENA_PRIMARY(i)            RAID_MODE(SPELL_ARENA_PRIMARY_N[i],SPELL_ARENA_PRIMARY_H[i])
const uint32 SPELL_ARENA_PRIMARY_N[]    = {35054, 62326, 62327, 62322, 64151, 42724};
const uint32 SPELL_ARENA_PRIMARY_H[]    = {35054, 62326, 62445, 62322, 64151, 42724};
#define SPELL_ARENA_SECONDARY(i)          RAID_MODE(SPELL_ARENA_SECONDARY_N[i],SPELL_ARENA_SECONDARY_H[i])
const uint32 SPELL_ARENA_SECONDARY_N[]  = {15578, 38313, 62321, 0, 62331, 62332};
const uint32 SPELL_ARENA_SECONDARY_H[]  = {15578, 38313, 62529, 0, 62418, 62420};
#define SPELL_AURA_OF_CELERITY            62320
#define SPELL_CHARGE                      32323
#define SPELL_RUNIC_MENDING               RAID_MODE(62328, 62446)

// Runic Colossus (Mini Boss) Spells
enum RunicSpells
{
    SPELL_SMASH                                 = 62339,
    SPELL_RUNIC_BARRIER                         = 62338,
    SPELL_CHARGE_10                             = 62613,
    SPELL_CHARGE_25                             = 62614
};

// Ancient Rune Giant (Mini Boss) Spells
enum AncientSpells
{
    SPELL_RUNIC_FORTIFICATION                   = 62942,
    SPELL_RUNE_DETONATION                       = 62526,
    SPELL_STOMP_10                              = 62411,
    SPELL_STOMP_25                              = 62413
};

const Position Pos[7] =
{
{2095.53, -279.48, 419.84, 0.504},
{2092.93, -252.96, 419.84, 6.024},
{2098.66, -240.79, 419.84, 5.715},
{2113.37, -225.85, 419.84, 5.221},
{2156.54, -225.99, 419.84, 4.441},
{2171.80, -241.20, 419.84, 3.760},
{2171.73, -285.07, 419.84, 2.761}
};

const Position PosCharge[7] =
{
{2104.99, -233.484, 419.573, 5.49779},
{2092.64, -262.594, 419.573, 6.26573},
{2104.76, -292.719, 419.573, 0.78539},
{2164.97, -293.375, 419.573, 2.35619},
{2164.58, -233.333, 419.573, 3.90954},
{2145.81, -222.196, 419.573, 4.45059},
{2123.91, -222.443, 419.573, 4.97419}
};


struct boss_thorimAI : public BossAI
{
    boss_thorimAI(Creature* pCreature) : BossAI(pCreature, BOSS_THORIM)
        , phase(PHASE_NULL)
    {
        pInstance = pCreature->GetInstanceData();
        FirstTime = true;
    }
    
    ScriptedInstance* pInstance;
    Phases phase;
    int32 PreAddsCount;
    uint8 spawnedAdds;
    uint32 EncounterTime;
    bool FirstTime;
    Creature *EnergySource;

    void Reset()
    {
        if (!FirstTime)
            DoScriptText(SAY_WIPE, me);
            
        _Reset();
        
        me->SetReactState(REACT_PASSIVE);
        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE | UNIT_FLAG_NON_ATTACKABLE);
        FirstTime = true;
        PreAddsCount = 0;
        spawnedAdds = 0;
        
        // Respawn Mini Bosses
        if (Creature* pRunicColossus = Unit::GetCreature(*me, pInstance->GetData64(DATA_RUNIC_COLOSSUS)))
            pRunicColossus->Respawn(true);
        if (Creature* pRuneGiant = Unit::GetCreature(*me, pInstance->GetData64(DATA_RUNE_GIANT)))
            pRuneGiant->Respawn(true);
        
        // Spawn Thorim Event Trigger
        me->SummonCreature(33725, 2134.93, -339.696, 437.311, 0, TEMPSUMMON_MANUAL_DESPAWN);
                
        // Spawn Pre-Phase Adds
        me->SummonCreature(PRE_PHASE_ADD[0], 2134.79, -263.03, 419.84, 5.377, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 3000);
        me->SummonCreature(PRE_PHASE_ADD[1], 2141.60, -271.64, 419.84, 2.188, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 3000);
        me->SummonCreature(PRE_PHASE_ADD[2], 2127.24, -251.31, 419.84, 5.89, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 3000);
        me->SummonCreature(PRE_PHASE_ADD[2], 2123.32, -254.77, 419.84, 6.17, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 3000);
        me->SummonCreature(PRE_PHASE_ADD[2], 2120.10, -258.99, 419.84, 6.25, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 3000);
        me->SummonCreature(PRE_PHASE_ADD[3], 2130.28, -274.60, 419.84, 1.22, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 3000);
    }

    void KilledUnit(Unit * victim)
    {
        DoScriptText(RAND(SAY_SLAY_1,SAY_SLAY_2), me);
    }

    void JustDied(Unit * victim)
    {
        DoScriptText(SAY_DEATH, me);
        _JustDied();
    }

    void EnterCombat(Unit* pWho)
    {
        DoScriptText(RAND(SAY_AGGRO_1,SAY_AGGRO_2), me);
        _EnterCombat();
        
        FirstTime = false;
        EncounterTime = 0;
        phase = PHASE_1;
        events.SetPhase(PHASE_1);
        DoCast(me, SPELL_SHEAT_OF_LIGHTNING);
        events.ScheduleEvent(EVENT_STORMHAMMER, 40000, 0, PHASE_1);
        events.ScheduleEvent(EVENT_CHARGE_ORB, 30000, 0, PHASE_1);
        events.ScheduleEvent(EVENT_SUMMON_ADDS, 20000, 0, PHASE_1);
        events.ScheduleEvent(EVENT_BERSERK, 300000, 0, PHASE_1);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!UpdateVictim())
            return;
            
        events.Update(diff);
        
        EncounterTime += diff;

        if (me->hasUnitState(UNIT_STAT_CASTING))
            return;
            
        if (phase == PHASE_1)
        {
            while (uint32 eventId = events.ExecuteEvent())
            {
                switch(eventId)
                {
                    case EVENT_STORMHAMMER:
                        if (Unit *pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0, 100, true))
                            if (pTarget->isAlive())
                                DoCast(pTarget, SPELL_STORMHAMMER);
                        events.ScheduleEvent(EVENT_STORMHAMMER, urand(15000, 20000), 0, PHASE_1);
                        break;
                    case EVENT_CHARGE_ORB:
                        DoCastAOE(SPELL_CHARGE_ORB);
                        events.ScheduleEvent(EVENT_CHARGE_ORB, urand(15000, 20000), 0, PHASE_1);
                        break;
                    case EVENT_SUMMON_ADDS:
                        spawnAdd();
                        events.ScheduleEvent(EVENT_SUMMON_ADDS, 2500, 0, PHASE_1);
                        break;
                    case EVENT_BERSERK:
                        DoCast(me, SPELL_BERSERK);
                        DoScriptText(SAY_BERSERK, me);
                        events.CancelEvent(EVENT_BERSERK);
                        break;
                }
            }
        }
        else
        {
            while (uint32 eventId = events.ExecuteEvent())
            {
                switch(eventId)
                {
                    case EVENT_UNBALANCING_STRIKE:
                        DoCastVictim(SPELL_UNBALANCING_STRIKE);
                        events.ScheduleEvent(EVENT_UNBALANCING_STRIKE, 25000, 0, PHASE_2);
                        break;
                    case EVENT_CHAIN_LIGHTNING:
                        if (Unit *pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0, 100, true))
                            if (pTarget->isAlive())
                                DoCast(pTarget, RAID_MODE(SPELL_CHAIN_LIGHTNING_10, SPELL_CHAIN_LIGHTNING_25));
                        events.ScheduleEvent(EVENT_CHAIN_LIGHTNING, urand(15000, 20000), 0, PHASE_2);
                        break;
                    /*case EVENT_TRANSFER_ENERGY:
                        //DoScriptText(SAY_SUMMON, me);
                        me->SummonCreature(32892, PosCharge[rand()%7], TEMPSUMMON_CORPSE_TIMED_DESPAWN, 3000);
                        events.ScheduleEvent(EVENT_TRANSFER_ENERGY, 20000, 0, PHASE_2);
                        break;*/
                    case EVENT_RELEASE_ENERGY:
                        DoCast(me, SPELL_LIGHTNING_CHARGE);
                        //DoCast(EnergySource, SPELL_LIGHTNING_RELEASE);
                        events.ScheduleEvent(EVENT_RELEASE_ENERGY, 20000, 0, PHASE_2);
                        break;
                }
            }
        }
                    
        DoMeleeAttackIfReady();
    }
    
    void DoAction(const int32 action)
    {
        switch (action)
        {
            case INCREASE_PREADDS_COUNT:
                ++PreAddsCount;
                break;
            case ACTION_CHANGE_PHASE:
                DoScriptText(SAY_JUMPDOWN, me);
                phase = PHASE_2;
                events.SetPhase(PHASE_2);
                me->RemoveAurasDueToSpell(SPELL_SHEAT_OF_LIGHTNING);
                if (EncounterTime >= 180000)
                    me->AddAura(SPELL_TOUCH_OF_DOMINION, me);
                me->SetReactState(REACT_AGGRESSIVE);
                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
                me->GetMotionMaster()->MoveJump(2134.79, -263.03, 419.84, 10.0f, 20.0f);
                events.ScheduleEvent(EVENT_UNBALANCING_STRIKE, 15000, 0, PHASE_2);
                events.ScheduleEvent(EVENT_CHAIN_LIGHTNING, 20000, 0, PHASE_2);
                //events.ScheduleEvent(EVENT_TRANSFER_ENERGY, 20000, 0, PHASE_2);
                events.ScheduleEvent(EVENT_RELEASE_ENERGY, 25000, 0, PHASE_2);
                events.ScheduleEvent(EVENT_BERSERK, 300000, 0, PHASE_2);
                break;
        }
        
        if (PreAddsCount >= 6 && FirstTime)
        {
            // Event starts
            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            DoZoneInCombat();
        }
    }
    
    void spawnAdd()
    {
        switch(spawnedAdds)
        {
            case 0:
                me->SummonCreature(ARENA_PHASE_ADD[0], Pos[rand()%7], TEMPSUMMON_CORPSE_TIMED_DESPAWN, 3000);
                break;
            case 1:
                me->SummonCreature(ARENA_PHASE_ADD[2], Pos[rand()%7], TEMPSUMMON_CORPSE_TIMED_DESPAWN, 3000);
                break;
            case 2:
                me->SummonCreature(ARENA_PHASE_ADD[3], Pos[rand()%7], TEMPSUMMON_CORPSE_TIMED_DESPAWN, 3000);
                break;
            case 3:
                for(uint8 n = 0; n < 10; n++)
                    me->SummonCreature(ARENA_PHASE_ADD[1], Pos[n], TEMPSUMMON_CORPSE_TIMED_DESPAWN, 3000);
                break;
        }

        spawnedAdds++;
        if(spawnedAdds > 3)
        {
            spawnedAdds = 0;
        }
    }
};

CreatureAI* GetAI_boss_thorim(Creature* pCreature)
{
    return new boss_thorimAI(pCreature);
}

// Pre-Phase Adds
struct mob_pre_phaseAI : public ScriptedAI
{
    mob_pre_phaseAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        pInstance = pCreature->GetInstanceData();
        id = PreAdds(0);
        for (uint8 i = 0; i < 4; ++i)
            if (me->GetEntry() == PRE_PHASE_ADD[i])
                id = PreAdds(i);
    }

    PreAdds id;
    ScriptedInstance* pInstance;
    int32 PrimaryTimer;
    int32 SecondaryTimer;

    void Reset()
    {
        PrimaryTimer = urand(3000, 6000);
        SecondaryTimer = urand (12000, 15000);
    }
    
    void UpdateAI(const uint32 uiDiff)
    {
        if (!UpdateVictim())
            return;
            
        if (me->hasUnitState(UNIT_STAT_CASTING))
            return;
            
        if (PrimaryTimer <= uiDiff)
        {
            DoCast(SPELL_PRE_PRIMARY(id));
            PrimaryTimer = urand(15000, 20000);
        }
        else PrimaryTimer -= uiDiff;
        
        if (SecondaryTimer <= uiDiff)
        {
            DoCast(SPELL_PRE_SECONDARY(id));
            SecondaryTimer = urand(4000, 8000);
        }
        else SecondaryTimer -= uiDiff;

        if (id == DARK_RUNE_ACOLYTE)
            DoSpellAttackIfReady(SPELL_HOLY_SMITE);
        else
            DoMeleeAttackIfReady();
    }
    
    void JustDied(Unit *victim)
    {
        if (Creature* pThorim = Unit::GetCreature(*me, pInstance->GetData64(DATA_THORIM)))
            if (pThorim->AI())
                pThorim->AI()->DoAction(INCREASE_PREADDS_COUNT);
    }
};

CreatureAI* GetAI_mob_pre_phase(Creature* pCreature)
{
    return new mob_pre_phaseAI(pCreature);
}

// Arena Phase Adds
struct mob_arena_phaseAI : public ScriptedAI
{
    mob_arena_phaseAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        pInstance = pCreature->GetInstanceData();        
        id = ArenaAdds(0);
        for (uint8 i = 0; i < 6; ++i)
            if (me->GetEntry() == ARENA_PHASE_ADD[i])
                id = ArenaAdds(i);
    }

    ArenaAdds id;
    ScriptedInstance* pInstance;
    int32 PrimaryTimer;
    int32 SecondaryTimer;
    int32 ChargeTimer;

    void Reset()
    {
        PrimaryTimer = urand(3000, 6000);
        SecondaryTimer = urand (7000, 9000);
        ChargeTimer = 8000;
    }
    
    void EnterCombat(Unit* pWho)
    {
        if (id == DARK_RUNE_WARBRINGER)
            DoCast(me, SPELL_AURA_OF_CELERITY);
            
        DoZoneInCombat();
    }
    
    void UpdateAI(const uint32 uiDiff)
    {
        if (!UpdateVictim())
            return;
            
        if (me->hasUnitState(UNIT_STAT_CASTING))
            return;
            
        if (PrimaryTimer <= uiDiff)
        {
            DoCast(SPELL_ARENA_PRIMARY(id));
            PrimaryTimer = urand(3000, 6000);
        }
        else PrimaryTimer -= uiDiff;
        
        if (SecondaryTimer <= uiDiff)
        {
            DoCast(SPELL_ARENA_SECONDARY(id));
            SecondaryTimer = urand(12000, 16000);
        }
        else SecondaryTimer -= uiDiff;
        
        if (ChargeTimer <= uiDiff)
        {
            if (id == DARK_RUNE_CHAMPION)
                if (Unit *pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0, 40, true))
                    DoCast(pTarget, SPELL_CHARGE);
            ChargeTimer = 12000;
        }
        else ChargeTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_arena_phase(Creature* pCreature)
{
    return new mob_arena_phaseAI(pCreature);
}

// Runic Colossus (Mini Boss)
struct mob_runic_colossusAI : public ScriptedAI
{
    mob_runic_colossusAI(Creature* pCreature) : ScriptedAI(pCreature), summons(me)
    {
        pInstance = pCreature->GetInstanceData();
    }

    ScriptedInstance* pInstance;
    SummonList summons;
        
    int32 BarrierTimer;
    int32 SmashTimer;
    int32 ChargeTimer;

    void Reset()
    {
        BarrierTimer = urand(12000, 15000);
        SmashTimer = urand (15000, 18000);
        ChargeTimer = urand (20000, 24000);
        
        me->GetMotionMaster()->MoveTargetedHome();
        // Close Runed Door
        if (pInstance)
            pInstance->SetData(DATA_RUNIC_DOOR, GO_STATE_READY);
            
        // Spawn trashes
        summons.DespawnAll();
        me->SummonCreature(32874, 2218.38, -297.50, 412.18, 1.030, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 3000);
        me->SummonCreature(32874, 2235.07, -297.98, 412.18, 1.613, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 3000);
        me->SummonCreature(32874, 2235.26, -338.34, 412.18, 1.589, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 3000);
        me->SummonCreature(32874, 2217.69, -337.39, 412.18, 1.241, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 3000);
        me->SummonCreature(33110, 2227.58, -308.30, 412.18, 1.591, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 3000);
        me->SummonCreature(33110, 2227.47, -345.37, 412.18, 1.566, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 3000);
    }
    
    void UpdateAI(const uint32 uiDiff)
    {
        // I cannot find the real spell
        if (!me->IsWithinMeleeRange(me->getVictim()))
            DoCast(me, SPELL_SMASH);
    
        if (!UpdateVictim())
            return;
            
        if (me->hasUnitState(UNIT_STAT_CASTING))
            return;
            
        if (BarrierTimer <= uiDiff)
        {
            me->MonsterTextEmote(EMOTE_MIGHT, 0, true);
            DoCast(me, SPELL_RUNIC_BARRIER);
            BarrierTimer = urand(35000, 45000);
        }
        else BarrierTimer -= uiDiff;
        
        if (SmashTimer <= uiDiff)
        {
            DoCast(me, SPELL_SMASH);
            SmashTimer = urand (15000, 18000);
        }
        else SmashTimer -= uiDiff;
        
        if (ChargeTimer <= uiDiff)
        {
            if (Unit *pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0, 30, true))
                DoCast(pTarget, RAID_MODE(SPELL_CHARGE_10, SPELL_CHARGE_25));
            ChargeTimer = 20000;
        }
        else ChargeTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
    
    void JustDied(Unit *victim)
    {
        // Open Runed Door
        if (pInstance)
            pInstance->SetData(DATA_RUNIC_DOOR, GO_STATE_ACTIVE);
    }
    
    void JustSummoned(Creature *summon)
    {
        summons.Summon(summon);
    }
};

CreatureAI* GetAI_mob_runic_colossus(Creature* pCreature)
{
    return new mob_runic_colossusAI(pCreature);
}

// Ancient Rune Giant (Mini Boss)
struct mob_rune_giantAI : public ScriptedAI
{
    mob_rune_giantAI(Creature* pCreature) : ScriptedAI(pCreature), summons(me)
    {
        pInstance = pCreature->GetInstanceData();
    }

    ScriptedInstance* pInstance;
    SummonList summons;
        
    int32 StompTimer;
    int32 DetonationTimer;

    void Reset()
    {
        StompTimer = urand(10000, 12000);
        DetonationTimer = 25000;
        
        me->GetMotionMaster()->MoveTargetedHome();
        // Close Stone Door
        if (pInstance)
            pInstance->SetData(DATA_STONE_DOOR, GO_STATE_READY);
            
        // Spawn trashes
        summons.DespawnAll();
        me->SummonCreature(32875, 2198.05, -428.77, 419.95, 6.056, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 3000);
        me->SummonCreature(32875, 2220.31, -436.22, 412.26, 1.064, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 3000);
        me->SummonCreature(32875, 2158.88, -441.73, 438.25, 0.127, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 3000);
        me->SummonCreature(33110, 2198.29, -436.92, 419.95, 0.261, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 3000);
    }
    
    void EnterCombat(Unit* pWho)
    {
        me->MonsterTextEmote(EMOTE_MIGHT, 0, true);
        DoCast(me, SPELL_RUNIC_FORTIFICATION); // need core support
    }
    
    void UpdateAI(const uint32 uiDiff)
    {    
        if (!UpdateVictim())
            return;
            
        if (me->hasUnitState(UNIT_STAT_CASTING))
            return;
            
        if (StompTimer <= uiDiff)
        {
            DoCast(me, RAID_MODE(SPELL_STOMP_10, SPELL_STOMP_25));
            StompTimer = urand(10000, 12000);
        }
        else StompTimer -= uiDiff;
        
        if (DetonationTimer <= uiDiff)
        {
            if (Unit *pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0, 40, true))
                DoCast(pTarget, SPELL_RUNE_DETONATION);
            DetonationTimer = urand(10000, 12000);
        }
        else DetonationTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
    
    void JustDied(Unit *victim)
    {
        // Open Stone Door
        if (pInstance)
            pInstance->SetData(DATA_STONE_DOOR, GO_STATE_ACTIVE);
    }
    
    void JustSummoned(Creature *summon)
    {
        summons.Summon(summon);
    }
};

CreatureAI* GetAI_mob_rune_giant(Creature* pCreature)
{
    return new mob_rune_giantAI(pCreature);
}

// Thorim Event Bunny
struct thorim_trap_bunnyAI : public Scripted_NoMovementAI
{
    thorim_trap_bunnyAI(Creature* pCreature) : Scripted_NoMovementAI(pCreature)
    {
        pInstance = pCreature->GetInstanceData();
    }

    ScriptedInstance* pInstance;
    
    void MoveInLineOfSight(Unit *who)
    {
        if (pInstance && pInstance->GetBossState(BOSS_THORIM) == IN_PROGRESS)
            if (me->IsWithinDistInMap(who, 20.0f) && who->GetTypeId() == TYPEID_PLAYER)
            {
                if (Creature* pThorim = Unit::GetCreature(*me, pInstance->GetData64(DATA_THORIM)))
                    if (pThorim->AI())
                        pThorim->AI()->DoAction(ACTION_CHANGE_PHASE);
                me->ForcedDespawn();
            }
        ScriptedAI::MoveInLineOfSight(who);
    }
};

CreatureAI* GetAI_thorim_trap_bunny(Creature* pCreature)
{
    return new thorim_trap_bunnyAI(pCreature);
}

// Thorim Energy Source
struct thorim_energy_sourceAI : public Scripted_NoMovementAI
{
    thorim_energy_sourceAI(Creature* pCreature) : Scripted_NoMovementAI(pCreature)
    {
        pInstance = pCreature->GetInstanceData();
        pThorim = Unit::GetCreature(*me, pInstance->GetData64(DATA_THORIM));
    }

    ScriptedInstance* pInstance;
    Creature* pThorim;
    int32 TransferTimer;

    void Reset()
    {
        TransferTimer = 0;
        me->ForcedDespawn(5000);
        me->SetReactState(REACT_PASSIVE);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!UpdateVictim())
            return;
            
        if (pThorim && pThorim->isAlive())
            DoCast(pThorim, 2400, true);
            me->StopMoving();
    }
};

CreatureAI* GetAI_thorim_energy_source(Creature* pCreature)
{
    return new thorim_energy_sourceAI(pCreature);
}

void AddSC_boss_thorim()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_thorim";
    newscript->GetAI = &GetAI_boss_thorim;
    newscript->RegisterSelf();
    
    newscript = new Script;
    newscript->Name = "mob_pre_phase";
    newscript->GetAI = &GetAI_mob_pre_phase;
    newscript->RegisterSelf();
    
    newscript = new Script;
    newscript->Name = "mob_arena_phase";
    newscript->GetAI = &GetAI_mob_arena_phase;
    newscript->RegisterSelf();
    
    newscript = new Script;
    newscript->Name = "mob_runic_colossus";
    newscript->GetAI = &GetAI_mob_runic_colossus;
    newscript->RegisterSelf();
    
    newscript = new Script;
    newscript->Name = "mob_rune_giant";
    newscript->GetAI = &GetAI_mob_rune_giant;
    newscript->RegisterSelf();
    
    newscript = new Script;
    newscript->Name = "thorim_trap_bunny";
    newscript->GetAI = &GetAI_thorim_trap_bunny;
    newscript->RegisterSelf();
    
    newscript = new Script;
    newscript->Name = "thorim_energy_source";
    newscript->GetAI = &GetAI_thorim_energy_source;
    newscript->RegisterSelf();

}
