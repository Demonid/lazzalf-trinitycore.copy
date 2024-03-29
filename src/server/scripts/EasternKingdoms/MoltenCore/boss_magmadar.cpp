/*
 * Copyright (C) 2008-2010 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2006-2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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

/* ScriptData
SDName: Boss_Magmadar
SD%Complete: 75
SDComment: Conflag on ground nyi, fear causes issues without VMAPs
SDCategory: Molten Core
EndScriptData */

#include "ScriptPCH.h"
#include "molten_core.h"

#define EMOTE_FRENZY                -1409001

#define SPELL_FRENZY                19451
#define SPELL_MAGMASPIT             19449                   //This is actually a buff he gives himself
#define SPELL_PANIC                 19408
#define SPELL_LAVABOMB              19411                   //This calls a dummy server side effect that isn't implemented yet
#define SPELL_LAVABOMB_ALT          19428                   //This is the spell that the lava bomb casts

class boss_magmadar : public CreatureScript
{
public:
    boss_magmadar() : CreatureScript("boss_magmadar") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new boss_magmadarAI (pCreature);
    }

    struct boss_magmadarAI : public BossAI
    {
        boss_magmadarAI(Creature *pCreature) : BossAI(pCreature, BOSS_MAGMADAR)
        {
            m_pInstance = pCreature->GetInstanceScript(); 
        }

        InstanceScript* m_pInstance;

        uint32 Frenzy_Timer;
        uint32 Panic_Timer;
        uint32 Lavabomb_Timer;

        void Reset()
        {
            _Reset();
            Frenzy_Timer = 30000;
            Panic_Timer = 20000;
            Lavabomb_Timer = 12000;

            DoCast(me, SPELL_MAGMASPIT, true);
        }

        void JustDied(Unit* /*pKiller*/)
        {
            _JustDied();
            if (m_pInstance)
                m_pInstance->SetData(DATA_MAGMADAR, 0);
        }

        void EnterCombat(Unit * /*who*/)
        {
            _EnterCombat();
        }

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            //Frenzy_Timer
            if (Frenzy_Timer <= diff)
            {
                DoScriptText(EMOTE_FRENZY, me);
                DoCast(me, SPELL_FRENZY);
                Frenzy_Timer = 15000;
            } else Frenzy_Timer -= diff;

            //Panic_Timer
            if (Panic_Timer <= diff)
            {
                DoCast(me->getVictim(), SPELL_PANIC);
                Panic_Timer = 35000;
            } else Panic_Timer -= diff;

            //Lavabomb_Timer
            if (Lavabomb_Timer <= diff)
            {
                if (Unit *pTarget = SelectUnit(SELECT_TARGET_RANDOM,0))
                    DoCast(pTarget, SPELL_LAVABOMB_ALT);

                Lavabomb_Timer = 12000;
            } else Lavabomb_Timer -= diff;

            DoMeleeAttackIfReady();
        }
    };

};

void AddSC_boss_magmadar()
{
    new boss_magmadar();
}
