/*
 * Copyright (C) 2008-2010 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2006-2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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

#ifndef DEF_MOLTEN_CORE_H
#define DEF_MOLTEN_CORE_H

enum Data
{
    DATA_FLAMEWAKERPRIEST = 1,
    DATA_GARRISDEAD = 2,
    DATA_GEDDONISDEAD = 3,
    DATA_GEHENNASISDEAD = 4,
    DATA_GOLEMAGGISDEAD = 5,
    DATA_GOLEMAGG_DEATH = 6,
    DATA_LUCIFRONISDEAD = 7,
    DATA_MAGMADARISDEAD = 8,
    DATA_MAJORDOMOISDEAD = 9,
    DATA_SHAZZRAHISDEAD = 10,
    DATA_SULFURON = 11,
    DATA_SULFURONISDEAD = 12,
    DATA_GOLEMAGG = 13,
    DATA_LUCIFRON = 14,
    DATA_MAGMADAR = 15,
    DATA_GEHENNAS = 16,
    DATA_GARR = 17,
    DATA_SHAZZRAH = 18,
    DATA_GEDDON = 19,
    DATA_MAJORDOMO = 20,
    DATA_MAJORDOMO_GUID = 21,
    DATA_RAGNAROS = 22,
    DATA_RAG_ELE_COUNTER = 23
};

enum Encounter
{
    BOSS_LUCIFRON = 0,
    BOSS_SULFURON,
    BOSS_GOLEMAGG,
    BOSS_MAGMADAR,
    BOSS_GEHENNAS,
    BOSS_GARR,
    BOSS_SHAZZRAH,
    BOSS_GEDDON,
    BOSS_MAJORDOMO,
    BOSS_RAGNAROS,
    MAX_BOSS_NUMBER
}; 
#endif

