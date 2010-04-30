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
SDName: Auriaya
Author: PrinceCreed
SD%Complete: 95%
SDComment: TODO: Savage Pounce not works properly
EndScriptData */

#include "ScriptedPch.h"
#include "ulduar.h"

// Auriaya
#define SPELL_SETINEL_BLAST                     RAID_MODE(64389, 64678)
#define SPELL_SONIC_SCREECH                     RAID_MODE(64422, 64688)
#define SPELL_TERRIFYING_SCREECH                64386
#define SPELL_SUMMON_SWARMING_GUARDIAN          64396
#define SPELL_ACTIVATE_DEFENDER                 64449
#define SPELL_DEFENDER_TRIGGER                  64448
#define SPELL_SUMMON_DEFENDER                   64447
#define SPELL_BERSERK                           47008

// Feral Defender
#define SPELL_FERAL_RUSH                        RAID_MODE(64496, 64674)
#define SPELL_FERAL_POUNCE                      RAID_MODE(64478, 64669)
#define SPELL_SEEPING_ESSENCE                   RAID_MODE(64458, 64676)
#define SPELL_SUMMON_ESSENCE                    64457
#define SPELL_FERAL_ESSENCE                     64455
#define SPELL_DEATH_EFFECT                      64165

// Sanctum Sentry
#define SPELL_SAVAGE_POUNCE                     RAID_MODE(64666, 64374)
#define SPELL_RIP_FLESH                         RAID_MODE(64375, 64667)
#define SPELL_STRENGHT_PACK                     64381
#define SANCTUM_SENTRY                          34014

// Achievements
#define ACHIEVEMENT_CRAZY_CAT_LADY              RAID_MODE(3006, 3007)
#define ACHIEVEMENT_NINE_LIVES                  RAID_MODE(3076, 3077)

enum Events
{
    EVENT_NONE,
    EVENT_SCREECH,
    EVENT_BLAST,
    EVENT_TERRIFYING,
    EVENT_SUMMON,
    EVENT_DEFENDER,
    EVENT_BERSERK
};

enum Yells
{
    SAY_AGGRO                                   = -1603050,
    SAY_SLAY_1                                  = -1603051,
    SAY_SLAY_2                                  = -1603052,
    SAY_DEATH                                   = -1603053,
    SAY_BERSERK                                 = -1603054,
};

#define EMOTE_FEAR        "Auriaya begins to cast Terrifying Screech."
#define EMOTE_DEFENDER    "Auriaya begins to activate the Feral Defender!"

enum Actions
{
    ACTION_CRAZY_CAT_LADY                       = 0,
    ACTION_NINE_LIVES                           = 1,
};

struct boss_auriaya_AI : public BossAI
{
    boss_auriaya_AI(Creature *pCreature) : BossAI(pCreature, BOSS_AURIAYA)
    {
        pInstance = pCreature->GetInstanceData();
    }
    
    ScriptedInstance* pInstance;
    
    Creature* Sentry[4];
    
    bool SentryDead;
    uint32 LivesCount;
    
