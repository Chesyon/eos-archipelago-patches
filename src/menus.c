#include <pmdsky.h>
#include <cot.h>
#include <ap_utils.h>
#include "extern.h"

// Custom script menus are disabled by default.
// Refer to README.md for more information.

#if CUSTOM_SCRIPT_MENUS

// This is not redundant. The length of the line is calculated wrong if not done.
char* noMissionChecksString = "[S:13]";
char* halfMissionChecksString = "[M:R1][S:8]";
char* fullMissionChecksString = "[M:S3][S:5]";

char* TailoredDungeonMissionMenuEntryFn(char* buffer, int option_id) {
    uint8_t *dungeonIdArray = GLOBAL_MENU_INFO.extra_info_ptr;
    uint32_t dungeonId = dungeonIdArray[option_id];
    
    int completedHalves = 0;
    char* dungeonSymbol = noMissionChecksString;
    uint32_t regularJobMax;
    uint32_t outlawJobMax;
    enum dungeon_check_type dungeonCt = GetDungeonCheckType(dungeonId);
    switch(dungeonCt) {
        case DCT_EARLY:;
            regularJobMax = newApSettings.nums.totalJobsEarly;
            if(regularJobMax > 0) {
                if(regularJobMax <= CUSTOM_SAVE_AREA.missionStats[dungeonId].completedJobs) {
                    completedHalves++;
                }
            }
            outlawJobMax = newApSettings.nums.totalOutlawsEarly;
            if(outlawJobMax > 0) {
                if(outlawJobMax <= CUSTOM_SAVE_AREA.missionStats[dungeonId].completedOutlaws) {
                    completedHalves++;
                }
            }
            break;
        case DCT_LATE:;
             regularJobMax = newApSettings.nums.totalJobsLate;
            if(regularJobMax > 0) {
                if(regularJobMax <= CUSTOM_SAVE_AREA.missionStats[dungeonId].completedJobs) {
                    completedHalves++;
                }
            }
           outlawJobMax = newApSettings.nums.totalOutlawsLate;
            if(outlawJobMax > 0) {
                if(outlawJobMax <= CUSTOM_SAVE_AREA.missionStats[dungeonId].completedOutlaws) {
                    completedHalves++;
                }
            }
            break;
        case DCT_RULE:;
        case DCT_OTHER:;
            break;
    }
    
    if (dungeonCt == DCT_EARLY || dungeonCt == DCT_LATE) {
        if (completedHalves == 1) {
            dungeonSymbol = halfMissionChecksString;
        } else if (completedHalves >= 2) {
            dungeonSymbol = fullMissionChecksString;
        }
    } else {
        dungeonSymbol = fullMissionChecksString;
    }
    
    struct preprocessor_args preArgs;
    preArgs.id_vals[0] = dungeonId;
    preArgs.strings[0] = dungeonSymbol;
    
    // Flags = 0xD5
    struct preprocessor_flags preFlags = {.timer_1 = true, .flags_1 = 0x6A};
    PreprocessString(buffer, 0x400, "[string0][CLUM_SET:26][dungeon:0]", preFlags, &preArgs);
    
    return buffer;
}

void CreateTailoredDungeonMissionMenu(void) {
    GLOBAL_MENU_INFO.state = 0;
    struct window_params winParams = {.x_offset = 2, .y_offset = 2, .box_type = {0xFF}, .screen = {SCREEN_MAIN}};
    struct window_flags winFlags = {.a_accept = true, .b_cancel = true, .se_on = true, .menu_title = true, .menu_lower_bar = true};
    struct window_extra_info winExInfo = {.title_string_id = 176, .title_height = 0x10};
    
    GLOBAL_MENU_INFO.extra_info_ptr = MemAlloc(0xB4, 0xF);
    
    int numDungeonsToPick = 0;
    for(uint32_t i = 0; i < 0xB4; i++) {
        if(GetDungeonMode(i) == DMODE_OPEN_AND_REQUEST && IsLookaLikeDungeonPlaceholder(i) == false) {
            ((uint8_t*)GLOBAL_MENU_INFO.extra_info_ptr)[numDungeonsToPick] = i;
            numDungeonsToPick++;
        }
    }
    
    // There are no dungeons to open a menu for. Bail building the menu.
    if (numDungeonsToPick == 0) {
        GLOBAL_MENU_INFO.state = -1;
        return;
    }
    
    GLOBAL_MENU_INFO.window_ids[0] = CreateAdvancedMenu(&winParams, winFlags, &winExInfo, TailoredDungeonMissionMenuEntryFn, numDungeonsToPick, 8);
}

