#ifndef OUTDOOR_PVP_WG_
#define OUTDOOR_PVP_WG_

#include "OutdoorPvPImpl.h"
#include "BattleGroundMgr.h"

#define POS_X_CENTER                5100
#define MAX_VEHICLE_PER_WORKSHOP    4
#define WG_MAP                      571

const uint32 WintergraspFaction[3] = {1732, 1735, 35};
const uint32 WG_MARK_OF_HONOR = 43589;
const uint32 VehNumWorldState[2] = {3680,3490};
const uint32 MaxVehNumWorldState[2] = {3681,3491};
const uint32 ClockWorldState[2] = {3781,4354};

/* Not used / Not implemented

const uint16 GameEventWintergraspDefender[2] = {50, 51};

enum OutdoorPvP_WG_Sounds
{
    OutdoorPvP_WG_SOUND_KEEP_CLAIMED            = 8192,
    OutdoorPvP_WG_SOUND_KEEP_CAPTURED_ALLIANCE  = 8173,
    OutdoorPvP_WG_SOUND_KEEP_CAPTURED_HORDE     = 8213,
    OutdoorPvP_WG_SOUND_KEEP_ASSAULTED_ALLIANCE = 8212,
    OutdoorPvP_WG_SOUND_KEEP_ASSAULTED_HORDE    = 8174,
    OutdoorPvP_WG_SOUND_NEAR_VICTORY            = 8456
};

enum DataId
{
    DATA_ENGINEER_DIE,
};

enum OutdoorPvP_WG_KeepStatus
{
    OutdoorPvP_WG_KEEP_TYPE_NEUTRAL             = 0,
    OutdoorPvP_WG_KEEP_TYPE_CONTESTED           = 1,
    OutdoorPvP_WG_KEEP_STATUS_ALLY_CONTESTED    = 1,
    OutdoorPvP_WG_KEEP_STATUS_HORDE_CONTESTED   = 2,
    OutdoorPvP_WG_KEEP_TYPE_OCCUPIED            = 3,
    OutdoorPvP_WG_KEEP_STATUS_ALLY_OCCUPIED     = 3,
    OutdoorPvP_WG_KEEP_STATUS_HORDE_OCCUPIED    = 4
};
*/

enum WG_STRING_LOCALE_INDEX
{
    WG_STRING_TOWER_FLAMEWATCH  = 0,
    WG_STRING_TOWER_SHADOWSIGHT,
    WG_STRING_TOWER_WINTERS_EDGE,
    WG_STRING_WORKSHOP_E,
    WG_STRING_WORKSHOP_W,
    WG_STRING_WORKSHOP_NW,
    WG_STRING_WORKSHOP_NE,
    WG_STRING_WORKSHOP_SW,
    WG_STRING_WORKSHOP_SE,
    WG_STRING_KEEP_TOWER_NW,
    WG_STRING_KEEP_TOWER_NE,
    WG_STRING_KEEP_TOWER_SW,
    WG_STRING_KEEP_TOWER_SE,
    WG_STRING_ENG_GOSSIP_1,
    WG_STRING_ENG_GOSSIP_2,
    WG_STRING_ENG_GOSSIP_3,
    WG_STRING_ENG_GOSSIP_4,
    WG_STRING_DEFAULT,
    MAX_WG_STRINGS
};

const std::string WG_STRING_LOCALE[MAX_WG_STRINGS][MAX_LOCALE] =
{
    {"Flamewatch", "", "", "Flammenaugenturm", "", "", "", "", ""},
    {"Shadowsight", "", "", "Schattenblickturm", "", "", "", "", ""},
    {"Winter's Edge", "", "", "Wintersturzturm", "", "", "", "", ""},

    {"Sunken Ring", "", "", "des versunkenen Rings", "", "", "", "", ""},
    {"Broken Temple", "", "", "des zerbrochene Tempels", "", "", "", "", ""},
    {"north-western", "", "", "der Feste (W)", "", "", "", "", ""},
    {"north-eastern", "", "", "der Feste (O)", "", "", "", "", ""},
    {"Westpark", "", "", "Westfunk", "", "", "", "", ""},
    {"Eastpark", "", "", "Ostfunk", "", "", "", "", ""},

    {"north-western", "", "", "Tausendwinterfestung (NW) Turm", "", "", "", "", ""},
    {"north-eastern", "", "", "Tausendwinterfestung (NO) Turm", "", "", "", "", ""},
    {"south-western", "", "", "Tausendwinterfestung (SW) Turm", "", "", "", "", ""},
    {"south-eastern", "", "", "Tausendwinterfestung (SO) Turm", "", "", "", "", ""},

    {"Build catapult.", "", "", "Baue ein Katapult.", "", "", "", "", ""},
    {"Build demolisher.", "", "", "Baue einen Verwüster.", "", "", "", "", ""},
    {"Build siege engine.", "", "", "Baue eine Belagerungsmaschine.", "", "", "", "", ""},
    {"I can't build more!", "", "", "Ich kann nicht mehr bauen!", "", "", "", "", ""},

    {"UNKNOWN", "", "", "UNBEKANNT", "", "", "", "", ""}
};

