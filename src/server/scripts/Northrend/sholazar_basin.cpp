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

/* ScriptData
SDName: Sholazar_Basin
SD%Complete: 100
SDComment: Quest support: 12570, 12573, 12621.
SDCategory: Sholazar_Basin
EndScriptData */

/* ContentData
npc_injured_rainspeaker_oracle
npc_vekjik
avatar_of_freya
EndContentData */

#include "ScriptPCH.h"
#include "ScriptedEscortAI.h"

/*######
## npc_injured_rainspeaker_oracle
######*/

#define GOSSIP_ITEM1 "I am ready to travel to your village now."

enum eRainspeaker
{
    SAY_START_IRO                       = -1571000,
    SAY_QUEST_ACCEPT_IRO                = -1571001,
    SAY_END_IRO                         = -1571002,

    QUEST_FORTUNATE_MISUNDERSTANDINGS   = 12570,
    FACTION_ESCORTEE_A                  = 774,
    FACTION_ESCORTEE_H                  = 775
};

class npc_injured_rainspeaker_oracle : public CreatureScript
{
public:
    npc_injured_rainspeaker_oracle() : CreatureScript("npc_injured_rainspeaker_oracle") { }

    struct npc_injured_rainspeaker_oracleAI : public npc_escortAI
    {
        npc_injured_rainspeaker_oracleAI(Creature* c) : npc_escortAI(c) { c_guid = c->GetGUID(); }

        uint64 c_guid;

        void Reset()
        {
            me->RestoreFaction();
            // if we will have other way to assign this to only one npc remove this part
            if (GUID_LOPART(me->GetGUID()) != 101030)
            {
                me->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
                me->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
            }
        }

        void WaypointReached(uint32 i)
        {
            Player* pPlayer = GetPlayerForEscort();

            if (!pPlayer)
                return;

            switch(i)
            {
            case 1: SetRun(); break;
            case 10:
            case 11:
            case 12:
            case 13:
            case 14:
            case 15:
            case 16:
            case 17:
            case 18:
                me->RemoveUnitMovementFlag(MOVEMENTFLAG_SWIMMING);
                me->RemoveUnitMovementFlag(MOVEMENTFLAG_JUMPING);
                me->SetSpeed(MOVE_SWIM, 0.85f, true);
                me->AddUnitMovementFlag(MOVEMENTFLAG_SWIMMING | MOVEMENTFLAG_LEVITATING);
                break;
            case 19:
                me->SetUnitMovementFlags(MOVEMENTFLAG_JUMPING);
                break;
            case 28:
                if (Player* pPlayer = GetPlayerForEscort())
                    pPlayer->GroupEventHappens(QUEST_FORTUNATE_MISUNDERSTANDINGS, me);
              //  me->RestoreFaction();
                DoScriptText(SAY_END_IRO,me);
                SetRun(false);
                break;
            }
        }

        void JustDied(Unit* /*killer*/)
        {
            if (!HasEscortState(STATE_ESCORT_ESCORTING))
                return;

            if (Player* pPlayer = GetPlayerForEscort())
            {
              if (pPlayer->GetQuestStatus(QUEST_FORTUNATE_MISUNDERSTANDINGS) != QUEST_STATUS_COMPLETE)
                pPlayer->FailQuest(QUEST_FORTUNATE_MISUNDERSTANDINGS);
            }
        }
    };

    bool OnGossipHello(Player* pPlayer, Creature* pCreature)
    {
        if (pCreature->isQuestGiver())
            pPlayer->PrepareQuestMenu(pCreature->GetGUID());

        if (pPlayer->GetQuestStatus(QUEST_FORTUNATE_MISUNDERSTANDINGS) == QUEST_STATUS_INCOMPLETE)
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);

        pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetGUID());

        return true;
    }

    bool OnGossipSelect(Player* pPlayer, Creature* pCreature, uint32 /*uiSender*/, uint32 uiAction)
    {
        pPlayer->PlayerTalkClass->ClearMenus();
        if (uiAction == GOSSIP_ACTION_INFO_DEF+1)
        {
            CAST_AI(npc_escortAI, (pCreature->AI()))->Start(true, false, pPlayer->GetGUID());
            CAST_AI(npc_escortAI, (pCreature->AI()))->SetMaxPlayerDistance(35.0f);
            pCreature->SetUnitMovementFlags(MOVEMENTFLAG_JUMPING);
            DoScriptText(SAY_START_IRO, pCreature);

            switch (pPlayer->GetTeam()){
            case ALLIANCE:
                pCreature->setFaction(FACTION_ESCORTEE_A);
                break;
            case HORDE:
                pCreature->setFaction(FACTION_ESCORTEE_H);
                break;
            }
        }
        return true;
    }

    bool OnQuestAccept(Player* /*pPlayer*/, Creature* pCreature, Quest const * /*_Quest*/)
    {
        DoScriptText(SAY_QUEST_ACCEPT_IRO, pCreature);
        return false;
    }

    CreatureAI *GetAI(Creature *creature) const
    {
        return new npc_injured_rainspeaker_oracleAI(creature);
    }
};

