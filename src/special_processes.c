#include <pmdsky.h>
#include <cot.h>
#include <ap_utils.h>
#include "extern.h"
#include "crass.h"

#if CUSTOM_SPECIAL_PROCESSES

// Special process 100: Returns if guest party members could cause problems for the player.
static int SpGetLevelScalingStatus() {
  return apSettings.levelScalingMode != LEVEL_SCALING_OFF && !apSettings.levelScaleGuests;
}

// Special process 101: Read/write mission status struct. First parameter: Read/Write. Second parameter: Jobs/outlaws
static int SpAccessMissionStatuses(short mode, short missionType) {
    int dungeonId = LoadScriptVariableValue(0, 41);
    enum dungeon_check_type dungeonCt = GetDungeonCheckType(dungeonId);
    if(dungeonCt == DCT_OTHER || dungeonCt == DCT_RULE) {
        return 255; // This should be larger than any mission 
    }
    
    int totalNumber;
    MissionStatus* missionStats = &(CUSTOM_SAVE_AREA.missionStats[dungeonId]); // get mission stats pointer for the dungeon specified in DUNGEON_ENTER_INDEX
    // load either jobs or outlaws:
    if (missionType == 1) { // outlaws
        if(dungeonCt == DCT_LATE) totalNumber = apSettings.totalOutlawsLate;
        else totalNumber = apSettings.totalOutlawsEarly;
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
        if(dungeonCt == DCT_LATE) totalNumber = apSettings.totalJobsLate;
        else totalNumber = apSettings.totalJobsEarly;
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
            if (CUSTOM_SAVE_AREA.acquiredInstruments < apSettings.requiredInstruments) {
                CUSTOM_SAVE_AREA.acquiredInstruments++; // increment by one
                numberToReturn = 1;
            }
            else numberToReturn = 0;
        }
        else numberToReturn = apSettings.requiredInstruments - CUSTOM_SAVE_AREA.acquiredInstruments; // Read mode
    }
    else { // relic fragment
        if (mode == 1) { // Write mode
            if (CUSTOM_SAVE_AREA.acquiredRelicFragmentShards < apSettings.requiredRelicFragmentShards) {
                CUSTOM_SAVE_AREA.acquiredRelicFragmentShards++; // increment by one
                numberToReturn = 1;
            }
            else numberToReturn = 0;
        }
        else numberToReturn = apSettings.requiredRelicFragmentShards - CUSTOM_SAVE_AREA.acquiredRelicFragmentShards; // Read mode
    }
    if (numberToReturn < 0) return 0;
    else return numberToReturn;
}

// Special process 103: Checks if HERO_FIRST_NAME matches any name in a list of dev names. Returns the index of the name plus one if there's a match, otherwise returns zero.
static int SpDoNameCheck() {
    #define num_of_names 7
    char name_check_names[num_of_names][10] = {"chesyon", "happylappy", "cryptic", "kattnip", "hecka", "tailsdk", "fieryblizz"};
    char name[10];
    LoadScriptVariableValueBytes(VAR_HERO_FIRST_NAME, name, 10);
    for (int i = 0; i < 10 && name[i] != NULL; i++) name[i] = (name[i] >= 'A' && name[i] <= 'Z' ? name[i] + 0x20 : name[i]); // Convert any uppercase characters to lowercase, so no case-sensitivity. This is done by adding 0x20 to each character value if it's between A and Z.
    for (int i = 0; i < num_of_names; i++) if (strncmp(name, name_check_names[i], 10) == 0) return i+1;
    return 0;
}

// Special process 104: Regenerates board missions.
static int SpRegenerateMissions() {
    GenerateDailyMissions();
    return 0;
}

