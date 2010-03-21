/*
 * Copyright (C) 2008 - 2009 Trinity <http://www.trinitycore.org/>
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

#include "ScriptedPch.h"
#include "ulduar.h"
#include "Vehicle.h"

enum Spells
{
    SPELL_PURSUED                  = 62374,
    SPELL_GATHERING_SPEED          = 62375,
    SPELL_BATTERING_RAM            = 62376,
    SPELL_FLAME_VENTS              = 62396,
    SPELL_MISSILE_BARRAGE          = 62400,
    SPELL_SYSTEMS_SHUTDOWN         = 62475,

    SPELL_FLAME_CANNON             = 62395,
    //SPELL_FLAME_CANNON           = 64692 trigger the same spell

    SPELL_OVERLOAD_CIRCUIT         = 62399,

    SPELL_SEARING_FLAME            = 62402,

    SPELL_BLAZE                    = 62292,

    SPELL_SMOKE_TRAIL              = 63575,

    SPELL_MIMIRON_INFERNO          = 62910,

    SPELL_HODIR_FURY               = 62297,

    SPELL_ELECTROSHOCK             = 62522
};

enum Mobs
{
    MOB_MECHANOLIFT = 33214,
    MOB_LIQUID      = 33189,
    MOB_CONTAINER   = 33218,
};

enum Events
{
    EVENT_NONE,
    EVENT_PURSUE,
    EVENT_MISSILE,
    EVENT_VENT,
    EVENT_SPEED,
    EVENT_SUMMON
};

enum Seats
{
    SEAT_PLAYER = 0,
    SEAT_TURRET = 1,
    SEAT_DEVICE = 2,
};

struct boss_flame_leviathanAI : public BossAI
{
    boss_flame_leviathanAI(Creature *pCreature) : BossAI(pCreature, TYPE_LEVIATHAN), vehicle(me->GetVehicleKit())
    {
        assert(vehicle);
    }

    Vehicle *vehicle;

    void Reset()
    {
        _Reset();
        me->SetReactState(REACT_AGGRESSIVE);
    }

    void EnterCombat(Unit *who)
    {
        _EnterCombat();
        me->SetReactState(REACT_DEFENSIVE);
        events.ScheduleEvent(EVENT_PURSUE, 0);
        events.ScheduleEvent(EVENT_MISSILE, 1500);
        events.ScheduleEvent(EVENT_VENT, 20000);
        events.ScheduleEvent(EVENT_SPEED, 15000);
        events.ScheduleEvent(EVENT_SUMMON, 0);
        if (Creature *turret = CAST_CRE(vehicle->GetPassenger(7)))
            turret->AI()->DoZoneInCombat();
    }

    void JustDied(Unit *victim)
    {
        _JustDied();
    }

    void SpellHit(Unit *caster, const SpellEntry *spell)
    {
        if(spell->Id == 62472)
            vehicle->InstallAllAccessories();
        else if(spell->Id == SPELL_ELECTROSHOCK)
            me->InterruptSpell(CURRENT_CHANNELED_SPELL);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!UpdateVictim())
            return;

        events.Update(diff);

        while(uint32 eventId = events.ExecuteEvent())
        {
            switch(eventId)
            {
            case 0: break; // this is a must
            case EVENT_PURSUE:
                Pursue();
                events.RescheduleEvent(EVENT_PURSUE, 35000);
                if(!me->getVictim()) // all siege engines and demolishers are dead
                    UpdateVictim(); // begin to kill other things
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
                events.RescheduleEvent(EVENT_SPEED, 15000);
                break;
            case EVENT_SUMMON:
                if(summons.size() < 15) // 4seat+1turret+10lift
                    if(Creature *lift = DoSummonFlyer(MOB_MECHANOLIFT, me, rand()%20 + 20, 50, 0))
                        lift->GetMotionMaster()->MoveRandom(100);
                events.RescheduleEvent(EVENT_SUMMON, 2000);
                break;
            default:
                events.PopEvent();
                break;
            }
        }

        DoSpellAttackIfReady(SPELL_BATTERING_RAM);
    }

    void Pursue()
    {
        std::list<Creature*> PursueList;
        me->GetCreatureListWithEntryInGrid(PursueList, RAND(33060,33109), 200.0f);
        if (PursueList.empty())
            {
                me->GetCreatureListWithEntryInGrid(PursueList, 33060, 200.0f);
                if (PursueList.empty())
                    me->GetCreatureListWithEntryInGrid(PursueList, 33109, 200.0f);
            }
		else
        {
            for (std::list<Creature*>::iterator itr = PursueList.begin(); itr != PursueList.end(); ++itr)
            {
                if (Creature* pPursued = *itr)
                    {
                        DoResetThreat();
                        me->AddThreat(pPursued, 5000000.0f);
                        AttackStart(pPursued);
                    }
            }
        }
        me->AddAura(SPELL_PURSUED, me->getVictim());
    }
};

//#define BOSS_DEBUG

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

CreatureAI* GetAI_boss_flame_leviathan(Creature* pCreature)
{
    return new boss_flame_leviathanAI (pCreature);
}

CreatureAI* GetAI_boss_flame_leviathan_seat(Creature* pCreature)
{
    return new boss_flame_leviathan_seatAI (pCreature);
}

CreatureAI* GetAI_boss_flame_leviathan_defense_turret(Creature* pCreature)
{
    return new boss_flame_leviathan_defense_turretAI (pCreature);
}

CreatureAI* GetAI_boss_flame_leviathan_overload_device(Creature* pCreature)
{
    return new boss_flame_leviathan_overload_deviceAI (pCreature);
}

CreatureAI* GetAI_boss_flame_leviathan_safety_containerAI(Creature* pCreature)
{
    return new boss_flame_leviathan_safety_containerAI(pCreature);
}

CreatureAI* GetAI_spell_pool_of_tar(Creature* pCreature)
{
    return new spell_pool_of_tarAI (pCreature);
}

void AddSC_boss_flame_leviathan()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_flame_leviathan";
    newscript->GetAI = &GetAI_boss_flame_leviathan;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "boss_flame_leviathan_seat";
    newscript->GetAI = &GetAI_boss_flame_leviathan_seat;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "boss_flame_leviathan_defense_turret";
    newscript->GetAI = &GetAI_boss_flame_leviathan_defense_turret;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "boss_flame_leviathan_overload_device";
    newscript->GetAI = &GetAI_boss_flame_leviathan_overload_device;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "boss_flame_leviathan_safety_container";
    newscript->GetAI = &GetAI_boss_flame_leviathan_safety_containerAI;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "spell_pool_of_tar";
    newscript->GetAI = &GetAI_spell_pool_of_tar;
    newscript->RegisterSelf();
}
