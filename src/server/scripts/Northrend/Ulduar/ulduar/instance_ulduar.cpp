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

#include "ScriptPCH.h"
#include "ulduar.h"

const DoorData doorData[] =
{
    {194416,    BOSS_LEVIATHAN, DOOR_TYPE_ROOM,     0},
    {194905,    BOSS_LEVIATHAN, DOOR_TYPE_PASSAGE,  0},
    {194631,    BOSS_XT002,     DOOR_TYPE_ROOM,     0},
    {194554,    BOSS_ASSEMBLY,  DOOR_TYPE_ROOM,     0},
    {194556,    BOSS_ASSEMBLY,  DOOR_TYPE_PASSAGE,  0},
    {194553,    BOSS_KOLOGARN,  DOOR_TYPE_ROOM,     0},
    {194441,    BOSS_HODIR,     DOOR_TYPE_PASSAGE,  0},
    {194634,    BOSS_HODIR,     DOOR_TYPE_PASSAGE,  0},
    {194442,    BOSS_HODIR,     DOOR_TYPE_ROOM,     0},
    {194559,    BOSS_THORIM,    DOOR_TYPE_ROOM,     0},
    {194774,    BOSS_MIMIRON,   DOOR_TYPE_ROOM,     0},
    {194775,    BOSS_MIMIRON,   DOOR_TYPE_ROOM,     0},
    {194776,    BOSS_MIMIRON,   DOOR_TYPE_ROOM,     0},
    {194750,    BOSS_VEZAX,     DOOR_TYPE_PASSAGE,  0},
    {194773,    BOSS_YOGGSARON, DOOR_TYPE_ROOM,     0},
    {0,         0,              DOOR_TYPE_ROOM,     0}, // EOF
};

enum eGameObjects
{
    GO_Leviathan_DOOR        = 194630,
    GO_Kologarn_CHEST_HERO   = 195047,
    GO_Kologarn_CHEST        = 195046,
    GO_Kologarn_BRIDGE       = 194232,
    GO_Hodir_CHEST_HERO      = 194308,
    GO_Hodir_CHEST           = 194307,
    GO_Hodir_Rare_CHEST      = 194200,
    GO_Hodir_Rare_CHEST_HERO = 194201,
    GO_Thorim_CHEST_HERO     = 194314,
    GO_Thorim_CHEST          = 194312,
    GO_Thorim_Rare_CHEST     = 194313,
    GO_Thorim_Rare_CHEST_HERO= 194315,
    GO_Runic_DOOR            = 194557,
    GO_Stone_DOOR            = 194558,
    GO_Thorim_LEVER          = 194265,
    GO_Mimiron_TRAM          = 194675,
    GO_Mimiron_ELEVATOR      = 194749,
    GO_Keepers_DOOR          = 194255
};

#define ACHIEVEMENT_DWARFAGEDDON_10         3097
#define ACHIEVEMENT_DWARFAGEDDON_25         3098
#define DWARFAGEDDON_MAX_TIMER              10 * 1000 // 10s
#define DWARFAGEDDON_MIN_COUNT              100
#define ACHIEVEMENT_LUMBERJACKED_10         2979
#define ACHIEVEMENT_LUMBERJACKED_25         3118
#define LUMBERJACKED_MAX_TIMER              15 * 1000 // 15s
#define ELDERS_NUMBER                       3
#define CONSPEEDATORY_MAX_TIMER             20 * MINUTE * IN_MILLISECONDS // 20 min
#define ACHI_COMING_OUT_OF_THE_WALLS_10     3014
#define ACHI_COMING_OUT_OF_THE_WALLS_25     3017
#define COMING_OUT_MAX_TIMER                12 * 1000 // 12s
#define COMING_OUT_MIN_COUNT                9

class instance_ulduar : public InstanceMapScript
{
    public:
    instance_ulduar() : InstanceMapScript("instance_ulduar", 603) { }

    InstanceScript* GetInstanceScript(InstanceMap* pMap) const
    {
        return new instance_ulduar_InstanceMapScript(pMap);
    }
    
