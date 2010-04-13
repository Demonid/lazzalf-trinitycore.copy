/* Copyright (C) 2006 - 2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 * Original Author: Copyright (C) 2009 Trinity <http://www.trinitycore.org/>
 * Edited By SKRBX (http://code.google.com/p/emeraldemu/)
 
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


/* ScriptData
SDName: Instance Trial of the Champion
SD%Complete: 70
SDComment:
SDCategory: Trial Of the Champion
EndScriptData */

#include "ScriptedPch.h"
#include "trial_of_the_champion.h"

#define MAX_ENCOUNTER  4

struct instance_trial_of_the_champion : public ScriptedInstance
{
    instance_trial_of_the_champion(Map* pMap) : ScriptedInstance(pMap) { Initialize(); }

    uint32 m_auiEncounter[MAX_ENCOUNTER+1];
	
	std::string m_strInstData;

    uint64 m_uiJacobGUID;
    uint64 m_uiAmbroseGUID;
    uint64 m_uiColososGUID;
    uint64 m_uiJaelyneGUID;
    uint64 m_uiLanaGUID;
    uint64 m_uiMokraGUID;
    uint64 m_uiEresseaGUID;
    uint64 m_uiRunokGUID;
    uint64 m_uiZultoreGUID;
    uint64 m_uiVisceriGUID;
	uint64 m_uiChampionsLootGUID;
    uint64 m_uiEadricGUID;
	uint64 m_uiEadricLootGUID;
    uint64 m_uiPaletressGUID;
	uint64 m_uiPaletressLootGUID;
    uint64 m_uiBlackKnightGUID;
	uint64 m_uiJaerenGUID;
	uint64 m_uiArelasGUID;
	uint64 m_uiAnnouncerGUID;
	uint32 m_uiChampionId1;
	uint32 m_uiChampionId2;
	uint32 m_uiChampionId3;
	uint32 m_uiChampionsCount;
	uint64 m_uiChampion1;
	uint64 m_uiChampion2;
	uint64 m_uiChampion3;
	uint64 m_uiBlackKnightMinionGUID;
	uint64 m_uiArgentChallenger;
	uint64 m_uiArgentChallengerID;
	uint64 m_uiMemoryGUID;
    uint8 uiMovementDone;
    uint8 uiGrandChampionsDeaths;
    uint8 uiArgentSoldierDeaths;
    uint64 uiAnnouncerGUID;
    uint64 uiMainGateGUID;
    uint64 uiGrandChampionVehicle1GUID;
    uint64 uiGrandChampionVehicle2GUID;
    uint64 uiGrandChampionVehicle3GUID;
	uint64 uiGrandChampion1GUID;
	uint64 uiGrandChampion2GUID;
	uint64 uiGrandChampion3GUID;
    uint64 uiChampionLootGUID;
    uint64 uiArgentChampionGUID;

    std::list<uint64> VehicleList;

    std::string str_data;

    bool bDone;

