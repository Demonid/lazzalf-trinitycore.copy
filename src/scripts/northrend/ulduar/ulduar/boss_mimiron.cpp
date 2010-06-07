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

#include "ScriptedPch.h"
#include "ulduar.h"

// Completed: first phase with Leviathan Mk II all working

enum Yells
{
    SAY_AGGRO                                   = -1603240,
    SAY_HARDMODE_ON                             = -1603241,
    SAY_MKII_ACTIVATE                           = -1603242,
    SAY_MKII_SLAY_1                             = -1603243,
    SAY_MKII_SLAY_2                             = -1603244,
    SAY_MKII_DEATH                              = -1603245,
    SAY_VX001_ACTIVATE                          = -1603246,
    SAY_VX001_SLAY_1                            = -1603247,
    SAY_VX001_SLAY_2                            = -1603248,
    SAY_VX001_DEATH                             = -1603249,
    SAY_AERIAL_ACTIVATE                         = -1603250,
    SAY_AERIAL_SLAY_1                           = -1603251,
    SAY_AERIAL_SLAY_2                           = -1603252,
    SAY_AERIAL_DEATH                            = -1603253,
    SAY_V07TRON_ACTIVATE                        = -1603254,
    SAY_V07TRON_SLAY_1                          = -1603255,
    SAY_V07TRON_SLAY_2                          = -1603256,
    SAY_V07TRON_DEATH                           = -1603257,
    SAY_BERSERK                                 = -1603258,
    SAY_YS_HELP                                 = -1603259,
};

enum eSpells
{
    // Leviathan MK II
    SPELL_PROXIMITY_MINES = 65347,
    SPELL_NAPALM_SHELL_10 = 63666,
    SPELL_NAPALM_SHELL_25 = 65026,
    SPELL_PLASMA_BLAST_10 = 62997,
    SPELL_PLASMA_BLAST_25 = 64529,
    SPELL_SHOCK_BLAST = 63631,
    SPELL_EXPLOSION_10 = 66351,
    SPELL_EXPLOSION_25 = 63009,
};

enum eEvents
{
    // Leviathan MK II
    EVENT_NONE,
    EVENT_PROXIMITY_MINE,
    EVENT_NAPALM_SHELL,
    EVENT_PLASMA_BLAST,
    EVENT_SHOCK_BLAST,
};

enum Phases
{
    PHASE_NONE,
    PHASE_INTRO,
    PHASE_COMBAT,
};

enum eActions
{
    DO_START_ENCOUNTER = 1,
};

struct boss_mimironAI : public BossAI
{
    boss_mimironAI(Creature *pCreature) : BossAI(pCreature, BOSS_MIMIRON)
    {
        pInstance = pCreature->GetInstanceData();
        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        me->SetReactState(REACT_PASSIVE);
    }
    ScriptedInstance *pInstance;

    uint32 uiPhase_timer;
    uint32 uiStep;
    uint32 uiCheck;

    Phases phase;

    void Reset()
    {
        phase = PHASE_NONE;
        uiStep = 0;
        uiPhase_timer = -1;
        uiCheck = 500;
    }

    void JustDied(Unit *victim)
    {
    }

    void EnterCombat(Unit *who)
    {
    }

    void JumpToNextStep(uint32 uiTimer)
    {
        uiPhase_timer = uiTimer;
        ++uiStep;
    }

    void UpdateAI(const uint32 diff)
    {
        if (uiPhase_timer == -1)
        {
            if (uiCheck <= diff)
            {
                if (me->SelectNearbyTarget(10))
                {
                    phase = PHASE_INTRO;
                    JumpToNextStep(100);
                }
                uiCheck = 500;
            }
            else
                uiCheck -= diff;

            return;
        }

        if (phase == PHASE_INTRO)
        {
            if (uiPhase_timer <= diff)
            {
                switch (uiStep)
                {
                case 1:
                    DoScriptText(SAY_AGGRO, me);
                    JumpToNextStep(8000);
                    break;
                case 2:
                    DoScriptText(SAY_MKII_ACTIVATE, me);
                    JumpToNextStep(15000);
                    break;
                case 3:
                    if (pInstance)
                        if (Creature *pLeviathan = Creature::GetCreature((*me), pInstance->GetData64(DATA_LEVIATHAN_MK_II)))
                        {
                            me->EnterVehicle(pLeviathan->GetVehicleKit());
                            pLeviathan->AI()->DoAction(DO_START_ENCOUNTER);
                            phase = PHASE_COMBAT;
                        }
                default:
                    break;
                }
            }
            else
                uiPhase_timer -= diff;
        }
    }
};

CreatureAI* GetAI_boss_mimiron(Creature* pCreature)
{
    return new boss_mimironAI(pCreature);
}

struct boss_leviathan_mkAI : public BossAI
{
    boss_leviathan_mkAI(Creature *pCreature) : BossAI(pCreature, BOSS_MIMIRON), pVehicle(me->GetVehicleKit())
    {
        pInstance = pCreature->GetInstanceData();
    }
    ScriptedInstance *pInstance;
    Vehicle *pVehicle;

    EventMap events;

    void Reset()
    {
        _Reset();
        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        me->SetReactState(REACT_PASSIVE);
    }