/*######
## npc_vekjik
######*/

#define GOSSIP_VEKJIK_ITEM1 "Shaman Vekjik, I have spoken with the big-tongues and they desire peace. I have brought this offering on their behalf."
#define GOSSIP_VEKJIK_ITEM2 "No no... I had no intentions of betraying your people. I was only defending myself. it was all a misunderstanding."

enum eVekjik
{
    GOSSIP_TEXTID_VEKJIK1       = 13137,
    GOSSIP_TEXTID_VEKJIK2       = 13138,

    SAY_TEXTID_VEKJIK1          = -1000208,

    SPELL_FREANZYHEARTS_FURY    = 51469,

    QUEST_MAKING_PEACE          = 12573
};

class npc_vekjik : public CreatureScript
{
public:
    npc_vekjik() : CreatureScript("npc_vekjik") { }

    bool OnGossipHello(Player* pPlayer, Creature* pCreature)
    {
        if (pCreature->isQuestGiver())
            pPlayer->PrepareQuestMenu(pCreature->GetGUID());

        if (pPlayer->GetQuestStatus(QUEST_MAKING_PEACE) == QUEST_STATUS_INCOMPLETE)
        {
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_VEKJIK_ITEM1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
            pPlayer->SEND_GOSSIP_MENU(GOSSIP_TEXTID_VEKJIK1, pCreature->GetGUID());
            return true;
        }

        pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetGUID());
        return true;
    }

    bool OnGossipSelect(Player* pPlayer, Creature* pCreature, uint32 /*uiSender*/, uint32 uiAction)
    {
        pPlayer->PlayerTalkClass->ClearMenus();
        switch(uiAction)
        {
            case GOSSIP_ACTION_INFO_DEF+1:
                pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_VEKJIK_ITEM2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+2);
                pPlayer->SEND_GOSSIP_MENU(GOSSIP_TEXTID_VEKJIK2, pCreature->GetGUID());
                break;
            case GOSSIP_ACTION_INFO_DEF+2:
                pPlayer->CLOSE_GOSSIP_MENU();
                DoScriptText(SAY_TEXTID_VEKJIK1, pCreature, pPlayer);
                pPlayer->AreaExploredOrEventHappens(QUEST_MAKING_PEACE);
                pCreature->CastSpell(pPlayer, SPELL_FREANZYHEARTS_FURY, false);
                break;
        }

        return true;
    }
};

/*######
## avatar_of_freya
######*/

#define GOSSIP_ITEM_AOF1 "I want to stop the Scourge as much as you do. How can I help?"
#define GOSSIP_ITEM_AOF2 "You can trust me. I am no friend of the Lich King."
#define GOSSIP_ITEM_AOF3 "I will not fail."

enum eFreya
{
    QUEST_FREYA_PACT         = 12621,

    SPELL_FREYA_CONVERSATION = 52045,

    GOSSIP_TEXTID_AVATAR1    = 13303,
    GOSSIP_TEXTID_AVATAR2    = 13304,
    GOSSIP_TEXTID_AVATAR3    = 13305
};

class npc_avatar_of_freya : public CreatureScript
{
public:
    npc_avatar_of_freya() : CreatureScript("npc_avatar_of_freya") { }

    bool OnGossipHello(Player* pPlayer, Creature* pCreature)
    {
        if (pCreature->isQuestGiver())
            pPlayer->PrepareQuestMenu(pCreature->GetGUID());

        if (pPlayer->GetQuestStatus(QUEST_FREYA_PACT) == QUEST_STATUS_INCOMPLETE)
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_AOF1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);

        pPlayer->PlayerTalkClass->SendGossipMenu(GOSSIP_TEXTID_AVATAR1, pCreature->GetGUID());
        return true;
    }

    bool OnGossipSelect(Player* pPlayer, Creature* pCreature, uint32 /*uiSender*/, uint32 uiAction)
    {
        pPlayer->PlayerTalkClass->ClearMenus();
        switch (uiAction)
        {
        case GOSSIP_ACTION_INFO_DEF+1:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_AOF2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+2);
            pPlayer->PlayerTalkClass->SendGossipMenu(GOSSIP_TEXTID_AVATAR2, pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+2:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_AOF3, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+3);
            pPlayer->PlayerTalkClass->SendGossipMenu(GOSSIP_TEXTID_AVATAR3, pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+3:
            pPlayer->CastSpell(pPlayer, SPELL_FREYA_CONVERSATION, true);
            pPlayer->CLOSE_GOSSIP_MENU();
            break;
        }
        return true;
    }
};

