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
#include "gundrak.h"

enum Spells
{
    SPELL_ECK_BERSERK                             = 55816, //Eck goes berserk, increasing his attack speed by 150% and all damage he deals by 500%.
    SPELL_ECK_BITE                                = 55813, //Eck bites down hard, inflicting 150% of his normal damage to an enemy.
    SPELL_ECK_SPIT                                = 55814, //Eck spits toxic bile at enemies in a cone in front of him, inflicting 2970 Nature damage and draining 220 mana every 1 sec for 3 sec.
    SPELL_ECK_SPRING_1                            = 55815, //Eck leaps at a distant target.  --> Drops aggro and charges a random player. Tank can simply taunt him back.
    SPELL_ECK_SPRING_2                            = 55837  //Eck leaps at a distant target.
};

static Position EckSpawnPoint = { 1643.877930f, 936.278015f, 107.204948f, 0.668432f };

class boss_eck : public CreatureScript
{
public:
    boss_eck() : CreatureScript("boss_eck") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new boss_eckAI (pCreature);
    }

    struct boss_eckAI : public ScriptedAI
    {
        boss_eckAI(Creature *c) : ScriptedAI(c)
        {
            pInstance = c->GetInstanceScript();
        }

        uint32 uiBerserkTimer;
        uint32 uiBiteTimer;
        uint32 uiSpitTimer;
        uint32 uiSpringTimer;

        bool bBerserk;

        InstanceScript* pInstance;

        void Reset()
        {
            uiBerserkTimer = urand(60*IN_MILLISECONDS,90*IN_MILLISECONDS); //60-90 secs according to wowwiki
            uiBiteTimer = 5*IN_MILLISECONDS;
            uiSpitTimer = 10*IN_MILLISECONDS;
            uiSpringTimer = 8*IN_MILLISECONDS;

            bBerserk = false;

            if (pInstance)
                pInstance->SetData(DATA_ECK_THE_FEROCIOUS_EVENT, NOT_STARTED);
        }

        void EnterCombat(Unit* /*who*/)
        {
            if (pInstance)
                pInstance->SetData(DATA_ECK_THE_FEROCIOUS_EVENT, IN_PROGRESS);
        }

        void DeleteFromThreatList(uint64 TargetGUID)
        {
            for (std::list<HostileReference*>::const_iterator itr = me->getThreatManager().getThreatList().begin(); itr != me->getThreatManager().getThreatList().end(); ++itr)
            {
                if ((*itr)->getUnitGuid() == TargetGUID)
                {
                    (*itr)->removeReference();
                    break;
                }
            }
        }

        void SpellHitTarget(Unit* pTarget, const SpellEntry *spell)
        {
            if (spell->Id == SPELL_ECK_SPIT)
            {
                if (pTarget->GetTypeId() == TYPEID_PLAYER && !pTarget->HasAura(SPELL_ECK_RESIDUE))
                    pTarget->CastSpell(pTarget, SPELL_ECK_RESIDUE, true);
            }
        }

        void UpdateAI(const uint32 diff)
        {
            //Return since we have no target
            if (!UpdateVictim())
                return;

            if (uiBiteTimer <= diff)
            {
                DoCast(me->getVictim(), SPELL_ECK_BITE);
                uiBiteTimer = urand(8*IN_MILLISECONDS,12*IN_MILLISECONDS);
            } else uiBiteTimer -= diff;

            if (uiSpitTimer <= diff)
            {
                DoCast(me->getVictim(), SPELL_ECK_SPIT);
                uiSpitTimer = urand(6*IN_MILLISECONDS,14*IN_MILLISECONDS);
            } else uiSpitTimer -= diff;

            if (uiSpringTimer <= diff)
            {
                Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM,1);
                if (pTarget && pTarget->GetTypeId() == TYPEID_PLAYER)
                {
                    DoCast(pTarget, RAND(SPELL_ECK_SPRING_1, SPELL_ECK_SPRING_2));
                    CAST_AI(boss_eckAI, me->AI())->DeleteFromThreatList(me->GetGUID());
                    uiSpringTimer = urand(5*IN_MILLISECONDS,10*IN_MILLISECONDS);
                }
            } else uiSpringTimer -= diff;

            //Berserk on timer or 20% of health
            if (!bBerserk)
            {
                if (uiBerserkTimer <= diff)
                {
                    DoCast(me, SPELL_ECK_BERSERK);
                    bBerserk = true;
                }
                else
                {
                    uiBerserkTimer -= diff;
                    if (HealthBelowPct(20))
                    {
                        DoCast(me, SPELL_ECK_BERSERK);
                        bBerserk = true;
                    }
                }
            }

            DoMeleeAttackIfReady();
        }

        void JustDied(Unit* /*killer*/)
        {
            if (pInstance)
                pInstance->SetData(DATA_ECK_THE_FEROCIOUS_EVENT, DONE);
        }
    };

};


class npc_ruins_dweller : public CreatureScript
{
public:
    npc_ruins_dweller() : CreatureScript("npc_ruins_dweller") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_ruins_dwellerAI (pCreature);
    }

    struct npc_ruins_dwellerAI : public ScriptedAI
    {
        npc_ruins_dwellerAI(Creature *c) : ScriptedAI(c)
        {
            pInstance = c->GetInstanceScript();
        }

        InstanceScript* pInstance;

        void JustDied(Unit * /*who*/)
        {
            if (pInstance)
            {
                pInstance->SetData64(DATA_RUIN_DWELLER_DIED,me->GetGUID());
                if (pInstance->GetData(DATA_ALIVE_RUIN_DWELLERS) == 0)
                    me->SummonCreature(CREATURE_ECK, EckSpawnPoint, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 300*IN_MILLISECONDS);
            }
        }
    };

};


void AddSC_boss_eck()
{
    new boss_eck();
    new npc_ruins_dweller();
}
