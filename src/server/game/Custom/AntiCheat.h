#ifndef ANTI_CHEAT_H
#define ANTI_CHEAT_H

/**
 *
 * @File : AntiCheat.h
 *
 * @Authors : Lazzalf based on AC2 and Manuel AntiCheat
 *
 **/

#include "Common.h"
#include "WorldSession.h"

enum eCheat
{
	CHEAT_MISTIMING = 0,
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

enum ePuniType
{
    PUNI_NONE = 0,
    PUNI_BLOCK,
	PUNI_KILL,
	PUNI_KICK,
	PUNI_BAN_CHAR,
    PUNI_BAN_ACC,
    PUNI_BAN_IP,
    MAX_PUNI
};

class AntiCheat_Local
{
	private:
		bool ac_block;        
		int32 ac_delta;

        MovementInfo lastpMovementInfo;
        uint32 uiLastOpcode;
        float fLastSpeedRate;

	public:
		AntiCheat_Local();

        uint32 number_cheat_find;
        int32 ac_goactivate;

        uint32 m_CheatList[MAX_CHEAT];
        uint32 m_CheatList_reset_diff;        
	
		time_t m_anti_LastClientTime;           // last movement client time
        time_t m_anti_LastServerTime;           // last movement server time
        time_t m_anti_DeltaClientTime;          // client side session time
        time_t m_anti_DeltaServerTime;          // server side session time
        uint32 m_anti_MistimingCount;           // mistiming count
        time_t m_logfile_time;
        time_t m_logdb_time;        

        time_t m_anti_LastSpeedChangeTime;      // last speed change time

        float m_anti_Last_HSpeed;               // horizontal speed, default RUN speed
        float m_anti_Last_VSpeed;               // vertical speed, default max jump height

        uint32 m_anti_TeleToPlane_Count;        // Teleport To Plane alarm counter

        uint64 m_anti_AlarmCount;               // alarm counter

        uint16 m_anti_JumpCount;                // Jump already began, anti air jump check
        float m_anti_JumpBaseZ;                 // Z coord before jump
		
		void SetBlock(bool /*block*/);
		bool GetBlock();
		bool GetAndUpdateDelta(int32 /*diff*/);
		void SetDelta(int32 /*delta*/);
        void ResetCheatList(uint32 /*diff*/);
        void SaveLastPacket(MovementInfo& pMovementInfo) { lastpMovementInfo = pMovementInfo; }
        MovementInfo& GetLastPacket() { return lastpMovementInfo; }
        void SetLastOpcode(uint32 uiOpcode) { uiLastOpcode = uiOpcode; }
        uint32 GetLastOpcode() { return uiLastOpcode; }
        void SetLastSpeedRate(float fSpeedRateRate) { fLastSpeedRate = fSpeedRateRate; }
        float GetLastSpeedRate() { return fLastSpeedRate; }
};

class AntiCheat
{        
	private: 
        bool cheat_find;

        bool map_count;
        bool map_block;
        bool map_puni;

		// Delthas
		int32 cClientTimeDelta;
		uint64 cServerTime;		
		uint32 cServerTimeDelta;
		int32 sync_time;
        uint32 difftime_log_file;
        uint32 difftime_log_db;

		// Variables
		float fSpeedRate;	
		float delta_x;
		float delta_y;
		float delta_z;
		float real_delta;
		bool fly_auras;
		bool no_swim_flags;
		bool no_swim_in_water;
		bool no_swim_above_water;
		bool no_swim_water;
		float time_delta;
		float tg_z;
		float allowed_delta;
		float JumpHeight;
		
		void CalcDeltas(Player* /*plMover*/, MovementInfo& /*pMovementInfo*/);
		void CalcVariables(Player* plMover, MovementInfo& pNewPacket, Unit* mover);
        void CalcVariablesSmall(Player* plMover, MovementInfo& pNewPacket, Unit* mover);
        bool CanFly(Player* plMover, MovementInfo& pMovementInfo);
		
		bool CheckMistiming(Player* /*plMover*/, Vehicle* /*vehMover*/, MovementInfo& /*pMovementInfo*/);
		bool CheckAntiGravity(Player* /*plMover*/, Vehicle* /*vehMover*/, MovementInfo& /*pMovementInfo*/);
		bool CheckAntiMultiJump(Player* plMover, Vehicle* vehMover, MovementInfo& pNewPacket, uint32 uiOpcode);
		bool CheckAntiSpeedTeleport(Player* plMover, Vehicle* vehMover, MovementInfo& pNewPacket, uint32 uiOpcode);
		bool CheckAntiMountain(Player* /*plMover*/, Vehicle* /*vehMover*/, MovementInfo& /*pMovementInfo*/);
		bool CheckAntiFly(Player* plMover, Vehicle* vehMover, MovementInfo& pOldPacket, MovementInfo& pNewPacket);
		bool CheckAntiWaterwalk(Player* plMover, Vehicle* vehMover, MovementInfo& pOldPacket, MovementInfo& pNewPacket);
		bool CheckAntiTeleToPlane(Player* /*plMover*/, Vehicle* /*vehMover*/, MovementInfo& /*pMovementInfo*/);
        void LogCheat(eCheat /*m_cheat*/, Player* /*plMover*/, MovementInfo& /*pMovementInfo*/);
        bool AntiCheatPunisher(Player* /*plMover*/, MovementInfo& /*pMovementInfo*/);
        inline bool ControllPunisher(Player* /*plMover*/);
	
    public:
        AntiCheat();
		bool Check(Player* /*plMover*/, Vehicle* /*vehMover*/, uint16 /*opcode*/, MovementInfo& /*pMovementInfo*/, Unit* /*mover*/);		
};

#endif