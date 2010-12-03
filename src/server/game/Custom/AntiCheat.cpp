#include "AntiCheat.h"
#include "Log.h"
#include "WorldPacket.h"
#include "Opcodes.h"
#include "Corpse.h"
#include "Config.h"
#include "Player.h"
#include "Vehicle.h"
#include "Transport.h"
#include "ObjectMgr.h"

/**
 *
 * @File : AntiCheat.cpp
 *
 * @Authors : Lazzalf based on AC2 and Manuel AntiCheat
 *
 **/

AntiCheat::AntiCheat()
{
	cheat_find = false;
    map_count = true;
    map_block = true;
    map_puni = true;
}

AntiCheat_Local::AntiCheat_Local()
{
	ac_block = false;
    number_cheat_find = 0;

	ac_delta = 0;
    ac_goactivate = 0;

    for (int i = 0; i < MAX_CHEAT; i++)
        m_CheatList[i] = 0;
    m_CheatList_reset_diff = sWorld.getIntConfig(CONFIG_AC_RESET_CHEATLIST_DELTA);
	
	m_anti_LastClientTime  = 0;          // last movement client time
    m_anti_LastServerTime  = 0;          // last movement server time
    m_anti_DeltaClientTime = 0;          // client side session time
    m_anti_DeltaServerTime = 0;          // server side session time
    m_anti_MistimingCount  = 0;          // mistiming count

    m_anti_LastSpeedChangeTime = 0;      // last speed change time

    m_anti_Last_HSpeed =  7.0f;          // horizontal speed, default RUN speed
    m_anti_Last_VSpeed = -2.3f;          // vertical speed, default max jump height

    m_anti_TeleToPlane_Count = 0;        // Teleport To Plane alarm counter

    m_anti_AlarmCount = 0;               // alarm counter

    m_anti_JumpCount = 0;                // Jump already began, anti air jump check
    m_anti_JumpBaseZ = 0;                // Z coord before jump (AntiGrav)

    m_logfile_time = 0;                  // Time for logs file
    m_logdb_time = 0;                    // Time for logs DB
}

void AntiCheat_Local::SetBlock(bool block)
{
	ac_block = block;
}

bool AntiCheat_Local::GetBlock()
{
	return ac_block;
}

bool AntiCheat_Local::GetAndUpdateDelta(int32 diff)
{
    if (ac_goactivate > 0)
    {
        ac_goactivate--;
        return true;
    }

    if (ac_delta == 0)
        ac_delta = int32(sWorld.getIntConfig(CONFIG_AC_SLEEP_DELTA));

    if (ac_delta > 0)
    {
        if (ac_delta > diff)
            ac_delta -= diff;
        else
        {
            ac_delta = 0;
            ac_goactivate = sWorld.getIntConfig(CONFIG_AC_ALIVE_COUNT);
        }
    }
    else if (ac_delta < 0)
    {
        if (ac_delta < -diff)
            ac_delta += diff;
        else
        {
            ac_delta = 0;
        }
    }
		
	return ac_delta <= 0;
}

void AntiCheat_Local::SetDelta(int32 delta)
{
    ac_delta = delta;
}

void AntiCheat_Local::ResetCheatList(uint32 diff)
{
    if (sWorld.getIntConfig(CONFIG_AC_RESET_CHEATLIST_DELTA) == 0)
        return;

    if (m_CheatList_reset_diff >= diff)
		m_CheatList_reset_diff -= diff;
	else 
		m_CheatList_reset_diff = 0;

	if (!m_CheatList_reset_diff)
    {
		for (int i = 0; i < MAX_CHEAT; i++)
            m_CheatList[i] = 0;

        number_cheat_find = 0;

        m_CheatList_reset_diff = sWorld.getIntConfig(CONFIG_AC_RESET_CHEATLIST_DELTA);
    }
}

