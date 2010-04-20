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
SDName: Kologarn
SDAuthor: PrinceCreed
SD%Complete: 85
SD%Comments: TODO: add Achievements, EyeBeam Visual and Stone Grip
EndScriptData */

#include "ScriptedPch.h"
#include "ulduar.h"
#include "Vehicle.h"

#define SPELL_ARM_DEAD_DAMAGE   RAID_MODE(63629,63979)
#define SPELL_TWO_ARM_SMASH     RAID_MODE(63356,64003)
#define SPELL_ONE_ARM_SMASH     RAID_MODE(63573,64006)
#define SPELL_STONE_SHOUT       RAID_MODE(63716,64005)
#define SPELL_PETRIFY_BREATH    RAID_MODE(62030,63980)
#define SPELL_SHOCKWAVE         RAID_MODE(63783,63982)

#define SPELL_STONE_GRIP        RAID_MODE(62166,63981)
#define SPELL_ARM_SWEEP         RAID_MODE(63766,63983)
#define SPELL_EYE_BEAM          RAID_MODE(63347,63977)
#define SPELL_ARM_RESPAWN       64753

enum Events
{
    EVENT_NONE = 0,
    EVENT_SMASH,
    EVENT_GRIP,
    EVENT_SWEEP,
    EVENT_SHOCKWAVE,
    EVENT_EYEBEAM,
    EVENT_STONESHOT,
    EVENT_RIGHT,
    EVENT_LEFT
};

enum Actions
{
    ACTION_RESPAWN_RIGHT      = 0,
    ACTION_RESPAWN_LEFT       = 1,
};

#define NPC_EYEBEAM_1           33632
#define NPC_EYEBEAM_2           33802
#define NPC_RUBBLE              33768

enum Yells
{
    SAY_AGGRO                                   = -1603230,
    SAY_SLAY_1                                  = -1603231,
    SAY_SLAY_2                                  = -1603232,
    SAY_LEFT_ARM_GONE                           = -1603233,
    SAY_RIGHT_ARM_GONE                          = -1603234,
    SAY_SHOCKWAVE                               = -1603235,
    SAY_GRAB_PLAYER                             = -1603236,
    SAY_DEATH                                   = -1603237,
    SAY_BERSERK                                 = -1603238,
};

#define EMOTE_RIGHT      "The Right Arm has regrown!"
#define EMOTE_LEFT       "The Left Arm has regrown!"
#define EMOTE_STONE      "Kologarn casts Stone Grip!"

// Positiones
const Position RubbleRight = {1781.814, -3.716, 448.808, 4.211};
const Position RubbleLeft  = {1781.814, -45.07, 448.808, 2.260};

