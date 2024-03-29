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
SDName: Icecrown
SD%Complete: 100
SDComment: Quest support: 12807
SDCategory: Icecrown
EndScriptData */

/* ContentData
npc_arete
EndContentData */

#include "ScriptPCH.h"

/*######
## npc_arete
######*/

#define GOSSIP_ARETE_ITEM1 "Lord-Commander, I would hear your tale."
#define GOSSIP_ARETE_ITEM2 "<You nod slightly but do not complete the motion as the Lord-Commander narrows his eyes before he continues.>"
#define GOSSIP_ARETE_ITEM3 "I thought that they now called themselves the Scarlet Onslaught?"
#define GOSSIP_ARETE_ITEM4 "Where did the grand admiral go?"
#define GOSSIP_ARETE_ITEM5 "That's fine. When do I start?"
#define GOSSIP_ARETE_ITEM6 "Let's finish this!"
#define GOSSIP_ARETE_ITEM7 "That's quite a tale, Lord-Commander."

enum eArete
{
    GOSSIP_TEXTID_ARETE1        = 13525,
    GOSSIP_TEXTID_ARETE2        = 13526,
    GOSSIP_TEXTID_ARETE3        = 13527,
    GOSSIP_TEXTID_ARETE4        = 13528,
    GOSSIP_TEXTID_ARETE5        = 13529,
    GOSSIP_TEXTID_ARETE6        = 13530,
    GOSSIP_TEXTID_ARETE7        = 13531,

    QUEST_THE_STORY_THUS_FAR    = 12807
};

class npc_arete : public CreatureScript
{
public:
    npc_arete() : CreatureScript("npc_arete") { }

    bool OnGossipHello(Player* pPlayer, Creature* pCreature)
    {
        if (pCreature->isQuestGiver())
            pPlayer->PrepareQuestMenu(pCreature->GetGUID());

        if (pPlayer->GetQuestStatus(QUEST_THE_STORY_THUS_FAR) == QUEST_STATUS_INCOMPLETE)
        {
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ARETE_ITEM1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
            pPlayer->SEND_GOSSIP_MENU(GOSSIP_TEXTID_ARETE1, pCreature->GetGUID());
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
                pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ARETE_ITEM2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
                pPlayer->SEND_GOSSIP_MENU(GOSSIP_TEXTID_ARETE2, pCreature->GetGUID());
                break;
            case GOSSIP_ACTION_INFO_DEF+2:
                pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ARETE_ITEM3, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
                pPlayer->SEND_GOSSIP_MENU(GOSSIP_TEXTID_ARETE3, pCreature->GetGUID());
                break;
            case GOSSIP_ACTION_INFO_DEF+3:
                pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ARETE_ITEM4, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 4);
                pPlayer->SEND_GOSSIP_MENU(GOSSIP_TEXTID_ARETE4, pCreature->GetGUID());
                break;
            case GOSSIP_ACTION_INFO_DEF+4:
                pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ARETE_ITEM5, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 5);
                pPlayer->SEND_GOSSIP_MENU(GOSSIP_TEXTID_ARETE5, pCreature->GetGUID());
                break;
            case GOSSIP_ACTION_INFO_DEF+5:
                pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ARETE_ITEM6, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 6);
                pPlayer->SEND_GOSSIP_MENU(GOSSIP_TEXTID_ARETE6, pCreature->GetGUID());
                break;
            case GOSSIP_ACTION_INFO_DEF+6:
                pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ARETE_ITEM7, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 7);
                pPlayer->SEND_GOSSIP_MENU(GOSSIP_TEXTID_ARETE7, pCreature->GetGUID());
                break;
            case GOSSIP_ACTION_INFO_DEF+7:
                pPlayer->CLOSE_GOSSIP_MENU();
                pPlayer->AreaExploredOrEventHappens(QUEST_THE_STORY_THUS_FAR);
                break;
        }

        return true;
    }
};

/*######
## npc_dame_evniki_kapsalis
######*/

enum eDameEnvikiKapsalis
{
    TITLE_CRUSADER    = 123
};

class npc_dame_evniki_kapsalis : public CreatureScript
{
public:
    npc_dame_evniki_kapsalis() : CreatureScript("npc_dame_evniki_kapsalis") { }

    bool OnGossipHello(Player* pPlayer, Creature* pCreature)
    {
        if (pPlayer->HasTitle(TITLE_CRUSADER))
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, GOSSIP_TEXT_BROWSE_GOODS, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_TRADE);

        pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetGUID());
        return true;
    }

    bool OnGossipSelect(Player* pPlayer, Creature* pCreature, uint32 /*uiSender*/, uint32 uiAction)
    {
        pPlayer->PlayerTalkClass->ClearMenus();
        if (uiAction == GOSSIP_ACTION_TRADE)
            pPlayer->SEND_VENDORLIST(pCreature->GetGUID());
        return true;
    }
};

/*######
## npc_squire_david
######*/

enum eSquireDavid
{
    QUEST_THE_ASPIRANT_S_CHALLENGE_H                    = 13680,
    QUEST_THE_ASPIRANT_S_CHALLENGE_A                    = 13679,

    NPC_ARGENT_VALIANT                                  = 33448,

    GOSSIP_TEXTID_SQUIRE                                = 14407
};

#define GOSSIP_SQUIRE_ITEM_1 "I am ready to fight!"
#define GOSSIP_SQUIRE_ITEM_2 "How do the Argent Crusader raiders fight?"

class npc_squire_david : public CreatureScript
{
public:
    npc_squire_david() : CreatureScript("npc_squire_david") { }

    bool OnGossipHello(Player* pPlayer, Creature* pCreature)
    {
        if (pPlayer->GetQuestStatus(QUEST_THE_ASPIRANT_S_CHALLENGE_H) == QUEST_STATUS_INCOMPLETE ||
            pPlayer->GetQuestStatus(QUEST_THE_ASPIRANT_S_CHALLENGE_A) == QUEST_STATUS_INCOMPLETE)//We need more info about it.
        {
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_SQUIRE_ITEM_1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_SQUIRE_ITEM_2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+2);
        }

        pPlayer->SEND_GOSSIP_MENU(GOSSIP_TEXTID_SQUIRE, pCreature->GetGUID());
        return true;
    }

    bool OnGossipSelect(Player* pPlayer, Creature* pCreature, uint32 /*uiSender*/, uint32 uiAction)
    {
        pPlayer->PlayerTalkClass->ClearMenus();
        if (uiAction == GOSSIP_ACTION_INFO_DEF+1)
        {
            pPlayer->CLOSE_GOSSIP_MENU();
            pCreature->SummonCreature(NPC_ARGENT_VALIANT,8575.451f,952.472f,547.554f,0.38f);
        }
        return true;
    }
};

/*######
## npc_squire_danny
######*/

enum eSquireDanny
{
    QUEST_THE_VALIANT_S_CHALLENGE_HORDE_UNDERCITY      = 13729,
    QUEST_THE_VALIANT_S_CHALLENGE_HORDE_SENJIN         = 13727,
    QUEST_THE_VALIANT_S_CHALLENGE_HORDE_THUNDERBLUFF   = 13728,
    QUEST_THE_VALIANT_S_CHALLENGE_HORDE_SILVERMOON     = 13731,
    QUEST_THE_VALIANT_S_CHALLENGE_HORDE_ORGRIMMAR      = 13726,
    QUEST_THE_VALIANT_S_CHALLENGE_ALLIANCE_DARNASSUS   = 13725,
    QUEST_THE_VALIANT_S_CHALLENGE_ALLIANCE_IRONFORGE   = 13713,
    QUEST_THE_VALIANT_S_CHALLENGE_ALLIANCE_GNOMEREGAN  = 13723,
    QUEST_THE_VALIANT_S_CHALLENGE_ALLIANCE_EXODAR      = 13724,
    QUEST_THE_VALIANT_S_CHALLENGE_ALLIANCE_STORMWIND   = 13699,

    NPC_ARGENT_CHAMPION                                = 33707,

    GOSSIP_TEXTID_SQUIRE_DANNY                         = 14407
};

class npc_squire_danny : public CreatureScript
{
public:
    npc_squire_danny() : CreatureScript("npc_squire_danny") { }

