#include "ScriptedPch.h"
#include "OutdoorPvPWG.h"
#include "World.h"

struct npc_demolisher_engineererAI : public ScriptedAI
{
    npc_demolisher_engineererAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        me->SetReactState(REACT_PASSIVE);
    }
};

CreatureAI* GetAI_npc_demolisher_engineerer(Creature* pCreature)
{
    return new npc_demolisher_engineererAI (pCreature);
}

bool GossipHello_npc_demolisher_engineerer(Player* pPlayer, Creature* pCreature)
{
    OutdoorPvPWG *pvpWG = (OutdoorPvPWG*) sOutdoorPvPMgr.GetOutdoorPvPToZoneId(NORTHREND_WINTERGRASP);

    if (!pvpWG)
        return false;

    if (pCreature->isQuestGiver())
        pPlayer->PrepareQuestMenu(pCreature->GetGUID());

    if (pPlayer->isGameMaster() || pCreature->GetZoneScript() && pCreature->GetZoneScript()->GetData(pCreature->GetDBTableGUIDLow()))
    {
        if (pPlayer->HasAura(SPELL_CORPORAL))
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, pvpWG->GetLocaleString(WG_STRING_ENG_GOSSIP_1, sWorld.GetDefaultDbcLocale()), GOSSIP_SENDER_MAIN,   GOSSIP_ACTION_INFO_DEF);
        else if (pPlayer->HasAura(SPELL_LIEUTENANT))
        {
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, pvpWG->GetLocaleString(WG_STRING_ENG_GOSSIP_1, sWorld.GetDefaultDbcLocale()), GOSSIP_SENDER_MAIN,   GOSSIP_ACTION_INFO_DEF);
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, pvpWG->GetLocaleString(WG_STRING_ENG_GOSSIP_2, sWorld.GetDefaultDbcLocale()), GOSSIP_SENDER_MAIN,   GOSSIP_ACTION_INFO_DEF+1);
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, pvpWG->GetLocaleString(WG_STRING_ENG_GOSSIP_3, sWorld.GetDefaultDbcLocale()), GOSSIP_SENDER_MAIN,   GOSSIP_ACTION_INFO_DEF+2);
        }
    }
    else
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, pvpWG->GetLocaleString(WG_STRING_ENG_GOSSIP_4, sWorld.GetDefaultDbcLocale()), GOSSIP_SENDER_MAIN,   GOSSIP_ACTION_INFO_DEF+9);

    pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetGUID());

    return true;
}

bool GossipSelect_npc_demolisher_engineerer(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
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

void AddSC_wintergrasp()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "npc_demolisher_engineerer";
    newscript->GetAI = &GetAI_npc_demolisher_engineerer;
    newscript->pGossipHello = &GossipHello_npc_demolisher_engineerer;
    newscript->pGossipSelect = &GossipSelect_npc_demolisher_engineerer;
    newscript->RegisterSelf();
}
