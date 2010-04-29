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

/*
#define SAY_AGGRO -1
#define SAY_SLAY -1
*/

struct boss_hodirAI : public BossAI
{
    boss_hodirAI(Creature *pCreature) : BossAI(pCreature, BOSS_HODIR){}

    void Reset()
    {
    }

    void KilledUnit(Unit * /*victim*/)
    {
    }

    void JustDied(Unit * /*victim*/)
    {
        _JustDied();
    }

    void EnterCombat(Unit* pWho)
    {
        _EnterCombat();
//        DoScriptText(SAY_AGGRO, me);
        me->SetInCombatWithZone();
    }

    void UpdateAI(const uint32 diff)
    {
        if (!UpdateVictim())
            return;
//SPELLS TODO:

//
        DoMeleeAttackIfReady();

        EnterEvadeIfOutOfCombatArea(diff);

    }

};

CreatureAI* GetAI_boss_hodir(Creature* pCreature)
{
    return new boss_hodirAI(pCreature);
}

void AddSC_boss_hodir()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_hodir";
    newscript->GetAI = &GetAI_boss_hodir;
    newscript->RegisterSelf();

}