bool AntiCheat::Check(Player* plMover, Vehicle *vehMover, uint16 opcode, MovementInfo& pMovementInfo, Unit *mover)
{
	if (!plMover)
		return true;
	
	if (!sWorld.getBoolConfig(CONFIG_AC_ENABLE))
		return true;
		
	if (plMover->GetSession() && plMover->GetSession()->GetSecurity() >= int32(sWorld.getIntConfig(CONFIG_AC_DISABLE_GM_LEVEL)))
		return true;
	
	// Calc Delthas for AntiCheat
	CalcDeltas(plMover, pMovementInfo);

    // Clean player cheatlist only if we founded a cheat
    if (plMover->ac_local.number_cheat_find)
        plMover->ac_local.ResetCheatList(cServerTimeDelta);
	
    // Diff for AntiCheat sleep
	if (!plMover->ac_local.GetAndUpdateDelta(int32(cServerTimeDelta)))
        return true;
  
    // AntiCheat Block (not used for now)
	//if (plMover->ac_local.GetBlock())
	//	return true;

    if (sWorld.iIgnoreMapIds_AC.count(plMover->GetMapId()))
        return true;

	// Set to false if find a Cheat
	bool check_passed = true;

    // Only if we are not coming from sleep
    if (plMover->ac_local.ac_goactivate < int32(sWorld.getIntConfig(CONFIG_AC_ALIVE_COUNT)))
    {
        // Check taxi flight
        const uint32 curDest = plMover->m_taxi.GetTaxiDestination();	
	    if (!curDest)
	    {            
            CalcVariables(plMover, pMovementInfo, mover);
            	
	        // Mistiming Cheat
	        if (sWorld.getBoolConfig(CONFIG_AC_ENABLE_MISTIMING))
		        if (!CheckMistiming(plMover, vehMover, pMovementInfo))
			        check_passed = false;

            // Gravity Cheat
		    if (sWorld.getBoolConfig(CONFIG_AC_ENABLE_ANTIGRAVITY))
			    if (!CheckAntiGravity(plMover, vehMover, pMovementInfo))
				    check_passed = false;

            // MultiJump Cheat
		    if (sWorld.getBoolConfig(CONFIG_AC_ENABLE_ANTIMULTIJUMP))
			    if (!CheckAntiMultiJump(plMover, vehMover, pMovementInfo, opcode))
				    check_passed = false;

            // Speed and Tele Cheat
		    if (sWorld.getBoolConfig(CONFIG_AC_ENABLE_ANTISPEEDTELE))
			    if (!CheckAntiSpeedTeleport(plMover, vehMover, pMovementInfo, opcode))
				    check_passed = false;

            // Mountain Cheat
		    if (sWorld.getBoolConfig(CONFIG_AC_ENABLE_ANTIMOUNTAIN))
			    if (!CheckAntiMountain(plMover, vehMover, pMovementInfo))
				    check_passed = false;

            // Fly Cheat
		    if (sWorld.getBoolConfig(CONFIG_AC_ENABLE_ANTIFLY))
			    if (!CheckAntiFly(plMover, vehMover, plMover->ac_local.GetLastPacket(), pMovementInfo))
				    check_passed = false;

            // Waterwalk Cheat
		    if (sWorld.getBoolConfig(CONFIG_AC_ENABLE_ANTIWATERWALK))
                if (!CheckAntiWaterwalk(plMover, vehMover, plMover->ac_local.GetLastPacket(), pMovementInfo))
				    check_passed = false;

            // Tele To Plane Cheat
		    if (sWorld.getBoolConfig(CONFIG_AC_ENABLE_ANTITELETOPLANE))
			    if (!CheckAntiTeleToPlane(plMover, vehMover, pMovementInfo))
				    check_passed = false;
	    }
        if (cheat_find)
        {
            // Yes, we found a cheater
            if (map_count)
            {
                ++(plMover->ac_local.number_cheat_find);

                if (sWorld.getIntConfig(CONFIG_AC_REPORTS_FOR_GM_WARNING) &&
                    plMover->ac_local.number_cheat_find > sWorld.getIntConfig(CONFIG_AC_REPORTS_FOR_GM_WARNING)) 
                {
                    // display warning at the center of the screen, hacky way.
                    std::string str = "";
                    str = "|cFFFFFC00[AC]|cFF00FFFF[|cFF60FF00" + std::string(plMover->GetName()) + "|cFF00FFFF] Possible cheater!";
                    WorldPacket data(SMSG_NOTIFICATION, (str.size()+1));
                    data << str;
                    sWorld.SendGlobalGMMessage(&data);
                }
            }
            // We are are not going to sleep
            plMover->ac_local.SetDelta(-abs(int32(sWorld.getIntConfig(CONFIG_AC_ALARM_DELTA))));
            // Increase reset cheat list time
            if (plMover->ac_local.m_CheatList_reset_diff < sWorld.getIntConfig(CONFIG_AC_RESET_CHEATLIST_DELTA_FOUND))
                plMover->ac_local.m_CheatList_reset_diff = sWorld.getIntConfig(CONFIG_AC_RESET_CHEATLIST_DELTA_FOUND);
            if (!check_passed && map_puni) // PunisherCount >= BlockCount
                AntiCheatPunisher(plMover, pMovementInfo); // Try Punish him
        }
    }
    else
        CalcVariablesSmall(plMover, pMovementInfo, mover);

    plMover->ac_local.SaveLastPacket(pMovementInfo);
    plMover->ac_local.SetLastOpcode(opcode);
    plMover->ac_local.SetLastSpeedRate(fSpeedRate);

	return check_passed;
}

bool AntiCheat::ControllPunisher(Player* plMover)
{
    if (plMover->ac_local.m_CheatList[CHEAT_MISTIMING] >= sWorld.getIntConfig(CONFIG_AC_MISTIMING_PUNI_COUNT))
        return true;
    else if (plMover->ac_local.m_CheatList[CHEAT_GRAVITY] >= sWorld.getIntConfig(CONFIG_AC_ANTIGRAVITY_PUNI_COUNT))
        return true;
    else if (plMover->ac_local.m_CheatList[CHEAT_MULTIJUMP] >= sWorld.getIntConfig(CONFIG_AC_ANTIMULTIJUMP_PUNI_COUNT))
        return true;
    else if (plMover->ac_local.m_CheatList[CHEAT_SPEED] >= sWorld.getIntConfig(CONFIG_AC_ANTISPEED_PUNI_COUNT))
        return true;
    else if (plMover->ac_local.m_CheatList[CHEAT_TELEPORT] >= sWorld.getIntConfig(CONFIG_AC_ANTITELE_PUNI_COUNT))
        return true;
    else if (plMover->ac_local.m_CheatList[CHEAT_MOUNTAIN] >= sWorld.getIntConfig(CONFIG_AC_ANTIMOUNTAIN_PUNI_COUNT))
        return true;
    else if (plMover->ac_local.m_CheatList[CHEAT_FLY] >= sWorld.getIntConfig(CONFIG_AC_ANTIFLY_PUNI_COUNT))
        return true;
    else if (plMover->ac_local.m_CheatList[CHEAT_WATERWALK] >= sWorld.getIntConfig(CONFIG_AC_ANTIWATERWALK_PUNI_COUNT))
        return true;
    else if (plMover->ac_local.m_CheatList[CHEAT_TELETOPLANE] >= sWorld.getIntConfig(CONFIG_AC_ANTITELETOPLANE_PUNI_COUNT))
        return true;

    return false;
}

