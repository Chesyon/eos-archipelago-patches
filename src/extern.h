#define STARTER_OPTION_VANILLA 0b00
#define STARTER_OPTION_RANDOM 0b01
#define STARTER_OPTION_OVERRIDE 0b10
#define STARTER_OPTION_CHOOSE 0b11

typedef struct ArchipelagoSettings {
    bool recruitment : 1;       // 0 (0x0)
    bool evolution : 1;         // 1 (0x1).
    bool teamFormation : 1;     // 2 (0x2)
    bool levelScaling : 1;      // 3 (0x3)
    bool typesanity : 1;        // 4 (0x4)
    uint8_t starterOptions : 2; // 5 (0x5)
    uint8_t iqMultiplier : 4;   // 7 (0x7)
    bool unused_3 : 1;          // 11 (0xB)
    bool unused_4 : 1;          // 12 (0xC)
    bool unused_5 : 1;          // 13 (0xD)
    bool unused_6 : 1;          // 14 (0xE)
    bool unused_7 : 1;          // 15 (0xF)
} ArchipelagoSettings;

extern ArchipelagoSettings apSettings;

extern char slot_name[16];

extern int seed; // 64 bits = 8 bytes

typedef struct ArchipelagoData {
    char slotName[16];
    unsigned int seed[2];
    ArchipelagoSettings settings;
} ArchipelagoData;

typedef struct MissionStatus {
    uint8_t completedJobs : 8;    // 0x0
    uint8_t totalJobs : 8;        // 0x1
    uint8_t completedOutlaws : 8; // 0x2
    uint8_t totalOutlaws : 8;     // 0x3
} MissionStatus;
ASSERT_SIZE(MissionStatus, 0x4);

typedef struct CustomSaveArea {
    uint32_t checksum;                   // 0x0: Something something validity, ask Adex. The value of this should not change!!
    MissionStatus missionStats[192];     // 0x4: Status of missions for each dungeon in the game.
    uint8_t acquiredRelicFragmentShards; // 0x304
    uint8_t totalRelicFragmentShards;    // 0x305
    uint8_t acquiredInstruments;         // 0x306
    uint8_t totalInstruments;            // 0x307
    undefined fields[0xDF8];             // 0x308: Unused.
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