void CloseTailoredDungeonMissionMenu(void) {
    if(GLOBAL_MENU_INFO.state == -1) {
        TAILORED_MISSION_DUNGEON = 0;
        MemFree(GLOBAL_MENU_INFO.extra_info_ptr);
        GLOBAL_MENU_INFO.return_val = 0xFF;
        return; // Failure to find any dungeons in dmode 3.
    } else if (GLOBAL_MENU_INFO.state == -2) {
        if (GLOBAL_MENU_INFO.window_ids[0] >= 0) {
            CloseAdvancedMenu(GLOBAL_MENU_INFO.window_ids[0]);
            GLOBAL_MENU_INFO.window_ids[0] = -1;
        }
        TAILORED_MISSION_DUNGEON = 0;
        MemFree(GLOBAL_MENU_INFO.extra_info_ptr);
        GLOBAL_MENU_INFO.return_val = 0xFE;
        return; // B Button 'failure'.
    }
    
    uint8_t selectedDungeon = ((uint8_t*)GLOBAL_MENU_INFO.extra_info_ptr)[GLOBAL_MENU_INFO.menu_results[0]];
    GLOBAL_MENU_INFO.return_val = selectedDungeon;
    TAILORED_MISSION_DUNGEON = selectedDungeon;
    MemFree(GLOBAL_MENU_INFO.extra_info_ptr);
    if (GLOBAL_MENU_INFO.window_ids[0] >= 0) {
        CloseAdvancedMenu(GLOBAL_MENU_INFO.window_ids[0]);
        GLOBAL_MENU_INFO.window_ids[0] = -1;
    }
    GenerateDailyMissions();
    TAILORED_MISSION_DUNGEON = 0;
}

// Normally this has a switch on the state but we only have one state for
// updating in this menu (waiting for the advanced menu to close).
bool UpdateTailoredDungeonMissionMenu(void) {
    if(GLOBAL_MENU_INFO.state == -1) {
        return true; // Failure to find any dungeons in dmode3.
    }
    
    int menuId = GLOBAL_MENU_INFO.window_ids[0];
    if(IsAdvancedMenuActive(menuId) == false) {
        int result = GetAdvancedMenuResult(menuId);
        if(result >= 0) {
            GLOBAL_MENU_INFO.menu_results[0] = result;
            GLOBAL_MENU_INFO.state = 2;
        } else {
            GLOBAL_MENU_INFO.state = -2; // Menu was cancelled w/ B button.
        }
        return true;
    } else {
        GLOBAL_MENU_INFO.state = 1;
        return false;
    }
}

// The initial menu function called to show a keyboard prompt for the player to type in a string.
// This is intended to be used by a variety of menus.
void CreateSimpleKeyboardMenu(void) {
    SetupAndShowKeyboard(GLOBAL_MENU_INFO.id, NULL, NULL);
}

// The menu function called repeatedly to check if the player has finished entering a string.
// This is intended to be used by a variety of menus.
bool UpdateSimpleKeyboardMenu(void) {
    return IS_BASE_GAME_MENU_FINISHED;
}

// Add your custom script menus to the list below.
// `create` is a pointer to the initial function that will run only once when a custom `message_Menu` runs. This is typically responsible for the initial creation of any windows.
// `close` is a pointer to the final function that will run only once when a custom `message_Menu` runs. This is typically responsible for the final closing of any windows, as well as setting a return value if not yet set.
// `update` is pointer to the function that will continously get called every frame when a custom `message_Menu` runs. This is typically responsible for checking the status of any menus and implementing control flow, i.e., "what happens if the player selects an option?"
// `keyboard_prompt_string_id` is the Text String ID shown when a keyboard prompt is displayed. This may not be necessary for all menus.
// `keyboard_confirm_string_id` is the Text String ID shown when confirming the player's keyboard input. This may not be necessary for all menus.
// Custom script menus use ID 80 + <array index>.
//
// Refer to menus.h for more information on the fields of `custom_menu` and `global_menu_info`!
struct custom_menu CUSTOM_MENUS[] = {
    // ID 80
    // Attempts to open a list of all dungeons that are DMODE_OPEN_AND_REQUEST.
    // If none are found, panic!
    {
        .create = CreateTailoredDungeonMissionMenu,
        .close = CloseTailoredDungeonMissionMenu,
        .update = UpdateTailoredDungeonMissionMenu
    }
};

struct global_menu_info GLOBAL_MENU_INFO;
const int CUSTOM_MENU_AMOUNT = ARRAY_LENGTH(CUSTOM_MENUS);

#endif