enum WG_QUESTGIVER_POS_INDEX
{   // Order is important! See WG_QUESTGIVER_POS_MAP!
    WG_ENHANCEMENT_A_POS = 0,
    WG_ENHANCEMENT_H_POS,
    WG_QUESTGIVER_1_A_POS,
    WG_QUESTGIVER_1_H_POS,
    WG_QUESTGIVER_2_A_POS,
    WG_QUESTGIVER_2_H_POS,
    WG_QUESTGIVER_3_A_POS,
    WG_QUESTGIVER_3_H_POS,
    WG_QUESTGIVER_4_A_POS,
    WG_QUESTGIVER_4_H_POS,
    WG_QUESTGIVER_5_A_POS,
    WG_QUESTGIVER_5_H_POS,
    WG_QUESTGIVER_6_A_POS,
    WG_QUESTGIVER_6_H_POS,
    // TODO: Don't know pos for these atm...
    WG_QUESTGIVER_PVP_1_A_POS,
    WG_QUESTGIVER_PVP_1_H_POS,
    WG_QUESTGIVER_PVP_2_A_POS,
    WG_QUESTGIVER_PVP_2_H_POS,
    MAX_WG_QUESTGIVER
};

const float WG_QUESTGIVER_POS_MAP[MAX_WG_QUESTGIVER][4] =
{   // Order is important! See WG_QUESTGIVER_POS_INDEX!
    {5016.57f, 3677.53f, 362.982f, 5.7525f},
    {5081.7f, 2173.73f, 365.878f, 0.855211f},
    {5030.44f, 3659.82f, 363.194f, 1.83336f},
    {5100.07f, 2168.89f, 365.779f, 1.97222f},
    {5008.64f, 3659.91f, 361.07f, 4.0796f},
    {5080.4f, 2199.0f, 359.489f, 2.96706f},
    {5032.33f, 3680.7f, 363.018f, 3.43167f},
    {5088.49f, 2188.18f, 365.647f, 5.25344f},
    {5032.66f, 3674.28f, 363.053f, 2.9447f},
    {5095.67f, 2193.28f, 365.924f, 4.93928f},
    {5032.44f, 3668.66f, 363.11f, 2.87402f},
    {5088.61f, 2167.66f, 365.689f, 0.680678f},
    {5022.43f, 3659.91f, 361.61f, 1.35426f},
    {5078.28f, 2183.7f, 365.029f, 1.46608f},
    // TODO: Don't know pos for these atm...
    {0.0f, 0.0f, 0.0f, 0.0f},
    {0.0f, 0.0f, 0.0f, 0.0f},
    {0.0f, 0.0f, 0.0f, 0.0f},
    {0.0f, 0.0f, 0.0f, 0.0f}
};

// Index and Type defines. Order is important! See WG_NPC_Entry_Pair!
enum WG_NPC_TYPE_AND_INDEX
{
    WG_CREATURE_GUARD = 0,
    WG_CREATURE_CHAMPION,

    WG_CREATURE_QUARTERMASTER_1,
    WG_CREATURE_QUARTERMASTER_2,

    WG_CREATURE_SPIRIT_GUIDE,
    WG_CREATURE_SPIRIT_HEALER,

    WG_CREATURE_ENGINEER,

