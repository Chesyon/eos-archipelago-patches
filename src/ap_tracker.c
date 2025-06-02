#include <pmdsky.h>
#include <cot.h>
#include <ap_utils.h>
#include "extern.h"

#define TR_BUFF_LEN 300

typedef struct TopScreenApTrackerWindow {
    uint8_t field_0x0; // 0x0: Maybe related to if the background is loaded?
    uint8_t padding1;
    uint8_t padding2;
    uint8_t padding3;
    uint32_t state; // 0x4: Controls top screen has an extra state? Seems redundant.
    // 0x8: varies among top screens? We will use it to store the window id.
    // Some other top screens may use it as a boolean instead for if the top
    // screen is like ready.
    signed char window_id;
    uint8_t faded;       // 0x9: Guess
    uint8_t displayable; // 0xA: Guess.
    // 0xB: In some way communicates this top screen option should begin
    // closing/ending?
    uint8_t closing;
} TopScreenApTrackerWindow;

typedef struct SomeMenuStruct {
    uint32_t something; // For submenus this is 0xD?
    void (*createMenuFunction)(void); // is null for some menus and actual init is handled in update.
    void (*closeMenuFunction)(void);
    uint32_t (*updateMenuFunction)(void);
} SomeMenuStruct;

TopScreenApTrackerWindow *apTrackerWindowPtr = NULL;
void* apTrackerTopScreenBG = NULL;
uint8_t displayedOption = 255;
signed char pickWindowId = -2;
signed char localizationWarningId = -2;
signed char dungeonTopScreenId = -2;
uint8_t dungeonModeDisplayed = 255;
uint32_t trackerRotate = 0;
uint32_t trackerVelocity = 0;
uint32_t updaterDelay = 0;
uint8_t drinksDisplayed = 0;
uint8_t drinkEventsDisplayed = 0;

uint8_t trackerLocationsDialga[] = {
    // General
    247, // Town
    254, // Guild
    180, // Dojo
    255, // Cafe
    // Early
    1,   // Beach Cave
    3,   // Drenched Bluff
    4,   // Mt Bristle
    6,   // Waterfall Cave
    7,   // Apple Woods
    8,   // Craggy Coast
    9,   // Side Path
    10,  // Mt. Horn
    11,  // Rock Path
    12,  // Foggy Forest
    13,  // Forest Path
    14,  // Steam Cave
    17,  // Amp Plains
    20,  // Northern Desert
    21,  // Quicksand Cave
    24,  // Crystal Cave
    25,  // Crystal Crossing
    27,  // Chasm Cave
    28,  // Dark Hill
    29,  // Sealed Ruin
    32,  // Dusk Forest
    33,  // Deep Dusk Forest
    34,  // Treeshroud Forest
    35,  // Brine Cave
    87,  // Serenity River
    88,  // Landslide Cave
    89,  // Lush Prairie
    90,  // Tiny Meadow
    91,  // Labyrinth Cave
    92,  // Oran Forest
    174, // Star Cave
    38,  // Hidden Land
    41,  // Temporal Tower
    // Special Episodes
    67,  // Dark Crater
    252, // Bidoof SE
    251, // Igglybuff SE
    250, // Sunflora SE
    249, // Charm SE
    248, // Future SE
    // Rule Dungeons (Long Loctions)
    99,  // Zero Isle North
    100, // Zero Isle East
    101, // Zero Isle West
    102, // Zero Isle South
    103, // Zero Isle Center
    104, // Destiny Tower
    107, // Oblivion Forest
    108, // Treacherous Waters
    109, // Southeastern Islands
    110, // Inferno Cave
};

uint8_t trackerLocationsDarkrai[] = {
    // General
    247, // Town
    254, // Guild
    180, // Dojo
    255, // Cafe
    // Early
    1,   // Beach Cave
    3,   // Drenched Bluff
    4,   // Mt Bristle
    6,   // Waterfall Cave
    7,   // Apple Woods
    8,   // Craggy Coast
    9,   // Side Path
    10,  // Mt. Horn
    11,  // Rock Path
    12,  // Foggy Forest
    13,  // Forest Path
    14,  // Steam Cave
    17,  // Amp Plains
    20,  // Northern Desert
    21,  // Quicksand Cave
    24,  // Crystal Cave
    25,  // Crystal Crossing
    27,  // Chasm Cave
    28,  // Dark Hill
    29,  // Sealed Ruin
    32,  // Dusk Forest
    33,  // Deep Dusk Forest
    34,  // Treeshroud Forest
    35,  // Brine Cave
    87,  // Serenity River
    88,  // Landslide Cave
    89,  // Lush Prairie
    90,  // Tiny Meadow
    91,  // Labyrinth Cave
    92,  // Oran Forest
    174, // Star Cave
    38,  // Hidden Land
    41,  // Temporal Tower
    // Late
    44,  // Mystifying Forest
    46,  // Blizzard Island
    47,  // Crevice Cave
    50,  // Surrounded Sea
    51,  // Miracle Sea
    54,  // Ice Aegis Cave
    62,  // Mt. Travail
    63,  // The Nightmare
    64,  // Spacial Rift
    70,  // Concealed Ruins
    72,  // Marine Resort
    73,  // Bottomless Sea (7T)
    75,  // Shimmer Desert (7T)
    77,  // Mt. Avalanche (7T)
    79,  // Giant Volcano (7T)
    81,  // World Abyss (7T)
    83,  // Sky Stairway (7T)
    85,  // Mystery Jungle (7T)
    93,  // Lake Afar
    94,  // Happy Outlook
    95,  // Mt Mistral
    96,  // Shimmer Hill
    97,  // Lost Wilderness
    98,  // Midnight Forest
    111, // 1st Station Pass
    112, // 2nd Station Pass
    113, // 3rd Station Pass
    114, // 4th Station Pass
    115, // 5th Station Pass
    116, // 6th Station Pass
    117, // 7th Station Pass
    118, // 8th Station Pass
    119, // 9th Station Pass
    120, // Sky Peak Summit Pass
    122, // Sky Peak Summit
    // Special Episodes
    67,  // Dark Crater
    252, // Bidoof SE
    251, // Igglybuff SE
    250, // Sunflora SE
    249, // Charm SE
    248, // Future SE
    // Rule Dungeons (Long Loctions)
    99,  // Zero Isle North
    100, // Zero Isle East
    101, // Zero Isle West
    102, // Zero Isle South
    103, // Zero Isle Center
    104, // Destiny Tower
    107, // Oblivion Forest
    108, // Treacherous Waters
    109, // Southeastern Islands
    110, // Inferno Cave
};

// Get which list of elements to use depending on goal.
uint8_t* GetTrackerList() {
    if (IsDarkraiGoal()) {
        return trackerLocationsDarkrai;
    } else {
        return trackerLocationsDialga;
    }
}

// Get the length of the list depending on goal.
int GetTrackerListLength() {
    int length;
    if (IsDarkraiGoal()) {
        length = sizeof(trackerLocationsDarkrai)/sizeof(trackerLocationsDarkrai[0]);
    } else {
        length = sizeof(trackerLocationsDialga)/sizeof(trackerLocationsDialga[0]);
    }
    
    // Remove long locations from the list if they are not enabled. This should only
    // remove the rule dungeons.
    if(!apSettings.longLocationsOn) {
        length -= 10;
    }
    
    return length;
}

// Check for factors that might make sense to override the tracker. For example,
// while in Special Episodes we ensure the tracker shows the checks available for
// that Special Episode.
uint8_t CheckLocationOverrides (uint8_t location) {
    switch(LoadScriptVariableValue(NULL, VAR_EXECUTE_SPECIAL_EPISODE_TYPE)) {
        case 0:;
            return 252;
        case 1:;
            return 251;
        case 2:;
            return 250;
        case 3:;
            return 249;
        case 4:;
            return 249;
        default:;
            return location;
    }
}

bool IsLocationBonusChecksComplete(enum dungeon_id location) {
    switch (location) {
        case DUNGEON_BEACH_CAVE:;
            if(!GetSubXBit(0) || !GetSubXBit(127)) { // Bag Upgrade 0, Team Name
                return false;
            }
            break;
        case DUNGEON_MT_BRISTLE:;
            if(!GetSubXBit(1)) { // Bag Upgrade 1
                return false;
            }
            break;
        case DUNGEON_APPLE_WOODS:;
            if(!GetSubXBit(2)) { // Bag Upgrade 2
                return false;
            }
            break;
        case DUNGEON_STEAM_CAVE:;
            if(!GetSubXBit(3)) { // Bag Upgrade 3
                return false;
            }
            break;
        case DUNGEON_MYSTIFYING_FOREST:; 
            if(!GetSubXBit(4)) { // Bag Upgrade 4
                return false;
            }
            break;
        case DUNGEON_CREVICE_CAVE:; 
            if(!GetSubXBit(47)) { // Secret Rank
                return false;
            }
            break;
        case DUNGEON_HIDDEN_LAND:; 
            if(!GetSubXBit(9)) { // SE5
                return false;
            }
            break;
        default:
            break;
    }
    
    if(!IsDarkraiGoal()) {
        return true;
    }
    
    switch (location) {
        case DUNGEON_STEAM_CAVE:;
            if(!GetSubXBit(25)) { // Uxie
                return false;
            }
            break;
        case DUNGEON_QUICKSAND_CAVE:;
            if(!GetSubXBit(26)) { // Mesprit
                return false;
            }
            break;
        case DUNGEON_CRYSTAL_CROSSING:;
            if(!GetSubXBit(27)) { // Azelf
                return false;
            }
            break;
        case DUNGEON_TEMPORAL_TOWER:;
            if(!GetSubXBit(28)) { // Dialga
                return false;
            }
            break;
        case DUNGEON_MIRACLE_SEA:;
            if(!GetSubXBit(29)) { // Phione
                return false;
            }
            break;
        case DUNGEON_SPACIAL_RIFT:;
            if(!GetSubXBit(30)) { // Palkia
                return false;
            }
            break;
        case DUNGEON_BOTTOMLESS_SEA:;
            if(!GetSubXBit(31) || !GetSubXBit(32)) { // Kyogre 7T
                return false;
            }
            break;
        case DUNGEON_SHIMMER_DESERT:;
            if(!GetSubXBit(33) || !GetSubXBit(34)) { // Groudon 7T
                return false;
            }
            break;
        case DUNGEON_MT_AVALANCHE:;
            if(!GetSubXBit(35) || !GetSubXBit(36)) { // Articuno 7T
                return false;
            }
            break;
        case DUNGEON_GIANT_VOLCANO:;
            if(!GetSubXBit(37) || !GetSubXBit(38)) { // Heatran 7T
                return false;
            }
            break;
        case DUNGEON_WORLD_ABYSS:;
            if(!GetSubXBit(39) || !GetSubXBit(40)) { // Giratina 7T
                return false;
            }
            break;
        case DUNGEON_SKY_STAIRWAY:;
            if(!GetSubXBit(41) || !GetSubXBit(42)) { // Rayquaza 7T
                return false;
            }
            break;
        case DUNGEON_MYSTERY_JUNGLE:;
            if(!GetSubXBit(43) || !GetSubXBit(44)) { // Mew 7T
                return false;
            }
            break;
        case DUNGEON_8TH_STATION_PASS:;
            if(!GetSubXBit(65)) { // Sneasel's Gratitude
                return false;
            }
            break;
        case DUNGEON_ICE_AEGIS_CAVE:;
            if(!GetSubXBit(69) || !GetSubXBit(70) || !GetSubXBit(71) || !GetSubXBit(72)) { // Regis
                return false;
            }
            for (int d = DUNGEON_ICE_AEGIS_CAVE; d <= DUNGEON_REGIGIGAS_CHAMBER; d++) {
                if(LoadScriptVariableValueAtIndex(NULL, VAR_DUNGEON_CONQUEST_LIST, d) != true) {
                    return false;
                }
            }
            break;
        case DUNGEON_SKY_PEAK_SUMMIT:;
            if(!GetSubXBit(46)) { // Shaymin
                return false;
            }
            break;
        default:
            break;
    }
    
    return true;
}

