/*
 * Copyright (C) 2008-2010 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2006-2010 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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
SDName: trial_of_the_crusader
SD%Complete: ??%
SDComment: based on /dev/rsa
SDCategory: Crusader Coliseum
EndScriptData */

// Known bugs:
//    - They should be floating but they aren't respecting the floor =(
//    - Lacks the powering up effect that leads to Empowering
//    - There's a workaround for the shared life effect

#include "ScriptPCH.h"
#include "trial_of_the_crusader.h"

enum Yells
{
    SAY_AGGRO           = -1649040,
    SAY_DEATH           = -1649041,
    SAY_BERSERK         = -1649042,
    EMOTE_SHIELD        = -1649043,
    SAY_SHIELD          = -1649044,
    SAY_KILL1           = -1649045,
    SAY_KILL2           = -1649046,
    EMOTE_LIGHT_VORTEX  = -1649047,
    SAY_LIGHT_VORTEX    = -1649048,
    EMOTE_DARK_VORTEX   = -1649049,
    SAY_DARK_VORTEX     = -1649050,
};

enum Equipment
{
    EQUIP_MAIN_1         = 49303,
    EQUIP_OFFHAND_1      = 47146,
    EQUIP_RANGED_1       = 47267,
    EQUIP_MAIN_2         = 45990,
    EQUIP_OFFHAND_2      = 47470,
    EQUIP_RANGED_2       = 47267,
    EQUIP_DONE           = EQUIP_NO_CHANGE,
};

enum Summons
{
    NPC_DARK_ESSENCE     = 34567,
    NPC_LIGHT_ESSENCE    = 34568,

    NPC_UNLEASHED_DARK   = 34628,
    NPC_UNLEASHED_LIGHT  = 34630,
};

enum BossSpells
{
    SPELL_LIGHT_TWIN_SPIKE      = 66075,
    SPELL_LIGHT_SURGE           = 65766,
    SPELL_LIGHT_SHIELD          = 65858,
    SPELL_LIGHT_TWIN_PACT       = 65876,
    SPELL_LIGHT_VORTEX          = 66046,
    SPELL_LIGHT_TOUCH           = 67297,

    SPELL_DARK_TWIN_SPIKE       = 66069,
    SPELL_DARK_SURGE            = 65768,
    SPELL_DARK_SHIELD           = 65874,
    SPELL_DARK_TWIN_PACT        = 65875,
    SPELL_DARK_VORTEX           = 66058,
    SPELL_DARK_TOUCH            = 67282,

    SPELL_TWIN_POWER            = 65916,
    SPELL_LIGHT_ESSENCE         = RAID_MODE(65686,67222,67223,67224),
    SPELL_DARK_ESSENCE          = RAID_MODE(65684,67176,67177,67178),
    SPELL_BERSERK               = 64238,
    SPELL_NONE                  = 0,

    SPELL_EMPOWERED_DARK        = 67215,
    SPELL_EMPOWERED_LIGHT       = 67218,

    SPELL_UNLEASHED_DARK        = 65808,
    SPELL_UNLEASHED_LIGHT       = 65795,
    //PowerUp 67604
};

enum Actions
{
    ACTION_VORTEX,
    ACTION_PACT
};

#define ACHIEVEMENT_SALT_AND_PEPPER RAID_MODE(3799, 3815)
#define SALT_AND_PEPPER_MAX_TIMER 3 * MINUTE * IN_MILLISECONDS

/*######
## boss_twin_base
######*/

struct boss_twin_baseAI : public ScriptedAI
{
    boss_twin_baseAI(Creature* pCreature) : ScriptedAI(pCreature), Summons(me)
    {
        m_pInstance = (InstanceScript*)pCreature->GetInstanceScript();
        me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
        me->ApplySpellImmune(0, IMMUNITY_ID, 49560, true); // Death Grip jump effect
    }

    InstanceScript* m_pInstance;
    SummonList Summons;

    uint8  m_uiStage;
    bool   m_bIsBerserk;
    uint8  m_uiWaveCount;
    uint32 m_uiColorballsTimer;
    uint32 m_uiSpecialAbilityTimer;
    uint32 m_uiSpikeTimer;
    uint32 m_uiTouchTimer;
    uint32 m_uiBerserkTimer;