    WG_CREATURE_ENHANCEMENT,
    WG_CREATURE_QUESTGIVER_1,
    WG_CREATURE_QUESTGIVER_2,
    WG_CREATURE_QUESTGIVER_3,
    WG_CREATURE_QUESTGIVER_4,
    WG_CREATURE_QUESTGIVER_5,
    WG_CREATURE_QUESTGIVER_6,
    WG_CREATURE_QUESTGIVER_PVP_1,
    WG_CREATURE_QUESTGIVER_PVP_2,

    WG_CREATURE_SIEGE_VEHICLE,
    WG_CREATURE_SIEGE_TURRET,
    WG_CREATURE_CATAPULT,
    WG_CREATURE_DEMOLISHER,
    WG_CREATURE_TOWER_CANNON,

    WG_CREATURE_FLIGHTMASTER,

    WG_CREATURE_WEAPON_CONTROL,

    WG_CREATURE_OTHER
};

enum WG_NPC_ENTRIES
{
    WG_CREATURE_GUARD_A             = 32308,
    WG_CREATURE_GUARD_H             = 32307,
    WG_CREATURE_CHAMPION_A          = 30740,
    WG_CREATURE_CHAMPION_H          = 30739,

    WG_CREATURE_QUARTERMASTER_1_A   = 32294,
    WG_CREATURE_QUARTERMASTER_1_H   = 32296,
    WG_CREATURE_QUARTERMASTER_2_A   = 39172,
    WG_CREATURE_QUARTERMASTER_2_H   = 39173,

    WG_CREATURE_SPIRIT_GUIDE_A      = 31842,
    WG_CREATURE_SPIRIT_GUIDE_H      = 31841,
    WG_CREATURE_SPIRIT_HEALER_A     = 6491,
    WG_CREATURE_SPIRIT_HEALER_H     = 6491,

    WG_CREATURE_ENGINEER_A          = 30499,
    WG_CREATURE_ENGINEER_H          = 30400,
    WG_CREATURE_ENHANCEMENT_A       = 31051,
    WG_CREATURE_ENHANCEMENT_H       = 31101,

    WG_CREATURE_QUESTGIVER_1_A      = 31052,
    WG_CREATURE_QUESTGIVER_1_H      = 31102,
    WG_CREATURE_QUESTGIVER_2_A      = 31109,
    WG_CREATURE_QUESTGIVER_2_H      = 31107,
    WG_CREATURE_QUESTGIVER_3_A      = 31153,
    WG_CREATURE_QUESTGIVER_3_H      = 31151,
    WG_CREATURE_QUESTGIVER_4_A      = 31108,
    WG_CREATURE_QUESTGIVER_4_H      = 31106,
    WG_CREATURE_QUESTGIVER_5_A      = 31054,
    WG_CREATURE_QUESTGIVER_5_H      = 31053,
    WG_CREATURE_QUESTGIVER_6_A      = 31036,
    WG_CREATURE_QUESTGIVER_6_H      = 31091,

    WG_CREATURE_QUESTGIVER_PVP_1_A  = 32626,
    WG_CREATURE_QUESTGIVER_PVP_1_H  = 32615,
    WG_CREATURE_QUESTGIVER_PVP_2_A  = 15351,
    WG_CREATURE_QUESTGIVER_PVP_2_H  = 15350,

    WG_CREATURE_SIEGE_VEHICLE_A     = 28312,
    WG_CREATURE_SIEGE_VEHICLE_H     = 32627,
    WG_CREATURE_SIEGE_TURRET_A      = 28319,
    WG_CREATURE_SIEGE_TURRET_H      = 32629,
    WG_CREATURE_CATAPULT_A          = 27881,
    WG_CREATURE_CATAPULT_H          = 27881,
    WG_CREATURE_DEMOLISHER_A        = 28094,
    WG_CREATURE_DEMOLISHER_H        = 28094,
    WG_CREATURE_TOWER_CANNON_A      = 28366,
    WG_CREATURE_TOWER_CANNON_H      = 28366,

    WG_CREATURE_FLIGHTMASTER_A      = 30869,
    WG_CREATURE_FLIGHTMASTER_H      = 30870,

    WG_CREATURE_WEAPON_CONTROL_A    = 27852,
    WG_CREATURE_WEAPON_CONTROL_H    = 27852
};

typedef uint32 TeamPairs[2];

