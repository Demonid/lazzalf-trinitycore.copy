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

#include "ScriptPCH.h"
#include "culling_of_stratholme.h"

#define MAX_ENCOUNTER 5

/* Culling of Stratholme encounters:
0 - Meathook
1 - Salramm the Fleshcrafter
2 - Chrono-Lord Epoch
3 - Mal'Ganis
4 - Infinite Corruptor (Heroic only)
*/

class instance_culling_of_stratholme : public InstanceMapScript
{
public:
    instance_culling_of_stratholme() : InstanceMapScript("instance_culling_of_stratholme", 595) { }

    InstanceScript* GetInstanceScript(InstanceMap* pMap) const
    {
        return new instance_culling_of_stratholme_InstanceMapScript(pMap);
    }

    struct instance_culling_of_stratholme_InstanceMapScript : public InstanceScript
    {
        instance_culling_of_stratholme_InstanceMapScript(Map* pMap) : InstanceScript(pMap) {Initialize();};

        uint64 uiArthas;
        uint64 uiMeathook;
        uint64 uiSalramm;
        uint64 uiEpoch;
        uint64 uiMalGanis;
        uint64 uiInfinite;

        uint64 uiShkafGate;
        uint64 uiMalGanisGate1;
        uint64 uiMalGanisGate2;
        uint64 uiExitGate;
        uint64 uiMalGanisChest;
        uint32 Minute;
        uint32 tMinutes;

        bool m_started;

        uint32 m_auiEncounter[MAX_ENCOUNTER];
        std::string str_data;

        bool IsEncounterInProgress() const
        {
            for (uint8 i = 0; i < MAX_ENCOUNTER; ++i)
                if (m_auiEncounter[i] == IN_PROGRESS) return true;

            return false;
        }

        void Inizialize()
        {
            DoUpdateWorldState(WORLD_STATE_TIMER, 0);
            DoUpdateWorldState(WORLD_STATE_TIME_COUNTER, 0);
            DoUpdateWorldState(WORLD_STATE_WAVES, 0);
            DoUpdateWorldState(WORLD_STATE_CRATES, 0);
            DoUpdateWorldState(WORLD_STATE_CRATES_2, 0);

            Minute = 60000;
            tMinutes = 0;
            m_started = false;
        }

        void OnCreatureCreate(Creature* creature)
        {
            switch(creature->GetEntry())
            {
                case NPC_ARTHAS:
                    uiArthas = creature->GetGUID();
                    break;
                case NPC_MEATHOOK:
                    uiMeathook = creature->GetGUID();
                    break;
                case NPC_SALRAMM:
                    uiSalramm = creature->GetGUID();
                    break;
                case NPC_EPOCH:
                    uiEpoch = creature->GetGUID();
                    break;
                case NPC_MAL_GANIS:
                    uiMalGanis = creature->GetGUID();
                    break;
                case NPC_INFINITE:
                    uiInfinite = creature->GetGUID();
                    DoUpdateWorldState(WORLD_STATE_TIMER, 1);
                    DoUpdateWorldState(WORLD_STATE_TIME_COUNTER, 25);
                    break;
            }
        }

        void OnGameObjectCreate(GameObject* go)
        {
            switch(go->GetEntry())
            {
                case GO_SHKAF_GATE:
                    uiShkafGate = go->GetGUID();
                    break;
                case GO_MALGANIS_GATE_1:
                    uiMalGanisGate1 = go->GetGUID();
                    break;
                case GO_MALGANIS_GATE_2:
                    uiMalGanisGate2 = go->GetGUID();
                    break;
                case GO_EXIT_GATE:
                    uiExitGate = go->GetGUID();
                    if (m_auiEncounter[3] == DONE)
                        HandleGameObject(uiExitGate,true);
                    break;
                case GO_MALGANIS_CHEST_N:
                case GO_MALGANIS_CHEST_H:
                    uiMalGanisChest = go->GetGUID();
                    if (m_auiEncounter[3] == DONE)
                        go->RemoveFlag(GAMEOBJECT_FLAGS,GO_FLAG_INTERACT_COND);
                    break;
            }
        }

