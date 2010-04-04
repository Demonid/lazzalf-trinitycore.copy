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
SDName: Ignis the Furnace Master
SDAuthor: PrinceCreed
SD%Complete: 100
EndScriptData */

#include "ScriptedPch.h"
#include "ulduar.h"

enum Yells
{
    SAY_AGGRO                                   = -1603220,
    SAY_SLAY_1                                  = -1603221,
    SAY_SLAY_2                                  = -1603222,
    SAY_DEATH                                   = -1603223,
    SAY_SUMMON                                  = -1603224,
    SAY_SLAG_POT                                = -1603225,
    SAY_SCORCH_1                                = -1603226,
    SAY_SCORCH_2                                = -1603227,
    SAY_BERSERK                                 = -1603228,
};

enum Spells
{
    SPELL_FLAME_JETS                            = 62680,
    H_SPELL_FLAME_JETS                          = 63472,
    SPELL_SCORCH                                = 62546,
    H_SPELL_SCORCH                              = 63474,
    SPELL_SLAG_POT                              = 62717,
    H_SPELL_SLAG_POT                            = 63477,
    SPELL_ACTIVATE_CONSTRUCT                    = 62488,
    SPELL_STRENGHT                              = 64473,
    SPELL_BERSERK                               = 47008
};

enum Events
{
    EVENT_NONE,
    EVENT_JET,
    EVENT_SCORCH,
    EVENT_POT,
    EVENT_CONSTRUCT,
    EVENT_END_POT,
    EVENT_BERSERK
};

#define EMOTE_JETS    "Ignis the Furnace Master begins to cast Flame Jets!"

// Mob and triggers
#define MOB_IRON_CONSTRUCT                        33121
#define GROUND_SCORCH                             33119

#define ACTION_REMOVE_BUFF                        20

enum ConstructSpells
{
    SPELL_HEAT                                  = 65667,
    SPELL_MOLTEN                                = 62373,
    SPELL_BRITTLE                               = 62382,
    SPELL_SHATTER                               = 62383
};

#define ACHIEVEMENT_STOKIN_THE_FURNACE        RAID_MODE(2930, 2929)
#define MAX_ENCOUNTER_TIME                    4 * 60 * 1000

// Water coords
#define WATER_1_X                                646.77
#define WATER_2_X                                526.77
#define WATER_Y                                  277.79
#define WATER_Z                                  359.88

const Position Pos[20] =
{
{630.366,216.772,360.891,3.001970},
{630.594,231.846,360.891,3.124140},
{630.435,337.246,360.886,3.211410},
{630.493,313.349,360.886,3.054330},
{630.444,321.406,360.886,3.124140},
{630.366,247.307,360.888,3.211410},
{630.698,305.311,360.886,3.001970},
{630.500,224.559,360.891,3.054330},
{630.668,239.840,360.890,3.159050},
{630.384,329.585,360.886,3.159050},
{543.220,313.451,360.886,0.104720},
{543.356,329.408,360.886,6.248280},
{543.076,247.458,360.888,6.213370},
{543.117,232.082,360.891,0.069813},
{543.161,305.956,360.886,0.157080},
{543.277,321.482,360.886,0.052360},
{543.316,337.468,360.886,6.195920},
{543.280,239.674,360.890,6.265730},
{543.265,217.147,360.891,0.174533},
{543.256,224.831,360.891,0.122173}
};

struct boss_ignis_AI : public BossAI
{
    boss_ignis_AI(Creature *pCreature) : BossAI(pCreature, BOSS_IGNIS), vehicle(me->GetVehicleKit())
    {
        // Do not let Ignis be affected by Scorch Ground haste buff
        me->ApplySpellImmune(0, IMMUNITY_ID, SPELL_HEAT, true);
        assert(vehicle);
    }

    std::vector<Creature*> triggers;
    Vehicle *vehicle;
    uint32 EncounterTime;

    void Reset()
    {
        _Reset();
    }

    void EnterCombat(Unit *who)
    {
        _EnterCombat();
        DoScriptText(SAY_AGGRO, me);
        events.ScheduleEvent(EVENT_JET, 30000);
        events.ScheduleEvent(EVENT_SCORCH, 25000);
        events.ScheduleEvent(EVENT_POT, 29000);
        events.ScheduleEvent(EVENT_CONSTRUCT, 15000);
        events.ScheduleEvent(EVENT_END_POT, 40000);
        events.ScheduleEvent(EVENT_BERSERK, 480000);
        EncounterTime = 0;

        for(uint32 i = 0; i < 20; ++i)
            if (Creature *trigger = DoSummon(WORLD_TRIGGER, Pos[i]))
                triggers.push_back(trigger);
        if (triggers.size() < 20)
        {
            EnterEvadeMode();
            return;
        }
    }

    void JustDied(Unit *victim)
    {
        _JustDied();
        DoScriptText(SAY_DEATH, me);

        if(EncounterTime <= MAX_ENCOUNTER_TIME)
        {
            AchievementEntry const *AchievStokinTheFurnace = GetAchievementStore()->LookupEntry(ACHIEVEMENT_STOKIN_THE_FURNACE);
            if(AchievStokinTheFurnace)
            {
                Map *pMap = m_creature->GetMap();
                if(pMap && pMap->IsDungeon())
                {
                    Map::PlayerList const &players = pMap->GetPlayers();
                    for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
                        itr->getSource()->CompletedAchievement(AchievStokinTheFurnace);
                }
            }
        }
    }