const TeamPairs WG_NPC_Entry_Pair[] =
{   // Order is important! See WG_NPC_TYPE_AND_INDEX!
    {WG_CREATURE_GUARD_A,           WG_CREATURE_GUARD_H},
    {WG_CREATURE_CHAMPION_A,        WG_CREATURE_CHAMPION_H},

    {WG_CREATURE_QUARTERMASTER_1_A, WG_CREATURE_QUARTERMASTER_1_H},
    {WG_CREATURE_QUARTERMASTER_2_A, WG_CREATURE_QUARTERMASTER_2_H},

    {WG_CREATURE_SPIRIT_GUIDE_A,    WG_CREATURE_SPIRIT_GUIDE_H},
    {WG_CREATURE_SPIRIT_HEALER_A,   WG_CREATURE_SPIRIT_HEALER_H},

    {WG_CREATURE_ENGINEER_A,        WG_CREATURE_ENGINEER_H},
    {WG_CREATURE_ENHANCEMENT_A,     WG_CREATURE_ENHANCEMENT_H},

    {WG_CREATURE_QUESTGIVER_1_A,    WG_CREATURE_QUESTGIVER_1_H},
    {WG_CREATURE_QUESTGIVER_2_A,    WG_CREATURE_QUESTGIVER_2_H},
    {WG_CREATURE_QUESTGIVER_3_A,    WG_CREATURE_QUESTGIVER_3_H},
    {WG_CREATURE_QUESTGIVER_4_A,    WG_CREATURE_QUESTGIVER_4_H},
    {WG_CREATURE_QUESTGIVER_5_A,    WG_CREATURE_QUESTGIVER_5_H},
    {WG_CREATURE_QUESTGIVER_6_A,    WG_CREATURE_QUESTGIVER_6_H},
    {WG_CREATURE_QUESTGIVER_PVP_1_A,WG_CREATURE_QUESTGIVER_PVP_1_H},
    {WG_CREATURE_QUESTGIVER_PVP_2_A,WG_CREATURE_QUESTGIVER_PVP_2_H},

    {WG_CREATURE_SIEGE_VEHICLE_A,   WG_CREATURE_SIEGE_VEHICLE_H},
    {WG_CREATURE_SIEGE_TURRET_A,    WG_CREATURE_SIEGE_TURRET_H},
    {WG_CREATURE_CATAPULT_A,        WG_CREATURE_CATAPULT_H},
    {WG_CREATURE_DEMOLISHER_A,      WG_CREATURE_DEMOLISHER_H},
    {WG_CREATURE_TOWER_CANNON_A,    WG_CREATURE_TOWER_CANNON_H},

    {WG_CREATURE_FLIGHTMASTER_A,    WG_CREATURE_FLIGHTMASTER_H},

    {WG_CREATURE_WEAPON_CONTROL_A,  WG_CREATURE_WEAPON_CONTROL_H},

    {    0,     0}  // END
};

const TeamPairs WG_GO_Display_Pair[] =
{
    // Ally, Horde
    {5651, 5652},
    {8256, 8257},

    {   0,    0}  // END
};

enum WG_GO_DISPLAYIDS
{
    // 194162: Doodad_WG_Keep_Door01_collision01 - "Invisible Wall"
    // 194323: Wintergrasp Keep Collision Wall - "Invisible Wall"
    WG_GO_DISPLAY_COLLISION     = 8556,
    // Defender's Portal
    // Vehicle Teleporter
    WG_GO_DISPLAY_TELEPORTER    = 8244,
    // Titan relic
    WG_GO_DISPLAY_RELICT        = 7967,
    // Wintergrasp Keep Door
    WG_GO_DISPLAY_KEEP_DOOR     = 8165,
    // Wintergrasp Fortress Wall
    WG_GO_DISPLAY_FORTRESS_WALL = 7877,
    // Wintergrasp Keep Tower
    WG_GO_DISPLAY_KEEP_TOWER    = 7878,
    // Wintergrasp Fortress Gate
    WG_GO_DISPLAY_FORTRESS_GATE = 7906,
    // Wintergrasp Wall
    WG_GO_DISPLAY_WALL          = 7909,
    // Flamewatch Tower
    // Shadowsight Tower
    // Winter's Edge Tower
    WG_GO_DISPLAY_TOWER         = 7900,
    // Goblin Workshop
    WG_GO_DISPLAY_WORKSHOP      = 8208,
    // Banner
    WG_GO_DISPLAY_BANNER_1_A    = 5651,
    WG_GO_DISPLAY_BANNER_1_H    = 5652,
    WG_GO_DISPLAY_BANNER_2_A    = 8256,
    WG_GO_DISPLAY_BANNER_2_H    = 8257
};

