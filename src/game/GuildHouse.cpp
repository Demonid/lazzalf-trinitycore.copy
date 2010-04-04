/*
 * Copyright (C) 2008-2010 Trinity <http://www.trinitycore.org/>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/**
 *
 * @File : GuildHouse.cpp
 *
 * @Authors : Lazzalf
 *
 * @Date : 31/03/2010
 *
 * @Version : 0.1
 *
 **/

#include "GuildHouse.h"

GuildHouseMap GH_map;
GH_Add GH_AddHouse;
GuildGuardID mGuildGuardID;

bool CheckGuildID(uint32 guild_id)
{
    if (!guild_id)
        return false;

    Guild* guild = objmgr.GetGuildById(guild_id);  
    
    if(!guild_id)
    {
        sLog.outError("La gilda %u non esiste", guild_id);
        return false;
    }
    return true;
}

bool CheckGuildHouse(uint32 guild_id)
{
    GuildHouseMap::const_iterator itr = GH_map.find(guild_id);
    if(itr == GH_map.end()) 
        return false;
    if(itr->second.Id == 0)
        return false;
    return true;
}

bool ChangeGuildHouse(uint32 guild_id, uint32 newid)
{
    GuildHouseMap::iterator itr = GH_map.find(guild_id);
    if(itr == GH_map.end()) //Inserisci nuova classe per la gilda
    {
        uint32 add = 0;

        QueryResult_AutoPtr result = CharacterDatabase.PQuery("SELECT `GuildHouse_Add` FROM `gh_guildadd` WHERE `guildId` = %u", guild_id);
        if(result)
        {
            Field *fields = result->Fetch();
            add = fields[0].GetUInt32();
        }

        GuildHouse NewGH(guild_id, add);
        GH_map[guild_id] = NewGH;

        itr = GH_map.find(guild_id);
        if(itr == GH_map.end())
            return false;
    }

    if(newid == 0) // Vendi
    {
        QueryResult_AutoPtr result = WorldDatabase.PQuery("UPDATE `guildhouses` SET `guildId` = 0 WHERE `guildId` = %u", guild_id);
        RemoveGuildHouseAdd(itr->second.Id);
        itr->second.Id = 0;                
    }
    else // Compra
    {        
        QueryResult_AutoPtr result = WorldDatabase.PQuery("UPDATE `guildhouses` SET `guildId` = %u WHERE `id` = %u", guild_id, newid);
        itr->second.ChangeId(newid);
        AddGuildHouseAdd(newid, itr->second.GuildHouse_Add, guild_id);
    }
    return true;
}

bool GetGuildHouseLocation(uint32 guild_id, float &x, float &y, float &z, float &o, uint32 &map)
{
    GuildHouseMap::const_iterator itr = GH_map.find(guild_id);
    if(itr == GH_map.end()) 
        return false;
    if(itr->second.Id == 0)
        return false;
    x = itr->second.m_X;
    y = itr->second.m_Y;
    z = itr->second.m_Z;
    o = itr->second.m_orient;
    map = itr->second.m_map;
    return true;
}

bool GetGuildHouseMap(uint32 guild_id, uint32 &map)
{
    GuildHouseMap::const_iterator itr = GH_map.find(guild_id);
    if(itr == GH_map.end()) 
        return false;
    if(itr->second.Id == 0)
        return false;
    map = itr->second.m_map;
    return true;
}

bool RemoveGuildHouseAdd(uint32 id)
{
    for(uint32 i = 1; i < NPC_MAX; i++)
    {
        uint32 find = ((id << 16) | i);
        GH_Add::iterator itr = GH_AddHouse.find(find);
        if(itr == GH_AddHouse.end()) 
            continue;
        if(!(itr->second.spawned))  // Non despawnare se è già despawnato
                continue;
        Item_Vector::iterator itr2 =  itr->second.AddCre.begin();
        for(; itr2 != itr->second.AddCre.end(); itr2++)
        {
            if( CreatureData const* data = objmgr.GetCreatureData(*itr2) )
            {
                objmgr.RemoveCreatureFromGrid(*itr2, data);
                if( Creature* pCreature = ObjectAccessor::Instance().GetObjectInWorld(MAKE_NEW_GUID(*itr2, data->id, HIGHGUID_UNIT), (Creature*)NULL) )
                    pCreature->AddObjectToRemoveList();
            }
        }
        itr2 =  itr->second.AddGO.begin();
        for(; itr2 != itr->second.AddGO.end(); itr2++)
        {
            if (GameObjectData const* data = objmgr.GetGOData(*itr2))
            { 
                objmgr.RemoveGameobjectFromGrid(*itr2, data);
                if( GameObject* pGameobject = ObjectAccessor::Instance().GetObjectInWorld(MAKE_NEW_GUID(*itr2, data->id, HIGHGUID_GAMEOBJECT), (GameObject*)NULL) )
                     pGameobject->AddObjectToRemoveList();
            }
        }               
        itr->second.spawned = false;
    }
    return true;
}