    void Reset()
    {
        _Reset();
        
        Sentry[0] = me->SummonCreature(SANCTUM_SENTRY, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), 0, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 3000);
        Sentry[0]->GetMotionMaster()->MoveFollow(me, 3, 1.5, MOTION_SLOT_ACTIVE);
        Sentry[1] = me->SummonCreature(SANCTUM_SENTRY, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), 0, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 3000);
        Sentry[1]->GetMotionMaster()->MoveFollow(me, 3, -1.5, MOTION_SLOT_ACTIVE);
        if (IsHeroic())
        {
            Sentry[2] = me->SummonCreature(SANCTUM_SENTRY, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), 0, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 3000);
            Sentry[2]->GetMotionMaster()->MoveFollow(me, 1, 1.5, MOTION_SLOT_ACTIVE);
            Sentry[3] = me->SummonCreature(SANCTUM_SENTRY, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), 0, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 3000);
            Sentry[3]->GetMotionMaster()->MoveFollow(me, 1, -1.5, MOTION_SLOT_ACTIVE);
        }
    }

    void EnterCombat(Unit* who)
    {
        _EnterCombat();
        DoScriptText(SAY_AGGRO,me);
        SentryDead = false;
        LivesCount = 0;
        events.ScheduleEvent(EVENT_SCREECH, 60000);
        events.ScheduleEvent(EVENT_BLAST, 50000);
        events.ScheduleEvent(EVENT_TERRIFYING, 40000);
        events.ScheduleEvent(EVENT_DEFENDER, 65000);
        events.ScheduleEvent(EVENT_SUMMON, 120000);
        events.ScheduleEvent(EVENT_BERSERK, 600000);
    }
    void KilledUnit(Unit* victim)
    {
        DoScriptText(RAND(SAY_SLAY_1, SAY_SLAY_2), me);
    }

    void JustDied(Unit *victim)
    {
        _JustDied();
        DoScriptText(SAY_DEATH, me);
        
        // Achievements
        if (pInstance)
        {
            // Crazy Cat Lady
            if (!SentryDead)
                pInstance->DoCompleteAchievement(ACHIEVEMENT_CRAZY_CAT_LADY);
            // Nine Lives
            if (LivesCount >= 9)
                pInstance->DoCompleteAchievement(ACHIEVEMENT_NINE_LIVES);
        }
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
                case EVENT_SCREECH:
                    DoCast(SPELL_SONIC_SCREECH);
                    events.ScheduleEvent(EVENT_SCREECH, urand(25000, 30000));
                    break;
                case EVENT_TERRIFYING:
                    me->MonsterTextEmote(EMOTE_FEAR, 0, true);
                    DoCast(SPELL_TERRIFYING_SCREECH);
                    events.ScheduleEvent(EVENT_TERRIFYING, 40000);
                    break;
                case EVENT_BLAST:
                    DoCastAOE(SPELL_SETINEL_BLAST);
                    events.ScheduleEvent(EVENT_BLAST, 40000);
                    break;
                case EVENT_DEFENDER:
                    me->MonsterTextEmote(EMOTE_DEFENDER, 0, true);
                    DoCast(SPELL_DEFENDER_TRIGGER);
                    DoCast(SPELL_ACTIVATE_DEFENDER);
                    events.CancelEvent(EVENT_DEFENDER);
                    break;
                case EVENT_SUMMON:
                    if (Unit *pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0, 200, true))
                        DoCast(pTarget, SPELL_SUMMON_SWARMING_GUARDIAN);
                    events.ScheduleEvent(EVENT_SUMMON, 35000);
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
    
    void DoAction(const int32 action)
    {
        switch (action)
        {
            case ACTION_CRAZY_CAT_LADY:
                SentryDead = true;
                break;
            case ACTION_NINE_LIVES:
                LivesCount++;
                break;
        }
    }
};

CreatureAI* GetAI_boss_auriaya(Creature* pCreature)
{
    return new boss_auriaya_AI (pCreature);
}

struct feral_defender_triggerAI : public ScriptedAI
{
    feral_defender_triggerAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = pCreature->GetInstanceData();
        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE | UNIT_FLAG_PACIFIED);
    }

    ScriptedInstance* m_pInstance;
    int32 SummomTimer;

    void Reset()
    {
        SummomTimer = 3000;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!UpdateVictim())
            return;

        if (SummomTimer <= uiDiff)
        {
            DoCast(me, SPELL_SUMMON_DEFENDER);
            SummomTimer = 30000;
        } 
        else SummomTimer -= uiDiff;
    }

    void JustSummoned(Creature *summon)
    {
        summon->AI()->DoAction(0);
        summon->AI()->DoZoneInCombat();
    }
};

CreatureAI* GetAI_feral_defender_trigger(Creature* pCreature)
{
    return new feral_defender_triggerAI(pCreature);
}

struct mob_sanctum_sentryAI : public ScriptedAI
{
    mob_sanctum_sentryAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        pInstance = pCreature->GetInstanceData();
    }

    ScriptedInstance* pInstance;
    int32 RipTimer;
    int32 PounceTimer;

    void Reset()
    {
        RipTimer = urand(4000, 8000);
        PounceTimer = urand(20000, 30000);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!UpdateVictim())
            return;

        if (RipTimer <= uiDiff)
        {
            DoCastVictim(SPELL_RIP_FLESH);
            RipTimer = urand(14000, 18000);
        } else RipTimer -= uiDiff;
        
        if (PounceTimer <= uiDiff)
        {
            if (Unit *pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0, 200, true))
            {
                me->AddThreat(pTarget, 100.0f);
                me->AI()->AttackStart(pTarget);
                DoCast(pTarget, SPELL_SAVAGE_POUNCE);
            }
            PounceTimer = urand(20000, 30000);
        } else PounceTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
    
    void JustDied(Unit* victim)
    {
 	    if (Creature *pAuriaya = me->GetCreature(*me, pInstance->GetData64(DATA_AURIAYA)))
            if (pAuriaya->AI())
                pAuriaya->AI()->DoAction(ACTION_CRAZY_CAT_LADY);
    }
};