// Various strings for subbing in symbols in the ap tracker.
char* townSymbol = "[M:S7][S:8]";
char* lockedSymbol = "[M:S4][S:8]";
char* completeSymbol = "[M:S3][S:8]";
char* unlockedSymbol = "[M:R7][S:8]";
char* remainingChecksSymbol = "[M:R4][S:8]";
char* checkSymbol = "[M:S2][S:8]";
char* instrumentSymbol = "[M:R9][S:8]";
char* relicSymbol = "[M:T4][S:8]";
char* bagSymbol = "[M:S6]";
char* moneySymbol = "[M:S0]";

// When the player selects the tracker option from the menu, open a menu to
// allow them to alter the top screen.
char* ApTrackerEntryFn(char* buffer, int option_id) {
    struct preprocessor_args preArgs;
    uint8_t location = GetTrackerList()[option_id];
    char* locationSymbol;
    if (location == 247) {
        locationSymbol = townSymbol;
    } else if(location == 255) {
        preArgs.strings[1] = "[CS:P]Spinda's Cafe[CR]";
        preArgs.strings[0] = townSymbol;
        struct preprocessor_flags preFlagTracker = {.timer_1 = true, .flags_1 = 0x6A};
        PreprocessString(buffer, 0x400, "[string:0][CLUM_SET:26][string:1]", preFlagTracker, &preArgs);
        return buffer;
    } else if(location == 254) {
        preArgs.strings[1] = "[CS:P]Wigglytuff Guild[CR]";
        preArgs.strings[0] = townSymbol;
        struct preprocessor_flags preFlagTracker = {.timer_1 = true, .flags_1 = 0x6A};
        PreprocessString(buffer, 0x400, "[string:0][CLUM_SET:26][string:1]", preFlagTracker, &preArgs);
        return buffer;
    } else if(location == 180) {
        preArgs.strings[1] = "[CS:P]Marowak Dojo[CR]";
        preArgs.strings[0] = townSymbol;
        struct preprocessor_flags preFlagTracker = {.timer_1 = true, .flags_1 = 0x6A};
        PreprocessString(buffer, 0x400, "[string:0][CLUM_SET:26][string:1]", preFlagTracker, &preArgs);
        return buffer;
    } else if(location == 252) {
        preArgs.strings[1] = "[CS:P]Bidoof's SE[CR]";
        preArgs.strings[0] = LoadScriptVariableValueAtIndex(NULL, VAR_SPECIAL_EPISODE_CONQUEST, 0) ? completeSymbol : 
                             LoadScriptVariableValueAtIndex(NULL, VAR_SPECIAL_EPISODE_OPEN, 0) ? unlockedSymbol : lockedSymbol;
        struct preprocessor_flags preFlagTracker = {.timer_1 = true, .flags_1 = 0x6A};
        PreprocessString(buffer, 0x400, "[string:0][CLUM_SET:26][string:1]", preFlagTracker, &preArgs);
        return buffer;
    } else if(location == 251) {
        preArgs.strings[1] = "[CS:P]Igglybuff's SE[CR]";
        preArgs.strings[0] = LoadScriptVariableValueAtIndex(NULL, VAR_SPECIAL_EPISODE_CONQUEST, 1) ? completeSymbol : 
                             LoadScriptVariableValueAtIndex(NULL, VAR_SPECIAL_EPISODE_OPEN, 1) ? unlockedSymbol : lockedSymbol;
        struct preprocessor_flags preFlagTracker = {.timer_1 = true, .flags_1 = 0x6A};
        PreprocessString(buffer, 0x400, "[string:0][CLUM_SET:26][string:1]", preFlagTracker, &preArgs);
        return buffer;
    } else if(location == 250) {
        preArgs.strings[1] = "[CS:P]Sunflora's SE[CR]";
        preArgs.strings[0] = LoadScriptVariableValueAtIndex(NULL, VAR_SPECIAL_EPISODE_CONQUEST, 2) ? completeSymbol : 
                             LoadScriptVariableValueAtIndex(NULL, VAR_SPECIAL_EPISODE_OPEN, 2) ? unlockedSymbol : lockedSymbol;
        struct preprocessor_flags preFlagTracker = {.timer_1 = true, .flags_1 = 0x6A};
        PreprocessString(buffer, 0x400, "[string:0][CLUM_SET:26][string:1]", preFlagTracker, &preArgs);
        return buffer;
    } else if(location == 249) {
        preArgs.strings[1] = "[CS:P]Team Charm SE[CR]";
        preArgs.strings[0] = LoadScriptVariableValueAtIndex(NULL, VAR_SPECIAL_EPISODE_CONQUEST, 3) ? completeSymbol : 
                             LoadScriptVariableValueAtIndex(NULL, VAR_SPECIAL_EPISODE_OPEN, 3) ? unlockedSymbol : lockedSymbol;
        struct preprocessor_flags preFlagTracker = {.timer_1 = true, .flags_1 = 0x6A};
        PreprocessString(buffer, 0x400, "[string:0][CLUM_SET:26][string:1]", preFlagTracker, &preArgs);
        return buffer;
    } else if(location == 248) {
        preArgs.strings[1] = "[CS:P]Future SE[CR]";
        preArgs.strings[0] = LoadScriptVariableValueAtIndex(NULL, VAR_SPECIAL_EPISODE_CONQUEST, 4) ? completeSymbol : 
                             LoadScriptVariableValueAtIndex(NULL, VAR_SPECIAL_EPISODE_OPEN, 4) ? unlockedSymbol : lockedSymbol;
        struct preprocessor_flags preFlagTracker = {.timer_1 = true, .flags_1 = 0x6A};
        PreprocessString(buffer, 0x400, "[string:0][CLUM_SET:26][string:1]", preFlagTracker, &preArgs);
        return buffer;
    } else if(location == DUNGEON_ICE_AEGIS_CAVE) {
        preArgs.strings[1] = "[CS:P]Aegis Cave[CR]";
        enum dungeon_mode dmode = GetDungeonMode(location);
        switch(dmode) {
            default:
            case DMODE_CLOSED:;
            case DMODE_REQUEST:;
                locationSymbol = lockedSymbol;
                break;
            case DMODE_OPEN:;
                locationSymbol = remainingChecksSymbol;
                break;
            case DMODE_OPEN_AND_REQUEST:;
                if (IsLocationBonusChecksComplete(location) == false) {
                    locationSymbol = remainingChecksSymbol;
                } else {
                    locationSymbol = completeSymbol;
                }
        }
        preArgs.strings[0] = IsLocationBonusChecksComplete(location) ? completeSymbol : remainingChecksSymbol;
        struct preprocessor_flags preFlagTracker = {.timer_1 = true, .flags_1 = 0x6A};
        PreprocessString(buffer, 0x400, "[string:0][CLUM_SET:26][string:1]", preFlagTracker, &preArgs);
    } else if(location == DUNGEON_HIDDEN_LAND || location == DUNGEON_TEMPORAL_TOWER) {
        enum dungeon_mode dmode = GetDungeonMode(location);
        switch(dmode) {
            default:
            case DMODE_CLOSED:;
            case DMODE_REQUEST:;
                locationSymbol = relicSymbol;
                break;
            case DMODE_OPEN:;
                locationSymbol = remainingChecksSymbol;
                break;
            case DMODE_OPEN_AND_REQUEST:;
                if (GetRemainingDungeonMissionChecks(location, false) != 0) {
                    locationSymbol = remainingChecksSymbol;
                } else if (GetRemainingDungeonMissionChecks(location, true) != 0) {
                    locationSymbol = remainingChecksSymbol;
                } else if (IsLocationBonusChecksComplete(location) == false) {
                    locationSymbol = remainingChecksSymbol;
                } else {
                    locationSymbol = completeSymbol;
                }
        }
    } else if (location == DUNGEON_DARK_CRATER) {
        locationSymbol = instrumentSymbol;
    } else {
        enum dungeon_mode dmode = GetDungeonMode(location);
        switch (dmode) {
            default:
            case DMODE_CLOSED:;
            case DMODE_REQUEST:;
                locationSymbol = lockedSymbol;
                break;
            case DMODE_OPEN:;
                locationSymbol = unlockedSymbol;
                break;
            case DMODE_OPEN_AND_REQUEST:;
                if (GetRemainingDungeonMissionChecks(location, false) != 0) {
                    locationSymbol = remainingChecksSymbol;
                } else if (GetRemainingDungeonMissionChecks(location, true) != 0) {
                    locationSymbol = remainingChecksSymbol;
                } else if (IsLocationBonusChecksComplete(location) == false) {
                    locationSymbol = remainingChecksSymbol;
                } else {
                    locationSymbol = completeSymbol;
                }
                break;
        }
    }
    preArgs.id_vals[0] = GetTrackerList()[option_id];
    preArgs.strings[0] = locationSymbol;
    
    struct preprocessor_flags preFlagTracker = {.timer_1 = true, .flags_1 = 0x6A};
    PreprocessString(buffer, 0x400, "[string:0][CLUM_SET:26][dungeon:0]", preFlagTracker, &preArgs);

    return buffer;
}

// Utility function to handle drawing circular progression bars in the tracker box.
void DrawCircleBarInTextBox(signed char idx, int radius, int centerX, int centerY, uint32_t toGet, uint32_t gotten, char* strLock, char* strUnlocked, int rotation) {
    if (toGet == 0) {
        return;
    }
    
    uint32_t step = _u32_div_f(4096, toGet);
    
    for(uint32_t i = 0; i < toGet; i++) {
        int x = ((int)TRIG_TABLE[0xFFF & (step * i + rotation)].cos * radius) >> 12;
        int y = ((int)TRIG_TABLE[0xFFF & (step * i + rotation)].sin * radius) >> 12;
        if(i < gotten) {
            DrawTextInWindow(idx, centerX + x, centerY + y, strUnlocked);
        } else {
            DrawTextInWindow(idx, centerX + x, centerY + y, strLock);
        }
    }
}

// TODO: Move into a text file or the text strings file to save space in overlay36 and
// for the purposes of localization.
// Generic Checks
char* giftCheck = "[kind:0]'s Gift: [string:0]";
char* itemCheck = "[item:0]: [string:0]";

// Town Checks
char* shopItemChecks1 = "[CLUM_SET:132]Shop Item 1: [CLUM_SET:202][string:0]\n"
                        "[CLUM_SET:132]Shop Item 2: [CLUM_SET:202][string:1]\n"
                        "[CLUM_SET:132]Shop Item 3: [CLUM_SET:202][string:2]\n"
                        "[CLUM_SET:132]Shop Item 4: [CLUM_SET:202][string:3]\n"
                        "[CLUM_SET:132]Shop Item 5: [CLUM_SET:202][string:4]\n";
char* shopItemChecks2 = "[CLUM_SET:132]Shop Item 6: [CLUM_SET:202][string:0]\n"
                        "[CLUM_SET:132]Shop Item 7: [CLUM_SET:202][string:1]\n"
                        "[CLUM_SET:132]Shop Item 8: [CLUM_SET:202][string:2]\n"
                        "[CLUM_SET:132]Shop Item 9: [CLUM_SET:202][string:3]\n"
                        "[CLUM_SET:132]Shop Item 10: [CLUM_SET:202][string:4]";  
char* townBankChecks1 = "[CLUM_SET:15][kind:0] Rewards\n"
                        "[CLUM_SET:13]100G: [string:0]\n"
                        "[CLUM_SET:15]5000G: [string:1]\n"
                        "[CLUM_SET:13]10000G: [string:2]\n"
                        "[CLUM_SET:15]20000G: [string:3]\n";
char* townBankChecks2 = "[CLUM_SET:15]50000G: [string:0]\n"
                        "[CLUM_SET:13]100000G: [string:1]\n"
                        "[CLUM_SET:15]9999999G: [string:2]\n";
char* townBankCheckNotice = "[CS:Z]Checks Above 20000G Are Non-Essential.[CR]";
                            
// Guild Checks
char* specialEpisodeChecks = "[CLUM_SET:95]Bidoof's SE Location: [string:0]\n"
                             "[CLUM_SET:95]Igglybuff's SE Location: [string:1]\n"
                             "[CLUM_SET:95]Sunflora's SE Location: [string:2]\n"
                             "[CLUM_SET:95]Team Charm SE Location: [string:3]";
