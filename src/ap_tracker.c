#include <pmdsky.h>
#include <cot.h>
#include <ap_utils.h>
#include "extern.h"
#include "custom_save_area.h"
#include "ap_settings.h"
#include "ap_tracker.h"

#define TR_BUFF_LEN 100

typedef struct TopScreenApTrackerWindow {
    uint8_t field_0x0; // 0x0: Maybe related to if the background is loaded?
    uint8_t padding_1;
    uint8_t padding_2;
    uint8_t padding_3;
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
    // For submenus this is 0xD?
    uint32_t something;
    // Is null for some menus and actual init is handled in update.
    void (*create_menu_func)(void);
    void (*close_menu_func)(void);
    uint32_t (*update_menu_func)(void);
} SomeMenuStruct;

TopScreenApTrackerWindow *tracker_window_ptr = NULL;
void* tracker_ground_mode_bg = NULL;
struct preprocessor_flags preprocessor_flags_none = {};

/* GetPageStrId
    Determine what str_id to use for the name. If the location
    has a specific name_str_id, use that. Otherwise, check the
    dungeon_id and use the dungeon's str_id from the text file.
    If that fails, panic and use the fallback error str id.
    page: page to get the str id for
    return: str id for page title */
uint16_t GetPageStrId(enum tracker_page page) {
    uint16_t page_name_str_id = tracker_book[page].name_str_id;
    enum dungeon_id page_dungeon_id = tracker_book[page].dungeon.val;
    if (0 == page_name_str_id) {
        if (DUNGEON_NONE == page_dungeon_id) {
            page_name_str_id = TRACKER_FALLBACK_ERROR_STR_ID;
        } else {
            page_name_str_id = DUNGEON_ID_TO_STR_ID(page_dungeon_id);
        }
    }

    return page_name_str_id;
}

/* IsPageActive
    check: The check to evaluate the status of.
    return: Is this check active for this game? */
bool IsPageActive(enum tracker_page page) {
    bool (*is_page_active_func)() = tracker_book[page].is_page_active_func;
    if(NULL == is_page_active_func) {
        return true;
    }

    return is_page_active_func();
}

/* IsCheckActive
    check: The check to evaluate the status of.
    return: Is this check active for this game? */
bool IsCheckActive(struct tracker_check *check) {
    bool (*is_check_active_func)() = check->is_check_active_func;
    if(NULL == is_check_active_func) {
        return true;
    }

    return is_check_active_func();
}

/* IsCheckComplete
    First, check if a check is zactually active (in this Archipelago).
    If it is not in this Archipelago, return the check as completed.
    If it is in this Archipelago, actually check if it has been
    completed.
    check: The check to evaluate the completion of.
    return: Is the check complete? */
bool IsCheckComplete(struct tracker_check *check) {
    if(false == IsCheckActive(check)) {
        return true;
    }

    enum tracker_check_type check_type = check->type;
    union check_data *check_data = &(check->data);
    switch (check_type)
    {
        default:
        case TRACKER_CHECK_TERMINATOR:
            return false;
        case TRACKER_CHECK_INFO:
        case TRACKER_CHECK_BOSS_INFO:
        case TRACKER_CHECK_BOSS_DUO_INFO:
        case TRACKER_CHECK_ESCORT_INFO:
        case TRACKER_CHECK_ESCORT_DUO_INFO:
            return true;
        case TRACKER_CHECK_CUSTOM:
        {
            struct custom_tracker_element *cte = (struct custom_tracker_element*)check_data;
            bool (*element_checks_complete_func)() = cte->element_checks_complete_func;
            return element_checks_complete_func();
        }
        case TRACKER_CHECK_NAMED:
        {
            struct subx_check *subx_check = (struct subx_check*)check_data;
            uint8_t subx_bit = subx_check->subx_bit;
            return GetSubXBit(subx_bit);
        }
        case TRACKER_CHECK_BANK:
        case TRACKER_CHECK_SHOP:
        case TRACKER_CHECK_SWAP_SHOP:
        case TRACKER_CHECK_BAG_UPGRADE:
        {
            struct numbered_subx_check *nsc = (struct numbered_subx_check*)check_data;
            uint8_t subx_bit = nsc->subx_bit;
            return GetSubXBit(subx_bit);
        }
        case TRACKER_CHECK_RANK:
        case TRACKER_CHECK_GIFT:
        case TRACKER_CHECK_ITEM:
        case TRACKER_CHECK_SEVEN_TREASURE_MISSION:
        {
            struct id_subx_check *isc = (struct id_subx_check*)check_data;
            uint8_t subx_bit = isc->subx_bit;
            return GetSubXBit(subx_bit);
        }
        case TRACKER_CHECK_DUNGEON_CONQUEST:
        case TRACKER_CHECK_SPECIAL_EPISODE_DUNGEON_CONQUEST:
        case TRACKER_CHECK_DOJO_CONQUEST:
        {
            struct dungeon_conquest_check *dqc = (struct dungeon_conquest_check*)check_data;
            enum dungeon_id dungeon_id = dqc->dungeon;
            return LoadScriptVariableValueAtIndex(NULL, VAR_DUNGEON_CONQUEST_LIST, dungeon_id);
        }
    }
}

