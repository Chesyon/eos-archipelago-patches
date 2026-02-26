#include <pmdsky.h>
#include <cot.h>
#include <ap_utils.h>
#include "extern.h"
#include "custom_save_area.h"
#include "ap_settings.h"
#include "ap_tracker.h"

#define TR_BUFF_LEN 300

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

TopScreenApTrackerWindow *ap_tracker_window_ptr = NULL;
void* ap_tracker_top_screen_bg = NULL;
signed char ap_tracker_dungeon_window_ptr = -2;
enum tracker_page last_opened_page = TRACKER_PAGE_ERROR;

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
    First, check if a check is actually active (in this Archipelago).
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
        TRACKER_CHECK_TERMINATOR:
            return false;
        TRACKER_CHECK_INFO:
        TRACKER_CHECK_BOSS_INFO:
        TRACKER_CHECK_BOSS_DUO_INFO:
        TRACKER_CHECK_ESCORT_INFO:
        TRACKER_CHECK_ESCORT_DUO_INFO:
            return true;
        TRACKER_CHECK_CUSTOM: {
            struct custom_tracker_element *cte = check_data;
            bool (*element_checks_complete_func)() = cte->element_checks_complete_func;
            return element_checks_complete_func();
        }
        TRACKER_CHECK_NAMED: {
            struct subx_check *subx_check = check_data;
            uint8_t subx_bit = subx_check->subx_bit;
            return GetSubXBit(subx_bit);
        }
        TRACKER_CHECK_BANK:
        TRACKER_CHECK_SHOP:
        TRACKER_CHECK_SWAP_SHOP:
        TRACKER_CHECK_BAG_UPGRADE: {
            struct numbered_subx_check *nsc = check_data;
            uint8_t subx_bit = nsc->subx_bit;
            return GetSubXBit(subx_bit);
        }
        TRACKER_CHECK_RANK:
        TRACKER_CHECK_GIFT:
        TRACKER_CHECK_ITEM:
        TRACKER_CHECK_SEVEN_TREASURE_MISSION: {
            struct id_subx_check *isc = check_data;
            uint8_t subx_bit = isc->subx_bit;
            return GetSubXBit(subx_bit);
        }
        TRACKER_CHECK_DUNGEON_CONQUEST:
        TRACKER_CHECK_SPECIAL_EPISODE_DUNGEON_CONQUEST:
        TRACKER_CHECK_DOJO_CONQUEST: {
            struct dungeon_conquest_check *dqc = check_data;
            enum dungeon_id dungeon_id = dqc->dungeon.val;
            return LoadScriptVariableValueAtIndex(NULL, VAR_DUNGEON_CONQUEST_LIST, dungeon_id);
        }
    }
}