    struct instance_ulduar_InstanceMapScript : public InstanceScript
    {
        instance_ulduar_InstanceMapScript (Map* pMap) : InstanceScript(pMap)
        {
            SetBossNumber(MAX_BOSS_NUMBER);
            LoadDoorData(doorData);
            vehicleRepaired = false;
            steelforgedDefendersCount = 0;
            dwarfageddonTimer = 0;
            achievementDwarfageddon = 0;
            eldersCount = 0;
            lumberjackedTimer = 0;
            achievementLumberjacked = 0;
            conspeedatoryTimer = 0;
            guardiansCount = 0;
            comingOutTimer = 0;
            achievementComingOut = 0;
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
        uint64 uiKologarnBridge;
        uint64 uiAuriaya;
        uint64 uiBrightleaf;
        uint64 uiIronbranch;
        uint64 uiStonebark;
        uint64 uiFreya;
        uint64 uiThorim;
        uint64 uiRunicColossus;
        uint64 uiRuneGiant;
        uint64 uiMimiron;
        uint64 uiLeviathanMKII;
        uint64 uiVX001;
        uint64 uiAerialUnit;
        uint64 uiMagneticCore;
        uint64 KeepersGateGUID;
        uint64 uiVezax;
        uint64 uiFreyaImage;
        uint64 uiThorimImage;
        uint64 uiMimironImage;
        uint64 uiHodirImage;
        uint64 uiFreyaYS;
        uint64 uiThorimYS;
        uint64 uiMimironYS;
        uint64 uiHodirYS;
        uint64 uiYoggSaronBrain;
        uint64 uiYoggSaron;
        
        // Achievements
        // Unbroken
        bool vehicleRepaired;
        // Dwarfageddon
        uint32 steelforgedDefendersCount;
        uint32 dwarfageddonTimer;
        uint32 achievementDwarfageddon;
        // Lumberjacked
        uint8 eldersCount;
        uint32 lumberjackedTimer;
        uint32 achievementLumberjacked;
        // Con-speed-atory
        uint32 conspeedatoryTimer;
        // They're Coming Out of the Walls
        uint32 guardiansCount;
        uint32 comingOutTimer;
        uint32 achievementComingOut;
            
        GameObject* pLeviathanDoor, /* *KologarnChest,*/ *HodirChest, *HodirRareChest, *ThorimChest, *ThorimRareChest, *pRunicDoor, *pStoneDoor, *pThorimLever,
            *MimironTram, *MimironElevator;

        void OnGameObjectCreate(GameObject* pGo)
        {
            AddDoor(pGo, true);

            switch(pGo->GetEntry())
            {
                case GO_Leviathan_DOOR: pLeviathanDoor = pGo; break;
                //case GO_Kologarn_CHEST_HERO: KologarnChest = add ? pGo : NULL; break;
                //case GO_Kologarn_CHEST: KologarnChest = add ? pGo : NULL; break;
                case GO_Kologarn_BRIDGE: uiKologarnBridge = pGo->GetGUID(); HandleGameObject(NULL, true, pGo); break;
                case GO_Hodir_CHEST_HERO: HodirChest = pGo; break;
                case GO_Hodir_CHEST: HodirChest = pGo; break;                
                case GO_Hodir_Rare_CHEST_HERO: HodirRareChest = pGo; break;
                case GO_Hodir_Rare_CHEST: HodirRareChest = pGo; break;
                case GO_Thorim_CHEST_HERO: ThorimChest = pGo; break;
                case GO_Thorim_CHEST: ThorimChest = pGo; break;
                case GO_Thorim_Rare_CHEST_HERO: ThorimRareChest = pGo; break;                    
                case GO_Thorim_Rare_CHEST: ThorimRareChest = pGo; break;
                case GO_Runic_DOOR: pRunicDoor = pGo; break;
                case GO_Stone_DOOR: pStoneDoor = pGo; break;
                case GO_Thorim_LEVER: pThorimLever = pGo; break;
                case GO_Mimiron_TRAM: MimironTram = pGo; break;
                case GO_Mimiron_ELEVATOR: MimironElevator = pGo; break;
                case GO_Keepers_DOOR: KeepersGateGUID = pGo->GetGUID();
                {
                    InstanceScript *data = pGo->GetInstanceScript();
                    pGo->RemoveFlag(GAMEOBJECT_FLAGS,GO_FLAG_LOCKED);
                    if (data)
                        for (uint32 i = BOSS_MIMIRON; i < BOSS_VEZAX; ++i)
                            if (data->GetBossState(i) != DONE)
                                pGo->SetFlag(GAMEOBJECT_FLAGS,GO_FLAG_LOCKED);
                    break;
                }
            }
        }

        void OnGameObjectRemove(GameObject* pGo)
        {
            AddDoor(pGo, false);

            switch(pGo->GetEntry())
            {
                case GO_Leviathan_DOOR: pLeviathanDoor = NULL; break;
                //case GO_Kologarn_CHEST_HERO: KologarnChest = add ? pGo : NULL; break;
                //case GO_Kologarn_CHEST: KologarnChest = add ? pGo : NULL; break;
                case GO_Hodir_CHEST_HERO: HodirChest = NULL; break;
                case GO_Hodir_CHEST: HodirChest = NULL; break;                
                case GO_Hodir_Rare_CHEST_HERO: HodirRareChest = NULL; break;
                case GO_Hodir_Rare_CHEST: HodirRareChest = NULL; break;
                case GO_Thorim_CHEST_HERO: ThorimChest = NULL; break;
                case GO_Thorim_CHEST: ThorimChest = NULL; break;
                case GO_Thorim_Rare_CHEST_HERO: ThorimRareChest = NULL; break;                    
                case GO_Thorim_Rare_CHEST: ThorimRareChest = NULL; break;
                case GO_Runic_DOOR: pRunicDoor = NULL; break;
                case GO_Stone_DOOR: pStoneDoor = NULL; break;
                case GO_Thorim_LEVER: pThorimLever = NULL; break;
                case GO_Mimiron_TRAM: MimironTram = NULL; break;
                case GO_Mimiron_ELEVATOR: MimironElevator = NULL; break;
            }
        }

        void OnCreatureCreate(Creature* pCreature)
        {
            Map::PlayerList const &players = instance->GetPlayers();
            uint32 TeamInInstance = 0;

            if (!players.isEmpty())
                if (Player* pPlayer = players.begin()->getSource())
                    TeamInInstance = pPlayer->GetTeam();
            
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
                case 32872: uiRunicColossus = pCreature->GetGUID(); return;
                case 32873: uiRuneGiant = pCreature->GetGUID(); return;
                case 33350: uiMimiron = pCreature->GetGUID(); return;
                case 33432: uiLeviathanMKII = pCreature->GetGUID(); return;
                case 33651: uiVX001 = pCreature->GetGUID(); return;
                case 33670: uiAerialUnit = pCreature->GetGUID(); return;
                case 34068: uiMagneticCore = pCreature->GetGUID(); return;
                case 33271: uiVezax = pCreature->GetGUID(); return;
                case 33410: uiFreyaYS = pCreature->GetGUID(); return;
                case 33413: uiThorimYS = pCreature->GetGUID(); return;
                case 33412: uiMimironYS = pCreature->GetGUID(); return;
                case 33411: uiHodirYS = pCreature->GetGUID(); return;
                case 33890: uiYoggSaronBrain = pCreature->GetGUID(); return;
                case 33288: uiYoggSaron = pCreature->GetGUID(); return;
                
                // Keeper's Images
                case 33241: uiFreyaImage = pCreature->GetGUID();
                {
                    InstanceScript *data = pCreature->GetInstanceScript();
                    pCreature->SetVisible(false);
                    if (data && data->GetBossState(BOSS_VEZAX) == DONE)
                        pCreature->SetVisible(true);
                }
                return;
                case 33242: uiThorimImage = pCreature->GetGUID();
                {
                    InstanceScript *data = pCreature->GetInstanceScript();
                    pCreature->SetVisible(false);
                    if (data && data->GetBossState(BOSS_VEZAX) == DONE)
                        pCreature->SetVisible(true);
                }
                return;
                case 33244: uiMimironImage = pCreature->GetGUID();
                {
                    InstanceScript *data = pCreature->GetInstanceScript();
                    pCreature->SetVisible(false);
                    if (data && data->GetBossState(BOSS_VEZAX) == DONE)
                        pCreature->SetVisible(true);
                }            
                return;
                case 33213: uiHodirImage = pCreature->GetGUID();
                {
                    InstanceScript *data = pCreature->GetInstanceScript();
                    pCreature->SetVisible(false);
                    if (data && data->GetBossState(BOSS_VEZAX) == DONE)
                        pCreature->SetVisible(true);
                }
                return;
            }

            // Hodir: Alliance npcs are spawned by default
            if (TeamInInstance == HORDE)
                switch(pCreature->GetEntry())
                {
                    case 33325: pCreature->UpdateEntry(32941, HORDE); return;
                    case 32901: pCreature->UpdateEntry(33333, HORDE); return;
                    case 33328: pCreature->UpdateEntry(33332, HORDE); return;
                    case 32900: pCreature->UpdateEntry(32950, HORDE); return;
                    case 32893: pCreature->UpdateEntry(33331, HORDE); return;
                    case 33327: pCreature->UpdateEntry(32946, HORDE); return;
                    case 32897: pCreature->UpdateEntry(32948, HORDE); return;
                    case 33326: pCreature->UpdateEntry(33330, HORDE); return;
                    case 32908: pCreature->UpdateEntry(32907, HORDE); return;
                    case 32885: pCreature->UpdateEntry(32883, HORDE); return;
                }
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
                case DATA_RUNIC_COLOSSUS:
                    return uiRunicColossus;
                case DATA_RUNE_GIANT:
                    return uiRuneGiant;
                case DATA_MIMIRON:
                    return uiMimiron;
                case DATA_LEVIATHAN_MK_II:
                    return uiLeviathanMKII;
                case DATA_VX_001:
                    return uiVX001;
                case DATA_AERIAL_UNIT:
                    return uiAerialUnit;
                case DATA_MAGNETIC_CORE:
                    return uiMagneticCore;
                case DATA_VEZAX:
                    return uiVezax;
                case DATA_YS_FREYA:
                    return uiFreyaYS;
                case DATA_YS_THORIM:
                    return uiThorimYS;
                case DATA_YS_MIMIRON:
                    return uiMimironYS;
                case DATA_YS_HODIR:
                    return uiHodirYS;
                case DATA_YOGGSARON_BRAIN:
                    return uiYoggSaronBrain;
                case DATA_YOGGSARON:
                    return uiYoggSaron;
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
                case DATA_RUNIC_DOOR:
                    if (pRunicDoor)
                        pRunicDoor->SetGoState(GOState(value));
                    break;
                case DATA_STONE_DOOR:
                    if (pStoneDoor)
                        pStoneDoor->SetGoState(GOState(value));
                    break;
                case DATA_CALL_TRAM:
                    if (MimironTram && instance)
                    {
                        // Load Mimiron Tram (unfortunally only server side)
                        instance->LoadGrid(2307, 284.632f);
                    
                        if (value == 0)
                            MimironTram->SetGoState(GO_STATE_READY);
                        if (value == 1)
                            MimironTram->SetGoState(GO_STATE_ACTIVE);
                        
                        // Send movement update to players
                        if (Map* pMap = MimironTram->GetMap())
                            if (pMap->IsDungeon())
                            {
                                Map::PlayerList const &PlayerList = pMap->GetPlayers();

                                if (!PlayerList.isEmpty())
                                    for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
                                        if (i->getSource())
                                        {
                                            UpdateData data;
                                            WorldPacket pkt;
                                            MimironTram->BuildValuesUpdateBlockForPlayer(&data, i->getSource());
                                            data.BuildPacket(&pkt);
                                            i->getSource()->GetSession()->SendPacket(&pkt);
                                        }
                            }
                    }
                    break;
                case DATA_MIMIRON_ELEVATOR:
                    if (MimironElevator)
                        MimironElevator->SetGoState(GOState(value));
                    break; 
                case DATA_HODIR_CHEST:
                    if (HodirChest && value == GO_STATE_READY)
                        HodirChest->SetRespawnTime(HodirChest->GetRespawnDelay());
                    break;
                case DATA_HODIR_RARE_CHEST:
                    if (HodirRareChest && value == GO_STATE_READY)
                        HodirRareChest->SetRespawnTime(HodirRareChest->GetRespawnDelay());
                    break;
                case DATA_THORIM_CHEST:
                    if (ThorimChest && value == GO_STATE_READY)
                        ThorimChest->SetRespawnTime(ThorimChest->GetRespawnDelay());
                    break;
                case DATA_THORIM_RARE_CHEST:
                    if (ThorimRareChest && value == GO_STATE_READY)
                        ThorimRareChest->SetRespawnTime(ThorimRareChest->GetRespawnDelay());
                    break;
                // Achievement
                case DATA_ACHI_UNBROKEN:
                    if (value == ACHI_FAILED)
                        vehicleRepaired = true;
                    break;
                case DATA_DWARFAGEDDON_START:
                    if (value == ACHI_START)
                        dwarfageddonTimer = DWARFAGEDDON_MAX_TIMER;
                    else if (value == ACHI_RESET)
                    {
                        dwarfageddonTimer = 0;
                        steelforgedDefendersCount = 0;
                    }
                    break;
                case DATA_DWARFAGEDDON_COUNT:
                    if (value == ACHI_INCREASE)
                        steelforgedDefendersCount++;
                    break;
                case DATA_LUMBERJACKED_START:
                    if (value == ACHI_START)
                        lumberjackedTimer = LUMBERJACKED_MAX_TIMER;
                    else if (value == ACHI_FAILED || value == ACHI_COMPLETED)
                        lumberjackedTimer = 0;
                    break;
                case DATA_LUMBERJACKED_COUNT:
                    if (value == ACHI_INCREASE)
                        eldersCount++;
                case DATA_CONSPEEDATORY:
                    if (value == ACHI_START)
                        conspeedatoryTimer = CONSPEEDATORY_MAX_TIMER;
                    else if (value == ACHI_FAILED || value == ACHI_COMPLETED)
                        conspeedatoryTimer = 0;
                    break;
                case DATA_COMING_OUT_START:
                    if (value == ACHI_START)
                        comingOutTimer = COMING_OUT_MAX_TIMER;
                    else if (value == ACHI_RESET)
                    {
                        comingOutTimer = 0;
                        guardiansCount = 0;
                    }
                    break;
                case DATA_COMING_OUT_COUNT:
                    if (value == ACHI_INCREASE)
                        guardiansCount++;
                    break;
            }
        }

