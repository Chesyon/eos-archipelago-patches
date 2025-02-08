#include <pmdsky.h>
#include <cot.h>
#include "extern.h"

#if CUSTOM_SPECIAL_PROCESSES

// Special process 100: Get level scaling status
static int SpGetLevelScalingStatus() {
  return apSettings.levelScaling;
}

// Special process 101: Read/write mission status struct. First parameter: Read/Write. Second parameter: Jobs/outlaws
static int SpAccessMissionStatuses(short arg1, short arg2) {
    MissionStatus* missionStats = &(CUSTOM_SAVE_AREA.missionStats[LoadScriptVariableValue(0, 41)]); // get mission stats pointer for the dungeon specified in DUNGEON_ENTER_INDEX
    // load either jobs or outlaws:
    if (arg2 == 1) { // outlaws
        if (arg1 == 1) { // Write mode
            if (missionStats->completedOutlaws < missionStats->totalOutlaws) {
                missionStats->completedOutlaws++; // increment by one
                return 1;
            }
            else return 0;
        }
        else return missionStats->totalOutlaws - missionStats->completedOutlaws; // Read mode
    }
    else { // jobs
        if (arg1 == 1) { // Write mode
            if (missionStats->completedJobs < missionStats->totalJobs) {
                missionStats->completedJobs++; // increment by one
                return 1;
            }
            else return 0;
        }
        else return missionStats->totalJobs - missionStats->completedJobs; // Read mode
    }
}

// Called for special process IDs 100 and greater.
//
// Set return_val to the return value that should be passed back to the game's script engine. Return true,
// if the special process was handled.
bool CustomScriptSpecialProcessCall(undefined4* unknown, uint32_t special_process_id, short arg1, short arg2, int* return_val) {
  switch (special_process_id) {
    case 100:
      *return_val = SpGetLevelScalingStatus();
      return true;
    case 101:
      *return_val = SpAccessMissionStatuses(arg1, arg2);
      return true;
    default:
      return false;
  }
}

#endif
