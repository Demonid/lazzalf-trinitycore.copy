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

#include "ScriptedPch.h"
#include "GuildHouse.h"

GuildHouseMap GH_map;
GH_Add GH_AddHouse;

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

bool CheckGuildHouse(uint32 guild, GuildHouse &GHm)
{
    GuildHouseMap::const_iterator itr = GH_map.find(guild);
    if(itr == GH_map.end()) 
        return false;
    GHm = itr->second;
    return true;
}

bool GetGuildHouseLocation(uint32 guild, float &x, float &y, float &z, float &o, uint32 &map)
{
    GuildHouseMap::const_iterator itr = GH_map.find(guild);
    if(itr == GH_map.end()) 
        return false;
    x = itr->second.m_X;
    y = itr->second.m_Y;
    z = itr->second.m_Z;
    o = itr->second.m_orient;
    map = itr->second.m_map;
    return true;
}

void LoadGuildHouse()
{
    GH_map.clear();
    QueryResult_AutoPtr result = WorldDatabase.Query("SELECT * FROM guildhouses WHERE guildId <> 0 ORDER BY guildId ASC");

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
        uint32 x = fields[2].GetFloat();
        uint32 y = fields[3].GetFloat();
        uint32 z = fields[4].GetFloat();
        uint16 map = fields[5].GetUInt32();
        uint32 add = 0;

        if(!CheckGuildID(guildID))
            continue;

        QueryResult_AutoPtr result2 = CharacterDatabase.PQuery("SELECT `GuildHouse_Add` FROM `guildhouses_guildadd` WHERE `guildId` = %u", guildID);
        if(result2)
        {
            Field *fields2 = result2->Fetch();
            add = fields2[0].GetUInt32();
        }

        GuildHouse NewGH(guildID, id, x, y, z, map, add);
        GH_map[guildID] = NewGH;
        
    } while (result->NextRow());

    sLog.outString();
    sLog.outString( ">> Loaded %u GuildHouse", GH_map.size() );
}

GuildHouse::GuildHouse()
{
    GuildId = 0;
    Id = 0;
    GuildHouse_Add = 0;
    m_X = 0;
    m_Y = 0;  
    m_Z = 0;
    m_orient = 0;
    m_map = 0;
}

GuildHouse::GuildHouse(uint32 guildID, uint32 id, uint32 x, uint32 y, uint32 z, uint32 map, uint32 add)
{
    GuildId = guildID;
    Id = id;
    m_X = x;
    m_Y = y;  
    m_Z = z;
    m_map = map;
    m_orient = 0;
    GuildHouse_Add = add;
}

void GuildHouse::SetGuildHouse(uint32 guildID, uint32 id, uint32 x, uint32 y, uint32 z, uint32 map)
{
    GuildId = guildID;
    Id = id;
    m_X = x;
    m_Y = y;  
    m_Z = z;
    m_map = map;
}

void GuildHouse::ChangeId(uint32 newid)
{
    QueryResult_AutoPtr result = WorldDatabase.PQuery("UPDATE `guildhouses` SET `guildId` = %u WHERE `id` = %u", GuildId, newid);
    result = WorldDatabase.PQuery("SELECT `x`, `y`, `z`, `map` FROM `guildhouses` WHERE `guildId` = %u", GuildId);
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

void GuildHouse::ChangeGuildHouse_Add(uint32 NewAdd)
{
    GuildHouse_Add &= NewAdd;
    QueryResult_AutoPtr result = CharacterDatabase.PQuery("UPDATE `guildhouses_guildadd` SET `GuildHouse_Add` = %u WHERE `guildId` = %u", GuildHouse_Add, GuildId);
};