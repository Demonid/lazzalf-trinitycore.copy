#include "ScriptPCH.h"
#include "Group.h"

enum Spells
{
    SPELL_ARC_LIGHTNING                         = 52921,
    SPELL_LIGHTNING_NOVA_N                      = 52960,
    SPELL_LIGHTNING_NOVA_H                      = 59835,

    SPELL_SCORCH_10                             = 62546,
    SPELL_SCORCH_25                             = 63474,

    SPELL_FLAME_JETS_10                         = 62680,
    SPELL_FLAME_JETS_25                         = 63472,
};

enum ePhases
{
    PHASE_0 = 0,
    PHASE_1,
    PHASE_2,
    PHASE_3,
    PHASE_4,
    PHASE_5,
};

#define GOSSIP_ITEM1 "I am ready"
#define MSG_START "Are you ready?"

#define MSG_NOT_ENOUGHT_PLAYER "Necessiti almeno di 10 player in gruppo per iniziare questo allenamento"

#define BOSS_KING_DUMMY 1
#define MIN_GROUP_SIZE 10

class boss_king_dummy : public CreatureScript
{
    public:
        boss_king_dummy(): CreatureScript("boss_king_dummy") {}

    typedef std::list<uint64> PlayerVct;
    PlayerVct player_vct;

    bool OnGossipHello(Player* pPlayer, Creature* pCreature)
    {
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);

        pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetGUID());
        return true;
    }

    bool OnGossipSelect(Player* pPlayer, Creature* pCreature, uint32 /*uiSender*/, uint32 uiAction)
    {
        pPlayer->PlayerTalkClass->ClearMenus();
        if (uiAction == GOSSIP_ACTION_INFO_DEF+1)
        {
            if (Group * grp = pPlayer->GetGroup())
            {
                if (grp->GetMembersCount() >= MIN_GROUP_SIZE)
                {
                    player_vct.clear();
                    for (GroupReference *itr = grp->GetFirstMember(); itr != NULL; itr = itr->next())
                    {
                        Player* member = itr->getSource();
                        if (member)
                            player_vct.push_back(member->GetGUID());
                        //sObjectMgr->GetPlayer(GUID);
                    }
                    UpdateStats(pCreature, grp->GetMembersCount());
                    pCreature->setFaction(14);
                    pCreature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                    return true;
                }
            } 
            pCreature->MonsterSay(MSG_NOT_ENOUGHT_PLAYER, LANG_UNIVERSAL, 0);
        }
        return true;
    }

    void UpdateStats(Creature* pCreature, uint32 player_number)
    {
        pCreature->SetMaxHealth(player_number * 1000000);
    }

    struct boss_king_dummyAI : public BossAI
    {
        boss_king_dummyAI(Creature *pCreature) : BossAI(pCreature, BOSS_KING_DUMMY)
        {

        }

        uint32 uPhase;

        typedef std::list<uint64> SummonVct;
        SummonVct summons;

        void Reset()
        {
            _Reset();
            me->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
            me->setFaction(35);
            uPhase = PHASE_0;
            summons.clear();     
        }

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            switch(uPhase)
            {
                case PHASE_0:
                    {
                        me->MonsterYell(MSG_START, LANG_UNIVERSAL, 0);
                        uPhase = PHASE_1;
                        return;
                    }
                    break;
                case PHASE_1:
                    {

                    }
                    break;
                case PHASE_2:
                    {

                    }
                    break;
                case PHASE_3:
                    {

                    }
                    break;
                case PHASE_4:
                    {

                    }
                    break;
                case PHASE_5:
                    {

                    }
                    break;
                default:
                    break;
            }

            DoMeleeAttackIfReady();
        }

        void JustSummoned(Creature *summon)
        {
            summons.push_back(summon->GetGUID());
        }

        void DamageTaken(Unit *attacker, uint32 &damage)
        {

        }

        void JustDie(Unit *attacker, uint32 &damage)
        {

        }
    };

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new boss_king_dummyAI (pCreature);
    };
};

void AddSC_boss_king_dummy()
{
    new boss_king_dummy();
}

// Scorch
/*if (Unit *pTarget = me->getVictim())
    me->SummonCreature(GROUND_SCORCH, pTarget->GetPositionX(), pTarget->GetPositionY(), pTarget->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN, 45000);
DoCast(RAID_MODE(SPELL_SCORCH_10, SPELL_SCORCH_25));
events.ScheduleEvent(EVENT_SCORCH, 25000);

// Flame Jets
DoCastAOE(RAID_MODE(SPELL_FLAME_JETS_10, SPELL_FLAME_JETS_25));
events.ScheduleEvent(EVENT_JET, urand(35000,40000));
break;*/