enum WG_GO_ENTRIES
{
    // Titan Reilc (click to win)
    WG_GO_TITAN_RELIC           = 192829,

    // Fortress Gate (destroy to reach the relict)
    WG_GO_FORTRESS_GATE         = 190375,

    // Doodad_WG_Keep_Door01_collision01 - "Invisible Wall"
    WG_GO_KEEP_DOOR01_COLLISION = 194162,
    // Wintergrasp Keep Collision Wall - "Invisible Wall"
    WG_GO_KEEP_COLLISION_WALL   = 194323,

    // Workshop NW
    WG_GO_WORKSHOP_NW           = 192028,
    // Workshop W
    WG_GO_WORKSHOP_W            = 192030,
    // Workshop SW
    WG_GO_WORKSHOP_SW           = 192032,
    // Workshop NE
    WG_GO_WORKSHOP_NE           = 192029,
    // Workshop E
    WG_GO_WORKSHOP_E            = 192031,
    // Workshop SE
    WG_GO_WORKSHOP_SE           = 192033,

    // Workshop Banner NO
    WG_GO_WORKSHOP_BANNER_NO    = 190475,
    // Workshop Banner NW
    WG_GO_WORKSHOP_BANNER_NW    = 190487,

    // Shadowsight Tower
    WG_GO_TOWER_SHADOWSIGHT     = 190356,
    // Winter's Edge Tower
    WG_GO_TOWER_WINTERS_EDGE    = 190357,
    // Flamewatch Tower
    WG_GO_TOWER_FLAMEWATCH      = 190358,

    // Keep Tower NW
    WG_GO_KEEP_TOWER_NW         = 190221,
    // Keep Tower NE
    WG_GO_KEEP_TOWER_NE         = 190378,
    // Keep Tower SW
    WG_GO_KEEP_TOWER_SW         = 190373,
    // Keep Tower SE
    WG_GO_KEEP_TOWER_SE         = 190377,

    // Wall 1
    WG_GO_WALL_1                = 191797,
    // Wall 2
    WG_GO_WALL_2                = 191798,
    // Wall 3
    WG_GO_WALL_3                = 191805,

    // Fortress Wall 1
    WG_GO_FORTRESS_WALL_1       = 191799,
    // Fortress Wall 2
    WG_GO_FORTRESS_WALL_2       = 191809,

    // Fortress Door
    WG_GO_FORTRESS_DOOR         = 191810,

    // Defender's Portal 1
    WG_GO_DEFENDER_PORTAL_1     = 190763,
    // Defender's Portal 2
    WG_GO_DEFENDER_PORTAL_2     = 191575,
    // Defender's Portal 3
    WG_GO_DEFENDER_PORTAL_3     = 192819,

    // Vehicle Teleporter - GAMEOBJECT_TYPE_GENERIC - Faction 114 and no dataX entries - must be scripted!
    WG_GO_VEHICLE_TELEPORTER    = 192951,

    WG_GO_BANNER_A              = 192487,
    WG_GO_BANNER_H              = 192488
};

enum WG_GO_EVENTIDS
{   // SELECT `entry`,`data9`,`data14` FROM `gameobject_template` WHERE `entry` IN
    // (190375,192028,192030,192032,192029,192031,192033,90356,190357,190358,190221,190378,190373,190377,191797,191798,191805,191799,191809,191810,190356);
    WG_GO_FORTRESS_GATE_DAMAGED         = 19956,
    WG_GO_FORTRESS_GATE_DESTROYED       = 19957,

    WG_GO_FORTRESS_DOOR_DAMAGED         = 19448,
    WG_GO_FORTRESS_DOOR_DESTROYED       = 19607,

    WG_GO_KEEP_TOWER_NW_DAMAGED         = 19657,
    WG_GO_KEEP_TOWER_NW_DESTROYED       = 19661,

    WG_GO_KEEP_TOWER_SW_DAMAGED         = 19659,
    WG_GO_KEEP_TOWER_SW_DESTROYED       = 19662,

    WG_GO_KEEP_TOWER_SE_DAMAGED         = 19660,
    WG_GO_KEEP_TOWER_SE_DESTROYED       = 19664,

