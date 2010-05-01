/*
 * Copyright (C) 2010 Trinity <http://www.trinitycore.org/>
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


/* ScriptData
SDName: Instance Trial of the Champion
SDComment:
SDCategory: Trial Of the Champion
EndScriptData */

#include "ScriptedPch.h"
#include "trial_of_the_champion.h"

#define MAX_ENCOUNTER  4


//enumerazione di alcuni say --> controllo corrispondenza sul db
enum eEnums
{
    SAY_START                               = -1999927,
	SAY_START11                             = -1999953,
	SAY_START_9                             = -1999950
};


//definizione della struttura instance
struct instance_trial_of_the_champion : public ScriptedInstance
{
	
    instance_trial_of_the_champion(Map* pMap) : ScriptedInstance(pMap) {Initialize();}
	//4 incontri
    uint32 m_auiEncounter[MAX_ENCOUNTER];
	
	uint8 uiAgroDone;
	uint8 uiAggroDone;
    uint8 uiMovementDone;
    uint8 uiGrandChampionsDeaths;
    uint8 uiArgentSoldierDeaths;

    uint64 uiAnnouncerGUID;
    uint64 uiMainGateGUID;
    uint64 uiMainGate1GUID;
    uint64 uiGrandChampionVehicle1GUID;
    uint64 uiGrandChampionVehicle2GUID;
    uint64 uiGrandChampionVehicle3GUID;
    uint64 uiGrandChampion1GUID;
    uint64 uiGrandChampion2GUID;
    uint64 uiGrandChampion3GUID;
    uint64 uiChampionLootGUID;
    uint64 uiArgentChampionGUID;

    std::list<uint64> VehicleList;
	std::list<uint64> VehicleListChampion;

    std::string str_data;

    bool bDone;

