/*
 * Copyright (C) 2005-2009 MaNGOS <http://getmangos.com/>
 *
 * Copyright (C) 2008-2010 Trinity <http://www.trinitycore.org/>
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

#ifndef __BATTLEGROUNDMGR_H
#define __BATTLEGROUNDMGR_H

#include "Common.h"
#include "DBCEnums.h"
#include "BattleGround.h"
#include "BattleGroundQueue.h"
#include "ace/Singleton.h"

typedef std::map<uint32, BattleGround*> BattleGroundSet;

typedef UNORDERED_MAP<uint32, BattleGroundTypeId> BattleMastersMap;

#define BATTLEGROUND_ARENA_POINT_DISTRIBUTION_DAY 86400     // seconds in a day
#define BATTLEGROUND_ARENA_MOD_RESET_HOUR         3600      // seconds in an hour
#define COUNT_OF_PLAYERS_TO_AVERAGE_WAIT_TIME 10
#define WS_ARENA_DISTRIBUTION_TIME 20001                    // Custom worldstate

struct QueueUpdateInfo
{
    uint32 ateamId;
    uint64 schedule_id;
};

enum WintergraspWorldStatesAndSpells
{
    // Worldstates
    WS_WINTERGRASP_CONTROLING_TEAMID    = 90001,            // Wintergrasp: Current team id
    WS_WINTERGRASP_ISWAR                = 90002,            // Wintergrasp: Is currently wartime?
    WS_WINTERGRASP_TIMER                = 90003,            // Wintergrasp: Timer
    WS_WINTERGRASP_CLOCK_ALLY           = 90004,            // Wintergrasp: Clock Ally
    WS_WINTERGRASP_CLOCK_HORDE          = 90005,            // Wintergrasp: Clock Horde
    WS_WINTERGRASP_SHOP_CNT_ALLY        = 90006,            // Wintergrasp: Workshop count Ally
    WS_WINTERGRASP_SHOP_CNT_HORDE       = 90007,            // Wintergrasp: Workshop count Horde
    WS_WINTERGRASP_TOWER_DEST_ALLY      = 90008,            // Wintergrasp: Tower destroyed Ally
    WS_WINTERGRASP_TOWER_DEST_HORDE     = 90009,            // Wintergrasp: Tower destroyed Horde
    WS_WINTERGRASP_VEHICLE_CNT_ALLY     = 90010,            // Wintergrasp: Vehicle count Ally
    WS_WINTERGRASP_VEHICLE_CNT_HORDE    = 90011,            // Wintergrasp: Vehicle count Horde
    // Special spells
    SPELL_ESSENCE_OF_WINTERGRASP_WINNER = 58045,            // Only within wintergrasp
    SPELL_ESSENCE_OF_WINTERGRASP_WORLD  = 57940             // For entire northrend
};

class BattleGroundMgr
{
    /// Todo: Thread safety?
    /* Construction */
    friend class ACE_Singleton<BattleGroundMgr, ACE_Null_Mutex>;
    BattleGroundMgr();
    
    public:
        ~BattleGroundMgr();
        void Update(uint32 diff);

        /* Packet Building */
        void BuildPlayerJoinedBattleGroundPacket(WorldPacket *data, Player *plr);
        void BuildPlayerLeftBattleGroundPacket(WorldPacket *data, const uint64& guid);
        void BuildBattleGroundListPacket(WorldPacket *data, const uint64& guid, Player *plr, BattleGroundTypeId bgTypeId, uint8 fromWhere);
        void BuildGroupJoinedBattlegroundPacket(WorldPacket *data, GroupJoinBattlegroundResult result);
        void BuildUpdateWorldStatePacket(WorldPacket *data, uint32 field, uint32 value);
        void BuildPvpLogDataPacket(WorldPacket *data, BattleGround *bg);
        void BuildBattleGroundStatusPacket(WorldPacket *data, BattleGround *bg, uint8 QueueSlot, uint8 StatusID, uint32 Time1, uint32 Time2, uint8 arenatype);
        void BuildPlaySoundPacket(WorldPacket *data, uint32 soundid);
        void SendAreaSpiritHealerQueryOpcode(Player *pl, BattleGround *bg, const uint64& guid);

        /* Battlegrounds */
        BattleGround* GetBattleGroundThroughClientInstance(uint32 instanceId, BattleGroundTypeId bgTypeId);
        BattleGround* GetBattleGround(uint32 InstanceID, BattleGroundTypeId bgTypeId); //there must be uint32 because MAX_BATTLEGROUND_TYPE_ID means unknown

        BattleGround* GetBattleGroundTemplate(BattleGroundTypeId bgTypeId);
        BattleGround* CreateNewBattleGround(BattleGroundTypeId bgTypeId, PvPDifficultyEntry const* bracketEntry, uint8 arenaType, bool isRated);

        uint32 CreateBattleGround(BattleGroundTypeId bgTypeId, bool IsArena, uint32 MinPlayersPerTeam, uint32 MaxPlayersPerTeam, uint32 LevelMin, uint32 LevelMax, char* BattleGroundName, uint32 MapID, float Team1StartLocX, float Team1StartLocY, float Team1StartLocZ, float Team1StartLocO, float Team2StartLocX, float Team2StartLocY, float Team2StartLocZ, float Team2StartLocO, uint32 scriptId);

        void AddBattleGround(uint32 InstanceID, BattleGroundTypeId bgTypeId, BattleGround* BG) { m_BattleGrounds[bgTypeId][InstanceID] = BG; };
        void RemoveBattleGround(uint32 instanceID, BattleGroundTypeId bgTypeId) { m_BattleGrounds[bgTypeId].erase(instanceID); }
        uint32 CreateClientVisibleInstanceId(BattleGroundTypeId bgTypeId, BattleGroundBracketId bracket_id);

        void CreateInitialBattleGrounds();
        void DeleteAllBattleGrounds();

        void SendToBattleGround(Player *pl, uint32 InstanceID, BattleGroundTypeId bgTypeId);

        /* Battleground queues */
        //these queues are instantiated when creating BattlegroundMrg
        BattleGroundQueue m_BattleGroundQueues[MAX_BATTLEGROUND_QUEUE_TYPES]; // public, because we need to access them in BG handler code

        BGFreeSlotQueueType BGFreeSlotQueue[MAX_BATTLEGROUND_TYPE_ID];

        void ScheduleQueueUpdate(uint32 arenaRating, uint8 arenaType, BattleGroundQueueTypeId bgQueueTypeId, BattleGroundTypeId bgTypeId, BattleGroundBracketId bracket_id);
        uint32 GetMaxRatingDifference() const;
        uint32 GetRatingDiscardTimer()  const;
        uint32 GetPrematureFinishTime() const;

        void InitAutomaticArenaPointDistribution();
        void DistributeArenaPoints();
        void ToggleArenaTesting();
        void ToggleTesting();

        void SetHolidayWeekends(uint32 mask);
        void LoadBattleMastersEntry();
        BattleGroundTypeId GetBattleMasterBG(uint32 entry) const
        {
            BattleMastersMap::const_iterator itr = mBattleMastersMap.find(entry);
            if (itr != mBattleMastersMap.end())
                return itr->second;
            return BATTLEGROUND_WS;
        }

        bool isArenaTesting() const { return m_ArenaTesting; }
        bool isTesting() const { return m_Testing; }

        static bool IsArenaType(BattleGroundTypeId bgTypeId);
        static bool IsBattleGroundType(BattleGroundTypeId bgTypeId) { return !BattleGroundMgr::IsArenaType(bgTypeId); }
        static BattleGroundQueueTypeId BGQueueTypeId(BattleGroundTypeId bgTypeId, uint8 arenaType);
        static BattleGroundTypeId BGTemplateId(BattleGroundQueueTypeId bgQueueTypeId);
        static uint8 BGArenaType(BattleGroundQueueTypeId bgQueueTypeId);

        static HolidayIds BGTypeToWeekendHolidayId(BattleGroundTypeId bgTypeId);
        static BattleGroundTypeId WeekendHolidayIdToBGType(HolidayIds holiday);
        static bool IsBGWeekend(BattleGroundTypeId bgTypeId);
        void DoCompleteAchievement(uint32 achievement, Player * player = NULL);
    private:
        BattleMastersMap    mBattleMastersMap;

        typedef std::map<BattleGroundTypeId, uint8> BattleGroundSelectionWeightMap; // TypeId and its selectionWeight
        /* Battlegrounds */
        BattleGroundSet m_BattleGrounds[MAX_BATTLEGROUND_TYPE_ID];
        BattleGroundSelectionWeightMap m_ArenaSelectionWeights;
        BattleGroundSelectionWeightMap m_BGSelectionWeights;
        //std::vector<uint64> m_QueueUpdateScheduler;
        std::vector<QueueUpdateInfo> m_QueueUpdateScheduler;
        
		std::set<uint32> m_ClientBattleGroundIds[MAX_BATTLEGROUND_TYPE_ID][MAX_BATTLEGROUND_BRACKETS]; //the instanceids just visible for the client
        uint32 m_NextRatingDiscardUpdate;
        time_t m_NextAutoDistributionTime;
        time_t m_NextArenaModResetTime;
        uint32 m_AutoDistributionTimeChecker;
        uint32 m_ArenaModResetChecker;
        bool   m_ArenaTesting;
        bool   m_Testing;
};

#define sBattleGroundMgr (*ACE_Singleton<BattleGroundMgr, ACE_Null_Mutex>::instance())
#endif