char* rankChecks1 = "[CLUM_SET:15]Bronze Rank: [string:0]\n"
                    "[CLUM_SET:15]Silver Rank: [string:1]\n"
                    "[CLUM_SET:15]Gold Rank: [string:2]\n"
                    "[CLUM_SET:15]Diamond Rank: [string:3]\n"
                    "[CLUM_SET:15]Super Rank: [string:4]\n";
char* rankChecks2 = "[CLUM_SET:15]Ultra Rank: [string:0]\n"
                    "[CLUM_SET:15]Hyper Rank: [string:1]\n"
                    "[CLUM_SET:15]Master Rank: [string:2]\n"
                    "[CLUM_SET:15]Master[M:S3] Rank: [string:3]\n"
                    "[CLUM_SET:15]Master[M:S3][M:S3] Rank: [string:4]\n";
char* rankChecks3 = "[CLUM_SET:110]Master[M:S3][M:S3][M:S3] Rank: [string:0]\n"
                    "[CLUM_SET:110]Guildmaster Rank: [string:1]\n";
char* rankChecksNotice = "[CS:Z]Ranks Above Master Are Non-Essential.[CR]";
                         
// Cafe Checks
char* cafeCheck1 = "[CLUM_SET:15]Aqua-Monica Mission: [string:0]\n"
                   "[CLUM_SET:15]Terra Cymbal Mission: [string:1]\n"
                   "[CLUM_SET:15]Icy Flute Mission: [string:2]\n"
                   "[CLUM_SET:15]Fiery Drum Mission: [string:3]\n"
                   "[CLUM_SET:15]Rock Horn Mission: [string:4]\n";
char* cafeCheck2 = "[CLUM_SET:15]Sky Melodica Mission: [string:0]\n"
                   "[CLUM_SET:15]Grass Cornet Mission: [string:1]\n"
                   "[CLUM_SET:15]Recycle Shop Treasure: [string:2]\n"
                   "[CLUM_SET:15]Ludicolo Dance: [string:3]\n"
                   "[CLUM_SET:15]Recycle Shop Dungeons:\n"
                   "[CLUM_SET:15]";
                  
// Dungeon Related Checks
char* dungeonCheck = "[dungeon:0]: [string:0]";
char* missionDungeonChecks = "[CLUM_SET:15]Completed: [CLUM_SET:70][string:0]\n"
                             "[CLUM_SET:15]Jobs: [CS:C][CLUM_SET:70][value:0:1]/[value:1:1][CR]\n"
                             "[CLUM_SET:15]Outlaws: [CS:C][CLUM_SET:70][value:2:1]/[value:3:1][CR]";
char* bagUpgradeCheck = "Bag Upgrade [value:0:1]: [string:0]";
char* nonEssentialExtraInfo = "[CS:Z]Non-essential for Completion.[CR]";
char* bossInfo = "[CLUM_SET:128]Boss: [kind:0]";
char* duoBossInfo = "[CLUM_SET:128]Boss: [CLUM_SET: 155][kind:0] &\n"
                    "[CLUM_SET: 155][kind:1]";
char* ruleDungeonInfo = "Completed: [CLUM_SET:70][string:0]";
char* checklessDungeonInfo = "[CS:Z]This dungeon has no checks.[CR]";
char* escortInfo = "[CS:Z]Escort: [kind:0][CR]";
char* duoEscortInfo = "[CS:Z]Escort: [kind:0] & [kind:1][CR]";
char* beachCaveChecks = "[CLUM_SET:15][CS:C]Talk To[CR] [CS:N]Wigglytuff[CR]\n"
                        "[CLUM_SET:15][CS:C]After Completing[CR]:\n"
                        "[CLUM_SET:15]Team Name: [string:0]\n";
char* beachCaveInfo = "[CS:Z]Unlocked from the start.[CR]";
char* skyPeakEightCheck = "[kind:0]'s Gratitude: [string:0]";
char* miracleSeaCheck = "[kind:0] Healed: [string:0]";

// Other Checks
char* blueGoomiCheck = "Blue Goomi #[value:0:0]: [string:0]";


// Special Episode Check
char* bidoofDungeonChecks = "[CLUM_SET:15][CS:P]SE Marowak Dojo's Revival[CR]: [string:0]\n"
                            "[CLUM_SET:15][CS:P]SE Deep Star Cave[CR]: [string:1]\n"
                            "[CLUM_SET:15][CS:P]SE Star Cave Pit[CR]: [string:2]";
char* iggybuffDungeonChecks = "[CLUM_SET:15][CS:P]SE Murky Forest[CR]: [string:0]\n"
                            "[CLUM_SET:15][CS:P]SE Eastern Cave[CR]: [string:1]\n"
                            "[CLUM_SET:15][CS:P]SE Fortune Ravine[CR]: [string:2]";
char* sunfloraDungeonChecks = "[CLUM_SET:15][CS:P]SE Middle Spring Cave[CR]: [string:1]\n"
                              "[CLUM_SET:15][CS:P]SE Upper Spring Cave[CR]: [string:0]\n"
                              "[CLUM_SET:15][CS:P]SE Spring Cave Pit[CR]: [string:2]";
char* charmDungeonChecks = "[CLUM_SET:15][CS:P]SE Southern Jungle[CR]: [string:0]\n"
                           "[CLUM_SET:15][CS:P]SE Boulder Quarry[CR]: [string:1]\n"
                           "[CLUM_SET:15][CS:P]SE Right Cave Path[CR]: [string:2]\n"
                           "[CLUM_SET:15][CS:P]SE Left Cave Path[CR]: [string:3]\n"
                           "[CLUM_SET:15][CS:P]SE Limestone Cavern[CR]: [string:4]";
char* susknoirDungeonChecks1 = "[CLUM_SET:15][CS:P]SE Barren Valley[CR]: [string:0]\n"
                               "[CLUM_SET:15][CS:P]SE Dark Wasteland[CR]: [string:1]\n"
                               "[CLUM_SET:15][CS:P]SE Temporal Tower[CR]: [string:2]\n"
                               "[CLUM_SET:15][CS:P]SE Dusk Forest[CR]: [string:3]\n"
                               "[CLUM_SET:15][CS:P]SE Spacial Cliffs[CR]: [string:4]";
char* susknoirDungeonChecks2 = "[CLUM_SET:15][CS:P]SE Dark Ice Mountain[CR]: [string:0]\n"
                               "[CLUM_SET:15][CS:P]SE Icicle Forest[CR]: [string:1]\n"
                               "[CLUM_SET:15][CS:P]SE Vast Ice Mountain[CR]: [string:2]";
char* bidoSunfCafeChecks1 = "[CLUM_SET:15]Igglybuff's SE Location: [string:0]\n"
                            "[CLUM_SET:15]Team Charm SE Location: [string:1]\n"
                            "[CLUM_SET:15]Recycle Shop Treasure: [string:2]\n"
                            "[CLUM_SET:15]Ludicolo Dance: [string:3]";
char* bidoSunfCafeChecks2= "[CLUM_SET:15]Caf~E9 Drinks: [value:0:1]/[value:1:1]\n"
                            "[CLUM_SET:15]Caf~E9 Events: [value:2:1]/[value:3:1]\n"
                            "[CLUM_SET:15]Recycle Shop Dungeons:\n"
                            "[CLUM_SET:15]";
char* bidoSunfGuildChecks = "[CLUM_SET:15]Bidoof's SE Location: [string:0]\n"
                            "[CLUM_SET:15]Sunflora's SE Location: [string:1]\n"
                            "[CLUM_SET:15]Note: the Igglybuff and Team Charm\n"
                            "[CLUM_SET:15]SE locations are in Spinda\'s Cafe\n"
                            "[CLUM_SET:15]during this Special Episode!";
char* fractionString = "[value:0:1]/[value:1:1]";
struct window_params trackerTopScreenWinParams = {.x_offset = 2, .y_offset = 2, .width = 0x1C, .height = 0x14, .screen = {.val = SCREEN_SUB}, .box_type = {.val = 0xFF}};
struct preprocessor_args preArgs = {};
struct preprocessor_flags preFlagTracker = {};

// Utility function to streamline displaying gift information.
void DrawGiftCheckInWindow(int idx, int y, char* buffer, enum monster_id monster_id, bool giftGiven) {
    preArgs.flag_vals[0] = monster_id;
    preArgs.strings[0] = giftGiven ? completeSymbol : lockedSymbol;
    PreprocessString(buffer, TR_BUFF_LEN, giftCheck, preFlagTracker, &preArgs);
    DrawTextInWindow(idx, 128, y, buffer);
}

// Utility function to streamline displaying item checks.
void DrawItemCheckInWindow(int idx, int y, char* buffer, enum item_id item_id, bool itemGotten) {
    preArgs.id_vals[0] = item_id;
    preArgs.strings[0] = itemGotten ? checkSymbol : lockedSymbol;
    preArgs.number_vals[0] = 0; // Maybe item quantity?
    preArgs.number_vals[1] = 0; // Why do items pull from number_vals + 1????
    PreprocessString(buffer, TR_BUFF_LEN, itemCheck, preFlagTracker, &preArgs);
    DrawTextInWindow(idx, 128, y, buffer);
}

// Utility function to streamline displaying checks for individual dungeons.
void DrawDungeonCheckInWindow(int idx, int y, char* buffer, enum dungeon_id dungeon_id) {
    preArgs.id_vals[0] = dungeon_id;
    preArgs.strings[0] = LoadScriptVariableValueAtIndex(NULL, VAR_DUNGEON_CONQUEST_LIST, dungeon_id) ? checkSymbol : lockedSymbol;
    PreprocessString(buffer, TR_BUFF_LEN, dungeonCheck, preFlagTracker, &preArgs);
    DrawTextInWindow(idx, 15, y, buffer);
}

// Utility functon to streamline displaying boss information with optional gift.
void DrawBossInfoInWindow(int idx, int y, char* buffer, enum monster_id boss0, enum monster_id boss1, bool hasGift, bool giftGiven) {
    preArgs.flag_vals[0] = boss0;
    preArgs.flag_vals[1] = boss1;
    PreprocessString(buffer, TR_BUFF_LEN, (boss1 == MONSTER_NONE) ? bossInfo : duoBossInfo, preFlagTracker, &preArgs);
    DrawTextInWindow(idx, 1, y, buffer);
    
    if(hasGift == false) {
        return;
    }
    
    DrawGiftCheckInWindow(idx, y + ((boss1 == MONSTER_NONE) ? 13 : 26), buffer, boss0, giftGiven);
}

void DrawEscortInfoInWindow(int idx, int y, char* buffer, enum monster_id escort0, enum monster_id escort1) {
    preArgs.flag_vals[0] = escort0;
    preArgs.flag_vals[1] = escort1;
    PreprocessString(buffer, TR_BUFF_LEN, (escort1 == MONSTER_NONE) ? escortInfo : duoEscortInfo, preFlagTracker, &preArgs);
    DrawTextInWindow(idx, 15, y, buffer);
}

