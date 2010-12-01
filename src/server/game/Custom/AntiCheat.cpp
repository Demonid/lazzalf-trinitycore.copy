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
 * @Authors : Lazzalf based on AC2
 *
 **/

AntiCheat::AntiCheat()
{
	cheat_find = false;
}

AntiCheat_Local::AntiCheat_Local()
{
	ac_block = false;

	ac_delta = 0;
    ac_goactivate = false;

    for (int i = 0; i < MAX_CHEAT; i++)
        m_CheatList[i] = 0;
    m_CheatList_reset_diff = 0;
	
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
    if (ac_goactivate)
    {
        ac_goactivate = false;
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
            // It's come from sleep
            ac_goactivate = true;
        }
    }
    else if (ac_delta < 0)
    {
        if (ac_delta < diff)
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
		for (int i = 0; i < MAX_CHEAT; i++)
            m_CheatList[i] = 0;

    m_CheatList_reset_diff = sWorld.getIntConfig(CONFIG_AC_RESET_CHEATLIST_DELTA);
}

bool AntiCheat::Check(Player* plMover, Vehicle *vehMover, uint16 opcode, MovementInfo& movementInfo, Unit *mover)
{
	if (!plMover)
		return true;
	
	if (!sWorld.getBoolConfig(CONFIG_AC_ENABLE))
		return true;
		
	if (plMover->GetSession() && plMover->GetSession()->GetSecurity() >= int32(sWorld.getIntConfig(CONFIG_AC_DISABLE_GM_LEVEL)))
		return true;
	
	// Calc Delthas for AntiCheat
	CalcDeltas(plMover, movementInfo);

    // Clean player cheatlist
    plMover->ac_local.ResetCheatList(cServerTimeDelta);
	
    // Diff for AntiCheat sleep
	if (!plMover->ac_local.GetAndUpdateDelta(int32(cServerTimeDelta)))
        return true;
  
    // AntiCheat Block (not used for now)
	//if (plMover->ac_local.GetBlock())
	//	return true;	

	// Set to false if find a Cheat
	bool check_passed = true;
	
	// Mistiming Cheat
	if (sWorld.getBoolConfig(CONFIG_AC_ENABLE_MISTIMING))
		if (!CheckMistiming(plMover, vehMover, movementInfo))
			check_passed = false;

    // check taxi flight
    const uint32 curDest = plMover->m_taxi.GetTaxiDestination();	
	if (!curDest)
	{            
        if (plMover->ac_local.ac_goactivate)
        {
            CalcVariablesSmall(plMover, movementInfo, mover);
            return check_passed;
        }
        else
		    CalcVariables(plMover, movementInfo, mover);

        // Gravity Cheat
		if (sWorld.getBoolConfig(CONFIG_AC_ENABLE_ANTIGRAVITY))
			if (!CheckAntiGravity(plMover, vehMover, movementInfo))
				check_passed = false;

        // MultiJump Cheat
		if (sWorld.getBoolConfig(CONFIG_AC_ENABLE_ANTIMULTIJUMP) && opcode == MSG_MOVE_JUMP)
			if (!CheckAntiMultiJump(plMover, vehMover, movementInfo))
				check_passed = false;

        // Speed and Tele Cheat
		if (sWorld.getBoolConfig(CONFIG_AC_ENABLE_ANTISPEEDTELE))
			if (!CheckAntiSpeedTeleport(plMover, vehMover, movementInfo))
				check_passed = false;

        // Mountain Cheat
		if (sWorld.getBoolConfig(CONFIG_AC_ENABLE_ANTIMOUNTAIN))
			if (!CheckAntiMountain(plMover, vehMover, movementInfo))
				check_passed = false;

        // Fly Cheat
		if (sWorld.getBoolConfig(CONFIG_AC_ENABLE_ANTIFLY))
			if (!CheckAntiFly(plMover, vehMover, movementInfo))
				check_passed = false;

        // Waterwalk Cheat
		if (sWorld.getBoolConfig(CONFIG_AC_ENABLE_ANTIWATERWALK))
			if (!CheckAntiWaterwalk(plMover, vehMover, movementInfo))
				check_passed = false;

        // Tele To Plane Cheat
		if (sWorld.getBoolConfig(CONFIG_AC_ENABLE_ANTITELETOPLANE))
			if (!CheckAntiTeleToPlane(plMover, vehMover, movementInfo))
				check_passed = false;
	}
    if (cheat_find)
        check_passed = AntiCheatPunisher(plMover, movementInfo);
	return check_passed;
}