/* IsCheckComplete
    First, check if a check is zactually active (in this Archipelago).
    If it is not in this Archipelago, return the check as completed.
    If it is in this Archipelago, actually check if it has been
    completed.
    check: The check to evaluate the completion of.
    return: Is the check complete? */
bool IsPageChecksComplete(enum tracker_page page) {
    // Sanity check our number of pages since crashing the game is
    // bad (obviously).
    if(TRACKER_BOOK_PAGE_COUNT <= page) {
        return false;
    }

    // Check if this page is a dungeon with missions.
    enum dungeon_id dungeon_id = tracker_book[page].dungeon.val;
    if(0 <= GetRemainingDungeonMissionChecks(dungeon_id, false)) {
        return false;
    }
    if(0 <= GetRemainingDungeonMissionChecks(dungeon_id, true)) {
        return false;
    }

    struct tracker_check *check = tracker_book[page].checks;
    while(TRACKER_CHECK_TERMINATOR != check->type) {
        if(false == IsCheckComplete(check)) {
            return false;
        }
        check++;
    }
    
    return true;
}

void RecycleShopDungeonCheckDrawer(int idx, struct tracker_check_built_layout *layout) {
    return;
}
bool IsAllRecycleShopDungeonChecksCompleted() {
    return false;
}

// Handle drink/drink events.
void DrinkAndDrinkEventCheckDrawer(int idx, struct tracker_check_built_layout *layout) {
    return;
}
bool IsAllDrinkAndDrinkEventChecksCompleted() {
    return false;
}

// Various strings for subbing in symbols in the ap tracker.
char locked_symbol_str[] = "[M:S4][S:8]";
char complete_symbol_str[] = "[M:S3][S:8]";
char unlocked_symbol_str[] = "[M:R7][S:8]";
char incomplete_symbol_str[] = "[M:R4][S:8]";
char check_symbol_str[] = "[M:S2][S:8]";
char instrument_symbol_str[] = "[M:R9][S:8]";
char relic_symbol_str[] = "[M:T4][S:8]";
char bag_symbol_str[] = "[M:S6]";
char money_symbol_str[] = "[M:S0]";
char debug_symbol_str[] = "[M:B32]";

// Utility function to handle drawing circular progression bars in the tracker box.
void DrawCircleBarInTextBox(signed char idx, int radius, int center_x, int center_y, uint32_t to_get, uint32_t gotten, char* locked_str, char* unlocked_str, int rotation) {
    if (to_get == 0) {
        return;
    }
    
    uint32_t step = _u32_div_f(4096, to_get);
    
    for(uint32_t i = 0; i < to_get; i++) {
        int x = ((int)TRIG_TABLE[0xFFF & (step * i + rotation)].cos * radius) >> 12;
        int y = ((int)TRIG_TABLE[0xFFF & (step * i + rotation)].sin * radius) >> 12;
        if(i < gotten) {
            DrawTextInWindow(idx, center_x + x, center_y + y, unlocked_str);
        } else {
            DrawTextInWindow(idx, center_x + x, center_y + y, locked_str);
        }
    }
}

struct window_params tracker_top_screen_window_params = {
    .x_offset = 2,
    .y_offset = 2,
    .width = 0x1C,
    .height = 0x14,
    .screen = {.val = SCREEN_SUB},
    .box_type = {.val = 0xFF}
};

