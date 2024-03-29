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
SDName: Flame Leviathan
SDAuthor: PrinceCreed
SD%Complete: 65
EndScriptData */

#include "ScriptPCH.h"
#include "ScriptedEscortAI.h"
#include "ulduar.h"
#include "Vehicle.h"

enum Spells
{
    SPELL_PURSUED                               = 62374,
    SPELL_GATHERING_SPEED                       = 62375,
    SPELL_BATTERING_RAM                         = 62376,
    SPELL_FLAME_VENTS                           = 62396,
    SPELL_MISSILE_BARRAGE                       = 62400,
    SPELL_SYSTEMS_SHUTDOWN                      = 62475,

    SPELL_FLAME_CANNON                          = 62395,
    //SPELL_FLAME_CANNON                        = 64692 trigger the same spell
    SPELL_AUTO_REPAIR                           = 62705,

    SPELL_OVERLOAD_CIRCUIT                      = 62399,
    SPELL_SEARING_FLAME                         = 62402,
    SPELL_BLAZE                                 = 62292,
    SPELL_SMOKE_TRAIL                           = 63575,
    SPELL_ELECTROSHOCK                          = 62522,
    SPELL_STUN                                  = 9032,
    
    // Ulduar Colossus spell
    SPELL_GROUND_SLAM                           = 62625,
};

enum Mobs
{
    MOB_MECHANOLIFT                             = 33214,
    MOB_LIQUID                                  = 33189,
    MOB_CONTAINER                               = 33218,
    MOB_COLOSSUS                                = 33240,
};

#define ACTION_VEHICLE_RESPAWN                    1
#define INCREASE_COLOSSUS_COUNT                   2

enum Events
{
    EVENT_NONE,
    EVENT_PURSUE,
    EVENT_MISSILE,
    EVENT_VENT,
    EVENT_SPEED,
    EVENT_SUMMON,
    EVENT_SHUTDOWN, // not offylike
};

enum Seats
{
    SEAT_PLAYER                                 = 0,
    SEAT_TURRET                                 = 1,
    SEAT_DEVICE                                 = 2,
};

#define EMOTE_PURSUE          "Flame Leviathan pursues $N."
#define EMOTE_OVERLOAD        "Flame Leviathan's circuits overloaded."
#define EMOTE_REPAIR          "Automatic repair sequence initiated."
#define GOSSIP_ITEM_1         "Summon vehicles!"

enum Yells
{
    SAY_AGGRO                                   = -1603060,
    SAY_SLAY                                    = -1603061,
    SAY_DEATH                                   = -1603062,
    SAY_TARGET_1                                = -1603063,
    SAY_TARGET_2                                = -1603064,
    SAY_TARGET_3                                = -1603065,
    SAY_HARDMODE                                = -1603066,
    SAY_TOWER_NONE                              = -1603067,
    SAY_TOWER_FROST                             = -1603068,
    SAY_TOWER_FLAME                             = -1603069,
    SAY_TOWER_NATURE                            = -1603070,
    SAY_TOWER_STORM                             = -1603071,
    SAY_PLAYER_RIDING                           = -1603072,
    SAY_OVERLOAD_1                              = -1603073,
    SAY_OVERLOAD_2                              = -1603074,
    SAY_OVERLOAD_3                              = -1603075,
};

#define VEHICLE_SIEGE                              33060
#define VEHICLE_CHOPPER                            33062
#define VEHICLE_DEMOLISHER                         33109

const Position PosSiege[5] =
{
{-814.59f,-64.54f,429.92f,5.969f},
{-784.37f,-33.31f,429.92f,5.096f},
{-808.99f,-52.10f,429.92f,5.668f},
{-798.59f,-44.00f,429.92f,5.663f},
{-812.83f,-77.71f,429.92f,0.046f}
};

const Position PosChopper[5] =
{
{-717.83f,-106.56f,430.02f,0.122f},
{-717.83f,-114.23f,430.44f,0.122f},
{-717.83f,-109.70f,430.22f,0.122f},
{-718.45f,-118.24f,430.26f,0.052f},
{-718.45f,-123.58f,430.41f,0.085f}
};