    uint32 m_uiVortexSay;
    uint32 m_uiVortexEmote;
    uint32 m_uiSisterNpcId;
    uint32 m_uiColorballNpcId;
    uint32 m_uiEssenceNpcId;
    uint32 m_uiMyEssenceSpellId;
    uint32 m_uiOtherEssenceSpellId;
    uint32 m_uiEmpoweredWeaknessSpellId;
    uint32 m_uiSurgeSpellId;
    uint32 m_uiVortexSpellId;
    uint32 m_uiShieldSpellId;
    uint32 m_uiTwinPactSpellId;
    uint32 m_uiSpikeSpellId;
    uint32 m_uiTouchSpellId;

    Position HomeLocation;
    Position EssenceLocation[2];

    void Reset() {
        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_OOC_NOT_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
        me->SetReactState(REACT_PASSIVE);
        /* Uncomment this once that they are flying above the ground
        me->AddUnitMovementFlag(MOVEMENTFLAG_LEVITATING);
        me->SetFlying(true); */
        m_bIsBerserk = false;

        m_uiWaveCount = 1;
        m_uiColorballsTimer = 15*IN_MILLISECONDS;
        m_uiSpecialAbilityTimer = urand(45,50)*IN_MILLISECONDS;
        m_uiSpikeTimer = 20*IN_MILLISECONDS;
        m_uiTouchTimer = urand(10, 15)*IN_MILLISECONDS;
        m_uiBerserkTimer = IsHeroic() ? 6*MINUTE*IN_MILLISECONDS : 10*MINUTE*IN_MILLISECONDS;

        Summons.DespawnAll();
    }

    void JustReachedHome()
    {
        if (m_pInstance)
        {
            m_pInstance->SetData(TYPE_VALKIRIES, FAIL);
            m_pInstance->SetData(DATA_HEALTH_TWIN_SHARED, me->GetMaxHealth());
        }
        me->ForcedDespawn();
    }

    void MovementInform(uint32 uiType, uint32 uiId)
    {
        if (uiType != POINT_MOTION_TYPE) return;

        switch (uiId)
        {
            case 0:
                me->GetMotionMaster()->MovePoint(1, HomeLocation.GetPositionX(), HomeLocation.GetPositionY(), HomeLocation.GetPositionZ());
                me->SetHomePosition(HomeLocation);
                break;
            case 1:
                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_OOC_NOT_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
                me->SetReactState(REACT_AGGRESSIVE);
                me->SetInCombatWithZone();
                break;
        }
    }

    void KilledUnit(Unit* pWho)
    {
        if (pWho->GetTypeId() == TYPEID_PLAYER)
        {
            DoScriptText(urand(0, 1) ? SAY_KILL1 : SAY_KILL2, me);
            if (m_pInstance)
                m_pInstance->SetData(DATA_TRIBUTE_TO_IMMORTALITY_ELEGIBLE, 0);
        }
    }

    void JustSummoned(Creature* pSummoned)
    {
        switch (pSummoned->GetEntry())
        {
            case NPC_UNLEASHED_DARK:
            case NPC_UNLEASHED_LIGHT:
                pSummoned->SetCorpseDelay(0);
                break;
        }
        Summons.Summon(pSummoned);
    }

    void SummonedCreatureDespawn(Creature* pSummoned)
    {
        switch (pSummoned->GetEntry())
        {
            case NPC_LIGHT_ESSENCE:
            case NPC_DARK_ESSENCE:
                Map* pMap = me->GetMap();
                Map::PlayerList const &lPlayers = pMap->GetPlayers();
                for (Map::PlayerList::const_iterator itr = lPlayers.begin(); itr != lPlayers.end(); ++itr)
                {
                    Unit* pPlayer = itr->getSource();
                    if (!pPlayer) 
                        continue;
                    if (pPlayer->isAlive())
                    {
                        if (pSummoned->GetEntry() == NPC_LIGHT_ESSENCE)
                            pPlayer->RemoveAurasDueToSpell(SPELL_LIGHT_ESSENCE);
                        if (pSummoned->GetEntry() == NPC_DARK_ESSENCE)
                            pPlayer->RemoveAurasDueToSpell(SPELL_DARK_ESSENCE);
                    }
                }
                break;
        }
        Summons.Despawn(pSummoned);
    }