/* DrawStringInNextSlotInWindow
    Finds the next available spot in layout to place the string in
    the window. This function is probably extremely overengineered
    since it's designed to work with as many columns as desired;
    however, the tracker only uses two columns because of the
    screen and font size. It probably doesn't cover every corner
    case if you have more than 3 columns. So, if this function is
    going to be reused for something else (why?), keep that in mind.
    idx: window idx
    layout: current layout data
    width_max: window max width
    str: the string to draw */
void DrawStringInNextSlotInWindow(int idx, struct tracker_check_built_layout *layout, int width_max, char* str) {
    // TODO: This function can probably be cleaned up in some way.
    int width_str = GetStringWidth(str);
    int row = layout->row;
    int col = layout->col;
    for(int x_layout = col; x_layout < LAYOUT_COLS; x_layout++) {
        for(int y_layout = row; y_layout < LAYOUT_ROWS; y_layout++) {
            if(layout->spot_usage[x_layout][y_layout] != 0) {
                continue;
            }

            // Gather width of all previous cells in this row.
            int width_sum_before = 0;
            for(int i = 0; i < x_layout; i++) {
                int cell_usage = layout->spot_usage[i][y_layout];
                if(cell_usage != 0) {
                    width_sum_before += cell_usage;
                    width_sum_before += PADDING_X;
                }
            }

            // Gather width of all cells in this row after.
            int width_sum_after = 0;
            for(int i = x_layout + 1; i < LAYOUT_COLS; i++) {
                int cell_usage = layout->spot_usage[i][y_layout];
                if(cell_usage != 0) {
                    width_sum_after += cell_usage;
                    width_sum_after += PADDING_X;
                }
            }
            
            // If this string doesn't fix next to the others... frown.
            if(width_sum_before + width_sum_after+ width_str > width_max - PADDING_X * 2) {
                continue;
            }
            layout->spot_usage[x_layout][y_layout] = width_str;
            int x_draw;
            if(x_layout == 0) {
                x_draw = PADDING_X;
            } else if (x_layout == LAYOUT_COLS - 1){
                x_draw = width_max - width_str - PADDING_X;
            } else {
                x_draw = width_sum_before + PADDING_X;
            }
            int y_draw = TRACKER_ROW_TO_Y(y_layout);
            layout->col = x_layout;
            layout->row = y_layout;
            DrawTextInWindow(idx, x_draw, y_draw, str);
            return;
        }
    }

    // If we can't find a location in the forward direction...
    // look for the first spot we can squish this into.
    for(int x_layout = 0; x_layout < col; x_layout++) {
        for(int y_layout = 0; y_layout < row; y_layout++) {
            if(layout->spot_usage[x_layout][y_layout] != 0) {
                continue;
            }

            // Gather width of all previous cells in this row.
            int width_sum_before = 0;
            for(int i = 0; i < x_layout; i++) {
                int cell_usage = layout->spot_usage[i][y_layout];
                if(cell_usage != 0) {
                    width_sum_before += cell_usage;
                    width_sum_before += PADDING_X;
                }
            }

            // Gather width of all cells in this row after.
            int width_sum_after = 0;
            for(int i = x_layout + 1; i < LAYOUT_COLS; i++) {
                int cell_usage = layout->spot_usage[i][y_layout];
                if(cell_usage != 0) {
                    width_sum_after += cell_usage;
                    width_sum_after += PADDING_X;
                }
            }
            
            // If this string doesn't fix next to the others... find another spot.
            if(width_sum_before + width_sum_after+ width_str > width_max - PADDING_X * 2) {
                continue;
            }
            layout->spot_usage[x_layout][y_layout] = width_str;
            int x_draw;
            if(x_layout == 0) {
                x_draw = PADDING_X;
            } else if (x_layout == LAYOUT_COLS - 1){
                x_draw = width_max - width_str - PADDING_X;
            } else {
                x_draw = width_sum_before + PADDING_X;
            }
            int y_draw = TRACKER_ROW_TO_Y(y_layout);
            DrawTextInWindow(idx, x_draw, y_draw, str);
            return;
        }
    }
}