const Position PosDemolisher[5] =
{
{-724.12f,-176.64f,430.03f,2.543f},
{-766.70f,-225.03f,430.50f,1.710f},
{-729.54f,-186.26f,430.12f,1.902f},
{-756.01f,-219.23f,430.50f,2.369f},
{-798.01f,-227.24f,429.84f,1.446f}
};

const Position PosColossus[2] =
{
{367.031f, 12.784f,409.886f,3.263f},
{368.768f,-46.847f,409.886f,3.036f}
};

#define ACHI_UNBROKEN             RAID_MODE(2905,2906)

class boss_flame_leviathan : public CreatureScript
{
    public:
        boss_flame_leviathan(): CreatureScript("boss_flame_leviathan") {}

    struct boss_flame_leviathanAI : public BossAI
    {
        boss_flame_leviathanAI(Creature *pCreature) : BossAI(pCreature, BOSS_LEVIATHAN), vehicle(me->GetVehicleKit())
        {
            assert(vehicle);
            pInstance = pCreature->GetInstanceScript();
            ColossusCount = 0;

            me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
            me->ApplySpellImmune(0, IMMUNITY_ID, 49560, true); // Death Grip jump effect
            
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_STUNNED);
            me->SetReactState(REACT_PASSIVE);
            
            // Summon Ulduar Colossus
            if (me->isAlive())
                for(uint32 i = 0; i < 2; ++i)
                    DoSummon(MOB_COLOSSUS, PosColossus[i], 7000, TEMPSUMMON_CORPSE_TIMED_DESPAWN);
        }

        InstanceScript* pInstance;
        Vehicle *vehicle;
        uint32 ColossusCount;

        void Reset()
        {
            _Reset();
        }

        void EnterCombat(Unit *who)
        {
            _EnterCombat();
            DoScriptText(SAY_AGGRO, me);
            events.ScheduleEvent(EVENT_PURSUE, 0);
            events.ScheduleEvent(EVENT_MISSILE, 1500);
            events.ScheduleEvent(EVENT_VENT, 20000);
            events.ScheduleEvent(EVENT_SPEED, 2000);
            //events.ScheduleEvent(EVENT_SUMMON, 0);
            events.ScheduleEvent(EVENT_SHUTDOWN, 80000);
            if (Creature *turret = CAST_CRE(vehicle->GetPassenger(7)))
                turret->AI()->DoZoneInCombat();
        }

        void JustDied(Unit *victim)
        {
            DoScriptText(SAY_DEATH, me);
            _JustDied();
            
            DespawnCreatures(VEHICLE_SIEGE, 200);
            DespawnCreatures(VEHICLE_DEMOLISHER, 200);
            DespawnCreatures(VEHICLE_CHOPPER, 200);
        }

        void SpellHit(Unit *caster, const SpellEntry *spell)
        {
            if(spell->Id == 62472)
                vehicle->InstallAllAccessories(me->GetEntry());
            else if(spell->Id == SPELL_ELECTROSHOCK)
                me->InterruptSpell(CURRENT_CHANNELED_SPELL);
        }

        void KilledUnit(Unit* Victim)
        {
            if (!(rand()%5))
                DoScriptText(SAY_SLAY, me);
        }

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            if (me->HasUnitState(UNIT_STAT_CASTING) || me->HasAura(SPELL_SYSTEMS_SHUTDOWN))
                return;

