#define RENAME_YOUR_EXPLORATION_TEAM_STR_ID 448
#define DEFAULT_TEAM_NAME_STRING_ID 564

#include <pmdsky.h>
#include <cot.h>
#include "extern.h"
#include "dungeon_generation.h"

void DungeonModeTrapCheck() {
    if(LoadScriptVariableValueAtIndex(NULL, VAR_PERFORMANCE_PROGRESS_LIST, 32) == 1) {
        SaveScriptVariableValueAtIndex(NULL, VAR_PERFORMANCE_PROGRESS_LIST, 32, 0);
        DisplayMessage(NULL, RENAME_YOUR_EXPLORATION_TEAM_STR_ID, true);
        char buffer1[16];
        char buffer2[16];
        char* defaultTeamName = StringFromId(DEFAULT_TEAM_NAME_STRING_ID);
        StrncpyName(buffer1, defaultTeamName, 10);
        StrncpyName(buffer2, defaultTeamName, 10);
        DungeonModeSetupAndShowKeyboard(buffer1, buffer2, NULL);
        SetMainTeamName(buffer1);
        SaveScriptVariableValueAtIndex(NULL, VAR_PERFORMANCE_PROGRESS_LIST, 1, 1);
    }
    
    /* Work In Progress For New Traps
    if (CUSTOM_SAVE_AREA.dungeonTraps.yawn == true) {
        for (int i = 0; i < 4; i++) {
            struct entity *monsterEntity = DUNGEON_PTR->entity_table.header.monster_slot_ptrs[i];
            TryInflictYawningStatus(monsterEntity, monsterEntity, 10);
        }
    }
    
    if (CUSTOM_SAVE_AREA.dungeonTraps.whiffer == true) {
        for (int i = 0; i < 4; i++) {
            struct entity *monsterEntity = DUNGEON_PTR->entity_table.header.monster_slot_ptrs[i];
            TryInflictWhifferStatus(monsterEntity, monsterEntity);
        }
    }
    
    if (CUSTOM_SAVE_AREA.dungeonTraps.dropItems == true) {
        // TODO
    }
    
    if (CUSTOM_SAVE_AREA.dungeonTraps.warp == true) {
        for (int i = 0; i < 4; i++) {
            struct entity *monsterEntity = DUNGEON_PTR->entity_table.header.monster_slot_ptrs[i];
            if (EntityIsValid(monsterEntity)) {
                TryWarp(monsterEntity, monsterEntity, WARP_RANDOM, NULL);
            }
        }
    }
    
    if (CUSTOM_SAVE_AREA.dungeonTraps.weather == true) {
        // TODO
    }
    
    if (CUSTOM_SAVE_AREA.dungeonTraps.pitfall == true) {
        // TODO
    }
    
    if (CUSTOM_SAVE_AREA.dungeonTraps.embargo == true) {
        for (int i = 0; i < 4; i++) {
            struct entity *monsterEntity = DUNGEON_PTR->entity_table.header.monster_slot_ptrs[i];
            TryInflictEmbargoStatus(monsterEntity, monsterEntity, true, false);
        }
    }
    */
}