// Special process 105: Read/write Cafe check count. 1 = write, anything else = 0.
// This is a really lazy implementation based on SP 102. Could be merged into SP 102 if I actually bothered to reorganize the structs, which... I don't feel like doing right now. This is future Chesyon's problem.
// Future Chesyon here: Fuck you past Chesyon. (in all seriousness the way to co about merging them would to be to convert the MacguffinMaxes struct into a single Maxes struct. I still don't feel like doing this, so future future Chesyon's problem.
// At the very least this won't require save struct adjustments. Probably. You know, *with* some save struct adjustments, I could actually merge into SP 101 as well... 101, 102, and 105 are all identical logic.
// Future future Chesyon here: Yeah merging these is definitely feasible now. I need to make sure Lappy won't kill me for doing it though.
static int SpAccessCafeStatus(short mode, short type) {
    if (type == 0) { // Cafe Events
        if (mode == 1) { // Write mode
            if (CUSTOM_SAVE_AREA.acquiredCafeEventChecks < apSettings.cafeEventMax) {
                CUSTOM_SAVE_AREA.acquiredCafeEventChecks++; // increment by one
                return 1;
            }
            else return 0;
        }
        else return apSettings.cafeEventMax - CUSTOM_SAVE_AREA.acquiredCafeEventChecks; // Read mode
    }
    else { // Cafe Drinks
        if (mode == 1) { // Write mode
            if (CUSTOM_SAVE_AREA.acquiredCafeDrinkChecks < apSettings.cafeDrinkMax) {
                CUSTOM_SAVE_AREA.acquiredCafeDrinkChecks++; // increment by one
                return 1;
            }
            else return 0;
        }
        else return apSettings.cafeDrinkMax - CUSTOM_SAVE_AREA.acquiredCafeDrinkChecks; // Read mode
    }
}

// Special process 106: Check for Unown Rocks. No parameters.
// Used by the Unown curse. Finds the first item in the bag that is a Unown rock and returns the item ID. If none is found, return 0.
static int SpCheckForUnownRocks(){
    int BagSize = GetCurrentBagCapacity(); // may be redundant? idk what happens if we call GetItemAtIdx on an index that the bag isn't large enough for.
    for (int i = 0; i < BagSize; i++) {
        int item_id = GetItemAtIdx(i)->id.val;
        if(item_id >= 400 && item_id <= 427) {  // is the item id a unown rock?
            RemoveItem(i);
            return item_id;
        }
    }
    return 0;
}

// Special process 107: Is the main game unlocked? No parameters.
static bool SpIsMainGameUnlocked(){
    return (CUSTOM_SAVE_AREA.mainGameUnlocked | (apSettings.startingEpisode == 0));
}


// Used by SP 108.
int __attribute__((naked)) GetItemIdAtStorageIdx(int index)
{
    // get BAG_ITEMS to r0
    asm("ldr r1,=BAG_ITEMS_PTR_MIRROR");
    asm("ldr r1,[r1]");
    // get the location in BAG_ITEMS by multiplying the index by 2 and adding 0x38A.
    asm("ldr r2,=0x38A");
    asm("add r0,r2,r0,lsl #1");
    // access the id from the bag
    asm("ldrsh r0,[r1,r0]");
    asm("bx lr");
    asm(".pool");
}

int __attribute__((naked)) GetItemQuantityAtStorageIdx(int index)
{
    // get BAG_ITEMS to r1
    asm("ldr r1,=BAG_ITEMS_PTR_MIRROR");
    asm("ldr r1,[r1]");
    // get the location in BAG_ITEMS by multiplying the index by 2 and adding 0x38A.
    asm("ldr r2,=0xB5A");
    asm("add r0,r2,r0,lsl #1");
    // access the quantity from the bag
    asm("ldrsh r0,[r1,r0]");
    asm("bx lr");
    asm(".pool");
}

void __attribute__((naked)) SetQuantityOfStorageItem(int index, int amountToSet)
{
    // get BAG_ITEMS to r1
    asm("ldr r2,=BAG_ITEMS_PTR_MIRROR");
    asm("ldr r2,[r2]");
    // get the location in BAG_ITEMS to r0 by multiplying the index by 2 and adding 0x38A.
    asm("ldr r3,=0xB5A");
    asm("add r0,r3,r0,lsl #1");
    // save the new quantity back to BAG_ITEMS.
    asm("strh r1,[r2,r0]");
    asm("bx lr");
    asm(".pool");
}

