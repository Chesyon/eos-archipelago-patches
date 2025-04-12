#include <pmdsky.h>
#include <cot.h>
#include <ap_utils.h>
#include "extern.h"

typedef struct TopScreenApTrackerWindow {
    uint8_t field_0x0;
    uint8_t padding1; // ?
    uint8_t padding2; // ?
    uint8_t padding3; // ?
    uint32_t state; // Init to 1 for Team Stat, 0 for Message Log?
    uint8_t field_0x8; // Init to 0?
    uint8_t field_0x9; // Init to 1?
    uint8_t field_0xA; // Init to 0?
    uint8_t field_0xB; // Init to 0?
} TopScreenApTrackerWindow;

TopScreenApTrackerWindow *apTrackerWindowPtr = NULL;
void* apTrackerTopScreenBG = NULL;

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

// Handles deeleting the top screen background probably.
void ApTrackerTopScreenUtil() {
    apTrackerWindowPtr->field_0x0 = 0;
    apTrackerWindowPtr->state = 1; // maybe try 0???
    apTrackerWindowPtr->field_0x9 = 1;
    UnkTopScreenFun3(0xFFFFFFFF);
    UnkTopScreenFun2(1);
    if(apTrackerTopScreenBG != NULL) {
        UnkTopScreenFun1(apTrackerTopScreenBG);
        MemFree(apTrackerTopScreenBG);
        apTrackerTopScreenBG = NULL;
    }
    apTrackerWindowPtr->field_0xA = 0;
}

uint32_t TrackerTopScreenCreate() {
    apTrackerWindowPtr = MemAlloc(sizeof(TopScreenApTrackerWindow), 0xF);
    ApTrackerTopScreenUtil();
    UnkTopScreenFun7(0x10);
    apTrackerWindowPtr->field_0x8 = 0;
    apTrackerWindowPtr->field_0xA = 1;
    apTrackerWindowPtr->field_0xB = 0;
    apTrackerWindowPtr->state = 0;
    return 1;
}

uint32_t TrackerTopScreenDelete(uint32_t num) {
    if (num == 0) {
        num = apTrackerWindowPtr->state;
        if(num != 7) {
            apTrackerWindowPtr->field_0xB = 1;
            return 0;
        }
    }
    
    ApTrackerTopScreenUtil();
    apTrackerWindowPtr->state = 7;
    if(apTrackerWindowPtr->field_0x8 != 0) {
        // TODO: Something goes here? Related to freeing windows.
    }
    
    MemFree((void*)apTrackerWindowPtr);
    apTrackerWindowPtr = NULL;
    return 1;
}

uint32_t TrackerTopScreenFun3() {
    if(apTrackerWindowPtr->field_0x0 == 0) {
        return apTrackerWindowPtr->field_0x9;
    }
    
    return 1;
}

// Leftover debug function for top screen stuff maybe? All variants are just abort
// bx lr.
void TrackerTopScreenFun4() {
    return;
}

// This looks like it gets run every frame. It updates the menu if need be.
// Seems to check for fades to avoid changing the top screen during a fade.
uint32_t TrackerTopScreenFun5() {
    switch(apTrackerWindowPtr->state) {
        case 0:;
            if (IsScreenFadeInProgress()) {
                apTrackerWindowPtr->field_0x9 = 1;
                return 0;
            }
            apTrackerWindowPtr->state = 1;
            // No break is intentional here. It should fall into case 1.
        case 1:;
            if (IsScreenFadeInProgress()) {
                apTrackerWindowPtr->field_0x9 = 1;
            } else {
                if(apTrackerWindowPtr->field_0xB == 0) {
                    if(apTrackerWindowPtr->field_0xA == 0) {
                        apTrackerWindowPtr->field_0x9 = 0;
                    } else {
                        apTrackerWindowPtr->field_0x9 = 1;
                        apTrackerWindowPtr->state = 2;
                        apTrackerWindowPtr->field_0xA = 0;
                    }
                } else {
                    UnkTopScreenFun4(0x10);
                    apTrackerWindowPtr->field_0x9 = 1;
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
                apTrackerWindowPtr->field_0x9 = 1;
                return 0;
            }
            apTrackerWindowPtr->state = 5;
            // No break is intentional here. It should fall into case 5.
        case 5:;
            if(apTrackerWindowPtr->field_0xB == 0 && apTrackerWindowPtr->field_0xA == 0) {
                // TODO: Something to update the team stats goes here. However,
                // for message log nothing is here/
                apTrackerWindowPtr->field_0x9 = 0;
            } else {
                UnkTopScreenFun4(0x10);
                apTrackerWindowPtr->state = 0;
                apTrackerWindowPtr->field_0x9 = 1;
            }
            break;
        case 6:;
            if(IsScreenFadeInProgress()) {
                return 0;
            }
            
            if(apTrackerWindowPtr->field_0x8 != 0) {
                // TODO: Add something here to delete our windows.
                apTrackerWindowPtr->field_0x8 = 0;
            }
            apTrackerWindowPtr->state = 7;
            // No break is intentional here. It should fall into case 7.
        case 7:;
            apTrackerWindowPtr->field_0x9 = 0;
            break;
    }
    
    return 0;
}

void TrackerTopScreenFun6() {
    if(apTrackerWindowPtr->state != 2) {
        return;
    }
    
    apTrackerWindowPtr->field_0x9 = 1;
    ApTrackerTopScreenUtil();
    apTrackerTopScreenBG = MemAlloc(0x2F4, 0xF);
    LoadTopScreenBGPart1(apTrackerTopScreenBG, 0x2323D98);
    LoadTopScreenBGPart2(apTrackerTopScreenBG, "BACK/expback.bgp", 0);
    if(apTrackerWindowPtr->field_0x8 == 0) {
        // TODO: Create the window and do initial update if applicable.
        apTrackerWindowPtr->field_0x8 = 1;
    } // TeamStatMenu has an else here to update stats every frame??
    apTrackerWindowPtr->state = 3;
}

uint32_t TrackerTopScreenFun7() {
    if(apTrackerWindowPtr->state == 3) {
        SomethingTopScreenBG(apTrackerTopScreenBG);
        UnkTopScreenFun5(1);
        apTrackerWindowPtr->field_0x0 = 1;
        apTrackerWindowPtr->state = 4;
    }
    
    return 0;
}

void TrackerTopScreenFun8() {
    if (apTrackerWindowPtr->field_0x0 == 0 || apTrackerTopScreenBG == 0) {
        return;
    }
    
    UnkTopScreenFun8(apTrackerTopScreenBG);
    apTrackerWindowPtr->field_0x0 = 0;
    UnkTopScreenFun6(0x10);
}