bool AntiCheat::AntiCheatPunisher(Player* plMover, MovementInfo& movementInfo)
{
    if (!sWorld.getIntConfig(CONFIG_AC_PUNI_TYPE))
        return true;

    if (plMover->getLevel() > sWorld.getIntConfig(CONFIG_AC_PUNI_LEVEL_MAX))
        return true;

    if (sWorld.getBoolConfig(CONFIG_AC_PUNI_MAP_SMALL))
        if (plMover->GetMapId() != 0 && plMover->GetMapId() != 1)
            return true;

    bool find = false;
    for (int i = 0; i < MAX_CHEAT; i++)
    {
        if (plMover->ac_local.m_CheatList[i] >= sWorld.getIntConfig(CONFIG_AC_PUNI_COUNT))
            find = true;
    }

    if (!find)
        return true;

    std::string announce = "";
    switch (CONFIG_AC_PUNI_TYPE)
    {
        case PUNI_NONE:
            return true;
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
            sWorld.BanAccount(BAN_CHARACTER,plMover->GetName(),sConfig.GetStringDefault("Anticheat.Punisher.BanTime", "-1"),"Cheat","AntiCheatPunisher");
            announce = "AntiCheatPunisher ha bannato il player ";
            announce += plMover->GetName();
            announce += "per uso di Hack";
            sWorld.SendServerMessage(SERVER_MSG_STRING,announce.c_str());
            return false;
        case PUNI_BAN_ACC:
            {
                sLog.outCheat("AC-Punisher-%s Map %u Area %u, X:%f Y:%f Z:%f, PUNISHER BAN_ACCOUNT",
                        plMover->GetName(), plMover->GetMapId(), plMover->GetAreaId(), plMover->GetPositionX(), plMover->GetPositionY(), plMover->GetPositionZ());
                announce = "AntiCheatPunisher ha bannato l'account del player ";
                announce += plMover->GetName();
                announce += "per uso di Hack";
                sWorld.SendServerMessage(SERVER_MSG_STRING,announce.c_str());
                QueryResult result = LoginDatabase.PQuery("SELECT username FROM account WHERE id=%u", plMover->GetSession()->GetAccountId());
                if (result)
                {

                    Field *fields = result->Fetch();
                    std::string Username = fields[0].GetString();
                    if(!Username.empty())
                    {
                        sWorld.BanAccount(BAN_ACCOUNT, Username,sConfig.GetStringDefault("Anticheat.Punisher.BanTime", "-1"),"Cheat","AntiCheatPunisher");
                    }
                }
                return false;
            }
        case PUNI_BAN_IP:
            {
                sLog.outCheat("AC-Punisher-%s Map %u Area %u, X:%f Y:%f Z:%f, PUNISHER BAN_IP",
                        plMover->GetName(), plMover->GetMapId(), plMover->GetAreaId(), plMover->GetPositionX(), plMover->GetPositionY(), plMover->GetPositionZ());
                announce = "AntiCheatPunisher ha bannato l'ip del player ";
                announce += plMover->GetName();
                announce += "per uso di Hack";
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

void AntiCheat::CalcDeltas(Player* plMover, MovementInfo& movementInfo)
{
	// calc time deltas
	cClientTimeDelta = 1500;
	if (plMover->ac_local.m_anti_LastClientTime != 0)
	{
		cClientTimeDelta = movementInfo.time - plMover->ac_local.m_anti_LastClientTime;
		plMover->ac_local.m_anti_DeltaClientTime += cClientTimeDelta;
		plMover->ac_local.m_anti_LastClientTime = movementInfo.time;
	}
	else
		plMover->ac_local.m_anti_LastClientTime = movementInfo.time;

	cServerTime = getMSTime();

	difftime_log_file = cServerTime - plMover->ac_local.m_logfile_time;
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
}

// Calc variables for next AntiCheat activation
void AntiCheat::CalcVariablesSmall(Player* plMover, MovementInfo& movementInfo, Unit *mover)
{
	// calculating section
    // current speed
    UnitMoveType move_type;
	if (movementInfo.flags & MOVEMENTFLAG_FLYING)
		move_type = movementInfo.flags & MOVEMENTFLAG_BACKWARD ? MOVE_FLIGHT_BACK : MOVE_FLIGHT;
	else if (movementInfo.flags & MOVEMENTFLAG_SWIMMING)
		move_type = movementInfo.flags & MOVEMENTFLAG_BACKWARD ? MOVE_SWIM_BACK : MOVE_SWIM;
	else if (movementInfo.flags & MOVEMENTFLAG_WALKING)
		move_type = MOVE_WALK;
	// hmm... in first time after login player has MOVE_SWIMBACK instead MOVE_WALKBACK
	else
		move_type = movementInfo.flags & MOVEMENTFLAG_BACKWARD ? MOVE_SWIM_BACK : MOVE_RUN;

	current_speed = mover->GetSpeed(move_type);
	// end current speed

    if (current_speed < plMover->ac_local.m_anti_Last_HSpeed && plMover->ac_local.m_anti_LastSpeedChangeTime == 0)
		plMover->ac_local.m_anti_LastSpeedChangeTime = movementInfo.time + uint32(floor(((plMover->ac_local.m_anti_Last_HSpeed / current_speed) * 1500)) + 100); // 100ms above for random fluctuation

	if (movementInfo.time > plMover->ac_local.m_anti_LastSpeedChangeTime)
	{
		plMover->ac_local.m_anti_Last_HSpeed = current_speed;                                    // store current speed
		plMover->ac_local.m_anti_Last_VSpeed = -2.3f;
		plMover->ac_local.m_anti_LastSpeedChangeTime = 0;
	}
}

void AntiCheat::CalcVariables(Player* plMover, MovementInfo& movementInfo, Unit *mover)
{
	// calculating section
    // current speed
    UnitMoveType move_type;
	if (movementInfo.flags & MOVEMENTFLAG_FLYING)
		move_type = movementInfo.flags & MOVEMENTFLAG_BACKWARD ? MOVE_FLIGHT_BACK : MOVE_FLIGHT;
	else if (movementInfo.flags & MOVEMENTFLAG_SWIMMING)
		move_type = movementInfo.flags & MOVEMENTFLAG_BACKWARD ? MOVE_SWIM_BACK : MOVE_SWIM;
	else if (movementInfo.flags & MOVEMENTFLAG_WALKING)
		move_type = MOVE_WALK;
	// hmm... in first time after login player has MOVE_SWIMBACK instead MOVE_WALKBACK
	else
		move_type = movementInfo.flags & MOVEMENTFLAG_BACKWARD ? MOVE_SWIM_BACK : MOVE_RUN;

	current_speed = mover->GetSpeed(move_type);
	// end current speed

	vehicleEntry = 0;
	if (plMover->GetVehicle())
	{
		if (plMover->GetVehicleCreatureBase())
			vehicleEntry = plMover->GetVehicleCreatureBase()->GetEntry();
	}

	// movement distance
	delta_x = plMover->m_transport || plMover->m_temp_transport ? 0 : plMover->GetPositionX() - movementInfo.pos.GetPositionX();
	delta_y = plMover->m_transport || plMover->m_temp_transport ? 0 : plMover->GetPositionY() - movementInfo.pos.GetPositionY();
	delta_z = plMover->m_transport || plMover->m_temp_transport ? 0 : plMover->GetPositionZ() - movementInfo.pos.GetPositionZ();
	real_delta = plMover->m_transport || plMover->m_temp_transport ? 0 : pow(delta_x, 2) + pow(delta_y, 2);
	// end movement distance

	no_fly_auras = !(plMover->HasAuraType(SPELL_AURA_FLY) || plMover->HasAuraType(SPELL_AURA_MOD_INCREASE_VEHICLE_FLIGHT_SPEED)
		|| plMover->HasAuraType(SPELL_AURA_MOD_INCREASE_MOUNTED_FLIGHT_SPEED) || plMover->HasAuraType(SPELL_AURA_MOD_INCREASE_FLIGHT_SPEED)
		|| plMover->HasAuraType(SPELL_AURA_MOD_MOUNTED_FLIGHT_SPEED_ALWAYS) || plMover->HasAuraType(SPELL_AURA_MOD_FLIGHT_SPEED_NOT_STACK)
		|| (plMover->GetVehicle() && vehicleEntry == FROSTBROOD_VANQUISHER));
	no_fly_flags = (movementInfo.flags & (MOVEMENTFLAG_CAN_FLY | MOVEMENTFLAG_CAN_FLY | MOVEMENTFLAG_FLYING)) == 0;

	no_swim_flags = (movementInfo.flags & MOVEMENTFLAG_SWIMMING) == 0;
	no_swim_in_water = !mover->IsInWater();
	no_swim_above_water = movementInfo.pos.GetPositionZ()-7.0f >= mover->GetBaseMap()->GetWaterLevel(movementInfo.pos.GetPositionX(),movementInfo.pos.GetPositionY());
	no_swim_water = no_swim_in_water && no_swim_above_water;

	no_waterwalk_flags = (movementInfo.flags & MOVEMENTFLAG_WATERWALKING) == 0;
	no_waterwalk_auras = !(plMover->HasAuraType(SPELL_AURA_WATER_WALK) || plMover->HasAuraType(SPELL_AURA_GHOST));

	if (cClientTimeDelta < 0)
		cClientTimeDelta = 0;
	time_delta = cClientTimeDelta < 1500 ? float(cClientTimeDelta)/1000.0f : 1.5f; // normalize time - 1.5 second allowed for heavy loaded server

	tg_z = (real_delta != 0 && no_fly_auras && no_swim_flags) ? (pow(delta_z, 2) / real_delta) : -99999; // movement distance tangents

	if (current_speed < plMover->ac_local.m_anti_Last_HSpeed && plMover->ac_local.m_anti_LastSpeedChangeTime == 0)
		plMover->ac_local.m_anti_LastSpeedChangeTime = movementInfo.time + uint32(floor(((plMover->ac_local.m_anti_Last_HSpeed / current_speed) * 1500)) + 100); // 100ms above for random fluctuation

	allowed_delta = plMover->m_transport || plMover->m_temp_transport ? 2 : // movement distance allowed delta
		pow(std::max(current_speed, plMover->ac_local.m_anti_Last_HSpeed) * time_delta, 2)
	    + 2                                                                             // minimum allowed delta
		+ (tg_z > 2.2 ? pow(delta_z, 2)/2.37f : 0);                                     // mountain fall allowed delta

	if (movementInfo.time > plMover->ac_local.m_anti_LastSpeedChangeTime)
	{
		plMover->ac_local.m_anti_Last_HSpeed = current_speed;                                    // store current speed
		plMover->ac_local.m_anti_Last_VSpeed = -2.3f;
		plMover->ac_local.m_anti_LastSpeedChangeTime = 0;
	}
	// end calculating section

	// AntiGravity (thanks to Meekro)
	JumpHeight = plMover->ac_local.m_anti_JumpBaseZ - movementInfo.pos.GetPositionZ();
}

void AntiCheat::LogCheat(eCheat m_cheat, Player* plMover, MovementInfo& movementInfo)
{
    std::string cheat_type = "";
	switch (m_cheat)
	{
		case CHEAT_MISTIMING:
            if (difftime_log_file > sWorld.getIntConfig(CONFIG_AC_DELTA_LOG_FILE))
			{
				plMover->ac_local.m_logfile_time = cServerTime;    
			    sLog.outCheat("AC-%s Map %u Area %u, X:%f Y:%f Z:%f, mistiming exception #%d, mistiming: %dms", 
                    plMover->GetName(), plMover->GetMapId(), plMover->GetAreaId(), plMover->GetPositionX(), plMover->GetPositionY(), plMover->GetPositionZ(),
			        plMover->ac_local.m_anti_MistimingCount, sync_time);
            }
            cheat_type = "Mistiming";
            break;
		case CHEAT_GRAVITY:
            if (difftime_log_file > sWorld.getIntConfig(CONFIG_AC_DELTA_LOG_FILE))
			{
				plMover->ac_local.m_logfile_time = cServerTime;   
		        sLog.outCheat("AC-%s Map %u Area %u, X:%f Y:%f Z:%f, AntiGravity exception. JumpHeight = %f, Allowed Vertical Speed = %f",
                    plMover->GetName(), plMover->GetMapId(), plMover->GetAreaId(), plMover->GetPositionX(), plMover->GetPositionY(), plMover->GetPositionZ(),
				    JumpHeight, plMover->ac_local.m_anti_Last_VSpeed);
            }
            cheat_type = "Gravity";
			break;
		case CHEAT_MULTIJUMP:
            if (difftime_log_file > sWorld.getIntConfig(CONFIG_AC_DELTA_LOG_FILE))
			{
				plMover->ac_local.m_logfile_time = cServerTime;  
			    sLog.outCheat("AC-%s Map %u Area %u, X:%f Y:%f Z:%f, multi jump  exception",
                    plMover->GetName(), plMover->GetMapId(), plMover->GetAreaId(), plMover->GetPositionX(), plMover->GetPositionY(), plMover->GetPositionZ());
            }
            cheat_type = "MultiJump";
            break;
		case CHEAT_SPEED:
            if (difftime_log_file > sWorld.getIntConfig(CONFIG_AC_DELTA_LOG_FILE))
			{
				plMover->ac_local.m_logfile_time = cServerTime;  
			    sLog.outCheat("AC-%s Map %u Area %u, X:%f Y:%f Z:%f, speed exception | cDelta=%f aDelta=%f | cSpeed=%f lSpeed=%f deltaTime=%f", plMover->GetName(), plMover->GetMapId(), 
				    plMover->GetPositionX(), plMover->GetPositionY(), plMover->GetPositionZ(), plMover->GetAreaId(), real_delta, allowed_delta, current_speed, plMover->ac_local.m_anti_Last_HSpeed, time_delta);
            }	
            cheat_type = "Speed";
            break;
		case CHEAT_TELEPORT:
            if (difftime_log_file > sWorld.getIntConfig(CONFIG_AC_DELTA_LOG_FILE))
			{
				plMover->ac_local.m_logfile_time = cServerTime;  
			    sLog.outCheat("AC-%s Map %u Area %u, X:%f Y:%f Z:%f, teleport exception | cDelta=%f aDelta=%f | cSpeed=%f lSpeed=%f deltaTime=%f", plMover->GetName(), plMover->GetMapId(), 
				    plMover->GetPositionX(), plMover->GetPositionY(), plMover->GetPositionZ(), plMover->GetAreaId(), real_delta, allowed_delta, current_speed, plMover->ac_local.m_anti_Last_HSpeed, time_delta);
            }
            cheat_type = "Teleport";
            break;
		case CHEAT_MOUNTAIN:
            if (difftime_log_file > sWorld.getIntConfig(CONFIG_AC_DELTA_LOG_FILE))
			{
				plMover->ac_local.m_logfile_time = cServerTime;  
			    sLog.outCheat("AC-%s Map %u Area %u, X:%f Y:%f Z:%f, umountain exception | tg_z=%f", 
                    plMover->GetName(), plMover->GetMapId(), plMover->GetAreaId(), plMover->GetPositionX(), plMover->GetPositionY(), plMover->GetPositionZ(),
                    tg_z);
            }
			cheat_type = "Mountain";
            break;
		case CHEAT_FLY:
            if (difftime_log_file > sWorld.getIntConfig(CONFIG_AC_DELTA_LOG_FILE))
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
            if (difftime_log_file > sWorld.getIntConfig(CONFIG_AC_DELTA_LOG_FILE))
			{
				plMover->ac_local.m_logfile_time = cServerTime;  
			    sLog.outCheat("AC-%s Map %u, X: %f, Y: %f, waterwalk exception. {movementInfo=[%X]}{SPELL_AURA_WATER_WALK=[%X]}",
                    plMover->GetName(), plMover->GetMapId(), plMover->GetAreaId(), plMover->GetPositionX(), plMover->GetPositionY(), plMover->GetPositionZ(),
                    movementInfo.flags, plMover->HasAuraType(SPELL_AURA_WATER_WALK));
            }
            cheat_type = "Waterwalk";
            break;
		case CHEAT_TELETOPLANE:
            if (difftime_log_file > sWorld.getIntConfig(CONFIG_AC_DELTA_LOG_FILE))
			{
				plMover->ac_local.m_logfile_time = cServerTime;  
			    sLog.outCheat("AC-%s Map %u, X: %f, Y: %f, Z: %f teleport to plane exception. Exception count: %d", plMover->GetName(), plMover->GetMapId(), 
				    plMover->GetPositionX(), plMover->GetPositionY(), plMover->GetPositionZ(), plMover->ac_local.m_anti_TeleToPlane_Count);
            }
            cheat_type = "TeleToPlane";
            break;
	}

    if (sWorld.getBoolConfig(CONFIG_AC_ENABLE_DBLOG))
        if (difftime_log_db > sWorld.getIntConfig(CONFIG_AC_DELTA_LOG_DB))
        {		                    
            ExtraDatabase.PExecute("INSERT INTO cheat_log(cheat_type, guid, name, level, map, area, pos_x, pos_y, pos_z, date) VALUES ('%s', '%u', '%s', '%u', '%u', '%f', '%f', '%f', NOW())", 
                cheat_type.c_str(), plMover->GetGUIDLow(), plMover->GetName(), plMover->getLevel(), plMover->GetMapId(), plMover->GetAreaId(), plMover->GetPositionX(), plMover->GetPositionY(), plMover->GetPositionZ());
            plMover->ac_local.m_logdb_time = cServerTime;
        }
}

bool AntiCheat::CheckMistiming(Player* plMover, Vehicle *vehMover, MovementInfo& movementInfo)
{		
	const int32 GetMistimingDelta = abs(int32(sWorld.getIntConfig(CONFIG_AC_ENABLE_MISTIMING_DELTHA)));
	
	if (sync_time > GetMistimingDelta)
	{
		cClientTimeDelta = cServerTimeDelta;
		++(plMover->ac_local.m_anti_MistimingCount);

		const bool bMistimingModulo = plMover->ac_local.m_anti_MistimingCount % 50 == 0;

		if (bMistimingModulo)
		{
			plMover->ac_local.SetDelta(-abs(int32(sWorld.getIntConfig(CONFIG_AC_ALARM_DELTA))));
            (plMover->ac_local.m_CheatList[CHEAT_MISTIMING])++;		
            cheat_find = true;
			LogCheat(CHEAT_MISTIMING, plMover, movementInfo);
		}
		if (sWorld.getBoolConfig(CONFIG_AC_ENABLE_MISTIMING_BLOCK))
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

bool AntiCheat::CheckAntiGravity(Player* plMover, Vehicle *vehMover, MovementInfo& movementInfo)
{
    if (no_fly_auras && no_swim_in_water && plMover->ac_local.m_anti_JumpBaseZ != 0 && JumpHeight < plMover->ac_local.m_anti_Last_VSpeed)
	{
		plMover->ac_local.SetDelta(-abs(int32(sWorld.getIntConfig(CONFIG_AC_ALARM_DELTA))));
        (plMover->ac_local.m_CheatList[CHEAT_GRAVITY])++;
        cheat_find = true;
		LogCheat(CHEAT_GRAVITY, plMover, movementInfo);
		if (sWorld.getBoolConfig(CONFIG_AC_ENABLE_ANTIGRAVITY_BLOCK))
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

bool AntiCheat::CheckAntiMultiJump(Player* plMover, Vehicle *vehMover, MovementInfo& movementInfo)
{
    if (no_fly_auras && no_swim_water)
	{
		if (plMover->ac_local.m_anti_JumpCount >= 1)
		{
			plMover->ac_local.SetDelta(-abs(int32(sWorld.getIntConfig(CONFIG_AC_ALARM_DELTA))));
            (plMover->ac_local.m_CheatList[CHEAT_MULTIJUMP])++;
            cheat_find = true;
			LogCheat(CHEAT_MULTIJUMP, plMover, movementInfo);
			if (sWorld.getBoolConfig(CONFIG_AC_ENABLE_ANTIMULTIJUMP_BLOCK))
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
			plMover->ac_local.m_anti_JumpBaseZ = movementInfo.pos.GetPositionZ();
		}
	}
	else
		plMover->ac_local.m_anti_JumpCount = 0;
		
	return true; 
}

bool AntiCheat::CheckAntiSpeedTeleport(Player* plMover, Vehicle *vehMover, MovementInfo& movementInfo)
{
	if (!plMover->GetVehicle()) // Disable on vehicle
    {
		if (real_delta > allowed_delta)
		{	
			plMover->ac_local.SetDelta(-abs(int32(sWorld.getIntConfig(CONFIG_AC_ALARM_DELTA))));
            (plMover->ac_local.m_CheatList[CHEAT_SPEED])++; // Speed and teleport are logged in one
            cheat_find = true;
			if (real_delta < 4900.0f)
				LogCheat(CHEAT_SPEED, plMover, movementInfo);
			else
				LogCheat(CHEAT_TELEPORT, plMover, movementInfo);
			if (sWorld.getBoolConfig(CONFIG_AC_ENABLE_ANTISPEEDTELE_BLOCK))
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
bool AntiCheat::CheckAntiMountain(Player* plMover, Vehicle *vehMover, MovementInfo& movementInfo)
{
	if (delta_z < plMover->ac_local.m_anti_Last_VSpeed && plMover->ac_local.m_anti_JumpCount == 0 && tg_z > 2.37f)
	{
		plMover->ac_local.SetDelta(-abs(int32(sWorld.getIntConfig(CONFIG_AC_ALARM_DELTA))));
        (plMover->ac_local.m_CheatList[CHEAT_MOUNTAIN])++;
        cheat_find = true;
		LogCheat(CHEAT_MOUNTAIN, plMover, movementInfo);
		if (sWorld.getBoolConfig(CONFIG_AC_ENABLE_ANTIMOUNTAIN_BLOCK))
		{
			if (vehMover)
				vehMover->Die();
			return false;
		}
	}
	return true;
}

bool AntiCheat::CheckAntiFly(Player* plMover, Vehicle *vehMover, MovementInfo& movementInfo)
{
	if (!plMover->GetVehicle()) // Disable on Vehicle
	{
		if (no_fly_auras && !no_fly_flags)
		{
			plMover->ac_local.SetDelta(-abs(int32(sWorld.getIntConfig(CONFIG_AC_ALARM_DELTA))));
            (plMover->ac_local.m_CheatList[CHEAT_FLY])++;
            cheat_find = true;
			LogCheat(CHEAT_FLY, plMover, movementInfo);
			if (sWorld.getBoolConfig(CONFIG_AC_ENABLE_ANTIFLY_BLOCK))
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
    }
	return true;
}

bool AntiCheat::CheckAntiWaterwalk(Player* plMover, Vehicle *vehMover, MovementInfo& movementInfo)
{
	if (no_waterwalk_auras && !no_waterwalk_flags)
	{
		plMover->ac_local.SetDelta(-abs(int32(sWorld.getIntConfig(CONFIG_AC_ALARM_DELTA))));
        (plMover->ac_local.m_CheatList[CHEAT_WATERWALK])++;
        cheat_find = true;
		LogCheat(CHEAT_WATERWALK, plMover, movementInfo);
		if (sWorld.getBoolConfig(CONFIG_AC_ENABLE_ANTIWATERWALK_BLOCK))
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

bool AntiCheat::CheckAntiTeleToPlane(Player* plMover, Vehicle *vehMover, MovementInfo& movementInfo)
{
	if (no_swim_in_water && movementInfo.pos.GetPositionZ() < 0.0001f && movementInfo.pos.GetPositionZ() > -0.0001f)
    {           
		if (const Map *map = plMover->GetMap())
		{
			float plane_z = map->GetHeight(movementInfo.pos.GetPositionX(), movementInfo.pos.GetPositionY(), MAX_HEIGHT) - movementInfo.pos.GetPositionZ();
			plane_z = (plane_z < -500.0f) ? 0.0f : plane_z; // check holes in height map
			if (plane_z > 0.1f || plane_z < -0.1f)
			{
				++(plMover->ac_local.m_anti_TeleToPlane_Count);
				if (plMover->ac_local.m_anti_TeleToPlane_Count > sWorld.getIntConfig(CONFIG_AC_ENABLE_ANTITELETOPLANE_ALARMS))
				{
					plMover->ac_local.SetDelta(-abs(int32(sWorld.getIntConfig(CONFIG_AC_ALARM_DELTA))));
                    (plMover->ac_local.m_CheatList[CHEAT_TELETOPLANE])++;
                    cheat_find = true;
					LogCheat(CHEAT_TELETOPLANE, plMover, movementInfo);
					if (sWorld.getBoolConfig(CONFIG_AC_ENABLE_ANTITELETOPLANE_BLOCK))
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