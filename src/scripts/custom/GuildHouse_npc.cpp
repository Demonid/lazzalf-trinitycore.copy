/*
 * Copyright (C) 2008-2010 Trinity <http://www.trinitycore.org/>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/**
 *
 * @File : GuildHouse_npc.cpp
 *
 * @Authors : Lazzalf
 *
 * @Date : 31/03/2010
 *
 * @Version : 0.1
 *
 **/

#include "ScriptedPch.h"
#include "ObjectMgr.h"
#include "ScriptMgr.h"
#include "World.h"
#include "Guild.h"
#include "GuildHouse.h"

#define SPELL_ID_PASSIVE_RESURRECTION_SICKNESS 15007

#define MSG_GOSSIP_TELE          "Teletrasportami Alla sede di gilda"
#define MSG_GOSSIP_BUY           "Crea Sede di gilda"
#define MSG_GOSSIP_SELL          "Vendi sede di gilda"
#define MSG_GOSSIP_ADD           "Compra aggiunte per la sede di Gilda"
#define MSG_GOSSIP_NEXTPAGE      "Successivo -->"
#define MSG_INCOMBAT             "Sei in combat!"
#define MSG_NOGUILDHOUSE         "La tua gilda non possiede una casa!"
#define MSG_NOFREEGH             "Purtroppo tutte le case sono occupate."
#define MSG_ALREADYHAVEGH        "La tua gilda possiede già una sede. (%s)."
#define MSG_NOTENOUGHMONEY       "Non hai abbastanza soldi per acquistare la casa. Hai bisogno di %u gold."
#define MSG_NOTENOUGHGUILDMEMBERS "Non hai abbastanza membri in gilda per acquistare la casa. Hai bisogno di %u membri."
#define MSG_GHOCCUPIED           "Sfortunatamente questa casa è già occupata."
#define MSG_CONGRATULATIONS      "Congratulazioni! La sede è stata creata."
#define MSG_SOLD                 "La gilda è stata venduta. ??? ???? %u ??????."
#define MSG_NOTINGUILD           "Non sei in nessuna gilda."

/*#define MSG_GUARD                "Acquista guardie"
#define GUARD_PRICE              5000
#define MSG_BUFFNPC              "Acquista NPC Buffatore"
#define BUFFNPC_PRICE            3000*/

#define CODE_SELL                "SELL"
#define MSG_CODEBOX_SELL         "Scrivi \"" CODE_SELL "\" in maiuscolo per vendere la casa, dopo premi accept."

#define OFFSET_GH_ID_TO_ACTION   1500
#define OFFSET_SHOWBUY_FROM      10000

#define ACTION_TELE               1001
#define ACTION_SHOW_BUYLIST       1002  //deprecated. Use (OFFSET_SHOWBUY_FROM + 0) instead
#define ACTION_SELL_GUILDHOUSE    1003
//#define ACTION_SHOW_BUYADD_LIST   1004

#define ICON_GOSSIP_BALOON       0
#define ICON_GOSSIP_WING         2
#define ICON_GOSSIP_BOOK         3
#define ICON_GOSSIP_WHEEL1       4
#define ICON_GOSSIP_WHEEL2       5
#define ICON_GOSSIP_GOLD         6
#define ICON_GOSSIP_BALOONDOTS   7
#define ICON_GOSSIP_TABARD       8
#define ICON_GOSSIP_XSWORDS      9

//#define COST_GH_BUY              10000000  //1000 g.
//#define COST_GH_SELL             5000000   //500 g.

#define GOSSIP_COUNT_MAX         10

bool isPlayerGuildLeader(Player *player)
{
    return ((player->GetRank() == 0) && (player->GetGuildId() != 0));
}

bool getGuildHouseCoords(uint32 guildId, float &x, float &y, float &z, uint32 &map)
{
    if (guildId == 0)
    {
        //if player has no guild
        return false;
    }

    QueryResult_AutoPtr result;
    result = WorldDatabase.PQuery("SELECT `x`, `y`, `z`, `map` FROM `guildhouses` WHERE `guildId` = %u", guildId);
    if(result)
    {
        Field *fields = result->Fetch();
        x = fields[0].GetFloat();
        y = fields[1].GetFloat();
        z = fields[2].GetFloat();
        map = fields[3].GetUInt32();        
        return true;
    }
    return false;
}

