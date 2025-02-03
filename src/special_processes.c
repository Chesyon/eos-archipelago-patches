#include <pmdsky.h>
#include <cot.h>
#include <extern.h>

#if CUSTOM_SPECIAL_PROCESSES

// Special process 100: Get level scaling status
static int SpGetLevelScalingStatus() {
  return AP_SETTINGS[3];
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
    default:
      return false;
  }
}

#endif
