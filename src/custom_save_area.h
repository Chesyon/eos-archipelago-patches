#pragma once

#include <pmdsky.h>
#include <cot.h>

#define NUM_HINTABLES 30

typedef struct MissionStatus {
    uint8_t completedJobs;    // 0x0
    uint8_t completedOutlaws; // 0x1
} MissionStatus;
ASSERT_SIZE(MissionStatus, 0x2);

// Note: Please set allyDeathMessage before reciever.
typedef struct DeathLinkTracker {
    bool receiver;
    bool sender;
    char skyDeathMessage[128];
    char allyDeathMessage[128];
    char allyDeathName[18];
} DeathLinkTracker;
ASSERT_SIZE(DeathLinkTracker, 0x114);

typedef struct DungeonTraps { // size: 1 byte
    bool maze : 1;      // 0x0
    bool yawn : 1;      // 0x1
    bool whiffer : 1;   // 0x2
    bool dropItems : 1; // 0x3
    bool warp : 1;      // 0x4
    bool weather : 1;   // 0x5
    bool pitfall : 1;   // 0x6
    bool embargo : 1;   // 0x7
} DungeonTraps;
ASSERT_SIZE(DungeonTraps, 0x1);

typedef struct CustomSaveArea {
    uint32_t checksum;                   // 0x0: Something something validity, ask Adex. The value of this should not change!!
    MissionStatus missionStats[192];     // 0x4: Status of missions for each dungeon in the game.
    uint8_t acquiredRelicFragmentShards; // 0x184: How many relic fragment shards the player has collected.
    uint8_t acquiredInstruments;         // 0x185: How many instruments the player has collected.
    DeathLinkTracker deathLinkTracker;   // 0x186: Stores information for deathlink.
    bool hintedItems[NUM_HINTABLES];     // 0x29A: list of which items have been hinted.
    DungeonTraps dungeonTraps;           // 0x2B8: wip dungeons traps
    uint8_t acquiredCafeEventChecks;     // 0x2B9: How many cafe event checks have been obtained. See my comment on SP 105.
    uint8_t acquiredCafeDrinkChecks;     // 0x2BA: How many cafe drink checks have been obtained.
    bool mainGameUnlocked;               // 0x2BB: Is main game unlocked?
    uint8_t bagSize;                     // 0x2BC: Current bag size. Follows the same logic as vanilla: https://wiki.skytemple.org/index.php/List_of_Script_Variables#Bag_size
    uint8_t trackerPage;                 // 0x2BD: Tracks which tab was last open in the tracker.
    uint8_t openSkyPeakPasses;           // 0x2BE: Tracks how many Sky Peak passes are open. I don't know what this is for but Cryptic wanted it.
    uint8_t legendaryListProgress;       // 0x2BF: Tracks how far we are along the legendary list. Requested by Cryptic in her endeavors to stop duplicate legendaries.
    undefined fields[0xE40];             // 0x2C0: Unused.
} CustomSaveArea;
ASSERT_SIZE(CustomSaveArea, 0x1100);

extern CustomSaveArea CUSTOM_SAVE_AREA;