void ApTrackerTopScreenWindowUpdate(int idx, uint32_t location) {
    ClearWindow(idx);
    char temp[TR_BUFF_LEN];
    // TODO: Replace these where possible with the area tag or copying text strings.
    if(location == 255) {
        strncpy(temp, "Tracker: [CS:P]Spinda's Caf~E9[CR]", TR_BUFF_LEN);
    } else if (location == 254) {
        strncpy(temp, "Tracker: [CS:P]Wigglytuff Guild[CR]", TR_BUFF_LEN);
    } else if (location == 180) {
        strncpy(temp, "Tracker: [CS:P]Marowak Dojo[CR]", TR_BUFF_LEN);
    } else if (location == 252) {
        strncpy(temp, "Tracker: [CS:P]Bidoof's Wish[CR]", TR_BUFF_LEN);
    } else if (location == 251) {
        strncpy(temp, "Tracker: [CS:P]Igglybuff The Prodigy[CR]", TR_BUFF_LEN);
    } else if (location == 250) {
        strncpy(temp, "Tracker: [CS:P]Today's \"Oh My Gosh\"[CR]", TR_BUFF_LEN);
    } else if (location == 249) {
        strncpy(temp, "Tracker: [CS:P]Here Comes Team Charm[CR]", TR_BUFF_LEN);
    } else if (location == 248) {
        strncpy(temp, "Tracker: [CS:P]In The Future of Darkness[CR]", TR_BUFF_LEN);
    } else if (location == DUNGEON_ICE_AEGIS_CAVE) {
        strncpy(temp, "Tracker: [CS:P]Aegis Cave[CR]", TR_BUFF_LEN);
    } else {
        preArgs.id_vals[0] = location;
        PreprocessString(temp, TR_BUFF_LEN, "Tracker: [dungeon:0]", preFlagTracker, &preArgs);
    }
    DrawTextInWindow(idx, (trackerTopScreenWinParams.width * 8 - GetStringWidth(temp)) / 2, 2, temp);
    
    switch(location) {
        case 247:; // Treasure Town
            // Shop Checks
            preArgs.strings[0] = (GetSubXBit(10)) ? checkSymbol : lockedSymbol;
            preArgs.strings[1] = (GetSubXBit(11)) ? checkSymbol : lockedSymbol;
            preArgs.strings[2] = (GetSubXBit(12)) ? checkSymbol : lockedSymbol;
            preArgs.strings[3] = (GetSubXBit(13)) ? checkSymbol : lockedSymbol;
            preArgs.strings[4] = (GetSubXBit(14)) ? checkSymbol : lockedSymbol;
            PreprocessString(temp, TR_BUFF_LEN, shopItemChecks1, preFlagTracker, &preArgs);
            DrawTextInWindow(idx, 1, 16, temp);
            preArgs.strings[0] = (GetSubXBit(15)) ? checkSymbol : lockedSymbol;
            preArgs.strings[1] = (GetSubXBit(16)) ? checkSymbol : lockedSymbol;
            preArgs.strings[2] = (GetSubXBit(17)) ? checkSymbol : lockedSymbol;
            preArgs.strings[3] = (GetSubXBit(18)) ? checkSymbol : lockedSymbol;
            preArgs.strings[4] = (GetSubXBit(19)) ? checkSymbol : lockedSymbol;
            PreprocessString(temp, TR_BUFF_LEN, shopItemChecks2, preFlagTracker, &preArgs);
            DrawTextInWindow(idx, 1, 81, temp);
            // Bank Checks
            preArgs.flag_vals[0] = MONSTER_DUSKULL;
            preArgs.strings[0] = (GetSubXBit(81)) ? moneySymbol : lockedSymbol;
            preArgs.strings[1] = (GetSubXBit(82)) ? moneySymbol : lockedSymbol;
            preArgs.strings[2] = (GetSubXBit(83)) ? moneySymbol : lockedSymbol;
            preArgs.strings[3] = (GetSubXBit(84)) ? moneySymbol : lockedSymbol;
            PreprocessString(temp, TR_BUFF_LEN, townBankChecks1, preFlagTracker, &preArgs);
            DrawTextInWindow(idx, 1, 16, temp);
            preArgs.strings[0] = (GetSubXBit(85)) ? moneySymbol : lockedSymbol;
            preArgs.strings[1] = (GetSubXBit(86)) ? moneySymbol : lockedSymbol;
            preArgs.strings[2] = (GetSubXBit(87)) ? moneySymbol : lockedSymbol;
            PreprocessString(temp, TR_BUFF_LEN, townBankChecks2, preFlagTracker, &preArgs);
            DrawTextInWindow(idx, 1, 81, temp);
            if(!apSettings.longLocationsOn) {
                DrawTextInWindow(idx, 15, 146, townBankCheckNotice);
            }
            UpdateWindow(idx);
            return;
        case 254:; // Guild
            // Rank Checks
            preArgs.strings[0] = (GetSubXBit(73)) ? checkSymbol : lockedSymbol;
            preArgs.strings[1] = (GetSubXBit(74)) ? checkSymbol : lockedSymbol;
            preArgs.strings[2] = (GetSubXBit(75)) ? checkSymbol : lockedSymbol;
            preArgs.strings[3] = (GetSubXBit(76)) ? checkSymbol : lockedSymbol;
            preArgs.strings[4] = (GetSubXBit(77)) ? checkSymbol : lockedSymbol;
            PreprocessString(temp, TR_BUFF_LEN, rankChecks1, preFlagTracker, &preArgs);
            DrawTextInWindow(idx, 1, 16, temp);
            preArgs.strings[0] = (GetSubXBit(78)) ? checkSymbol : lockedSymbol;
            preArgs.strings[1] = (GetSubXBit(79)) ? checkSymbol : lockedSymbol;
            preArgs.strings[2] = (GetSubXBit(80)) ? checkSymbol : lockedSymbol;
            preArgs.strings[3] = (GetSubXBit(55)) ? checkSymbol : lockedSymbol;
            preArgs.strings[4] = (GetSubXBit(56)) ? checkSymbol : lockedSymbol;
            PreprocessString(temp, TR_BUFF_LEN, rankChecks2, preFlagTracker, &preArgs);
            DrawTextInWindow(idx, 1, 81, temp);
            preArgs.strings[0] = (GetSubXBit(57)) ? checkSymbol : lockedSymbol;
            preArgs.strings[1] = (GetSubXBit(58)) ? checkSymbol : lockedSymbol;
            PreprocessString(temp, TR_BUFF_LEN, rankChecks3, preFlagTracker, &preArgs);
            DrawTextInWindow(idx, 1, 16, temp);
            // Special Episode Checks (Main)
            preArgs.strings[0] = (GetSubXBit(5)) ? checkSymbol : lockedSymbol;
            preArgs.strings[1] = (GetSubXBit(6)) ? checkSymbol : lockedSymbol;
            preArgs.strings[2] = (GetSubXBit(7)) ? checkSymbol : lockedSymbol;
            preArgs.strings[3] = (GetSubXBit(8)) ? checkSymbol : lockedSymbol;
            PreprocessString(temp, TR_BUFF_LEN, specialEpisodeChecks, preFlagTracker, &preArgs);
            DrawTextInWindow(idx, 1, 68, temp);
            // Blue Goomi Check (1)
            preArgs.strings[0] = (GetSubXBit(5)) ? checkSymbol : lockedSymbol;
            preArgs.number_vals[0] = 1;
            PreprocessString(temp, TR_BUFF_LEN, blueGoomiCheck, preFlagTracker, &preArgs);
            DrawTextInWindow(idx, 120, 133, temp);
            // Long Location Notice
            if(!apSettings.longLocationsOn) {
                DrawTextInWindow(idx, 15, 146, rankChecksNotice);
            }
            UpdateWindow(idx);
            return;
        case 180:; // Dojo Checks
            int y = 16;
            for(int d = DUNGEON_NORMAL_FLY_MAZE; d <= 189; d++) {
                DrawDungeonCheckInWindow(idx, y, temp, d);
                y += 13;
            }
            DrawDungeonCheckInWindow(idx, 146, temp, 191);
            UpdateWindow(idx);
            return;
        case 255:; // Cafe
            DrawTextInWindow(idx, 1, 16, "[CLUM_SET:142]Caf~E9 Drinks");
            DrawTextInWindow(idx, 1, 79, "[CLUM_SET:142]Caf~E9 Events");
            preArgs.strings[0] = (GetSubXBit(48)) ? checkSymbol : lockedSymbol;
            preArgs.strings[1] = (GetSubXBit(49)) ? checkSymbol : lockedSymbol;
            preArgs.strings[2] = (GetSubXBit(50)) ? checkSymbol : lockedSymbol;
            preArgs.strings[3] = (GetSubXBit(51)) ? checkSymbol : lockedSymbol;
            preArgs.strings[4] = (GetSubXBit(52)) ? checkSymbol : lockedSymbol;
            PreprocessString(temp, TR_BUFF_LEN, cafeCheck1, preFlagTracker, &preArgs);
            DrawTextInWindow(idx, 1, 16, temp);
            preArgs.strings[0] = (GetSubXBit(53)) ? checkSymbol : lockedSymbol;
            preArgs.strings[1] = (GetSubXBit(54)) ? checkSymbol : lockedSymbol;
            preArgs.strings[2] = (GetSubXBit(59)) ? checkSymbol : lockedSymbol;
            preArgs.strings[3] = (GetSubXBit(88)) ? checkSymbol : lockedSymbol;
            PreprocessString(temp, TR_BUFF_LEN, cafeCheck2, preFlagTracker, &preArgs);
            strncat(temp, (GetSubXBit(60)) ? checkSymbol : lockedSymbol, TR_BUFF_LEN);
            strncat(temp, (GetSubXBit(61)) ? checkSymbol : lockedSymbol, TR_BUFF_LEN);
            strncat(temp, (GetSubXBit(62)) ? checkSymbol : lockedSymbol, TR_BUFF_LEN);
            if (IsDarkraiGoal()) {
                strncat(temp, (GetSubXBit(63)) ? checkSymbol : lockedSymbol, TR_BUFF_LEN);
                strncat(temp, (GetSubXBit(64)) ? checkSymbol : lockedSymbol, TR_BUFF_LEN);
            }
            DrawTextInWindow(idx, 1, 81, temp);
            UpdateWindow(idx);
            return;
        case 252:; // Bidoof SE
            preArgs.strings[0] = GetSubXBit(66) ? checkSymbol : lockedSymbol;
            preArgs.strings[1] = GetDungeonMode(DUNGEON_DEEP_STAR_CAVE) == DMODE_OPEN_AND_REQUEST ? checkSymbol : lockedSymbol;
            preArgs.strings[2] = GetDungeonMode(DUNGEON_STAR_CAVE_PIT) == DMODE_OPEN_AND_REQUEST ? checkSymbol : lockedSymbol;
            PreprocessString(temp, TR_BUFF_LEN, bidoofDungeonChecks, preFlagTracker, &preArgs);
            DrawTextInWindow(idx, 1, 16, temp);
            switch (LoadScriptVariableValue(NULL, VAR_GROUND_ENTER)) {
                case 323:; // Likely because our pmdsky-debug is outdated. The value for T01P01A/T01P02A
                case 324:; // is different between EU/NA/JP. So, need to use raw non-enum value.
                    preArgs.strings[0] = (GetSubXBit(10)) ? checkSymbol : lockedSymbol;
                    preArgs.strings[1] = (GetSubXBit(11)) ? checkSymbol : lockedSymbol;
                    preArgs.strings[2] = (GetSubXBit(12)) ? checkSymbol : lockedSymbol;
                    preArgs.strings[3] = (GetSubXBit(13)) ? checkSymbol : lockedSymbol;
                    preArgs.strings[4] = (GetSubXBit(14)) ? checkSymbol : lockedSymbol;
                    PreprocessString(temp, TR_BUFF_LEN, shopItemChecks1, preFlagTracker, &preArgs);
                    DrawTextInWindow(idx, 1, 29, temp);
                    preArgs.strings[0] = (GetSubXBit(15)) ? checkSymbol : lockedSymbol;
                    preArgs.strings[1] = (GetSubXBit(16)) ? checkSymbol : lockedSymbol;
                    preArgs.strings[2] = (GetSubXBit(17)) ? checkSymbol : lockedSymbol;
                    preArgs.strings[3] = (GetSubXBit(18)) ? checkSymbol : lockedSymbol;
                    preArgs.strings[4] = (GetSubXBit(19)) ? checkSymbol : lockedSymbol;
                    PreprocessString(temp, TR_BUFF_LEN, shopItemChecks2, preFlagTracker, &preArgs);
                    DrawTextInWindow(idx, 1, 94, temp);
                    preArgs.strings[0] = (GetSubXBit(81)) ? moneySymbol : lockedSymbol;
                    preArgs.strings[1] = (GetSubXBit(82)) ? moneySymbol : lockedSymbol;
                    preArgs.strings[2] = (GetSubXBit(83)) ? moneySymbol : lockedSymbol;
                    preArgs.strings[3] = (GetSubXBit(84)) ? moneySymbol : lockedSymbol;
                    PreprocessString(temp, TR_BUFF_LEN, townBankChecks1, preFlagTracker, &preArgs);
                    DrawTextInWindow(idx, 1, 55, temp);
                    preArgs.strings[0] = (GetSubXBit(85)) ? moneySymbol : lockedSymbol;
                    preArgs.strings[1] = (GetSubXBit(86)) ? moneySymbol : lockedSymbol;
                    preArgs.strings[2] = (GetSubXBit(87)) ? moneySymbol : lockedSymbol;
                    PreprocessString(temp, TR_BUFF_LEN, townBankChecks2, preFlagTracker, &preArgs);
                    DrawTextInWindow(idx, 1, 120, temp);
                    break;
                case LEVEL_P01P04A:;
                    preArgs.strings[0] = (GetSubXBit(6)) ? checkSymbol : lockedSymbol;
                    preArgs.strings[1] = (GetSubXBit(8)) ? checkSymbol : lockedSymbol;
                    preArgs.strings[2] = (GetSubXBit(59)) ? checkSymbol : lockedSymbol;
                    preArgs.strings[3] = (GetSubXBit(88)) ? checkSymbol : lockedSymbol;
                    PreprocessString(temp, TR_BUFF_LEN, bidoSunfCafeChecks1, preFlagTracker, &preArgs);
                    DrawTextInWindow(idx, 1, 55, temp);
                    preArgs.number_vals[0] = CUSTOM_SAVE_AREA.acquiredCafeDrinkChecks;
                    preArgs.number_vals[1] = apSettings.cafeDrinkMax;
                    preArgs.number_vals[2] = CUSTOM_SAVE_AREA.acquiredCafeEventChecks;
                    preArgs.number_vals[3] = apSettings.cafeEventMax;
                    PreprocessString(temp, TR_BUFF_LEN, bidoSunfCafeChecks2, preFlagTracker, &preArgs);
                    strncat(temp, (GetSubXBit(60)) ? checkSymbol : lockedSymbol, TR_BUFF_LEN);
                    strncat(temp, (GetSubXBit(61)) ? checkSymbol : lockedSymbol, TR_BUFF_LEN);
                    strncat(temp, (GetSubXBit(62)) ? checkSymbol : lockedSymbol, TR_BUFF_LEN);
                    if (IsDarkraiGoal()) {
                        strncat(temp, (GetSubXBit(63)) ? checkSymbol : lockedSymbol, TR_BUFF_LEN);
                        strncat(temp, (GetSubXBit(64)) ? checkSymbol : lockedSymbol, TR_BUFF_LEN);
                    }
                    DrawTextInWindow(idx, 1, 107, temp);
                    break;
                case LEVEL_G01P01A:;
                case LEVEL_G01P02A:;
                case LEVEL_G01P03A:;
                case LEVEL_G01P04A:;
                case LEVEL_G01P05A:;
                case LEVEL_G01P06A:;
                case LEVEL_G01P07A:;
                case LEVEL_G01P08A:;
                case LEVEL_G01P09A:;
                case LEVEL_G01P10A:;
                    preArgs.strings[0] = (GetSubXBit(5)) ? checkSymbol : lockedSymbol;
                    preArgs.strings[1] = (GetSubXBit(7)) ? checkSymbol : lockedSymbol;
                    PreprocessString(temp, TR_BUFF_LEN, bidoSunfGuildChecks, preFlagTracker, &preArgs);
                    DrawTextInWindow(idx, 1, 64, temp);
                    break;
                default:;
                    break;
            }
            UpdateWindow(idx);
            return;
        case 251:; // Igglybuff SE
            preArgs.strings[0] = GetDungeonMode(128) == DMODE_OPEN_AND_REQUEST ? checkSymbol : lockedSymbol;
            preArgs.strings[1] = GetDungeonMode(129) == DMODE_OPEN_AND_REQUEST ? checkSymbol : lockedSymbol;
            preArgs.strings[2] = GetDungeonMode(130) == DMODE_OPEN_AND_REQUEST ? checkSymbol : lockedSymbol;
            PreprocessString(temp, TR_BUFF_LEN, iggybuffDungeonChecks, preFlagTracker, &preArgs);
            DrawTextInWindow(idx, 1, 16, temp);
            UpdateWindow(idx);
            return;
        case 250:; // Sunflora SE
            preArgs.strings[0] = GetDungeonMode(159) == DMODE_OPEN_AND_REQUEST ? checkSymbol : lockedSymbol;
            preArgs.strings[1] = GetDungeonMode(161) == DMODE_OPEN_AND_REQUEST ? checkSymbol : lockedSymbol;
            preArgs.strings[2] = GetDungeonMode(164) == DMODE_OPEN_AND_REQUEST ? checkSymbol : lockedSymbol;
            PreprocessString(temp, TR_BUFF_LEN, sunfloraDungeonChecks, preFlagTracker, &preArgs);
            DrawTextInWindow(idx, 1, 16, temp);
            switch (LoadScriptVariableValue(NULL, VAR_GROUND_ENTER)) {
                case 323:; // Likely because our pmdsky-debug is outdated. The value for T01P01A/T01P02A
                case 324:; // is different between EU/NA/JP. So, need to use raw non-enum value.
                    preArgs.strings[0] = (GetSubXBit(10)) ? checkSymbol : lockedSymbol;
                    preArgs.strings[1] = (GetSubXBit(11)) ? checkSymbol : lockedSymbol;
                    preArgs.strings[2] = (GetSubXBit(12)) ? checkSymbol : lockedSymbol;
                    preArgs.strings[3] = (GetSubXBit(13)) ? checkSymbol : lockedSymbol;
                    preArgs.strings[4] = (GetSubXBit(14)) ? checkSymbol : lockedSymbol;
                    PreprocessString(temp, TR_BUFF_LEN, shopItemChecks1, preFlagTracker, &preArgs);
                    DrawTextInWindow(idx, 1, 29, temp);
                    preArgs.strings[0] = (GetSubXBit(15)) ? checkSymbol : lockedSymbol;
                    preArgs.strings[1] = (GetSubXBit(16)) ? checkSymbol : lockedSymbol;
                    preArgs.strings[2] = (GetSubXBit(17)) ? checkSymbol : lockedSymbol;
                    preArgs.strings[3] = (GetSubXBit(18)) ? checkSymbol : lockedSymbol;
                    preArgs.strings[4] = (GetSubXBit(19)) ? checkSymbol : lockedSymbol;
                    PreprocessString(temp, TR_BUFF_LEN, shopItemChecks2, preFlagTracker, &preArgs);
                    DrawTextInWindow(idx, 1, 94, temp);
                    preArgs.strings[0] = (GetSubXBit(81)) ? moneySymbol : lockedSymbol;
                    preArgs.strings[1] = (GetSubXBit(82)) ? moneySymbol : lockedSymbol;
                    preArgs.strings[2] = (GetSubXBit(83)) ? moneySymbol : lockedSymbol;
                    preArgs.strings[3] = (GetSubXBit(84)) ? moneySymbol : lockedSymbol;
                    PreprocessString(temp, TR_BUFF_LEN, townBankChecks1, preFlagTracker, &preArgs);
                    DrawTextInWindow(idx, 1, 55, temp);
                    preArgs.strings[0] = (GetSubXBit(85)) ? moneySymbol : lockedSymbol;
                    preArgs.strings[1] = (GetSubXBit(86)) ? moneySymbol : lockedSymbol;
                    preArgs.strings[2] = (GetSubXBit(87)) ? moneySymbol : lockedSymbol;
                    PreprocessString(temp, TR_BUFF_LEN, townBankChecks2, preFlagTracker, &preArgs);
                    DrawTextInWindow(idx, 1, 120, temp);
                    break;
                case LEVEL_P01P04A:;
                    preArgs.strings[0] = (GetSubXBit(6)) ? checkSymbol : lockedSymbol;
                    preArgs.strings[1] = (GetSubXBit(8)) ? checkSymbol : lockedSymbol;
                    preArgs.strings[2] = (GetSubXBit(59)) ? checkSymbol : lockedSymbol;
                    preArgs.strings[3] = (GetSubXBit(88)) ? checkSymbol : lockedSymbol;
                    PreprocessString(temp, TR_BUFF_LEN, bidoSunfCafeChecks1, preFlagTracker, &preArgs);
                    DrawTextInWindow(idx, 1, 55, temp);
                    preArgs.number_vals[0] = CUSTOM_SAVE_AREA.acquiredCafeDrinkChecks;
                    preArgs.number_vals[1] = apSettings.cafeDrinkMax;
                    preArgs.number_vals[2] = CUSTOM_SAVE_AREA.acquiredCafeEventChecks;
                    preArgs.number_vals[3] = apSettings.cafeEventMax;
                    PreprocessString(temp, TR_BUFF_LEN, bidoSunfCafeChecks2, preFlagTracker, &preArgs);
                    strncat(temp, (GetSubXBit(60)) ? checkSymbol : lockedSymbol, TR_BUFF_LEN);
                    strncat(temp, (GetSubXBit(61)) ? checkSymbol : lockedSymbol, TR_BUFF_LEN);
                    strncat(temp, (GetSubXBit(62)) ? checkSymbol : lockedSymbol, TR_BUFF_LEN);
                    if (IsDarkraiGoal()) {
                        strncat(temp, (GetSubXBit(63)) ? checkSymbol : lockedSymbol, TR_BUFF_LEN);
                        strncat(temp, (GetSubXBit(64)) ? checkSymbol : lockedSymbol, TR_BUFF_LEN);
                    }
                    DrawTextInWindow(idx, 1, 107, temp);
                    break;
                case LEVEL_G01P01A:;
                case LEVEL_G01P02A:;
                case LEVEL_G01P03A:;
                case LEVEL_G01P04A:;
                case LEVEL_G01P05A:;
                case LEVEL_G01P06A:;
                case LEVEL_G01P07A:;
                case LEVEL_G01P08A:;
                case LEVEL_G01P09A:;
                case LEVEL_G01P10A:;
                    preArgs.strings[0] = (GetSubXBit(5)) ? checkSymbol : lockedSymbol;
                    preArgs.strings[1] = (GetSubXBit(7)) ? checkSymbol : lockedSymbol;
                    PreprocessString(temp, TR_BUFF_LEN, bidoSunfGuildChecks, preFlagTracker, &preArgs);
                    DrawTextInWindow(idx, 1, 64, temp);
                    break;
                default:;
                    break;
            }
            UpdateWindow(idx);
            return;
        case 249:; // Team Charm SE
            preArgs.strings[0] = GetDungeonMode(149) == DMODE_OPEN_AND_REQUEST ? checkSymbol : lockedSymbol;
            preArgs.strings[1] = GetDungeonMode(150) == DMODE_OPEN_AND_REQUEST ? checkSymbol : lockedSymbol;
            preArgs.strings[2] = GetDungeonMode(153) == DMODE_OPEN_AND_REQUEST ? checkSymbol : lockedSymbol;
            preArgs.strings[3] = GetDungeonMode(154) == DMODE_OPEN_AND_REQUEST ? checkSymbol : lockedSymbol;
            preArgs.strings[2] = GetDungeonMode(155) == DMODE_OPEN_AND_REQUEST ? checkSymbol : lockedSymbol;
            PreprocessString(temp, TR_BUFF_LEN, charmDungeonChecks, preFlagTracker, &preArgs);
            DrawTextInWindow(idx, 1, 16, temp);
            UpdateWindow(idx);
            return;
        case 248:; // Future SE
            preArgs.strings[0] = GetDungeonMode(133) == DMODE_OPEN_AND_REQUEST ? checkSymbol : lockedSymbol;
            preArgs.strings[1] = GetDungeonMode(136) == DMODE_OPEN_AND_REQUEST ? checkSymbol : lockedSymbol;
            preArgs.strings[2] = GetDungeonMode(137) == DMODE_OPEN_AND_REQUEST ? checkSymbol : lockedSymbol;
            preArgs.strings[3] = GetDungeonMode(139) == DMODE_OPEN_AND_REQUEST ? checkSymbol : lockedSymbol;
            preArgs.strings[2] = GetDungeonMode(141) == DMODE_OPEN_AND_REQUEST ? checkSymbol : lockedSymbol;
            PreprocessString(temp, TR_BUFF_LEN, susknoirDungeonChecks1, preFlagTracker, &preArgs);
            DrawTextInWindow(idx, 1, 16, temp);
            preArgs.strings[0] = GetDungeonMode(142) == DMODE_OPEN_AND_REQUEST ? checkSymbol : lockedSymbol;
            preArgs.strings[1] = GetDungeonMode(145) == DMODE_OPEN_AND_REQUEST ? checkSymbol : lockedSymbol;
            preArgs.strings[2] = GetDungeonMode(146) == DMODE_OPEN_AND_REQUEST ? checkSymbol : lockedSymbol;
            PreprocessString(temp, TR_BUFF_LEN, susknoirDungeonChecks2, preFlagTracker, &preArgs);
            DrawTextInWindow(idx, 1, 81, temp);
            UpdateWindow(idx);
            return;
        case DUNGEON_HIDDEN_LAND:;
            // Note: Dusknoir is a boss here. I am not sure where to put it while the tracker is still
            // in spinning mode (before you would fight Dusknoir). After the dungeon is completed I'll
            // add that information; however it's not relevant anymore then. Feel free to change this
            // should you find a better arrangement.
            if(GetDungeonMode(location) == DMODE_OPEN_AND_REQUEST && IsDarkraiGoal()) {
                DrawBossInfoInWindow(idx, 16, temp, MONSTER_DUSKNOIR, MONSTER_SABLEYE, false, false);
                break;
            }
        case DUNGEON_TEMPORAL_TOWER:;
            if(GetDungeonMode(location) == DMODE_OPEN_AND_REQUEST && IsDarkraiGoal()) {
                DrawBossInfoInWindow(idx, 16, temp, MONSTER_DIALGA, MONSTER_NONE, true, GetSubXBit(28));
                break;
            }
            DrawCircleBarInTextBox(idx, 60, 112, 84, apSettings.requiredRelicFragmentShards, CUSTOM_SAVE_AREA.acquiredRelicFragmentShards, lockedSymbol, relicSymbol, trackerRotate);
            preArgs.number_vals[0] = CUSTOM_SAVE_AREA.acquiredRelicFragmentShards;
            preArgs.number_vals[1] = apSettings.requiredRelicFragmentShards;
            PreprocessString(temp, TR_BUFF_LEN, fractionString, preFlagTracker, &preArgs);
            DrawTextInWindow(idx, (trackerTopScreenWinParams.width * 8 - GetStringWidth(temp)) / 2, 84, temp);
            UpdateWindow(idx);
            return;
        case DUNGEON_DARK_CRATER:;
            if(IsDarkraiGoal()) {
                DrawCircleBarInTextBox(idx, 60, 109, 84, apSettings.requiredInstruments, CUSTOM_SAVE_AREA.acquiredInstruments, lockedSymbol, instrumentSymbol, trackerRotate);
                preArgs.number_vals[0] = CUSTOM_SAVE_AREA.acquiredInstruments;
                preArgs.number_vals[1] = apSettings.requiredInstruments;
                PreprocessString(temp, TR_BUFF_LEN, fractionString, preFlagTracker, &preArgs);
                DrawTextInWindow(idx, (trackerTopScreenWinParams.width * 8 - GetStringWidth(temp)) / 2, 84, temp);
                UpdateWindow(idx);
                return;
            }
            break;
        case DUNGEON_BEACH_CAVE:;
            // Team Name Check
            preArgs.strings[0] = (GetSubXBit(127)) ? checkSymbol : lockedSymbol;
            PreprocessString(temp, TR_BUFF_LEN, beachCaveChecks, preFlagTracker, &preArgs);
            DrawTextInWindow(idx, 1, 81, temp);
            // Bag Upgrade (0) Check
            preArgs.number_vals[0] = 1;
            preArgs.strings[0] = (GetSubXBit(0)) ? bagSymbol : lockedSymbol;
            PreprocessString(temp, TR_BUFF_LEN, bagUpgradeCheck, preFlagTracker, &preArgs);
            DrawTextInWindow(idx, 15, 120, temp);
            // Boss Information
            DrawBossInfoInWindow(idx, 16, temp, MONSTER_ZUBAT, MONSTER_KOFFING, false, false);
            // Extra Information
            DrawTextInWindow(idx, 15, 146, beachCaveInfo);
            break;
        case DUNGEON_MT_BRISTLE:;
            // Bag Upgrade (1) Check
            preArgs.strings[0] = (GetSubXBit(1)) ? bagSymbol : lockedSymbol;
            preArgs.number_vals[0] = 2;
            PreprocessString(temp, TR_BUFF_LEN, bagUpgradeCheck, preFlagTracker, &preArgs);
            DrawTextInWindow(idx, 15, 81, temp);
            // Boss Information
            DrawBossInfoInWindow(idx, 16, temp, MONSTER_DROWZEE, MONSTER_NONE, false, false);
            break;
        case DUNGEON_APPLE_WOODS:;
            // Bag Upgrade (2) Check
            preArgs.strings[0] = (GetSubXBit(2)) ? bagSymbol : lockedSymbol;
            preArgs.number_vals[0] = 3;
            PreprocessString(temp, TR_BUFF_LEN, bagUpgradeCheck, preFlagTracker, &preArgs);
            DrawTextInWindow(idx, 15, 81, temp);
            break;
        case DUNGEON_CRAGGY_COAST:;
        case DUNGEON_SIDE_PATH:;
        case DUNGEON_MT_HORN:;
        case DUNGEON_ROCK_PATH:;
            // Escort Information
            if(LoadScriptVariableValueAtIndex(NULL, VAR_DUNGEON_CONQUEST_LIST, location) == false) {
                DrawEscortInfoInWindow(idx, 146, temp, MONSTER_BIDOOF, MONSTER_NONE);
            }
            break;
        case DUNGEON_STEAM_CAVE:;
            // Bag Upgrade (3) Check
            preArgs.strings[0] = (GetSubXBit(3)) ? bagSymbol : lockedSymbol;
            preArgs.number_vals[0] = 4;
            PreprocessString(temp, TR_BUFF_LEN, bagUpgradeCheck, preFlagTracker, &preArgs);
            DrawTextInWindow(idx, 15, 81, temp);
            // Boss Information
            DrawBossInfoInWindow(idx, 16, temp, MONSTER_GROUDON, MONSTER_NONE, false, false);
            // Boss 2/Gift Information
            if(IsDarkraiGoal()) {
                DrawBossInfoInWindow(idx, 42, temp, MONSTER_UXIE, MONSTER_NONE, true, GetSubXBit(25));
            }
            break;
        case DUNGEON_AMP_PLAINS:;
            // Boss Information
            DrawBossInfoInWindow(idx, 16, temp, MONSTER_MANECTRIC, MONSTER_ELECTRIKE, false, false);
            break;
        case DUNGEON_QUICKSAND_CAVE:;
            // Boss/Gift Information
            DrawBossInfoInWindow(idx, 16, temp, MONSTER_MESPRIT, MONSTER_NONE, IsDarkraiGoal(), GetSubXBit(26));
            break;
        case DUNGEON_CRYSTAL_CROSSING:;
            // Boss Information
            DrawBossInfoInWindow(idx, 16, temp, MONSTER_GROVYLE, MONSTER_NONE, false, false);
            // Boss 2/Gift Information
            if(IsDarkraiGoal()) {
                DrawBossInfoInWindow(idx, 42, temp, MONSTER_AZELF, MONSTER_NONE, true, GetSubXBit(27));
            }
            break;
        case DUNGEON_SEALED_RUIN:;
            // Boss Information
            DrawBossInfoInWindow(idx, 16, temp, MONSTER_SPIRITOMB, MONSTER_NONE, false, false);
            break;
        case DUNGEON_DUSK_FOREST:;
        case DUNGEON_TREESHROUD_FOREST:;
            // Escort Information
            if(LoadScriptVariableValueAtIndex(NULL, VAR_DUNGEON_CONQUEST_LIST, location) == false) {
                DrawEscortInfoInWindow(idx, 146, temp, MONSTER_GROVYLE, MONSTER_NONE);
            }
            break;
        case DUNGEON_DEEP_DUSK_FOREST:;
            // Escort Information
            if(LoadScriptVariableValueAtIndex(NULL, VAR_DUNGEON_CONQUEST_LIST, location) == false) {
                DrawEscortInfoInWindow(idx, 146, temp, MONSTER_GROVYLE, MONSTER_CELEBI);
            }
            break;
        case DUNGEON_BRINE_CAVE:;
            // Boss Information
            DrawBossInfoInWindow(idx, 16, temp, MONSTER_KABUTOPS, MONSTER_OMASTAR, false, false);
            // Escort Information
            if(LoadScriptVariableValueAtIndex(NULL, VAR_DUNGEON_CONQUEST_LIST, location) == false && !GetPerformanceFlagWithChecks(25)) {
                DrawEscortInfoInWindow(idx, 146, temp, MONSTER_CHATOT, MONSTER_NONE);
            }
            break;
        case DUNGEON_MIRACLE_SEA:;
            // Boss Information
            DrawBossInfoInWindow(idx, 16, temp, MONSTER_GYARADOS, MONSTER_NONE, false, false);
            // Gift Information
            if(IsDarkraiGoal()) {
                DrawGiftCheckInWindow(idx, 42, temp, MONSTER_PHIONE, GetSubXBit(29));
            }
            break;
        case DUNGEON_SPACIAL_RIFT:;
            // Boss/Gift Information
            DrawBossInfoInWindow(idx, 16, temp, MONSTER_PALKIA, MONSTER_NONE, IsDarkraiGoal(), GetSubXBit(30));
            break;
        case DUNGEON_5TH_STATION_PASS:;
            // Boss Information
            DrawBossInfoInWindow(idx, 16, temp, MONSTER_CARNIVINE, MONSTER_NONE, false, false);
            if(IsDarkraiGoal()) {
                DrawDungeonCheckInWindow(idx, 68, temp, DUNGEON_5TH_STATION_CLEARING);
            }
            break;
        case DUNGEON_8TH_STATION_PASS:;
            // Gratitude Check? Why is this not just called Sneasel's Gift?
            if(IsDarkraiGoal()) {
                preArgs.flag_vals[0] = MONSTER_SNEASEL;
                preArgs.strings[0] = GetSubXBit(65) ? checkSymbol : lockedSymbol;
                PreprocessString(temp, TR_BUFF_LEN, skyPeakEightCheck, preFlagTracker, &preArgs);
                DrawTextInWindow(idx, 128, 16, temp);
            }
            break;
        case DUNGEON_SKY_PEAK_SUMMIT:;
            // Boss Information
            DrawBossInfoInWindow(idx, 16, temp, MONSTER_MUK, MONSTER_GRIMER, false, false);
            // Gift Information
            if(IsDarkraiGoal()) {
                DrawGiftCheckInWindow(idx, 55, temp, MONSTER_SHAYMIN_LAND, GetSubXBit(46));
            }
            UpdateWindow(idx);
            return;
        case DUNGEON_MYSTIFYING_FOREST:;
            preArgs.strings[0] = (GetSubXBit(4)) ? bagSymbol : lockedSymbol;
            preArgs.number_vals[0] = 5; // Upgrade 5
            PreprocessString(temp, TR_BUFF_LEN, bagUpgradeCheck, preFlagTracker, &preArgs);
            DrawTextInWindow(idx, 15, 81, temp);
            break;
        case DUNGEON_CREVICE_CAVE:;
            // Boss Information
            DrawBossInfoInWindow(idx, 16, temp, MONSTER_FROSLASS, MONSTER_NONE, false, false);
            // Gift Information
            if(IsDarkraiGoal()) {
                DrawGiftCheckInWindow(idx, 42, temp, MONSTER_SCIZOR, GetSubXBit(47));
            }
            break;
        case DUNGEON_BOTTOMLESS_SEA:;
            // Boss/Gift Information
            DrawBossInfoInWindow(idx, 16, temp, MONSTER_KYOGRE, MONSTER_NONE, IsDarkraiGoal(), GetSubXBit(32));
            // Seven Treasure Information
            if(IsDarkraiGoal()) {
                DrawItemCheckInWindow(idx, 42, temp, ITEM_AQUA_MONICA, GetSubXBit(31));
            }
            break;
        case DUNGEON_SHIMMER_DESERT:;
            // Boss/Gift Information
            DrawBossInfoInWindow(idx, 16, temp, MONSTER_GROUDON, MONSTER_NONE, IsDarkraiGoal(), GetSubXBit(34));
            // Seven Treasure Information
            if(IsDarkraiGoal()) {
                DrawItemCheckInWindow(idx, 42, temp, ITEM_TERRA_CYMBAL, GetSubXBit(33));
            }
            break;
        case DUNGEON_MT_AVALANCHE:;
            // Boss/Gift Information
            DrawBossInfoInWindow(idx, 16, temp, MONSTER_ARTICUNO, MONSTER_NONE, IsDarkraiGoal(), GetSubXBit(36));
            // Seven Treasure Information
            if(IsDarkraiGoal()) {
                DrawItemCheckInWindow(idx, 42, temp, ITEM_ICY_FLUTE, GetSubXBit(35));
            }
            break;
        case DUNGEON_GIANT_VOLCANO:;
            // Boss/Gift Information
            DrawBossInfoInWindow(idx, 16, temp, MONSTER_HEATRAN, MONSTER_NONE, IsDarkraiGoal(), GetSubXBit(38));
            // Seven Treasure Information
            if(IsDarkraiGoal()) {
                DrawItemCheckInWindow(idx, 42, temp, ITEM_FIERY_DRUM, GetSubXBit(37));
            }
            break;
        case DUNGEON_WORLD_ABYSS:;
            // Boss/Gift Information
            DrawBossInfoInWindow(idx, 16, temp, MONSTER_GIRATINA_ALTERED, MONSTER_NONE, IsDarkraiGoal(), GetSubXBit(40));
            // Seven Treasure Information
            if(IsDarkraiGoal()) {
                DrawItemCheckInWindow(idx, 42, temp, ITEM_ROCK_HORN, GetSubXBit(39));
            }
            break;
        case DUNGEON_SKY_STAIRWAY:;
        // Boss/Gift Information
            DrawBossInfoInWindow(idx, 16, temp, MONSTER_RAYQUAZA, MONSTER_NONE, IsDarkraiGoal(), GetSubXBit(42));
            // Seven Treasure Information
            if(IsDarkraiGoal()) {
                DrawItemCheckInWindow(idx, 42, temp, ITEM_SKY_MELODICA, GetSubXBit(41));
            }
            break;
        case DUNGEON_MYSTERY_JUNGLE:;
            // Boss/Gift Information
            DrawBossInfoInWindow(idx, 16, temp, MONSTER_MEW, MONSTER_NONE, IsDarkraiGoal(), GetSubXBit(44));
            // Seven Treasure Information
            if(IsDarkraiGoal()) {
                DrawItemCheckInWindow(idx, 42, temp, ITEM_GRASS_CORNET, GetSubXBit(43));
            }
            break;
        case DUNGEON_LABYRINTH_CAVE:;
            // Boss Information
            DrawBossInfoInWindow(idx, 16, temp, MONSTER_GABITE, MONSTER_NONE, false, false);
            break;
        case DUNGEON_ICE_AEGIS_CAVE:;
            if(IsDarkraiGoal()) {
                // Conquest Information (Dungeon Completion)
                DrawDungeonCheckInWindow(idx, 16, temp, DUNGEON_ICE_AEGIS_CAVE);
                DrawDungeonCheckInWindow(idx, 29, temp, DUNGEON_REGICE_CHAMBER);
                DrawDungeonCheckInWindow(idx, 42, temp, DUNGEON_ROCK_AEGIS_CAVE);
                DrawDungeonCheckInWindow(idx, 55, temp, DUNGEON_REGIROCK_CHAMBER);
                DrawDungeonCheckInWindow(idx, 68, temp, DUNGEON_STEEL_AEGIS_CAVE);
                DrawDungeonCheckInWindow(idx, 81, temp, DUNGEON_REGISTEEL_CHAMBER);
                DrawDungeonCheckInWindow(idx, 94, temp, DUNGEON_AEGIS_CAVE_PIT);
                DrawDungeonCheckInWindow(idx, 107, temp, DUNGEON_REGIGIGAS_CHAMBER);
                // Boss/Gift Information
                DrawBossInfoInWindow(idx, 16, temp, MONSTER_REGICE, MONSTER_NONE, true, GetSubXBit(69));
                DrawBossInfoInWindow(idx, 55, temp, MONSTER_REGIROCK, MONSTER_NONE, true, GetSubXBit(70));
                DrawBossInfoInWindow(idx, 94, temp, MONSTER_REGISTEEL, MONSTER_NONE, true, GetSubXBit(71));
                DrawBossInfoInWindow(idx, 133, temp, MONSTER_REGIGIGAS, MONSTER_NONE, true, GetSubXBit(72));
            } else {
                PreprocessString(temp, TR_BUFF_LEN, checklessDungeonInfo, preFlagTracker, &preArgs);
                DrawTextInWindow(idx, 1, 146, temp);
            }
            UpdateWindow(idx);
            return;
        default:
            break;
    }
    
    // For regular dungeons.
    preArgs.strings[0] = (GetDungeonMode(location) == DMODE_OPEN_AND_REQUEST) ? checkSymbol : lockedSymbol;
    preArgs.number_vals[0] = CUSTOM_SAVE_AREA.missionStats[location].completedJobs;
    preArgs.number_vals[2] = CUSTOM_SAVE_AREA.missionStats[location].completedOutlaws;
    switch(GetDungeonCheckType(location)) {
        case DCT_LATE:;
            preArgs.number_vals[1] = apSettings.totalJobsLate;
            preArgs.number_vals[3] = apSettings.totalOutlawsLate;
            PreprocessString(temp, TR_BUFF_LEN, missionDungeonChecks, preFlagTracker, &preArgs);
            DrawTextInWindow(idx, 1, 16, temp);
            if(!IsDarkraiGoal()) {
                DrawTextInWindow(idx, 15, 146, nonEssentialExtraInfo);
            }
            break;
        case DCT_EARLY:;
            preArgs.number_vals[1] = apSettings.totalJobsEarly;
            preArgs.number_vals[3] = apSettings.totalOutlawsEarly;
            PreprocessString(temp, TR_BUFF_LEN, missionDungeonChecks, preFlagTracker, &preArgs);
            DrawTextInWindow(idx, 1, 16, temp);
            break;
        case DCT_RULE:;
            if(AreLongLocationsOn()) {
                PreprocessString(temp, TR_BUFF_LEN, ruleDungeonInfo, preFlagTracker, &preArgs);
                DrawTextInWindow(idx, 15, 16, temp);
                PreprocessString(temp, TR_BUFF_LEN, nonEssentialExtraInfo, preFlagTracker, &preArgs);
            } else {
                PreprocessString(temp, TR_BUFF_LEN, checklessDungeonInfo, preFlagTracker, &preArgs);
            }
            DrawTextInWindow(idx, 15, 146, temp);
            break;
        case DCT_OTHER:;
            PreprocessString(temp, TR_BUFF_LEN, checklessDungeonInfo, preFlagTracker, &preArgs);
            DrawTextInWindow(idx, 15, 146, temp);
            break;
    }
    
    UpdateWindow(idx);
}

