#pragma once

#include <pmdsky.h>
#include <cot.h>

#define STARTER_OPTION_VANILLA 0b00
#define STARTER_OPTION_RANDOM 0b01
#define STARTER_OPTION_OVERRIDE 0b10
#define STARTER_OPTION_CHOOSE 0b11

#define DEATHLINK_NONE 0b00
#define DEATHLINK_REVIVER 0b01
#define DEATHLINK_TRUE_DEATH 0b10

#define LEVEL_SCALING_OFF 0b00
#define LEVEL_SCALING_EASY 0b01
#define LEVEL_SCALING_DIFFICULT 0b10

typedef struct ArchipelagoSlotData {
    char apSlotName[16];
    char apSlotNameCleaned[16];
    uint32_t apTrimmedSeed[2];
} ArchipelagoSlotData;
ASSERT_SIZE(struct ArchipelagoSlotData, 0x28);

extern ArchipelagoSlotData slotData;

// Q: What's up with all the unused values?
// A: Alignment. If a value spans multiple bytes, CoT gets angry and won't compile it properly, so we need a little bit of padding to ensure each value doesn't span multiple bytes. We *can* put these unused values to use as long as they use that bit count!
typedef struct ArchipelagoSettings { // size: 5 bytes
    uint8_t iqMultiplier : 4;                // bits 1 through 4
    bool earlyMissionFloors : 1;             // bit 5
    bool moveShortcuts : 1;                  // bit 6
    undefined unused_1 : 1;                  // bit 7
    bool typesanity : 1;                     // bit 8
    uint8_t starterOptions : 2;              // bits 9 and 10
    uint8_t deathLinkType : 2;               // bits 11 and 12
    uint8_t expMultiplier : 4;               // bits 13 through 16
    undefined unused_2 : 1;                  // bit 17
    bool dialgaIsGoal : 1;                   // bit 18
    bool longLocationsOn : 1;                // bit 19
    uint8_t totalJobsEarly : 5;              // bits 20 through 24
    uint8_t levelScalingMode : 2;            // bits 25 and 26
    bool levelScaleGuests : 1;               // bit 27
    uint8_t totalOutlawsEarly : 5;           // bits 28 through 32
    uint8_t startingEpisode : 3;             // bits 33 through 35
    uint8_t totalJobsLate : 5;               // bits 36 through 40
    undefined unused_3 : 3;                  // bits 41 through 43
    uint8_t totalOutlawsLate : 5;            // bits 44 through 48
    uint8_t requiredRelicFragmentShards : 4; // bits 49 through 52
    uint8_t requiredInstruments : 4;         // bits 53 through 56
    uint8_t cafeEventMax : 5;                // bits 57 through 61
    undefined unused_4 : 3;                  // bits 62 through 64
    uint8_t cafeDrinkMax : 5;                // bits 65 through 69
    undefined unused_5 : 3;                  // bits 70 through 72
} ArchipelagoSettings;
ASSERT_SIZE(struct ArchipelagoSettings, 0x9);

typedef struct ArchipelagoNums {
    uint8_t expMultiplier;               // 0x0
    uint8_t iqMultiplier;                // 0x1
    uint8_t levelScalingMode;            // 0x2
    uint8_t goal;                        // 0x3: For the time being, 0x0=Darkrai, 0x1=Dialga. I know, it should be the other way around, but this used to be a bool called dialgaIsGoal. I'll fix it when we get to recruitemall.
    uint8_t startingEpisode;             // 0x4
    uint8_t deathLinkType;               // 0x5
    uint8_t totalJobsEarly;              // 0x6
    uint8_t totalOutlawsEarly;           // 0x7
    uint8_t totalJobsLate;               // 0x8
    uint8_t totalOutlawsLate;            // 0x9
    uint8_t starterOptions;              // 0xA
    uint8_t requiredRelicFragmentShards; // 0xB
    uint8_t requiredInstruments;         // 0xC
    uint8_t cafeDrinkMax;                // 0xD
    uint8_t cafeEventMax;                // 0xE
} ArchipelagoNums;
ASSERT_SIZE(struct ArchipelagoNums, 0xF);

typedef struct ArchipelagoFlags {
    bool earlyMissionFloors : 1; // bit 0 : 0b1
    bool moveShortcuts : 1;      // bit 1 : 0b10
    bool typesanity : 1;         // bit 2 : 0b100
    bool longLocationsOn : 1;    // bit 3 : 0b1000
    bool levelScaleGuests : 1;   // bit 4 : 0b10000
    bool specialEpisodesOn : 1;  // but 5 : 0b100000 TODO client side
} ArchipelagoFlags;
ASSERT_SIZE(struct ArchipelagoFlags, 0x1);

typedef struct NewArchipelagoSettings {
    ArchipelagoNums nums;   // 0x0
    ArchipelagoFlags flags; // 0xF
} NewArchipelagoSettings;
ASSERT_SIZE(struct NewArchipelagoSettings, 0x10);

extern NewArchipelagoSettings newApSettings;
extern ArchipelagoSettings apSettings;