/*
 * Copyright (C) 2009 Trinity <http://www.trinitycore.org/>
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

/* Script Data Start
SDName: Boss gal_darah
SDAuthor: LordVanMartin, some fixes added by pivonroll
SD%Complete:
SDComment:
SDCategory:
Script Data End */

/*** SQL START ***
update creature_template set scriptname = '' where entry = '';
*** SQL END ***/
#include "ScriptedPch.h"
#include "gundrak.h"

//Spells
enum Spells
{
    SPELL_ENRAGE                                  = 55285,
    H_SPELL_ENRAGE                                = 59828,

    SPELL_IMPALING_CHARGE                         = 54956,
    H_SPELL_IMPALING_CHARGE                       = 59827,

    SPELL_STOMP                                   = 55292,
    H_SPELL_STOMP                                 = 59829,

    SPELL_PUNCTURE                                = 55276,
    H_SPELL_PUNCTURE                              = 59826,

    SPELL_STAMPEDE                                = 55218,

    SPELL_WHIRLING_SLASH                          = 55250,
    H_SPELL_WHIRLING_SLASH                        = 55824,

    SPELL_ECK_RESIDUE                             = 55817
};

enum Diplays
{
    DISPLAY_RHINO       = 26265,
    DISPLAY_TROLL       = 27061
};

enum Achievements
{
    ACHIEVEMENT_WHAT_THE_ECK      = 1864,
    ACHIEVEMENT_SHARE_THE_LOVE    = 2152
};

//Yells
enum Yells
{
    SAY_AGGRO                                  = -1604000,
    SAY_SLAY_1                                 = -1604001,
    SAY_SLAY_2                                 = -1604002,
    SAY_SLAY_3                                 = -1604003,
    SAY_DEATH                                  = -1604004,
    SAY_SUMMON_RHINO_1                         = -1604005,
    SAY_SUMMON_RHINO_2                         = -1604006,
    SAY_SUMMON_RHINO_3                         = -1604007,
    SAY_TRANSFORM_1                            = -1604008,  //Phase change
    SAY_TRANSFORM_2                            = -1604009
};

enum CombatPhase
{
    TROLL,
    RHINO
};

struct boss_gal_darahAI : public ScriptedAI
{
    boss_gal_darahAI(Creature *c) : ScriptedAI(c)
    {
        pInstance = c->GetInstanceData();
    }

    uint32 uiStampedeTimer;
    uint32 uiWhirlingSlashTimer;
    uint32 uiPunctureTimer;
    uint32 uiEnrageTimer;
    uint32 uiImpalingChargeTimer;
    uint32 uiStompTimer;
    uint32 uiTransformationTimer;    
    std::set<uint64> lImpaledPlayers;

    bool StartOfTransformation;

    CombatPhase Phase;

    uint8 uiPhaseCounter;

    ScriptedInstance* pInstance;

    void Reset()
    {
        uiStampedeTimer = 10000;
        uiWhirlingSlashTimer = 21000;
        uiPunctureTimer = 10000;
        uiEnrageTimer = 15000;
        uiStompTimer = 25000;
        uiImpalingChargeTimer = 21000;

        lImpaledPlayers.clear();

        //This timer is used for Creature to wait for his Whirlwind spell to finish casting and then after approximately 5 s
        // after casting of Whirling Slash is complete he will transform to Rhino. This goes same for transforming back to Troll but
        // in this case he needs approximately 5s.
        uiTransformationTimer = 9000;

        //This is used to stop from multiple setting up Creature's state to Stunned and React-state to React-passive
        StartOfTransformation = true;

        Phase = TROLL;
        m_creature->SetDisplayId(DISPLAY_TROLL);

        if (pInstance)
            pInstance->SetData(DATA_GAL_DARAH_EVENT, NOT_STARTED);
    }