            while(uint32 eventId = events.ExecuteEvent())
            {
                switch(eventId)
                {
                case 0: break;
                case EVENT_PURSUE:
                    DoScriptText(RAND(SAY_TARGET_1, SAY_TARGET_2, SAY_TARGET_3), me);
                    {
                        DoZoneInCombat();
                        Unit* pTarget;
                        std::vector<Unit *> target_list;
                        std::list<HostileReference*> ThreatList = me->getThreatManager().getThreatList();
                        for (std::list<HostileReference*>::const_iterator itr = ThreatList.begin(); itr != ThreatList.end(); ++itr)
                        {
                            pTarget = Unit::GetUnit(*me, (*itr)->getUnitGuid());
                            
                            if (!pTarget)
                                continue;
                                
                            if (pTarget->GetEntry() == VEHICLE_SIEGE || pTarget->GetEntry() == VEHICLE_DEMOLISHER)
                                target_list.push_back(pTarget);
                                
                            pTarget = NULL;
                        }
                        
                        if (!target_list.empty())
                            pTarget = *(target_list.begin()+rand()%target_list.size());
                        else
                            pTarget = me->getVictim();
                            
                        if (pTarget && pTarget->isAlive())
                        {
                            DoResetThreat();
                            me->AddThreat(pTarget, 5000000.0f);
                            me->AddAura(SPELL_PURSUED, pTarget);
                            me->MonsterTextEmote(EMOTE_PURSUE, pTarget->GetGUID(), true);
                        }                        
                    }
                    events.RescheduleEvent(EVENT_PURSUE, 35000);
                    break;
                case EVENT_MISSILE:
                    DoCastAOE(SPELL_MISSILE_BARRAGE);
                    events.RescheduleEvent(EVENT_MISSILE, 1500);
                    break;
                case EVENT_VENT:
                    DoCastAOE(SPELL_FLAME_VENTS);
                    events.RescheduleEvent(EVENT_VENT, 20000);
                    break;
                case EVENT_SPEED:
                    DoCastAOE(SPELL_GATHERING_SPEED);
                    events.RescheduleEvent(EVENT_SPEED, 10000);
                    break;
                case EVENT_SUMMON:
                    if(summons.size() < 15) // 4seat+1turret+10lift
                        if(Creature *lift = DoSummonFlyer(MOB_MECHANOLIFT, me, rand()%20 + 20, 50, 0))
                            lift->GetMotionMaster()->MoveRandom(100);
                    events.RescheduleEvent(EVENT_SUMMON, 2000);
                    break;
                case EVENT_SHUTDOWN:
                    DoScriptText(RAND(SAY_OVERLOAD_1, SAY_OVERLOAD_2, SAY_OVERLOAD_3), me);
                    me->MonsterTextEmote(EMOTE_OVERLOAD, 0, true);
                    DoCast(SPELL_SYSTEMS_SHUTDOWN);
                    me->RemoveAurasDueToSpell(SPELL_GATHERING_SPEED);
                    me->MonsterTextEmote(EMOTE_REPAIR, 0, true);
                    events.RescheduleEvent(EVENT_SHUTDOWN, 80000);
                    break;
                default:
                    events.PopEvent();
                    break;
                }
            }
            
            if (me->IsWithinMeleeRange(me->getVictim()))
                DoSpellAttackIfReady(SPELL_BATTERING_RAM);
        }
     
        void DoAction(const int32 action)
        {
            switch (action)
            {
                case INCREASE_COLOSSUS_COUNT:
                    ++ColossusCount;
                    break;
            }
            
            if (ColossusCount >= 2)
            {
                // Event starts
                if (pInstance)
                    pInstance->SetData(DATA_LEVIATHAN_DOOR, GO_STATE_ACTIVE_ALTERNATIVE);
                    
                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_STUNNED);
                me->SetReactState(REACT_AGGRESSIVE);
                me->SetHomePosition(318.74f, -13.75f, 409.803f, 3.12723f); // new Home Position
                me->GetMotionMaster()->MoveTargetedHome();
                DoZoneInCombat();
            }
        }

        // HACK: this fixes problems in instance bind and loot.
        void DamageTaken(Unit* pKiller, uint32 &damage)
        {
            if(damage >= me->GetHealth())
            {
                if (pKiller && pKiller->IsVehicle())
                {
                    damage = 0;
                    if (!pKiller->GetVehicleKit()->HasEmptySeat(0))
                        pKiller->GetVehicleKit()->GetPassenger(0)->Kill(me, false);
                    else if (!pKiller->GetVehicleKit()->HasEmptySeat(1))
                        pKiller->GetVehicleKit()->GetPassenger(1)->Kill(me, false);
                }
            }
        }

        void DespawnCreatures(uint32 entry, float distance, bool discs = false)
        {
            std::list<Creature*> m_pCreatures;
            GetCreatureListWithEntryInGrid(m_pCreatures, me, entry, distance);
     
            if (m_pCreatures.empty())
                return;
     
            for(std::list<Creature*>::iterator iter = m_pCreatures.begin(); iter != m_pCreatures.end(); ++iter)
                (*iter)->ForcedDespawn();
        }

    };

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new boss_flame_leviathanAI (pCreature);
    };
};

