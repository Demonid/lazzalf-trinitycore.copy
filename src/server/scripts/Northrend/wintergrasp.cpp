#include "ScriptPCH.h"
#include "../OutdoorPvP/OutdoorPvPWG.h"
#include "OutdoorPvPMgr.h"
#include "World.h"

/*******************************************************
 * npc_demolisher_engineerer
 *******************************************************/

class npc_demolisher_engineerer : public CreatureScript
{
    public:
    npc_demolisher_engineerer() : CreatureScript("npc_demolisher_engineerer") { }

    struct npc_demolisher_engineererAI : public ScriptedAI
    {
        npc_demolisher_engineererAI(Creature* pCreature) : ScriptedAI(pCreature)
        {
            me->SetReactState(REACT_PASSIVE);
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_demolisher_engineererAI (pCreature);
    }

    bool OnGossipHello(Player* pPlayer, Creature* pCreature)
    {
        OutdoorPvPWG *pvpWG = (OutdoorPvPWG*) sOutdoorPvPMgr->GetOutdoorPvPToZoneId(NORTHREND_WINTERGRASP);

        if (!pvpWG)
            return false;

        if (pCreature->isQuestGiver())
            pPlayer->PrepareQuestMenu(pCreature->GetGUID());

        if (pPlayer->isGameMaster() || pCreature->GetZoneScript() && pCreature->GetZoneScript()->GetData(pCreature->GetDBTableGUIDLow()))
        {
            if (pPlayer->HasAura(SPELL_CORPORAL))
                pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, pvpWG->GetLocaleString(WG_STRING_ENG_GOSSIP_1, sWorld->GetDefaultDbcLocale()), GOSSIP_SENDER_MAIN,   GOSSIP_ACTION_INFO_DEF);
            else if (pPlayer->HasAura(SPELL_LIEUTENANT))
            {
                pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, pvpWG->GetLocaleString(WG_STRING_ENG_GOSSIP_1, sWorld->GetDefaultDbcLocale()), GOSSIP_SENDER_MAIN,   GOSSIP_ACTION_INFO_DEF);
                pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, pvpWG->GetLocaleString(WG_STRING_ENG_GOSSIP_2, sWorld->GetDefaultDbcLocale()), GOSSIP_SENDER_MAIN,   GOSSIP_ACTION_INFO_DEF+1);
                pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, pvpWG->GetLocaleString(WG_STRING_ENG_GOSSIP_3, sWorld->GetDefaultDbcLocale()), GOSSIP_SENDER_MAIN,   GOSSIP_ACTION_INFO_DEF+2);
            }
        }
        else
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, pvpWG->GetLocaleString(WG_STRING_ENG_GOSSIP_4, sWorld->GetDefaultDbcLocale()), GOSSIP_SENDER_MAIN,   GOSSIP_ACTION_INFO_DEF+9);

        pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetGUID());

        return true;
    }

    bool OnGossipSelect(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
    {
        pPlayer->PlayerTalkClass->ClearMenus();
        pPlayer->CLOSE_GOSSIP_MENU();

        if (pPlayer->isGameMaster() || pCreature->GetZoneScript() && pCreature->GetZoneScript()->GetData(pCreature->GetDBTableGUIDLow()))
        {
            switch(uiAction - GOSSIP_ACTION_INFO_DEF)
            {
                case 0: pPlayer->CastSpell(pPlayer, SPELL_BUILD_CATAPULT, false, NULL, NULL, pCreature->GetGUID()); break;
                case 1: pPlayer->CastSpell(pPlayer, SPELL_BUILD_DEMOLISHER, false, NULL, NULL, pCreature->GetGUID()); break;
                case 2: pPlayer->CastSpell(pPlayer, pPlayer->GetTeamId() ? SPELL_BUILD_SIEGE_ENGINE_H : SPELL_BUILD_SIEGE_ENGINE_A, false, NULL, NULL, pCreature->GetGUID()); break;
            }
        }

        return true;
    }
};

/*******************************************************
 * npc_wg_misc
 *******************************************************/

enum NPC_WG_MISC_SPELLS
{
    SPELL_STRIKE                = 11976,    // WG_CREATURE_GUARD + WG_CREATURE_CHAMPION
    SPELL_SLEEPING_SLEEP        = 42648,    // WG_CREATURE_CHAMPION
    SPELL_ARCANE_MISSILES       = 31743,    // WG_CREATURE_ENHANCEMENT_A
    SPELL_SLOW                  = 31741,    // WG_CREATURE_ENHANCEMENT_A
    SPELL_HEALING_WAVE          = 11986,    // WG_CREATURE_ENHANCEMENT_H
    SPELL_LIGHTNING_BOLT        = 9532,     // WG_CREATURE_ENHANCEMENT_H
    SPELL_QUICK_FLAME_WARD      = 4979,     // WG_CREATURE_ENHANCEMENT_H
    SPELL_MORTAL_STRIKE         = 15708,    // WG_CREATURE_QUESTGIVER_1 + WG_CREATURE_QUESTGIVER_2 + WG_CREATURE_QUESTGIVER_3 + WG_CREATURE_QUESTGIVER_4
    SPELL_HEAL                  = 34945,    // WG_CREATURE_QUESTGIVER_5_A
    SPELL_HOLY_NOVA             = 34944,    // WG_CREATURE_QUESTGIVER_5_A
    SPELL_POWER_WORD_SHIELD     = 17139,    // WG_CREATURE_QUESTGIVER_5_A
    SPELL_CLEAVE                = 15284     // WG_CREATURE_QUESTGIVER_5_H + WG_CREATURE_QUESTGIVER_6
};

