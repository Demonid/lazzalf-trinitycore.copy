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

#ifndef DEF_ARCHAVON_H
#define DEF_ARCHAVON_H

#include "OutdoorPvPMgr.h"

enum Creatures 
{
    CREATURE_ARCHAVON   = 31125,
    CREATURE_EMALON     = 33993,
    CREATURE_KORALON    = 35013,
    CREATURE_TORAVON    = 38433,
};

enum Data
{
    DATA_ARCHAVON_EVENT,
    DATA_EMALON_EVENT,
    DATA_KORALON_EVENT,
    DATA_TORAVON_EVENT,
    DATA_EWF_START,
    DATA_EWF_COUNT,
};

enum Data64
{
    DATA_ARCHAVON,
    DATA_EMALON,
    DATA_KORALON,
    DATA_TORAVON,
};

#define MAX_ENCOUNTER 4

enum AchievementControl
{
    ACHI_IS_NOT_STARTED,
    ACHI_START,
    ACHI_IS_IN_PROGRESS,
    ACHI_COMPLETED,
    ACHI_FAILED,
    ACHI_RESET,
    ACHI_INCREASE,
};

#endif