//#define BOSS_DEBUG
class boss_flame_leviathan_seat : public CreatureScript
{
    public:
        boss_flame_leviathan_seat(): CreatureScript("boss_flame_leviathan_seat") {}

    struct boss_flame_leviathan_seatAI : public PassiveAI
    {
        boss_flame_leviathan_seatAI(Creature *c) : PassiveAI(c), vehicle(c->GetVehicleKit())
        {
            assert(vehicle);
    #ifdef BOSS_DEBUG
            me->SetReactState(REACT_AGGRESSIVE);
    #endif
        }

        Vehicle *vehicle;

    #ifdef BOSS_DEBUG
        void MoveInLineOfSight(Unit *who)
        {
            if(who->GetTypeId() == TYPEID_PLAYER && CAST_PLR(who)->isGameMaster()
                && !who->GetVehicle() && vehicle->GetPassenger(SEAT_TURRET))
                who->EnterVehicle(vehicle, SEAT_PLAYER);
        }
    #endif

        void PassengerBoarded(Unit *who, int8 seatId, bool apply)
        {
            if(!me->GetVehicle())
                return;

            if(seatId == SEAT_PLAYER)
            {
                if(!apply)
                    return;

                if(Creature *turret = CAST_CRE(vehicle->GetPassenger(SEAT_TURRET)))
                {
                    turret->setFaction(me->GetVehicleBase()->getFaction());
                    turret->SetUInt32Value(UNIT_FIELD_FLAGS, 0); // unselectable
                    turret->AI()->AttackStart(who);
                }
                if(Unit *device = vehicle->GetPassenger(SEAT_DEVICE))
                {
                    device->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK);
                    device->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                }
            }
            else if(seatId == SEAT_TURRET)
            {
                if(apply)
                    return;

                if(Unit *device = vehicle->GetPassenger(SEAT_DEVICE))
                {
                    device->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK);
                    device->SetUInt32Value(UNIT_FIELD_FLAGS, 0); // unselectable
                }
            }
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new boss_flame_leviathan_seatAI (pCreature);
    };
};

class boss_flame_leviathan_defense_turret : public CreatureScript
{
    public:
        boss_flame_leviathan_defense_turret(): CreatureScript("boss_flame_leviathan_defense_turret") {}

    struct boss_flame_leviathan_defense_turretAI : public TurretAI
    {
        boss_flame_leviathan_defense_turretAI(Creature *c) : TurretAI(c) {}

        void DamageTaken(Unit *who, uint32 &damage)
        {
            if(!CanAIAttack(who))
                damage = 0;
        }

        bool CanAIAttack(const Unit *who) const
        {
            if (who->GetTypeId() != TYPEID_PLAYER || !who->GetVehicle() || who->GetVehicleBase()->GetEntry() != 33114)
                return false;
            return true;
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new boss_flame_leviathan_defense_turretAI (pCreature);
    };
};

class boss_flame_leviathan_overload_device : public CreatureScript
{
    public:
        boss_flame_leviathan_overload_device(): CreatureScript("boss_flame_leviathan_overload_device") {}

    struct boss_flame_leviathan_overload_deviceAI : public PassiveAI
    {
        boss_flame_leviathan_overload_deviceAI(Creature *c) : PassiveAI(c) {}

        void DoAction(const int32 param)
        {
            if(param == EVENT_SPELLCLICK)
            {
                me->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                if(me->GetVehicle())
                {
                    if(Unit *player = me->GetVehicle()->GetPassenger(SEAT_PLAYER))
                    {
                        player->ExitVehicle();
                        me->GetVehicleBase()->CastSpell(player, SPELL_SMOKE_TRAIL, true);
                        if(Unit *leviathan = me->GetVehicleBase()->GetVehicleBase())
                            player->GetMotionMaster()->MoveKnockbackFrom(leviathan->GetPositionX(), leviathan->GetPositionY(), 30, 30);
                    }
                }
            }
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new boss_flame_leviathan_overload_deviceAI (pCreature);
    };
};

class boss_flame_leviathan_safety_container : public CreatureScript
{
    public:
        boss_flame_leviathan_safety_container(): CreatureScript("boss_flame_leviathan_safety_container") {}

