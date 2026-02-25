#include <pmdsky.h>
#include <cot.h>
#include "extern.h"
#include "custom_save_area.h"
#include "ap_settings.h"
#include "ap_utils.h"

#define GOAL_DIALGA 1
#define GOAL_DARKRAI 0

enum dungeon_check_type GetDungeonCheckType(short dungeon_id) {
    if (newApSettings.nums.goal == GOAL_DIALGA) {
        if(dungeon_id == DUNGEON_TEMPORAL_TOWER || dungeon_id == DUNGEON_HIDDEN_LAND) {
            return DCT_OTHER;
        }
    }
    
    if(dungeon_id == DUNGEON_DARK_CRATER || dungeon_id == DUNGEON_SKY_PEAK_SUMMIT) {
        return DCT_OTHER;
    }
    
    if(dungeon_id >= DUNGEON_ICE_AEGIS_CAVE && dungeon_id <= DUNGEON_REGIGIGAS_CHAMBER) {
        return DCT_OTHER;
    }
    
    if(dungeon_id == DUNGEON_STAR_CAVE) {
        return DCT_EARLY;
    }
    
    if(DUNGEON_ZERO_ISLE_NORTH <= dungeon_id && dungeon_id <= DUNGEON_INFERNO_CAVE) {
        return DCT_RULE;
    }
    
    if(dungeon_id <= DUNGEON_BRINE_CAVE_PIT || (DUNGEON_SERENITY_RIVER <= dungeon_id && dungeon_id <= DUNGEON_ORAN_FOREST)) {
        return DCT_EARLY;
    }
    
    if(dungeon_id <= DUNGEON_DEEP_MYSTERY_JUNGLE || (DUNGEON_LAKE_AFAR <= dungeon_id && dungeon_id <= DUNGEON_SKY_PEAK_SUMMIT)) {
        return DCT_LATE;
    }
    
    return DCT_OTHER;
}

// Convenience function to grab a bit from the SubX array.
bool GetSubXBit(int bit) {
    if(bit >= 128) {
        return false;
    }

    uint32_t temp = bit >> 3;
    uint32_t subXVal = LoadScriptVariableValueAtIndex(NULL, VAR_SCENARIO_SUB1 + (bit >> 4), temp & 0b1);
    uint32_t subXBitMask = 0b1 << (bit - (temp << 3));
    return (subXVal & subXBitMask) != 0;
}

// For checking if mission checks remain.
int GetRemainingDungeonMissionChecks(enum dungeon_id dungeon, bool outlaw) {
    // This should never be run! However, double check anyway. Just in case.
    if (dungeon <= DUNGEON_TEST_DUNGEON || DUNGEON_DOJO_0xC0 <= dungeon) {
        return 404;
    }
    
    enum dungeon_check_type dungeonCt = GetDungeonCheckType(dungeon);
    if(dungeonCt != DCT_LATE && dungeonCt != DCT_EARLY) {
        return 0;
    }
    
    int completed;
    int expected;
    if (outlaw) {
        completed = CUSTOM_SAVE_AREA.missionStats[dungeon].completedOutlaws;
        if (dungeonCt == DCT_LATE) {
            expected = newApSettings.nums.totalOutlawsLate;
        } else {
            expected = newApSettings.nums.totalOutlawsEarly;
        }
    } else {
        completed = CUSTOM_SAVE_AREA.missionStats[dungeon].completedJobs;
        if (dungeonCt == DCT_LATE) {
            expected = newApSettings.nums.totalJobsLate;
        } else {
            expected = newApSettings.nums.totalJobsEarly;
        }
    }
    
    if (completed >= expected) {
        return 0;
    }
    
    return expected - completed;
}

bool IsDarkraiGoal() {
    return newApSettings.nums.goal == GOAL_DARKRAI;
}

bool AreLongLocationsOn() {
    return newApSettings.flags.longLocationsOn;
}

enum dungeon_mode GetDungeonModeWithSevenTreasureCheck(enum dungeon_id dungeon_id) {
    if(DUNGEON_BOTTOMLESS_SEA <= dungeon_id && dungeon_id <= DUNGEON_DEEP_MYSTERY_JUNGLE) {
        return GetSubXBit((dungeon_id - DUNGEON_BOTTOMLESS_SEA)/2 + 48) ? DMODE_OPEN_AND_REQUEST : DMODE_CLOSED;
    }
    
    return GetDungeonMode(dungeon_id);
}

bool IsLookaLikeDungeonPlaceholder(enum dungeon_id dungeon_id) {
    switch(dungeon_id) {
        default:
            return false;
        case 2:
        case 5:
        case 15:
        case 16:
        case 18:
        case 65:
        case 71:
        case 86:
        case 190:
            return true;
    }
}

// Note: This works by checking the instruction in overlay11 being changed to
// 'cmp r0,#0x1' specifically.
bool __attribute__((naked)) GetDebugRomFlag() {
    asm("ldr r0,=ScriptingDebugInstructionAddr");
    asm("ldr r0,[r0,#0x0]");
    asm("ldr r1,=0xE3500001");
    asm("cmp r0,r1");
    asm("moveq r0,#1");
    asm("movne r0,#0");
    asm("bx lr");
}

// Utility function to help us debug.
void DrawDebugInfoInWindow(int idx, int x, int y, int max_width, char* buffer, int buffer_len, enum script_var_id var) {
    struct preprocessor_args pre_args = {};
    struct preprocessor_flags pre_flags = {};
    struct script_var *variable_info = &(SCRIPT_VARS.vars[var]);
    int width = GetStringWidth(variable_info->name);
    DrawTextInWindow(idx, x, y, variable_info->name);
    DrawTextInWindow(idx, x + width + 2, y, ":");
    width+=8;
    if (width >= max_width) {
        width = 0;
        y += 13;
    }
    for(int i = 0; i < variable_info->n_values; i++) {
        switch(variable_info->type.val) {
            case VARTYPE_NONE:;
                return;
            case VARTYPE_BIT:;
                if(LoadScriptVariableValueAtIndex(NULL, var, i)){
                    DrawTextInWindow(idx, x + width, y, "[M:S2]");
                    width += 8;
                } else {
                    DrawTextInWindow(idx, x + width, y, "[M:S4]");
                    width += 8;
                }
                break;
            case VARTYPE_STRING:;
                buffer[0] = LoadScriptVariableValueAtIndex(NULL, var, i);
                buffer[1] = '\0';
                width += GetStringWidth(buffer);
                DrawTextInWindow(idx, x + width, y, buffer);
                break;
            case VARTYPE_UINT8:;
            case VARTYPE_INT8:;
            case VARTYPE_UINT16:;
            case VARTYPE_INT16:;
            case VARTYPE_UINT32:;
            case VARTYPE_INT32:;
            case VARTYPE_SPECIAL:;
                pre_args.number_vals[0] = LoadScriptVariableValueAtIndex(NULL, var, i);
                PreprocessString(buffer, buffer_len, (i > 0) ? ",[value:0]" : "[value:0]", pre_flags, &pre_args);
                DrawTextInWindow(idx, x + width, y, buffer);
                width += GetStringWidth(buffer);
                break;
        }
        if(width >= max_width) {
            width = 0;
            y += 13;
        }
    }
}