void teleportPlayerToGuildHouse(Player *player, Creature *_creature)
{
    if (player->GetGuildId() == 0)
    {
        //if player has no guild
        _creature->MonsterWhisper(MSG_NOTINGUILD, player->GetGUID());
        return;
    }

    if (!player->getAttackers().empty())
    {
        //if player in combat
        _creature->MonsterSay(MSG_INCOMBAT, LANG_UNIVERSAL, player->GetGUID());
        return;
    }

    float x, y, z, o;
    uint32 map;

    if (GetGuildHouseLocation(player->GetGuildId(), x, y, z, o, map))
    {
        //teleport player to the specified location
        player->TeleportTo(map, x, y, z, o);
    }
    else
        _creature->MonsterWhisper(MSG_NOGUILDHOUSE, player->GetGUID());
    /*
    if (getGuildHouseCoords(player->GetGuildId(), x, y, z, map))
    {
        //teleport player to the specified location
        player->TeleportTo(map, x, y, z, 0.0f);
    }
    else
        _creature->MonsterWhisper(MSG_NOGUILDHOUSE, player->GetGUID());*/

}

bool showBuyList(Player *player, Creature *_creature, uint32 showFromId = 0)
{
    if (!player)
        return false;

    //show not occupied guildhouses

    QueryResult_AutoPtr result;

    uint32 guildsize = 1;

    Guild *guild = objmgr.GetGuildById(player->GetGuildId());
    if (guild)
        guildsize = guild->GetMemberSize();

    result = WorldDatabase.PQuery("SELECT `id`, `comment`, `price` FROM `guildhouses` WHERE `guildId` = 0 AND (`faction` = 3 OR `faction` = %u) AND `id` > %u AND `minguildsize` <= %u ORDER BY `minguildsize` DESC LIMIT %u",
        (player->GetTeam() == HORDE)?1:0, showFromId, guildsize, GOSSIP_COUNT_MAX);

    if (result)
    {
        uint32 guildhouseId = 0;
        std::string comment = "";
        uint32 price = 0;
        do
        {
            Field *fields = result->Fetch();

            guildhouseId = fields[0].GetInt32();
            comment = fields[1].GetString();
            price = fields[2].GetUInt32();
            
            std::stringstream complete_comment;
            complete_comment << "price " << price << " - " << comment;

            //send comment as a gossip item
            //transmit guildhouseId in Action variable
            player->ADD_GOSSIP_ITEM(ICON_GOSSIP_TABARD, complete_comment.str().c_str(), GOSSIP_SENDER_MAIN,
                guildhouseId + OFFSET_GH_ID_TO_ACTION);

        } while (result->NextRow());

        if (result->GetRowCount() == GOSSIP_COUNT_MAX)
        {
            //assume that we have additional page
            //add link to next GOSSIP_COUNT_MAX items
            player->ADD_GOSSIP_ITEM(ICON_GOSSIP_BALOONDOTS, MSG_GOSSIP_NEXTPAGE, GOSSIP_SENDER_MAIN, 
                guildhouseId + OFFSET_SHOWBUY_FROM);
        }       

        player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _creature->GetGUID());

        return true;
    } 
    else
    {
        if (showFromId = 0)
        {
            //all guildhouses are occupied
            _creature->MonsterWhisper(MSG_NOFREEGH, player->GetGUID());
            player->CLOSE_GOSSIP_MENU();
        } 
        else
        {
            //this condition occurs when COUNT(guildhouses) % GOSSIP_COUNT_MAX == 0
            //just show GHs from beginning
            showBuyList(player, _creature, 0);
        }
    }

    return false;
}

bool isPlayerHasGuildhouse(Player *player, Creature *_creature, bool whisper = false)
{
    QueryResult_AutoPtr result;

    result = WorldDatabase.PQuery("SELECT `comment` FROM `guildhouses` WHERE `guildId` = %u", player->GetGuildId());

    if (result)
    {
        if (whisper)
        {
            //whisper to player "already have etc..."
            Field *fields = result->Fetch();
            char msg[200];
            sprintf(msg, MSG_ALREADYHAVEGH, fields[0].GetString());
            _creature->MonsterWhisper(msg, player->GetGUID());
        }        
        return true;
    }
    return false;
}