    struct boss_flame_leviathan_safety_containerAI : public PassiveAI
    {
        boss_flame_leviathan_safety_containerAI(Creature *c) : PassiveAI(c) {}

        void MovementInform(uint32 type, uint32 id)
        {
            if(id == me->GetEntry())
            {
                if(Creature *liquid = DoSummon(MOB_LIQUID, me, 0))
                    liquid->CastSpell(liquid, 62494, true);
                me->DisappearAndDie(); // this will relocate creature to sky
            }
        }

        void UpdateAI(const uint32 diff)
        {
            if(!me->GetVehicle() && me->isSummon() && me->GetMotionMaster()->GetCurrentMovementGeneratorType() != POINT_MOTION_TYPE)
                me->GetMotionMaster()->MoveFall(409.8f, me->GetEntry());
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new boss_flame_leviathan_safety_containerAI(pCreature);
    };
};

class spell_pool_of_tar : public CreatureScript
{
    public:
        spell_pool_of_tar(): CreatureScript("spell_pool_of_tar") {}

    struct spell_pool_of_tarAI : public TriggerAI
    {
        spell_pool_of_tarAI(Creature *c) : TriggerAI(c)
        {
            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        }

        void DamageTaken(Unit *who, uint32 &damage)
        {
            damage = 0;
        }

        void SpellHit(Unit* caster, const SpellEntry *spell)
        {
            if(spell->SchoolMask & SPELL_SCHOOL_MASK_FIRE && !me->HasAura(SPELL_BLAZE))
                me->CastSpell(me, SPELL_BLAZE, true);
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new spell_pool_of_tarAI (pCreature);
    };
};

#define SPELL_CANNON    62357
//#define SPELL_FLAME_1   65045
//#define SPELL_FLAME_2   65044
#define SPELL_MORTAR    62635
#define SPELL_BOULDER   62307

class npc_keeper_norgannon : public CreatureScript
{
    public:
        npc_keeper_norgannon(): CreatureScript("npc_keeper_norgannon") {}

    struct keeper_norgannonAI : public ScriptedAI
    {
        keeper_norgannonAI(Creature *c) : ScriptedAI(c), summons(me)
        {
            pInstance = c->GetInstanceScript();
        }

        InstanceScript* pInstance;
        SummonList summons;

        void JustSummoned(Creature *summon)
        {
            summons.Summon(summon);
        }
        
        void DoAction(const int32 action)
        {
            switch (action)
            {
                case ACTION_VEHICLE_RESPAWN:
                    summons.DespawnAll();
                    pInstance->SetData(DATA_ACHI_UNBROKEN, 0);
                    for(int32 i = 0; i < (RAID_MODE(2, 5)); ++i)
                    {
                        if (Creature* summon = DoSummon(VEHICLE_SIEGE, PosSiege[i], 3000, TEMPSUMMON_CORPSE_TIMED_DESPAWN))
                        {
                            summon->ApplySpellImmune(0, IMMUNITY_ID, SPELL_CANNON, true);
                            summon->ApplySpellImmune(0, IMMUNITY_ID, SPELL_MORTAR, true);
                            summon->ApplySpellImmune(0, IMMUNITY_ID, SPELL_BOULDER, true);
                        }
                    }
                    for(int32 i = 0; i < (RAID_MODE(2, 5)); ++i)
                    {
                        if (Creature* summon = DoSummon(VEHICLE_CHOPPER, PosChopper[i], 3000, TEMPSUMMON_CORPSE_TIMED_DESPAWN))
                        {
                            summon->ApplySpellImmune(0, IMMUNITY_ID, SPELL_CANNON, true);
                            summon->ApplySpellImmune(0, IMMUNITY_ID, SPELL_MORTAR, true);
                            summon->ApplySpellImmune(0, IMMUNITY_ID, SPELL_BOULDER, true);
                        }
                    }
                    for(int32 i = 0; i < (RAID_MODE(2, 5)); ++i)
                    {
                        if (Creature* summon = DoSummon(VEHICLE_DEMOLISHER, PosDemolisher[i], 3000, TEMPSUMMON_CORPSE_TIMED_DESPAWN))
                        {
                            summon->ApplySpellImmune(0, IMMUNITY_ID, SPELL_CANNON, true);
                            summon->ApplySpellImmune(0, IMMUNITY_ID, SPELL_MORTAR, true);
                            summon->ApplySpellImmune(0, IMMUNITY_ID, SPELL_BOULDER, true);
                            summon->SetPower(POWER_ENERGY,summon->GetMaxPower(POWER_ENERGY));
                        }
                    }
                    break;
            }
        }
    };