// Handles deleting the top screen background.
void ApTrackerFreeTopScreenBG() {
    apTrackerWindowPtr->field_0x0 = 0;
    apTrackerWindowPtr->state = 1; // maybe try 0???
    apTrackerWindowPtr->faded = 1;
    UnkTopScreenFun3(0xFFFFFFFF);
    UnkTopScreenFun2(1);
    if(apTrackerTopScreenBG != NULL) {
        UnkTopScreenFun1(apTrackerTopScreenBG);
        MemFree(apTrackerTopScreenBG);
        apTrackerTopScreenBG = NULL;
    }
    apTrackerWindowPtr->displayable = 0;
}

uint32_t CreateTrackerTopScreen() {
    drinksDisplayed = 0;
    drinkEventsDisplayed = 0;
    updaterDelay = 0;
    trackerRotate = 0;
    trackerVelocity = 200;
    apTrackerWindowPtr = MemAlloc(sizeof(TopScreenApTrackerWindow), 0xF);
    ApTrackerFreeTopScreenBG();
    UnkTopScreenFun7(0x10);
    displayedOption = 255;
    apTrackerWindowPtr->window_id = -2;
    apTrackerWindowPtr->displayable = 1;
    apTrackerWindowPtr->closing = 0;
    apTrackerWindowPtr->state = 0;
    return 1;
}

