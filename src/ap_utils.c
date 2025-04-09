#include <pmdsky.h>
#include <cot.h>
#include "extern.h"

bool IsDungeonLateGame(short dunId) { // rn this is the AP logic- making it a function in case the logic changes
    if (dunId >= 38) return dunId <= 86 || dunId >= 93;
    else return false;
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
    
    int completed;
    int expected;
    if (outlaw) {
        completed = CUSTOM_SAVE_AREA.missionStats[dungeon].completedOutlaws;
        if (IsDungeonLateGame(dungeon)) {
            expected = missionMaxes.totalOutlawsLate;
        } else {
            expected = missionMaxes.totalOutlawsEarly;
        }
    } else {
        completed = CUSTOM_SAVE_AREA.missionStats[dungeon].completedJobs;
        if (IsDungeonLateGame(dungeon)) {
            expected = missionMaxes.totalJobsLate;
        } else {
            expected = missionMaxes.totalJobsEarly;
        }
    }
    
    if (completed >= expected) {
        return 0;
    }
    
    return expected - completed;
}