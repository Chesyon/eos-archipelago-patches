#include <pmdsky.h>
#include <cot.h>
#include "extern.h"

#if CUSTOM_SPECIAL_PROCESSES

// Special process 100: Get level scaling status
static int SpGetLevelScalingStatus() {
  return apSettings.levelScaling;
}

bool IsDungeonPostDialga(short dunId) { // rn this is the AP logic- making it a function in case the logic changes
    if (dunId >= 44) return dunId <= 86 || dunId >= 93;
    else return false;
}

// Special process 101: Read/write mission status struct. First parameter: Read/Write. Second parameter: Jobs/outlaws
static int SpAccessMissionStatuses(short arg1, short arg2) {
    int dungeonId = LoadScriptVariableValue(0, 41);
    int totalNumber;
    MissionStatus* missionStats = &(CUSTOM_SAVE_AREA.missionStats[dungeonId]); // get mission stats pointer for the dungeon specified in DUNGEON_ENTER_INDEX
    // load either jobs or outlaws:
    if (arg2 == 1) { // outlaws
        if(IsDungeonPostDialga(dungeonId)) totalNumber = missionMaxes.totalOutlawsLate;
        else totalNumber = missionMaxes.totalOutlawsEarly;
        if (arg1 == 1) { // Write mode
            if (missionStats->completedOutlaws < totalNumber) {
                missionStats->completedOutlaws++; // increment by one
                return 1;
            }
            else return 0;
        }
        else return totalNumber - missionStats->completedOutlaws; // Read mode
    }
    else { // jobs
        if(IsDungeonPostDialga(dungeonId)) totalNumber = missionMaxes.totalJobsLate;
        else totalNumber = missionMaxes.totalJobsEarly;
        if (arg1 == 1) { // Write mode
            if (missionStats->completedJobs < totalNumber) {
                missionStats->completedJobs++; // increment by one
                return 1;
            }
            else return 0;
        }
        else return totalNumber - missionStats->completedJobs; // Read mode
    }
}

// Special process 102: Read/write Instrument/Relic Fragment
static int SpAccessMacguffinStatus(short arg1, short arg2) {
    if(arg2 == 1) { // instruments
        if (arg1 == 1) { // Write mode
            if (CUSTOM_SAVE_AREA.acquiredInstruments < macguffinMaxes.totalInstruments) {
                CUSTOM_SAVE_AREA.acquiredInstruments++; // increment by one
                return 1;
            }
            else return 0;
        }
        else return macguffinMaxes.totalInstruments - CUSTOM_SAVE_AREA.acquiredInstruments; // Read mode
    }
    else { // relic fragment
        if (arg1 == 1) { // Write mode
            if (CUSTOM_SAVE_AREA.acquiredRelicFragmentShards < macguffinMaxes.totalRelicFragmentShards) {
                CUSTOM_SAVE_AREA.acquiredRelicFragmentShards++; // increment by one
                return 1;
            }
            else return 0;
        }
        else return macguffinMaxes.totalRelicFragmentShards - CUSTOM_SAVE_AREA.acquiredRelicFragmentShards; // Read mode
    }
}

void GetLowercaseName(const char* src, char* dst) // Used in NameCheck
{
    MemZero(dst, 10);
    for(int i = 0; i < 10 && src[i] != NULL; i++) dst[i] = src[i] >= 'A' && src[i] <= 'Z' ? src[i]+0x20 : src[i];
}

// Special process 103: Checks if HERO_FIRST_NAME matches any name in a list of dev names. Returns the index of the name plus one if there's a match, otherwise returns zero. Saves the return value to nameCheckResult in the save.
static int SpDoNameCheck() {
    #define num_of_names 3
    char name_check_names[num_of_names][10] = {"chesyon", "happylappy", "cryptic"};
    char name[10];
    char lower_name[10];
    LoadScriptVariableValueBytes(VAR_HERO_FIRST_NAME, name, 10);
    GetLowercaseName(name, lower_name);
    for (int i = 0; i < num_of_names; i++) if (strncmp(lower_name, name_check_names[i], 10) == 0) return i + 1;
    return 0;
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
    case 102:
        *return_val = SpAccessMacguffinStatus(arg1, arg2);
        return true;
    case 103:
        *return_val = SpDoNameCheck();
        return true;
    default:
        return false;
  }
}

#endif