/*######
## npc_bushwhacker
######*/

class npc_bushwhacker : public CreatureScript
{
public:
    npc_bushwhacker() : CreatureScript("npc_bushwhacker") { }

    struct npc_bushwhackerAI : public ScriptedAI
    {
        npc_bushwhackerAI(Creature* pCreature) : ScriptedAI(pCreature)
        {
            MoveToSummoner();
        }

        void MoveToSummoner()
        {
            if (me->isSummon())
                if (Unit* pSummoner = CAST_SUM(me)->GetSummoner())
                    if (pSummoner)
                        me->GetMotionMaster()->MovePoint(0,pSummoner->GetPositionX(),pSummoner->GetPositionY(),pSummoner->GetPositionZ());
        }

        void UpdateAI(const uint32 /*uiDiff*/)
        {
            if (!UpdateVictim())
                return;

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI *GetAI(Creature *creature) const
    {
        return new npc_bushwhackerAI(creature);
    }
};

/*######
## npc_engineer_helice
######*/

enum eEnums
{
    SPELL_EXPLODE_CRYSTAL       = 62487,
    SPELL_FLAMES                = 64561,

    SAY_WP_7                    = -1800047,
    SAY_WP_6                    = -1800048,
    SAY_WP_5                    = -1800049,
    SAY_WP_4                    = -1800050,
    SAY_WP_3                    = -1800051,
    SAY_WP_2                    = -1800052,
    SAY_WP_1                    = -1800053,

    QUEST_DISASTER              = 12688
};

class npc_engineer_helice : public CreatureScript
{
public:
    npc_engineer_helice() : CreatureScript("npc_engineer_helice") { }

    struct npc_engineer_heliceAI : public npc_escortAI
    {
        npc_engineer_heliceAI(Creature* pCreature) : npc_escortAI(pCreature) { }

        uint32 m_uiChatTimer;

        void WaypointReached(uint32 i)
        {
            Player* pPlayer = GetPlayerForEscort();
            switch (i)
            {
                case 0:
                    DoScriptText(SAY_WP_2, me);
                    break;
                case 1:
                    DoScriptText(SAY_WP_3, me);
                    me->CastSpell(5918.33f, 5372.91f, -98.770f, SPELL_EXPLODE_CRYSTAL, true);
                    me->SummonGameObject(184743, 5918.33f, 5372.91f, -98.770f, 0, 0, 0, 0, 0, TEMPSUMMON_MANUAL_DESPAWN);     //approx 3 to 4 seconds
                    me->HandleEmoteCommand(EMOTE_ONESHOT_LAUGH);
                    break;
                case 2:
                    DoScriptText(SAY_WP_4, me);
                    break;
                case 7:
                    DoScriptText(SAY_WP_5, me);
                    break;
                case 8:
                    me->CastSpell(5887.37f, 5379.39f, -91.289f, SPELL_EXPLODE_CRYSTAL, true);
                    me->SummonGameObject(184743, 5887.37f, 5379.39f, -91.289f, 0, 0, 0, 0, 0, TEMPSUMMON_MANUAL_DESPAWN);      //approx 3 to 4 seconds
                    me->HandleEmoteCommand(EMOTE_ONESHOT_LAUGH);
                    break;
                case 9:
                    DoScriptText(SAY_WP_6, me);
                    break;
                case 13:
                    if (pPlayer)
                    {
                        pPlayer->GroupEventHappens(QUEST_DISASTER, me);
                        DoScriptText(SAY_WP_7, me);
                    }
                    break;
            }
        }

        void Reset()
        {
            m_uiChatTimer = 4000;
        }
        void JustDied(Unit* /*pKiller*/)
        {
            Player* pPlayer = GetPlayerForEscort();
            if (HasEscortState(STATE_ESCORT_ESCORTING))
            {
                if (pPlayer)
                    pPlayer->FailQuest(QUEST_DISASTER);
            }
        }

        void UpdateAI(const uint32 uiDiff)
        {
            npc_escortAI::UpdateAI(uiDiff);

            if (HasEscortState(STATE_ESCORT_ESCORTING))
            {
                if (m_uiChatTimer <= uiDiff)
                {
                    m_uiChatTimer = 12000;
                }
                else
                    m_uiChatTimer -= uiDiff;
            }
        }
    };

    CreatureAI *GetAI(Creature *creature) const
    {
        return new npc_engineer_heliceAI(creature);
    }