    WG_GO_KEEP_TOWER_NE_DAMAGED         = 19658,
    WG_GO_KEEP_TOWER_NE_DESTROYED       = 19663,

    WG_GO_TOWER_WINTERS_EDGE_DAMAGED    = 19673,
    WG_GO_TOWER_WINTERS_EDGE_DESTROYED  = 19676,

    WG_GO_TOWER_FLAMEWATCH_DAMAGED      = 19672,
    WG_GO_TOWER_FLAMEWATCH_DESTROYED    = 19675,

    WG_GO_TOWER_SHADOWSIGHT_DAMAGED     = 19674,
    WG_GO_TOWER_SHADOWSIGHT_DESTROYED   = 19677,

    WG_GO_WALL_1_DAMAGED                = 19934,
    WG_GO_WALL_1_DESTROYED              = 19943,

    WG_GO_WALL_2_DAMAGED                = 19940,
    WG_GO_WALL_2_DESTROYED              = 19949,

    WG_GO_WALL_3_DAMAGED                = 19937,
    WG_GO_WALL_3_DESTROYED              = 19946,

    WG_GO_FORTRESS_WALL_1_DAMAGED       = 19909,
    WG_GO_FORTRESS_WALL_1_DESTROYED     = 19923,

    WG_GO_FORTRESS_WALL_2_DAMAGED       = 19908,
    WG_GO_FORTRESS_WALL_2_DESTROYED     = 19922,

    WG_GO_WORKSHOP_NW_DAMAGED           = 19782,
    WG_GO_WORKSHOP_NW_DESTROYED         = 19786,

    WG_GO_WORKSHOP_NE_DAMAGED           = 19783,
    WG_GO_WORKSHOP_NE_DESTROYED         = 19787,

    WG_GO_WORKSHOP_W_DAMAGED            = 19777,
    WG_GO_WORKSHOP_W_DESTROYED          = 19779,

    WG_GO_WORKSHOP_E_DAMAGED            = 19776,
    WG_GO_WORKSHOP_E_DESTROYED          = 19778,

    WG_GO_WORKSHOP_SW_DAMAGED           = 19784,
    WG_GO_WORKSHOP_SW_DESTROYED         = 19788,

    WG_GO_WORKSHOP_SE_DAMAGED           = 19785,
    WG_GO_WORKSHOP_SE_DESTROYED         = 19789
};

enum OutdoorPvPWGSpell
{
    // Wartime auras
    SPELL_RECRUIT               = 37795,
    SPELL_CORPORAL              = 33280,
    SPELL_LIEUTENANT            = 55629,
    SPELL_TENACITY              = 58549,
    SPELL_TENACITY_VEHICLE      = 59911,
    SPELL_TOWER_CONTROL         = 62064,
    SPELL_SPIRITUAL_IMMUNITY    = 58729,

    // Reward spells
    SPELL_VICTORY_REWARD        = 56902,
    SPELL_DEFEAT_REWARD         = 58494,
    SPELL_DAMAGED_TOWER         = 59135,
    SPELL_DESTROYED_TOWER       = 59136,
    SPELL_DAMAGED_BUILDING      = 59201,
    SPELL_INTACT_BUILDING       = 59203,

    SPELL_BUILD_CATAPULT        = 56663,
    SPELL_BUILD_DEMOLISHER      = 56575,
    SPELL_BUILD_SIEGE_ENGINE_A  = 56661,
    SPELL_BUILD_SIEGE_ENGINE_H  = 61408,

    SPELL_TELEPORT_DALARAN      = 53360,
    SPELL_VICTORY_AURA          = 60044
};

enum OutdoorPvPWGCreType
{
    CREATURE_OTHER,
    CREATURE_SIEGE_VEHICLE,
    CREATURE_TURRET,
    CREATURE_ENGINEER,
    CREATURE_GUARD,
    CREATURE_SPECIAL,
    CREATURE_SPIRIT_GUIDE,
    CREATURE_SPIRIT_HEALER,
    CREATURE_QUESTGIVER,
};

enum OutdoorPvPWGBuildingType
{
    BUILDING_WALL,
    BUILDING_WORKSHOP,
    BUILDING_TOWER,
};

enum OutdoorPvPWGDamageState
{ // Do not change order
    DAMAGE_INTACT,
    DAMAGE_DAMAGED,
    DAMAGE_DESTROYED,
};