    void DamageTaken(Unit* pDoneBy, uint32 &uiDamage)
    {
        if (!me || !me->isAlive())
            return;

        if (pDoneBy->GetGUID() == me->GetGUID())
            return;

        if (pDoneBy->GetTypeId() == TYPEID_PLAYER)
        {
            if (pDoneBy->HasAura(m_uiOtherEssenceSpellId))
                uiDamage += uiDamage/2;
            if (pDoneBy->HasAura(m_uiEmpoweredWeaknessSpellId))
                uiDamage += uiDamage;
            else
                if (pDoneBy->HasAura(m_uiMyEssenceSpellId))
                    uiDamage /= 2;
        }

        if (m_pInstance)
            m_pInstance->SetData(DATA_HEALTH_TWIN_SHARED, me->GetHealth() >= uiDamage ? me->GetHealth() - uiDamage : 0);
    }

    void SpellHit(Unit* caster, const SpellEntry* spell)
    {
        if (caster->ToCreature() == me)
            if (spell->Effect[0] == 136) //Effect Heal
                if (m_pInstance)
                    m_pInstance->SetData(DATA_HEALTH_TWIN_SHARED, me->GetHealth() + me->CountPctFromMaxHealth(spell->EffectBasePoints[0]));
    }

    void SummonColorballs(uint8 quantity)
    {
        float x0 = ToCCommonLoc[1].GetPositionX(), y0 = ToCCommonLoc[1].GetPositionY(), r = 47.0f;
        float y = y0;
        for (uint8 i = 0; i < quantity; i++)
        {
            float x = float(urand(uint32(x0 - r), uint32(x0 + r)));
            if (urand(0, 1))
                y = y0 + sqrt(pow(r, 2) - pow((x-x0), 2));
            else
                y = y0 - sqrt(pow(r, 2) - pow((x-x0), 2));
            me->SummonCreature(m_uiColorballNpcId, x, y, me->GetPositionZ(), TEMPSUMMON_CORPSE_DESPAWN);
        }
    }

    void JustDied(Unit* /*pKiller*/)
    {
        DoScriptText(SAY_DEATH, me);
        if (m_pInstance)
        {
            m_pInstance->SetData(DATA_HEALTH_TWIN_SHARED, 0);
            if (Creature* pSister = GetSister())
            {
                if (!pSister->isAlive())
                {
                    m_pInstance->SetData(TYPE_VALKIRIES, DONE);
                    Summons.DespawnAll();
                }
                else 
                {
                    m_pInstance->SetData(TYPE_VALKIRIES, SPECIAL);
                    me->Kill(pSister);
                }                    
            }
        }
        Summons.DespawnAll();
    }

    // Called when sister pointer needed
    Creature* GetSister()
    {
        return Unit::GetCreature((*me), m_pInstance->GetData64(m_uiSisterNpcId));
    }

    void EnterCombat(Unit* /*pWho*/)
    {
        me->SetInCombatWithZone();
        if (m_pInstance)
        {
            m_pInstance->SetData(TYPE_VALKIRIES, IN_PROGRESS);
            m_pInstance->SetData(DATA_HEALTH_TWIN_SHARED, me->GetMaxHealth());
        }
        if (me->isAlive())
        {
            me->SummonCreature(m_uiEssenceNpcId, EssenceLocation[0].GetPositionX(), EssenceLocation[0].GetPositionY(), EssenceLocation[0].GetPositionZ());
            me->SummonCreature(m_uiEssenceNpcId, EssenceLocation[1].GetPositionX(), EssenceLocation[1].GetPositionY(), EssenceLocation[1].GetPositionZ());
        }
        DoScriptText(SAY_AGGRO, me);
        DoCast(me, m_uiSurgeSpellId);
    }

