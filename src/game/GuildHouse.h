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
 * @File : GuildHouse.h
 *
 * @Authors : Lazzalf
 *
 * @Date : 31/03/2010
 *
 * @Version : 0.1
 *
 **/

#ifndef SC_GUILDHOUSE_SYSTEM
#define SC_GUILDHOUSE_SYSTEM

#include "Common.h"
#include "ProgressBar.h"
#include "Map.h"
#include "World.h"
#include "Guild.h"
#include "ObjectMgr.h"
#include "Log.h"

#include <map>

enum GuildAdd_Type
{
    NPC_GUARD        = 0x00000001,
    NPC_BUFFMAN      = 0x00000002,
    NPC_TELE         = 0x00000004,
};

enum GH_ItemTemplate_Type
{
    CREATURE        = 0,
    OBJECT          = 1,
};

class GH_ItemTemp
{
    GH_ItemTemplate_Type type;
    uint32               id_template;
    float                m_X, m_Y, m_Z, m_orient;
    uint32               m_map; 
};

class GuildHouse
{
    public:        
        GuildHouse();
        GuildHouse(uint32 guildID, uint32 id, uint32 x, uint32 y, uint32 z, uint32 map, uint32 add);
        
        uint32        GuildId;      
        uint32        Id;    
        uint32        GuildHouse_Add;
        float         m_X, m_Y, m_Z, m_orient;
        uint32        m_map; 

        void ChangeId(uint32 newid);
        void SetGuildHouse_Add(uint32 NewAdd);
        void ChangeGuildHouse_Add(uint32 NewAdd);
        void SetGuildHouse(uint32 guildID, uint32 id, uint32 x, uint32 y, uint32 z, uint32 map);
};

typedef UNORDERED_MAP<uint32, GuildHouse> GuildHouseMap;

typedef std::list<GH_ItemTemp> GH_I;
typedef UNORDERED_MAP<uint32, GH_I> GH_AI;
typedef UNORDERED_MAP<uint32, GH_AI> GH_Add;

void LoadGuildHouse();
bool CheckGuildID(uint32 guild_id);
bool CheckGuildHouse(uint32 guild, GuildHouse &GHm);
bool GetGuildHouseLocation(uint32 guild, float &x, float &y, float &z, float &o, uint32 &map);

#endif