    bool OnGossipHello(Player* pPlayer, Creature* pCreature)
    {
        if (pPlayer->GetQuestStatus(QUEST_THE_VALIANT_S_CHALLENGE_HORDE_UNDERCITY) == QUEST_STATUS_INCOMPLETE
            || pPlayer->GetQuestStatus(QUEST_THE_VALIANT_S_CHALLENGE_HORDE_SENJIN) == QUEST_STATUS_INCOMPLETE
            || pPlayer->GetQuestStatus(QUEST_THE_VALIANT_S_CHALLENGE_HORDE_THUNDERBLUFF) == QUEST_STATUS_INCOMPLETE
            || pPlayer->GetQuestStatus(QUEST_THE_VALIANT_S_CHALLENGE_HORDE_SILVERMOON) == QUEST_STATUS_INCOMPLETE
            || pPlayer->GetQuestStatus(QUEST_THE_VALIANT_S_CHALLENGE_HORDE_ORGRIMMAR) == QUEST_STATUS_INCOMPLETE
            || pPlayer->GetQuestStatus(QUEST_THE_VALIANT_S_CHALLENGE_ALLIANCE_DARNASSUS) == QUEST_STATUS_INCOMPLETE
            || pPlayer->GetQuestStatus(QUEST_THE_VALIANT_S_CHALLENGE_ALLIANCE_IRONFORGE) == QUEST_STATUS_INCOMPLETE
            || pPlayer->GetQuestStatus(QUEST_THE_VALIANT_S_CHALLENGE_ALLIANCE_GNOMEREGAN) == QUEST_STATUS_INCOMPLETE
            || pPlayer->GetQuestStatus(QUEST_THE_VALIANT_S_CHALLENGE_ALLIANCE_EXODAR) == QUEST_STATUS_INCOMPLETE
            || pPlayer->GetQuestStatus(QUEST_THE_VALIANT_S_CHALLENGE_ALLIANCE_STORMWIND) == QUEST_STATUS_INCOMPLETE) //We need more info about it.
        {
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_SQUIRE_ITEM_1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_SQUIRE_ITEM_2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+2);
        }

        pPlayer->SEND_GOSSIP_MENU(GOSSIP_TEXTID_SQUIRE_DANNY, pCreature->GetGUID());
        return true;
    }

    bool OnGossipSelect(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
    {
        if (uiAction == GOSSIP_ACTION_INFO_DEF+1)
        {
            pPlayer->CLOSE_GOSSIP_MENU();
            pCreature->SummonCreature(NPC_ARGENT_CHAMPION,8562.451f,1095.72f,556.784f,1.76f);
        }
        //else
            //pPlayer->SEND_GOSSIP_MENU(???, pCreature->GetGUID()); Missing text
        return true;
    };
};

/*######
## npc_argent_valiant
######*/

enum eArgentValiant
{
    SPELL_CHARGE                = 63010,
    SPELL_SHIELD_BREAKER        = 65147,

    NPC_ARGENT_VALIANT_CREDIT   = 24108
};

class npc_argent_valiant : public CreatureScript
{
public:
    npc_argent_valiant() : CreatureScript("npc_argent_valiant") { }

    struct npc_argent_valiantAI : public ScriptedAI
    {
        npc_argent_valiantAI(Creature* pCreature) : ScriptedAI(pCreature)
        {
            pCreature->GetMotionMaster()->MovePoint(0,8599.258f,963.951f,547.553f);
            pCreature->setFaction(35); //wrong faction in db?
        }

        uint32 uiChargeTimer;
        uint32 uiShieldBreakerTimer;

        void Reset()
        {
            uiChargeTimer = 7000;
            uiShieldBreakerTimer = 10000;
        }

        void MovementInform(uint32 uiType, uint32 /*uiId*/)
        {
            if (uiType != POINT_MOTION_TYPE)
                return;

            me->setFaction(14);
        }

        void DamageTaken(Unit* pDoneBy, uint32& uiDamage)
        {
            if (uiDamage > me->GetHealth() && pDoneBy->GetTypeId() == TYPEID_PLAYER)
            {
                uiDamage = 0;
                CAST_PLR(pDoneBy)->KilledMonsterCredit(NPC_ARGENT_VALIANT_CREDIT,0);
                me->setFaction(35);
                me->ForcedDespawn(5000);
                me->SetHomePosition(me->GetPositionX(),me->GetPositionY(),me->GetPositionZ(),me->GetOrientation());
                EnterEvadeMode();
            }
        }

        void UpdateAI(const uint32 uiDiff)
        {
            if (!UpdateVictim())
                return;

            if (uiChargeTimer <= uiDiff)
            {
                DoCastVictim(SPELL_CHARGE);
                uiChargeTimer = 7000;
            } else uiChargeTimer -= uiDiff;

            if (uiShieldBreakerTimer <= uiDiff)
            {
                DoCastVictim(SPELL_SHIELD_BREAKER);
                uiShieldBreakerTimer = 10000;
            } else uiShieldBreakerTimer -= uiDiff;

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI *GetAI(Creature *creature) const
    {
        return new npc_argent_valiantAI(creature);
    };
};

/*######
## npc_argent_tournament_post
######*/

enum eArgentTournamentPost
{
    SPELL_ROPE_BEAM                 = 63413,
    NPC_GORMOK_THE_IMPALER          = 35469,
    NPC_ICEHOWL                     = 35470
};

class npc_argent_tournament_post : public CreatureScript
{
public:
    npc_argent_tournament_post() : CreatureScript("npc_argent_tournament_post") { }

