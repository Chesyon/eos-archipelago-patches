#define STARTER_OPTION_VANILLA 0b00
#define STARTER_OPTION_RANDOM 0b01
#define STARTER_OPTION_OVERRIDE 0b10
#define STARTER_OPTION_CHOOSE 0b11

typedef struct ArchipelagoSettings { // size: 2 bytes
    uint8_t iqMultiplier : 4;   // 0 (0x0)
    bool recruitment : 1;       // 4 (0x4)
    bool evolution : 1;         // 5 (0x5)
    bool teamFormation : 1;     // 6 (0x6)
    bool levelScaling : 1;      // 7 (0x7)
    bool typesanity : 1;        // 8 (0x8)
    uint8_t starterOptions : 2; // 10 (0x9)
    bool unused_1 : 1;          // 11 (0xB)
    bool unused_2 : 1;          // 12 (0xC)
    bool unused_3 : 1;          // 13 (0xD)
    bool unused_4 : 1;          // 14 (0xE)
    bool unused_5 : 1;          // 15 (0xF)
} ArchipelagoSettings;
ASSERT_SIZE(ArchipelagoSettings, 0x2);

extern ArchipelagoSettings apSettings;

extern char slot_name[16];

extern int seed; // 64 bits = 8 bytes

typedef struct MissionMax {
    uint8_t totalJobs;    // 0x0
    uint8_t totalOutlaws; // 0x1
} MissionMax;
ASSERT_SIZE(MissionMax, 0x2);
extern MissionMax missionMaxes[192];

typedef struct MacguffinMax {
    uint8_t totalRelicFragmentShards; // 0x0
    uint8_t totalInstruments;         // 0x1
} MacguffinMax;
ASSERT_SIZE(MacguffinMax, 0x2);
extern MacguffinMax macguffinMaxes;

typedef struct MissionStatus {
    uint8_t completedJobs : 8;    // 0x0
    uint8_t completedOutlaws : 8; // 0x1
} MissionStatus;
ASSERT_SIZE(MissionStatus, 0x2);

typedef struct CustomSaveArea {
    uint32_t checksum;                   // 0x0: Something something validity, ask Adex. The value of this should not change!!
    MissionStatus missionStats[192];     // 0x4: Status of missions for each dungeon in the game.
    uint8_t acquiredRelicFragmentShards; // 0x184: How many relic fragment shards the player has collected.
    uint8_t acquiredInstruments;         // 0x185: How many instruments the player has collected.
    undefined fields[0xF7A];             // 0x186: Unused.
} CustomSaveArea;
ASSERT_SIZE(CustomSaveArea, 0x1100);

extern CustomSaveArea CUSTOM_SAVE_AREA;

extern int PARTNER_SELECT_MENU_OPTION_TRACKER;
extern int PARTNER_SELECT_MENU_OPTION_TIMER;
extern struct vec2 PARTNER_SELECT_PORTRAIT_OFFSETS;

/*typedef struct SomeTeamSetupThing {
    struct monster_id_16 hero_id;
    struct monster_id_16 partner_id;
    uint8_t unk_0x4[0x14]; // maybe like hero name idk
    char partnerName[0x14]; // maybe only 11 long idk
    int auraColor;
} SomeTeamSetupThing;

undefined UNK_FUN_2039514_EU(void);
undefined UNK_FUN_2038AB0_EU(SomeTeamSetupThing* thing);*/
