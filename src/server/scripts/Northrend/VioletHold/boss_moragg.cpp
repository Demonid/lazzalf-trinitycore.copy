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
#include "violet_hold.h"

//Spells
enum Spells
{
    SPELL_CORROSIVE_SALIVA                     = 54527,
    SPELL_OPTIC_LINK                           = 54396,
    SPELL_RAY_OF_PAIN                          = 59525
};

class boss_moragg : public CreatureScript
{
public:
    boss_moragg() : CreatureScript("boss_moragg") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new boss_moraggAI (pCreature);
    }

    struct boss_moraggAI : public ScriptedAI
    {
        boss_moraggAI(Creature *c) : ScriptedAI(c)
        {
            pInstance = c->GetInstanceScript();
        }

        uint32 uiOpticLinkTimer;
        uint32 uiCorrosiveSalivaTimer;
    uint32 uiRayOfPainTimer;

        InstanceScript* pInstance;

        void Reset()
        {
            uiOpticLinkTimer = 10000;
            uiCorrosiveSalivaTimer = 5000;
        uiRayOfPainTimer = 8000; // Not Offy-Like

            if (pInstance)
            {
                if (pInstance->GetData(DATA_WAVE_COUNT) == 6)
                    pInstance->SetData(DATA_1ST_BOSS_EVENT, NOT_STARTED);
                else if (pInstance->GetData(DATA_WAVE_COUNT) == 12)
                    pInstance->SetData(DATA_2ND_BOSS_EVENT, NOT_STARTED);
            }
        }

        void EnterCombat(Unit* /*who*/)
        {
            if (pInstance)
            {
                if (GameObject *pDoor = pInstance->instance->GetGameObject(pInstance->GetData64(DATA_MORAGG_CELL)))
                    if (pDoor->GetGoState() == GO_STATE_READY)
                   {
                        EnterEvadeMode();
                        return;
                    }
                if (pInstance->GetData(DATA_WAVE_COUNT) == 6)
                    pInstance->SetData(DATA_1ST_BOSS_EVENT, IN_PROGRESS);
                else if (pInstance->GetData(DATA_WAVE_COUNT) == 12)
                    pInstance->SetData(DATA_2ND_BOSS_EVENT, IN_PROGRESS);
            }
        }

        void AttackStart(Unit* pWho)
        {
            if (me->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_OOC_NOT_ATTACKABLE) || me->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE))
                return;

            if (me->Attack(pWho, true))
            {
                me->AddThreat(pWho, 0.0f);
                me->SetInCombatWith(pWho);
                pWho->SetInCombatWith(me);
                DoStartMovement(pWho);
            }
        }

        void MoveInLineOfSight(Unit* /*who*/) {}

        void UpdateAI(const uint32 diff)
        {
            //Return since we have no target
            if (!UpdateVictim())
                return;

            if (uiOpticLinkTimer <= diff)
            {
            // Hack per Optical Link (Hack from Loken script)
            Map* pMap = me->GetMap();
            if (pMap->IsDungeon())
            {
                Map::PlayerList const &PlayerList = pMap->GetPlayers();

                if (PlayerList.isEmpty())
                    return;

                for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
                    if (i->getSource() && i->getSource()->isAlive() && i->getSource()->isTargetableForAttack())
                    {
                        int32 dmg;
                        float m_fDist = me->GetExactDist(i->getSource()->GetPositionX(), i->getSource()->GetPositionY(), i->getSource()->GetPositionZ());

                        dmg = 150; // need to correct damage
                        if (m_fDist > 1.0f) // Further from 1 yard
                            dmg *= m_fDist;
                        
                        Unit* pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0, 100, true);
                        me->CastCustomSpell(pTarget, SPELL_OPTIC_LINK, &dmg, 0, 0, false);
                    }
            }
            uiOpticLinkTimer = 25000;
            } else uiOpticLinkTimer -= diff;

            if (uiCorrosiveSalivaTimer <= diff)
            {
                DoCast(me->getVictim(), SPELL_CORROSIVE_SALIVA);
                uiCorrosiveSalivaTimer = 10000;
            } else uiCorrosiveSalivaTimer -= diff;

        if (uiRayOfPainTimer <= diff)
        { 
            DoCast(me->getVictim(), SPELL_RAY_OF_PAIN);
            uiRayOfPainTimer = 12000;
        } else uiRayOfPainTimer -= diff;

            DoMeleeAttackIfReady();
        }
        void JustDied(Unit* /*killer*/)
        {
            if (pInstance)
            {
                if (pInstance->GetData(DATA_WAVE_COUNT) == 6)
                {
                    pInstance->SetData(DATA_1ST_BOSS_EVENT, DONE);
                    pInstance->SetData(DATA_WAVE_COUNT, 7);
                }
                else if (pInstance->GetData(DATA_WAVE_COUNT) == 12)
                {
                    pInstance->SetData(DATA_2ND_BOSS_EVENT, DONE);
                    pInstance->SetData(DATA_WAVE_COUNT,13);
                }
            }
        }
    };

};


void AddSC_boss_moragg()
{
    new boss_moragg();
}