        uint32 GetData(uint32 id)
        {
            switch (id)
            {
                case DATA_ACHI_UNBROKEN:
                    if (vehicleRepaired == true)
                        return ACHI_FAILED;
                    else
                        return ACHI_IS_IN_PROGRESS;
                case DATA_DWARFAGEDDON_START:
                    if (dwarfageddonTimer > 0)
                        return ACHI_IS_IN_PROGRESS;
                    else
                        return ACHI_IS_NOT_STARTED;
                case DATA_LUMBERJACKED_START:
                    if (lumberjackedTimer > 0)
                        return ACHI_IS_IN_PROGRESS;
                    else
                        return ACHI_IS_NOT_STARTED;
                case DATA_CONSPEEDATORY:
                    if (conspeedatoryTimer > 0)
                        return ACHI_IS_IN_PROGRESS;
                    else
                        return ACHI_IS_NOT_STARTED;
                case DATA_COMING_OUT_START:
                    if (comingOutTimer > 0)
                        return ACHI_IS_IN_PROGRESS;
                    else
                        return ACHI_IS_NOT_STARTED;
                default:
                    return 0;
            }
        }

        bool SetBossState(uint32 id, EncounterState state)
        {
            if (!InstanceScript::SetBossState(id, state))
                return false;
                
            switch (id)
            {
                case BOSS_KOLOGARN:
                    if (state == DONE)
                    {
                        HandleGameObject(uiKologarnBridge, false);
                        //KologarnChest->SetRespawnTime(KologarnChest->GetRespawnDelay());
                    }
                    break;
                case BOSS_HODIR:
                    CheckKeepersState();
                    break;
                case BOSS_THORIM:
                    if (state == IN_PROGRESS)
                        pThorimLever->RemoveFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNK1);
                    CheckKeepersState();
                    break;
                case BOSS_MIMIRON:
                    CheckKeepersState();
                    break;
                case BOSS_FREYA:
                    CheckKeepersState();
                    break;
                case BOSS_VEZAX:
                    if (state == DONE)
                    {
                        // Keeper's Images
                        if (Creature* pFreya = instance->GetCreature(uiFreyaImage))
                            pFreya->SetVisible(true);
                        if (Creature* pThorim = instance->GetCreature(uiThorimImage))
                            pThorim->SetVisible(true);
                        if (Creature* pMimiron = instance->GetCreature(uiMimironImage))
                            pMimiron->SetVisible(true);
                        if (Creature* pHodir = instance->GetCreature(uiHodirImage))
                            pHodir->SetVisible(true);
                    }
                    break;
            }
            
