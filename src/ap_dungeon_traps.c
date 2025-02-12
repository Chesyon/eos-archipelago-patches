#include <pmdsky.h>
#include <cot.h>
#include "extern.h"

void DungeonModeTrapCheck() {
    if(LoadScriptVariableValueAtIndex(NULL, VAR_PERFORMANCE_PROGRESS_LIST, 32) == 0) {
        return;
    }
    
    SaveScriptVariableValueAtIndex(NULL, VAR_PERFORMANCE_PROGRESS_LIST, 32, 0);
}