void DrawMissionChecksInWindow(int idx, char* buffer, struct tracker_check_built_layout *layout, enum dungeon_id dungeon_id) {
    enum dungeon_check_type dct = GetDungeonCheckType(dungeon_id);
    if(DCT_OTHER == dct) {
        return; // Do nothing.
    }

    struct preprocessor_args preprocessor_args = {};
    bool conquest = LoadScriptVariableValueAtIndex(NULL, VAR_DUNGEON_CONQUEST_LIST, dungeon_id);
    preprocessor_args.strings[0] = conquest ? check_symbol_str : locked_symbol_str;
    PreprocessStringFromId(buffer, TR_BUFF_LEN, DUNGEON_COMPLETED_CHECK_STR_ID, preprocessor_flags_none, &preprocessor_args);
    DrawStringInNextSlotInWindow(idx, layout, tracker_top_screen_window_params.width * 8, buffer);
}

char *tracker_prefix_str = "Tracker: ";
char *title_color_prefix_str = "[CS:P]";
char *title_color_suffix_str = "[CR]";
void DrawTrackerPageInWindow(int idx, enum tracker_page page) {
    ClearWindow(idx);
    char buffer[TR_BUFF_LEN];
    buffer[0] = '\0';
    uint16_t page_name_str_id = GetPageStrId(page);
    struct preprocessor_args preprocessor_args = {};
    strncat(buffer, title_color_prefix_str, TR_BUFF_LEN);
    size_t color_prefix_str_len = strlen(buffer);
    PreprocessStringFromId(buffer + color_prefix_str_len, TR_BUFF_LEN, page_name_str_id, preprocessor_flags_none, &preprocessor_args);

    // Do math to center the title and then draw it.
    // Q: Why do we seperate the prefix 'Tracker: ' and the location name?
    // A: This is more conveninent than having to preprocess one string and
    // add the word 'Tracker: ' in front or add new string to the text file
    // for EVERY location with 'Tracker: ' prefixed onto it.
    int width_title_name = GetStringWidth(buffer + color_prefix_str_len);
    int width_title_prefix = GetStringWidth(tracker_prefix_str);
    int width_title_total = width_title_name + width_title_prefix;
    int width_window = tracker_top_screen_window_params.width * 8;
    int title_x_start = (width_window - width_title_total) / 2;
    strncat(buffer, title_color_suffix_str, TR_BUFF_LEN);
    DrawTextInWindow(idx, title_x_start, 2, tracker_prefix_str);
    DrawTextInWindow(idx, title_x_start + width_title_prefix, 2, buffer);

    struct tracker_check_built_layout layout = {.row = 0, .col = 0, .spot_usage = {{0}, {0}}};
    enum dungeon_id dungeon_id = tracker_book[page].dungeon.val;
    if(DUNGEON_NONE != dungeon_id) {
        DrawMissionChecksInWindow(idx, buffer, &layout, dungeon_id);
    }


    UpdateWindow(idx);
}

// Handles deleting the top screen background.
void ApTrackerFreeTopScreenBG() {
    tracker_window_ptr->field_0x0 = 0;
    tracker_window_ptr->state = 1; // maybe try 0???
    tracker_window_ptr->faded = 1;
    UnkTopScreenFun3(0xFFFFFFFF);
    UnkTopScreenFun2(1);
    if(tracker_ground_mode_bg != NULL) {
        UnkTopScreenFun1(tracker_ground_mode_bg);
        MemFree(tracker_ground_mode_bg);
        tracker_ground_mode_bg = NULL;
    }
    tracker_window_ptr->displayable = 0;
}

uint32_t CreateTrackerTopScreen() {
    tracker_window_ptr = MemAlloc(sizeof(TopScreenApTrackerWindow), 0xF);
    ApTrackerFreeTopScreenBG();
    UnkTopScreenFun7(0x10);
    ground_mode_opened_page = TRACKER_PAGE_ERROR;
    tracker_window_ptr->window_id = -2;
    tracker_window_ptr->displayable = 1;
    tracker_window_ptr->closing = 0;
    tracker_window_ptr->state = 0;
    return 1;
}

uint32_t CloseTrackerTopScreen(uint32_t num) {
    if (num == 0) {
        if(tracker_window_ptr->state!= 7) {
            tracker_window_ptr->closing = 1;
            return 0;
        }
    }
    
    ApTrackerFreeTopScreenBG();
    tracker_window_ptr->state = 7;
    if(tracker_window_ptr->window_id != -2) {
        CloseTextBox(tracker_window_ptr->window_id);
        tracker_window_ptr->window_id = -2;
    }
    
    MemFree((void*)tracker_window_ptr);
    tracker_window_ptr = NULL;
    return 1;
}

