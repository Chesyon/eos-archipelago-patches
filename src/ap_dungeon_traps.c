#define RENAME_YOUR_EXPLORATION_TEAM_STR_ID 448
#define DEFAULT_TEAM_NAME_STRING_ID 564

#include <pmdsky.h>
#include <cot.h>
#include "extern.h"

void DungeonModeTrapCheck() {
    if(LoadScriptVariableValueAtIndex(NULL, VAR_PERFORMANCE_PROGRESS_LIST, 32) == 0) {
        return;
    }
    
    SaveScriptVariableValueAtIndex(NULL, VAR_PERFORMANCE_PROGRESS_LIST, 32, 0);
    DisplayMessage(NULL, RENAME_YOUR_EXPLORATION_TEAM_STR_ID, true);
    char buffer1[16];
    char buffer2[16];
    
    char* defaultTeamName = StringFromId(DEFAULT_TEAM_NAME_STRING_ID);
    StrncpyName(buffer1, defaultTeamName, 10);
    StrncpyName(buffer2, defaultTeamName, 10);
    DungeonModeSetupAndShowKeyboard(buffer1, buffer2, NULL);
    SetMainTeamName(buffer1);
}