bool AddGuildHouseAdd(uint32 id, uint32 add, uint32 guild)
{
    for(uint8 i = 1; i < NPC_MAX; i++)
    {
        if(((uint32)1 << (i-1)) & add)
        {
            uint32 find = ((id << 16) | i);
            GH_Add::iterator itr = GH_AddHouse.find(find);
            if(itr == GH_AddHouse.end())
                continue;
            if(itr->second.spawned) //Non rispawnare se è già spawnato
                continue;
            Item_Vector::iterator itr2 =  itr->second.AddGO.begin();
            for(; itr2 != itr->second.AddGO.end(); itr2++)
            {
                if (GameObjectData const* data = objmgr.GetGOData(*itr2))
                {
                    objmgr.AddGameobjectToGrid(*itr2, data);

                    Map* map = const_cast<Map*>(MapManager::Instance().CreateBaseMap(data->mapid));

                    if (!map->Instanceable() && map->IsLoaded(data->posX, data->posY))
                    {
                        GameObject* pGameobject = new GameObject;
                        //sLog.outDebug("Spawning gameobject %u", itr2->first);
                        if (!pGameobject->LoadFromDB(*itr2, map))
                            delete pGameobject;
                        else
                        {
                            if (pGameobject->isSpawnedByDefault())
                                map->Add(pGameobject);
                        }
                    }                       
                }
            }
            itr2 =  itr->second.AddCre.begin();
            for(; itr2 != itr->second.AddCre.end(); itr2++)
            {                
                if (CreatureData const* cre_data = objmgr.GetCreatureData(*itr2))
                {
                    if (CreatureData const* data = objmgr.GetCreatureData(*itr2))
                    {
                        objmgr.AddCreatureToGrid(*itr2, data);

                        Map* map = const_cast<Map*>(MapManager::Instance().CreateBaseMap(data->mapid));
                        
                        if (!map->Instanceable() && map->IsLoaded(data->posX, data->posY))
                        {
                            Creature* pCreature = new Creature;
                            //sLog.outDebug("Spawning creature %u",itr2->first);
                            if (!pCreature->LoadFromDB(*itr2, map))
                                delete pCreature;
                            else
                                map->Add(pCreature);
                        }                           
                        if(i == 2) //Guardie
                            UpdateGuardMap(MAKE_NEW_GUID(*itr2, data->id, HIGHGUID_GAMEOBJECT), guild);
                    }
                                              
                }
                
            }
            itr->second.spawned = true;
        }
    }
    return true;
}
        

void LoadGuildHouse()
{
    GH_map.clear();
    QueryResult_AutoPtr result = WorldDatabase.Query("SELECT `id`,`guildId`,`x`,`y`,`z`,`map` FROM guildhouses ORDER BY guildId ASC");

    if (!result)
    {
        barGoLink bar(1);

        bar.step();

        sLog.outString("");
        sLog.outErrorDb(">> Loaded 0 guild house.");
        return;
    }

    barGoLink bar(result->GetRowCount());

    do
    {
        Field *fields = result->Fetch();
        bar.step();

        uint32 id = fields[0].GetUInt32();
        uint32 guildID = fields[1].GetUInt32();
        float x   = fields[2].GetFloat();
        float y   = fields[3].GetFloat();
        float z   = fields[4].GetFloat();
        uint32 map = fields[5].GetUInt32();
        uint32 add = 0;

        if(guildID)
        {
            if(!CheckGuildID(guildID))
                continue;

            QueryResult_AutoPtr result2 = CharacterDatabase.PQuery("SELECT `GuildHouse_Add` FROM `gh_guildadd` WHERE `guildId` = %u", guildID);
            if(result2)
            {
                Field *fields2 = result2->Fetch();
                add = fields2[0].GetUInt32();
            }

            GuildHouse NewGH(guildID, id, x, y, z, map, add);
            GH_map[guildID] = NewGH;

            RemoveGuildHouseAdd(id);
            AddGuildHouseAdd(id, add, guildID);
        }
        else
            RemoveGuildHouseAdd(id);
    } while (result->NextRow());

    sLog.outString();
    sLog.outString( ">> Loaded %u GuildHouse", GH_map.size() );
}

