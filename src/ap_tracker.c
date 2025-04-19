#include <pmdsky.h>
#include <cot.h>
#include <ap_utils.h>
#include "extern.h"

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

typedef struct ApTrackerTracker {
    int pickWindowId;
    int optionState;
} ApTrackerTracker;

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
signed char dungeonTopScreenId = -2;
uint8_t dungeonModeDisplayed = 255;
uint32_t trackerRotate = 0;
uint32_t trackerVelocity = 0;

uint8_t trackerLocationDungeonIds[] = {
    // General
    247, // Town
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
    38,  // Hidden Land
    41,  // Temporal Tower
    // Late
    44,  // Mystifying Forest
    46,  // Blizzard Island
    47,  // Crevice Cave
    50,  // Surrounded Sea
    51,  // Miracle Sea
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
    174, // Star Cave
    // Special Episodes TODO
    // WISH
    // PRODIGY
    // OH MY GOSH
    // HERE COMES TEAM CHARM
    // IN THE FUTURE OF DARKNESS
    // Goals (TT)
    67   // Dark Crater
};

bool IsLocationBonusChecksComplete(enum dungeon_id location) {
    switch (location) {
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
    
    return true;
}

char* townSymbol = "[M:S7][S:8]";
char* lockedSymbol = "[M:S4][S:8]";
char* completeSymbol = "[M:S3][S:8]";
char* unlockedSymbol = "[M:R7][S:8]";
char* remainingChecksSymbol = "[M:R4][S:8]";
char* checkSymbol = "[M:S2][S:8]";
char* instrumentSymbol = "[M:R9][S:8]";
char* relicSymbol = "[M:T4][S:8]";

// When the player selects the tracker option from the menu, open a menu to
// allow them to alter the top screen.
char* ApTrackerEntryFn(char* buffer, int option_id) {
    struct preprocessor_args preArgs;
    uint8_t location = trackerLocationDungeonIds[option_id];
    char* locationSymbol;
    if (location == 247) {
        locationSymbol = townSymbol;
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
    preArgs.id_vals[0] = trackerLocationDungeonIds[option_id];
    preArgs.strings[0] = locationSymbol;
    
    struct preprocessor_flags preFlags = {.timer_1 = true, .flags_1 = 0x6A};
    if(GetDungeonMode(location) == DMODE_OPEN_AND_REQUEST) {
        PreprocessString(buffer, 0x400, "[string0][CLUM_SET:26][dungeon:0]", preFlags, &preArgs);
    } else {
        PreprocessString(buffer, 0x400, "[string0][CS:B][CLUM_SET:26][dungeon:0][CR]", preFlags, &preArgs);
    }
    
    return buffer;
}

void DrawCircleBarInTextBox(signed char idx, int radius, int centerX, int centerY, int toGet, int gotten, char* strLock, char* strUnlocked, int rotation) {
    if (toGet == 0) {
        return;
    }
    
    uint32_t step = _u32_div_f(4096, toGet);
    
    for(int i = 0; i < toGet; i++) {
        int x = ((int)TRIG_TABLE[0xFFF & (step * i + rotation)].cos * radius) >> 12;
        int y = ((int)TRIG_TABLE[0xFFF & (step * i + rotation)].sin * radius) >> 12;
        if(i < gotten) {
            DrawTextInWindow(idx, centerX + x, centerY + y, strLock);
        } else {
            DrawTextInWindow(idx, centerX + x, centerY + y, strUnlocked);
        }
    }
}

char* genericDungeon = "[CLUM_SET:15]Completed: [CLUM_SET:70][string:0]\n"
                       "[CLUM_SET:15]Jobs: [CS:C][CLUM_SET:70][value:0:1]/[value:1:1][CR]\n"
                       "[CLUM_SET:15]Outlaws: [CS:C][CLUM_SET:70][value:2:1]/[value:3:1][CR]";
char* shopItemString1 = "[CLUM_SET:128]Shop Item 1: [CLUM_SET:199][string:0]\n"
                        "[CLUM_SET:128]Shop Item 2: [CLUM_SET:199][string:1]\n"
                        "[CLUM_SET:128]Shop Item 3: [CLUM_SET:199][string:2]\n"
                        "[CLUM_SET:128]Shop Item 4: [CLUM_SET:199][string:3]\n"
                        "[CLUM_SET:128]Shop Item 5: [CLUM_SET:199][string:4]\n";
char* shopItemString2 = "[CLUM_SET:128]Shop Item 6: [CLUM_SET:199][string:0]\n"
                        "[CLUM_SET:128]Shop Item 7: [CLUM_SET:199][string:1]\n"
                        "[CLUM_SET:128]Shop Item 8: [CLUM_SET:199][string:2]\n"
                        "[CLUM_SET:128]Shop Item 9: [CLUM_SET:199][string:3]\n"
                        "[CLUM_SET:128]Shop Item 10: [CLUM_SET:199][string:4]";
char* specialEpisodeString = "[CLUM_SET:15]Bidoof SE: [string:0]\n"
                             "[CLUM_SET:15]Sunflora SE: [string:1]\n"
                             "[CLUM_SET:15]Wigglytuff SE: [string:2]\n"
                             "[CLUM_SET:15]Team Charm SE: [string:3]";
char* townExtras = "[CLUM_SET:15][CS:C]Must beat [CR][CS:G]Beach Cave[CR]\n"
                   "[CLUM_SET:15][CS:C]& Talk To Wigglytuff[CR]\n"
                   "[CLUM_SET:15]Bag Check: [string:0]\n"
                   "[CLUM_SET:15]Team Name Check: [string:0]";
char* beachCaveExtraInfo = "Unlocked from the start.";
char* nonEssentialExtraInfo = "Non-essential for Completion.";
char* fractionString = "[value:0:1]/[value:1:1]";
struct window_params trackerTopScreenWinParams = {.x_offset = 2, .y_offset = 2, .width = 0x1C, .height = 0x14, .screen = {.val = SCREEN_SUB}, .box_type = {.val = 0xFF}};
void ApTrackerTopScreenWindowUpdate(int idx, uint32_t location) {
    ClearWindow(idx);
    char temp[300];
    struct preprocessor_args preArgs = {.id_vals[0] = location};
    struct preprocessor_flags preFlags = {};
    PreprocessString(temp, 300, "Tracker: [dungeon:0]", preFlags, &preArgs);
    DrawTextInWindow(idx, (trackerTopScreenWinParams.width * 8 - GetStringWidth(temp)) / 2, 2, temp);
    displayedOption = CUSTOM_SAVE_AREA.trackerPage;
    if (location == 247) { // Treasure Town
        preArgs.strings[0] = (GetSubXBit(10)) ? checkSymbol : lockedSymbol;
        preArgs.strings[1] = (GetSubXBit(11)) ? checkSymbol : lockedSymbol;
        preArgs.strings[2] = (GetSubXBit(12)) ? checkSymbol : lockedSymbol;
        preArgs.strings[3] = (GetSubXBit(13)) ? checkSymbol : lockedSymbol;
        preArgs.strings[4] = (GetSubXBit(14)) ? checkSymbol : lockedSymbol;
        PreprocessString(temp, 300, shopItemString1, preFlags, &preArgs);
        DrawTextInWindow(idx, 1, 16, temp);
        preArgs.strings[0] = (GetSubXBit(15)) ? checkSymbol : lockedSymbol;
        preArgs.strings[1] = (GetSubXBit(16)) ? checkSymbol : lockedSymbol;
        preArgs.strings[2] = (GetSubXBit(17)) ? checkSymbol : lockedSymbol;
        preArgs.strings[3] = (GetSubXBit(18)) ? checkSymbol : lockedSymbol;
        preArgs.strings[4] = (GetSubXBit(19)) ? checkSymbol : lockedSymbol;
        PreprocessString(temp, 300, shopItemString2, preFlags, &preArgs);
        DrawTextInWindow(idx, 1, 81, temp);
        preArgs.strings[0] = (GetSubXBit(5)) ? checkSymbol : lockedSymbol;
        preArgs.strings[1] = (GetSubXBit(6)) ? checkSymbol : lockedSymbol;
        preArgs.strings[2] = (GetSubXBit(7)) ? checkSymbol : lockedSymbol;
        preArgs.strings[3] = (GetSubXBit(8)) ? checkSymbol : lockedSymbol;
        PreprocessString(temp, 300, specialEpisodeString, preFlags, &preArgs);
        DrawTextInWindow(idx, 1, 16, temp);
        preArgs.strings[0] = (GetSubXBit(0)) ? checkSymbol : lockedSymbol;
        preArgs.strings[1] = (GetSubXBit(127)) ? checkSymbol : lockedSymbol;
        PreprocessString(temp, 300, townExtras, preFlags, &preArgs);
        DrawTextInWindow(idx, 1, 81, temp);
        UpdateWindow(idx);
        return;
    } else if(location == DUNGEON_HIDDEN_LAND || location == DUNGEON_TEMPORAL_TOWER) {
        if(GetDungeonMode(location) != DMODE_OPEN_AND_REQUEST) {
            DrawCircleBarInTextBox(idx, 60, 112, 84, macguffinMaxes.requiredRelicFragmentShards, CUSTOM_SAVE_AREA.acquiredRelicFragmentShards, lockedSymbol, relicSymbol, trackerRotate);
            preArgs.number_vals[0] = CUSTOM_SAVE_AREA.acquiredRelicFragmentShards;
            preArgs.number_vals[1] = macguffinMaxes.requiredRelicFragmentShards;
            PreprocessString(temp, 300, fractionString, preFlags, &preArgs);
            DrawTextInWindow(idx, (trackerTopScreenWinParams.width * 8 - GetStringWidth(temp)) / 2, 84, temp);
            UpdateWindow(idx);
            return;
        }
    } else if(location == DUNGEON_DARK_CRATER) {
        DrawCircleBarInTextBox(idx, 60, 109, 84, macguffinMaxes.requiredInstruments, CUSTOM_SAVE_AREA.acquiredInstruments, lockedSymbol, instrumentSymbol, trackerRotate);
        preArgs.number_vals[0] = CUSTOM_SAVE_AREA.acquiredInstruments;
        preArgs.number_vals[1] = macguffinMaxes.requiredInstruments;
        PreprocessString(temp, 300, fractionString, preFlags, &preArgs);
        DrawTextInWindow(idx, (trackerTopScreenWinParams.width * 8 - GetStringWidth(temp)) / 2, 84, temp);
        UpdateWindow(idx);
        return;
    } else if(location == DUNGEON_BEACH_CAVE) {
    }
    
    // For regular dungeons.
    if (GetDungeonMode(location) == DMODE_OPEN_AND_REQUEST) {
        preArgs.strings[0] = checkSymbol;
    } else {
        preArgs.strings[0] = lockedSymbol;
    }
    preArgs.number_vals[0] = CUSTOM_SAVE_AREA.missionStats[location].completedJobs;
    preArgs.number_vals[2] = CUSTOM_SAVE_AREA.missionStats[location].completedOutlaws;
    if(IsDungeonLateGame(location)) {
        preArgs.number_vals[1] = missionMaxes.totalJobsLate;
        preArgs.number_vals[4] = missionMaxes.totalOutlawsLate;
    } else {
        preArgs.number_vals[1] = missionMaxes.totalJobsEarly;
        preArgs.number_vals[4] = missionMaxes.totalOutlawsEarly;
    }
    PreprocessString(temp, 300, genericDungeon, preFlags, &preArgs);
    DrawTextInWindow(idx, 1, 16, temp);
    
    UpdateWindow(idx);
}

// Handles deeleting the top screen background.
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
    trackerRotate = 0;
    trackerVelocity = 10;
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
            if(apTrackerWindowPtr->closing == 0 && apTrackerWindowPtr->displayable == 0) {
                if(displayedOption != CUSTOM_SAVE_AREA.trackerPage) {
                    trackerVelocity = 10;
                    ApTrackerTopScreenWindowUpdate(apTrackerWindowPtr->window_id, trackerLocationDungeonIds[displayedOption]);
                } else if (trackerLocationDungeonIds[displayedOption] == DUNGEON_TEMPORAL_TOWER || trackerLocationDungeonIds[displayedOption] == DUNGEON_DARK_CRATER) {
                    trackerRotate += 1 + (trackerVelocity >> 5);
                    if (trackerVelocity > 0) {
                        trackerVelocity--;
                    }
                    ApTrackerTopScreenWindowUpdate(apTrackerWindowPtr->window_id, trackerLocationDungeonIds[displayedOption]);
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
        ApTrackerTopScreenWindowUpdate(apTrackerWindowPtr->window_id, trackerLocationDungeonIds[displayedOption]);
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
    struct window_flags winFlags = {.b_cancel = true, .se_on = true, .set_choice = true, .menu_title = true, .menu_lower_bar = true};
    struct window_extra_info winExInfo = {.set_choice_id = CUSTOM_SAVE_AREA.trackerPage, .title_string_id = 527, .title_height = 0x10};
    pickWindowId = CreateAdvancedMenu(&pickWinParams, winFlags, &winExInfo, ApTrackerEntryFn, sizeof(trackerLocationDungeonIds)/sizeof(trackerLocationDungeonIds[0]), 8);
    if(GetTopScreenOptionType() != 5) {
        SetupGroundTopScreenFunctions(&apTrackerMode);
    }
}

void CloseTrackerMenu() {
    CloseTextBox(pickWindowId);
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
    trackerVelocity = 10;
    dungeonModeDisplayed = dunId;
    dungeonTopScreenId = CreateTextBox(&trackerTopScreenWinParams, NULL);
    ApTrackerTopScreenWindowUpdate(dungeonTopScreenId, dunId);
}

uint32_t UpdateTrackerTopScreenDungeon() {
    if(dungeonModeDisplayed == DUNGEON_DARK_CRATER || dungeonModeDisplayed == DUNGEON_HIDDEN_LAND) {
        trackerRotate += 1 + (trackerVelocity >> 5);
        if (trackerVelocity > 0) {
            trackerVelocity--;
        }
        ApTrackerTopScreenWindowUpdate(dungeonTopScreenId, dungeonModeDisplayed);
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

void __attribute((naked)) SetTopScreenTypeGroundCheck(void) {
    asm("cmp r0,#5");
    asm("bleq TopScreenModeSetDungeonMode");
    asm("b SetTopScreenTypeGroundUnhook");
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