uint32_t CloseTrackerTopScreen(uint32_t num) {
    if (num == 0) {
        if(apTrackerWindowPtr->state!= 7) {
            apTrackerWindowPtr->closing = 1;
            return 0;
        }
    }
    
    ApTrackerFreeTopScreenBG();
    apTrackerWindowPtr->state = 7;
    if(apTrackerWindowPtr->window_id != -2) {
        CloseTextBox(apTrackerWindowPtr->window_id);
        apTrackerWindowPtr->window_id = -2;
    }
    
    MemFree((void*)apTrackerWindowPtr);
    apTrackerWindowPtr = NULL;
    return 1;
}

// Name is a guess to actual purpose.
uint32_t IsReadyTrackerTopScreen() {
    if(apTrackerWindowPtr->field_0x0 == 0) {
        return apTrackerWindowPtr->faded;
    }
    
    return 1;
}

// Leftover debug function for top screen stuff maybe? All variants are just abort
// "bx lr" if not NULL.
void DebugTrackerTopScreen() {
    return;
}

// This looks like it gets run every frame. It updates the menu if need be.
// Seems to check for fades to avoid changing the top screen during a fade.
uint32_t StateManagerTrackerTopScreen() {
    switch(apTrackerWindowPtr->state) {
        case 0:;
            if (IsScreenFadeInProgress()) {
                apTrackerWindowPtr->faded = 1;
                return 0;
            }
            apTrackerWindowPtr->state = 1;
            // No break is intentional here. It should fall into case 1.
        case 1:;
            if (IsScreenFadeInProgress()) {
                apTrackerWindowPtr->faded = 1;
            } else {
                if(apTrackerWindowPtr->closing == 0) {
                    if(apTrackerWindowPtr->displayable == 0) {
                        apTrackerWindowPtr->faded = 0;
                    } else {
                        apTrackerWindowPtr->faded = 1;
                        apTrackerWindowPtr->state = 2;
                        apTrackerWindowPtr->displayable = 0;
                    }
                } else {
                    UnkTopScreenFun4(0x10);
                    apTrackerWindowPtr->faded = 1;
                    apTrackerWindowPtr->state = 6;
                }
            }
            break;
        case 2:;
            break;
        case 3:;
            break;
        case 4:;
            if(IsScreenFadeInProgress()) {
                apTrackerWindowPtr->faded = 1;
                return 0;
            }
            apTrackerWindowPtr->state = 5;
            // No break is intentional here. It should fall into case 5.
        case 5:;
            uint8_t location = CheckLocationOverrides(GetTrackerList()[CUSTOM_SAVE_AREA.trackerPage]);
            if(apTrackerWindowPtr->closing == 0 && apTrackerWindowPtr->displayable == 0) {
                if(displayedOption != CUSTOM_SAVE_AREA.trackerPage) {
                    displayedOption = CUSTOM_SAVE_AREA.trackerPage;
                    trackerVelocity = 200;
                    ApTrackerTopScreenWindowUpdate(apTrackerWindowPtr->window_id, location);
                    if(location == 255) {
                        drinksDisplayed = 0;
                        drinkEventsDisplayed = 0;
                    }                        
                    updaterDelay = 0;
                } else {
                    bool shouldSpinTracker = location == DUNGEON_DARK_CRATER;
                    if(location == DUNGEON_TEMPORAL_TOWER || location == DUNGEON_HIDDEN_LAND) {
                        if(IsDarkraiGoal()) { // If Darkrai is Goal, only spin while not completed yet.
                            shouldSpinTracker = GetDungeonMode(location) == DMODE_OPEN_AND_REQUEST;
                        } else { // If Dialga is Goal, always spin tracker.
                            shouldSpinTracker = true;
                        }
                    }
                    if(shouldSpinTracker) {
                        trackerRotate += 1 + (trackerVelocity >> 5);
                        if (trackerVelocity > 0) {
                            trackerVelocity--;
                        }
                        ApTrackerTopScreenWindowUpdate(apTrackerWindowPtr->window_id, location);
                    } else if(location == 247 || location == 254 || location == 252 || location == 250) {
                        if(updaterDelay >= 30) {
                            // There's so much on the top menu for treasure town that updating every frame causes noticeable lag :(
                            // There could be a better solution? However, the base game does use this method to delay stuff some
                            // number of frames in some places so it's probably okay.
                            ApTrackerTopScreenWindowUpdate(apTrackerWindowPtr->window_id, location);
                            updaterDelay = 0;
                        } else {
                            updaterDelay++;
                        }
                    } else if(location == 255) {
                        if(updaterDelay >= 4) {
                            updaterDelay = 0;
                            if(drinksDisplayed != apSettings.cafeDrinkMax) {
                                DrawTextInWindow(apTrackerWindowPtr->window_id, 142 + 9*(drinkEventsDisplayed & 0x7), 26 + 10*(drinkEventsDisplayed / 8),
                                    (CUSTOM_SAVE_AREA.acquiredCafeDrinkChecks > drinksDisplayed) ? checkSymbol : lockedSymbol);
                                drinksDisplayed++;
                                UpdateWindow(apTrackerWindowPtr->window_id);
                            }
                            if(drinkEventsDisplayed != apSettings.cafeEventMax) {
                                DrawTextInWindow(apTrackerWindowPtr->window_id, 142 + 9*(drinkEventsDisplayed & 0x7), 89 + 10*(drinkEventsDisplayed / 8),
                                    (CUSTOM_SAVE_AREA.acquiredCafeEventChecks > drinkEventsDisplayed) ? checkSymbol : lockedSymbol);
                                drinkEventsDisplayed++;
                                UpdateWindow(apTrackerWindowPtr->window_id);
                            }
                        }
                        updaterDelay++;
                    }
                }
                apTrackerWindowPtr->faded = 0;
            } else {
                UnkTopScreenFun4(0x10);
                apTrackerWindowPtr->state = 0;
                apTrackerWindowPtr->faded = 1;
            }
            break;
        case 6:;
            if(IsScreenFadeInProgress()) {
                return 0;
            }
            
            if(apTrackerWindowPtr->window_id != -2) {
                CloseTextBox(apTrackerWindowPtr->window_id);
                apTrackerWindowPtr->window_id = -2;
            }
            apTrackerWindowPtr->state = 7;
            // No break is intentional here. It should fall into case 7.
        case 7:;
            apTrackerWindowPtr->faded = 0;
            break;
    }
    
    return 0;
}