enum NPC_WG_MISC_EVENTS
{
    EVENT_STRIKE = 1,
    EVENT_ARCANE_MISSILES,
    EVENT_SLOW,
    EVENT_HEALING_WAVE,
    EVENT_LIGHTNING_BOLT,
    EVENT_QUICK_FLAME_WARD,
    EVENT_MORTAL_STRIKE,
    EVENT_HEAL,
    EVENT_HOLY_NOVA,
    EVENT_CLEAVE
 };

class npc_wg_misc : public CreatureScript
{
    public:
    npc_wg_misc() : CreatureScript("npc_wg_misc") { }

    struct npc_wg_miscAI : public ScriptedAI
    {
        npc_wg_miscAI(Creature* pCreature) : ScriptedAI(pCreature) {}

        EventMap events;
        OutdoorPvPWG *pvpWG;

        void Reset()
        {
            events.Reset();
            pvpWG = (OutdoorPvPWG*) sOutdoorPvPMgr->GetOutdoorPvPToZoneId(NORTHREND_WINTERGRASP);

            switch(me->GetEntry())
            {
                case WG_CREATURE_ENHANCEMENT_A:
                case WG_CREATURE_ENHANCEMENT_H:
                case WG_CREATURE_QUESTGIVER_5_A:
                    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
                    return;
            }

            switch(me->GetEntry())
            {
                case WG_CREATURE_CHAMPION_A:
                case WG_CREATURE_CHAMPION_H:
                    if (pvpWG && !pvpWG->isWarTime())
                        DoCast(me, SPELL_SLEEPING_SLEEP);
                    break;
            }
        }

        void EnterCombat(Unit *who)
        {
            events.ScheduleEvent(EVENT_STRIKE, 5000);
            events.ScheduleEvent(EVENT_ARCANE_MISSILES, 500);
            events.ScheduleEvent(EVENT_SLOW, 100);
            events.ScheduleEvent(EVENT_HEALING_WAVE, 5000);
            events.ScheduleEvent(EVENT_LIGHTNING_BOLT, 500);
            events.ScheduleEvent(EVENT_QUICK_FLAME_WARD, 100);
            events.ScheduleEvent(EVENT_MORTAL_STRIKE, 5000);
            events.ScheduleEvent(EVENT_HEAL, 5000);
            events.ScheduleEvent(EVENT_HOLY_NOVA, 4000);
            events.ScheduleEvent(EVENT_CLEAVE, 5000);

            switch(me->GetEntry())
            {
                case WG_CREATURE_QUESTGIVER_5_A:
                    DoCast(me, SPELL_POWER_WORD_SHIELD);
                    break;
            }
        }