    bool OnGossipHello(Player* pPlayer, Creature* pCreature)
    {
        if (pPlayer)
        {
            InstanceScript *instance = pCreature->GetInstanceScript();
            if (instance && (instance->GetBossState(BOSS_LEVIATHAN) != DONE))
            {
                pPlayer->PrepareGossipMenu(pCreature);
                pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT,GOSSIP_ITEM_1,GOSSIP_SENDER_MAIN,GOSSIP_ACTION_INFO_DEF);
                pPlayer->SEND_GOSSIP_MENU(13910, pCreature->GetGUID());
            }            
            else pPlayer->SEND_GOSSIP_MENU(1, pCreature->GetGUID());
        }
        return true;
    };

    bool OnGossipSelect(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
    {
        pPlayer->PlayerTalkClass->ClearMenus();

        InstanceScript* pInstance = pCreature->GetInstanceScript();

        if (!pInstance)
            return true;

        switch (uiAction)
        {
            case GOSSIP_ACTION_INFO_DEF:
                pPlayer->CLOSE_GOSSIP_MENU();
                if (Creature* Norgannon = Unit::GetCreature(*pCreature, pInstance ? pInstance->GetData64(DATA_NORGANNON) : 0))
                    if (Norgannon->isAlive())
                    {
                        Norgannon->AI()->DoAction(ACTION_VEHICLE_RESPAWN);
                        //pInstance->SetBossState(BOSS_LEVIATHAN, SPECIAL);
                    }
                break;
        }
        return true;
    };

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new keeper_norgannonAI (pCreature);
    };
};

class mob_colossus : public CreatureScript
{
    public:
        mob_colossus(): CreatureScript("mob_colossus") {}

    struct mob_colossusAI : public ScriptedAI
    {
        mob_colossusAI(Creature* pCreature) : ScriptedAI(pCreature)
        {
            m_pInstance = me->GetInstanceScript();
        }

        InstanceScript* m_pInstance;
        int32 uiGroundSlamTimer;

        void Reset()
        {
            uiGroundSlamTimer = 12000;
        }
        
        void JustDied(Unit *victim)
        {
            if (Creature* pLeviathan = Unit::GetCreature(*me, m_pInstance->GetData64(DATA_LEVIATHAN)))
                if (pLeviathan->AI())
                    pLeviathan->AI()->DoAction(INCREASE_COLOSSUS_COUNT);
        }

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            if (uiGroundSlamTimer <= int32(diff))
            {
                DoCast(me->getVictim(), SPELL_GROUND_SLAM);
                uiGroundSlamTimer = 12000;
            } else uiGroundSlamTimer -= diff;

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new mob_colossusAI(pCreature);
    };
};

#define EMOTE_REPAIR          "Automatic repair sequence initiated."

class at_RX_214_repair_o_matic_station : public AreaTriggerScript
{
    public:
    at_RX_214_repair_o_matic_station() : AreaTriggerScript("at_RX_214_repair_o_matic_station") { }

    bool OnTrigger(Player* pPlayer, const AreaTriggerEntry* /*pAt*/)
    {
        if (!pPlayer)
            return false;

        if (Creature* vehicle = pPlayer->GetVehicleCreatureBase())
        {
            if (!vehicle->HasAura(SPELL_AUTO_REPAIR) && (vehicle->GetHealth() != vehicle->GetMaxHealth()) &&
                !pPlayer->isInCombat())
            {
                pPlayer->MonsterTextEmote(EMOTE_REPAIR, pPlayer->GetGUID(), true);
                vehicle->SetHealth(vehicle->GetMaxHealth()); // Correct spell not works
                pPlayer->CastSpell(vehicle, SPELL_AUTO_REPAIR, true);
                if(InstanceScript *data = pPlayer->GetInstanceScript())
                    data->SetData(DATA_ACHI_UNBROKEN, ACHI_FAILED);
            }
        }
        return true;
    };
};

class ulduar_repair_npc : public CreatureScript
{
    public:
        ulduar_repair_npc(): CreatureScript("ulduar_repair_npc") {}

