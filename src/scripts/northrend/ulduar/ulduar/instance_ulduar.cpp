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

#include "ScriptedPch.h"
#include "ulduar.h"

const DoorData doorData[] =
{
    {194416,    BOSS_LEVIATHAN, DOOR_TYPE_ROOM,     0},
    {194905,    BOSS_LEVIATHAN, DOOR_TYPE_PASSAGE,  0},
    {194631,    BOSS_XT002,     DOOR_TYPE_ROOM,     0},
    {194553,    BOSS_ASSEMBLY,  DOOR_TYPE_PASSAGE,  0},
    {194554,    BOSS_ASSEMBLY,  DOOR_TYPE_ROOM,     0},
    {194556,    BOSS_ASSEMBLY,  DOOR_TYPE_PASSAGE,  0},
    {194553,    BOSS_KOLOGARN,  DOOR_TYPE_ROOM,     0},
    {194441,    BOSS_HODIR,     DOOR_TYPE_PASSAGE,  0},
    {194634,    BOSS_HODIR,     DOOR_TYPE_PASSAGE,  0},
    {194442,    BOSS_HODIR,     DOOR_TYPE_ROOM,     0},
    {0,         0,              DOOR_TYPE_ROOM,     0}, // EOF
};

enum eGameObjects
{
    GO_Leviathan_DOOR       = 194630,
    GO_Kologarn_CHEST_HERO  = 195047,
    GO_Kologarn_CHEST       = 195046,
    GO_Kologarn_BRIDGE      = 194232,
    GO_Hodir_CHEST_HERO     = 194308,
    GO_Hodir_CHEST          = 194307,
    GO_Thorim_GATE          = 194560,
};

struct instance_ulduar : public InstanceData
{
    instance_ulduar(Map* pMap) : InstanceData(pMap)
    {
        SetBossNumber(MAX_BOSS_NUMBER);
        LoadDoorData(doorData);
    }

    uint64 uiLeviathan;
    uint64 uiNorgannon;
    uint64 uiIgnis;
    uint64 uiRazorscale;
    uint64 uiExpCommander;
    uint64 uiXT002;
    uint64 uiSteelbreaker;
    uint64 uiMolgeim;
    uint64 uiBrundir;
    uint64 uiKologarn;
    uint64 uiRightArm;
    uint64 uiLeftArm;
    uint64 uiAuriaya;
    uint64 uiBrightleaf;
    uint64 uiIronbranch;
    uint64 uiStonebark;
    uint64 uiFreya;
    uint64 uiThorim;
    uint64 uiKologarnBridge;
    
    GameObject* pLeviathanDoor, *KologarnChest, *pThorimGate, *HodirChest;

    void OnGameObjectCreate(GameObject* pGo, bool add)
    {
        AddDoor(pGo, add);
        switch(pGo->GetEntry())
        {
            case GO_Leviathan_DOOR: pLeviathanDoor = add ? pGo : NULL; break;
            case GO_Kologarn_CHEST_HERO: KologarnChest = add ? pGo : NULL; break;
            case GO_Kologarn_CHEST: KologarnChest = add ? pGo : NULL; break;
            case GO_Kologarn_BRIDGE: uiKologarnBridge = pGo->GetGUID(); HandleGameObject(NULL, true, pGo); break;
            case GO_Hodir_CHEST_HERO: HodirChest = add ? pGo : NULL; break;
            case GO_Hodir_CHEST: HodirChest = add ? pGo : NULL; break;
            case GO_Thorim_GATE: pThorimGate = add ? pGo : NULL; break;
        }
    }

    void OnCreatureCreate(Creature* pCreature, bool add)
    {
        switch(pCreature->GetEntry())
        {
            case 33113: uiLeviathan = pCreature->GetGUID(); return;
            case 33686: uiNorgannon = pCreature->GetGUID(); return;
            case 33118: uiIgnis = pCreature->GetGUID(); return;
            case 33186: uiRazorscale = pCreature->GetGUID(); return;
            case 33210: uiExpCommander = pCreature->GetGUID(); return;
            case 33293: uiXT002 = pCreature->GetGUID(); return;
            case 32867: uiSteelbreaker = pCreature->GetGUID(); return;
            case 32927: uiMolgeim = pCreature->GetGUID(); return;
            case 32857: uiBrundir = pCreature->GetGUID(); return;
            case 32930: uiKologarn = pCreature->GetGUID(); return;
            case 32934: uiRightArm = pCreature->GetGUID(); return;
            case 32933: uiLeftArm = pCreature->GetGUID(); return;
            case 33515: uiAuriaya = pCreature->GetGUID(); return;
            case 32915: uiBrightleaf = pCreature->GetGUID(); return;
            case 32913: uiIronbranch = pCreature->GetGUID(); return;
            case 32914: uiStonebark = pCreature->GetGUID(); return;
            case 32906: uiFreya = pCreature->GetGUID(); return;
            case 32865: uiThorim = pCreature->GetGUID(); return;
        }

        AddMinion(pCreature, add);
    }

    uint64 GetData64(uint32 id)
    {
        switch(id)
        {
        case DATA_LEVIATHAN:
            return uiLeviathan;
        case DATA_NORGANNON:
            return uiNorgannon;
        case DATA_IGNIS:
            return uiIgnis;
        case DATA_RAZORSCALE:
            return uiRazorscale;
        case DATA_EXP_COMMANDER:
            return uiExpCommander;
        case DATA_XT002:
            return uiXT002;
        case DATA_STEELBREAKER:
            return uiSteelbreaker;
        case DATA_MOLGEIM:
            return uiMolgeim;
        case DATA_BRUNDIR:
            return uiBrundir;
        case DATA_KOLOGARN:
            return uiKologarn;
        case DATA_RIGHT_ARM:
            return uiRightArm;
        case DATA_LEFT_ARM:
            return uiLeftArm;
        case DATA_AURIAYA:
            return uiAuriaya;
        case DATA_BRIGHTLEAF:
            return uiBrightleaf;
        case DATA_IRONBRANCH:
            return uiIronbranch;
        case DATA_STONEBARK:
            return uiStonebark;
        case DATA_FREYA:
            return uiFreya;
        case DATA_THORIM:
            return uiThorim;
        }
        return 0;
    }
    
    void SetData(uint32 id, uint32 value)
    {
        switch(id)
        {
            case DATA_LEVIATHAN_DOOR:
                if (pLeviathanDoor)
                    pLeviathanDoor->SetGoState(GOState(value));
                break;
            case DATA_THORIM_GATE:
                if (pThorimGate)
                    pThorimGate->SetGoState(GOState(value));
                break;
        }
    }

    bool SetBossState(uint32 id, EncounterState state)
    {
        if (!InstanceData::SetBossState(id, state))
            return false;
            
        if (id == BOSS_KOLOGARN && state == DONE)
        {
            HandleGameObject(uiKologarnBridge, false);
            KologarnChest->SetRespawnTime(KologarnChest->GetRespawnDelay());
        }
        
        if (id == BOSS_HODIR && state == DONE)
        {
            HodirChest->SetRespawnTime(HodirChest->GetRespawnDelay());
        }

        return true;
    }
};

InstanceData* GetInstanceData_instance_ulduar(Map* pMap)
{
    return new instance_ulduar(pMap);
}

void AddSC_instance_ulduar()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "instance_ulduar";
    newscript->GetInstanceData = &GetInstanceData_instance_ulduar;
    newscript->RegisterSelf();
}
