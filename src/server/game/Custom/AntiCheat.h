#ifndef ANTI_CHEAT_H
#define ANTI_CHEAT_H

/**
 *
 * @File : AntiCheat.h
 *
 * @Authors : Lazzalf based on AC2
 *
 * @Version : 0.1
 *
 **/

#include "Common.h"
#include "WorldSession.h"

#define FROSTBROOD_VANQUISHER 28670

enum eCheat
{
	CHEAT_MISTIMING,
	CHEAT_GRAVITY,
	CHEAT_MULTIJUMP,
	CHEAT_SPEED,
	CHEAT_TELEPORT,
	CHEAT_MOUNTAIN,
	CHEAT_FLY,
	CHEAT_WATERWALK,
	CHEAT_TELETOPLANE,
    MAX_CHEAT
};

class AntiCheat_Local
{
	private:
		bool ac_block;
		uint32 ac_block_diff;
		int32 ac_count;
		
	public:
		AntiCheat_Local();	
	
		time_t m_anti_LastClientTime;           // last movement client time
        time_t m_anti_LastServerTime;           // last movement server time
        time_t m_anti_DeltaClientTime;          // client side session time
        time_t m_anti_DeltaServerTime;          // server side session time
        uint32 m_anti_MistimingCount;           // mistiming count
        time_t m_logcheat_time;

        time_t m_anti_LastSpeedChangeTime;      // last speed change time

        float m_anti_Last_HSpeed;               // horizontal speed, default RUN speed
        float m_anti_Last_VSpeed;               // vertical speed, default max jump height

        uint32 m_anti_TeleToPlane_Count;        // Teleport To Plane alarm counter

        uint64 m_anti_AlarmCount;               // alarm counter

        uint16 m_anti_JumpCount;                // Jump already began, anti air jump check
        float m_anti_JumpBaseZ;                 // Z coord before jump
		
		void SetBlock(bool /*block*/);
		bool GetBlock();
        void AddBlockDiff(uint32 /*diff*/);
		bool GetAndUpdateBlockDiff(uint32 /*diff*/);
		bool GetAndUpdateCount();
		void SetCount(int32 /*m_num*/);	
};

class AntiCheat
{
	private:
		// Delthas
		int32 cClientTimeDelta;
		uint64 cServerTime;
		uint32 difftime_log;
		uint32 cServerTimeDelta;
		int32 sync_time;

		// Variables
		float current_speed;
		uint32 vehicleEntry;		
		float delta_x;
		float delta_y;
		float delta_z;
		float real_delta;
		bool no_fly_auras;
		bool no_fly_flags;
		bool no_swim_flags;
		bool no_swim_in_water;
		bool no_swim_above_water;
		bool no_swim_water;
		bool no_waterwalk_flags;
		bool no_waterwalk_auras;
		float time_delta;
		float tg_z;
		float allowed_delta;
		float JumpHeight;
		
		void CalcDeltas(Player* /*plMover*/, MovementInfo& /*movementInfo*/);
		void CalcVariables(Player* /*plMover*/, MovementInfo& /*movementInfo*/, Unit* /*mover*/);
		
		bool CheckMistiming(Player* /*plMover*/, Vehicle* /*vehMover*/, MovementInfo& /*movementInfo*/);
		bool CheckAntiGravity(Player* /*plMover*/, Vehicle* /*vehMover*/, MovementInfo& /*movementInfo*/);
		bool CheckAntiMultiJump(Player* /*plMover*/, Vehicle* /*vehMover*/, MovementInfo& /*movementInfo*/);
		bool CheckAntiSpeedTeleport(Player* /*plMover*/, Vehicle* /*vehMover*/, MovementInfo& /*movementInfo*/);
		bool CheckAntiMountain(Player* /*plMover*/, Vehicle* /*vehMover*/, MovementInfo& /*movementInfo*/);
		bool CheckAntiFly(Player* /*plMover*/, Vehicle* /*vehMover*/, MovementInfo& /*movementInfo*/);
		bool CheckAntiWaterwalk(Player* /*plMover*/, Vehicle* /*vehMover*/, MovementInfo& /*movementInfo*/);
		bool CheckAntiTeleToPlane(Player* /*plMover*/, Vehicle* /*vehMover*/, MovementInfo& /*movementInfo*/);
        void LogCheat(eCheat /*m_cheat*/, Player* /*plMover*/, MovementInfo& /*movementInfo*/);
	
	public:
		bool Check(Player* /*plMover*/, Vehicle* /*vehMover*/, uint16 /*opcode*/, MovementInfo& /*movementInfo*/, Unit* /*mover*/);		
};

// bool CONFIG_AC_ENABLE
// bool CONFIG_AC_DISABLE_GM
// bool CONFIG_AC_ENABLE_MISTIMING
// bool CONFIG_AC_ENABLE_MISTIMING_BLOCK
// bool CONFIG_AC_ENABLE_ANTIGRAVITY
// bool CONFIG_AC_ENABLE_ANTIGRAVITY_BLOCK
// bool CONFIG_AC_ENABLE_ANTIMULTIJUMP
// bool CONFIG_AC_ENABLE_ANTIMULTIJUMP_BLOCK
// bool CONFIG_AC_ENABLE_ANTISPEEDTELE
// bool CONFIG_AC_ENABLE_ANTISPEEDTELE_BLOCK
// bool CONFIG_AC_ENABLE_ANTIMOUNTAIN
// bool CONFIG_AC_ENABLE_ANTIMOUNTAIN_BLOCK
// bool CONFIG_AC_ENABLE_ANTIFLY
// bool CONFIG_AC_ENABLE_ANTIFLY_BLOCK
// bool CONFIG_AC_ENABLE_ANTIWATERWALK
// bool CONFIG_AC_ENABLE_ANTIWATERWALK_BLOCK
// bool CONFIG_AC_ENABLE_ANTITELETOPLANE
// bool CONFIG_AC_ENABLE_ANTITELETOPLANE_BLOCK

// int CONFIG_AC_ENABLE_ANTITELETOPLANE_ALARMS
// int CONFIG_AC_ENABLE_MISTIMING_DELTHA
// int CONFIG_AC_DELTA_LOG
// int CONFIG_AC_SLEEP_COUNT
// int CONFIG_AC_ALARM_COUNT
#endif