        void UpdateAI(const uint32 uiDiff)
        {
            if (!UpdateVictim())
                return;

            events.Update(uiDiff);

            if (me->HasUnitState(UNIT_STAT_CASTING))
                return;

            while(uint32 eventId = events.ExecuteEvent())
            {
                switch(eventId)
                {
                    case EVENT_STRIKE:
                        switch(me->GetEntry())
                        {
                            case WG_CREATURE_GUARD_A:
                            case WG_CREATURE_GUARD_H:
                            case WG_CREATURE_CHAMPION_A:
                            case WG_CREATURE_CHAMPION_H:
                                DoCast(me->getVictim(), SPELL_STRIKE);
                                events.ScheduleEvent(EVENT_STRIKE, 5000);
                                return;
                        }
                    case EVENT_ARCANE_MISSILES:
                        switch(me->GetEntry())
                        {
                            case WG_CREATURE_ENHANCEMENT_A:
                                DoCast(me->getVictim(), SPELL_ARCANE_MISSILES);
                                events.ScheduleEvent(EVENT_ARCANE_MISSILES, 5000);
                                return;
                        }
                    case EVENT_SLOW:
                        switch(me->GetEntry())
                        {
                            case WG_CREATURE_ENHANCEMENT_A:
                                DoCast(me->getVictim(), SPELL_SLOW);
                                events.ScheduleEvent(EVENT_SLOW, 15000);
                                return;
                        }
                    case EVENT_HEALING_WAVE:
                        switch(me->GetEntry())
                        {
                            case WG_CREATURE_ENHANCEMENT_H:
                                if (HealthBelowPct(50))
                                    DoCast(me, SPELL_HEALING_WAVE);
                                events.ScheduleEvent(EVENT_HEALING_WAVE, 3000);
                                return;
                        }
                    case EVENT_LIGHTNING_BOLT:
                        switch(me->GetEntry())
                        {
                            case WG_CREATURE_ENHANCEMENT_H:
                                DoCast(me->getVictim(), SPELL_LIGHTNING_BOLT);
                                events.ScheduleEvent(EVENT_LIGHTNING_BOLT, 5000);
                                return;
                        }
                    case EVENT_QUICK_FLAME_WARD:
                        switch(me->GetEntry())
                        {
                            case WG_CREATURE_ENHANCEMENT_H:
                                DoCast(me, SPELL_QUICK_FLAME_WARD);
                                events.ScheduleEvent(EVENT_QUICK_FLAME_WARD, 10000);
                                return;
                        }
                    case EVENT_MORTAL_STRIKE:
                        switch(me->GetEntry())
                        {
                            case WG_CREATURE_QUESTGIVER_1_A:
                            case WG_CREATURE_QUESTGIVER_1_H:
                            case WG_CREATURE_QUESTGIVER_2_A:
                            case WG_CREATURE_QUESTGIVER_2_H:
                            case WG_CREATURE_QUESTGIVER_3_A:
                            case WG_CREATURE_QUESTGIVER_3_H:
                            case WG_CREATURE_QUESTGIVER_4_A:
                            case WG_CREATURE_QUESTGIVER_4_H:
                                DoCast(me->getVictim(), SPELL_MORTAL_STRIKE);
                                events.ScheduleEvent(EVENT_MORTAL_STRIKE, 10000);
                                return;
                        }
                    case EVENT_HEAL:
                        switch(me->GetEntry())
                        {
                            case WG_CREATURE_QUESTGIVER_5_A:
                                if (HealthBelowPct(50))
                                    DoCast(me, SPELL_HEAL);
                                events.ScheduleEvent(EVENT_HEAL, 5000);
                                return;
                        }
                    case EVENT_HOLY_NOVA:
                        switch(me->GetEntry())
                        {
                            case WG_CREATURE_QUESTGIVER_5_A:
                                DoCast(me, SPELL_HOLY_NOVA);
                                events.ScheduleEvent(EVENT_HOLY_NOVA, 8000);
                                return;
                        }
                    case EVENT_CLEAVE:
                        switch(me->GetEntry())
                        {
                            case WG_CREATURE_QUESTGIVER_5_H:
                            case WG_CREATURE_QUESTGIVER_6_A:
                            case WG_CREATURE_QUESTGIVER_6_H:
                                DoCast(me->getVictim(), SPELL_CLEAVE);
                                events.ScheduleEvent(EVENT_CLEAVE, 10000);
                                return;
                        }
                }
            }

            switch(me->GetEntry())
            {
                case WG_CREATURE_ENHANCEMENT_A:
                case WG_CREATURE_ENHANCEMENT_H:
                case WG_CREATURE_QUESTGIVER_5_A:
                    return;
                default:
                    DoMeleeAttackIfReady();
                    return;
            }
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_wg_miscAI (pCreature);
    }
};

/*******************************************************
 * npc_winterguard
 *******************************************************/

class npc_winterguard : public CreatureScript
{
    public:
    npc_winterguard() : CreatureScript("npc_winterguard") { }

    struct npc_winterguardAI : public Scripted_NoMovementAI
    {
        npc_winterguardAI(Creature* pCreature) : Scripted_NoMovementAI(pCreature)
        {
            pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            pCreature->ApplySpellImmune(0, IMMUNITY_DAMAGE, SPELL_SCHOOL_NORMAL, true);
            pCreature->ApplySpellImmune(0, IMMUNITY_DAMAGE, SPELL_SCHOOL_MASK_MAGIC, true);
            
            pvpWG = (OutdoorPvPWG*)sOutdoorPvPMgr->GetOutdoorPvPToZoneId(NORTHREND_WINTERGRASP);
        }

        OutdoorPvPWG *pvpWG;

        void Reset(){}

        void Aggro(Unit* /*pWho*/){}

        void AttackStart(Unit* /*pWho*/){}

        void MoveInLineOfSight(Unit *pWho)
        {
            if (!pWho || !pWho->IsInWorld() || pWho->GetZoneId() != NORTHREND_WINTERGRASP)
                return;

            if (!pvpWG || (pvpWG->isWarTime() == true))
                return;

            if (!me->IsWithinDist(pWho, 40.0f, false))
                return;

            Player *pPlayer = pWho->GetCharmerOrOwnerPlayerOrPlayerItself();

            if (!pPlayer || pPlayer->isGameMaster() || pPlayer->IsBeingTeleported())
                return;

            if ((pPlayer->GetTeam() == ALLIANCE && pvpWG->getDefenderTeamId() != TEAM_ALLIANCE)
                || (pPlayer->GetTeam() == TEAM_HORDE && pvpWG->getDefenderTeamId() != TEAM_HORDE))
                    pPlayer->TeleportTo(571, 5047.93f, 2848.57f, 393, 0);  // Out the Fortress

            return;
        }

        void UpdateAI(const uint32 /*diff*/){}
    };

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_winterguardAI(pCreature);
    }
};

void AddSC_wintergrasp()
{
    new npc_demolisher_engineerer;
    new npc_wg_misc;
    new npc_winterguard;
}
