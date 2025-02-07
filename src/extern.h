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

extern ArchipelagoSettings* apSettings;

extern char slot_name[16];

extern int seed; // 64 bits = 8 bytes

typedef struct ArchipelagoData {
    char slotName[16];
    unsigned int seed[2];
    ArchipelagoSettings settings;
} ArchipelagoData;

typedef struct CustomSaveArea {
    uint32_t checksum;
    undefined fields[0x10FC];
} CustomSaveArea;
ASSERT_SIZE(CustomSaveArea, 0x1100);

extern CustomSaveArea CUSTOM_SAVE_AREA;