            return true;
        }
        
        void CheckKeepersState()
        {
            if (GameObject* pGo = instance->GetGameObject(KeepersGateGUID))
            {
                InstanceScript *data = pGo->GetInstanceScript();
                pGo->RemoveFlag(GAMEOBJECT_FLAGS,GO_FLAG_LOCKED);
                if (data)
                    for (uint32 i = BOSS_MIMIRON; i < BOSS_VEZAX; ++i)
                        if (data->GetBossState(i) != DONE)
                            pGo->SetFlag(GAMEOBJECT_FLAGS,GO_FLAG_LOCKED);
            }
        }

        void Update(uint32 diff)
        {
            // Achievement Dwarfageddon control            
            if (dwarfageddonTimer)
            {
                if (steelforgedDefendersCount >= DWARFAGEDDON_MIN_COUNT)
                {
                    if (Difficulty(instance->GetSpawnMode()) == RAID_DIFFICULTY_10MAN_NORMAL)
                        achievementDwarfageddon = ACHIEVEMENT_DWARFAGEDDON_10;
                    else if (Difficulty(instance->GetSpawnMode()) == RAID_DIFFICULTY_25MAN_NORMAL)
                        achievementDwarfageddon = ACHIEVEMENT_DWARFAGEDDON_25;

                    AchievementEntry const *AchievDwarfageddon = GetAchievementStore()->LookupEntry(achievementDwarfageddon);
                    if (AchievDwarfageddon)
                        DoCompleteAchievement(achievementDwarfageddon);

                    SetData(DATA_DWARFAGEDDON_START, ACHI_RESET);
                    return;
                }

                if (dwarfageddonTimer <= diff)
                    SetData(DATA_DWARFAGEDDON_START, ACHI_RESET);
                else dwarfageddonTimer -= diff;
            }

            // Achievement Lumberjacked control
            if (lumberjackedTimer)
            {
                if (eldersCount == ELDERS_NUMBER)
                {
                    if (Difficulty(instance->GetSpawnMode()) == RAID_DIFFICULTY_10MAN_NORMAL)
                        achievementLumberjacked = ACHIEVEMENT_LUMBERJACKED_10;
                    else if (Difficulty(instance->GetSpawnMode()) == RAID_DIFFICULTY_25MAN_NORMAL)
                        achievementLumberjacked = ACHIEVEMENT_LUMBERJACKED_25;

                    AchievementEntry const *AchievLumberjacked = GetAchievementStore()->LookupEntry(achievementLumberjacked);
                    if (AchievLumberjacked)
                        DoCompleteAchievement(achievementLumberjacked);

                    SetData(DATA_LUMBERJACKED_START, ACHI_COMPLETED);
                    return;
                }

                if (lumberjackedTimer <= diff)
                    SetData(DATA_LUMBERJACKED_START, ACHI_FAILED);
                else lumberjackedTimer -= diff;
            }

            // Achievement Con-speed-atory Timer
            if (conspeedatoryTimer)
            {
                if (conspeedatoryTimer <= diff)
                    SetData(DATA_CONSPEEDATORY, ACHI_FAILED);
                else conspeedatoryTimer -= diff;
            }

            // Achievement They're Coming Out of the Walls control
            if (comingOutTimer)
            {
                if (guardiansCount >= COMING_OUT_MIN_COUNT)
                {
                    if (Difficulty(instance->GetSpawnMode()) == RAID_DIFFICULTY_10MAN_NORMAL)
                        achievementComingOut = ACHI_COMING_OUT_OF_THE_WALLS_10;
                    if (Difficulty(instance->GetSpawnMode()) == RAID_DIFFICULTY_25MAN_NORMAL)
                        achievementComingOut = ACHI_COMING_OUT_OF_THE_WALLS_25;

                    AchievementEntry const *AchievComingOut = GetAchievementStore()->LookupEntry(achievementComingOut);
                    if (AchievComingOut)
                        DoCompleteAchievement(achievementComingOut);

                    SetData(DATA_COMING_OUT_START, ACHI_RESET);
                    return;
                }

                if (comingOutTimer <= diff)
                    SetData(DATA_COMING_OUT_START, ACHI_RESET);
                else comingOutTimer -= diff;
            }
        }
    };
};

