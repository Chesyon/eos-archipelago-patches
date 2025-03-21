#include <pmdsky.h>
#include <cot.h>
#include <ap_utils.h>
#include "extern.h"

#if CUSTOM_SPECIAL_PROCESSES

// Special process 100: Get level scaling status
static int SpGetLevelScalingStatus() {
  return apSettings.levelScaling;
}

// Special process 101: Read/write mission status struct. First parameter: Read/Write. Second parameter: Jobs/outlaws
static int SpAccessMissionStatuses(short mode, short missionType) {
    int dungeonId = LoadScriptVariableValue(0, 41);
    int totalNumber;
    MissionStatus* missionStats = &(CUSTOM_SAVE_AREA.missionStats[dungeonId]); // get mission stats pointer for the dungeon specified in DUNGEON_ENTER_INDEX
    // load either jobs or outlaws:
    if (missionType == 1) { // outlaws
        if(IsDungeonLateGame(dungeonId)) totalNumber = missionMaxes.totalOutlawsLate;
        else totalNumber = missionMaxes.totalOutlawsEarly;
        if (mode == 1) { // Write mode
            if (missionStats->completedOutlaws < totalNumber) {
                missionStats->completedOutlaws++; // increment by one
                return 1;
            }
            else return 0;
        }
        else return totalNumber - missionStats->completedOutlaws; // Read mode
    }
    else { // jobs
        if(IsDungeonLateGame(dungeonId)) totalNumber = missionMaxes.totalJobsLate;
        else totalNumber = missionMaxes.totalJobsEarly;
        if (mode == 1) { // Write mode
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
static int SpAccessMacguffinStatus(short mode, short macguffin) {
    short numberToReturn;
    if(macguffin == 1) { // instruments
        if (mode == 1) { // Write mode
            if (CUSTOM_SAVE_AREA.acquiredInstruments < macguffinMaxes.requiredInstruments) {
                CUSTOM_SAVE_AREA.acquiredInstruments++; // increment by one
                numberToReturn = 1;
            }
            else numberToReturn = 0;
        }
        else numberToReturn = macguffinMaxes.requiredInstruments - CUSTOM_SAVE_AREA.acquiredInstruments; // Read mode
    }
    else { // relic fragment
        if (mode == 1) { // Write mode
            if (CUSTOM_SAVE_AREA.acquiredRelicFragmentShards < macguffinMaxes.requiredRelicFragmentShards) {
                CUSTOM_SAVE_AREA.acquiredRelicFragmentShards++; // increment by one
                numberToReturn = 1;
            }
            else numberToReturn = 0;
        }
        else numberToReturn = macguffinMaxes.requiredRelicFragmentShards - CUSTOM_SAVE_AREA.acquiredRelicFragmentShards; // Read mode
    }
    if (numberToReturn < 0) return 0;
    else return numberToReturn;
}

void GetLowercaseName(const char* src, char* dst) // Used in NameCheck
{
    MemZero(dst, 10);
    for(int i = 0; i < 10 && src[i] != NULL; i++) dst[i] = src[i] >= 'A' && src[i] <= 'Z' ? src[i]+0x20 : src[i];
}

// Special process 103: Checks if HERO_FIRST_NAME matches any name in a list of dev names. Returns the index of the name plus one if there's a match, otherwise returns zero. Saves the return value to nameCheckResult in the save.
static int SpDoNameCheck() {
    #define num_of_names 7
    char name_check_names[num_of_names][10] = {"chesyon", "happylappy", "cryptic", "kattnip", "hecka", "tailsdk", "fieryblizz"};
    char name[10];
    char lower_name[10];
    LoadScriptVariableValueBytes(VAR_HERO_FIRST_NAME, name, 10);
    GetLowercaseName(name, lower_name);
    // return i+1
    for (int i = 0; i < num_of_names; i++) if (strncmp(lower_name, name_check_names[i], 10) == 0) return i+1;
    return 0;
}

// Special process 104: Regenerates board missions.
static int SpRegenerateMissions() {
    GenerateDailyMissions();
    return 0;
}

// Special process 105: Read/write Cafe check count. 1 = write, anything else = 0.
// This is a really lazy implementation based on SP 102. Could be merged into SP 102 if I actually bothered to reorganize the structs, which... I don't feel like doing right now. This is future Chesyon's problem.
static int SpAccessCafeStatus(short mode) {
    if (mode == 1) { // Write mode
        if (CUSTOM_SAVE_AREA.acquiredCafeChecks < cafeMax) {
            CUSTOM_SAVE_AREA.acquiredCafeChecks++; // increment by one
            return 1;
        }
        else return 0;
    }
    else return cafeMax - CUSTOM_SAVE_AREA.acquiredCafeChecks; // Read mode
}

// Special process Read/write DeathLink
/*static int SpAccessDeathLinkStatus(short action, short value) {
    switch (action) {
        case 0: // Check Deathlink Type
            return apSettings.deathLinkType;
        case 1: // Check If We Have Received
            return CUSTOM_SAVE_AREA.deathLinkTracker.receiver;
        case 2: // Check If We Have Sent? Not sure why we would need to test this.
            return CUSTOM_SAVE_AREA.deathLinkTracker.sender;
        case 3: // Set Received
            CUSTOM_SAVE_AREA.deathLinkTracker.receiver = value;
        case 4: // Set Sender
            CUSTOM_SAVE_AREA.deathLinkTracker.sender = value;
    }
}*/

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
    case 104:
        *return_val = SpRegenerateMissions();
        return true;
    case 105:
        *return_val = SpAccessCafeStatus(arg1);
    default:
        return false;
  }
}

#endif
