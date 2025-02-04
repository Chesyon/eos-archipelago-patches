extern uint32_t AP_SETTINGS[4];

char slot_name[16];

int seed; // 64 bits = 8 bytes

struct archipelago_flags {
    bool recruitment : 1;        // 0 (0x0)
    bool evolution : 1;        // 1 (0x1).
    bool team_formation : 1;   // 2 (0x2)
    bool level_scaling : 1; // 3 (0x3)
    bool typesanity : 1;           // 4 (0x4)
    bool unused_0 : 1;      // 5 (0x5)
    bool unused_1 : 1;      // 6 (0x6)
    bool unused_2 : 1;      // 7 (0x7)
    bool unused_3 : 1;      // 8 (0x8)
    bool unused_4 : 1;      // 9 (0x9)
    bool unused_5 : 1;      // 10 (0xA)
    bool unused_6 : 1;      // 11 (0xB)
    bool unused_7 : 1;      // 12 (0xC)
    bool unused_8 : 1;      // 13 (0xD)
    bool unused_9 : 1;      // 14 (0xE)
    bool unused_10 : 1;      // 15 (0xF)
};

int starter_option; // 4 options - 2 bits are needed to store 0-3
