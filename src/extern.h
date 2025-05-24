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

#define NUM_HINTABLES 30

#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2

typedef struct ArchipelagoSlotData {
    char apSlotName[16];
    uint32_t apTrimmedSeed[2];
} ArchipelagoSlotData;
ASSERT_SIZE(struct ArchipelagoSlotData, 0x18);

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

extern ArchipelagoSettings apSettings;

typedef struct MissionStatus {
    uint8_t completedJobs : 8;    // 0x0
    uint8_t completedOutlaws : 8; // 0x1
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
    undefined fields[0xE41];             // 0x2BF: Unused.
} CustomSaveArea;
ASSERT_SIZE(CustomSaveArea, 0x1100);

// Some times the functions have returns. Sometimes they don't make them all
// void* because they have differing signatures.
typedef struct TopScreenMode {
    uint32_t thing;
    void* createFunction;
    void* closeFunction;
    void* function3;
    void* function4;
    void* function5;
    void* function6;
    void* function7;
    void* function8;
} TopScreenMode;

extern CustomSaveArea CUSTOM_SAVE_AREA;

extern int PARTNER_SELECT_MENU_OPTION_TRACKER;
extern int PARTNER_SELECT_MENU_OPTION_TIMER;
extern struct vec2 PARTNER_SELECT_PORTRAIT_OFFSETS;
extern int MESSAGE_SET_WAIT_MODE_PARAMS[2];
extern void* UNIONALL_RAM_ADDRESS;

extern struct portrait_params SCRIPT_PORTRAIT_PARAMS;

extern uint32_t TAILORED_MISSION_DUNGEON;

void PlayEffectAnimationEntityWrapper(struct entity* entity, int effect_id);
void DetermineTileAppearence(int x, int y);
void DungeonModeSetupAndShowKeyboard(char* buf1, char* buf2, undefined thing);
int SomeDeathMsgCheckFun(union damage_source damage_source_or_result);
void MessageSetWaitMode(int speed1, int speed2);
void InitScriptRoutineFromCoroutineInfo(struct script_routine* routine, undefined4 param_2, struct coroutine_info* coroutine_info, int status);
void GetSceneName(char* dst, const char* src);
uint32_t GetStringWidth(char* str);

void UnkTopScreenFun1(void* num);
void UnkTopScreenFun2(uint32_t num);
void UnkTopScreenFun3(uint32_t num);
void UnkTopScreenFun4(uint32_t num);
void UnkTopScreenFun5(uint32_t num);
void UnkTopScreenFun6(uint32_t num);
void UnkTopScreenFun7(uint32_t num);
void UnkTopScreenFun8(void* num);
void LoadTopScreenBGPart1(void* thing, uint32_t other); // other is actually a pointer, but lazy
void LoadTopScreenBGPart2(void* thing, char* bg_string, uint32_t num);
void FreeTopScreenBG(void* thing);
void SomethingTopScreenBG(void* thing);
void SetupTopGroundMenuNext();
void SetupMenuNext(void* menu_thing);
void SetupGroundTopScreenFunctions(TopScreenMode* mode);
void SetupGroundTopScreenFunctions2(TopScreenMode* mode);
uint32_t GetTopScreenOptionType();
void InitBackgroundRegionDungeon(uint32_t* regionInfo);
void LoadBackgroundDungeon(void* thing, uint32_t* regionInfo, uint32_t num1, uint32_t num2);
void SetupBackgroundDungeon(void* thing);
void FreeTopScreenBGDungeon(void* thing);
void RemoveEmptyItemsInStorage();
void AnalyzeTextPreamble(undefined param_1, undefined param_2, undefined param_3, char* buffer);

static inline bool IsWithinRange(int x, int min, int max) { return min <= x && x <= max; }