struct boss_kologarnAI : public BossAI
{
    boss_kologarnAI(Creature *pCreature) : BossAI(pCreature, BOSS_KOLOGARN), vehicle(me->GetVehicleKit()),
        left(false), right(false)
    {
        assert(vehicle);
        me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_STUNNED);
    }

    ScriptedInstance* m_pInstance;

    Vehicle *vehicle;
    bool left, right;
    
    Creature* EyeBeam[2];
    Creature* RightArm;
    Creature* LeftArm;

    void AttackStart(Unit *who)
    {
        me->Attack(who, true);
    }

    void JustDied(Unit *victim)
    {
        DoScriptText(SAY_DEATH, me);
        _JustDied();
    }

    void PassengerBoarded(Unit *who, int8 seatId, bool apply)
    {
        if(who->GetTypeId() == TYPEID_UNIT)
        {
            if(who->GetEntry() == 32933)
                left = apply;
            else if(who->GetEntry() == 32934)
                right = apply;
            who->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_STUNNED);
            CAST_CRE(who)->SetReactState(REACT_PASSIVE);
        }
    }

    void EnterCombat(Unit *who)
    {
        DoScriptText(SAY_AGGRO, me);
        _EnterCombat();
        events.ScheduleEvent(EVENT_SMASH, 5000);
        events.ScheduleEvent(EVENT_SWEEP, 10000);
        events.ScheduleEvent(EVENT_EYEBEAM, 10000);
        events.ScheduleEvent(EVENT_SHOCKWAVE, 12000);
        //events.ScheduleEvent(EVENT_GRIP, 40000);
    }
    
    void KilledUnit(Unit* Victim)
    {
        if (!(rand()%5))
            DoScriptText(RAND(SAY_SLAY_1, SAY_SLAY_2), me);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!UpdateVictim())
            return;

        events.Update(diff);

        if (me->hasUnitState(UNIT_STAT_CASTING))
            return;
            
        if (events.GetTimer() > 15000 && !me->IsWithinMeleeRange(me->getVictim()))
            DoCastAOE(SPELL_PETRIFY_BREATH, true);
            
        if (!left && !right)
            DoCast(me, SPELL_STONE_SHOUT, true);

        switch(events.GetEvent())
        {
            case EVENT_NONE: break;
            case EVENT_SMASH:
                if (left && right)
                    DoCastVictim(SPELL_TWO_ARM_SMASH, true);
                else if(left || right)
                    DoCastVictim(SPELL_ONE_ARM_SMASH, true);
                events.RepeatEvent(15000);
                break;
            case EVENT_SWEEP:
                if (left)
                    DoCastAOE(SPELL_ARM_SWEEP, true);
                events.RepeatEvent(15000);
                break;
            case EVENT_GRIP: // TODO: Add Grip in Arm #65343
                if (right)
                {
                    if (Unit *pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0, 200, true))
                        DoCastVictim(SPELL_STONE_GRIP, true);
                    me->MonsterTextEmote(EMOTE_STONE, 0, true);
                    DoScriptText(SAY_GRAB_PLAYER, me);
                }
                events.RepeatEvent(15000);
                break;
            case EVENT_SHOCKWAVE:
                if (left)
                {
                    DoScriptText(SAY_SHOCKWAVE, me);
                    DoCastAOE(SPELL_SHOCKWAVE, true);
                }
                events.RepeatEvent(urand(15000, 25000));
                break;
            case EVENT_EYEBEAM: // TODO: Add Eye Beam spell visual
                if (Unit *pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0, 200, true))
                {
                    if (EyeBeam[0] = me->SummonCreature(NPC_EYEBEAM_1, pTarget->GetPositionX(), pTarget->GetPositionY() + 3, pTarget->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN, 10000))
                    EyeBeam[0]->GetMotionMaster()->MoveChase(pTarget, -0.5);
                    if (EyeBeam[1] = me->SummonCreature(NPC_EYEBEAM_2, pTarget->GetPositionX(), pTarget->GetPositionY() - 3, pTarget->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN, 10000))
                    EyeBeam[1]->GetMotionMaster()->MoveChase(pTarget, -0.5);
                }
                events.RepeatEvent(20000);
                break;
            case EVENT_RIGHT:
                if (RightArm = me->SummonCreature(32934, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), 0, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 0))
                {    
                    RightArm->EnterVehicle(vehicle, 1);
                    DoCast(me, SPELL_ARM_RESPAWN, true);
                    me->MonsterTextEmote(EMOTE_RIGHT, 0, true);
                }
                events.CancelEvent(EVENT_RIGHT);
                break;
            case EVENT_LEFT:
                if (LeftArm = me->SummonCreature(32933, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), 0, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 0))
                {    
                    LeftArm->EnterVehicle(vehicle, 0);
                    DoCast(me, SPELL_ARM_RESPAWN, true);
                    me->MonsterTextEmote(EMOTE_LEFT, 0, true);
                }
                events.CancelEvent(EVENT_LEFT);
                break;                
            default:
                events.PopEvent();
                break;
        }

        DoMeleeAttackIfReady();
    }
    
    void DoAction(const int32 action)
    {
        switch (action)
        {
            case ACTION_RESPAWN_RIGHT:
                DoScriptText(SAY_RIGHT_ARM_GONE, me);
                me->DealDamage(me, int32(me->GetMaxHealth() * 0.15)); // decreases Kologarn's health by 15%
                events.ScheduleEvent(EVENT_RIGHT, 30000);
                break;
            case ACTION_RESPAWN_LEFT:
                DoScriptText(SAY_LEFT_ARM_GONE, me);
                me->DealDamage(me, int32(me->GetMaxHealth() * 0.15));
                events.ScheduleEvent(EVENT_LEFT, 30000);
                break;
        }
    }
};