    bool OnQuestAccept(Player* pPlayer, Creature* pCreature, const Quest* pQuest)
    {
        if (pQuest->GetQuestId() == QUEST_DISASTER)
        {
            if (npc_engineer_heliceAI* pEscortAI = CAST_AI(npc_engineer_helice::npc_engineer_heliceAI, pCreature->AI()))
            {
                pCreature->GetMotionMaster()->MoveJumpTo(0, 0.4f, 0.4f);
                pCreature->setFaction(113);

                pEscortAI->Start(false, false, pPlayer->GetGUID());
                DoScriptText(SAY_WP_1, pCreature);
            }
        }
        return true;
    }
};

/*######
## npc_adventurous_dwarf
######*/

#define GOSSIP_OPTION_ORANGE    "Can you spare an orange?"
#define GOSSIP_OPTION_BANANAS   "Have a spare bunch of bananas?"
#define GOSSIP_OPTION_PAPAYA    "I could really use a papaya."

enum eAdventurousDwarf
{
    QUEST_12634         = 12634,

    ITEM_BANANAS        = 38653,
    ITEM_PAPAYA         = 38655,
    ITEM_ORANGE         = 38656,

    SPELL_ADD_ORANGE    = 52073,
    SPELL_ADD_BANANAS   = 52074,
    SPELL_ADD_PAPAYA    = 52076,

    GOSSIP_MENU_DWARF   = 13307,

    SAY_DWARF_OUCH      = -1571042,
    SAY_DWARF_HELP      = -1571043
};

class npc_adventurous_dwarf : public CreatureScript
{
public:
    npc_adventurous_dwarf() : CreatureScript("npc_adventurous_dwarf") { }

    CreatureAI *GetAI(Creature *pCreature) const
    {
        DoScriptText(SAY_DWARF_OUCH, pCreature);
        return NULL;
    }

    bool OnGossipHello(Player *pPlayer, Creature *pCreature)
    {
        if (pPlayer->GetQuestStatus(QUEST_12634) != QUEST_STATUS_INCOMPLETE)
            return false;

        if (pPlayer->GetItemCount(ITEM_ORANGE) < 1)
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_OPTION_ORANGE, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);

        if (pPlayer->GetItemCount(ITEM_BANANAS) < 2)
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_OPTION_BANANAS, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);

        if (pPlayer->GetItemCount(ITEM_PAPAYA) < 1)
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_OPTION_PAPAYA, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);

        pPlayer->PlayerTalkClass->SendGossipMenu(GOSSIP_MENU_DWARF, pCreature->GetGUID());
        return true;
    }

    bool OnGossipSelect(Player *pPlayer, Creature *pCreature, uint32 /*uiSender*/, uint32 uiAction)
    {
        pPlayer->PlayerTalkClass->ClearMenus();
        uint32 spellId = 0;
        switch (uiAction)
        {
            case GOSSIP_ACTION_INFO_DEF + 1: spellId = SPELL_ADD_ORANGE;     break;
            case GOSSIP_ACTION_INFO_DEF + 2: spellId = SPELL_ADD_BANANAS;    break;
            case GOSSIP_ACTION_INFO_DEF + 3: spellId = SPELL_ADD_PAPAYA;     break;
        }
        if (spellId)
            pPlayer->CastSpell(pPlayer, spellId, true);
        DoScriptText(SAY_DWARF_HELP, pCreature);
        pCreature->ForcedDespawn();
        return true;
    }
};

enum eEnums_oracle_frezy
{
    NPC_ORACLE       = 28667,
    NPC_FREZYNE      = 28668,
};

class npc_oracle_frezy : public CreatureScript
{
public:
    npc_oracle_frezy() : CreatureScript("npc_oracle_frezy") { }

    struct npc_oracle_frezyAI : public ScriptedAI
    {
        npc_oracle_frezyAI(Creature* pCreature) : ScriptedAI(pCreature) 
        { 
            entry = pCreature->GetEntry();
            me->setFaction(14);
        }

        uint32 entry;

        void JustDied(Unit* /*pKiller*/)
        {
            if (Unit* pTarget = me->FindNearestCreature(me->GetEntry() == NPC_ORACLE ? NPC_FREZYNE : NPC_ORACLE, 100.0f))
            {            
                pTarget->setFaction(35);
                pTarget->CombatStop();
            }
        }
    };

    CreatureAI *GetAI(Creature *creature) const
    {
        return new npc_oracle_frezyAI(creature);
    }
};


void AddSC_sholazar_basin()
{
    new npc_injured_rainspeaker_oracle();
    new npc_vekjik();
    new npc_avatar_of_freya();
    new npc_bushwhacker();
    new npc_engineer_helice();
    new npc_adventurous_dwarf();
    new npc_oracle_frezy();
}
