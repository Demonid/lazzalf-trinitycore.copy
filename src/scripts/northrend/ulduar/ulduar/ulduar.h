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

enum Data
{
    DATA_STEELBREAKER,
    DATA_MOLGEIM,
    DATA_BRUNDIR,
    DATA_RUNEMASTER_MOLGEIM,
    DATA_STORMCALLER_BRUNDIR,
    DATA_NADA,
    DATA_VAI
};

enum Data64
{
    DATA_LEVIATHAN,
    DATA_LEVIATHAN_TRIGGER,
    DATA_IGNIS,
    DATA_XT002,
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

/*enum eTypes
{
    MAX_ENCOUNTER               = 14,

    TYPE_LEVIATHAN              = 0,
    TYPE_IGNIS                  = 1,
    TYPE_RAZORSCALE             = 2,
    TYPE_XT002                  = 3,
    TYPE_ASSEMBLY               = 4,
    TYPE_KOLOGARN               = 5,
    TYPE_AURIAYA                = 6,
    TYPE_MIMIRON                = 7,
    TYPE_HODIR                  = 8,
    TYPE_THORIM                 = 9,
    TYPE_FREYA                  = 10,
    TYPE_VEZAX                  = 11,
    TYPE_YOGGSARON              = 12,
    TYPE_ALGALON                = 13,

    DATA_STEELBREAKER           = 20,
    DATA_MOLGEIM                = 21,
    DATA_BRUNDIR                = 22,
    DATA_RUNEMASTER_MOLGEIM     = 23,
    DATA_STORMCALLER_BRUNDIR    = 24,

    NPC_LEVIATHAN               = 33113,
    NPC_IGNIS                   = 33118,
    NPC_RAZORSCALE              = 33186,
    NPC_XT002                   = 33293,
    NPC_STEELBREAKER            = 32867,
    NPC_MOLGEIM                 = 32927,
    NPC_BRUNDIR                 = 32857,
    NPC_KOLOGARN                = 32930,
    NPC_AURIAYA                 = 33515,
    NPC_MIMIRON                 = 33350,
    NPC_HODIR                   = 32845,
    NPC_THORIM                  = 32865,
    NPC_FREYA                   = 32906,
    NPC_VEZAX                   = 33271,
    NPC_YOGGSARON               = 33288,
    NPC_ALGALON                 = 32871,

    NPC_COMMANDER               = 33210,
    NPC_GTRAPPER                = 33259,
    NPC_ENGINEER          		= 33287,
    NPC_DEFENDER          		= 33816,
    NPC_IRON_CONSTRUCT          = 33121,
    NPC_FIRE_STATE              = 33184
};

enum Data
{
    DATA_NADA,
    DATA_VAI
};*/

#endif