CreatureAI* GetAI_mob_sanctum_sentry(Creature* pCreature)
{
    return new mob_sanctum_sentryAI(pCreature);
}

struct mob_feral_defenderAI : public ScriptedAI
{
    mob_feral_defenderAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        pInstance = pCreature->GetInstanceData();
    }
    
    ScriptedInstance* pInstance;
    int32 PounceTimer;
    int32 RushTimer;
    int32 RessTimer;
    int32 Lifes;
    bool Dead;

    void Reset()
    {
        PounceTimer = 5000;
        RushTimer = 12000;
        Lifes = 8;
        Dead = false;
        RessTimer = 999999;
        
        for (uint8 i = 0; i < Lifes; ++i)
            DoCast(me, SPELL_FERAL_ESSENCE);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!UpdateVictim())
            me->ForcedDespawn();

        if (PounceTimer <= uiDiff)
        {
            if (Unit *pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0, 200, true))
            {
                me->AddThreat(pTarget, 100.0f);
                me->AI()->AttackStart(pTarget);
                DoCast(pTarget, SPELL_FERAL_POUNCE);
            }
            PounceTimer = urand(15000, 20000);
        } 
        else PounceTimer -= uiDiff;
        
        if (RushTimer <= uiDiff)
        {
            if (Unit *pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0, 200, true))
            {
                me->AddThreat(pTarget, 100.0f);
                me->AI()->AttackStart(pTarget);
                DoCast(pTarget, SPELL_FERAL_RUSH);
            }
            RushTimer = urand(15000, 20000);
        } 
        else RushTimer -= uiDiff;
        
        if (RessTimer <= uiDiff)
        {
            me->RemoveAurasDueToSpell(SPELL_DEATH_EFFECT);
            me->SetHealth(me->GetMaxHealth());
            for (uint8 i = 0; i < Lifes; ++i)
                DoCast(me, SPELL_FERAL_ESSENCE);
 	        me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE | UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_PACIFIED);
 	        me->SetReactState(REACT_AGGRESSIVE);
 	        Dead = false;
            RessTimer = 999999;
            DoZoneInCombat();
        } else RessTimer -= uiDiff;
        
        DoMeleeAttackIfReady();
    }
    
    void DamageTaken(Unit* pKiller, uint32 &damage)
 	{
 	    if(damage >= me->GetHealth()) // Feign Death
 	    {
 	        if (!Dead)
 	        {
 	            DoCast(me, SPELL_SUMMON_ESSENCE);
 	            me->SetHealth(0);
 	            me->RemoveAllAuras();
 	            me->AddAura(SPELL_DEATH_EFFECT, me);
 	            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE | UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_PACIFIED);
 	            me->SetReactState(REACT_PASSIVE);
 	            me->AttackStop();
 	            Lifes--;
 	            Dead = true;
 	            PounceTimer = 35000;
                RushTimer = 42000;
 	            RessTimer = 30000;
 	            if (Creature *pAuriaya = me->GetCreature(*me, pInstance->GetData64(DATA_AURIAYA)))
                    if (pAuriaya->AI())
                        pAuriaya->AI()->DoAction(ACTION_NINE_LIVES);
            }
            else damage = 0;
 	    }
 	}
};

CreatureAI* GetAI_mob_feral_defender(Creature* pCreature)
{
    return new mob_feral_defenderAI(pCreature);
}

struct seeping_triggerAI : public ScriptedAI
{
    seeping_triggerAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE | UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_PACIFIED);
    }
    
    void Reset()
    {
        me->ForcedDespawn(600000);
        DoCast(me, SPELL_SEEPING_ESSENCE);
    }
};

CreatureAI* GetAI_seeping_trigger(Creature* pCreature)
{
    return new seeping_triggerAI(pCreature);
}

void AddSC_boss_auriaya()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "boss_auriaya";
    newscript->GetAI = &GetAI_boss_auriaya;
    newscript->RegisterSelf();
    
    newscript = new Script;
    newscript->Name = "feral_defender_trigger";
    newscript->GetAI = &GetAI_feral_defender_trigger;
    newscript->RegisterSelf();
    
    newscript = new Script;
    newscript->Name = "mob_sanctum_sentry";
    newscript->GetAI = &GetAI_mob_sanctum_sentry;
    newscript->RegisterSelf();
    
    newscript = new Script;
    newscript->Name = "mob_feral_defender";
    newscript->GetAI = &GetAI_mob_feral_defender;
    newscript->RegisterSelf();
    
    newscript = new Script;
    newscript->Name = "seeping_trigger";
    newscript->GetAI = &GetAI_seeping_trigger;
    newscript->RegisterSelf();
}
