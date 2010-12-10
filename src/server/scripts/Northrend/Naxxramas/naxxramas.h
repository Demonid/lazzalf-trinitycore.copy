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

#ifndef DEF_NAXXRAMAS_H
#define DEF_NAXXRAMAS_H

enum Encounter
{
    BOSS_ANUBREKHAN,
    BOSS_FAERLINA,
    BOSS_MAEXXNA,
    BOSS_NOTH,
    BOSS_HEIGAN,
    BOSS_LOATHEB,
    BOSS_PATCHWERK,
    BOSS_GROBBULUS,
    BOSS_GLUTH,
    BOSS_THADDIUS,
    BOSS_RAZUVIOUS,
    BOSS_GOTHIK,
    BOSS_HORSEMEN,
    BOSS_SAPPHIRON,
    BOSS_KELTHUZAD,
    MAX_BOSS_NUMBER
};

enum Data
{
    DATA_HEIGAN_ERUPT,
    DATA_GOTHIK_GATE,
    DATA_SAPPHIRON_BIRTH,

    DATA_HORSEMEN0,
    DATA_HORSEMEN1,
    DATA_HORSEMEN2,
    DATA_HORSEMEN3,

    DATA_IMMORTAL,
};

enum Data64
{
    DATA_FAERLINA,
    DATA_THANE,
    DATA_LADY,
    DATA_BARON,
    DATA_SIR,
    DATA_THADDIUS,
    DATA_FEUGEN,
    DATA_STALAGG,
    DATA_KELTHUZAD,
    DATA_KELTHUZAD_2,
    DATA_KELTHUZAD_PORTAL01,
    DATA_KELTHUZAD_PORTAL02,
    DATA_KELTHUZAD_PORTAL03,
    DATA_KELTHUZAD_PORTAL04,
    DATA_KELTHUZAD_TRIGGER,
};

const Position TeleporterPositions[5] =
{
    // Four Horsemen
    {2563.346f, -2995.048f, 241.417f, 2.427f},
    // Loatheb
    {2919.880f, -3954.188f, 273.582f, 4.780f},
    // Maexxna
    {3462.289f, -3850.636f, 307.369f, 5.566f},
    // Thaddius
    {3441.054f, -2998.450f, 297.159f, 3.956f},
    // Kel'Thuzad
    {3658.311f, -5087.248f, 143.624f, 4.489f},
};

#define CREATURE_TELEPORTER     60200

#define GO_BIRTH                181356

#endif