    void Initialize()
    {
    uint64 m_uiJacobGUID          			=0;
    uint64 m_uiAmbroseGUID          		=0;
    uint64 m_uiColososGUID          		=0;
    uint64 m_uiJaelyneGUID          		=0;
    uint64 m_uiLanaGUID          			=0;
    uint64 m_uiMokraGUID          		    =0;
    uint64 m_uiEresseaGUID          		=0;
    uint64 m_uiRunokGUID          			=0;
    uint64 m_uiZultoreGUID          		=0;
    uint64 m_uiVisceriGUID          		=0;
	uint64 m_uiChampionsLootGUID          	=0;
    uint64 m_uiEadricGUID          			=0;
	uint64 m_uiEadricLootGUID          		=0;
    uint64 m_uiPaletressGUID          		=0;
	uint64 m_uiPaletressLootGUID          	=0;
    uint64 m_uiBlackKnightGUID          	=0;
	uint64 m_uiJaerenGUID          			=0;
	uint64 m_uiArelasGUID          			=0;
	uint64 m_uiAnnouncerGUID				=0;
	uint32 m_uiChampionId1          		=0;
	uint32 m_uiChampionId2          		=0;
	uint32 m_uiChampionId3					=0;
	uint32 m_uiChampionsCount				=0;
	uint64 m_uiChampion1					=0;
	uint64 m_uiChampion2					=0;
	uint64 m_uiChampion3					=0;
	uint64 m_uiBlackKnightMinionGUID		=0;
	uint64 m_uiArgentChallenger          	=0;
	uint64 m_uiArgentChallengerID           =0;
	uint64 m_uiMemoryGUID	          		=0;
    uint8 uiMovementDone          			=0;
    uint8 uiGrandChampionsDeaths      		=0;
    uint8 uiArgentSoldierDeaths          	=0;
    uint64 uiAnnouncerGUID          		=0;
    uint64 uiMainGateGUID	         		=0;
    uint64 uiGrandChampionVehicle1GUID		=0;
    uint64 uiGrandChampionVehicle2GUID		=0;
    uint64 uiGrandChampionVehicle3GUID		=0;
    uint64 uiGrandChampion1GUID				=0;
	uint64 uiGrandChampion2GUID				=0;
	uint64 uiGrandChampion3GUID				=0;
	uint64 uiChampionLootGUID				=0;
    uint64 uiArgentChampionGUID				=0;
        
	bDone = false;

        VehicleList.clear();

        memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));
    }

    bool IsEncounterInProgress() const
    {
        for(uint8 i = 0; i < MAX_ENCOUNTER; ++i)
        {
            if (m_auiEncounter[i] == IN_PROGRESS)
                return true;
        }

        return false;
    }

    void OnCreatureCreate(Creature* pCreature, bool bAdd)
	{
        Map::PlayerList const &players = instance->GetPlayers();
        uint32 TeamInInstance = 0;

        if (!players.isEmpty())
        {
            if (Player* pPlayer = players.begin()->getSource())
                TeamInInstance = pPlayer->GetTeam();
        }
        switch(pCreature->GetEntry())
        {
            // Champions Vechiles
            case VEHICLE_MOKRA_SKILLCRUSHER_MOUNT:
                if (TeamInInstance == HORDE)
                    pCreature->UpdateEntry(VEHICLE_MARSHAL_JACOB_ALERIUS_MOUNT, ALLIANCE);
                break;
            case VEHICLE_ERESSEA_DAWNSINGER_MOUNT:
                if (TeamInInstance == HORDE)
                    pCreature->UpdateEntry(VEHICLE_AMBROSE_BOLTSPARK_MOUNT, ALLIANCE);
                break;
            case VEHICLE_RUNOK_WILDMANE_MOUNT:
                if (TeamInInstance == HORDE)
                    pCreature->UpdateEntry(VEHICLE_COLOSOS_MOUNT, ALLIANCE);
                break;
            case VEHICLE_ZUL_TORE_MOUNT:
                if (TeamInInstance == HORDE)
                    pCreature->UpdateEntry(VEHICLE_EVENSONG_MOUNT, ALLIANCE);
                break;
            case VEHICLE_DEATHSTALKER_VESCERI_MOUNT:
                if (TeamInInstance == HORDE)
                    pCreature->UpdateEntry(VEHICLE_LANA_STOUTHAMMER_MOUNT, ALLIANCE);
                break;
                if (TeamInInstance == ALLIANCE)
                    pCreature->UpdateEntry(NPC_ARELAS,ALLIANCE);
                break;
            case VEHICLE_ARGENT_WARHORSE:
            case VEHICLE_ARGENT_BATTLEWORG:
                VehicleList.push_back(pCreature->GetGUID());
                break;
			// Champions of the Alliance
            case NPC_JACOB:
                m_uiJacobGUID = pCreature->GetGUID();
				if (m_uiChampion1 == 0)
					m_uiChampion1 = pCreature->GetGUID();
				else
					if (m_uiChampion2 == 0)
						m_uiChampion2 = pCreature->GetGUID();
					else
						if (m_uiChampion3 == 0)
							m_uiChampion3 = pCreature->GetGUID();
                break;
            case NPC_AMBROSE:
                m_uiAmbroseGUID = pCreature->GetGUID();
				if (m_uiChampion1 == 0)
					m_uiChampion1 = pCreature->GetGUID();
				else
					if (m_uiChampion2 == 0)
						m_uiChampion2 = pCreature->GetGUID();
					else
						if (m_uiChampion3 == 0)
							m_uiChampion3 = pCreature->GetGUID();
                break;
			case NPC_COLOSOS:
                m_uiColososGUID = pCreature->GetGUID();
				if (m_uiChampion1 == 0)
					m_uiChampion1 = pCreature->GetGUID();
				else
					if (m_uiChampion2 == 0)
						m_uiChampion2 = pCreature->GetGUID();
					else
						if (m_uiChampion3 == 0)
							m_uiChampion3 = pCreature->GetGUID();
                break;
            case NPC_JAELYNE:
                m_uiJaelyneGUID = pCreature->GetGUID();
				if (m_uiChampion1 == 0)
					m_uiChampion1 = pCreature->GetGUID();
				else
					if (m_uiChampion2 == 0)
						m_uiChampion2 = pCreature->GetGUID();
					else
						if (m_uiChampion3 == 0)
							m_uiChampion3 = pCreature->GetGUID();
                break;
            case NPC_LANA:
                m_uiLanaGUID = pCreature->GetGUID();
				if (m_uiChampion1 == 0)
					m_uiChampion1 = pCreature->GetGUID();
				else
					if (m_uiChampion2 == 0)
						m_uiChampion2 = pCreature->GetGUID();
					else
						if (m_uiChampion3 == 0)
							m_uiChampion3 = pCreature->GetGUID();
                break;
            // Champions of the Horde
            case NPC_MOKRA:
                m_uiMokraGUID = pCreature->GetGUID();
				if (m_uiChampion1 == 0)	
				m_uiChampion1 = pCreature->GetGUID();
				
				else
					if (m_uiChampion2 == 0)
						m_uiChampion2 = pCreature->GetGUID();
					else
						if (m_uiChampion3 == 0)
							m_uiChampion3 = pCreature->GetGUID();
                break;
            case NPC_ERESSEA:
                m_uiEresseaGUID = pCreature->GetGUID();
				if (m_uiChampion1 == 0)
					m_uiChampion1 = pCreature->GetGUID();
				else
					if (m_uiChampion2 == 0)
						m_uiChampion2 = pCreature->GetGUID();
					else
						if (m_uiChampion3 == 0)
							m_uiChampion3 = pCreature->GetGUID();
                break;
            case NPC_RUNOK:
                m_uiRunokGUID = pCreature->GetGUID();
				if (m_uiChampion1 == 0)
					m_uiChampion1 = pCreature->GetGUID();
				else
					if (m_uiChampion2 == 0)
						m_uiChampion2 = pCreature->GetGUID();
					else
						if (m_uiChampion3 == 0)
							m_uiChampion3 = pCreature->GetGUID();
                break;
            case NPC_ZULTORE:
                m_uiZultoreGUID = pCreature->GetGUID();
				if (m_uiChampion1 == 0)
					m_uiChampion1 = pCreature->GetGUID();
				else
					if (m_uiChampion2 == 0)
						m_uiChampion2 = pCreature->GetGUID();
					else
						if (m_uiChampion3 == 0)
							m_uiChampion3 = pCreature->GetGUID();
                break;
			case NPC_VISCERI:
                m_uiVisceriGUID = pCreature->GetGUID();
				if (m_uiChampion1 == 0)
					m_uiChampion1 = pCreature->GetGUID();
				else
					if (m_uiChampion2 == 0)
						m_uiChampion2 = pCreature->GetGUID();
					else
						if (m_uiChampion3 == 0)
							m_uiChampion3 = pCreature->GetGUID();
                break;
			 // Argent Challenge
			case NPC_EADRIC:
                m_uiEadricGUID = pCreature->GetGUID();
				m_uiArgentChallenger = pCreature->GetGUID();
                break;
            case NPC_PALETRESS:
                m_uiPaletressGUID = pCreature->GetGUID();
				m_uiArgentChallenger = pCreature->GetGUID();
                break;
			// Black Knight
            case NPC_BLACK_KNIGHT:
                m_uiBlackKnightGUID = pCreature->GetGUID();
                break;
			case NPC_RISEN_JAEREN:
                m_uiBlackKnightMinionGUID = pCreature->GetGUID();
                break;
            case NPC_RISEN_ARELAS:
                m_uiBlackKnightMinionGUID = pCreature->GetGUID();
                break;
			// Coliseum Announcers
            case NPC_JAEREN:
                m_uiJaerenGUID = pCreature->GetGUID();
                break;
            case NPC_ARELAS:
                m_uiArelasGUID = pCreature->GetGUID();
                break;
			// memories
			case MEMORY_ALGALON:
				m_uiMemoryGUID = pCreature->GetGUID();
				break;
			case MEMORY_ARCHIMONDE:
				m_uiMemoryGUID = pCreature->GetGUID();
				break;
			case MEMORY_CHROMAGGUS:
				m_uiMemoryGUID = pCreature->GetGUID();
				break;
			case MEMORY_CYANIGOSA:
				m_uiMemoryGUID = pCreature->GetGUID();
				break;
			case MEMORY_DELRISSA:
				m_uiMemoryGUID = pCreature->GetGUID();
				break;
			case MEMORY_ECK:
				m_uiMemoryGUID = pCreature->GetGUID();
				break;
			case MEMORY_ENTROPIUS:
				m_uiMemoryGUID = pCreature->GetGUID();
				break;
			case MEMORY_GRUUL:
				m_uiMemoryGUID = pCreature->GetGUID();
				break;
			case MEMORY_HAKKAR:
				m_uiMemoryGUID = pCreature->GetGUID();
				break;
			case MEMORY_HEIGAN:
				m_uiMemoryGUID = pCreature->GetGUID();
				break;
			case MEMORY_HEROD:
				m_uiMemoryGUID = pCreature->GetGUID();
				break;
			case MEMORY_HOGGER:
				m_uiMemoryGUID = pCreature->GetGUID();
				break;
			case MEMORY_IGNIS:
				m_uiMemoryGUID = pCreature->GetGUID();
				break;
			case MEMORY_ILLIDAN:
				m_uiMemoryGUID = pCreature->GetGUID();
				break;
			case MEMORY_INGVAR:
				m_uiMemoryGUID = pCreature->GetGUID();
				break;
			case MEMORY_KALITHRESH:
				m_uiMemoryGUID = pCreature->GetGUID();
				break;
			case MEMORY_LUCIFRON:
				m_uiMemoryGUID = pCreature->GetGUID();
				break;
			case MEMORY_MALCHEZAAR:
				m_uiMemoryGUID = pCreature->GetGUID();
				break;
			case MEMORY_MUTANUS:
				m_uiMemoryGUID = pCreature->GetGUID();
				break;
			case MEMORY_ONYXIA:
				m_uiMemoryGUID = pCreature->GetGUID();
				break;
			case MEMORY_THUNDERAAN:
				m_uiMemoryGUID = pCreature->GetGUID();
				break;
			case MEMORY_VANCLEEF:
				m_uiMemoryGUID = pCreature->GetGUID();
				break;
			case MEMORY_VASHJ:
				m_uiMemoryGUID = pCreature->GetGUID();
				break;
			case MEMORY_VEKNILASH:
				m_uiMemoryGUID = pCreature->GetGUID();
				break;
			case MEMORY_VEZAX:
				m_uiMemoryGUID = pCreature->GetGUID();
				break;
			uiArgentChampionGUID = pCreature->GetGUID();
                break;
		}
	}

    void OnGameObjectCreate(GameObject* pGO, bool bAdd)
	{
        switch(pGO->GetEntry())
        {
            case GO_MAIN_GATE:
                uiMainGateGUID = pGO->GetGUID();
                break;
			case GO_CHAMPIONS_LOOT:
				m_uiChampionsLootGUID = pGO->GetGUID();
				break;
			case GO_EADRIC_LOOT:
				m_uiEadricLootGUID = pGO->GetGUID();
				break;
			case GO_PALETRESS_LOOT:
				m_uiPaletressLootGUID = pGO->GetGUID();
				break;
			case GO_CHAMPIONS_LOOT_H:
				m_uiChampionsLootGUID = pGO->GetGUID();
				break;
			case GO_EADRIC_LOOT_H:
				m_uiEadricLootGUID = pGO->GetGUID();
				break;
			case GO_PALETRESS_LOOT_H:
				m_uiPaletressLootGUID = pGO->GetGUID();
				break;
		}
    }
    void SetData(uint32 uiType, uint32 uiData)
	{
        switch(uiType)
        {
			case DATA_MOVEMENT_DONE:
               uiMovementDone = uiData;
               if (uiMovementDone == 3)		
			case DATA_TOC5_ANNOUNCER:
				m_uiAnnouncerGUID = uiData;
			break;
			case DATA_CHAMPIONID_1:
				m_uiChampionId1 = uiData;
			break;
			case DATA_CHAMPIONID_2:
				m_uiChampionId2 = uiData;
			break;
			case DATA_CHAMPIONID_3:
				m_uiChampionId3 = uiData;
			break;
			case DATA_CHAMPIONS_COUNT:
				m_uiChampionsCount = uiData;
			break;
			case DATA_ARGENT_CHALLENGER:
				m_uiArgentChallengerID = uiData;
			break;
			case DATA_BLACK_KNIGHT_MINION:
				m_uiBlackKnightMinionGUID = uiData;
			break;
            case TYPE_ARGENT_CHALLENGE:
				m_auiEncounter[1] = uiData;
                  if (uiData == DONE)
                {
					if (m_uiArgentChallenger == m_uiEadricGUID)
						if (GameObject* pChest = instance->GetGameObject(m_uiEadricLootGUID))
							if (pChest && !pChest->isSpawned())
								pChest->SetRespawnTime(DAY);
					if (m_uiArgentChallenger == m_uiPaletressGUID)
						if (GameObject* pChest = instance->GetGameObject(m_uiPaletressLootGUID))
							if (pChest && !pChest->isSpawned())
								pChest->SetRespawnTime(DAY);
                }
                if (uiData == FAIL)
                {
                      m_auiEncounter[1] = NOT_STARTED;
                }
                break;
            case TYPE_GRAND_CHAMPIONS:
                m_auiEncounter[0] = uiData;
                if (uiData == IN_PROGRESS)
				{
                    for(std::list<uint64>::iterator itr = VehicleList.begin(); itr != VehicleList.end(); ++itr)
                        if (Creature* pSummon = instance->GetCreature(*itr))
                            pSummon->RemoveFromWorld();
                }
				else if (uiData == DONE)
                {
                    ++uiGrandChampionsDeaths;
                    if (uiGrandChampionsDeaths == 3)
                    {
                        if (Creature* pAnnouncer =  instance->GetCreature(uiAnnouncerGUID))
                        {
                            pAnnouncer->GetMotionMaster()->MovePoint(0,748.309,619.487,411.171);
                            pAnnouncer->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                            pAnnouncer->SummonGameObject(instance->IsHeroic()? GO_CHAMPIONS_LOOT_H : GO_CHAMPIONS_LOOT,746.59,618.49,411.09,1.42,0, 0, 0, 0,90000000);
                        }
                    }
                }
                break;
            case BOSS_ARGENT_CHALLENGE_E:
                m_auiEncounter[1] = uiData;
                if (Creature* pAnnouncer = instance->GetCreature(uiAnnouncerGUID))
                {
                    pAnnouncer->GetMotionMaster()->MovePoint(0,748.309,619.487,411.171);
                    pAnnouncer->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                    pAnnouncer->SummonGameObject(instance->IsHeroic()? GO_EADRIC_LOOT_H : GO_EADRIC_LOOT,746.59,618.49,411.09,1.42,0, 0, 0, 0,90000000);
                }
                break;
            case TYPE_BLACK_KNIGHT:
                m_auiEncounter[2] = uiData;
				if (uiData == FAIL)
                {
					     m_auiEncounter[2] = NOT_STARTED;
					if (Creature* pAnnouncer = instance->GetCreature(uiAnnouncerGUID))
					{
						pAnnouncer->GetMotionMaster()->MovePoint(0,748.309,619.487,411.171);
						pAnnouncer->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
						pAnnouncer->SummonGameObject(instance->IsHeroic()? GO_PALETRESS_LOOT_H : GO_PALETRESS_LOOT,746.59,618.49,411.09,1.42,0, 0, 0, 0,90000000);
				
					break;
					}
				}
		}
	}
    uint32 GetData(uint32 uiData)
    {
        switch(uiData)
        {
            case TYPE_ARGENT_CHALLENGE: return m_auiEncounter[2];

            case DATA_MOVEMENT_DONE: return uiMovementDone;
            case DATA_BLACK_KNIGHT_MINION: return uiArgentSoldierDeaths;
        }

        return 0;
	}

    uint64 GetData64(uint32 uiData)
    {
        switch(uiData)
        {
            case DATA_ANNOUNCER: return uiAnnouncerGUID;
            case DATA_MAIN_GATE: return uiMainGateGUID;

            case DATA_GRAND_CHAMPION_1: return uiGrandChampion1GUID;
            case DATA_GRAND_CHAMPION_2: return uiGrandChampion2GUID;
            case DATA_GRAND_CHAMPION_3: return uiGrandChampion3GUID;
			case DATA_CHAMPION_1: return m_uiChampion1;
			case DATA_CHAMPION_2: return m_uiChampion2;
			case DATA_MEMORY: return m_uiMemoryGUID;
			case DATA_ARGENT_CHALLENGER: return m_uiArgentChallenger;
		}
        return 0;
	}

	void SetData64(uint32 uiType, uint64 uiData)
    {
        switch(uiType)
        {
            case DATA_GRAND_CHAMPION_1: uiGrandChampion1GUID = uiData;
                break;
            case DATA_GRAND_CHAMPION_2: uiGrandChampion2GUID = uiData;
                break;
            case DATA_GRAND_CHAMPION_3: uiGrandChampion3GUID = uiData;
                break;
        }
	}
	    const char* Save()
    {
        return m_strInstData.c_str();
    }

    std::string GetSaveData()
    {
        OUT_SAVE_INST_DATA;

        std::ostringstream saveStream;

        saveStream << "T C " << m_auiEncounter[0]
            << " " << m_auiEncounter[1]
            << " " << m_auiEncounter[2]
            << " " << m_auiEncounter[3]
            << " " << uiGrandChampionsDeaths
            << " " << uiMovementDone;

        str_data = saveStream.str();

        OUT_SAVE_INST_DATA_COMPLETE;
        return str_data;
	}
    void Load(const char* in)
    {
        if (!in)
        {
            OUT_LOAD_INST_DATA_FAIL;
            return;
        }

        OUT_LOAD_INST_DATA(in);

        char dataHead1, dataHead2;
        uint16 data0, data1, data2, data3, data4, data5;

        std::istringstream loadStream(in);
        loadStream >> dataHead1 >> dataHead2 >> data0 >> data1 >> data2 >> data3 >> data4 >> data5;
		for(uint8 i = 0; i < MAX_ENCOUNTER; ++i)
        {
            loadStream >> m_auiEncounter[i];
            if (m_auiEncounter[i] == IN_PROGRESS)
                m_auiEncounter[i] = NOT_STARTED;
        }
        if (dataHead1 == 'T' && dataHead2 == 'C')
        {
            m_auiEncounter[0] = data0;
            m_auiEncounter[1] = data1;
            m_auiEncounter[2] = data2;
            m_auiEncounter[3] = data3;

            for (uint8 i = 0; i < MAX_ENCOUNTER; ++i)
                if (m_auiEncounter[i] == IN_PROGRESS)
                    m_auiEncounter[i] = NOT_STARTED;

            uiGrandChampionsDeaths = data4;
            uiMovementDone = data5;
        } 
		else OUT_LOAD_INST_DATA_FAIL;

        OUT_LOAD_INST_DATA_COMPLETE;
	}
};

InstanceData* GetInstanceData_instance_trial_of_the_champion(Map* pMap)
{
    return new instance_trial_of_the_champion(pMap);
}

void AddSC_instance_trial_of_the_champion()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "instance_trial_of_the_champion";
    newscript->GetInstanceData = &GetInstanceData_instance_trial_of_the_champion;
    newscript->RegisterSelf();
}