// Special process 108: Recycle shop stuff. 
static int SpRecycleShopStuff(int itemSetId1, int itemSetId2){
    if(IsBagFull() && IsStorageFull()) {
        return 3; // the player does not have the needed space to recieve the output item.
    }
    
    struct bulk_item itemToRemove;
    ItemAtTableIdx(itemSetId1, &itemToRemove);
    int idRequired = itemToRemove.id.val;
    int amountToRemove = itemToRemove.quantity;
    
    int numItemInBag = CountItemTypeInBag(idRequired);
    if (numItemInBag + CountItemTypeInStorage(&itemToRemove) < itemToRemove.quantity) {
        return 2; // player does not have the required amount of the needed item.
    }
    
    struct bulk_item itemToAdd;
    ItemAtTableIdx(itemSetId2, &itemToAdd);
    int bagSize = GetCurrentBagCapacity();
    
    // Amount to remove is assumed to be more than 0 at the start.
    if(numItemInBag >= 0) {
        for(int i = 0; i < bagSize; i++) {
            struct item* thisItem = GetItemAtIdx(i);
            if(thisItem->id.val == idRequired) {
                if(thisItem->quantity <= amountToRemove) {
                    amountToRemove -= thisItem->quantity;
                    ItemZInit(thisItem);
                } else {
                    thisItem->quantity -= amountToRemove;
                    amountToRemove = 0;
                    break;
                }
            }
        }
    }
    
    if(amountToRemove > 0) {
        int storageSize = GetRankStorageSize();
        for(int i = 0; i < storageSize; i++) {
            if (GetItemIdAtStorageIdx(i) == idRequired){
                int itemQuantity = GetItemQuantityAtStorageIdx(i);
                if(itemQuantity > amountToRemove) {
                    SetQuantityOfStorageItem(i, itemQuantity - amountToRemove);
                    // we don't need amountToRemove past this point so we can just break without updating it.
                    break;
                }
                else {
                    amountToRemove -= itemQuantity;
                    RemoveItemAtIdxInStorage(i);
                    RemoveEmptyItemsInStorage();
                    // if this would be exactly enough or less, lower the amount to remove by this quantity and delete the item.
                }
            }
        }
    }
    
    // Double check for places to stack in the inventory first.
    RemoveEmptyItemsInBag();
    int amountToAdd = itemToAdd.quantity;
    for(int i = 0; i < bagSize; i++) {
        struct item* thisItem = GetItemAtIdx(i);
        if(thisItem->id.val == itemToAdd.id.val) {
            uint16_t newQuantity = thisItem->quantity + amountToAdd;
            if(newQuantity > 99) {
                thisItem->quantity = 99;
                amountToAdd = newQuantity - 99;
            } else {
                thisItem->quantity = newQuantity;
                return 0; // Item succesfully stacked in bag.
            }
        } else if (thisItem->id.val == ITEM_NOTHING) {
            thisItem->f_exists = true;
            thisItem->id.val = itemToAdd.id.val;
            thisItem->quantity = amountToAdd;
            return 0; // Item succesfully added to bag.
        }
    }
    
    // Hunt down spaces in storage to stack the item.
    if (amountToAdd > 0) {
        int storageSize = GetRankStorageSize();
        for(int i = 0; i < storageSize; i++) {
            if (GetItemIdAtStorageIdx(i) == itemToAdd.id.val){
                uint16_t newQuantity = GetItemQuantityAtStorageIdx(i) + amountToAdd;
                if(newQuantity > 99) {
                    SetQuantityOfStorageItem(i, 99);
                    amountToAdd = newQuantity - 99;
                } else {
                    SetQuantityOfStorageItem(i, newQuantity);
                    return 1; // Item succesfully stacked in storage.
                }
            }
        }
    }
    
    // If any remain add it as a new stack to storage.
    itemToAdd.quantity = amountToAdd;
    AddBulkItemToStorage(&itemToAdd);
    return 1; // Add item to storage as a new item.
}

// Special process 109: Get rank. No parameters, returns rank as enum: https://github.com/UsernameFodder/pmdsky-debug/blob/master/headers/types/ground_mode/enums.h#L2079
static int SpGetRank(){
    return GetRank();
}

// Special process 110: Set portrait monster and emotion. First parameter: Monster ID, second parameter: emotion.
static int SpSetPortraitMonster(int monsterId, int emotion){
    SCRIPT_PORTRAIT_PARAMS.monster_id.val = monsterId;
    SetPortraitEmotion(&SCRIPT_PORTRAIT_PARAMS, emotion);
    return 0;
}

