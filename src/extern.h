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

#define HINTABLE_ITEM_COUNT 10
#define ITEM_NAME_LENGTH 42

typedef struct PreviewableItems {
    char names[HINTABLE_ITEM_COUNT][ITEM_NAME_LENGTH];
} PreviewableItems;
ASSERT_SIZE(PreviewableItems, 0x1A4);

extern PreviewableItems hintableItems;

typedef struct DimensionalScreamInfo{
    char who[16];
    char what[32];
    char where[32];
    bool recieved;
} DimensionalScreamInfo;
ASSERT_SIZE(DimensionalScreamInfo, 0x51);

extern DimensionalScreamInfo dimensionalScreamInfo;

typedef struct ArchipelagoSettings { // size: 2 bytes
    uint8_t iqMultiplier : 4;    // 0 (0x0)
    bool earlyMissionFloors : 1; // 4 (0x4)
    bool moveShortcuts : 1;      // 5 (0x5)
    bool levelScaling : 1;       // 6 (0x6)
    bool typesanity : 1;         // 7 (0x7)
    uint8_t starterOptions : 2;  // 8 (0x8)
    uint8_t deathLinkType : 2;   // 10 (0xA)
    uint8_t expMultiplier : 4;   // 12 (0xC)
} ArchipelagoSettings;
ASSERT_SIZE(ArchipelagoSettings, 0x2);

extern ArchipelagoSettings apSettings;

extern char apSlotName[16];

extern uint32_t apSeed[2]; // 64 bits = 8 bytes

extern bool startWithMainGameUnlocked;

typedef struct MissionMax {
    uint8_t totalJobsEarly;    // 0x0
    uint8_t totalOutlawsEarly; // 0x1
    uint8_t totalJobsLate;     // 0x2
    uint8_t totalOutlawsLate;  // 0x3
} MissionMax;
ASSERT_SIZE(MissionMax, 0x4);
extern MissionMax missionMaxes;

typedef struct MacguffinMax {
    uint8_t requiredRelicFragmentShards; // 0x0
    uint8_t requiredInstruments;         // 0x1
} MacguffinMax;
ASSERT_SIZE(MacguffinMax, 0x2);
extern MacguffinMax macguffinMaxes;

typedef struct CafeMax { // See my comment on SP 105.
    uint8_t cafeEventMax; // 0x0
    uint8_t cafeDrinkMax; // 0x1
} CafeMax;
ASSERT_SIZE(CafeMax, 0x2);
extern CafeMax cafeMaxes;

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
    uint32_t checksum;                           // 0x0: Something something validity, ask Adex. The value of this should not change!!
    MissionStatus missionStats[192];             // 0x4: Status of missions for each dungeon in the game.
    uint8_t acquiredRelicFragmentShards;         // 0x184: How many relic fragment shards the player has collected.
    uint8_t acquiredInstruments;                 // 0x185: How many instruments the player has collected.
    DeathLinkTracker deathLinkTracker;           // 0x186: Stores information for deathlink.
    bool hintedItems[HINTABLE_ITEM_COUNT];       // 0x29A: list of which items have been hinted.
    DungeonTraps dungeonTraps;                   // 0x2A4: wip dungeons traps
    uint8_t acquiredCafeEventChecks;             // 0x2A5: How many cafe event checks have been obtained. See my comment on SP 105.
    uint8_t acquiredCafeDrinkChecks;             // 0x2A6: How many cafe drink checks have been obtained.
    bool mainGameUnlocked;                       // 0x2A7: Is main game unlocked?
    DimensionalScreamInfo dimensionalScreamInfo; // 0x2A8: info for dimensional scream hints.
    uint8_t bagSize;                             // 0x2F9: Current bag size. Follows the same logic as vanilla: https://wiki.skytemple.org/index.php/List_of_Script_Variables#Bag_size
    uint8_t trackerPage;                         // 0x2FA: Tracks which tab was last open in the tracker.
    undefined fields[0xE05];                     // 0x2FB: Unused.
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

void UnkTopScreenFun1(uint32_t num);
void UnkTopScreenFun2(uint32_t num);
void UnkTopScreenFun3(uint32_t num);
void UnkTopScreenFun4(uint32_t num);
void UnkTopScreenFun5(uint32_t num);
void UnkTopScreenFun6(uint32_t num);
void UnkTopScreenFun7(uint32_t num);
void UnkTopScreenFun8(uint32_t num);
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
void LoadBackgroundDungeon(uint32_t thing, uint32_t* regionInfo, uint32_t num1, uint32_t num2);

static inline bool IsWithinRange(int x, int min, int max) { return min <= x && x <= max; }