    void EnterCombat(Unit* who)
    {
        DoScriptText(SAY_AGGRO, m_creature);

        if (pInstance)
            pInstance->SetData(DATA_GAL_DARAH_EVENT, IN_PROGRESS);
    }

    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!UpdateVictim())
            return;

        switch (Phase)
        {
            case TROLL:
                if (uiPhaseCounter == 2)
                {

                    if(uiTransformationTimer <= diff)
                    {
                        m_creature->SetDisplayId(DISPLAY_RHINO);
                        Phase = RHINO;
                        uiPhaseCounter = 0;
                        DoScriptText(SAY_TRANSFORM_1,m_creature);

                        //When Gal'Darah is in the Rhino phase he need approximately 5s to transform after he casts Impaling Charge
                        uiTransformationTimer = 5000;

                        StartOfTransformation =true;

                        // Let Creature move and attack attackers
                        m_creature->clearUnitState(UNIT_STAT_STUNNED | UNIT_STAT_ROOT);
                        m_creature->SetReactState(REACT_AGGRESSIVE);
                    }
                    else
                    {
                        uiTransformationTimer -=diff;

                        if(StartOfTransformation)
                        {
                            //Stop creature from moving and attacking
                            m_creature->addUnitState(UNIT_STAT_STUNNED | UNIT_STAT_ROOT);
                            m_creature->SetReactState(REACT_PASSIVE);
                            StartOfTransformation = false;
                        }
                    }
                }
                else
                {
                    if (uiStampedeTimer <= diff)
                    {
                        DoCast(m_creature, SPELL_STAMPEDE);
                        DoScriptText(RAND(SAY_SUMMON_RHINO_1,SAY_SUMMON_RHINO_2,SAY_SUMMON_RHINO_3),m_creature);
                        uiStampedeTimer = 15000;
                    } else uiStampedeTimer -= diff;

                    if (uiWhirlingSlashTimer <= diff)
                    {
                        DoCast(m_creature->getVictim(), DUNGEON_MODE(SPELL_WHIRLING_SLASH, H_SPELL_WHIRLING_SLASH));
                        uiWhirlingSlashTimer = 21000;
                        ++uiPhaseCounter;
                    } else uiWhirlingSlashTimer -= diff;
                }
                break;
            case RHINO:
                if (uiPhaseCounter == 2)
                {

                    if(uiTransformationTimer <= diff)
                    {
                        m_creature->SetDisplayId(DISPLAY_TROLL);
                        Phase = TROLL;
                        uiPhaseCounter = 0;
                        DoScriptText(SAY_TRANSFORM_2,m_creature);

                        //When Gal'Darah is in the Troll phase he need approximately 9s to transform after he casts Whirling Slash
                        uiTransformationTimer = 9000;
                        StartOfTransformation = true;
                        m_creature->clearUnitState(UNIT_STAT_STUNNED | UNIT_STAT_ROOT);
                        m_creature->SetReactState(REACT_AGGRESSIVE);
                    }
                    else
                    {
                        uiTransformationTimer -= diff;

                        if(StartOfTransformation)
                        {
                            m_creature->addUnitState(UNIT_STAT_STUNNED | UNIT_STAT_ROOT);
                            m_creature->SetReactState(REACT_PASSIVE);
                            StartOfTransformation = false;
                        }
                    }
                }
                else
                {
                    if (uiPunctureTimer <= diff)
                    {
                        DoCast(m_creature->getVictim(), DUNGEON_MODE(SPELL_PUNCTURE, H_SPELL_PUNCTURE));
                        uiPunctureTimer = 8000;
                    } else uiPunctureTimer -= diff;

                    if (uiEnrageTimer <= diff)
                    {
                        DoCast(m_creature->getVictim(), DUNGEON_MODE(SPELL_ENRAGE, H_SPELL_ENRAGE));
                        uiEnrageTimer = 20000;
                    } else uiEnrageTimer -= diff;

                    if (uiStompTimer <= diff)
                    {
                        DoCast(m_creature->getVictim(), DUNGEON_MODE(SPELL_STOMP, H_SPELL_STOMP));
                        uiStompTimer = 20000;
                    } else uiStompTimer -= diff;

                    if (uiImpalingChargeTimer <= diff)
                    {
                        if (Unit* pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0, 100, true))
                        {
                            DoCast(pTarget, DUNGEON_MODE(SPELL_IMPALING_CHARGE, H_SPELL_IMPALING_CHARGE));
                            lImpaledPlayers.insert(pTarget->GetGUID());
                        }
                        uiImpalingChargeTimer = 31000;
                        ++uiPhaseCounter;
                    } else uiImpalingChargeTimer -= diff;
                }
                break;
        }

        DoMeleeAttackIfReady();
    }

    void JustDied(Unit* killer)
    {
        DoScriptText(SAY_DEATH, m_creature);

        if (IsHeroic())
        {
            AchievementEntry const *achievWhatTheEck = GetAchievementStore()->LookupEntry(ACHIEVEMENT_WHAT_THE_ECK);
            if (achievWhatTheEck)
            {
                Map* pMap = m_creature->GetMap();
                if (pMap && pMap->IsDungeon())
                {
                    Map::PlayerList const &players = pMap->GetPlayers();
                    for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
                        if (itr->getSource() && itr->getSource()->HasAura(SPELL_ECK_RESIDUE))
                            itr->getSource()->CompletedAchievement(achievWhatTheEck);
                }
            }
        }

        if (pInstance && IsHeroic() && lImpaledPlayers.size() == 5)
            pInstance->DoCompleteAchievement(ACHIEVEMENT_SHARE_THE_LOVE);

        if (pInstance)
            pInstance->SetData(DATA_GAL_DARAH_EVENT, DONE);
    }

    void KilledUnit(Unit *victim)
    {
        if (victim == m_creature)
            return;

        DoScriptText(RAND(SAY_SLAY_1,SAY_SLAY_2,SAY_SLAY_3), m_creature);
    }
};

CreatureAI* GetAI_boss_gal_darah(Creature* pCreature)
{
    return new boss_gal_darahAI (pCreature);
}

void AddSC_boss_gal_darah()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "boss_gal_darah";
    newscript->GetAI = &GetAI_boss_gal_darah;
    newscript->RegisterSelf();
}