    bool OnGossipHello(Player *player, Creature *_Creature)
    {
        if (!player)
            return true;

        player->ADD_GOSSIP_ITEM( 5, "Repair Chopper"         , GOSSIP_SENDER_MAIN, 1005);
        player->ADD_GOSSIP_ITEM( 5, "Repair Siege"           , GOSSIP_SENDER_MAIN, 1010);
        player->ADD_GOSSIP_ITEM( 5, "Repair Demolisher"      , GOSSIP_SENDER_MAIN, 1015);
         
        player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,_Creature->GetGUID());

        return true;
    };

    void SendDefaultMenu_ulduar_repair_npc(Player *player, Creature *_Creature, uint32 action)
    {
        if (!player)
            return;

        // Not allow in combat
        if (!player->getAttackers().empty())
        {
            player->CLOSE_GOSSIP_MENU();
            _Creature->MonsterSay("Sei in combat!", LANG_UNIVERSAL, 0);
            return;
        }

        if (InstanceScript *data = player->GetInstanceScript())
            if (data->GetBossState(BOSS_LEVIATHAN) == IN_PROGRESS)
            {
                player->CLOSE_GOSSIP_MENU();
                _Creature->MonsterSay("Sei in combat con il leviathan!", LANG_UNIVERSAL, 0);
                return;            
            }
        
        switch(action)
        {
            case 1005: //Chopper
                if (Creature* vehicle = _Creature->FindNearestCreature(VEHICLE_CHOPPER, 20))
                {
                    if (vehicle->GetHealth() != vehicle->GetMaxHealth())
                    {
                        vehicle->SetHealth(vehicle->GetMaxHealth());
                        _Creature->MonsterSay("Chopper riparato!", LANG_UNIVERSAL, 0);
                        if (InstanceScript *data = player->GetInstanceScript())
                            data->SetData(DATA_ACHI_UNBROKEN, ACHI_FAILED);
                    }
                    else
                        _Creature->MonsterSay("Il chopper non ha bisogno di riparazioni!", LANG_UNIVERSAL, 0);
                }
                else
                    _Creature->MonsterSay("Non trovo un chopper nelle vicinanze", LANG_UNIVERSAL, 0);
                player->CLOSE_GOSSIP_MENU();
                break;
            case 1010: //Siege
                if (Creature* vehicle = _Creature->FindNearestCreature(VEHICLE_SIEGE, 20))
                {
                    if (vehicle->GetHealth() != vehicle->GetMaxHealth())
                    {
                        vehicle->SetHealth(vehicle->GetMaxHealth());
                        _Creature->MonsterSay("Siege riparato!", LANG_UNIVERSAL, 0);
                        if (InstanceScript *data = player->GetInstanceScript())
                            data->SetData(DATA_ACHI_UNBROKEN, ACHI_FAILED);
                    }
                    else
                        _Creature->MonsterSay("Il siege non ha bisogno di riparazioni!", LANG_UNIVERSAL, 0);
                }
                else
                    _Creature->MonsterSay("Non trovo un siege nelle vicinanze", LANG_UNIVERSAL, 0);
                player->CLOSE_GOSSIP_MENU();
                break; 
            case 1015: //Demolisher
                if (Creature* vehicle = _Creature->FindNearestCreature(VEHICLE_DEMOLISHER, 20))
                {
                    if ((vehicle->GetHealth() != vehicle->GetMaxHealth()) || (vehicle->GetPower(POWER_ENERGY) != vehicle->GetMaxPower(POWER_ENERGY)))
                    {
                        vehicle->SetHealth(vehicle->GetMaxHealth());
                        vehicle->SetPower(POWER_ENERGY,vehicle->GetMaxPower(POWER_ENERGY));
                        _Creature->MonsterSay("Demolisher riparato!", LANG_UNIVERSAL, 0);
                        if (InstanceScript *data = player->GetInstanceScript())
                            data->SetData(DATA_ACHI_UNBROKEN, ACHI_FAILED);
                    }
                    else
                        _Creature->MonsterSay("Il demolisher non ha bisogno di riparazioni!", LANG_UNIVERSAL, 0);
                }
                else
                    _Creature->MonsterSay("Non trovo un demolisher nelle vicinanze", LANG_UNIVERSAL, 0);
                player->CLOSE_GOSSIP_MENU();
                break; 
        }
    };

