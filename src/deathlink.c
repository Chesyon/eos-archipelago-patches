// This equates to string 2585.
#define DEATHLINK_DAMAGE_SOURCE DAMAGE_SOURCE_DUMMY_625

#include <pmdsky.h>
#include <cot.h>
#include "extern.h"
#include "custom_save_area.h"

void DeathLinkInitDungeon() {
    CUSTOM_SAVE_AREA.deathLinkTracker.receiver = false;
    CUSTOM_SAVE_AREA.deathLinkTracker.allyDeathMessage[0] = '\0';
}

union damage_source damageSource = {.other = DEATHLINK_DAMAGE_SOURCE};
void DeathLinkReceiverCheck() {
    int dlType = newApSettings.nums.deathLinkType;

    if(dlType == DEATHLINK_NONE) {
        return; // Do nothing if not deathLink.
    }
    
    if (CUSTOM_SAVE_AREA.deathLinkTracker.receiver == false) {
        return; // Do nothing if not recieved.
    }
    
    struct entity *leader = GetLeader();
    // Tell the player what happened and to who.
    // Messages may not look so cool because of how EoS wraps text.
    if(CUSTOM_SAVE_AREA.deathLinkTracker.allyDeathMessage[0] != '\0') {
        LogMessageWithPopupCheckUser(leader, CUSTOM_SAVE_AREA.deathLinkTracker.allyDeathMessage);
        MemZero(CUSTOM_SAVE_AREA.deathLinkTracker.allyDeathMessage, 128);
    }
    
    // Roll for a random cool visual experience for your death.
    switch (DungeonRandInt(18)) {
        default:
        case 0: // Rocks
            ChangeMonsterAnimation(leader, 6, DIR_NONE);
            PlayEffectAnimationEntityWrapper(leader, 55);
            break;
        case 1: // Fire Boom
            ChangeMonsterAnimation(leader, 6, DIR_NONE);
            PlayEffectAnimationEntityWrapper(leader, 66);
            break;
        case 2: // Wisp Spin
            PlayEffectAnimationEntityWrapper(leader, 75);
            leader->is_visible = false;
            break;
        case 3: // Lock On + Item
            PlayEffectAnimationEntityWrapper(leader, 80);
            leader->is_visible = false;
            struct item item;
            struct position pos = {.x = leader->pos.x, .y = leader->pos.y};
            GenerateStandardItem(&item, GetItemIdToSpawn(ITEM_LIST_REGULAR), GEN_ITEM_STICKY_RANDOM);
            SpawnDroppedItemWrapper(leader, &pos, &item, 0);
            break;
        case 4:; // Blocked In
            if (dlType == DEATHLINK_REVIVER) {
                break;
            }
            int xStart = leader->pos.x - 1;
            int yStart = leader->pos.y - 1;
            int xEnd = leader->pos.x + 1;
            int yEnd = leader->pos.y + 1;
            if(xStart < 0) {
                xStart = 0;
            }
            if(yStart < 0) {
                yStart = 0;
            }
            if(xEnd > 0x37) {
                xEnd = 0x37;
            }
            if(yEnd > 0x20) {
                yEnd = 0x20;
            }
            for(int x = xStart; x <= xEnd; x++) {
                for(int y = yStart; y <= yEnd; y++) {
                    struct tile* tile = GetTileSafe(x, y);
                    tile->terrain_type = TERRAIN_WALL;
                }
            }
            ChangeMonsterAnimation(leader, 6, DIR_NONE);
            xStart = xStart - 1;
            yStart = yStart - 1;
            xEnd = xEnd + 1;
            yEnd = yEnd + 1;
            if(xStart < 0) {
                xStart = 0;
            }
            if(yStart < 0) {
                yStart = 0;
            }
            if(xEnd > 0x37) {
                xEnd = 0x37;
            }
            if(yEnd > 0x20) {
                yEnd = 0x20;
            }
            for(int x = xStart; x <= xEnd; x++) {
                for(int y = yStart; y <= yEnd; y++) {
                    DetermineTileAppearence(x, y);
                    UpdateTrapsVisibility();
                    for (int i = 0; i < 15; i++) {
                        AdvanceFrame(0);
                    }
                }
            }
            break;
        case 5: // Thunder
            PlayEffectAnimationEntityWrapper(leader, 177);
            ChangeMonsterAnimation(leader, 6, DIR_NONE);
            PlayEffectAnimationEntityWrapper(leader, 178);
            break;
        case 6: // Star Fire
            PlayEffectAnimationEntityWrapper(leader, 265);
            ChangeMonsterAnimation(leader, 6, DIR_NONE);
            break;
        case 7: // Item Squish
            PlayEffectAnimationEntityWrapper(leader, 268);
            ChangeMonsterAnimation(leader, 6, DIR_NONE);
            break;
        case 8: // Music Notes Fall
            PlayEffectAnimationEntityWrapper(leader, 348);
            ChangeMonsterAnimation(leader, 6, DIR_NONE);
            break;
        case 9: // Wait and... kersplat.
            PlayEffectAnimationEntityWrapper(leader, 10);
            ChangeMonsterAnimation(leader, 6, DIR_NONE);
            for (int i = 0; i < 120; i++) {
                AdvanceFrame(0);
            }
            leader->is_visible = false;
            PlayEffectAnimationEntityWrapper(leader, 368);
            break;
        case 10: // Chestnuts
            PlayEffectAnimationEntityWrapper(leader, 393);
            ChangeMonsterAnimation(leader, 6, DIR_NONE);
            break;
        case 11: // Spiral Glass?
            PlayEffectAnimationEntityWrapper(leader, 420);
            ChangeMonsterAnimation(leader, 6, DIR_NONE);
            PlayEffectAnimationEntityWrapper(leader, 24);
            break;
        case 12: // Hand Smash
            PlayEffectAnimationEntityWrapper(leader, 449);
            ChangeMonsterAnimation(leader, 6, DIR_NONE);
            PlayEffectAnimationEntityWrapper(leader, 450);
            for (int i = 0; i < 60; i++) {
                AdvanceFrame(0);
            }
            break;
        case 13: // Toss Leader
            for(int i = 0; i < 10; i++) {
                TryPounce(leader, leader, DungeonRandInt(8));
                for (int i = 0; i < 20; i++) {
                    AdvanceFrame(0);
                }
                ChangeMonsterAnimation(leader, 6, DIR_NONE);
            }
            break;
        case 14: // Crushed By Rock
            PlayEffectAnimationEntityWrapper(leader, 482);
            PlayEffectAnimationEntityWrapper(leader, 483);
            leader->is_visible = false;
            PlayEffectAnimationEntityWrapper(leader, 481);
            for (int i = 0; i < 60; i++) {
                AdvanceFrame(0);
            }
            break;
        case 15: // Spin Hand
            for (int i = 0; i < 8; i++) {
                PlayEffectAnimationEntityWrapper(leader, 516);
                for(int j = 0; j < 30; j++) {
                    AdvanceFrame(0);
                }
                struct monster *leaderMonster = (leader->info);
                leaderMonster->action.direction.val = (leaderMonster->action.direction.val + 0x1) & 0x7;
            }
            break;
        case 16: // Turned Into The Moon, That's Rough Buddy
            leader->is_visible = false;
            PlayEffectAnimationEntityWrapper(leader, 593);
            break;
        case 17: // Meteor
            PlayEffectAnimationEntityWrapper(leader, 614);
            leader->is_visible = false;
            break;
    }
    
    if(dlType == DEATHLINK_TRUE_DEATH) {
        HandleFaint(leader, damageSource, leader);
    } else if (dlType == DEATHLINK_REVIVER) {
        leader->is_visible = true;
        struct monster *leaderMonster = leader->info;
        leaderMonster->statuses.reflect.val = STATUS_REFLECT_NONE; // Stop Endure from Activating.
        CalcDamageFixedNoCategory(leader, leader, 9999, false, NULL, TYPE_NONE, damageSource,
            DAMAGE_MESSAGE_HUNGER, 0, 0);
    } else { // Panic!
        leader->is_visible = true;
    }
    
    // Reuse [string:2] to use our fallen ally name.
    strncpy(DUNGEON_PTR->fallen_escort_monster_name, CUSTOM_SAVE_AREA.deathLinkTracker.allyDeathName, 10);
    
    CUSTOM_SAVE_AREA.deathLinkTracker.receiver = false;
}