    void DoAction(const int32 action)
    {
        switch (action)
        {
            case ACTION_VORTEX:
                m_uiStage = me->GetEntry() == NPC_LIGHTBANE ? 2 : 1;
                break;
            case ACTION_PACT:
                m_uiStage = me->GetEntry() == NPC_LIGHTBANE ? 1 : 2;
                DoCast(me, SPELL_TWIN_POWER);
                break;
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_pInstance || !UpdateVictim())
            return;

        if (m_pInstance->GetData(DATA_HEALTH_TWIN_SHARED) != 0)
            me->SetHealth(m_pInstance->GetData(DATA_HEALTH_TWIN_SHARED));
        else
            me->SetHealth(1);

        switch (m_uiStage)
        {
            case 0:
                break;
            case 1: // Vortex
                if (m_uiSpecialAbilityTimer <= uiDiff)
                {
                    if (Creature* pSister = GetSister())
                        pSister->AI()->DoAction(ACTION_VORTEX);
                    DoScriptText(m_uiVortexEmote, me);
                    DoScriptText(m_uiVortexSay, me);
                    DoCastAOE(m_uiVortexSpellId);
                    m_uiStage = 0;
                    m_uiSpecialAbilityTimer = urand(45,50)*IN_MILLISECONDS;
                } else m_uiSpecialAbilityTimer -= uiDiff;
                break;
            case 2: // Shield+Pact
                if (m_uiSpecialAbilityTimer <= uiDiff)
                {
                    if (Creature* pSister = GetSister())
                        pSister->AI()->DoAction(ACTION_PACT);
                    DoScriptText(EMOTE_SHIELD, me);
                    DoScriptText(SAY_SHIELD, me);
                    DoCast(me, m_uiShieldSpellId);
                    DoCast(me, m_uiTwinPactSpellId);
                    m_uiStage = 0;
                    m_uiSpecialAbilityTimer = urand(45,50)*IN_MILLISECONDS;
                } m_uiSpecialAbilityTimer -= uiDiff;
                break;
            default:
                break;
        }

        if (m_uiSpikeTimer <= uiDiff)
        {
            DoCastVictim(m_uiSpikeSpellId);
            m_uiSpikeTimer = 20*IN_MILLISECONDS;
        } m_uiSpikeTimer -= uiDiff;

        if (IsHeroic() && m_uiTouchTimer <= uiDiff)
        {
            if (Unit *pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0, 200, true, m_uiOtherEssenceSpellId))
                DoCast(pTarget, m_uiTouchSpellId);
            m_uiTouchTimer = urand(10, 15)*IN_MILLISECONDS;
        } m_uiTouchTimer -= uiDiff;

        if (m_uiColorballsTimer <= uiDiff)
        {
            if (m_uiWaveCount >= 2)
            {
                SummonColorballs(12);
                m_uiWaveCount = 0;
            }
            else
            {
                SummonColorballs(2);
                m_uiWaveCount++;
            }
            m_uiColorballsTimer = 15*IN_MILLISECONDS;
        } else m_uiColorballsTimer -= uiDiff;

        if (!m_bIsBerserk && m_uiBerserkTimer <= uiDiff)
        {
            DoCast(me, SPELL_BERSERK);
            DoScriptText(SAY_BERSERK, me);
            m_bIsBerserk = true;
        } else m_uiBerserkTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

/*######
## boss_fjola
######*/

class boss_fjola : public CreatureScript
{
public:
    boss_fjola() : CreatureScript("boss_fjola") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new boss_fjolaAI(pCreature);
    }

    struct boss_fjolaAI : public boss_twin_baseAI
    {
        boss_fjolaAI(Creature* pCreature) : boss_twin_baseAI(pCreature) {}

        uint32 saltAndPepperTimer;

        void Reset() {
            boss_twin_baseAI::Reset();
            SetEquipmentSlots(false, EQUIP_MAIN_1, EQUIP_OFFHAND_1, EQUIP_RANGED_1);
            m_uiStage = 0;
            m_uiVortexEmote = EMOTE_LIGHT_VORTEX;
            m_uiVortexSay = SAY_LIGHT_VORTEX;
            m_uiSisterNpcId = NPC_DARKBANE;
            m_uiColorballNpcId = NPC_UNLEASHED_LIGHT;
            m_uiEssenceNpcId = NPC_LIGHT_ESSENCE;
            m_uiMyEssenceSpellId = SPELL_LIGHT_ESSENCE;
            m_uiOtherEssenceSpellId = SPELL_DARK_ESSENCE;
            m_uiEmpoweredWeaknessSpellId = SPELL_EMPOWERED_DARK;
            m_uiSurgeSpellId = SPELL_LIGHT_SURGE;
            m_uiVortexSpellId = SPELL_LIGHT_VORTEX;
            m_uiShieldSpellId = SPELL_LIGHT_SHIELD;
            m_uiTwinPactSpellId = SPELL_LIGHT_TWIN_PACT;
            m_uiTouchSpellId = SPELL_LIGHT_TOUCH;
            m_uiSpikeSpellId = SPELL_LIGHT_TWIN_SPIKE;

            HomeLocation = ToCCommonLoc[8];
            EssenceLocation[0] = TwinValkyrsLoc[2];
            EssenceLocation[1] = TwinValkyrsLoc[3];

            saltAndPepperTimer = 0;

            /*if (m_pInstance)
            {
                m_pInstance->DoStopTimedAchievement(ACHIEVEMENT_TIMED_TYPE_EVENT,  EVENT_START_TWINS_FIGHT);
            }*/
        }