// Mimiron Tram
class go_call_tram : public GameObjectScript
{
    public:
    go_call_tram() : GameObjectScript("go_call_tram") {}
    
    bool OnGossipHello(Player* pPlayer, GameObject* pGo)
    {
        InstanceScript* pInstance = pGo->GetInstanceScript();

        if (!pInstance)
            return false;

        switch(pGo->GetEntry())
        {
            case 194914:
            case 194438:
                pInstance->SetData(DATA_CALL_TRAM, 0);
                break;
            case 194912:
            case 194437:
                pInstance->SetData(DATA_CALL_TRAM, 1);
                break;
        }
        return true;
    };
};

// Archivum Console
/*enum ConsoleActions
{
    VALANYR,
    FREYA,
    HODIR,
    MIMIRON,
    THORIM,
};

enum ConsoleQuests
{
    QUEST_ANCIENT_HISTORY = 13622,
    QUEST_CELESTIAL_PLANETARIUM_10 = 13607,
    QUEST_CELESTIAL_PLANETARIUM_25 = 13816,
};

class go_archivum_console : public GameObjectScript
{
    public:
        go_archivum_console() : GameObjectScript("go_archivum_console") { }

    bool OnGossipHello(Player *pPlayer, GameObject *pGO)
    {
        InstanceScript* pInstance = pGO->GetInstanceScript();

        if (!pInstance)
            return false;

        if (pPlayer->GetQuestStatus(QUEST_ANCIENT_HISTORY) == QUEST_STATUS_COMPLETE)
            pPlayer->ADD_GOSSIP_ITEM(0, "Tell me about the Fragments", GOSSIP_SENDER_MAIN, VALANYR);

        if (pPlayer->GetQuestStatus(QUEST_CELESTIAL_PLANETARIUM_10) == QUEST_STATUS_COMPLETE || 
            pPlayer->GetQuestStatus(QUEST_CELESTIAL_PLANETARIUM_25) == QUEST_STATUS_COMPLETE)
        {
            pPlayer->ADD_GOSSIP_ITEM(0, "Tell me about Freya and her sigil", GOSSIP_SENDER_MAIN, FREYA);
            pPlayer->ADD_GOSSIP_ITEM(0, "Tell me about Hodir and his sigil", GOSSIP_SENDER_MAIN, HODIR);
            pPlayer->ADD_GOSSIP_ITEM(0, "Tell me about Mimiron and his sigil", GOSSIP_SENDER_MAIN, MIMIRON);
            pPlayer->ADD_GOSSIP_ITEM(0, "Tell me about Thorim and his sigil", GOSSIP_SENDER_MAIN, THORIM);
        }

        pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, pGO->GetGUID());

        return true;
    };

    bool OnGossipSelect(Player *pPlayer, GameObject *pGO, uint32 sender, uint32 action)
    {
        if(sender != GOSSIP_SENDER_MAIN) return true;

        switch(action)
        {
            case VALANYR:
            case FREYA:
            case HODIR:
            case MIMIRON:
            case THORIM:
                pGO->MonsterSay("Entry denied. Access level insufficient.", LANG_UNIVERSAL, 0);
                pPlayer->CLOSE_GOSSIP_MENU();
                //go->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                break;
        }

        return true;
    }
        
        //void UpdateAI(const uint32 diff)
        //{
        //    if (valanyrAnalysisPhase == 1)
        //    {
        //        if (valanyrAnalysisTimer <= diff)
        //        {
        //            go->MonsterSay("Fragment analysis underway.", LANG_UNIVERSAL, 0);
        //            valanyrAnalysisTimer = 2000;
        //            valanyrAnalysisPhase = 2;
        //        }
        //        else valanyrAnalysisTimer -= diff;
        //    }
        //    else if (valanyrAnalysisPhase == 2)
        //    {
        //        if (valanyrAnalysisTimer <= diff)
        //        {
        //            go->MonsterSay("Object identified: Val'anyr, Hammer of Ancient Kings.", LANG_UNIVERSAL, 0);
        //            valanyrAnalysisTimer = 6000;
        //            valanyrAnalysisPhase = 3;
        //        }
        //        else valanyrAnalysisTimer -= diff;
        //    }
        //    else if (valanyrAnalysisPhase == 3)
        //    {
        //        if (valanyrAnalysisTimer <= diff)
        //        {
        //            go->MonsterSay("Created by the titans themselves, Val'anyr was given to the first Earthen king, Urel Stoneheart. With the hammer he was to create and give life to the rest of his brethren.", LANG_UNIVERSAL, 0);
        //            valanyrAnalysisTimer = 2500;
        //            valanyrAnalysisPhase = 4;
        //        }
        //        else valanyrAnalysisTimer -= diff;
        //    }
        //    else if (valanyrAnalysisPhase == 4)
        //    {
        //        if (valanyrAnalysisTimer <= diff)
        //        {
        //            go->MonsterSay("Val'anyr was shattered during the first war between the Earthen and the Iron Dwarves.", LANG_UNIVERSAL, 0);
        //            valanyrAnalysisTimer = 2000;
        //            valanyrAnalysisPhase = 5;
        //        }
        //        else valanyrAnalysisTimer -= diff;
        //    }
        //    else if (valanyrAnalysisPhase == 5)
        //    {
        //        if (valanyrAnalysisTimer <= diff)
        //        {
        //            go->MonsterSay("The Weapon's remnants were believed lost in the conflict.", LANG_UNIVERSAL, 0);
        //            valanyrAnalysisTimer = 2500;
        //            valanyrAnalysisPhase = 6;
        //        }
        //        else valanyrAnalysisTimer -= diff;
        //    }
        //    else if (valanyrAnalysisPhase == 6)
        //    {
        //        if (valanyrAnalysisTimer <= diff)
        //        {
        //            go->MonsterSay("Probability of successful repair by ordinary means available in this world is close to nil.", LANG_UNIVERSAL, 0);
        //            valanyrAnalysisTimer = 2000;
        //            valanyrAnalysisPhase = 7;
        //        }
        //        else valanyrAnalysisTimer -= diff;
        //    }
        //    else if (valanyrAnalysisPhase == 7)
        //    {
        //        if (valanyrAnalysisTimer <= diff)
        //        {
        //            go->MonsterSay("Please hold while theoretical means are analyzed.", LANG_UNIVERSAL, 0);
        //            valanyrAnalysisTimer = 6000;
        //            valanyrAnalysisPhase = 8;
        //        }
        //        else valanyrAnalysisTimer -= diff;
        //    }
        //    else if (valanyrAnalysisPhase == 8)
        //    {
        //        if (valanyrAnalysisTimer <= diff)
        //        {
        //            go->MonsterSay("Powerful acidic content theoretically found inside the being Yogg-Saron would count for the liquefaction of Saronite. Submersion in this substance might be sufficient to rebind an alloy of titan origin.", LANG_UNIVERSAL, 0);
        //            valanyrAnalysisTimer = 0;
        //            valanyrAnalysisPhase = 0;
        //            go->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        //        }
        //        else valanyrAnalysisTimer -= diff;
        //    }
        //}

};*/

void AddSC_instance_ulduar()
{
    new instance_ulduar();
    new go_call_tram();
    //new go_archivum_console();
}