    void JustReachedHome()
    {
        if (Creature *pMimiron = Creature::GetCreature((*me), pInstance->GetData64(DATA_MIMIRON)))
        {
            pMimiron->ExitVehicle();
            pMimiron->GetMotionMaster()->MoveKnockbackFrom(me->GetPositionX(), me->GetPositionY(), 10, 5);
            pMimiron->AI()->Reset();
        }
        DespawnCreatures(34362, 100);
    }

    void KilledUnit(Unit *who)
    {
        DoScriptText(RAND(SAY_MKII_SLAY_1, SAY_MKII_SLAY_2), me);
    }

    void DespawnCreatures(uint32 entry, float distance, bool discs = false)
    {
        std::list<Creature*> m_pCreatures;
        GetCreatureListWithEntryInGrid(m_pCreatures, me, entry, distance);
 
        if (m_pCreatures.empty())
            return;
 
        for(std::list<Creature*>::iterator iter = m_pCreatures.begin(); iter != m_pCreatures.end(); ++iter)
            (*iter)->ForcedDespawn();
    }

    void DamageTaken(Unit *who, uint32 &damage)
    {
        if (damage >= me->GetHealth())
        {
            damage = 0;
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
            me->AttackStop();
            me->SetReactState(REACT_PASSIVE);
            DoScriptText(SAY_MKII_DEATH, me);
            if (Creature *pMimiron = Creature::GetCreature((*me), pInstance->GetData64(DATA_MIMIRON)))
            {
                pMimiron->ExitVehicle();
                pMimiron->GetMotionMaster()->MoveKnockbackFrom(me->GetPositionX(), me->GetPositionY(), 10, 5);
                pMimiron->AI()->Reset();
            }
            DespawnCreatures(34362, 100);
        }
    }

    void EnterCombat(Unit *who)
    {
        events.Reset();
        events.ScheduleEvent(EVENT_PROXIMITY_MINE, 15000, 0);
        events.ScheduleEvent(EVENT_PLASMA_BLAST, 20000, 0);
    }

    void DoAction(const int32 action)
    {
        switch(action)
        {
        case DO_START_ENCOUNTER:
            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
            me->SetReactState(REACT_AGGRESSIVE);
            break;
        }
    }

    void UpdateAI(const uint32 diff)
    {
        if(!UpdateVictim())
            return;

        events.Update(diff);

        if (me->hasUnitState(UNIT_STAT_CASTING))
            return;

        while (uint32 eventId = events.ExecuteEvent())
        {
            switch(eventId)
            {
                case EVENT_PROXIMITY_MINE:
                    for (int i = 0; i < 10; i++)
                        DoCast(SPELL_PROXIMITY_MINES);
                    events.RescheduleEvent(EVENT_PROXIMITY_MINE, 30000);
                    return;
                case EVENT_PLASMA_BLAST:
                    DoCast(RAID_MODE(SPELL_PLASMA_BLAST_10, SPELL_PLASMA_BLAST_25));
                    events.RescheduleEvent(EVENT_PLASMA_BLAST, 45000);
                    events.ScheduleEvent(EVENT_SHOCK_BLAST, 5000);
                case EVENT_SHOCK_BLAST:
                    DoCastAOE(SPELL_SHOCK_BLAST);
                    return;
            }
        }

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_leviathan_mk(Creature* pCreature)
{
    return new boss_leviathan_mkAI(pCreature);
}

struct boss_leviathan_mk_turretAI : public ScriptedAI
{
    boss_leviathan_mk_turretAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        pInstance = pCreature->GetInstanceData();
    }
    ScriptedInstance *pInstance;

    uint32 uiNapalmShell;

    void Reset()
    {
        uiNapalmShell = 35000;
    }

    void UpdateAI(const uint32 diff)
    {
        if(!UpdateVictim())
            return;

        if (uiNapalmShell <= diff)
        {
            if (Unit *pTarget = SelectUnit(SELECT_TARGET_FARTHEST, 0))
                DoCast(pTarget, RAID_MODE(SPELL_NAPALM_SHELL_10, SPELL_NAPALM_SHELL_25));
            uiNapalmShell = 35000;
        }
        else
            uiNapalmShell -= diff;
    }
};

CreatureAI* GetAI_boss_leviathan_mk_turret(Creature* pCreature)
{
    return new boss_leviathan_mk_turretAI(pCreature);
}

struct mob_proximity_mineAI : public ScriptedAI
{
    mob_proximity_mineAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
       me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE | UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_PACIFIED);
       uiCheck = 500;
    }
    
    uint32 uiCheck;

    void UpdateAI(const uint32 diff)
    {
        if (uiCheck <= diff)
        {
            if (Unit *pTarget = me->SelectNearbyTarget(1))
            {
                DoCastAOE(RAID_MODE(SPELL_EXPLOSION_10, SPELL_EXPLOSION_25));
                me->ForcedDespawn(1000);
            }
            uiCheck = 500;
        }
        else
            uiCheck -= diff;
    }
};

CreatureAI* GetAI_mob_proximity_mine(Creature* pCreature)
{
    return new mob_proximity_mineAI(pCreature);
}

void AddSC_boss_mimiron()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "boss_mimiron";
    newscript->GetAI = &GetAI_boss_mimiron;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "boss_leviathan_mk";
    newscript->GetAI = &GetAI_boss_leviathan_mk;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "boss_leviathan_mk_turret";
    newscript->GetAI = &GetAI_boss_leviathan_mk_turret;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_proximity_mine";
    newscript->GetAI = &GetAI_mob_proximity_mine;
    newscript->RegisterSelf();
}
