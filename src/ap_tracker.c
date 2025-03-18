#include <pmdsky.h>
#include <cot.h>
#include <ap_utils.h>
#include "extern.h"

typedef struct ApTrackerTracker {
    int displayWindowId;
    int pickWindowId;
    int previousOption;
    int optionState;
} ApTrackerTracker;

ApTrackerTracker tracker = {.displayWindowId = -2, .pickWindowId = -2, .previousOption = 0};

typedef struct SomeMenuStruct {
    uint32_t something; // For submenus this is 0xD?
    void (*createMenuFunction)(void); // is null for some menus and actual init is handled in update.
    void (*closeMenuFunction)(void);
    uint32_t (*updateMenuFunction)(void);
} SomeMenuStruct;

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
        DUNGEON_BEACH_CAVE:;
            if(!GetSubXBit(0)) { // Bag Upgrade 0
                return false;
            }
            break;
        DUNGEON_MT_BRISTLE:;
            if(!GetSubXBit(1)) { // Bag Upgrade 1
                return false;
            }
            break;
        DUNGEON_APPLE_WOODS:;
            if(!GetSubXBit(2)) { // Bag Upgrade 2
                return false;
            }
            break;
        DUNGEON_MYSTIFYING_FOREST:; 
            if(!GetSubXBit(2)) { // Bag Upgrade 3
                return false;
            }
            break;
        DUNGEON_CREVICE_CAVE:; 
            if(!GetSubXBit(47)) { // Secret Rank
                return false;
            }
            break;
        default:
            return true;
    }
}

char* townSymbol = "[M:S7]";
char* lockedSymbol = "[M:S4]";
char* completeSymbol = "[M:S3]";
char* unlockedSymbol = "[M:R7]";
char* remainingChecksSymbol = "[M:R4]";

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
    preArgs.strings[0] = dungeonSymbol;
    
    // Flags = 0xD5
    struct preprocessor_flags preFlags = {.timer_1 = true, .flags_1 = 0x6A};
    PreprocessString(buffer, 0x400, "[string0][CLUM_SET:26][dungeon:0]", preFlags, &preArgs);
}

void CreateTrackerMenu() {
    struct window_params viewerWinParams = {.x_offset = 2, .y_offset = 2, .width = 28,
        .height = 20, .box_type = {0xFF}, .screen = {SCREEN_SUB}};
    struct window_params pickWinParams = {.x_offset = 2, .y_offset = 20, .width = 28,
        .height = 2, .box_type = {0xFF}, .screen = {SCREEN_MAIN}};
    struct window_flags winFlags = {.b_cancel = true, .se_on = true, .set_choice = true, .menu_title = true, .menu_lower_bar = true};
    struct window_extra_info winExInfo = {.set_choice_id = tracker.optionState, .title_string_id = 527};
    tracker.displayWindowId = CreateTextBox(&viewerWinParams, NULL);
    tracker.pickWindowId = CreateAdvancedMenu(&pickWinParams, winFlags, &winExInfo, sizeof(trackerLocationDungeonIds)/sizeof(trackerLocationDungeonIds[0]), )
}

void CloseTrackerMenu() {
    CloseTextBox(tracker.displayWindowId);
    return;
}

uint32_t UpdateTrackerMenu() {
    if(IsOptionsMenuActive(tracker.optionWindowId) == true) {
        return 1;
    } else {
        SetupTopGroundMenuNext();
        return 4;
    }
}

SomeMenuStruct apTrackerMenu = {
    .something = 0xD,
    .createMenuFunction = CreateTrackerMenu,
    .closeMenuFunction = CloseTrackerMenu,
    .updateMenuFunction = UpdateTrackerMenu
};

void __attribute((naked)) ApTrackerSetupFromTopMenu (void) {
    asm("ldr r0,=apTrackerMenu");
    asm("bl SetupMenuNext");
    asm("mov r0,#0x4");
    asm("ldmia sp!,{r3,pc}");
}