    void UpdateAI(const uint32 diff)
    {
        if (!UpdateVictim() || !CheckInRoom())
            return;

        if(me->GetPositionY() < 150 || me->GetPositionX() < 450 || me->getVictim()->GetTypeId() == !TYPEID_PLAYER)
        {
            me->RemoveAllAuras();
            me->DeleteThreatList();
            me->CombatStop(false);
            me->GetMotionMaster()->MoveTargetedHome();
        }

        events.Update(diff);

        if (me->hasUnitState(UNIT_STAT_CASTING))
            return;

        EncounterTime += diff;

        while(uint32 eventId = events.ExecuteEvent())
        {
            switch(eventId)
            {
                case EVENT_JET:
                    me->MonsterTextEmote(EMOTE_JETS, 0, true);
                    DoCastAOE(RAID_MODE(SPELL_FLAME_JETS, H_SPELL_FLAME_JETS));
                    events.ScheduleEvent(EVENT_JET, urand(35000,40000));
                    break;
                case EVENT_POT:
                    DoScriptText(SAY_SLAG_POT, me);
                    if (Unit *pTarget = SelectUnit(SELECT_TARGET_RANDOM, 1))
                       {
                           DoCast(pTarget, RAID_MODE(SPELL_SLAG_POT, H_SPELL_SLAG_POT));
                           pTarget->EnterVehicle(vehicle);
                       }
                    events.ScheduleEvent(EVENT_END_POT, 10000);
                    events.ScheduleEvent(EVENT_POT, 15000);
                    break;
                case EVENT_END_POT:
                    vehicle->RemoveAllPassengers();
                    events.ScheduleEvent(EVENT_END_POT, 30000);
                    break;
                case EVENT_SCORCH:
                    DoScriptText(RAND(SAY_SCORCH_1, SAY_SCORCH_2), me);
                    if (Unit *pTarget = me->getVictim())
                        me->SummonCreature(GROUND_SCORCH, pTarget->GetPositionX(), pTarget->GetPositionY(), pTarget->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN, 45000);
                    DoCast(RAID_MODE(SPELL_SCORCH, H_SPELL_SCORCH));
                    events.ScheduleEvent(EVENT_SCORCH, 25000);
                    break;
                case EVENT_CONSTRUCT:
                    DoScriptText(SAY_SUMMON, me);
                    DoSummon(MOB_IRON_CONSTRUCT, triggers[rand()%20]);
                    DoCast(SPELL_STRENGHT);
                    DoCast(me, SPELL_ACTIVATE_CONSTRUCT);
                    events.ScheduleEvent(EVENT_CONSTRUCT, RAID_MODE(40000, 30000));
                    break;
                case EVENT_BERSERK:
                    DoCast(me, SPELL_BERSERK, true);
                    DoScriptText(SAY_BERSERK, me);
                    break;
            }
        }

        DoMeleeAttackIfReady();
    }

    void KilledUnit(Unit* Victim)
    {
        if (!(rand()%5))
            DoScriptText(RAND(SAY_SLAY_1, SAY_SLAY_2), me);
    }

    void JustSummoned(Creature *summon)
    {
        if (summon->GetEntry() == WORLD_TRIGGER)
            summon->setActive(true);

        if (summon->GetEntry() == MOB_IRON_CONSTRUCT)
            summon->setFaction(16);
	
        summon->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
        summon->AI()->AttackStart(me->getVictim());
        summon->AI()->DoZoneInCombat();
        summons.Summon(summon);
    }

    void DoAction(const int32 action)
    {
        switch(action)
        {
            case ACTION_REMOVE_BUFF:
                me->RemoveAuraFromStack(SPELL_STRENGHT);
                break;
        }
    }
};

CreatureAI* GetAI_boss_ignis(Creature* pCreature)
{
    return new boss_ignis_AI (pCreature);
}

struct mob_iron_constructAI : public ScriptedAI
{
    mob_iron_constructAI(Creature *c) : ScriptedAI(c)
	{
        pInstance = c->GetInstanceData();
	}

    ScriptedInstance* pInstance;

    bool Brittled;

    void Reset()
    {
        Brittled = false;
    }

    void DamageTaken(Unit *attacker, uint32 &damage)
    {
        if (m_creature->HasAura(SPELL_BRITTLE) && damage >= 5000)
        {
            DoCast(SPELL_SHATTER);
            if (Creature *pIgnis = m_creature->GetCreature(*m_creature, pInstance->GetData64(DATA_IGNIS)))
                if (pIgnis->AI())
                    pIgnis->AI()->DoAction(ACTION_REMOVE_BUFF);
            m_creature->ForcedDespawn();
        }
    }

	void UpdateAI(const uint32 uiDiff)
    {
        Map *cMap = m_creature->GetMap();

        if (m_creature->HasAura(SPELL_MOLTEN) && m_creature->HasAura(SPELL_HEAT))
            m_creature->RemoveAura(SPELL_HEAT);

        if (Aura * aur = m_creature->GetAura((SPELL_HEAT), GetGUID()))
        {
            if (aur->GetStackAmount() == 20)
            {
                m_creature->RemoveAura(SPELL_HEAT);
                DoCast(SPELL_MOLTEN);
                Brittled = false;
            }
        }

        // Water pools
        if(cMap->GetId() == 603 && !Brittled && m_creature->HasAura(SPELL_MOLTEN) 
            && m_creature->GetDistance(WATER_1_X, WATER_Y, WATER_Z) <= 18 || m_creature->GetDistance(WATER_2_X, WATER_Y, WATER_Z) <= 18)
        {
            DoCast(SPELL_BRITTLE);
            m_creature->RemoveAura(SPELL_MOLTEN);
            Brittled = true;
        }

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_iron_construct(Creature* pCreature)
{
    return new mob_iron_constructAI (pCreature);
}

void AddSC_boss_ignis()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "boss_ignis";
    newscript->GetAI = &GetAI_boss_ignis;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_iron_construct";
    newscript->GetAI = &GetAI_mob_iron_construct;
    newscript->RegisterSelf();

}
