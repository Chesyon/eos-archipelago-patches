#define RENAME_YOUR_EXPLORATION_TEAM_STR_ID 448
#define DEFAULT_TEAM_NAME_STRING_ID 564

#include <pmdsky.h>
#include <cot.h>
#include "extern.h"
#include "dungeon_generation.h"

void __attribute__((naked)) GenerateFloorCustomLayouts() {
    asm("cmp r0,#15");
    asm("ldr lr,=GeneateFloorUnhook1");
    asm("beq GenerateMazeBacktrackingFloor");
    asm("b   GenerateSidewinderFloor");
}

uint8_t GetMazeTrapCheck(enum floor_layout layout) {
    if(CUSTOM_SAVE_AREA.dungeonTraps.maze) {
        CUSTOM_SAVE_AREA.dungeonTraps.maze = false;
        return DungeonRandInt(2) + 14;
    }
    
    if(layout < LAYOUT_UNUSED_0xC || DUNGEON_PTR->id.val == 172) {
        return layout;
    }
    
    return LAYOUT_LARGE;
}

void __attribute__((naked)) ApMazeTrapCheck() {
    asm("stmdb sp!,{r1,r2,r3,lr}");
    asm("mov r0,r10");
    asm("bl GetMazeTrapCheck");
    asm("mov r10,r0");
    asm("ldmia sp!,{r1,r2,r3,pc}");
}

void DungeonModeInitApTrap() {
    CUSTOM_SAVE_AREA.dungeonTraps.yawn = false;
    CUSTOM_SAVE_AREA.dungeonTraps.whiffer = false;
    CUSTOM_SAVE_AREA.dungeonTraps.dropItems = false;
    CUSTOM_SAVE_AREA.dungeonTraps.weather = false;
    CUSTOM_SAVE_AREA.dungeonTraps.warp = false;
    CUSTOM_SAVE_AREA.dungeonTraps.pitfall = false;
    CUSTOM_SAVE_AREA.dungeonTraps.embargo = false;
    CUSTOM_SAVE_AREA.dungeonTraps.maze = false;
}

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
    
    // TODO: Fix this.
    if (CUSTOM_SAVE_AREA.dungeonTraps.dropItems == true) {
        for (int i = 0; i < 4; i++) {
            struct entity *monsterEntity = DUNGEON_PTR->entity_table.header.monster_slot_ptrs[i];
            //ApplyTripTrapEffect(monsterEntity, monsterEntity);
        }
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
        switch(DungeonRandInt(3)) {
            case 0:
                DUNGEON_PTR->weather_turns[2] = 3000; // Sandstorm
                break;
            case 1:
                DUNGEON_PTR->weather_turns[3] = 3000; // Cloudy
                break;
            case 2:
            default:
                DUNGEON_PTR->weather_turns[5] = 3000; // Hail
                break;
        }
        TryActivateWeather(true, false);
    }
    
    if (CUSTOM_SAVE_AREA.dungeonTraps.pitfall == true) {
        DUNGEON_PTR->end_floor_flag = 2;
    }
    
    if (CUSTOM_SAVE_AREA.dungeonTraps.embargo == true) {
        for (int i = 0; i < 4; i++) {
            struct entity *monsterEntity = DUNGEON_PTR->entity_table.header.monster_slot_ptrs[i];
            TryInflictEmbargoStatus(monsterEntity, monsterEntity, true, false);
        }
    }
    
    CUSTOM_SAVE_AREA.dungeonTraps.yawn = false;
    CUSTOM_SAVE_AREA.dungeonTraps.whiffer = false;
    CUSTOM_SAVE_AREA.dungeonTraps.dropItems = false;
    CUSTOM_SAVE_AREA.dungeonTraps.weather = false;
    CUSTOM_SAVE_AREA.dungeonTraps.warp = false;
    CUSTOM_SAVE_AREA.dungeonTraps.pitfall = false;
    CUSTOM_SAVE_AREA.dungeonTraps.embargo = false;
}