typedef uint32 TeamPair[2];

enum OutdoorPvPWGQuest
{
    A_VICTORY_IN_WG = 13181,
    H_VICTORY_IN_WG = 13183,
    CRE_PVP_KILL    = 31086, //Quest Objective - Fixme: this should be handled by DB
    CRE_PVP_KILL_V  = 31093, //Quest Objective - Fixme: this should be handled by DB
};

enum OutdoorPvPWGCreEntry
{
    CRE_ENG_A   = 30499,
    CRE_ENG_H   = 30400,
    CRE_SPI_A   = 31842,
    CRE_SPI_H   = 31841,
};

const TeamPair OutdoorPvPWGCreEntryPair[] =
{
    {32307, 32308}, // Guards
    {30739, 30740}, // Champions
    {32296, 32294}, // Quartermaster
    {39173, 39172}, // Quartermaster
    {32615, 32626}, // Warbringer & Brigadier General
    {0,0} // Do not delete Used in LoadTeamPair
};

const TeamPair OutdoorPvPWGGODisplayPair[] =
{
    {5651, 5652},
    {8256, 8257},
    {0,0} // Do not delete Used in LoadTeamPair
};

const uint32 AreaPOIIconId[3][3] = {{7,8,9},{4,5,6},{1,2,3}};
typedef std::list<const AreaPOIEntry *> AreaPOIList;

struct BuildingState
{
    explicit BuildingState(uint32 _worldState, TeamId _team, bool asDefault) : worldState(_worldState), health(0), defaultTeam(asDefault ? _team : OTHER_TEAM(_team)), team(_team),
        damageState(DAMAGE_INTACT), building(NULL), type(BUILDING_WALL), graveTeam(NULL) {}

    uint32 worldState;
    uint32 health;
    TeamId defaultTeam;
    OutdoorPvPWGDamageState damageState;
    GameObject *building;
    uint32 *graveTeam;
    OutdoorPvPWGBuildingType type;

    void SendUpdate(Player *player) const
    {
        player->SendUpdateWorldState(worldState, AreaPOIIconId[team][damageState]);
    }

    void FillData(WorldPacket &data) const
    {
        data << worldState << AreaPOIIconId[team][damageState];
    }

    TeamId GetTeam() const { return team; }

    void SetTeam(TeamId t)
    {
        team = t;
        if(graveTeam)
            if (uint32 newTeam = TeamId2Team[t])
                *graveTeam = newTeam;
    }

private:
    TeamId team;
};

typedef std::map<uint32, uint32> TeamPairMap;

class OPvPCapturePointWG;

class OutdoorPvPWG : public OutdoorPvP
{
    protected:
        typedef std::map<uint32, BuildingState *> BuildingStateMap;
        typedef std::set<Creature*> CreatureSet;
        typedef std::set<GameObject*> GameObjectSet;
        typedef std::map<std::pair<uint32, bool>, Position> QuestGiverPositionMap;
        typedef std::map<uint32, Creature*> QuestGiverMap;
    public:
        OutdoorPvPWG();
        bool SetupOutdoorPvP();

        uint32 GetCreatureEntry(uint32 guidlow, const CreatureData *data);

        std::string GetLocaleString(WG_STRING_LOCALE_INDEX idx, LocaleConstant loc);

        void OnCreatureCreate(Creature *creature, bool add);
        void OnGameObjectCreate(GameObject *go, bool add);

        void ProcessEvent(GameObject *obj, uint32 eventId);

        void HandlePlayerEnterZone(Player *plr, uint32 zone);
        void HandlePlayerLeaveZone(Player *plr, uint32 zone);
        void HandlePlayerResurrects(Player *plr, uint32 zone);
        void HandleKill(Player *killer, Unit *victim);
        void HandleEssenceOfWintergrasp(Player *plr, uint32 zoneId);

        bool Update(uint32 diff);

        void BroadcastStateChange(BuildingState *state) const;

        uint32 GetData(uint32 id);
        void SetData(uint32 id, uint32 value) {};