bool IsLocationChecksComplete(enum tracker_page page) {
    // Sanity check our number of pages since crashing the game is
    // bad (obviously).
    if(TRACKER_BOOK_PAGE_COUNT <= page) {
        return false;
    }

    // Check if this page is a dungeon with missions.
    enum dungeon_id dungeon_id = tracker_book[page].dungeon.val;
    if(0 != GetRemainingDungeonMissionChecks(dungeon_id, false)) {
        return false;
    }
    if(0 != GetRemainingDungeonMissionChecks(dungeon_id, true)) {
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
char* debugSymbol = "[M:B30]";

char* ApTrackerEntryFn(char* buffer, int option_id) {
    // TODO
}

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

struct window_params tracker_top_screen_window_params = {.x_offset = 2, .y_offset = 2, .width = 0x1C, .height = 0x14, .screen = {.val = SCREEN_SUB}, .box_type = {.val = 0xFF}};

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

// Utility functon to streamline displaying boss information with optional gift.
void DrawEscortInfoInWindow(int idx, int y, char* buffer, enum monster_id escort0, enum monster_id escort1) {
    preArgs.flag_vals[0] = escort0;
    preArgs.flag_vals[1] = escort1;
    PreprocessString(buffer, TR_BUFF_LEN, (escort1 == MONSTER_NONE) ? escortInfo : duoEscortInfo, preFlagTracker, &preArgs);
    DrawTextInWindow(idx, 15, y, buffer);
}


void ApTrackerTopScreenWindowUpdate(int idx, uint32_t location) {
    // TODO: Rewrite this whole function for new table.
}

// Handles deleting the top screen background.
void ApTrackerFreeTopScreenBG() {
    ap_tracker_window_ptr->field_0x0 = 0;
    ap_tracker_window_ptr->state = 1; // maybe try 0???
    ap_tracker_window_ptr->faded = 1;
    UnkTopScreenFun3(0xFFFFFFFF);
    UnkTopScreenFun2(1);
    if(ap_tracker_top_screen_bg != NULL) {
        UnkTopScreenFun1(ap_tracker_top_screen_bg);
        MemFree(ap_tracker_top_screen_bg);
        ap_tracker_top_screen_bg = NULL;
    }
    ap_tracker_window_ptr->displayable = 0;
}

uint32_t CreateTrackerTopScreen() {
    ap_tracker_window_ptr = MemAlloc(sizeof(TopScreenApTrackerWindow), 0xF);
    ApTrackerFreeTopScreenBG();
    UnkTopScreenFun7(0x10);
    last_opened_page = TRACKER_PAGE_ERROR;
    ap_tracker_window_ptr->window_id = -2;
    ap_tracker_window_ptr->displayable = 1;
    ap_tracker_window_ptr->closing = 0;
    ap_tracker_window_ptr->state = 0;
    return 1;
}

uint32_t CloseTrackerTopScreen(uint32_t num) {
    if (num == 0) {
        if(ap_tracker_window_ptr->state!= 7) {
            ap_tracker_window_ptr->closing = 1;
            return 0;
        }
    }
    
    ApTrackerFreeTopScreenBG();
    ap_tracker_window_ptr->state = 7;
    if(ap_tracker_window_ptr->window_id != -2) {
        CloseTextBox(ap_tracker_window_ptr->window_id);
        ap_tracker_window_ptr->window_id = -2;
    }
    
    MemFree((void*)ap_tracker_window_ptr);
    ap_tracker_window_ptr = NULL;
    return 1;
}

// Name is a guess to actual purpose.
uint32_t IsReadyTrackerTopScreen() {
    if(ap_tracker_window_ptr->field_0x0 == 0) {
        return ap_tracker_window_ptr->faded;
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
    switch(ap_tracker_window_ptr->state) {
        case 0:;
            if (IsScreenFadeInProgress()) {
                ap_tracker_window_ptr->faded = 1;
                return 0;
            }
            ap_tracker_window_ptr->state = 1;
            // No break is intentional here. It should fall into case 1.
        case 1:;
            if (IsScreenFadeInProgress()) {
                ap_tracker_window_ptr->faded = 1;
            } else {
                if(ap_tracker_window_ptr->closing == 0) {
                    if(ap_tracker_window_ptr->displayable == 0) {
                        ap_tracker_window_ptr->faded = 0;
                    } else {
                        ap_tracker_window_ptr->faded = 1;
                        ap_tracker_window_ptr->state = 2;
                        ap_tracker_window_ptr->displayable = 0;
                    }
                } else {
                    UnkTopScreenFun4(0x10);
                    ap_tracker_window_ptr->faded = 1;
                    ap_tracker_window_ptr->state = 6;
                }
            }
            break;
        case 2:;
            break;
        case 3:;
            break;
        case 4:;
            if(IsScreenFadeInProgress()) {
                ap_tracker_window_ptr->faded = 1;
                return 0;
            }
            ap_tracker_window_ptr->state = 5;
            // Falls through to case 5.
        case 5:;
            enum tracker_page current_page = CUSTOM_SAVE_AREA.trackerPage;
            if(ap_tracker_window_ptr->closing == 0 && ap_tracker_window_ptr->displayable == 0) {
                if(last_opened_page != CUSTOM_SAVE_AREA.trackerPage) {
                    last_opened_page = CUSTOM_SAVE_AREA.trackerPage;
                    ApTrackerTopScreenWindowUpdate(ap_tracker_window_ptr->window_id, current_page);
                }
                ap_tracker_window_ptr->faded = 0;
            } else {
                UnkTopScreenFun4(0x10);
                ap_tracker_window_ptr->state = 0;
                ap_tracker_window_ptr->faded = 1;
            }
            break;
        case 6:;
            if(IsScreenFadeInProgress()) {
                return 0;
            }
            
            if(ap_tracker_window_ptr->window_id != -2) {
                CloseTextBox(ap_tracker_window_ptr->window_id);
                ap_tracker_window_ptr->window_id = -2;
            }
            ap_tracker_window_ptr->state = 7;
            // Falls through to case 7.
        case 7:;
            ap_tracker_window_ptr->faded = 0;
            break;
    }
    
    return 0;
}

void InitializeTrackerTopScreen() {
    if(ap_tracker_window_ptr->state != 2) {
        return;
    }
    
    ap_tracker_window_ptr->faded = 1;
    ApTrackerFreeTopScreenBG();
    ap_tracker_top_screen_bg = MemAlloc(0x2F4, 0xF);
    // TODO: What is this magic number doing here????
    LoadTopScreenBGPart1(ap_tracker_top_screen_bg, 0x2323D98);
    LoadTopScreenBGPart2(ap_tracker_top_screen_bg, "BACK/expback.bgp", 0);
    if(ap_tracker_window_ptr->window_id == -2) {
        enum tracker_page current_page = CUSTOM_SAVE_AREA.trackerPage;
        last_opened_page = current_page;
        ap_tracker_window_ptr->window_id = CreateTextBox(&tracker_top_screen_window_params, NULL);
        ApTrackerTopScreenWindowUpdate(ap_tracker_window_ptr->window_id, current_page);
    }
    ap_tracker_window_ptr->state = 3;
}

uint32_t TrackerTopScreenFun7() {
    if(ap_tracker_window_ptr->state == 3) {
        SomethingTopScreenBG(ap_tracker_top_screen_bg);
        UnkTopScreenFun5(1);
        ap_tracker_window_ptr->field_0x0 = 1;
        ap_tracker_window_ptr->state = 4;
    }
    // In every state, team stats appears to update the monster animations?
    return 0;
}

void EndTrackerTopScreen() {
    if (ap_tracker_window_ptr->field_0x0 == 0 || ap_tracker_top_screen_bg == 0) {
        return;
    }
    
    UnkTopScreenFun8(ap_tracker_top_screen_bg);
    ap_tracker_window_ptr->field_0x0 = 0;
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
enum tracker_page *active_page_list;
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

void CreateTrackerSelectorMenu() {
    // Build a list of currently selectable pages.
    uint32_t max_pages = sizeof(tracker_book)/sizeof(tracker_book[0]);
    active_page_list = MemAlloc(max_pages*sizeof(enum tracker_page), 0xF);
    active_page_count = 0;
    for(enum tracker_page p = 0; p < max_pages; p++) {
        if(true == IsPageActive(p)) {
            active_page_list[active_page_count] = p;
            active_page_count++;
        }
    }

    struct window_extra_info selector_window_extra_info = {
        .set_choice_id = CUSTOM_SAVE_AREA.trackerPage,
        .title_string_id = TRACKER_SELECTOR_TITLE_STRING,
        .title_height = 0x10
    };

    ap_tracker_page_selector_window_id = CreateAdvancedMenu(
        &selector_window_params,
        selector_window_flags,
        &selector_window_extra_info,
        ApTrackerEntryFn, 
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
    
    return;
}

uint32_t UpdateTrackerSelectorMenu() {
    int selected = GetAdvancedMenuCurrentOption(ap_tracker_page_selector_window_id);
    CUSTOM_SAVE_AREA.trackerPage = active_page_list[selected];

    if(IsAdvancedMenuActive(ap_tracker_page_selector_window_id) == true) {
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
    uint8_t location = CheckLocationOverrides(dungeonModeDisplayed);
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
    asm("ldr r0,=ground_ap_tracker_mode");
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