    struct npc_argent_tournament_postAI : public ScriptedAI
    {
        npc_argent_tournament_postAI(Creature* pCreature) : ScriptedAI(pCreature) {}

        void UpdateAI(const uint32 /*uiDiff*/)
        {
            if (me->IsNonMeleeSpellCasted(false))
                return;

            if (Creature* pTarget = me->FindNearestCreature(NPC_GORMOK_THE_IMPALER, 6.0f))
                DoCast(pTarget, SPELL_ROPE_BEAM);

            if (Creature* pTarget2 = me->FindNearestCreature(NPC_ICEHOWL, 6.0f))
                DoCast(pTarget2, SPELL_ROPE_BEAM);

            if (!UpdateVictim())
                return;
        }
    };

    CreatureAI *GetAI(Creature *creature) const
    {
        return new npc_argent_tournament_postAI(creature);
    };
};

/*######
## npc_alorah_and_grimmin
######*/

enum ealorah_and_grimmin
{
    SPELL_CHAIN                     = 68341,
    NPC_FJOLA_LIGHTBANE             = 36065,
    NPC_EYDIS_DARKBANE              = 36066,
    NPC_PRIESTESS_ALORAH            = 36101,
    NPC_PRIEST_GRIMMIN              = 36102
};

class npc_alorah_and_grimmin : public CreatureScript
{
public:
    npc_alorah_and_grimmin() : CreatureScript("npc_alorah_and_grimmin") { }

    struct npc_alorah_and_grimminAI : public ScriptedAI
    {
        npc_alorah_and_grimminAI(Creature* pCreature) : ScriptedAI(pCreature) {}

        bool uiCast;

        void Reset()
        {
            uiCast = false;
        }

        void UpdateAI(const uint32 /*uiDiff*/)
        {
            if (uiCast)
                return;
            uiCast = true;
            Creature* pTarget = NULL;

            switch(me->GetEntry())
            {
                case NPC_PRIESTESS_ALORAH:
                    pTarget = me->FindNearestCreature(NPC_EYDIS_DARKBANE, 10.0f);
                    break;
                case NPC_PRIEST_GRIMMIN:
                    pTarget = me->FindNearestCreature(NPC_FJOLA_LIGHTBANE, 10.0f);
                    break;
            }
            if (pTarget)
                DoCast(pTarget, SPELL_CHAIN);

            if (!UpdateVictim())
                return;
        }
    };

    CreatureAI *GetAI(Creature *creature) const
    {
        return new npc_alorah_and_grimminAI(creature);
    };
};

/*######
## npc_argent_champion
######*/

enum eArgentChampion
{
    SPELL_CHARGE_CHAMPION           = 63010,
    SPELL_SHIELD_BREAKER_CHAMPION   = 65147,

    NPC_ARGENT_CHAMPION_CREDIT      = 33708
};

class npc_argent_champion : public CreatureScript
{
public:
    npc_argent_champion() : CreatureScript("npc_argent_champion") { }

    struct npc_argent_championAI : public ScriptedAI
    {
        npc_argent_championAI(Creature* pCreature) : ScriptedAI(pCreature)
        {
            pCreature->GetMotionMaster()->MovePoint(0,8552.43f,1124.95f,556.786f);
            pCreature->setFaction(35); //wrong faction in db?
        }

        uint32 uiChargeTimer;
        uint32 uiShieldBreakerTimer;

        void Reset()
        {
            uiChargeTimer = 7000;
            uiShieldBreakerTimer = 10000;
        }

        void MovementInform(uint32 uiType, uint32 uiId)
        {
            if (uiType != POINT_MOTION_TYPE)
                return;

            me->setFaction(14);
        }

        void DamageTaken(Unit* pDoneBy, uint32& uiDamage)
        {
            if (uiDamage > me->GetHealth() && pDoneBy->GetTypeId() == TYPEID_PLAYER)
            {
                uiDamage = 0;
                CAST_PLR(pDoneBy)->KilledMonsterCredit(NPC_ARGENT_CHAMPION_CREDIT,0);
                me->setFaction(35);
                me->ForcedDespawn(5000);
                me->SetHomePosition(me->GetPositionX(),me->GetPositionY(),me->GetPositionZ(),me->GetOrientation());
                EnterEvadeMode();
            }
        }