        void ModifyWorkshopCount(TeamId team, bool add);
        uint32 GetTimer() const { return m_timer / 1000; };
        bool isWarTime() const { return m_wartime; };
        void setTimer(uint32 timer) { if (timer >= 0) m_timer = timer; };
        uint32 GetNumPlayersA() const { return m_players[TEAM_ALLIANCE].size(); };
        uint32 GetNumPlayersH() const { return m_players[TEAM_HORDE].size(); };
        TeamId getDefenderTeamId() const { return m_defender; };
        TeamId getAttackerTeamId() const { return OTHER_TEAM(m_defender); };

        void forceChangeTeam();
        void forceStopBattle();
        void forceStartBattle();

        // Temporal BG specific till 3.2
        void SendAreaSpiritHealerQueryOpcode(Player *pl, const uint64& guid);
        void AddPlayerToResurrectQueue(uint64 npc_guid, uint64 player_guid);
        void RemovePlayerFromResurrectQueue(uint64 player_guid);
        void RelocateDeadPlayers(Creature *cr);
        // BG end

        void ResetCreatureEntry(Creature *cr, uint32 entry);

    protected:
        // Temporal BG specific till 3.2
        std::vector<uint64> m_ResurrectQueue;               // Player GUID
        uint32 m_LastResurrectTime;
        // Spirit Guide guid + Player list GUIDS
        std::map<uint64, std::vector<uint64> >  m_ReviveQueue;

        uint32 GetLastResurrectTime() const { return m_LastResurrectTime; }
        uint32 GetReviveQueueSize() const { return m_ReviveQueue.size(); }
        // BG end

        TeamId m_defender;

        BuildingStateMap m_buildingStates;
        BuildingState *m_gate;

        GameObject *m_gate_collision1;
        GameObject *m_gate_collision2;

        CreatureSet m_creatures;
        CreatureSet m_vehicles[2];
        CreatureSet m_turrets[2];

        GameObjectSet m_gobjects;
        QuestGiverMap m_questgivers;

        TeamPairMap m_creEntryPair, m_goDisplayPair;
        QuestGiverPositionMap m_qgPosMap;

        bool m_wartime;
        bool m_changeDefender;
        uint64 m_timer;
        uint64 m_clock[2];
        uint32 m_workshopCount[2];
        uint32 m_towerDestroyedCount[2];
        uint32 m_towerDamagedCount[2];
        uint32 m_VehicleCnt[2];
        uint32 m_saveinterval; // Minimum save interval if nothing happends - 300000 - 5 Min.
        int32 m_tenacityStack;

        OPvPCapturePointWG *GetWorkshop(uint32 lowguid) const;
        OPvPCapturePointWG *GetWorkshopByEngGuid(uint32 lowguid) const;
        OPvPCapturePointWG *GetWorkshopByGOGuid(uint32 lowguid) const;

        void StartBattle();
        void EndBattle();

        void UpdateClock();
        void UpdateClockDigit(uint32 &timer, uint32 digit, uint32 mod);
        void PromotePlayer(Player *player) const;
        void UpdateTenacityStack();
        void UpdateAllWorldObject();
        bool UpdateCreatureInfo(Creature *creature);
        bool UpdateGameObjectInfo(GameObject *go) const;

        bool CanBuildVehicle(OPvPCapturePointWG *workshop) const;
        OutdoorPvPWGCreType GetCreatureType(uint32 entry) const;

        void RebuildAllBuildings();

        void SendInitWorldStatesTo(Player *player = NULL) const;
        void RewardMarkOfHonor(Player *player, uint32 count);
        void MoveQuestGiver(uint32 guid);
        void LoadQuestGiverMap(uint32 guid, Position posHorde, Position posAlli);
        bool UpdateQuestGiverPosition(uint32 guid, Creature *creature);

    private:
        void LoadTeamPair(TeamPairMap &pairMap, const TeamPair *pair);
        void SaveData();
};

class OPvPCapturePointWG : public OPvPCapturePoint
{
    public:
        explicit OPvPCapturePointWG(OutdoorPvPWG *opvp, BuildingState *state);
        void SetTeamByBuildingState();
        void ChangeState() {}
        void ChangeTeam(TeamId oldteam);

        uint32 *m_spiEntry;
        uint32 m_spiGuid;
        Creature *m_spiritguide;

        uint32 *m_engEntry;
        uint32 m_engGuid;
        Creature *m_engineer;
        uint32 m_workshopGuid;
        BuildingState *m_buildingState;
    protected:
        OutdoorPvPWG *m_wintergrasp;
};

#endif
