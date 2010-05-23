/*
 * Copyright (C) 2008 - 2009 Trinity <http://www.trinitycore.org/>
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

#ifndef DEF_ULDUAR_H
#define DEF_ULDUAR_H

enum Encounter
{
    BOSS_LEVIATHAN,
    BOSS_IGNIS,
    BOSS_RAZORSCALE,
    BOSS_XT002,
    BOSS_ASSEMBLY,
    BOSS_KOLOGARN,
    BOSS_AURIAYA,
    BOSS_MIMIRON,
    BOSS_HODIR,
    BOSS_THORIM,
    BOSS_FREYA,
    BOSS_VEZAX,
    BOSS_YOGGSARON,
    BOSS_ALGALON,
    MAX_BOSS_NUMBER
};

enum Data64
{
    DATA_LEVIATHAN,
    DATA_NORGANNON,
    DATA_IGNIS,
    DATA_RAZORSCALE,
    DATA_EXP_COMMANDER,
    DATA_XT002,
    DATA_STEELBREAKER,
    DATA_MOLGEIM,
    DATA_BRUNDIR,
    DATA_KOLOGARN,
    DATA_RIGHT_ARM,
    DATA_LEFT_ARM,
    DATA_AURIAYA,
    DATA_BRIGHTLEAF,
    DATA_IRONBRANCH,
    DATA_STONEBARK,
    DATA_FREYA,
    DATA_THORIM
};

enum Data
{
    DATA_LEVIATHAN_DOOR,
    DATA_THORIM_GATE,
};

enum Npcs
{
    NPC_COMMANDER               = 33210,
    NPC_GTRAPPER                = 33259,
    NPC_ENGINEER                = 33287,
    NPC_DEFENDER                = 33816,
    NPC_IRON_CONSTRUCT          = 33121,
    NPC_FIRE_STATE              = 33184
};

#endif