void InitializeTrackerTopScreen() {
    if(apTrackerWindowPtr->state != 2) {
        return;
    }
    
    apTrackerWindowPtr->faded = 1;
    ApTrackerFreeTopScreenBG();
    apTrackerTopScreenBG = MemAlloc(0x2F4, 0xF);
    LoadTopScreenBGPart1(apTrackerTopScreenBG, 0x2323D98);
    LoadTopScreenBGPart2(apTrackerTopScreenBG, "BACK/expback.bgp", 0);
    if(apTrackerWindowPtr->window_id == -2) {
        displayedOption = CUSTOM_SAVE_AREA.trackerPage;
        apTrackerWindowPtr->window_id = CreateTextBox(&trackerTopScreenWinParams, NULL);
        ApTrackerTopScreenWindowUpdate(apTrackerWindowPtr->window_id, CheckLocationOverrides(GetTrackerList()[displayedOption]));
    }
    apTrackerWindowPtr->state = 3;
}

uint32_t TrackerTopScreenFun7() {
    if(apTrackerWindowPtr->state == 3) {
        SomethingTopScreenBG(apTrackerTopScreenBG);
        UnkTopScreenFun5(1);
        apTrackerWindowPtr->field_0x0 = 1;
        apTrackerWindowPtr->state = 4;
    }
    
    // In every state, team stats appears to update the monster animations?
    
    return 0;
}