CreatureAI* GetAI_boss_kologarn(Creature* pCreature)
{
    return new boss_kologarnAI (pCreature);
}

struct mob_focused_eyebeamAI : public ScriptedAI
{
    mob_focused_eyebeamAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_PACIFIED);
    }

    void Reset()
    {
        DoCast(me, SPELL_EYE_BEAM);
    }
};

CreatureAI* GetAI_mob_focused_eyebeam(Creature* pCreature)
{
    return new mob_focused_eyebeamAI(pCreature);
}

// Right Arm
struct mob_right_armAI : public ScriptedAI
{
    mob_right_armAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = me->GetInstanceData();
    }

    ScriptedInstance* m_pInstance;

    void UpdateAI(const uint32 diff)
    {
        if (!UpdateVictim())
            return;

    }
    
    void JustDied(Unit *victim)
    {
        for (uint8 i = 0; i < 5; ++i)
            me->SummonCreature(NPC_RUBBLE, RubbleRight, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 3000);
            
        if (m_pInstance)
            if (Creature* pKologarn = me->GetCreature(*me, m_pInstance->GetData64(DATA_KOLOGARN)))
                if (pKologarn->AI())
                    pKologarn->AI()->DoAction(ACTION_RESPAWN_RIGHT);
    }
    
    void JustSummoned(Creature *summon)
    {
        summon->AI()->DoAction(0);
        summon->AI()->DoZoneInCombat();
    }
};

CreatureAI* GetAI_mob_right_arm(Creature* pCreature)
{
    return new mob_right_armAI(pCreature);
}

// Left Arm
struct mob_left_armAI : public ScriptedAI
{
    mob_left_armAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = me->GetInstanceData();
    }

    ScriptedInstance* m_pInstance;

    void UpdateAI(const uint32 diff)
    {
        if (!UpdateVictim())
            return;

    }
    
    void JustDied(Unit *victim)
    {
        for (uint8 i = 0; i < 5; ++i)
            me->SummonCreature(NPC_RUBBLE, RubbleLeft, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 3000);
            
        if (m_pInstance)
            if (Creature* pKologarn = me->GetCreature(*me, m_pInstance->GetData64(DATA_KOLOGARN)))
                if (pKologarn->AI())
                    pKologarn->AI()->DoAction(ACTION_RESPAWN_LEFT);
    }
    
    void JustSummoned(Creature *summon)
    {
        summon->AI()->DoAction(0);
        summon->AI()->DoZoneInCombat();
    }
};

CreatureAI* GetAI_mob_left_arm(Creature* pCreature)
{
    return new mob_left_armAI(pCreature);
}

void AddSC_boss_kologarn()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_kologarn";
    newscript->GetAI = &GetAI_boss_kologarn;
    newscript->RegisterSelf();
    
    newscript = new Script;
    newscript->Name = "mob_focused_eyebeam";
    newscript->GetAI = &GetAI_mob_focused_eyebeam;
    newscript->RegisterSelf();
    
    newscript = new Script;
    newscript->Name = "mob_right_arm";
    newscript->GetAI = &GetAI_mob_right_arm;
    newscript->RegisterSelf();
    
    newscript = new Script;
    newscript->Name = "mob_left_arm";
    newscript->GetAI = &GetAI_mob_left_arm;
    newscript->RegisterSelf();
    
}