        void SetData(uint32 type, uint32 data)
        {
            switch(type)
            {
                case DATA_MEATHOOK_EVENT:
                    m_auiEncounter[0] = data;
                    break;
                case DATA_SALRAMM_EVENT:
                    m_auiEncounter[1] = data;
                    break;
                case DATA_EPOCH_EVENT:
                    m_auiEncounter[2] = data;
                    break;
                case DATA_MAL_GANIS_EVENT:
                    m_auiEncounter[3] = data;

                    switch(m_auiEncounter[3])
                    {
                        case NOT_STARTED:
                            HandleGameObject(uiMalGanisGate2,true);
                            break;
                        case IN_PROGRESS:
                            HandleGameObject(uiMalGanisGate2,false);
                            break;
                        case DONE:
                            HandleGameObject(uiExitGate, true);
                            if (GameObject* go = instance->GetGameObject(uiMalGanisChest))
                                go->RemoveFlag(GAMEOBJECT_FLAGS,GO_FLAG_INTERACT_COND);
                            break;
                    }
                    break;
                case DATA_INFINITE_EVENT:
                    m_auiEncounter[4] = data;
                    if (data == DONE)
                    {
                        DoUpdateWorldState(WORLD_STATE_TIMER, 0);
                        DoUpdateWorldState(WORLD_STATE_TIME_COUNTER, 0);
                    }
                    break;
                case DATA_INFINITE_EVENT_START:
                    m_started = true;
                    break;
            }

            if (data == DONE)
                SaveToDB();
        }

        uint32 GetData(uint32 type)
        {
            switch(type)
            {
                case DATA_MEATHOOK_EVENT:             return m_auiEncounter[0];
                case DATA_SALRAMM_EVENT:              return m_auiEncounter[1];
                case DATA_EPOCH_EVENT:                return m_auiEncounter[2];
                case DATA_MAL_GANIS_EVENT:            return m_auiEncounter[3];
                case DATA_INFINITE_EVENT:             return m_auiEncounter[4];
            }
            return 0;
        }

        uint64 GetData64(uint32 identifier)
        {
            switch(identifier)
            {
                case DATA_ARTHAS:                     return uiArthas;
                case DATA_MEATHOOK:                   return uiMeathook;
                case DATA_SALRAMM:                    return uiSalramm;
                case DATA_EPOCH:                      return uiEpoch;
                case DATA_MAL_GANIS:                  return uiMalGanis;
                case DATA_INFINITE:                   return uiInfinite;
                case DATA_SHKAF_GATE:                 return uiShkafGate;
                case DATA_MAL_GANIS_GATE_1:           return uiMalGanisGate1;
                case DATA_MAL_GANIS_GATE_2:           return uiMalGanisGate2;
                case DATA_EXIT_GATE:                  return uiExitGate;
                case DATA_MAL_GANIS_CHEST:            return uiMalGanisChest;
            }
            return 0;
        }

        void Update(uint32 diff)
        {        
           if (tMinutes == 25)
           {
               m_auiEncounter[4] = FAIL;
               if (Creature *pInfinite = instance->GetCreature(uiInfinite))
               {
                   pInfinite->DisappearAndDie();
                   pInfinite->SetLootRecipient(NULL);
               }
               DoUpdateWorldState(WORLD_STATE_TIMER, 0);             
           }

           if (m_started)
           {
               if (Minute <= diff)
               {
                  tMinutes++;
                  DoUpdateWorldState(WORLD_STATE_TIME_COUNTER, 25 - tMinutes);
                  Minute = 60000;
               }
               else 
                   Minute -= diff;
           }
           return;
        }

        std::string GetSaveData()
        {
            OUT_SAVE_INST_DATA;

            std::ostringstream saveStream;
            saveStream << "C S " << m_auiEncounter[0] << " " << m_auiEncounter[1] << " "
                << m_auiEncounter[2] << " " << m_auiEncounter[3] << " " << m_auiEncounter[4];

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
            uint16 data0, data1, data2, data3, data4;

            std::istringstream loadStream(in);
            loadStream >> dataHead1 >> dataHead2 >> data0 >> data1 >> data2 >> data3 >> data4;

            if (dataHead1 == 'C' && dataHead2 == 'S')
            {
                m_auiEncounter[0] = data0;
                m_auiEncounter[1] = data1;
                m_auiEncounter[2] = data2;
                m_auiEncounter[3] = data3;
                m_auiEncounter[4] = data4;

                for (uint8 i = 0; i < MAX_ENCOUNTER; ++i)
                    if (m_auiEncounter[i] == IN_PROGRESS)
                        m_auiEncounter[i] = NOT_STARTED;

            } else OUT_LOAD_INST_DATA_FAIL;

            OUT_LOAD_INST_DATA_COMPLETE;
        }
    };

};


void AddSC_instance_culling_of_stratholme()
{
    new instance_culling_of_stratholme();
}