void EndTrackerTopScreen() {
    if (apTrackerWindowPtr->field_0x0 == 0 || apTrackerTopScreenBG == 0) {
        return;
    }
    
    UnkTopScreenFun8(apTrackerTopScreenBG);
    apTrackerWindowPtr->field_0x0 = 0;
    UnkTopScreenFun6(0x10);
}

TopScreenMode apTrackerMode = {
    .thing = 0xD,
    .createFunction = CreateTrackerTopScreen,
    .closeFunction = CloseTrackerTopScreen,
    .function3 = IsReadyTrackerTopScreen,
    .function4 = DebugTrackerTopScreen,
    .function5 = StateManagerTrackerTopScreen,
    .function6 = InitializeTrackerTopScreen,
    .function7 = TrackerTopScreenFun7,
    .function8 = EndTrackerTopScreen
};

void CreateTrackerMenu() {
    struct window_params pickWinParams = {.x_offset = 2, .y_offset = 2, .box_type = {0xFF}, .screen = {SCREEN_MAIN}};
    struct window_params warningWinParams = {.x_offset = 22, .y_offset = 2, .box_type = {0xFF}, .screen = {SCREEN_MAIN}, .width = 8, .height = 18};
    struct window_flags winFlags = {.b_cancel = true, .se_on = true, .set_choice = true, .menu_title = true, .menu_lower_bar = true};
    struct window_extra_info winExInfo = {.set_choice_id = CUSTOM_SAVE_AREA.trackerPage, .title_string_id = 527, .title_height = 0x10};
    // TODO: Remove warning once localizations are complete AND/OR remove it for languages with complete localizations.
    if(GetLanguage() != 0) {
        localizationWarningId = CreateTextBox(&warningWinParams, NULL);
        DrawTextInWindow(localizationWarningId, 1, 1, "Warning:[R]Localization[R]for the AP[R]Tracker is[R]incomplete.[R]Contact if[R]you'd like to[R]help.");
        UpdateWindow(localizationWarningId);
    }
    pickWindowId = CreateAdvancedMenu(&pickWinParams, winFlags, &winExInfo, ApTrackerEntryFn, GetTrackerListLength(), 8);
    if(GetTopScreenOptionType() != 5) {
        SetupGroundTopScreenFunctions(&apTrackerMode);
    }
}

void CloseTrackerMenu() {
    if (pickWindowId != -2) {
        CloseAdvancedMenu(pickWindowId);
        pickWindowId = -2;
    }
    
    if(localizationWarningId != -2) {
        CloseTextBox(localizationWarningId);
        localizationWarningId = -2;
    }
    return;
}

uint32_t UpdateTrackerMenu() {
    if(IsAdvancedMenuActive(pickWindowId) == true) {
        CUSTOM_SAVE_AREA.trackerPage = GetAdvancedMenuCurrentOption(pickWindowId);
        return 1;
    }
    
    CUSTOM_SAVE_AREA.trackerPage = GetAdvancedMenuCurrentOption(pickWindowId);
    SetupTopGroundMenuNext();
    return 4;
}

SomeMenuStruct apTrackerMenu = {
    .something = 0xD,
    .createMenuFunction = CreateTrackerMenu,
    .closeMenuFunction = CloseTrackerMenu,
    .updateMenuFunction = UpdateTrackerMenu
};

struct simple_menu_id_item newTopGroundMenuList[] = {{.string_id = 0x218, .result_value = 2},
                                 {.string_id = 0x219, .result_value = 3},
                                 {.string_id = 0x21B, .result_value = 4},
                                 {.string_id = 0x21C, .result_value = 5},
                                 {.string_id = 0x21D, .result_value = 6},
                                 {.string_id = 0x21E, .result_value = 7},
                                 {.string_id = 0x0, .result_value = 1}};

uint16_t newTopScreenOptionsList[] = {0x18A, 0x18B, 0x18C, 0x18D, 0x18E, 0x21D, 0x0};

void CreateTrackerTopScreenDungeon() {
    uint32_t regionStuff[5];
    InitBackgroundRegionDungeon(regionStuff);
    LoadBackgroundDungeon(&(TOP_SCREEN_STATUS_PTR->field107_0x80), regionStuff, 0, 1);
    SetupBackgroundDungeon(&(TOP_SCREEN_STATUS_PTR->field107_0x80));
    enum dungeon_group_id dunGroup = GetDungeonGroup(DUNGEON_PTR->id.val);
    enum dungeon_id dunId = DUNGEON_PTR->id.val;
    for(int i = 0; i < 255; i++) {
        if(GetDungeonGroup(i) == dunGroup) {
            dunId = i;
            break;
        }
    }
    trackerVelocity = 200;
    dungeonModeDisplayed = dunId;
    dungeonTopScreenId = CreateTextBox(&trackerTopScreenWinParams, NULL);
    ApTrackerTopScreenWindowUpdate(dungeonTopScreenId, CheckLocationOverrides(dunId));
}

uint32_t UpdateTrackerTopScreenDungeon() {
    uint8_t location = CheckLocationOverrides(GetTrackerList()[CUSTOM_SAVE_AREA.trackerPage]);
    bool shouldSpinTracker = location == DUNGEON_DARK_CRATER;
    if(location == DUNGEON_TEMPORAL_TOWER || location == DUNGEON_HIDDEN_LAND) {
        if(IsDarkraiGoal()) { // If Darkrai is Goal, only spin while not completed yet.
            shouldSpinTracker = GetDungeonMode(location) == DMODE_OPEN_AND_REQUEST;
        } else { // If Dialga is Goal, always spin tracker.
            shouldSpinTracker = true;
        }
    }
    if(shouldSpinTracker) {
        trackerRotate += 1 + (trackerVelocity >> 5);
        if (trackerVelocity > 0) {
            trackerVelocity--;
        }
        ApTrackerTopScreenWindowUpdate(dungeonTopScreenId, CheckLocationOverrides(dungeonModeDisplayed));
    }
    
    return 1;
}

void UnkTrackerTopScreenDungeon(uint32_t* param_1) {
    if (param_1[4] == 0) {
        return;
    }
    
    SetupBackgroundDungeon(&(TOP_SCREEN_STATUS_PTR->field107_0x80));
    param_1[4] = 0;
}

void CloseTrackerTopScreenDungeon() {
    if(dungeonTopScreenId == -2) {
        return;
    }
    
    FreeTopScreenBGDungeon(&(TOP_SCREEN_STATUS_PTR->field107_0x80));
    CloseTextBox(dungeonTopScreenId);
    dungeonTopScreenId = -2;
}

void __attribute((naked)) TopScreenDungeonModeTrackerCheck(void) {
    asm("cmp r0,#5");
    asm("ldreq r1,=CreateTrackerTopScreenDungeon");
    asm("ldreq r2,=UpdateTrackerTopScreenDungeon");
    asm("ldreq r3,=UnkTrackerTopScreenDungeon");
    asm("ldreq r12,=CloseTrackerTopScreenDungeon");
    asm("streq r12,[sp,#0x0]");
    asm("addeq r0,r4,#0x14");
    asm("bleq  AssignTopScreenHandlers");
    asm("cmp r0,#6");
    asm("ldreq r1,=CreateMode5TopScreenDungeon");
    asm("ldreq r2,=UpdateMode5TopScreenDungeon");
    asm("ldreq r3,=UnkMode5TopScreenDungeon");
    asm("ldreq r12,=CloseMode5TopScreenDungeon");
    asm("streq r12,[sp,#0x0]");
    asm("addeq r0,r4,#0x14");
    asm("bleq  AssignTopScreenHandlers");
    asm("b TopScreenDungeonModeTrackerUnhook");
}

void __attribute((naked)) SetTopScreenTypeDungeonCheck(void) {
    asm("cmp r0,#5");
    asm("bleq TopScreenModeSetDungeonMode");
    asm("b SetTopScreenTypeDungeonUnhook");
}

void __attribute((naked)) ApTrackerSetupMenuCheck (void) {
    asm("ldr r0,=apTrackerMenu");
    asm("bl SetupMenuNext");
    asm("mov r0,#0x4");
    asm("ldmia sp!,{r3,pc}");
}

void __attribute((naked)) ApTrackerTopScreenCheck (void) {
    asm("cmp r0,#0x5");
    asm("bne ApTrackerTopScreenUnhook");
    asm("ldr r0,=apTrackerMode");
    asm("bl SetupGroundTopScreenFunctions");
    asm("ldmia sp!,{r3,pc}");
}

void __attribute((naked)) ApTrackerTopScreenCheck2 (void) {
    asm("cmp r0,#0x5");
    asm("bne ApTrackerTopScreenUnhook2");
    asm("ldr r0,=apTrackerMode");
    asm("bl SetupGroundTopScreenFunctions2");
    asm("ldmia sp!,{r3,pc}");
}
