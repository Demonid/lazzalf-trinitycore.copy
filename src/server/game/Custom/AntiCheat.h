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

class AntiCheat
{
    private:
        Player* plMover;

		bool ac_block;        
		int32 ac_delta;

        MovementInfo lastpMovementInfo;
        uint32 uiLastOpcode;
        float fLastSpeedRate;

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
        uint32 uiDiffTime_packets;

		// Variables
		float fSpeedRate;	
		//float delta_x;
		//float delta_y;
		float delta_z;
		float real_delta;
		bool fly_auras;
		bool no_swim_in_water;
		bool no_swim_water;
		float client_time_delta;
		float tg_z;
		float allowed_delta;
		float JumpHeight;

        float fClientRate;
        float fServerRate;
        float fDistance2d;
		
		void CalcDeltas(MovementInfo& pNewPacket, MovementInfo& pOldPacket);
		void CalcVariables(MovementInfo& pOldPacket, MovementInfo& pNewPacket, Unit* mover);
        void CalcVariablesSmall(MovementInfo& pNewPacket, Unit* mover);
        bool CanFly(MovementInfo& pMovementInfo);
		
		bool CheckMistiming(Vehicle* /*vehMover*/, MovementInfo& /*pMovementInfo*/);
		bool CheckAntiGravity(Vehicle* /*vehMover*/, MovementInfo& /*pMovementInfo*/);
		bool CheckAntiMultiJump(Vehicle* vehMover, MovementInfo& pNewPacket, uint32 uiOpcode);
        bool CheckAntiSpeed(Vehicle *vehMover, MovementInfo& pOldPacket, MovementInfo& pNewPacket, uint32 uiOpcode);
        bool CheckAntiTele(Vehicle *vehMover, MovementInfo& pNewPacket, uint32 uiOpcode);
		bool CheckAntiMountain(Vehicle* /*vehMover*/, MovementInfo& /*pMovementInfo*/);
		bool CheckAntiFly(Vehicle* vehMover, MovementInfo& pOldPacket, MovementInfo& pNewPacket);
		bool CheckAntiWaterwalk(Vehicle* vehMover, MovementInfo& pOldPacket, MovementInfo& pNewPacket);
		bool CheckAntiTeleToPlane(Vehicle* /*vehMover*/, MovementInfo& /*pMovementInfo*/);
        void LogCheat(eCheat /*m_cheat*/, MovementInfo& /*pMovementInfo*/);
        bool AntiCheatPunisher(MovementInfo& /*pMovementInfo*/);
        inline bool ControllPunisher();
	
    public:
        AntiCheat(Player* new_plMover);
		bool DoAntiCheatCheck(Vehicle* /*vehMover*/, uint16 /*opcode*/, MovementInfo& /*pMovementInfo*/, Unit* /*mover*/);

    public:
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
        int32 GetDelta();
        void  SetSleep(int32 delta);
        void ResetCheatList(uint32 /*diff*/);
        void SaveLastPacket(MovementInfo& pMovementInfo) { lastpMovementInfo = pMovementInfo; }
        MovementInfo& GetLastPacket() { return lastpMovementInfo; }
        void SetLastOpcode(uint32 uiOpcode) { uiLastOpcode = uiOpcode; }
        uint32 GetLastOpcode() { return uiLastOpcode; }
        void SetLastSpeedRate(float fSpeedRateRate) { fLastSpeedRate = fSpeedRateRate; }
        float GetLastSpeedRate() { return fLastSpeedRate; }
};

#endif