// Name is a guess to actual purpose.
uint32_t IsReadyTrackerTopScreen() {
    if(tracker_window_ptr->field_0x0 == 0) {
        return tracker_window_ptr->faded;
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
    switch(tracker_window_ptr->state) {
        case 0:;
            if (IsScreenFadeInProgress()) {
                tracker_window_ptr->faded = 1;
                return 0;
            }
            tracker_window_ptr->state = 1;
            // No break is intentional here. It should fall into case 1.
        case 1:;
            if (IsScreenFadeInProgress()) {
                tracker_window_ptr->faded = 1;
            } else {
                if(tracker_window_ptr->closing == 0) {
                    if(tracker_window_ptr->displayable == 0) {
                        tracker_window_ptr->faded = 0;
                    } else {
                        tracker_window_ptr->faded = 1;
                        tracker_window_ptr->state = 2;
                        tracker_window_ptr->displayable = 0;
                    }
                } else {
                    UnkTopScreenFun4(0x10);
                    tracker_window_ptr->faded = 1;
                    tracker_window_ptr->state = 6;
                }
            }
            break;
        case 2:;
            break;
        case 3:;
            break;
        case 4:;
            if(IsScreenFadeInProgress()) {
                tracker_window_ptr->faded = 1;
                return 0;
            }
            tracker_window_ptr->state = 5;
            // Falls through to case 5.
        case 5:;
            enum tracker_page current_page = CUSTOM_SAVE_AREA.trackerPage;
            if(tracker_window_ptr->closing == 0 && tracker_window_ptr->displayable == 0) {
                if(ground_mode_opened_page != CUSTOM_SAVE_AREA.trackerPage) {
                    ground_mode_opened_page = CUSTOM_SAVE_AREA.trackerPage;
                    DrawTrackerPageInWindow(tracker_window_ptr->window_id, current_page);
                }
                tracker_window_ptr->faded = 0;
            } else {
                UnkTopScreenFun4(0x10);
                tracker_window_ptr->state = 0;
                tracker_window_ptr->faded = 1;
            }
            break;
        case 6:;
            if(IsScreenFadeInProgress()) {
                return 0;
            }
            
            if(tracker_window_ptr->window_id != -2) {
                CloseTextBox(tracker_window_ptr->window_id);
                tracker_window_ptr->window_id = -2;
            }
            tracker_window_ptr->state = 7;
            // Falls through to case 7.
        case 7:;
            tracker_window_ptr->faded = 0;
            break;
    }
    
    return 0;
}

void InitializeTrackerTopScreen() {
    if(tracker_window_ptr->state != 2) {
        return;
    }
    
    tracker_window_ptr->faded = 1;
    ApTrackerFreeTopScreenBG();
    tracker_ground_mode_bg = MemAlloc(0x2F4, 0xF);
    // TODO: What is this magic number doing here????
    LoadTopScreenBGPart1(tracker_ground_mode_bg, 0x2323D98);
    LoadTopScreenBGPart2(tracker_ground_mode_bg, "BACK/expback.bgp", 0);
    if(tracker_window_ptr->window_id == -2) {
        enum tracker_page current_page = CUSTOM_SAVE_AREA.trackerPage;
        ground_mode_opened_page = current_page;
        tracker_window_ptr->window_id = CreateTextBox(&tracker_top_screen_window_params, NULL);
        DrawTrackerPageInWindow(tracker_window_ptr->window_id, current_page);
    }
    tracker_window_ptr->state = 3;
}

uint32_t TrackerTopScreenFun7() {
    if(tracker_window_ptr->state == 3) {
        SomethingTopScreenBG(tracker_ground_mode_bg);
        UnkTopScreenFun5(1);
        tracker_window_ptr->field_0x0 = 1;
        tracker_window_ptr->state = 4;
    }
    // In every state, team stats appears to update the monster animations?
    return 0;
}

void EndTrackerTopScreen() {
    if (tracker_window_ptr->field_0x0 == 0 || tracker_ground_mode_bg == 0) {
        return;
    }
    
    UnkTopScreenFun8(tracker_ground_mode_bg);
    tracker_window_ptr->field_0x0 = 0;
    UnkTopScreenFun6(0x10);
}

TopScreenMode ground_ap_tracker_mode = {
    .thing = 0xD,
    .create_func = CreateTrackerTopScreen,
    .close_func = CloseTrackerTopScreen,
    .unk_3_func = IsReadyTrackerTopScreen,
    .unk_4_func  = DebugTrackerTopScreen,
    .unk_5_func  = StateManagerTrackerTopScreen,
    .unk_6_func  = InitializeTrackerTopScreen,
    .unk_7_func  = TrackerTopScreenFun7,
    .unk_8_func  = EndTrackerTopScreen
};


signed char ap_tracker_page_selector_window_id = -2;
enum tracker_page *active_page_list = NULL;
uint32_t active_page_count;
struct window_params selector_window_params = {
    .x_offset = 2,
    .y_offset = 2,
    .box_type = {0xFF},
    .screen = {SCREEN_MAIN}
};
struct window_flags selector_window_flags = {
    .b_cancel = true,
    .se_on = true,
    .set_choice = true,
    .menu_title = true,
    .menu_lower_bar = true
};

char selector_entry_padding_str[] = "[CLUM_SET:26]";
char *ApTrackerSelectorEntryFn(char *buffer, int option_id) {
    enum tracker_page page = active_page_list[option_id];
    enum dungeon_id dungeon_id = tracker_book[page].dungeon.val;
    char* symbol_prefix_str;
    if(true == IsPageChecksComplete(page)) {
        symbol_prefix_str = complete_symbol_str;
    } else if (DUNGEON_NONE == dungeon_id) {
        symbol_prefix_str = unlocked_symbol_str;
    } else if (DUNGEON_HIDDEN_LAND == dungeon_id || DUNGEON_TEMPORAL_TOWER == dungeon_id) {
        symbol_prefix_str = relic_symbol_str;
    } else if (DUNGEON_DARK_CRATER == dungeon_id) {
        symbol_prefix_str = instrument_symbol_str;
    } else {
        switch(GetDungeonMode(dungeon_id)) {
            default:
            case DMODE_CLOSED:
            case DMODE_REQUEST:
                symbol_prefix_str = locked_symbol_str;
                break;
            case DMODE_OPEN:
                symbol_prefix_str = unlocked_symbol_str;
                break;
            case DMODE_OPEN_AND_REQUEST:
                symbol_prefix_str = incomplete_symbol_str;
                break;
        }
    }
    buffer[0] = '\0';
    strncat(buffer, symbol_prefix_str, 0x400);
    strncat(buffer, selector_entry_padding_str, 0x400);
    size_t prefix_length = strlen(buffer);
    uint16_t page_name_str_id = GetPageStrId(page);
    struct preprocessor_args preprocessor_args = {};
    PreprocessStringFromId(buffer + prefix_length, 0x400 - prefix_length, page_name_str_id, preprocessor_flags_none, &preprocessor_args);

    return buffer;
}

void CreateTrackerSelectorMenu() {
    // Build a list of currently selectable pages. Additionally, always make
    // the current page ALWAYS selectable. This is not required since the
    // code would fall back to the first option (0), but might look strange
    // to the user if they enter a special episode and have their current
    // page change/disappear as soon as they open the menu.
    uint32_t max_pages = sizeof(tracker_book)/sizeof(tracker_book[0]);
    active_page_list = MemAlloc(max_pages*sizeof(enum tracker_page), 0xF);
    enum tracker_page current_page = CUSTOM_SAVE_AREA.trackerPage;
    uint32_t current_choice = 0;
    active_page_count = 0;
    for(enum tracker_page p = 0; p < max_pages; p++) {
        if (current_page == p) {
            current_choice = active_page_count;
            active_page_list[active_page_count] = p;
            active_page_count++;
        } else if (IsPageActive(p)) {
            active_page_list[active_page_count] = p;
            active_page_count++;
        }
    }

    struct window_extra_info selector_window_extra_info = {
        .set_choice_id = current_choice,
        .title_string_id = TRACKER_SELECTOR_TITLE_STRING,
        .title_height = 0x10
    };

    ap_tracker_page_selector_window_id = CreateAdvancedMenu(
        &selector_window_params,
        selector_window_flags,
        &selector_window_extra_info,
        ApTrackerSelectorEntryFn, 
        active_page_count,
        8
    );

    if(GetTopScreenOptionType() != 5) {
        SetupGroundTopScreenFunctions(&ground_ap_tracker_mode);
    }
}

void CloseTrackerSelectorMenu() {
    if (ap_tracker_page_selector_window_id != -2) {
        CloseAdvancedMenu(ap_tracker_page_selector_window_id);
        ap_tracker_page_selector_window_id = -2;
    }
    MemFree(active_page_list);
    active_page_list = NULL;
    
    return;
}

uint32_t UpdateTrackerSelectorMenu() {
    int selected = GetAdvancedMenuCurrentOption(ap_tracker_page_selector_window_id);
    CUSTOM_SAVE_AREA.trackerPage = active_page_list[selected];

    if(true == IsAdvancedMenuActive(ap_tracker_page_selector_window_id)) {
        return 1;
    }
    
    SetupTopGroundMenuNext();
    return 4;
}

SomeMenuStruct ap_tracker_selector_menu = {
    .something = 0xD,
    .create_menu_func = CreateTrackerSelectorMenu,
    .close_menu_func = CloseTrackerSelectorMenu,
    .update_menu_func = UpdateTrackerSelectorMenu
};

struct simple_menu_id_item new_main_game_top_ground_menu_list[] = {
    // TODO: Use defines and not magic numbers.
    {.string_id = 0x218, .result_value = 2},
    {.string_id = 0x219, .result_value = 3},
    {.string_id = 0x21B, .result_value = 4},
    {.string_id = 0x21C, .result_value = 5},
    {.string_id = 0x21D, .result_value = 6},
    {.string_id = 0x21E, .result_value = 7},
    {.string_id = 0x0, .result_value = 1}
};

// TODO: Use defines and not magic numbers.
uint16_t new_top_screen_mode_option_str_ids[] = {
    0x18A,
    0x18B,
    0x18C,
    0x18D,
    0x18E,
    0x21D,
    0x0
};

signed char tracker_dungeon_window_id = -2;
enum tracker_page dungeon_mode_page = TRACKER_PAGE_ERROR;
void CreateTrackerTopScreenDungeon() {
    uint32_t region_stuff[5];
    InitBackgroundRegionDungeon(region_stuff);
    LoadBackgroundDungeon(&(TOP_SCREEN_STATUS_PTR->field107_0x80), region_stuff, 0, 1);
    SetupBackgroundDungeon(&(TOP_SCREEN_STATUS_PTR->field107_0x80));
    enum dungeon_group_id dun_group = GetDungeonGroup(DUNGEON_PTR->id.val);
    enum dungeon_id base_dungeon_id = DUNGEON_PTR->id.val;
    for(int i = 0; i < 255; i++) {
        if(GetDungeonGroup(i) == dun_group) {
            base_dungeon_id = i;
            break;
        }
    }

    for(enum tracker_page p = 0; p < TRACKER_BOOK_PAGE_COUNT; p++) {
        if(tracker_book[p].dungeon.val == base_dungeon_id) {
            dungeon_mode_page = TRACKER_PAGE_ERROR;
        }
    }

    tracker_dungeon_window_id = CreateTextBox(&tracker_top_screen_window_params, NULL);
    DrawTrackerPageInWindow(tracker_dungeon_window_id, dungeon_mode_page);
}

uint32_t UpdateTrackerTopScreenDungeon() {
    
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
    if(tracker_dungeon_window_id == -2) {
        return;
    }
    
    FreeTopScreenBGDungeon(&(TOP_SCREEN_STATUS_PTR->field107_0x80));
    CloseTextBox(tracker_dungeon_window_id);
    tracker_dungeon_window_id = -2;
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

void __attribute((naked)) ApTrackerSelectorMenuCheck (void) {
    asm("ldr r0,=ap_tracker_selector_menu");
    asm("bl SetupMenuNext");
    asm("mov r0,#0x4");
    asm("ldmia sp!,{r3,pc}");
}

void __attribute((naked)) ApTrackerTopScreenCheck (void) {
    asm("cmp r0,#0x5");
    asm("bne ApTrackerTopScreenUnhook");
    asm("ldr r0,=ground_ap_tracker_mode");
    asm("bl SetupGroundTopScreenFunctions");
    asm("ldmia sp!,{r3,pc}");
}

void __attribute((naked)) ApTrackerTopScreenCheck2 (void) {
    asm("cmp r0,#0x5");
    asm("bne ApTrackerTopScreenUnhook2");
    asm("ldr r0,=ground_ap_tracker_mode");
    asm("bl SetupGroundTopScreenFunctions2");
    asm("ldmia sp!,{r3,pc}");
}