    void Initialize()
    {
    	uiAgroDone = 0;
		uiAggroDone = 0;
        uiMovementDone = 0;
        uiGrandChampionsDeaths = 0;
        uiArgentSoldierDeaths = 0;

        uiAnnouncerGUID        = 0;
        uiMainGateGUID         = 0;
        uiMainGate1GUID         = 0;
        uiGrandChampionVehicle1GUID   = 0;
        uiGrandChampionVehicle2GUID   = 0;
        uiGrandChampionVehicle3GUID   = 0;
        uiGrandChampion1GUID          = 0;
        uiGrandChampion2GUID          = 0;
        uiGrandChampion3GUID          = 0;
        uiChampionLootGUID            = 0;
        uiArgentChampionGUID          = 0;

        bDone = false;


        VehicleList.clear();
		VehicleListChampion.clear();
		//m_auiEncounter[i]=0 per ogni i compreso tra 0 e 3 estremi inclusi
        memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));
    }

	//true se � in progress almeno un incontro 
    bool IsEncounterInProgress() const
    {

		//possibile valore di m_auiEncounter[i] � evidentemente 1 che equivale a incontro in progress
        for(uint8 i = 0; i < MAX_ENCOUNTER; ++i)
        {
            if (m_auiEncounter[i] == IN_PROGRESS)
                return true;
        }

        return false;
    }
	/*
	 * 1)alla creazione di una creatura controlla se i players in instance sono ally o horda
	 * 2)con uno switch sceglie in base all'entry creatura non so precisamente cosa va ad aggiornare da capire...
	 *
	 */
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
            // Champions
            case VEHICLE_MOKRA_SKILLCRUSHER_MOUNT:
                if (TeamInInstance == HORDE)
                    pCreature->UpdateEntry(VEHICLE_MARSHAL_JACOB_ALERIUS_MOUNT, ALLIANCE);
				VehicleListChampion.push_back(pCreature->GetGUID());
                break;
            case VEHICLE_ERESSEA_DAWNSINGER_MOUNT:
                if (TeamInInstance == HORDE)
                    pCreature->UpdateEntry(VEHICLE_AMBROSE_BOLTSPARK_MOUNT, ALLIANCE);
				VehicleListChampion.push_back(pCreature->GetGUID());

                break;
            case VEHICLE_RUNOK_WILDMANE_MOUNT:
                if (TeamInInstance == HORDE)
                    pCreature->UpdateEntry(VEHICLE_COLOSOS_MOUNT, ALLIANCE);
				VehicleListChampion.push_back(pCreature->GetGUID());
                break;
            case VEHICLE_ZUL_TORE_MOUNT:
                if (TeamInInstance == HORDE)
                    pCreature->UpdateEntry(VEHICLE_EVENSONG_MOUNT, ALLIANCE);
				VehicleListChampion.push_back(pCreature->GetGUID());
                break;
            case VEHICLE_DEATHSTALKER_VESCERI_MOUNT:
                if (TeamInInstance == HORDE)
                    pCreature->UpdateEntry(VEHICLE_LANA_STOUTHAMMER_MOUNT, ALLIANCE);
				VehicleListChampion.push_back(pCreature->GetGUID());
                break;
			case VEHICLE_ORGRIMMAR_WOLF:
                if (TeamInInstance == HORDE)
                    pCreature->UpdateEntry(VEHICLE_STORMWIND_STEED, ALLIANCE);
				VehicleListChampion.push_back(pCreature->GetGUID());
                break;
			case VEHICLE_SILVERMOON_HAWKSTRIDER:
                if (TeamInInstance == HORDE)
                    pCreature->UpdateEntry(VEHICLE_GNOMEREGAN_MECHANOSTRIDER, ALLIANCE);
				VehicleListChampion.push_back(pCreature->GetGUID());
                break;
			case VEHICLE_THUNDER_BLUFF_KODO:
                if (TeamInInstance == HORDE)
                    pCreature->UpdateEntry(VEHICLE_EXODAR_ELEKK, ALLIANCE);
				VehicleListChampion.push_back(pCreature->GetGUID());
                break;
			case VEHICLE_DARKSPEAR_RAPTOR:
                if (TeamInInstance == HORDE)
                    pCreature->UpdateEntry(VEHICLE_DARNASSIA_NIGHTSABER, ALLIANCE);
				VehicleListChampion.push_back(pCreature->GetGUID());
                break;	
			case VEHICLE_FORSAKE_WARHORSE:
                if (TeamInInstance == HORDE)
                    pCreature->UpdateEntry(VEHICLE_IRONFORGE_RAM, ALLIANCE);
				VehicleListChampion.push_back(pCreature->GetGUID());
                break;	
			case NPC_RISEN_JAEREN:
                if (TeamInInstance == ALLIANCE)
                    pCreature->UpdateEntry(NPC_RISEN_ARELAS, HORDE);
                break;	
            // Coliseum Announcer || Just NPC_JAEREN must be spawned.
            case NPC_JAEREN:
                uiAnnouncerGUID = pCreature->GetGUID();
                if (TeamInInstance == ALLIANCE)
                    pCreature->UpdateEntry(NPC_ARELAS,ALLIANCE);
                break;
            case NPC_JAEREN_AN:
                if (TeamInInstance == ALLIANCE)
                    pCreature->UpdateEntry(NPC_ARELAS_AN,ALLIANCE);
                break;				
            case VEHICLE_ARGENT_WARHORSE:
            case VEHICLE_ARGENT_BATTLEWORG:
                VehicleList.push_back(pCreature->GetGUID());
                break;
            case NPC_EADRIC:
            case NPC_PALETRESS:
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
            case GO_MAIN_GATE1:
                uiMainGate1GUID = pGO->GetGUID();
                break;
            case GO_CHAMPIONS_LOOT:
            case GO_CHAMPIONS_LOOT_H:
                uiChampionLootGUID = pGO->GetGUID();
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
                {
                    if (Creature* pAnnouncer =  instance->GetCreature(uiAnnouncerGUID))
                        pAnnouncer->AI()->SetData(DATA_IN_POSITION,0);
                }
                break;
            case BOSS_GRAND_CHAMPIONS:
                m_auiEncounter[0] = uiData;
                if (uiData == IN_PROGRESS)
                {
                    for(std::list<uint64>::const_iterator itr = VehicleList.begin(); itr != VehicleList.end(); ++itr)
                        if (Creature* pSummon = instance->GetCreature(*itr))
						{
							
							Vehicle* pVehicle=pSummon->GetVehicleKit();
							if(pVehicle)
								pVehicle->Dismiss();
							pSummon->SetVisibility(VISIBILITY_OFF);
						}
				}else if (uiData == DONE)
					{
                    ++uiGrandChampionsDeaths;
                    if (uiGrandChampionsDeaths == 3)
                    {
					
                        if (Creature* pAnnouncer =  instance->GetCreature(uiAnnouncerGUID))
                        {
							DoScriptText(SAY_START, pAnnouncer);
                            pAnnouncer->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                            pAnnouncer->SummonGameObject(instance->IsHeroic()? GO_CHAMPIONS_LOOT_H : GO_CHAMPIONS_LOOT,746.59,618.49,411.09,1.42,0, 0, 0, 0,90000000);
						}
                    }
                }else if(uiData== FAIL)
				{
					if (Creature* pAnnouncer =  instance->GetCreature(uiAnnouncerGUID))
					{
						
						for(std::list<uint64>::const_iterator itr = VehicleList.begin(); itr != VehicleList.end(); ++itr)
							if (Creature* pSummon = instance->GetCreature(*itr))
								pSummon->SetVisibility(VISIBILITY_ON);
						    	uiAgroDone = 0;
					

						//rimuovo le eventuali mount
						if(!VehicleListChampion.empty())
							for(std::list<uint64>::const_iterator itr = VehicleListChampion.begin(); itr != VehicleListChampion.end(); ++itr)
								if (Creature* pSummon = instance->GetCreature(*itr))
									pSummon->RemoveFromWorld();

						VehicleListChampion.clear();
						VehicleList.clear();
						//rimuovo gli eventuali boss
						if(Creature* boss1=instance->GetCreature(uiGrandChampion1GUID))
							boss1->RemoveFromWorld();
						if(Creature* boss2=instance->GetCreature(uiGrandChampion2GUID))
							boss2->RemoveFromWorld();
						if(Creature* boss3=instance->GetCreature(uiGrandChampion3GUID))
							boss3->RemoveFromWorld();
								
						
						uiAggroDone = 0;
						uiMovementDone = 0;
						uiGrandChampionsDeaths = 0;
						uiArgentSoldierDeaths = 0;

						
						
						uiGrandChampionVehicle1GUID   = 0;
						uiGrandChampionVehicle2GUID   = 0;
						uiGrandChampionVehicle3GUID   = 0;
						uiGrandChampion1GUID          = 0;
						uiGrandChampion2GUID          = 0;
						uiGrandChampion3GUID          = 0;
						uiChampionLootGUID            = 0;
						uiArgentChampionGUID          = 0;
						bDone = false;
						memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));
						pAnnouncer->AI()->SetData(DATA_RESET,0);
						//Initialize();
						
						//Creature* tmp=pAnnouncer->SummonCreature(35005, 746.626, 618.54, 411.09, 4.63158, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 600000);
						//tmp->AI()->SetData(DATA_START,0);
						 //pAnnouncer->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
						//pAnnouncer->RemoveFromWorld();
						//tmp->AI()->SetData(DATA_IN_POSITION,DONE);

						
						

					}
				}
                break;
            case DATA_ARGENT_SOLDIER_DEFEATED:
                uiArgentSoldierDeaths = uiData;
                if (uiArgentSoldierDeaths == 9)
                {
                    for(std::list<uint64>::const_iterator itr = VehicleList.begin(); itr != VehicleList.end(); ++itr)
                        if (Creature* pSummon = instance->GetCreature(*itr))
						{
                            pSummon->RemoveFromWorld();
						}
                    if (Creature* pBoss =  instance->GetCreature(uiArgentChampionGUID))
                    {
                        pBoss->RemoveFlag(UNIT_FIELD_FLAGS,UNIT_FLAG_NON_ATTACKABLE);
                        pBoss->RemoveFlag(UNIT_FIELD_FLAGS,UNIT_FLAG_NOT_SELECTABLE);
						//fix faction
						pBoss->setFaction(14);
                        pBoss->SetReactState(REACT_AGGRESSIVE);
                    }
                    if (Creature* pAnnouncer = instance->GetCreature(uiAnnouncerGUID))
                    {
						DoScriptText(SAY_START_9, pAnnouncer);
                    }
                }
                break;
            case BOSS_ARGENT_CHALLENGE_E:
                m_auiEncounter[1] = uiData;
				if (uiData == IN_PROGRESS)
                {
                    for(std::list<uint64>::const_iterator itr = VehicleList.begin(); itr != VehicleList.end(); ++itr)
                        if (Creature* pSummon = instance->GetCreature(*itr))
                            pSummon->RemoveFromWorld();
                }else if (uiData == DONE)				
                if (Creature* pAnnouncer = instance->GetCreature(uiAnnouncerGUID))
                {
         			DoScriptText(SAY_START, pAnnouncer);

                    pAnnouncer->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
					pAnnouncer->GetMotionMaster()->MovePoint(0,742.835, 639.134, 411.571);
                    pAnnouncer->SummonGameObject(instance->IsHeroic()? GO_EADRIC_LOOT_H : GO_EADRIC_LOOT,746.59,618.49,411.09,1.42,0, 0, 0, 0,90000000);
                }
                break;
            case DATA_AGGRO_DONE:
                uiAgroDone = uiData;
                if (Creature* pAnnouncer = instance->GetCreature(uiAnnouncerGUID))
                {   
				    
			    	DoScriptText(SAY_START11, pAnnouncer);			
                }
                break;
            case DATA_AGRO_DONE:
                uiAggroDone = uiData;
                if (Creature* pAnnouncer = instance->GetCreature(uiAnnouncerGUID))
                {   
				    pAnnouncer->SetVisibility(VISIBILITY_OFF);			    	
                }
                break;
            case BOSS_ARGENT_CHALLENGE_P:
                m_auiEncounter[2] = uiData;
				if (uiData == IN_PROGRESS)
                {
                    for(std::list<uint64>::const_iterator itr = VehicleList.begin(); itr != VehicleList.end(); ++itr)
                        if (Creature* pSummon = instance->GetCreature(*itr))
                            pSummon->RemoveFromWorld();
                }else if (uiData == DONE)				
                if (Creature* pAnnouncer = instance->GetCreature(uiAnnouncerGUID))
                {
         			DoScriptText(SAY_START, pAnnouncer);
                    pAnnouncer->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
					pAnnouncer->GetMotionMaster()->MovePoint(0,742.835, 639.134, 411.571);
                    pAnnouncer->SummonGameObject(instance->IsHeroic()? GO_PALETRESS_LOOT_H : GO_PALETRESS_LOOT,746.59,618.49,411.09,1.42,0, 0, 0, 0,90000000);
                }
                break;
        }

        if (uiGrandChampionsDeaths == 3)
            SaveToDB();
    }

    uint32 GetData(uint32 uiData)
    {
        switch(uiData)
        {
            case BOSS_GRAND_CHAMPIONS:  return m_auiEncounter[0];
            case BOSS_ARGENT_CHALLENGE_E: return m_auiEncounter[1];
            case BOSS_ARGENT_CHALLENGE_P: return m_auiEncounter[2];
            case BOSS_BLACK_KNIGHT: return m_auiEncounter[3];
			
		    case DATA_MOVEMENT_DONE: return uiMovementDone;
            case DATA_ARGENT_SOLDIER_DEFEATED: return uiArgentSoldierDeaths;
        }

        return 0;
    }

    uint64 GetData64(uint32 uiData)
    {
        switch(uiData)
        {
            case DATA_ANNOUNCER: return uiAnnouncerGUID;
            case DATA_MAIN_GATE: return uiMainGateGUID;
            case DATA_MAIN_GATE1: return uiMainGate1GUID;

            case DATA_GRAND_CHAMPION_1: return uiGrandChampion1GUID;
            case DATA_GRAND_CHAMPION_2: return uiGrandChampion2GUID;
            case DATA_GRAND_CHAMPION_3: return uiGrandChampion3GUID;
        }

        return 0;
    }

	//posso aggiornare i guid
    void SetData64(uint32 uiType, uint64 uiData)
    {
        switch(uiType)
        {
            case DATA_GRAND_CHAMPION_1:
                uiGrandChampion1GUID = uiData;
                break;
            case DATA_GRAND_CHAMPION_2:
                uiGrandChampion2GUID = uiData;
                break;
            case DATA_GRAND_CHAMPION_3:
                uiGrandChampion3GUID = uiData;
                break;
        }
    }

    std::string GetSaveData()
    {
        OUT_SAVE_INST_DATA;

        std::ostringstream saveStream;

        saveStream << "T C" << m_auiEncounter[0]
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
        } else OUT_LOAD_INST_DATA_FAIL;

        OUT_LOAD_INST_DATA_COMPLETE;
    }
};

InstanceData* GetInstanceData_instance_trial_of_the_champion(Map* pMap)
{
    return new instance_trial_of_the_champion(pMap);
}

//registrazione script
void AddSC_instance_trial_of_the_champion()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "instance_trial_of_the_champion";
    newscript->GetInstanceData = &GetInstanceData_instance_trial_of_the_champion;
    newscript->RegisterSelf();
}