        void EnterCombat(Unit* pWho)
        {
            saltAndPepperTimer = 0;
            boss_twin_baseAI::EnterCombat(pWho);
            /*if (m_pInstance)
            {
                m_pInstance->DoStartTimedAchievement(ACHIEVEMENT_TIMED_TYPE_EVENT,  EVENT_START_TWINS_FIGHT);
            }*/
        }

        void JustDied(Unit* pKiller)
        {
            if (saltAndPepperTimer <= SALT_AND_PEPPER_MAX_TIMER)
                m_pInstance->DoCompleteAchievement(ACHIEVEMENT_SALT_AND_PEPPER);
            
            boss_twin_baseAI::JustDied(pKiller);
        }

        void UpdateAI(const uint32 uiDiff)
        {
            saltAndPepperTimer += uiDiff;
            boss_twin_baseAI::UpdateAI(uiDiff);
        }
    };

};


/*######
## boss_eydis
######*/

class boss_eydis : public CreatureScript
{
public:
    boss_eydis() : CreatureScript("boss_eydis") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new boss_eydisAI(pCreature);
    }

    struct boss_eydisAI : public boss_twin_baseAI
    {
        boss_eydisAI(Creature* pCreature) : boss_twin_baseAI(pCreature) {}

        void Reset() {
            boss_twin_baseAI::Reset();
            SetEquipmentSlots(false, EQUIP_MAIN_2, EQUIP_OFFHAND_2, EQUIP_RANGED_2);
            m_uiStage = 1;
            m_uiVortexEmote = EMOTE_DARK_VORTEX;
            m_uiVortexSay = SAY_DARK_VORTEX;
            m_uiSisterNpcId = NPC_LIGHTBANE;
            m_uiColorballNpcId = NPC_UNLEASHED_DARK;
            m_uiEssenceNpcId = NPC_DARK_ESSENCE;
            m_uiMyEssenceSpellId = SPELL_DARK_ESSENCE;
            m_uiOtherEssenceSpellId = SPELL_LIGHT_ESSENCE;
            m_uiEmpoweredWeaknessSpellId = SPELL_EMPOWERED_LIGHT;
            m_uiSurgeSpellId = SPELL_DARK_SURGE;
            m_uiVortexSpellId = SPELL_DARK_VORTEX;
            m_uiShieldSpellId = SPELL_DARK_SHIELD;
            m_uiTwinPactSpellId = SPELL_DARK_TWIN_PACT;
            m_uiTouchSpellId = SPELL_DARK_TOUCH;
            m_uiSpikeSpellId = SPELL_DARK_TWIN_SPIKE;

            HomeLocation = ToCCommonLoc[9];
            EssenceLocation[0] = TwinValkyrsLoc[0];
            EssenceLocation[1] = TwinValkyrsLoc[1];
        }
    };

};


class mob_essence_of_twin : public CreatureScript
{
public:
    mob_essence_of_twin() : CreatureScript("mob_essence_of_twin") { }

    bool OnGossipHello(Player* player, Creature* creature)
    {
        switch (creature->GetEntry())
        {
            case NPC_LIGHT_ESSENCE:
                player->RemoveAurasDueToSpell(SPELL_DARK_ESSENCE);
                player->CastSpell(player, SPELL_LIGHT_ESSENCE, true);
                break;
            case NPC_DARK_ESSENCE:
                player->RemoveAurasDueToSpell(SPELL_LIGHT_ESSENCE);
                player->CastSpell(player, SPELL_DARK_ESSENCE, true);
                break;
            default:
                break;
        }
        player->CLOSE_GOSSIP_MENU();
        return true;
    }
};