void buyGuildhouse(Player *player, Creature *_creature, uint32 guildhouseId)
{
    if (!player)
        return;

    if (isPlayerHasGuildhouse(player, _creature, true))
    {
        //player already have GH
        return;
    }

    QueryResult_AutoPtr result;

    
    /*uint32 guildsize = 1;

    Guild *guild = objmgr.GetGuildById(player->GetGuildId());
    if (guild)
        guildsize = guild->GetMemberSize();

    result = WorldDatabase.PQuery("SELECT `minguildsize` FROM `guildhouses` WHERE `id` = %u", guildhouseId);
    
    if (!result)
        return; 

    Field *fields = result->Fetch();
    uint32 minguildsize = fields[0].GetUInt32();

    if (guildsize < minguildsize)
    {
        char msg[100];
        sprintf(msg, MSG_NOTENOUGHGUILDMEMBERS, minguildsize);
        _creature->MonsterWhisper(msg, player->GetGUID());
        return;
    }*/

    result = WorldDatabase.PQuery("SELECT `price` FROM `guildhouses` WHERE `id` = %u AND `guildId` = 0" , guildhouseId);

    if (!result)
    {
        _creature->MonsterWhisper(MSG_GHOCCUPIED, player->GetGUID());
        return;
    }   
    
    Field *fields = result->Fetch();
    int32 price = fields[0].GetInt32();

    if (player->GetMoney() < price * 10000)
    {
        //show how much money player need to buy GH (in gold)
        char msg[200];
        sprintf(msg, MSG_NOTENOUGHMONEY, price);
        _creature->MonsterWhisper(msg, player->GetGUID());
        return;
    }

    //update DB
    result = WorldDatabase.PQuery("UPDATE `guildhouses` SET `guildId` = %u WHERE `id` = %u",
        player->GetGuildId(), guildhouseId);

    player->ModifyMoney(-(price*10000));
    _creature->MonsterSay(MSG_CONGRATULATIONS, LANG_UNIVERSAL, player->GetGUID());
    
}

void sellGuildhouse(Player *player, Creature *_creature)
{
    if (isPlayerHasGuildhouse(player, _creature))
    {
        QueryResult_AutoPtr result;

        result = WorldDatabase.PQuery("SELECT `price` FROM `guildhouses` WHERE `guildId` = %u", player->GetGuildId());

        if (!result)
            return;
    
        Field *fields = result->Fetch();
        uint32 price = fields[0].GetUInt32();

        result = WorldDatabase.PQuery("UPDATE `guildhouses` SET `guildId` = 0 WHERE `guildId` = %u", player->GetGuildId());
        
        player->ModifyMoney(price*5000);

        //display message e.g. "here your money etc."
        char msg[200];
        sprintf(msg, MSG_SOLD, price / 2);
        _creature->MonsterWhisper(msg, player->GetGUID());
    }
}

bool GossipHello_guildmaster(Player *player, Creature *_creature)
{
    player->ADD_GOSSIP_ITEM(ICON_GOSSIP_BALOON, MSG_GOSSIP_TELE, 
        GOSSIP_SENDER_MAIN, ACTION_TELE);

    if (isPlayerGuildLeader(player))
    {
        //show additional menu for guild leader
        player->ADD_GOSSIP_ITEM(ICON_GOSSIP_GOLD, MSG_GOSSIP_BUY, GOSSIP_SENDER_MAIN, ACTION_SHOW_BUYLIST);
        if (isPlayerHasGuildhouse(player, _creature))
        {
            //and additional for guildhouse owner
            player->PlayerTalkClass->GetGossipMenu().AddMenuItem(ICON_GOSSIP_GOLD, MSG_GOSSIP_SELL, GOSSIP_SENDER_MAIN, ACTION_SELL_GUILDHOUSE, MSG_CODEBOX_SELL, 0, true);
            //player->ADD_GOSSIP_ITEM(ICON_GOSSIP_GOLD, MSG_GOSSIP_ADD, GOSSIP_SENDER_MAIN, ACTION_SHOW_BUYADD_LIST);
        }
    }
    player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _creature->GetGUID());
    return true;
}