void LoadGuildHouseAdd()
{
    GH_AddHouse.clear();
    mGuildGuardID.clear();
    sLog.outString( "Loading Guild House System");
    sLog.outString("");

    QueryResult_AutoPtr result = WorldDatabase.Query("SELECT `guid`,`type`,`id`,`add_type` FROM guildhouses_add ORDER BY Id ASC");

    if (!result)
    {
        barGoLink bar(1);

        bar.step();

        sLog.outString("");
        sLog.outErrorDb(">> Loaded 0 GuildHouse Add");
        return;
    }

    barGoLink bar(result->GetRowCount());

    do
    {
        Field *fields = result->Fetch();
        bar.step();

        uint32 guid         = fields[0].GetUInt32();
        uint16 type         = fields[1].GetUInt16();
        uint16 id           = fields[2].GetUInt16();
        uint16 add_type     = fields[3].GetUInt16();

        uint32 find = 0;
        find = ( (uint32)id << 16 ) | (uint32)add_type;
        if(type == CREATURE)
        {
            if(!objmgr.GetCreatureData(guid))                
            {                
                sLog.outString( "Data per Creature Guid %u non esistente", guid );
                continue;
            }
            GH_AddHouse[find].AddCre.push_back(guid);
        }
        else if(type == OBJECT)
        {
            if(!objmgr.GetGOData(guid))
            {                
                sLog.outString( "Data per GameObject Guid %u non esistente", guid );
                continue;
            }
            GH_AddHouse[find].AddGO.push_back(guid);
        }
    } while (result->NextRow());

    sLog.outString();
    sLog.outString( ">> Loaded %u GuildHouse Add", GH_AddHouse.size() );
}

GuildHouse::GuildHouse(uint32 guild_id, uint32 guild_add)
{
    GuildId = guild_id;    
    GuildHouse_Add = guild_add;
    Id = 0;
    m_X = 0;
    m_Y = 0;  
    m_Z = 0;
    m_orient = 0;
    m_map = 0;
}

GuildHouse::GuildHouse(uint32 newGuildId, uint32 newId, float x, float y, float z, uint32 map, uint32 add)
{
    GuildId = newGuildId;
    Id = newId;
    m_X = x;
    m_Y = y;  
    m_Z = z;
    m_map = map;
    m_orient = 0;
    GuildHouse_Add = add;
}

void GuildHouse::ChangeId(uint32 newid)
{
    QueryResult_AutoPtr result = WorldDatabase.PQuery("SELECT `x`, `y`, `z`, `map` FROM `guildhouses` WHERE `id` = %u", newid);
    if(result)
    {
        Id = newid;
        Field *fields = result->Fetch();
        m_X = fields[0].GetFloat();
        m_Y = fields[1].GetFloat();
        m_Z = fields[2].GetFloat();
        m_map = fields[3].GetUInt32();        
        return;
    }
};

void GuildHouse::SetGuildHouse_Add(uint32 NewAdd)
{
    GuildHouse_Add = NewAdd;
    QueryResult_AutoPtr result = CharacterDatabase.PQuery("UPDATE `guildhouses_guildadd` SET `GuildHouse_Add` = %u WHERE `guildId` = %u", GuildHouse_Add, GuildId);
};

void GuildHouse::AddGuildHouse_Add(uint32 NewAdd)
{
    GuildHouse_Add &= NewAdd;
    QueryResult_AutoPtr result = CharacterDatabase.PQuery("UPDATE `guildhouses_guildadd` SET `GuildHouse_Add` = %u WHERE `guildId` = %u", GuildHouse_Add, GuildId);
    AddGuildHouseAdd(Id, NewAdd, GuildId);
};

// Guardie di gilda
/*void LoadGuildGuardID()
{
    mGuildGuardID.clear();
    QueryResult_AutoPtr result = WorldDatabase.Query("SELECT creature_guid, guild_house_id FROM guild_guard ORDER BY creature_guid ASC");

    if (!result)
    {
        barGoLink bar(1);

        bar.step();

        sLog.outString("");
        sLog.outErrorDb(">> Loaded 0 guild guards. DB table `guild_guards` is empty.");
        return;
    }

    barGoLink bar(result->GetRowCount());

    do
    {
        Field *fields = result->Fetch();
        bar.step();

        uint64 creature_guid = fields[0].GetUInt64();
        uint32 guid_house_id = fields[1].GetUInt64();
        QueryResult_AutoPtr result2 = WorldDatabase.PQuery("SELECT guildId FROM guildhouses WHERE id = %u", guid_house_id);
        
        if (result2)
        {
            Field *fields2 = result2->Fetch();
            uint32 guild_id = fields2[0].GetUInt32();

            if(CheckGuildGuardID(creature_guid, guild_id))
                mGuildGuardID[creature_guid] = guild_id;
            else 
                mGuildGuardID[creature_guid] = 0;            
        }
        else
        {
            sLog.outError("La GuildHouse '%u' non esiste in guildhouses per il caricamento delle guardia %u", guid_house_id, creature_guid);
        }
    } while (result->NextRow());   

    sLog.outString();
    sLog.outString( ">> Loaded %u guild guard", mGuildGuardID.size() );
}*/

uint32 GetGuildByGuardID(uint64 guid)
{
    GuildGuardID::const_iterator itr = mGuildGuardID.find(guid);
    if(itr == mGuildGuardID.end()) return 0;
    return itr->second;
}


void UpdateGuardMap(uint64 guid, uint32 guild)
{
    GuildGuardID::iterator itr = mGuildGuardID.find(guid);
    if(itr == mGuildGuardID.end()) 
        mGuildGuardID[guid] = guild;
    else
        itr->second = guild;
}