struct mob_unleashed_ballAI : public ScriptedAI
{
    mob_unleashed_ballAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (InstanceScript*)pCreature->GetInstanceScript();
    }

    InstanceScript* m_pInstance;
    uint32 m_uiRangeCheckTimer;

    void MoveToNextPoint()
    {
        float x0 = ToCCommonLoc[1].GetPositionX(), y0 = ToCCommonLoc[1].GetPositionY(), r = 47.0f;
        float y = y0;
        float x = float(urand(uint32(x0 - r), uint32(x0 + r)));
        if (urand(0, 1))
            y = y0 + sqrt(pow(r, 2) - pow((x-x0), 2));
        else
            y = y0 - sqrt(pow(r, 2) - pow((x-x0), 2));
        me->GetMotionMaster()->MovePoint(0, x, y, me->GetPositionZ());
    }

    void Reset()
    {
        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_OOC_NOT_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
        me->SetReactState(REACT_PASSIVE);
        me->AddUnitMovementFlag(MOVEMENTFLAG_LEVITATING);
        me->SetFlying(true);
        SetCombatMovement(false);
        MoveToNextPoint();
        m_uiRangeCheckTimer = IN_MILLISECONDS;
    }

    void MovementInform(uint32 uiType, uint32 uiId)
    {
        if (uiType != POINT_MOTION_TYPE) return;

        switch (uiId)
        {
            case 0:
                if (urand(0, 3) == 0)
                    MoveToNextPoint();
                else
                    me->DisappearAndDie();
                break;
        }
    }
};

class mob_unleashed_dark : public CreatureScript
{
public:
    mob_unleashed_dark() : CreatureScript("mob_unleashed_dark") { }

    CreatureAI* GetAI(Creature *pCreature) const
    {
        return new mob_unleashed_darkAI(pCreature);
    }

    struct mob_unleashed_darkAI : public mob_unleashed_ballAI
    {
        mob_unleashed_darkAI(Creature *pCreature) : mob_unleashed_ballAI(pCreature) {}

        void UpdateAI(const uint32 uiDiff)
        {
            if (m_uiRangeCheckTimer < uiDiff)
            {
                if (Unit *pTarget = me->SelectNearestTarget(2.0f))
                    if (pTarget->GetTypeId() == TYPEID_PLAYER && pTarget->isAlive())
                    {
                        DoCastAOE(SPELL_UNLEASHED_DARK);
                        me->GetMotionMaster()->MoveIdle();
                        me->ForcedDespawn(500);
                    }
                m_uiRangeCheckTimer = IN_MILLISECONDS;
            }
            else m_uiRangeCheckTimer -= uiDiff;
        }
    };

};


class mob_unleashed_light : public CreatureScript
{
public:
    mob_unleashed_light() : CreatureScript("mob_unleashed_light") { }

    CreatureAI* GetAI(Creature *pCreature) const
    {
        return new mob_unleashed_lightAI(pCreature);
    }

    struct mob_unleashed_lightAI : public mob_unleashed_ballAI
    {
        mob_unleashed_lightAI(Creature *pCreature) : mob_unleashed_ballAI(pCreature) {}

        void UpdateAI(const uint32 uiDiff)
        {
            if (m_uiRangeCheckTimer < uiDiff)
            {
                if (Unit *pTarget = me->SelectNearestTarget(2.0f))
                    if (pTarget->GetTypeId() == TYPEID_PLAYER && pTarget->isAlive())
                    {
                        DoCastAOE(SPELL_UNLEASHED_LIGHT);
                        me->GetMotionMaster()->MoveIdle();
                        me->ForcedDespawn(500);
                    }
                m_uiRangeCheckTimer = IN_MILLISECONDS;
            }
            else m_uiRangeCheckTimer -= uiDiff;
        }
    };

};


void AddSC_boss_twin_valkyr()
{
    new boss_fjola();
    new boss_eydis();
    new mob_unleashed_light();
    new mob_unleashed_dark();
    new mob_essence_of_twin();
}