void DeathLinkSenderCheck() {
    union damage_source damageSource = {.other = (DUNGEON_PTR->fainted_monster_dungeon_end_reason.other.val)};
    // If this was a succesful mission, we got smited by the deathlink,
    // or deathlink is disabled, do nothing.
    if (damageSource.other == DEATHLINK_DAMAGE_SOURCE || damageSource.other >= DAMAGE_SOURCE_ESCAPE
        || damageSource.other == DEATHLINK_NONE) {
        return;
    }
    
    // Zero our death message to remove old one.
    MemZero(CUSTOM_SAVE_AREA.deathLinkTracker.skyDeathMessage, 128);
    
    // We have to rebuild the death string manually.
    struct preprocessor_args preArgs;
    preArgs.strings[0] = DUNGEON_PTR->fainted_monster_cause_entity_name;
    preArgs.strings[1] = DUNGEON_PTR->fainted_monster_name;
    preArgs.strings[2] = DUNGEON_PTR->fallen_escort_monster_name;
    
    if (SomeDeathMsgCheckFun(damageSource) != 0) {
        strncpy(CUSTOM_SAVE_AREA.deathLinkTracker.skyDeathMessage, StringFromId(0x9CD), 127);
        int used = strlen(CUSTOM_SAVE_AREA.deathLinkTracker.skyDeathMessage);
        if(used < 125) {
            CUSTOM_SAVE_AREA.deathLinkTracker.skyDeathMessage[used] = ' ';
            GetDungeonResultMsg(damageSource,
                CUSTOM_SAVE_AREA.deathLinkTracker.skyDeathMessage + used + 1,
                127 - used - 1, (undefined*)&preArgs);
        }
    } else {
        CUSTOM_SAVE_AREA.deathLinkTracker.skyDeathMessage[0] = ' ';
        GetDungeonResultMsg(damageSource, CUSTOM_SAVE_AREA.deathLinkTracker.skyDeathMessage + 1,
            127 - 1, (undefined*)&preArgs);
    }
    
    CUSTOM_SAVE_AREA.deathLinkTracker.sender = true;
    CUSTOM_SAVE_AREA.deathLinkTracker.receiver = false;
}
