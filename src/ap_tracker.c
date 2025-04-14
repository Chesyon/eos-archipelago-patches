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

TopScreenApTrackerWindow *apTrackerWindowPtr = NULL;
void* apTrackerTopScreenBG = NULL;
uint8_t displayedOption = 255;

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
    41,  // Temporal Tower
    67   // Dark Crater
};

bool IsLocationBonusChecksComplete(enum dungeon_id location) {
    switch (location) {
        case DUNGEON_BEACH_CAVE:;
            if(!GetSubXBit(0)) { // Bag Upgrade 0
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
        case DUNGEON_MYSTIFYING_FOREST:; 
            if(!GetSubXBit(2)) { // Bag Upgrade 3
                return false;
            }
            break;
        case DUNGEON_CREVICE_CAVE:; 
            if(!GetSubXBit(47)) { // Secret Rank
                return false;
            }
            break;
        default:
            break;
    }
    
    return true;
}

char* townSymbol = "[M:S7]";
char* lockedSymbol = "[M:S4]";
char* completeSymbol = "[M:S3]";
char* unlockedSymbol = "[M:R7]";
char* remainingChecksSymbol = "[M:R4]";
char* checkSymbol = "[M:S2]";

// When the player selects the tracker option from the menu, open a menu to
// allow them to alter the top screen.
char* ApTrackerEntryFn(char* buffer, int option_id) {
    struct preprocessor_args preArgs;
    uint8_t location = trackerLocationDungeonIds[0];
    char* locationSymbol;
    if (location == 247) {
        locationSymbol = townSymbol;
    } else {
        enum dungeon_mode dmode = GetDungeonMode(location) ;
        switch (dmode) {
            default:
            case DMODE_CLOSED:
            case DMODE_REQUEST:
                locationSymbol = lockedSymbol;
                break;
            case DMODE_OPEN:
                locationSymbol = unlockedSymbol;
                break;
            case DMODE_OPEN_AND_REQUEST:
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
    preArgs.id_vals[0] = trackerLocationDungeonIds[0];
    preArgs.strings[0] = locationSymbol;
    
    // Flags = 0xD5
    struct preprocessor_flags preFlags = {.timer_1 = true, .flags_1 = 0x6A};
    PreprocessString(buffer, 0x400, "[string0][CLUM_SET:26][dungeon:0]", preFlags, &preArgs);
    
    return buffer;
}

/*void CreateTrackerMenu() {
    struct window_params pickWinParams = {.x_offset = 2, .y_offset = 20, .width = 28,
        .height = 2, .box_type = {0xFF}, .screen = {SCREEN_MAIN}};
    struct window_flags winFlags = {.b_cancel = true, .se_on = true, .set_choice = true, .menu_title = true, .menu_lower_bar = true};
    struct window_extra_info winExInfo = {.set_choice_id = tracker.optionState, .title_string_id = 527};
    pickWindowId = CreateAdvancedMenu(&pickWinParams, winFlags, &winExInfo, &ApTrackerEntryFn, sizeof(trackerLocationDungeonIds)/sizeof(trackerLocationDungeonIds[0]), 6);
} */

uint8_t circlePoints20[] = {60, 120,
                            79, 117,
                            95, 109,
                            109, 95,
                            117, 79,
                            120, 60,
                            117, 41,
                            109, 25,
                            95, 11,
                            79, 3,
                            60, 0,
                            41, 3,
                            25, 11,
                            11, 25,
                            3, 41,
                            0, 60,
                            3, 79,
                            11, 95,
                            25, 109,
                            41, 117};

char* genericDungeon = "[CLUM_SET:15]Completed: [CLUM_SET:60][string:0]"
                       "[CLUM_SET:15]Jobs: [CLUM_SET:60][value:0:2]/[value:1:2]"
                       "[CLUM_SET:15]Outlaws: [CLUM_SET:60][value:3:2]/[value:4:2]";
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
char* specialEpisodeString = "[CLUM_SET:15]Bidood SE: [string:0]\n"
                             "[CLUM_SET:15]Bunflora SE: [string:1]\n"
                             "[CLUM_SET:15]Wigglytuff SE: [string:2]\n"
                             "[CLUM_SET:15]Team Charm SE: [string:3]";
char* townExtras = "[CLUM_SET:15][CS:C]Must beat [CS:G]Beach Cave[CS:C]:[CR]\n"
                   "[CLUM_SET:15]Bag Check: [string:0]\n"
                   "[CLUM_SET:15]Team Name Check: [string:0]";
struct window_params trackerTopScreenWinParams = {.x_offset = 2, .y_offset = 2, .width = 0x1C, .height = 0x14, .screen = {.val = SCREEN_SUB}, .box_type = {.val = 0xFF}};
void ApTrackerTopScreenWindowCallback(int idx) {
    uint8_t location = trackerLocationDungeonIds[CUSTOM_SAVE_AREA.trackerPage];
    char temp[400];
    struct preprocessor_args preArgs = {.id_vals[0] = location};
    struct preprocessor_flags preFlags = {};
    PreprocessString(temp, 300, "Tracker: [dungeon:0]", preFlags, &preArgs);
    DrawTextInWindow(idx, (trackerTopScreenWinParams.width * 8 - GetStringWidth(temp)) / 2, 2, temp);
    
    if(displayedOption == CUSTOM_SAVE_AREA.trackerPage) {
        return;
    }
    
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
    } else if(location == 41) { // Temporal Tower
        int startX = 48;
        int startY = 24;
        for(int i = 0; i < 20; i++) {
            DrawTextInWindow(idx, startX + circlePoints20[i * 2], startY + circlePoints20[i * 2 + 1] , lockedSymbol);
        }
        
    } else if(location == 67) { // Dark Crater

    }
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
                // TODO: Something to update the team stats goes here. However,
                // for message log nothing is here/
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
        apTrackerWindowPtr->window_id = CreateTextBox(&trackerTopScreenWinParams, ApTrackerTopScreenWindowCallback);
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