bool GossipSelect_guildmaster(Player *player, Creature *_creature, uint32 sender, uint32 action )
{
    if (sender != GOSSIP_SENDER_MAIN)
        return false;

    switch (action)
    {
        case ACTION_TELE:
            //teleport player to GH
            player->CLOSE_GOSSIP_MENU();
            teleportPlayerToGuildHouse(player, _creature);
            break;
        case ACTION_SHOW_BUYLIST:
            //show list of GHs which currently not occupied
            showBuyList(player, _creature);
            break;
        default:
            if (action > OFFSET_SHOWBUY_FROM)
            {
                showBuyList(player, _creature, action - OFFSET_SHOWBUY_FROM);
            } 
            else if (action > OFFSET_GH_ID_TO_ACTION)
            {
                //player clicked on buy list
                player->CLOSE_GOSSIP_MENU();
                //get guildhouseId from action
                //guildhouseId = action - OFFSET_GH_ID_TO_ACTION
                buyGuildhouse(player, _creature, action - OFFSET_GH_ID_TO_ACTION);
            }
            break;
    }
    
    return true;
}

bool GossipSelectWithCode_guildmaster( Player *player, Creature *_creature,
                                      uint32 sender, uint32 action, const char* sCode )
{
    if(sender == GOSSIP_SENDER_MAIN)
    {
        if(action == ACTION_SELL_GUILDHOUSE)
        {
            int i = -1;
            try
            {
                //compare code
                if (strlen(sCode) + 1 == sizeof CODE_SELL)
                    i = strcmp(CODE_SELL, sCode);

            } catch(char *str) {error_db_log(str);}

            if (i == 0)
            {
                //right code
                sellGuildhouse(player, _creature);
            }
            player->CLOSE_GOSSIP_MENU();
            return true;
        }
    }
    return false;
}

/*########
# guild_guard
#########*/
#define SAY_AGGRO "Tu non fai parte di questa gilda!"
struct guild_guardAI : public ScriptedAI
{
    guild_guardAI(Creature *c) : ScriptedAI(c) {}

    uint32 Check_Timer;

    void Reset()
    {
        Check_Timer = 1000;
    }

    void EnterCombat(Unit* who)
    {
        m_creature->MonsterYell(SAY_AGGRO, LANG_UNIVERSAL, 0);
    }

    void AttackStart(Unit *who) 
    {
        if (!who)
            return;

        if (who->GetTypeId() != TYPEID_PLAYER)
            return;

        uint32 guild =((Player*)who)->GetGuildId();

        uint32 guardguild = m_creature->GetGuildByGuard();

        if ( guardguild && guild!=guardguild && m_creature->Attack(who, true) )
        {
            m_creature->AddThreat(who, 0.0f);

            if (!m_creature->isInCombat())
            {
                m_creature->SetInCombatWith(who);                
            }
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!UpdateVictim())
            return;
        
        if (Check_Timer <= uiDiff)
        {
            m_creature->Kill(m_creature->getVictim());
        } else Check_Timer -= uiDiff;

        DoMeleeAttackIfReady();
    }           
};

CreatureAI* GetAI_guild_guardAI(Creature *_Creature)
{
    return new guild_guardAI(_Creature);
}

