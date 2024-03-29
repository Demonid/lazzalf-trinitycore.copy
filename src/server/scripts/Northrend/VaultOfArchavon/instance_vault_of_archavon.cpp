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
#include "vault_of_archavon.h"
#include "OutdoorPvPMgr.h"
#include "../../OutdoorPvP/OutdoorPvPWG.h"

#define ENCOUNTERS  4

/* Vault of Archavon encounters:
1 - Archavon the Stone Watcher event
2 - Emalon the Storm Watcher event
3 - Koralon the Flame Watcher event
4 - Toravon the Ice Watcher event
*/

// Earth, Wind & Fire
#define ACHIEVEMENT_EARTH_WIND_FIRE_10  4016
#define ACHIEVEMENT_EARTH_WIND_FIRE_25  4017
#define EWF_MAX_TIMER                   60 * 1000 // 60s
#define EWF_WATCHERS_COUNT              3

class instance_archavon : public InstanceMapScript
{
public:
    instance_archavon() : InstanceMapScript("instance_archavon", 624) { }

    InstanceScript* GetInstanceScript(InstanceMap* pMap) const
    {
        return new instance_archavon_InstanceMapScript(pMap);
    }

    struct instance_archavon_InstanceMapScript : public InstanceScript
    {
        instance_archavon_InstanceMapScript(Map *Map) : InstanceScript(Map) {};

        uint32 uiEncounters[ENCOUNTERS];

        uint64 uiArchavon;
        uint64 uiEmalon;
        uint64 uiKoralon;
        uint64 uiToravon;

        // Earth Wind & Fire
        bool ewfStartCount;
        uint32 watchersCount;
        uint32 timer;
        uint32 achievementEWF;

        void Initialize()
        {
            uiArchavon = 0;
            uiEmalon = 0;
            uiKoralon = 0;
            uiToravon = 0;

            for (uint8 i = 0; i < ENCOUNTERS; i++)
                uiEncounters[i] = NOT_STARTED;

            ewfStartCount = false;
            watchersCount = 0;
            timer = 0;
            achievementEWF = 0;
        }

        bool IsEncounterInProgress() const
        {
            for (uint8 i = 0; i < ENCOUNTERS; i++)
                if (uiEncounters[i] == IN_PROGRESS)
                    return true;

            return false;
        }

        void OnPlayerEnter(Player *m_player)
        {
            if (sWorld->getBoolConfig(CONFIG_OUTDOORPVP_WINTERGRASP_ENABLED))
            {
                if(OutdoorPvPWG *pvpWG = (OutdoorPvPWG*)sOutdoorPvPMgr->GetOutdoorPvPToZoneId(4197))
                {
                    if ((pvpWG->getDefenderTeamId()==TEAM_ALLIANCE) && (m_player->ToPlayer()->GetTeam() == ALLIANCE))
                        return;
                    else if ((pvpWG->getDefenderTeamId()!=TEAM_ALLIANCE) && (m_player->ToPlayer()->GetTeam() == HORDE))
                        return;
                    else m_player->CastSpell(m_player, SPELL_TELEPORT_FORTRESS, true);
                }
            }
        }

        void OnCreatureCreate(Creature *creature)
        {
            switch(creature->GetEntry())
            {
                case CREATURE_ARCHAVON: uiArchavon  = creature->GetGUID(); break;
                case CREATURE_EMALON:   uiEmalon    = creature->GetGUID(); break;
                case CREATURE_KORALON:  uiKoralon   = creature->GetGUID(); break;
                case CREATURE_TORAVON:  uiToravon   = creature->GetGUID(); break;
            }
        }

        uint32 GetData(uint32 type)
        {
            switch(type)
            {
                case DATA_ARCHAVON_EVENT:   return uiEncounters[0];
                case DATA_EMALON_EVENT:     return uiEncounters[1];
                case DATA_KORALON_EVENT:    return uiEncounters[2];
                case DATA_TORAVON_EVENT:    return uiEncounters[3];
            }
            return 0;
        }

        uint64 GetData64(uint32 identifier)
        {
            switch(identifier)
            {
                case DATA_ARCHAVON: return uiArchavon;
                case DATA_EMALON:   return uiEmalon;
                case DATA_KORALON:  return uiKoralon;
                case DATA_TORAVON:  return uiToravon;
            }
            return 0;
        }

        void SetData(uint32 type, uint32 data)
        {
            switch(type)
            {
                case DATA_ARCHAVON_EVENT:   uiEncounters[0] = data; break;
                case DATA_EMALON_EVENT:     uiEncounters[1] = data; break;
                case DATA_KORALON_EVENT:    uiEncounters[2] = data; break;
                case DATA_TORAVON_EVENT:    uiEncounters[3] = data; break;
                case DATA_EWF_START:
                    if (data == ACHI_START)
                        timer = EWF_MAX_TIMER;
                    else if (data == ACHI_FAILED || data == ACHI_COMPLETED)
                        timer = 0;
                    break;
                case DATA_EWF_COUNT:
                    if (data == ACHI_INCREASE)
                        watchersCount++;
                    break;
            }

            if (data == DONE)
                SaveToDB();
        }

        void Update(uint32 diff)
        {
            // Achievement Earth, Wind & Fire control
            if (timer)
            {
                if (watchersCount == EWF_WATCHERS_COUNT)
                {
                    if (Difficulty(instance->GetSpawnMode()) == RAID_DIFFICULTY_10MAN_NORMAL)
                        achievementEWF = ACHIEVEMENT_EARTH_WIND_FIRE_10;
                    if (Difficulty(instance->GetSpawnMode()) == RAID_DIFFICULTY_25MAN_NORMAL)
                        achievementEWF = ACHIEVEMENT_EARTH_WIND_FIRE_25;

                    AchievementEntry const *AchievEWF = GetAchievementStore()->LookupEntry(achievementEWF);
                    if (AchievEWF)
                        DoCompleteAchievement(achievementEWF);

                    SetData(DATA_EWF_START, ACHI_COMPLETED);
                }

                if (timer <= diff)
                    SetData(DATA_EWF_START, ACHI_FAILED);
                else timer -= diff;
            }
        }

        std::string GetSaveData()
        {
            OUT_SAVE_INST_DATA;
            std::ostringstream stream;
            stream << uiEncounters[0] << " " << uiEncounters[1] << " " << uiEncounters[2] << " " << uiEncounters[3];

            char* out = new char[stream.str().length() + 1];
            strcpy(out, stream.str().c_str());
            if (out)
            {
                OUT_SAVE_INST_DATA_COMPLETE;
                return out;
            }

            return NULL;
        }

        void Load(const char* in)
        {
            if (!in)
            {
                OUT_LOAD_INST_DATA_FAIL;
                return;
            }

            OUT_LOAD_INST_DATA(in);

            std::istringstream stream(in);
            stream >> uiEncounters[0] >> uiEncounters[1] >> uiEncounters[2] >> uiEncounters[3];

            for (uint8 i = 0; i < ENCOUNTERS; ++i)
                if (uiEncounters[i] == IN_PROGRESS)
                    uiEncounters[i] = NOT_STARTED;

            OUT_LOAD_INST_DATA_COMPLETE;
        }
    };

};


void AddSC_instance_archavon()
{
    new instance_archavon();
}
