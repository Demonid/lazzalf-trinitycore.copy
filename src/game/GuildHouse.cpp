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

        QueryResult_AutoPtr result = CharacterDatabase.PQuery("SELECT `GuildHouse_Add` FROM `guildhouses_guildadd` WHERE `guildId` = %u", guild_id);
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
        QueryResult_AutoPtr result = WorldDatabase.PQuery("UPDATE `guildhouses` SET `guildId` = 0 WHERE `id` = %u", newid);
        itr->second.Id = 0;
        //RemoveGuildHouseAdd(newid);        
    }
    else // Compra
    {        
        QueryResult_AutoPtr result = WorldDatabase.PQuery("UPDATE `guildhouses` SET `guildId` = %u WHERE `id` = %u", guild_id, newid);
        itr->second.ChangeId(newid);
        //RemoveGuildHouseAdd(newid);
    }
    return true;
}

bool GetGuildHouseLocation(uint32 guild_id, float &x, float &y, float &z, float &o, uint16 &map)
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

bool RemoveGuildHouseAdd(uint32 id)
{
    for(uint32 i = 1; i < NPC_MAX; i++)
    {
        uint32 find = id << 16 || i;
        GH_Add::iterator itr = GH_AddHouse.find(find);
        if(itr == GH_AddHouse.end()) 
            continue;
        GH_Item::iterator itr2 = (*itr).second.begin();
        for(; itr2 != (*itr).second.end(); itr2++)
        {
            if((*itr2).guid)
            {
                if((*itr2).type == CREATURE)
                {
                    objmgr.DeleteCreatureData((*itr2).guid);
                    (*itr2).guid = 0;
                }
                else
                {
                    objmgr.DeleteGOData((*itr2).guid);
                    (*itr2).guid = 0;                                       
                }
            }
            else
                break;
        }           
    }
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
        uint32 x   = fields[2].GetFloat();
        uint32 y   = fields[3].GetFloat();
        uint32 z   = fields[4].GetFloat();
        uint16 map = fields[5].GetUInt16();
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

void LoadGuildHouseAdd()
{
    GH_AddHouse.clear();
    QueryResult_AutoPtr result = WorldDatabase.Query("SELECT * FROM guildhouses_add ORDER BY Id ASC");

    if (!result)
    {
        barGoLink bar(1);

        bar.step();

        sLog.outString("");
        sLog.outErrorDb(">> Loaded 0 guild house add.");
        return;
    }

    barGoLink bar(result->GetRowCount());

    do
    {
        Field *fields = result->Fetch();
        bar.step();

        uint16 id           = fields[0].GetUInt16();
        uint16 add_type     = fields[1].GetUInt16();
        uint32 id_template  = fields[2].GetUInt32();
        uint8 type          = fields[3].GetUInt8();
        uint32 x            = fields[4].GetFloat();
        uint32 y            = fields[5].GetFloat();
        uint32 z            = fields[6].GetFloat();
        uint32 o            = fields[7].GetFloat();
        uint16 map          = fields[5].GetUInt16();

        GH_ItemTemp NewItemTemp(id_template, (GH_ItemTemplate_Type)type, x, y, z, o, map);
        uint32 find = 0;
        find = ( (uint32)id << 16 ) || (uint32)add_type;
        GH_AddHouse[find].push_back(NewItemTemp);        
    } while (result->NextRow());

    sLog.outString();
    sLog.outString( ">> Loaded %u GuildHouseAdd", GH_AddHouse.size() );
}

GH_ItemTemp::GH_ItemTemp(uint32 new_id_template, GH_ItemTemplate_Type newtype, float X, float Y, float Z, float O, uint16 map)
{
    guid = 0;    
    id_template = new_id_template;
    type = newtype;
    m_X = X;
    m_Y = Y; 
    m_Z = Z;
    m_orient = O;
    m_map = map;
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

GuildHouse::GuildHouse(uint32 guildID, uint32 id, uint32 x, uint32 y, uint32 z, uint16 map, uint32 add)
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

void GuildHouse::SetGuildHouse(uint32 guildID, uint32 id, uint32 x, uint32 y, uint32 z, uint16 map)
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
    QueryResult_AutoPtr result = WorldDatabase.PQuery("SELECT `x`, `y`, `z`, `map` FROM `guildhouses` WHERE `id` = %u", newid);
    if(result)
    {
        Id = newid;
        Field *fields = result->Fetch();
        m_X = fields[0].GetFloat();
        m_Y = fields[1].GetFloat();
        m_Z = fields[2].GetFloat();
        m_map = fields[3].GetUInt16();        
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