bool GossipHello_buffnpc(Player *player, Creature *_Creature)
{
    //player->SetTaxiCheater(true);

    // Main Menu for Alliance
    if ( player->GetTeam() == ALLIANCE )
    {
        player->ADD_GOSSIP_ITEM( 5, "Remove Res Sickness"                           , GOSSIP_SENDER_MAIN, 1180);
        //player->ADD_GOSSIP_ITEM( 5, "Give me gold"                                , GOSSIP_SENDER_MAIN, 1185);
        //player->ADD_GOSSIP_ITEM( 5, "Give me Soul Shards"                         , GOSSIP_SENDER_MAIN, 1190);
        player->ADD_GOSSIP_ITEM( 5, "Heal me please"                                , GOSSIP_SENDER_MAIN, 1195);
        player->ADD_GOSSIP_ITEM( 5, "Ritual of Souls please"                        , GOSSIP_SENDER_MAIN, 1200);
        player->ADD_GOSSIP_ITEM( 5, "Table please"                                  , GOSSIP_SENDER_MAIN, 1205);                     
        player->ADD_GOSSIP_ITEM( 5, "Buff me Arcane Intelect"                       , GOSSIP_SENDER_MAIN, 1210);
        player->ADD_GOSSIP_ITEM( 5, "Buff me Mark of the Wild"                      , GOSSIP_SENDER_MAIN, 1215);
        player->ADD_GOSSIP_ITEM( 5, "Buff me Thorns"                                , GOSSIP_SENDER_MAIN, 1220);
        player->ADD_GOSSIP_ITEM( 5, "Buff me Greater Blessing of Sanctuary"         , GOSSIP_SENDER_MAIN, 1225);
        player->ADD_GOSSIP_ITEM( 5, "Buff me Greater Blessing of Might"             , GOSSIP_SENDER_MAIN, 1230);
        //player->ADD_GOSSIP_ITEM( 5, "Buff me Greater Blessing of Light"             , GOSSIP_SENDER_MAIN, 1235);
        player->ADD_GOSSIP_ITEM( 5, "Buff me Greater Blessing of Wisdom"            , GOSSIP_SENDER_MAIN, 1240);
        player->ADD_GOSSIP_ITEM( 5, "Buff me Greater Blessing of Kings"             , GOSSIP_SENDER_MAIN, 1245);
        player->ADD_GOSSIP_ITEM( 5, "Buff me Divine Spirit"                         , GOSSIP_SENDER_MAIN, 1250);
        player->ADD_GOSSIP_ITEM( 5, "Buff me Shadow Protection"                     , GOSSIP_SENDER_MAIN, 1251);
        player->ADD_GOSSIP_ITEM( 5, "Buff me Power Word: Fortitude"                 , GOSSIP_SENDER_MAIN, 1252);
    }
    else // Main Menu for Horde
    {
        player->ADD_GOSSIP_ITEM( 5, "Remove Res Sickness"                           , GOSSIP_SENDER_MAIN, 1180);
        //player->ADD_GOSSIP_ITEM( 5, "Give me gold"                                , GOSSIP_SENDER_MAIN, 1185);
        //player->ADD_GOSSIP_ITEM( 5, "Give me Soul Shards"                         , GOSSIP_SENDER_MAIN, 1190);
        player->ADD_GOSSIP_ITEM( 5, "Heal me please"                                , GOSSIP_SENDER_MAIN, 1195);
        player->ADD_GOSSIP_ITEM( 5, "Ritual of Souls please"                        , GOSSIP_SENDER_MAIN, 1200);
        player->ADD_GOSSIP_ITEM( 5, "Table please"                                  , GOSSIP_SENDER_MAIN, 1205);
        player->ADD_GOSSIP_ITEM( 5, "Buff me Arcane Intelect"                       , GOSSIP_SENDER_MAIN, 1210);
        player->ADD_GOSSIP_ITEM( 5, "Buff me Mark of the Wild"                      , GOSSIP_SENDER_MAIN, 1215);
        player->ADD_GOSSIP_ITEM( 5, "Buff me Thorns"                                , GOSSIP_SENDER_MAIN, 1220);
        player->ADD_GOSSIP_ITEM( 5, "Buff me Greater Blessing of Sanctuary"         , GOSSIP_SENDER_MAIN, 1225);
        player->ADD_GOSSIP_ITEM( 5, "Buff me Greater Blessing of Might"             , GOSSIP_SENDER_MAIN, 1230);
        //player->ADD_GOSSIP_ITEM( 5, "Buff me Greater Blessing of Light"             , GOSSIP_SENDER_MAIN, 1235);
        player->ADD_GOSSIP_ITEM( 5, "Buff me Greater Blessing of Wisdom"            , GOSSIP_SENDER_MAIN, 1240);
        player->ADD_GOSSIP_ITEM( 5, "Buff me Greater Blessing of Kings"             , GOSSIP_SENDER_MAIN, 1245);
        player->ADD_GOSSIP_ITEM( 5, "Buff me Divine Spirit"                         , GOSSIP_SENDER_MAIN, 1250);
        player->ADD_GOSSIP_ITEM( 5, "Buff me Shadow Protection"                     , GOSSIP_SENDER_MAIN, 1251);
        player->ADD_GOSSIP_ITEM( 5, "Buff me Power Word: Fortitude"                 , GOSSIP_SENDER_MAIN, 1252);
    }

    player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,_Creature->GetGUID());

return true;
}