        void UpdateAI(const uint32 uiDiff)
        {
            if (!UpdateVictim())
                return;

            if (uiChargeTimer <= uiDiff)
            {
                DoCastVictim(SPELL_CHARGE);
                uiChargeTimer = 7000;
            } else uiChargeTimer -= uiDiff;

            if (uiShieldBreakerTimer <= uiDiff)
            {
                DoCastVictim(SPELL_SHIELD_BREAKER);
                uiShieldBreakerTimer = 10000;
            } else uiShieldBreakerTimer -= uiDiff;

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_argent_championAI (pCreature);
    };
};

/*######
## lake_frog
######*/

#define MAIDEN 33220
#define WARTS_SPELL 62581
#define LIP_BALM_SPELL 62574
#define SUMMON_ASHOOD_BRAND_SPELL 62554
#define MAIDEN_SAY "thank to you, here is you Ashwood Brand !"

struct A_BLADE_FIT_FOR_A_CHAMPION_QUEST
{ 
  uint32 quest_id; 
};

A_BLADE_FIT_FOR_A_CHAMPION_QUEST new_quest[] = {13603, 13666, 13673, 13741, 13746, 13752, 13757, 13762, 13768, 13773, 13778, 13783};

class npc_lake_frog : public CreatureScript
{
    public:
    npc_lake_frog() : CreatureScript("npc_lake_frog") { }

    struct npc_lake_frogAI : public ScriptedAI
    {
        npc_lake_frogAI(Creature *c) : ScriptedAI(c) {}

        void ReceiveEmote(Player* pPlayer, uint32 emote)
        {
            switch (emote)
            {
                case TEXTEMOTE_KISS:
                    for (int i = 0; i < 12; i++)
                    {
                        if (pPlayer->GetQuestStatus(new_quest[i].quest_id) == QUEST_STATUS_INCOMPLETE && pPlayer->HasAura(LIP_BALM_SPELL) && rand()%10 == 1)
                        {
                            Unit* summon = me->SummonCreature(MAIDEN, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), 0.0f, TEMPSUMMON_TIMED_DESPAWN, 120000);
                            me->ForcedDespawn();
                            if (summon)
                            {
                                summon->CastSpell(pPlayer, SUMMON_ASHOOD_BRAND_SPELL, true, 0, 0, 0);
                                summon->MonsterSay(MAIDEN_SAY, LANG_UNIVERSAL, NULL);
                            }
                        }
                        else if (!pPlayer->HasAura(LIP_BALM_SPELL) && ((rand()%100) > 40)) 
                            me->CastSpell(pPlayer, WARTS_SPELL, true, 0, 0, 0);
                    }
                    break;
            }
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_lake_frogAI (pCreature);
    };
};

/*######
## npc_guardian_pavilion
######*/

enum eGuardianPavilion
{
    SPELL_TRESPASSER_H                            = 63987,
    AREA_SUNREAVER_PAVILION                       = 4676,

    AREA_SILVER_COVENANT_PAVILION                 = 4677,
    SPELL_TRESPASSER_A                            = 63986,
};

class npc_guardian_pavilion : public CreatureScript
{
public:
    npc_guardian_pavilion() : CreatureScript("npc_guardian_pavilion") { }

    struct npc_guardian_pavilionAI : public Scripted_NoMovementAI
    {
        npc_guardian_pavilionAI(Creature* pCreature) : Scripted_NoMovementAI(pCreature) {}

        void MoveInLineOfSight(Unit* pWho)
        {
            if (me->GetAreaId() != AREA_SUNREAVER_PAVILION && me->GetAreaId() != AREA_SILVER_COVENANT_PAVILION)
                return;

            if (!pWho || pWho->GetTypeId() != TYPEID_PLAYER || !me->IsHostileTo(pWho) || !me->isInBackInMap(pWho, 5.0f))
                return;

            if (pWho->HasAura(SPELL_TRESPASSER_H) || pWho->HasAura(SPELL_TRESPASSER_A))
                return;

            if (pWho->ToPlayer()->GetTeamId() == TEAM_ALLIANCE)
                pWho->CastSpell(pWho, SPELL_TRESPASSER_H, true);
            else
                pWho->CastSpell(pWho, SPELL_TRESPASSER_A, true);

        }
    };

    CreatureAI *GetAI(Creature *creature) const
    {
        return new npc_guardian_pavilionAI(creature);
    };
};

void AddSC_icecrown()
{
    new npc_arete();
    new npc_dame_evniki_kapsalis();
    new npc_squire_david();
    new npc_squire_danny;
    new npc_argent_valiant();
    new npc_argent_tournament_post();
    new npc_alorah_and_grimmin();
    new npc_argent_champion();
    new npc_lake_frog();
    new npc_guardian_pavilion();
}