// Special Process 111: Assign check. Parameter 1: Check ID. Currently unused as I need to test the hell out of it to make sure it has the same behavior as Lappy's macro.
static int SpAssignCheck(int checkId){
    int subXVar = 5;
    int num = checkId >> 4;
    subXVar += num;
    checkId -= num << 4;
    uint16_t val = 0;
    enum script_var_id var_id = subXVar;
    LoadScriptVariableValueBytes(var_id, &val, 2);
    val |= 1 << checkId;
    SaveScriptVariableValueBytes(var_id, &val, 2);
    return 0;
}

// Special process 112: Get current bag capacity. No parameters, returns current bag capacity.
static int SpGetCurrentBagCapacity(){
    return GetCurrentBagCapacity();
}

// Special Process 113: Get if a hint has been checked or not. Takes hint idx as input, returns 0 if unchecked, 1 if checked, 2 if idx is invalid.
static int SpIsHintChecked(short idx){
    if(idx >= NUM_HINTABLES) return 2;
    else return (int)CUSTOM_SAVE_AREA.hintedItems[idx];
}

// Special Process 114: Get starting episode. No parameters, returns starting episode. 0 = Main Episode, 1 = Bidoof, 2 = Igglybuff, 3 = Sunflora, 4 = Team Charm, 5 = Dark Future
static int SpGetStartingEpisode() {
	return apSettings.startingEpisode;
}

// Special Process 115: Gets current number of open Sky Peak passes. No parameters.
static int SpGetSkyPeaksOpen() {
    return CUSTOM_SAVE_AREA.openSkyPeakPasses;
}

// Special process 116: Updates adventure log recycle count. No params/return, but instead uses EVENT_LOCAL as both a parameter and output variable.
// Input: Number to increase adventure log recycle count by.
// Output: New adventure log recycle count.
// NOTE: Couldn't we just... read from VAR_RECYCLE_COUNT...? Eh, lappy asked for it to be implemented this way. Come back to this.
static void SpUpdateAdventureLogRecycles(){
    uint32_t num_to_add = LoadScriptVariableValue(NULL, VAR_EVENT_LOCAL);
    uint32_t recycle_count = GetNbRecycled() + num_to_add;
    if(recycle_count > 999999) recycle_count = 999999; // for consistency with what'll be shown in adventure log
    SetNbRecycled(recycle_count);
    SaveScriptVariableValue(NULL, VAR_EVENT_LOCAL, recycle_count);
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

// Special process 255: Return either the current cutscene_skip_settings::crass_kind value or the ID of the OPCODE_MESSAGE_MENU that was skipped.
static int SpGetCrassKind() {
    #if CANCEL_RECOVER_ACTING_SKIP_SYSTEM
    return CRASS_SETTINGS.menu_skipped > 0 ? CRASS_SETTINGS.menu_skipped : CRASS_SETTINGS.crass_kind;
    #else
    return 0;
    #endif
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
    case 104:
        *return_val = SpRegenerateMissions();
        return true;
    case 105:
        *return_val = SpAccessCafeStatus(arg1, arg2);
        return true;
    case 106:
        *return_val = SpCheckForUnownRocks();
        return true;
    case 107:
        *return_val = SpIsMainGameUnlocked();
        return true;
    case 108:
        *return_val = SpRecycleShopStuff(arg1, arg2);
        return true;
    case 109:
        *return_val = SpGetRank();
        return true;
    case 110:
        *return_val = SpSetPortraitMonster(arg1, arg2);
        return true;
    case 111:
        *return_val = SpAssignCheck(arg1);
        return true;
    case 112:
        *return_val = SpGetCurrentBagCapacity();
        return true;
    case 113:
        *return_val = SpIsHintChecked(arg1);
        return true;
    case 114:
        *return_val = SpGetStartingEpisode();
        return true;
    case 115:
        *return_val = SpGetSkyPeaksOpen();
        return true;
    case 116:
        SpUpdateAdventureLogRecycles();
        return true;
    case 255:
        *return_val = SpGetCrassKind();
        return true;
    default:
        return false;
  }
}

#endif