    bool OnGossipSelect(Player *pPlayer, Creature *_Creature, uint32 sender, uint32 action)
    {
        pPlayer->PlayerTalkClass->ClearMenus();

        // Main menu
        if (sender == GOSSIP_SENDER_MAIN)
            SendDefaultMenu_ulduar_repair_npc( pPlayer, _Creature, action );

        return true;
    };
};

class mob_flameleviathan_loot : public CreatureScript
{
    public:
        mob_flameleviathan_loot(): CreatureScript("mob_flameleviathan_loot") {}

    struct mob_flameleviathan_lootAI : public ScriptedAI
    {
        mob_flameleviathan_lootAI(Creature* pCreature) : ScriptedAI(pCreature)
        {
            pInstance = me->GetInstanceScript();
            bLeviathan = true;
        }

        InstanceScript* pInstance;
        bool bLeviathan;
        uint32 uiExplosion;

        void EnterCombat(Unit *who)
        {
            uiExplosion = 5000;

            if (!pInstance)
            {
                bLeviathan = true;
                return;
            }

            if (Unit *pLeviathan = Unit::GetUnit(*me, pInstance->GetData64(DATA_LEVIATHAN)))
            {
                if (pLeviathan->isAlive())
                {
                    bLeviathan = true;   
                    me->MonsterYell("Rilevato tentativo di Kill Illegale, attivazione sistema di distruzione personaggi", LANG_UNIVERSAL,0);
                    if (who)
                        sLog->outCheat("Boss-%s, Tentativo di kill del loot del Flame Leviathan senza aver fatto il boss", who->GetName());
                    return;
                }
            }
            bLeviathan = false;
        }

        void DamageTaken(Unit* pKiller, uint32 &damage)
        {
            if (bLeviathan)
                damage = 0;            
        }

        void JustDied(Unit *victim)
        {
            if (pInstance->GetData(DATA_ACHI_UNBROKEN) == ACHI_IS_IN_PROGRESS)
                pInstance->DoCompleteAchievement(ACHI_UNBROKEN);
        }

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;           

            if (bLeviathan)
                if (uiExplosion < diff)
                {
                    DoCast(me, 64487, true); //Ascend to the Heavens
                    uiExplosion = 5000;
                } else uiExplosion -= diff;                    

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new mob_flameleviathan_lootAI(pCreature);
    };
};

class mob_steelforged_defender : public CreatureScript //33236
{
    public:
        mob_steelforged_defender() : CreatureScript("mob_steelforged_defender") { }

    struct mob_steelforged_defenderAI : public ScriptedAI
    {
       mob_steelforged_defenderAI(Creature *c) : ScriptedAI(c)
       {
           pInstance = me->GetInstanceScript();
       }

       InstanceScript* pInstance;

       void JustDied(Unit *victim)
       {
           if(pInstance->GetData(DATA_DWARFAGEDDON_START) == ACHI_IS_NOT_STARTED)
               pInstance->SetData(DATA_DWARFAGEDDON_START, ACHI_START);
           
           pInstance->SetData(DATA_DWARFAGEDDON_COUNT, ACHI_INCREASE);
       }
    };

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new mob_steelforged_defenderAI (pCreature);
    };
};

void AddSC_boss_flame_leviathan()
{
    new boss_flame_leviathan();
    new boss_flame_leviathan_seat();
    new boss_flame_leviathan_defense_turret();
    new boss_flame_leviathan_overload_device();
    new boss_flame_leviathan_safety_container();
    new spell_pool_of_tar();
    new npc_keeper_norgannon();
    new mob_colossus();
    new ulduar_repair_npc();
    new at_RX_214_repair_o_matic_station();
    new mob_flameleviathan_loot();
    new mob_steelforged_defender();
}