bool AntiCheat::AntiCheatPunisher(Player* plMover, MovementInfo& pMovementInfo)
{
    if (!sWorld.getIntConfig(CONFIG_AC_PUNI_TYPE))
        return true;

    if (plMover->getLevel() > sWorld.getIntConfig(CONFIG_AC_PUNI_LEVEL_MAX))
        return true;

    if (sWorld.getBoolConfig(CONFIG_AC_PUNI_MAP_SMALL))
        if (plMover->GetMapId() != 0 && plMover->GetMapId() != 1)
            return true;

    // Lagghi test
    if ((plMover->ac_local.m_CheatList[CHEAT_MISTIMING] >= (plMover->ac_local.m_CheatList[CHEAT_SPEED] / 15)) &&
        (plMover->ac_local.m_CheatList[CHEAT_FLY] < (plMover->ac_local.m_CheatList[CHEAT_SPEED] / 10)))
        return true;

    if (!ControllPunisher(plMover))
        return true; 

    // Yes, We Can!
    std::string announce = "";
    switch (sWorld.getIntConfig(CONFIG_AC_PUNI_TYPE))
    {
        case PUNI_NONE:
            return true;
        case PUNI_BLOCK:
            sLog.outCheat("AC-Punisher-%s Map %u Area %u, X:%f Y:%f Z:%f, PUNISHER BLOCK",
                    plMover->GetName(), plMover->GetMapId(), plMover->GetAreaId(), plMover->GetPositionX(), plMover->GetPositionY(), plMover->GetPositionZ());
            return false;
        case PUNI_KILL:
            plMover->DealDamage(plMover, plMover->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
            sLog.outCheat("AC-Punisher-%s Map %u Area %u, X:%f Y:%f Z:%f, PUNISHER KILL",
                    plMover->GetName(), plMover->GetMapId(), plMover->GetAreaId(), plMover->GetPositionX(), plMover->GetPositionY(), plMover->GetPositionZ());
            return false;
        case PUNI_KICK:
            plMover->GetSession()->KickPlayer();
            sLog.outCheat("AC-Punisher-%s Map %u Area %u, X:%f Y:%f Z:%f, PUNISHER KICK",
                    plMover->GetName(), plMover->GetMapId(), plMover->GetAreaId(), plMover->GetPositionX(), plMover->GetPositionY(), plMover->GetPositionZ());
            announce = "AntiCheatPunisher ha Kickato il player ";
            announce += plMover->GetName();
            announce += "per uso di Hack";
            sWorld.SendServerMessage(SERVER_MSG_STRING,announce.c_str());
            return false;
        case PUNI_BAN_CHAR:
            sLog.outCheat("AC-Punisher-%s Map %u Area %u, X:%f Y:%f Z:%f, PUNISHER BAN_CHARACTER",
                    plMover->GetName(), plMover->GetMapId(), plMover->GetAreaId(), plMover->GetPositionX(), plMover->GetPositionY(), plMover->GetPositionZ());            
            announce = "AntiCheatPunisher ha bannato il player ";
            announce += plMover->GetName();
            announce += " per uso di Hack";
            sWorld.SendServerMessage(SERVER_MSG_STRING,announce.c_str());
            sWorld.BanCharacter(plMover->GetName(),sConfig.GetStringDefault("Anticheat.Punisher.BanTime", "-1"),"Cheat","AntiCheatPunisher");
            return false;
        case PUNI_BAN_ACC:
            {
                sLog.outCheat("AC-Punisher-%s Map %u Area %u, X:%f Y:%f Z:%f, PUNISHER BAN_ACCOUNT",
                        plMover->GetName(), plMover->GetMapId(), plMover->GetAreaId(), plMover->GetPositionX(), plMover->GetPositionY(), plMover->GetPositionZ());
                announce = "AntiCheatPunisher ha bannato l'account del player ";
                announce += plMover->GetName();
                announce += " per uso di Hack";
                sWorld.SendServerMessage(SERVER_MSG_STRING,announce.c_str());
                sWorld.BanAccount(BAN_CHARACTER,plMover->GetName(),sConfig.GetStringDefault("Anticheat.Punisher.BanTime", "-1"),"Cheat","AntiCheatPunisher");
            }
        case PUNI_BAN_IP:
            {
                sLog.outCheat("AC-Punisher-%s Map %u Area %u, X:%f Y:%f Z:%f, PUNISHER BAN_IP",
                        plMover->GetName(), plMover->GetMapId(), plMover->GetAreaId(), plMover->GetPositionX(), plMover->GetPositionY(), plMover->GetPositionZ());
                announce = "AntiCheatPunisher ha bannato l'ip del player ";
                announce += plMover->GetName();
                announce += " per uso di Hack";
                sWorld.SendServerMessage(SERVER_MSG_STRING,announce.c_str());
                QueryResult result = LoginDatabase.PQuery("SELECT last_ip FROM account WHERE id=%u", plMover->GetSession()->GetAccountId());
                if (result)
                {

                    Field *fields = result->Fetch();
                    std::string LastIP = fields[0].GetString();
                    if(!LastIP.empty())
                    {
                        sWorld.BanAccount(BAN_IP,LastIP,sConfig.GetStringDefault("Anticheat.Punisher.BanTime", "-1"),"Cheat","AntiCheatPunisher");
                    }
                }
                return false;
            }
        default:
            sLog.outCheat("AC-Punisher-%s Map %u Area %u, X:%f Y:%f Z:%f, PUNISHER TYPE NOT VALID",
                    plMover->GetName(), plMover->GetMapId(), plMover->GetAreaId(), plMover->GetPositionX(), plMover->GetPositionY(), plMover->GetPositionZ());
            return false;
    }
}

void AntiCheat::CalcDeltas(Player* plMover, MovementInfo& pMovementInfo)
{
	// calc time deltas
	cClientTimeDelta = 1500;
	if (plMover->ac_local.m_anti_LastClientTime != 0)
	{
		cClientTimeDelta = pMovementInfo.time - plMover->ac_local.m_anti_LastClientTime;
		plMover->ac_local.m_anti_DeltaClientTime += cClientTimeDelta;
		plMover->ac_local.m_anti_LastClientTime = pMovementInfo.time;
	}
	else
		plMover->ac_local.m_anti_LastClientTime = pMovementInfo.time;

	cServerTime = getMSTime();

    if (!plMover->ac_local.m_logfile_time)
	    difftime_log_file = cServerTime - plMover->ac_local.m_logfile_time;
    if (!plMover->ac_local.m_logdb_time)
        difftime_log_db = cServerTime - plMover->ac_local.m_logdb_time;

	cServerTimeDelta = 1500;
	if (plMover->ac_local.m_anti_LastServerTime != 0)
	{
		cServerTimeDelta = cServerTime - plMover->ac_local.m_anti_LastServerTime;
		plMover->ac_local.m_anti_DeltaServerTime += cServerTimeDelta;
		plMover->ac_local.m_anti_LastServerTime = cServerTime;
	}
	else
		plMover->ac_local.m_anti_LastServerTime = cServerTime;

	// resync times on client login (first 15 sec for heavy areas)
	if (plMover->ac_local.m_anti_DeltaServerTime < 15000 && plMover->ac_local.m_anti_DeltaClientTime < 15000)
		plMover->ac_local.m_anti_DeltaClientTime = plMover->ac_local.m_anti_DeltaServerTime;

	sync_time = plMover->ac_local.m_anti_DeltaClientTime - plMover->ac_local.m_anti_DeltaServerTime;

    map_count = !sWorld.iIgnoreMapIds_ACCount.count(plMover->GetMapId());
    map_block = !sWorld.iIgnoreMapIds_ACBlock.count(plMover->GetMapId());
    map_puni = !sWorld.iIgnoreMapIds_ACPuni.count(plMover->GetMapId());
}

// Calc variables for next AntiCheat activation
void AntiCheat::CalcVariablesSmall(Player* plMover, MovementInfo& pNewPacket, Unit *mover)
{
	// calculating section
    // current speed
    UnitMoveType move_type;
	if (pNewPacket.flags & MOVEMENTFLAG_FLYING)
		move_type = pNewPacket.flags & MOVEMENTFLAG_BACKWARD ? MOVE_FLIGHT_BACK : MOVE_FLIGHT;
	else if (pNewPacket.flags & MOVEMENTFLAG_SWIMMING)
		move_type = pNewPacket.flags & MOVEMENTFLAG_BACKWARD ? MOVE_SWIM_BACK : MOVE_SWIM;
	else if (pNewPacket.flags & MOVEMENTFLAG_WALKING)
		move_type = MOVE_WALK;
	// hmm... in first time after login player has MOVE_SWIMBACK instead MOVE_WALKBACK
	else
		move_type = pNewPacket.flags & MOVEMENTFLAG_BACKWARD ? MOVE_SWIM_BACK : MOVE_RUN;
    
    plMover->ac_local.SetLastSpeedRate(fSpeedRate);
	fSpeedRate = mover->GetSpeed(move_type);
	// end current speed

    if (fSpeedRate < plMover->ac_local.m_anti_Last_HSpeed && plMover->ac_local.m_anti_LastSpeedChangeTime == 0)
		plMover->ac_local.m_anti_LastSpeedChangeTime = pNewPacket.time + uint32(floor(((plMover->ac_local.m_anti_Last_HSpeed / fSpeedRate) * 1500)) + sWorld.getFloatConfig(CONFIG_AC_MAX_DISTANCE_DIFF_ALLOWED)); // 100ms above for random fluctuation

	if (pNewPacket.time > plMover->ac_local.m_anti_LastSpeedChangeTime)
	{
		plMover->ac_local.m_anti_Last_HSpeed = fSpeedRate;                                    // store current speed
		plMover->ac_local.m_anti_Last_VSpeed = -2.3f;
		plMover->ac_local.m_anti_LastSpeedChangeTime = 0;
	}
}

void AntiCheat::CalcVariables(Player* plMover, MovementInfo& pNewPacket, Unit *mover)
{
	// calculating section
    // current speed
    UnitMoveType move_type;
	if (pNewPacket.flags & MOVEMENTFLAG_FLYING)
		move_type = pNewPacket.flags & MOVEMENTFLAG_BACKWARD ? MOVE_FLIGHT_BACK : MOVE_FLIGHT;
	else if (pNewPacket.flags & MOVEMENTFLAG_SWIMMING)
		move_type = pNewPacket.flags & MOVEMENTFLAG_BACKWARD ? MOVE_SWIM_BACK : MOVE_SWIM;
	else if (pNewPacket.flags & MOVEMENTFLAG_WALKING)
		move_type = MOVE_WALK;
	// hmm... in first time after login player has MOVE_SWIMBACK instead MOVE_WALKBACK
	else
		move_type = pNewPacket.flags & MOVEMENTFLAG_BACKWARD ? MOVE_SWIM_BACK : MOVE_RUN;

	fSpeedRate = mover->GetSpeed(move_type);
	// end current speed

	// movement distance
	delta_x = plMover->m_transport || plMover->m_temp_transport ? 0 : plMover->GetPositionX() - pNewPacket.pos.GetPositionX();
	delta_y = plMover->m_transport || plMover->m_temp_transport ? 0 : plMover->GetPositionY() - pNewPacket.pos.GetPositionY();
	delta_z = plMover->m_transport || plMover->m_temp_transport ? 0 : plMover->GetPositionZ() - pNewPacket.pos.GetPositionZ();
	real_delta = plMover->m_transport || plMover->m_temp_transport ? 0 : pow(delta_x, 2) + pow(delta_y, 2);
	// end movement distance

    // Check if he have fly auras
	fly_auras = CanFly(plMover, pNewPacket);

	no_swim_flags = (pNewPacket.flags & MOVEMENTFLAG_SWIMMING) == 0;
	no_swim_in_water = !mover->IsInWater();
	no_swim_above_water = pNewPacket.pos.GetPositionZ()-7.0f >= mover->GetBaseMap()->GetWaterLevel(pNewPacket.pos.GetPositionX(),pNewPacket.pos.GetPositionY());
	no_swim_water = no_swim_in_water && no_swim_above_water;

	if (cClientTimeDelta < 0)
		cClientTimeDelta = 0;
	time_delta = cClientTimeDelta < 1500 ? float(cClientTimeDelta)/1000.0f : 1.5f; // normalize time - 1.5 second allowed for heavy loaded server

	tg_z = (real_delta != 0 && !fly_auras && no_swim_flags) ? (pow(delta_z, 2) / real_delta) : -99999; // movement distance tangents

	if (fSpeedRate < plMover->ac_local.m_anti_Last_HSpeed && plMover->ac_local.m_anti_LastSpeedChangeTime == 0)
		plMover->ac_local.m_anti_LastSpeedChangeTime = pNewPacket.time + uint32(floor(((plMover->ac_local.m_anti_Last_HSpeed / fSpeedRate) * 1500)) + sWorld.getFloatConfig(CONFIG_AC_MAX_DISTANCE_DIFF_ALLOWED));

	allowed_delta = plMover->m_transport || plMover->m_temp_transport ? 2 : // movement distance allowed delta
		pow(std::max(fSpeedRate, plMover->ac_local.m_anti_Last_HSpeed) * time_delta, 2)
	    + 2                                                                             // minimum allowed delta
		+ (tg_z > 2.2 ? pow(delta_z, 2)/2.37f : 0);                                     // mountain fall allowed delta

	if (pNewPacket.time > plMover->ac_local.m_anti_LastSpeedChangeTime)
	{
		plMover->ac_local.m_anti_Last_HSpeed = fSpeedRate;                                    // store current speed
		plMover->ac_local.m_anti_Last_VSpeed = -2.3f;
		plMover->ac_local.m_anti_LastSpeedChangeTime = 0;
	}
	// end calculating section

	// AntiGravity (thanks to Meekro)
	JumpHeight = plMover->ac_local.m_anti_JumpBaseZ - pNewPacket.pos.GetPositionZ();
}

bool AntiCheat::CanFly(Player* plMover, MovementInfo& pMovementInfo)
{
    if (plMover->IsUnderWater())
        return true;

    if (plMover->HasAuraType(SPELL_AURA_FLY) || 
        plMover->HasAuraType(SPELL_AURA_MOD_INCREASE_VEHICLE_FLIGHT_SPEED) || 
        plMover->HasAuraType(SPELL_AURA_MOD_INCREASE_MOUNTED_FLIGHT_SPEED) || 
        plMover->HasAuraType(SPELL_AURA_MOD_INCREASE_FLIGHT_SPEED) ||
        plMover->HasAuraType(SPELL_AURA_MOD_MOUNTED_FLIGHT_SPEED_ALWAYS) || 
        plMover->HasAuraType(SPELL_AURA_MOD_FLIGHT_SPEED_NOT_STACK) ||
        plMover->HasAuraType(SPELL_AURA_FEATHER_FALL) ||
        plMover->HasAuraType(SPELL_AURA_SAFE_FALL))
        return true;

    if (Creature* pCreature = plMover->GetVehicleCreatureBase())
        if (pCreature->GetCreatureInfo()->InhabitType & INHABIT_AIR)
            return true;

    if (plMover->HasUnitMovementFlag(MOVEMENTFLAG_JUMPING) || pMovementInfo.HasMovementFlag(MOVEMENTFLAG_JUMPING) || plMover->GetMap()->GetGameObject(pMovementInfo.t_guid))
        return true;

    return false;
}

void AntiCheat::LogCheat(eCheat m_cheat, Player* plMover, MovementInfo& pMovementInfo)
{
    std::string cheat_type = "";
	switch (m_cheat)
	{
		case CHEAT_MISTIMING:
            if (difftime_log_file >= sWorld.getIntConfig(CONFIG_AC_DELTA_LOG_FILE))
			{
				plMover->ac_local.m_logfile_time = cServerTime;    
			    sLog.outCheat("AC-%s Map %u Area %u, X:%f Y:%f Z:%f, mistiming exception #%d, mistiming: %dms", 
                    plMover->GetName(), plMover->GetMapId(), plMover->GetAreaId(), plMover->GetPositionX(), plMover->GetPositionY(), plMover->GetPositionZ(),
			        plMover->ac_local.m_anti_MistimingCount, sync_time);
            }
            cheat_type = "Mistiming";
            break;
		case CHEAT_GRAVITY:
            if (difftime_log_file >= sWorld.getIntConfig(CONFIG_AC_DELTA_LOG_FILE))
			{
				plMover->ac_local.m_logfile_time = cServerTime;   
		        sLog.outCheat("AC-%s Map %u Area %u, X:%f Y:%f Z:%f, AntiGravity exception. JumpHeight = %f, Allowed Vertical Speed = %f",
                    plMover->GetName(), plMover->GetMapId(), plMover->GetAreaId(), plMover->GetPositionX(), plMover->GetPositionY(), plMover->GetPositionZ(),
				    JumpHeight, plMover->ac_local.m_anti_Last_VSpeed);
            }
            cheat_type = "Gravity";
			break;
		case CHEAT_MULTIJUMP:
            if (difftime_log_file >= sWorld.getIntConfig(CONFIG_AC_DELTA_LOG_FILE))
			{
				plMover->ac_local.m_logfile_time = cServerTime;  
			    sLog.outCheat("AC-%s Map %u Area %u, X:%f Y:%f Z:%f, multi jump  exception",
                    plMover->GetName(), plMover->GetMapId(), plMover->GetAreaId(), plMover->GetPositionX(), plMover->GetPositionY(), plMover->GetPositionZ());
            }
            cheat_type = "MultiJump";
            break;
		case CHEAT_SPEED:
            if (difftime_log_file >= sWorld.getIntConfig(CONFIG_AC_DELTA_LOG_FILE))
			{
				plMover->ac_local.m_logfile_time = cServerTime;  
			    sLog.outCheat("AC-%s Map %u Area %u, X:%f Y:%f Z:%f, speed exception | cDelta=%f aDelta=%f | cSpeed=%f lSpeed=%f deltaTime=%f", plMover->GetName(), plMover->GetMapId(), 
				    plMover->GetPositionX(), plMover->GetPositionY(), plMover->GetPositionZ(), plMover->GetAreaId(), real_delta, allowed_delta, fSpeedRate, plMover->ac_local.m_anti_Last_HSpeed, time_delta);
            }	
            cheat_type = "Speed";
            break;
		case CHEAT_TELEPORT:
            if (difftime_log_file >= sWorld.getIntConfig(CONFIG_AC_DELTA_LOG_FILE))
			{
				plMover->ac_local.m_logfile_time = cServerTime;  
			    sLog.outCheat("AC-%s Map %u Area %u, X:%f Y:%f Z:%f, teleport exception | cDelta=%f aDelta=%f | cSpeed=%f lSpeed=%f deltaTime=%f", plMover->GetName(), plMover->GetMapId(), 
				    plMover->GetPositionX(), plMover->GetPositionY(), plMover->GetPositionZ(), plMover->GetAreaId(), real_delta, allowed_delta, fSpeedRate, plMover->ac_local.m_anti_Last_HSpeed, time_delta);
            }
            cheat_type = "Teleport";
            break;
		case CHEAT_MOUNTAIN:
            if (difftime_log_file >= sWorld.getIntConfig(CONFIG_AC_DELTA_LOG_FILE))
			{
				plMover->ac_local.m_logfile_time = cServerTime;  
			    sLog.outCheat("AC-%s Map %u Area %u, X:%f Y:%f Z:%f, umountain exception | tg_z=%f", 
                    plMover->GetName(), plMover->GetMapId(), plMover->GetAreaId(), plMover->GetPositionX(), plMover->GetPositionY(), plMover->GetPositionZ(),
                    tg_z);
            }
			cheat_type = "Mountain";
            break;
		case CHEAT_FLY:
            if (difftime_log_file >= sWorld.getIntConfig(CONFIG_AC_DELTA_LOG_FILE))
			{
				plMover->ac_local.m_logfile_time = cServerTime;  
			    sLog.outCheat("AC-%s Map %u Area %u X:%f Y:%f Z:%f flight exception. {SPELL_AURA_FLY=[%X]} {SPELL_AURA_MOD_INCREASE_MOUNTED_FLIGHT_SPEED=[%X]} {SPELL_AURA_MOD_INCREASE_FLIGHT_SPEED=[%X]} {SPELL_AURA_MOD_MOUNTED_FLIGHT_SPEED_ALWAYS=[%X]} {SPELL_AURA_MOD_FLIGHT_SPEED_NOT_STACK=[%X]} {plMover->GetVehicle()=[%X]}",
				    plMover->GetName(), plMover->GetMapId(), plMover->GetAreaId(), plMover->GetPositionX(), plMover->GetPositionY(), plMover->GetPositionZ(),
				    plMover->HasAuraType(SPELL_AURA_FLY), plMover->HasAuraType(SPELL_AURA_MOD_INCREASE_MOUNTED_FLIGHT_SPEED),
				    plMover->HasAuraType(SPELL_AURA_MOD_INCREASE_FLIGHT_SPEED), plMover->HasAuraType(SPELL_AURA_MOD_MOUNTED_FLIGHT_SPEED_ALWAYS),
				    plMover->HasAuraType(SPELL_AURA_MOD_FLIGHT_SPEED_NOT_STACK), plMover->GetVehicle());                        
            }
            cheat_type = "Fly";
            break;
		case CHEAT_WATERWALK:
            if (difftime_log_file >= sWorld.getIntConfig(CONFIG_AC_DELTA_LOG_FILE))
			{
				plMover->ac_local.m_logfile_time = cServerTime;  
			    sLog.outCheat("AC-%s Map %u, X: %f, Y: %f, waterwalk exception. {pMovementInfo=[%X]}{SPELL_AURA_WATER_WALK=[%X]}",
                    plMover->GetName(), plMover->GetMapId(), plMover->GetAreaId(), plMover->GetPositionX(), plMover->GetPositionY(), plMover->GetPositionZ(),
                    pMovementInfo.flags, plMover->HasAuraType(SPELL_AURA_WATER_WALK));
            }
            cheat_type = "Waterwalk";
            break;
		case CHEAT_TELETOPLANE:
            if (difftime_log_file >= sWorld.getIntConfig(CONFIG_AC_DELTA_LOG_FILE))
			{
				plMover->ac_local.m_logfile_time = cServerTime;  
			    sLog.outCheat("AC-%s Map %u, X: %f, Y: %f, Z: %f teleport to plane exception. Exception count: %d", plMover->GetName(), plMover->GetMapId(), 
				    plMover->GetPositionX(), plMover->GetPositionY(), plMover->GetPositionZ(), plMover->ac_local.m_anti_TeleToPlane_Count);
            }
            cheat_type = "TeleToPlane";
            break;
	}

    if (sWorld.getBoolConfig(CONFIG_AC_ENABLE_DBLOG))
        if (difftime_log_db >= sWorld.getIntConfig(CONFIG_AC_DELTA_LOG_DB))
        {		                    
            ExtraDatabase.PExecute("INSERT INTO cheat_log(cheat_type, guid, name, level, map, area, pos_x, pos_y, pos_z, date) VALUES ('%s', '%u', '%s', '%u', '%u', '%u', '%f', '%f', '%f', NOW())", 
                cheat_type.c_str(), plMover->GetGUIDLow(), plMover->GetName(), plMover->getLevel(), plMover->GetMapId(), plMover->GetAreaId(), plMover->GetPositionX(), plMover->GetPositionY(), plMover->GetPositionZ());
            plMover->ac_local.m_logdb_time = cServerTime;
        }
}

bool AntiCheat::CheckMistiming(Player* plMover, Vehicle *vehMover, MovementInfo& pMovementInfo)
{		
	const int32 GetMistimingDelta = abs(int32(sWorld.getIntConfig(CONFIG_AC_ENABLE_MISTIMING_DELTHA)));
	
	if (sync_time > GetMistimingDelta)
	{
		cClientTimeDelta = cServerTimeDelta;
        if (map_count)
		    ++(plMover->ac_local.m_anti_MistimingCount);

		const bool bMistimingModulo = plMover->ac_local.m_anti_MistimingCount % 50 == 0;

		if (bMistimingModulo)
		{
            ++(plMover->ac_local.m_CheatList[CHEAT_MISTIMING]);		
            cheat_find = true;
			LogCheat(CHEAT_MISTIMING, plMover, pMovementInfo);
		}
        if (map_block && plMover->ac_local.m_CheatList[CHEAT_MISTIMING] &&
            plMover->ac_local.m_CheatList[CHEAT_MISTIMING] >= sWorld.getIntConfig(CONFIG_AC_MISTIMING_BLOCK_COUNT))     
	    {
		    if (vehMover)
			    vehMover->Die();
		    // Tell the player "Sure, you can fly!"
		    {
			    WorldPacket data(SMSG_MOVE_SET_CAN_FLY, 12);
			    data.append(plMover->GetPackGUID());
			    data << uint32(0);
			    plMover->GetSession()->SendPacket(&data);
		    }
		    // Then tell the player "Wait, no, you can't."
		    {
			    WorldPacket data(SMSG_MOVE_UNSET_CAN_FLY, 12);
			    data.append(plMover->GetPackGUID());
			    data << uint32(0);
			    plMover->GetSession()->SendPacket(&data);
		    }
		    plMover->FallGround(2);
		    return false;
	    }
	}
	return true; 
}

bool AntiCheat::CheckAntiGravity(Player* plMover, Vehicle *vehMover, MovementInfo& pMovementInfo)
{
    if (!fly_auras && no_swim_in_water && plMover->ac_local.m_anti_JumpBaseZ != 0 && JumpHeight < plMover->ac_local.m_anti_Last_VSpeed)
	{
        if (map_count)
            ++(plMover->ac_local.m_CheatList[CHEAT_GRAVITY]);
        cheat_find = true;
		LogCheat(CHEAT_GRAVITY, plMover, pMovementInfo);
        if (map_block && plMover->ac_local.m_CheatList[CHEAT_GRAVITY] &&
            plMover->ac_local.m_CheatList[CHEAT_GRAVITY] >= sWorld.getIntConfig(CONFIG_AC_ANTIGRAVITY_BLOCK_COUNT))
	    {
		    if (vehMover)
			    vehMover->Die();
		    // Tell the player "Sure, you can fly!"
		    {
			    WorldPacket data(SMSG_MOVE_SET_CAN_FLY, 12);
			    data.append(plMover->GetPackGUID());
			    data << uint32(0);
			    plMover->GetSession()->SendPacket(&data);
		    }
		    // Then tell the player "Wait, no, you can't."
		    {
			    WorldPacket data(SMSG_MOVE_UNSET_CAN_FLY, 12);
			    data.append(plMover->GetPackGUID());
			    data << uint32(0);
			    plMover->GetSession()->SendPacket(&data);
		    }
		    plMover->FallGround(2);
		    return false;
	    }
	}
	return true; 
}

bool AntiCheat::CheckAntiMultiJump(Player* plMover, Vehicle *vehMover, MovementInfo& pNewPacket, uint32 uiOpcode)
{
    if (uiOpcode != MSG_MOVE_JUMP)
        return true;

    if (!fly_auras && no_swim_water && plMover->ac_local.GetLastOpcode() == MSG_MOVE_JUMP)
	{
		if (plMover->ac_local.m_anti_JumpCount >= 1)
        {
            if (map_count)
                ++(plMover->ac_local.m_CheatList[CHEAT_MULTIJUMP]);
            cheat_find = true;
			LogCheat(CHEAT_MULTIJUMP, plMover,  pNewPacket);
			if (map_block && plMover->ac_local.m_CheatList[CHEAT_MULTIJUMP] &&
                plMover->ac_local.m_CheatList[CHEAT_MULTIJUMP] >= sWorld.getIntConfig(CONFIG_AC_ANTIMULTIJUMP_BLOCK_COUNT))
		    {
			    // don't process new jump packet
			    if (vehMover)
				    vehMover->Die();
			    // Tell the player "Sure, you can fly!"
			    {
				    WorldPacket data(SMSG_MOVE_SET_CAN_FLY, 12);
				    data.append(plMover->GetPackGUID());
				    data << uint32(0);
				    plMover->GetSession()->SendPacket(&data);
			    }
			    // Then tell the player "Wait, no, you can't."
			    {
				    WorldPacket data(SMSG_MOVE_UNSET_CAN_FLY, 12);
				    data.append(plMover->GetPackGUID());
				    data << uint32(0);
				    plMover->GetSession()->SendPacket(&data);
			    }
			    plMover->FallGround(2);
			    plMover->ac_local.m_anti_JumpCount = 0;
			    return false;
		    }
		}
		else
		{
			plMover->ac_local.m_anti_JumpCount += 1;
			plMover->ac_local.m_anti_JumpBaseZ =  pNewPacket.pos.GetPositionZ();
		}
	}
	else
		plMover->ac_local.m_anti_JumpCount = 0;
		
	return true; 
}

bool AntiCheat::CheckAntiSpeedTeleport(Player* plMover, Vehicle *vehMover, MovementInfo& pNewPacket, uint32 uiOpcode)
{
    // strange packet
    if (uiOpcode == MSG_MOVE_SET_FACING)        
        return true;

    // just to prevent false reports
    if (plMover->GetVehicle())
        return true;

    // the best way is checking the ip of the target, if it is the same this check should return.
    if (plMover->GetMotionMaster()->GetCurrentMovementGeneratorType() == TARGETED_MOTION_TYPE)
        return true;

    // it will make false reports
    if (plMover->IsFalling() && fly_auras)
        return true;

	if (real_delta > allowed_delta)
	{          
        cheat_find = true;
		if (real_delta < 4900.0f)
        {
            if (map_count)
                ++(plMover->ac_local.m_CheatList[CHEAT_SPEED]);
            LogCheat(CHEAT_SPEED, plMover, pNewPacket);
            if (map_block && plMover->ac_local.m_CheatList[CHEAT_SPEED] &&
                plMover->ac_local.m_CheatList[CHEAT_SPEED] >= sWorld.getIntConfig(CONFIG_AC_ANTISPEED_BLOCK_COUNT))
            {
	            if (vehMover)
		            vehMover->Die();
	            plMover->FallGround(2);
	            return false;
            }
        }
        else
        {
            if (map_count)
                ++(plMover->ac_local.m_CheatList[CHEAT_TELEPORT]);
			LogCheat(CHEAT_TELEPORT, plMover, pNewPacket);
            if (map_block && plMover->ac_local.m_CheatList[CHEAT_TELEPORT] &&
                plMover->ac_local.m_CheatList[CHEAT_TELEPORT] >= sWorld.getIntConfig(CONFIG_AC_ANTITELE_BLOCK_COUNT))
            {
	            if (vehMover)
		            vehMover->Die();
	            plMover->FallGround(2);
	            return false;
            }
        }
	}    
	return true;
}

// mountain hack checks // 1.56f (delta_z < GetPlayer()->m_anti_Last_VSpeed))
bool AntiCheat::CheckAntiMountain(Player* plMover, Vehicle *vehMover, MovementInfo& pMovementInfo)
{
	if (delta_z < plMover->ac_local.m_anti_Last_VSpeed && plMover->ac_local.m_anti_JumpCount == 0 && tg_z > 2.37f)
	{
        if (map_count)
            ++(plMover->ac_local.m_CheatList[CHEAT_MOUNTAIN]);
        cheat_find = true;
		LogCheat(CHEAT_MOUNTAIN, plMover, pMovementInfo);
        if (map_block && plMover->ac_local.m_CheatList[CHEAT_MOUNTAIN] &&
            plMover->ac_local.m_CheatList[CHEAT_MOUNTAIN] >= sWorld.getIntConfig(CONFIG_AC_ANTIMOUNTAIN_BLOCK_COUNT))
	    {
		    if (vehMover)
			    vehMover->Die();
		    return false;
	    }
	}
	return true;
}

bool AntiCheat::CheckAntiFly(Player* plMover, Vehicle *vehMover, MovementInfo& pOldPacket, MovementInfo& pNewPacket)
{	
    if (plMover->IsFalling())
        return true;

    // we like check heartbeat movements
    if (pNewPacket.GetMovementFlags() != plMover->GetUnitMovementFlags() || 
        pNewPacket.GetMovementFlags() != pOldPacket.GetMovementFlags())
        return true;

	if (!fly_auras && (pNewPacket.flags & (MOVEMENTFLAG_CAN_FLY | MOVEMENTFLAG_CAN_FLY | MOVEMENTFLAG_FLYING)))
	{
        if (map_count)
            ++(plMover->ac_local.m_CheatList[CHEAT_FLY]);
        cheat_find = true;
		LogCheat(CHEAT_FLY, plMover, pNewPacket);
        if (map_block && plMover->ac_local.m_CheatList[CHEAT_FLY] &&
            plMover->ac_local.m_CheatList[CHEAT_FLY] >= sWorld.getIntConfig(CONFIG_AC_ANTIFLY_BLOCK_COUNT))
	    {
		    if (vehMover)
			    vehMover->Die();
		    // Tell the player "Sure, you can fly!"
		    {
			    WorldPacket data(SMSG_MOVE_SET_CAN_FLY, 12);
			    data.append(plMover->GetPackGUID());
			    data << uint32(0);
			    plMover->GetSession()->SendPacket(&data);
		    }
		    // Then tell the player "Wait, no, you can't."
		    {
			    WorldPacket data(SMSG_MOVE_UNSET_CAN_FLY, 12);
			    data.append(plMover->GetPackGUID());
			    data << uint32(0);
			    plMover->GetSession()->SendPacket(&data);
		    }
		    plMover->FallGround(2);
		    return false;
	    }
	}    
	return true;
}

bool AntiCheat::CheckAntiWaterwalk(Player* plMover, Vehicle *vehMover, MovementInfo& pOldPacket, MovementInfo& pNewPacket)
{
    if (!plMover->isAlive())
        return true;

    // 1066 -> Aquatic Form
    if (plMover->HasAura(1066))
        return true;

    if (plMover->IsUnderWater())
        return true;

    float water_level = plMover->GetBaseMap()->GetWaterLevel(plMover->GetPositionX(),plMover->GetPositionY());
    float water_level_diff = fabs(water_level - plMover->GetPositionZ());

    if (water_level_diff > 0.15f)
        return true;

    // we like check heartbeat movements
    if (pNewPacket.GetMovementFlags() != plMover->GetUnitMovementFlags() || 
        pNewPacket.GetMovementFlags() != pOldPacket.GetMovementFlags())
        return true;

	if (plMover->HasAuraType(SPELL_AURA_WATER_WALK) ||
        plMover->HasAuraType(SPELL_AURA_FEATHER_FALL) ||
        plMover->HasAuraType(SPELL_AURA_SAFE_FALL) ||
        plMover->HasAuraType(SPELL_AURA_GHOST))
        return true;

	if (plMover->HasUnitMovementFlag(MOVEMENTFLAG_WATERWALKING) &&
        !fly_auras && !plMover->IsFlying())
	{
        if (map_count)
            ++(plMover->ac_local.m_CheatList[CHEAT_WATERWALK]);
        cheat_find = true;
		LogCheat(CHEAT_WATERWALK, plMover, pNewPacket);
		if (map_block && plMover->ac_local.m_CheatList[CHEAT_WATERWALK] && 
            plMover->ac_local.m_CheatList[CHEAT_WATERWALK] >= sWorld.getIntConfig(CONFIG_AC_ANTIWATERWALK_BLOCK_COUNT))
	    {
		    if (vehMover)
			    vehMover->Die();
		    // Tell the player "Sure, you can fly!"
		    {
			    WorldPacket data(SMSG_MOVE_SET_CAN_FLY, 12);
			    data.append(plMover->GetPackGUID());
			    data << uint32(0);
			    plMover->GetSession()->SendPacket(&data);
		    }
		    // Then tell the player "Wait, no, you can't."
		    {
			    WorldPacket data(SMSG_MOVE_UNSET_CAN_FLY, 12);
			    data.append(plMover->GetPackGUID());
			    data << uint32(0);
			    plMover->GetSession()->SendPacket(&data);
		    }
		    plMover->FallGround(2);
		    return false;
	    }
	}
	return true;
}

bool AntiCheat::CheckAntiTeleToPlane(Player* plMover, Vehicle *vehMover, MovementInfo& pMovementInfo)
{
	if (no_swim_in_water && pMovementInfo.pos.GetPositionZ() < 0.0001f && pMovementInfo.pos.GetPositionZ() > -0.0001f)
    {           
		if (const Map *map = plMover->GetMap())
		{
			float plane_z = map->GetHeight(pMovementInfo.pos.GetPositionX(), pMovementInfo.pos.GetPositionY(), MAX_HEIGHT) - pMovementInfo.pos.GetPositionZ();
			plane_z = (plane_z < -500.0f) ? 0.0f : plane_z; // check holes in height map
			if (plane_z > 0.1f || plane_z < -0.1f)
			{
				++(plMover->ac_local.m_anti_TeleToPlane_Count);
				if (plMover->ac_local.m_anti_TeleToPlane_Count > sWorld.getIntConfig(CONFIG_AC_ENABLE_ANTITELETOPLANE_ALARMS))
				{
                    if (map_count)
                        ++(plMover->ac_local.m_CheatList[CHEAT_TELETOPLANE]);
                    cheat_find = true;
					LogCheat(CHEAT_TELETOPLANE, plMover, pMovementInfo);
					if (map_block && plMover->ac_local.m_CheatList[CHEAT_TELETOPLANE] &&
                        plMover->ac_local.m_CheatList[CHEAT_TELETOPLANE] >= sWorld.getIntConfig(CONFIG_AC_ANTITELETOPLANE_BLOCK_COUNT))
	        	    {						
					    if (vehMover)
						    vehMover->Die();
					    return false;
				    }
				}
			}
		}
    }
	else
		plMover->ac_local.m_anti_TeleToPlane_Count = 0;
	return true;
}