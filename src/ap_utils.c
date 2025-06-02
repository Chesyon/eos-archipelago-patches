#include <pmdsky.h>
#include <cot.h>
#include "extern.h"
#include "ap_utils.h"

enum dungeon_check_type GetDungeonCheckType(short dunId) {
    if (apSettings.dialgaIsGoal) {
        if(dunId == DUNGEON_TEMPORAL_TOWER || dunId == DUNGEON_HIDDEN_LAND) {
            return DCT_OTHER;
        }
    }
    
    if(dunId == DUNGEON_DARK_CRATER || dunId == DUNGEON_SKY_PEAK_SUMMIT) {
        return DCT_OTHER;
    }
    
    if(dunId >= DUNGEON_ICE_AEGIS_CAVE && dunId <= DUNGEON_REGIGIGAS_CHAMBER) {
        return DCT_OTHER;
    }
    
    if(dunId == DUNGEON_STAR_CAVE) {
        return DCT_EARLY;
    }
    
    if(DUNGEON_ZERO_ISLE_NORTH <= dunId && dunId <= DUNGEON_INFERNO_CAVE) {
        return DCT_RULE;
    }
    
    if(dunId <= DUNGEON_BRINE_CAVE_PIT || (DUNGEON_SERENITY_RIVER <= dunId && dunId <= DUNGEON_ORAN_FOREST)) {
        return DCT_EARLY;
    }
    
    if(dunId <= DUNGEON_DEEP_MYSTERY_JUNGLE || (DUNGEON_LAKE_AFAR <= dunId && dunId <= DUNGEON_SKY_PEAK_SUMMIT)) {
        return DCT_LATE;
    }
    
    return DCT_OTHER;
}

// Convenience function to grab a bit from the SubX array.
bool GetSubXBit(int bit) {
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
            expected = apSettings.totalOutlawsLate;
        } else {
            expected = apSettings.totalOutlawsEarly;
        }
    } else {
        completed = CUSTOM_SAVE_AREA.missionStats[dungeon].completedJobs;
        if (dungeonCt == DCT_LATE) {
            expected = apSettings.totalJobsLate;
        } else {
            expected = apSettings.totalJobsEarly;
        }
    }
    
    if (completed >= expected) {
        return 0;
    }
    
    return expected - completed;
}

bool IsDarkraiGoal() {
    return !apSettings.dialgaIsGoal;
}

bool AreLongLocationsOn() {
    return apSettings.longLocationsOn;
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