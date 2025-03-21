#include <pmdsky.h>
#include <cot.h>
#include "extern.h"

bool IsDungeonLateGame(short dunId) { // rn this is the AP logic- making it a function in case the logic changes
    if (dunId >= 38) return dunId <= 86 || dunId >= 93;
    else return false;
}