void SendDefaultMenu_buffnpc(Player *player, Creature *_Creature, uint32 action )
{
    // Not allow in combat
    if(!player->getAttackers().empty())
    {
        player->CLOSE_GOSSIP_MENU();
        _Creature->MonsterSay(MSG_INCOMBAT, LANG_UNIVERSAL, player->GetGUID());
        return;
    }

    switch(action)
    {
        case 1180://Remove Res Sickness
            if(!player->HasAura(SPELL_ID_PASSIVE_RESURRECTION_SICKNESS,0)) 
            {
                GossipHello_buffnpc(player, _Creature);
                return;
            }

            _Creature->CastSpell(player,38588,false); // Healing effect
            player->RemoveAurasDueToSpell(SPELL_ID_PASSIVE_RESURRECTION_SICKNESS);
            player->CLOSE_GOSSIP_MENU();
            break;

        /*case 1185://Give me Gold
            _Creature->CastSpell(player,46642,false); // 5000 gold
        break;*/

        /*case 1190://Give me Soul Shards
            player->CastSpell(player,24827,false);
        break;*/

        case 1195: // Heal me please
            player->CLOSE_GOSSIP_MENU();
            _Creature->CastSpell(player,38588,false);
            break;
        case 1200: // Ritual of Souls please
            player->CLOSE_GOSSIP_MENU();
            player->CastSpell(player,29886,false);
            break;
        case 1205: // Table please
            player->CLOSE_GOSSIP_MENU();
            player->CastSpell(player,43985,false);
            break;
        case 1210: // Buff me Arcane Intelect
            player->CLOSE_GOSSIP_MENU();
            _Creature->CastSpell(player,27126,false);
            break;
        case 1215: // Buff me Mark of the Wild
            player->CLOSE_GOSSIP_MENU();
            _Creature->CastSpell(player,26990,false);
            break;
        case 1220: // Buff me Thorns
            player->CLOSE_GOSSIP_MENU();
            _Creature->CastSpell(player,26992,false);
            break;
        case 1225: // Buff me Greater Blessing of Sanctuary
            player->CLOSE_GOSSIP_MENU();
            _Creature->CastSpell(player,25899,false);
            break;
        case 1230: // Buff me Greater Blessing of Might
            player->CLOSE_GOSSIP_MENU();
            _Creature->CastSpell(player,48934,false);
            break;
        case 1235: // Buff me Greater Blessing of Light
            player->CLOSE_GOSSIP_MENU();
            _Creature->CastSpell(player,27145,false);
            break;
        case 1240: // Buff me Greater Blessing of Wisdom
            player->CLOSE_GOSSIP_MENU();
            _Creature->CastSpell(player,48938,false);
            break;
        case 1245: // Buff me Greater Blessing of Kings
            player->CLOSE_GOSSIP_MENU();
            _Creature->CastSpell(player,25898,false);
            break;
        case 1250: // Buff me Divine Spirit
            player->CLOSE_GOSSIP_MENU();
            _Creature->CastSpell(player,48073,false);
            break;
        case 1251: // Buff me Shadow Protection
            player->CLOSE_GOSSIP_MENU();
            _Creature->CastSpell(player,48169,false);
            break;
        case 1252: // Buff me Power Word: Fortitude
            player->CLOSE_GOSSIP_MENU();
            _Creature->CastSpell(player,48161,false);
            break;
    }
}

bool GossipSelect_buffnpc(Player *player, Creature *_Creature, uint32 sender, uint32 action)
{
    // Main menu
    if (sender == GOSSIP_SENDER_MAIN)
    SendDefaultMenu_buffnpc( player, _Creature, action );

return true;
}

void AddSC_guildhouse_npcs()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "guildmaster";
    newscript->pGossipHello = &GossipHello_guildmaster;
    newscript->pGossipSelect = &GossipSelect_guildmaster;
    newscript->pGossipSelectWithCode =  &GossipSelectWithCode_guildmaster;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "guild_guard";
    newscript->GetAI = &GetAI_guild_guardAI;
    newscript->RegisterSelf();

    newscript->Name="buffnpc";
    newscript->pGossipHello = &GossipHello_buffnpc;
    newscript->pGossipSelect = &GossipSelect_buffnpc;
    newscript->RegisterSelf();
}