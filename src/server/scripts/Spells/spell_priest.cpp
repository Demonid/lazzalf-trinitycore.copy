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

/*
 * Scripts for spells with SPELLFAMILY_PRIEST and SPELLFAMILY_GENERIC spells used by priest players.
 * Ordered alphabetically using scriptname.
 * Scriptnames of files in this file should be prefixed with "spell_pri_".
 */

#include "ScriptPCH.h"
#include "SpellAuraEffects.h"

enum PriestSpells
{
    PRIEST_SPELL_PENANCE_R1                      = 47540,
    PRIEST_SPELL_PENANCE_R1_DAMAGE               = 47758,
    PRIEST_SPELL_PENANCE_R1_HEAL                 = 47757,
    PRIEST_SPELL_PENANCE_R2                      = 53005,
    PRIEST_SPELL_PENANCE_R2_DAMAGE               = 53001,
    PRIEST_SPELL_PENANCE_R2_HEAL                 = 52986,
    PRIEST_SPELL_PENANCE_R3                      = 53006,
    PRIEST_SPELL_PENANCE_R3_DAMAGE               = 53002,
    PRIEST_SPELL_PENANCE_R3_HEAL                 = 52987,
    PRIEST_SPELL_PENANCE_R4                      = 53007,
    PRIEST_SPELL_PENANCE_R4_DAMAGE               = 53003,
    PRIEST_SPELL_PENANCE_R4_HEAL                 = 52988,
};

class spell_pri_mana_burn : public SpellScriptLoader
{
    public:
        spell_pri_mana_burn() : SpellScriptLoader("spell_pri_mana_burn") { }

        class spell_pri_mana_burn_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_pri_mana_burn_SpellScript)
            bool Validate(SpellEntry const * /*spellEntry*/)
            {
                return true;
            }

            void HandleAfterHit()
            {
                Unit * unitTarget = GetHitUnit();
                if (!unitTarget)
                    return;

                unitTarget->RemoveAurasWithMechanic((1 << MECHANIC_FEAR) | (1 << MECHANIC_POLYMORPH));
            }

            void Register()
            {
                AfterHit += SpellHitFn(spell_pri_mana_burn_SpellScript::HandleAfterHit);
            }
        };

        SpellScript * GetSpellScript() const
        {
            return new spell_pri_mana_burn_SpellScript;
        }
};

class spell_pri_pain_and_suffering_proc : public SpellScriptLoader
{
    public:
        spell_pri_pain_and_suffering_proc() : SpellScriptLoader("spell_pri_pain_and_suffering_proc") { }

        // 47948 Pain and Suffering (proc)
        class spell_pri_pain_and_suffering_proc_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_pri_pain_and_suffering_proc_SpellScript)
            void HandleEffectScriptEffect(SpellEffIndex /*effIndex*/)
            {
                // Refresh Shadow Word: Pain on target
                if (Unit *unitTarget = GetHitUnit())
                    if (AuraEffect* aur = unitTarget->GetAuraEffect(SPELL_AURA_PERIODIC_DAMAGE, SPELLFAMILY_PRIEST, 0x8000, 0, 0, GetCaster()->GetGUID()))
                        aur->GetBase()->RefreshDuration();
            }

            void Register()
            {
                OnEffect += SpellEffectFn(spell_pri_pain_and_suffering_proc_SpellScript::HandleEffectScriptEffect, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript *GetSpellScript() const
        {
            return new spell_pri_pain_and_suffering_proc_SpellScript;
        }
};

class spell_pri_penance : public SpellScriptLoader
{
    public:
        spell_pri_penance() : SpellScriptLoader("spell_pri_penance") { }

        class spell_pri_penance_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_pri_penance_SpellScript)
            bool Validate(SpellEntry const * spellEntry)
            {
                if (!sSpellStore.LookupEntry(PRIEST_SPELL_PENANCE_R1))
                    return false;
                if (!sSpellStore.LookupEntry(PRIEST_SPELL_PENANCE_R1_DAMAGE))
                    return false;
                if (!sSpellStore.LookupEntry(PRIEST_SPELL_PENANCE_R1_HEAL))
                    return false;

                if (!sSpellStore.LookupEntry(PRIEST_SPELL_PENANCE_R2))
                    return false;
                if (!sSpellStore.LookupEntry(PRIEST_SPELL_PENANCE_R2_DAMAGE))
                    return false;
                if (!sSpellStore.LookupEntry(PRIEST_SPELL_PENANCE_R2_HEAL))
                    return false;

                if (!sSpellStore.LookupEntry(PRIEST_SPELL_PENANCE_R3))
                    return false;
                if (!sSpellStore.LookupEntry(PRIEST_SPELL_PENANCE_R3_DAMAGE))
                    return false;
                if (!sSpellStore.LookupEntry(PRIEST_SPELL_PENANCE_R3_HEAL))
                    return false;

                if (!sSpellStore.LookupEntry(PRIEST_SPELL_PENANCE_R4))
                    return false;
                if (!sSpellStore.LookupEntry(PRIEST_SPELL_PENANCE_R4_DAMAGE))
                    return false;
                if (!sSpellStore.LookupEntry(PRIEST_SPELL_PENANCE_R4_HEAL))
                    return false;

                return true;
            }

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                Unit *unitTarget = GetHitUnit();
                if (!unitTarget || !unitTarget->isAlive())
                    return;

                Unit *caster = GetCaster();
                SpellEntry const *spellInfo = GetSpellInfo();

                int hurt = 0;
                int heal = 0;
                switch(spellInfo->Id)
                {
                    case PRIEST_SPELL_PENANCE_R1:
                        hurt = PRIEST_SPELL_PENANCE_R1_DAMAGE;
                        heal = PRIEST_SPELL_PENANCE_R1_HEAL;
                        break;
                    case PRIEST_SPELL_PENANCE_R2:
                        hurt = PRIEST_SPELL_PENANCE_R2_DAMAGE;
                        heal = PRIEST_SPELL_PENANCE_R2_HEAL;
                        break;
                    case PRIEST_SPELL_PENANCE_R3:
                        hurt = PRIEST_SPELL_PENANCE_R3_DAMAGE;
                        heal = PRIEST_SPELL_PENANCE_R3_HEAL;
                        break;
                    case PRIEST_SPELL_PENANCE_R4:
                        hurt = PRIEST_SPELL_PENANCE_R4_DAMAGE;
                        heal = PRIEST_SPELL_PENANCE_R4_HEAL;
                        break;
                    default:
                        sLog.outError("spell_pri_penance_SpellScript::HandleDummy: Spell %u Penance need set correct heal/damage spell", spellInfo->Id);
                        return;
                }

                if (caster->IsFriendlyTo(unitTarget))
                    caster->CastSpell(unitTarget, heal, false, 0);
                else
                    caster->CastSpell(unitTarget, hurt, false, 0);
            }

            void Register()
            {
                // add dummy effect spell handler to Penance
                OnEffect += SpellEffectFn(spell_pri_penance_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript *GetSpellScript() const
        {
            return new spell_pri_penance_SpellScript;
        }
};

void AddSC_priest_spell_scripts()
{
    new spell_pri_mana_burn;
    new spell_pri_pain_and_suffering_proc;
    new spell_pri_penance;
}
