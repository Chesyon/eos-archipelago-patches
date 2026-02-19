#include <pmdsky.h>
#include <cot.h>
#include "ap_utils.h"

// TRACKER_CHECK_BUNDLE bundles a bunch of check with a null terminator check at the end.
#define TRACKER_CHECK_BUNDLE(...) {__VA_ARGS__ , {.check_str_id = 0, .type = TRACKER_CHECK_TERMINATOR}}
#define TRACKER_EMPTY_CHECK_BUNDLE() {{.check_str_id = 0, .type = TRACKER_CHECK_TERMINATOR}}
#define DUNGEON_NONE DUNGEON_TEST_DUNGEON

// TODO: Adapt this for NA when we want to support NA version.
#define TOWN_STR_ID 16814
#define GUILD_STR_ID 16438
#define CAFE_STR_ID 16550
#define HABITAT_STR_ID 16439
#define DOJO_STR_ID 16562
#define AEGIS_CAVE_STR_ID 16880

#define SHOP_CHECK_STR_ID 0
#define BANK_CHECK_STR_ID 0
#define TEAM_NAME_CHECK_STR_ID 0
#define BAG_UPGRADE_CHECK_STR_ID 0
#define BOSS_GIFT_CHECK_STR_ID 0

#define START_ITEM_INFO_STR_ID 0
#define BOSS_INFO_STR_ID 0
#define BOSS_DUO_INFO_STR_ID 0
#define ESCORT_INFO_STR_ID 0

enum tracker_page {
    TRACKER_PAGE_TOWN,
    TRACKER_PAGE_GUILD,
    TRACKER_PAGE_DOJO,
    TRACKER_PAGE_CAFE,
    TRACKER_PAGE_HABITAT, // Aka Sharpedo Bluff
    TRACKER_PAGE_BEACH_CAVE,
    TRACKER_PAGE_DRENCHED_BLUFF,
    TRACKER_PAGE_MT_BRISTLE,
    TRACKER_PAGE_WATERFALL_CAVE,
    TRACKER_PAGE_APPLE_WOODS,
    TRACKER_PAGE_CRAGGY_COAST,
    TRACKER_PAGE_SIDE_PATH,
    TRACKER_PAGE_MT_HORN,
    TRACKER_PAGE_ROCK_PATH,
    TRACKER_PAGE_FOGGY_FOREST,
    TRACKER_PAGE_FOREST_PATH,
    TRACKER_PAGE_STEAM_CAVE,
    TRACKER_PAGE_AMP_PLAINS,
    TRACKER_PAGE_NORTHERN_DESERT,
    TRACKER_PAGE_QUICKSAND_CAVE,
    TRACKER_PAGE_CRYSTAL_CAVE,
    TRACKER_PAGE_CRYSTAL_CROSSING,
    TRACKER_PAGE_CHASM_CAVE,
    TRACKER_PAGE_DARK_HILL,
    TRACKER_PAGE_SEALED_RUIN,
    TRACKER_PAGE_DUSK_FOREST,
    TRACKER_PAGE_DEEP_DUSK_FOREST,
    TRACKER_PAGE_TREESHROUD_FOREST,
    TRACKER_PAGE_BRINE_CAVE,
    TRACKER_PAGE_SERENITY_RIVER,
    TRACKER_PAGE_LANDSLIDE_CAVE,
    TRACKER_PAGE_LUSH_PRAIRIE,
    TRACKER_PAGE_TINY_MEADOW,
    TRACKER_PAGE_LABYRINTH_CAVE,
    TRACKER_PAGE_ORAN_FOREST,
    TRACKER_PAGE_STAR_CAVE,
    TRACKER_PAGE_HIDDEN_LAND,
    TRACKER_PAGE_TEMPORAL_TOWER,
    TRACKER_PAGE_MYSTIFYING_FOREST,
    TRACKER_PAGE_BLIZZARD_ISLAND,
    TRACKER_PAGE_CREVICE_CAVE,
    TRACKER_PAGE_SURROUNDED_SEA,
    TRACKER_PAGE_MIRACLE_SEA,
    TRACKER_PAGE_AEGIS_CAVE, // DUNGEON_ICE_AEGIS_CAVE is first in this dungeon group.
    TRACKER_PAGE_MT_TRAVAIL,
    TRACKER_PAGE_THE_NIGHTMARE,
    TRACKER_PAGE_SPACIAL_RIFT,
    TRACKER_PAGE_CONCEALED_RUINS,
    TRACKER_PAGE_MARINE_RESORT,
    TRACKER_PAGE_BOTTOMLESS_SEA, // 7 Treasure
    TRACKER_PAGE_SHIMMER_DESERT, // 7 Treasure
    TRACKER_PAGE_MT_AVALANCHE,   // 7 Treasure
    TRACKER_PAGE_GIANT_VOLCANO,  // 7 Treasure
    TRACKER_PAGE_WORLD_ABYSS,    // 7 Treasure
    TRACKER_PAGE_SKY_STAIRWAY,   // 7 Treasure
    TRACKER_PAGE_MYSTERY_JUNGLE, // 7 Treasure
    TRACKER_PAGE_LAKE_AFAR,
    TRACKER_PAGE_HAPPY_OUTLOOK,
    TRACKER_PAGE_MY_MISTRAL,
    TRACKER_PAGE_SHIMMER_HILL,
    TRACKER_PAGE_LOST_WILDERNESS,
    TRACKER_PAGE_MIDNIGHT_FOREST,
    TRACKER_PAGE_1ST_STATION_PASS,
    TRACKER_PAGE_2ND_STATION_PASS,
    TRACKER_PAGE_3RD_STATION_PASS,
    TRACKER_PAGE_4TH_STATION_PASS,
    TRACKER_PAGE_5TH_STATION_PASS,
    TRACKER_PAGE_6TH_STATION_PASS,
    TRACKER_PAGE_7TH_STATION_PASS,
    TRACKER_PAGE_8TH_STATION_PASS,
    TRACKER_PAGE_9TH_STATION_PASS,
    TRACKER_PAGE_SKY_PEAK_SUMMIT_PASS,
    TRACKER_PAGE_SKY_PEAK_SUMMIT,
    TRACKER_PAGE_ZERO_ISLE_NORTH,
    TRACKER_PAGE_ZERO_ISLE_EAST,
    TRACKER_PAGE_ZERO_ISLE_WEST,
    TRACKER_PAGE_ZERO_ISLE_SOUTH,
    TRACKER_PAGE_ZERO_ISLE_CENTER,
    TRACKER_PAGE_DESTINY_TOWER,
    TRACKER_PAGE_OBLIVION_FOREST,
    TRACKER_PAGE_TREACHEROUS_WATERS,
    TRACKER_PAGE_SOUTHEASTERN_ISLANDS,
    TRACKER_PAGE_INFERNO_CAVE,
    TRACKER_PAGE_SE_BIDOOFS_WISH,
    TRACKER_PAGE_SE_IGGLYBUFF_THE_PRODIGY,
    TRACKER_PAGE_SE_TODAYS_OH_MY_GOSH,
    TRACKER_PAGE_SE_HERE_COMES_TEAM_CHARM,
    TRACKER_PAGE_SE_INTO_THE_FUTURE_OF_DARKNESS,
    TRACKER_PAGE_DARK_CRATER,
};

int DrawTextInTrackerWindow(char* string);

// Force the enum to be as small as possible for better struct
// memory usage.
enum __attribute__((__packed__)) tracker_check_type {
    TRACKER_CHECK_TERMINATOR = 0,
    TRACKER_CHECK_INFO,         // Not actually a check.
    TRACKER_CHECK_MONSTER_INFO, // Not actually a check.
    TRACKER_CHECK_SUBX,
    TRACKER_CHECK_NUMBERED_SUBX,
    TRACKER_CHECK_ID_VAL_SUBX,
};

struct monster_info {
    struct monster_id_16 monster_0;
    struct monster_id_16 monster_1;
};

struct subx_check {
    uint8_t subx_bit;
};

struct numbered_subx_check {
    uint8_t subx_bit;
    uint32_t number;
};

struct id_val_subx_check {
    uint8_t subx_bit;
    uint32_t id_val;
};

struct dungeon_conquest_check {
    struct dungeon_id_8 dungeon;
};

union check_data {
    struct monster_info monster_info;
    struct subx_check subx_check;
    struct numbered_subx_check numbered_subx_check;
    struct id_val_subx_check id_val_subx_check;
};


struct tracker_check {
    uint16_t check_str_id;
    enum tracker_check_type type;
    union check_data data;
    bool (*is_check_active_func)(); // NULL means the check is always active.
};

struct tracker_page_entry {
    uint16_t name_str_id;
    struct dungeon_id_16 dungeon;
    bool (*is_page_active_func)(); // NULL means page is always active.
    struct tracker_check checks[];
};

struct tracker_page_entry tracker_book[] = {
    [TRACKER_PAGE_TOWN] = {
        .name_str_id = TOWN_STR_ID,
        .checks = TRACKER_CHECK_BUNDLE(
            {
                .check_str_id = SHOP_CHECK_STR_ID,
                .type = TRACKER_CHECK_NUMBERED_SUBX,
                .data = {.numbered_subx_check = {
                    .subx_bit = 10, .number = 1
                }}
            },
            {
                .check_str_id = SHOP_CHECK_STR_ID,
                .type = TRACKER_CHECK_NUMBERED_SUBX,
                .data = {.numbered_subx_check = {
                    .subx_bit = 11, .number = 2
                }}
            },
            {
                .check_str_id = SHOP_CHECK_STR_ID,
                .type = TRACKER_CHECK_NUMBERED_SUBX,
                .data = {.numbered_subx_check = {
                    .subx_bit = 12, .number = 3
                }}
            },
            {
                .check_str_id = SHOP_CHECK_STR_ID,
                .type = TRACKER_CHECK_NUMBERED_SUBX,
                .data = {.numbered_subx_check = {
                    .subx_bit = 13, .number = 4
                }}
            },
            {
                .check_str_id = SHOP_CHECK_STR_ID,
                .type = TRACKER_CHECK_NUMBERED_SUBX,
                .data = {.numbered_subx_check = {
                    .subx_bit = 14, .number = 5
                }}
            },
            {
                .check_str_id = SHOP_CHECK_STR_ID,
                .type = TRACKER_CHECK_NUMBERED_SUBX,
                .data = {.numbered_subx_check = {
                    .subx_bit = 15, .number = 6
                }}
            },
            {
                .check_str_id = SHOP_CHECK_STR_ID,
                .type = TRACKER_CHECK_NUMBERED_SUBX,
                .data = {.numbered_subx_check = {
                    .subx_bit = 16, .number = 7
                }}
            },
            {
                .check_str_id = SHOP_CHECK_STR_ID,
                .type = TRACKER_CHECK_NUMBERED_SUBX,
                .data = {.numbered_subx_check = {
                    .subx_bit = 17, .number = 8
                }}
            },
            {
                .check_str_id = SHOP_CHECK_STR_ID,
                .type = TRACKER_CHECK_NUMBERED_SUBX,
                .data = {.numbered_subx_check = {
                    .subx_bit = 18, .number = 9
                }}
            },
            {
                .check_str_id = SHOP_CHECK_STR_ID,
                .type = TRACKER_CHECK_NUMBERED_SUBX,
                .data = {.numbered_subx_check = {
                    .subx_bit = 19, .number = 10
                }}
            }
        )
    },
    [TRACKER_PAGE_GUILD] = {
        .name_str_id = GUILD_STR_ID,
    },
    [TRACKER_PAGE_DOJO] = {
        .name_str_id = DOJO_STR_ID,
    },
    [TRACKER_PAGE_CAFE] = {
        .name_str_id = CAFE_STR_ID,
    },
    [TRACKER_PAGE_HABITAT] = {
        .name_str_id = HABITAT_STR_ID,
    },
    [TRACKER_PAGE_BEACH_CAVE] = {
        .dungeon = DUNGEON_BEACH_CAVE,
        .checks = TRACKER_CHECK_BUNDLE(
            {
                .check_str_id = TEAM_NAME_CHECK_STR_ID,
                .type = TRACKER_CHECK_SUBX,
                .data = {.subx_check = {
                    .subx_bit = 127
                }}
            },
            {
                .check_str_id = BAG_UPGRADE_CHECK_STR_ID,
                .type = TRACKER_CHECK_NUMBERED_SUBX,
                .data = {.numbered_subx_check = {
                    .subx_bit = 0, .number = 1
                }}
            },
            {
                .check_str_id = START_ITEM_INFO_STR_ID,
                .type = TRACKER_CHECK_INFO,
            },
            {
                .check_str_id = BOSS_DUO_INFO_STR_ID,
                .type = TRACKER_CHECK_MONSTER_INFO,
                .data = {.monster_info = {
                    .monster_0 = MONSTER_ZUBAT,
                    .monster_1 = MONSTER_KOFFING
                }}
            }
        )
    },
    [TRACKER_PAGE_DRENCHED_BLUFF] = {
        .dungeon = DUNGEON_DRENCHED_BLUFF,
        .checks = TRACKER_EMPTY_CHECK_BUNDLE(),
    },
    [TRACKER_PAGE_MT_BRISTLE] = {
        .dungeon = DUNGEON_MT_BRISTLE,
        .checks = TRACKER_CHECK_BUNDLE(
            {
                .check_str_id = BAG_UPGRADE_CHECK_STR_ID,
                .type = TRACKER_CHECK_NUMBERED_SUBX,
                .data = {.numbered_subx_check = {
                    .subx_bit = 1, .number = 2
                }}
            },
            {
                .check_str_id = BOSS_INFO_STR_ID,
                .type = TRACKER_CHECK_MONSTER_INFO,
                .data = {.monster_info = {
                    .monster_0 = MONSTER_DROWZEE,
                }}
            }
        )
    },
    [TRACKER_PAGE_WATERFALL_CAVE] = {
        .dungeon = DUNGEON_WATERFALL_CAVE,
        .checks = TRACKER_EMPTY_CHECK_BUNDLE(),
    },
    [TRACKER_PAGE_APPLE_WOODS] = {
        .dungeon = DUNGEON_APPLE_WOODS,
        .checks = TRACKER_CHECK_BUNDLE(
            {
                .check_str_id = BAG_UPGRADE_CHECK_STR_ID,
                .type = TRACKER_CHECK_NUMBERED_SUBX,
                .data = {.numbered_subx_check = {
                    .subx_bit = 2, .number = 3
                }}
            },
            {
                .check_str_id = BOSS_INFO_STR_ID,
                .type = TRACKER_CHECK_MONSTER_INFO,
                .data = {.monster_info = {
                    .monster_0 = MONSTER_DROWZEE,
                }}
            }
        )
    },
    [TRACKER_PAGE_CRAGGY_COAST] = {
        .dungeon = DUNGEON_CRAGGY_COAST,
        .checks = TRACKER_CHECK_BUNDLE(
            {
                .check_str_id = ESCORT_INFO_STR_ID,
                .type = TRACKER_CHECK_MONSTER_INFO,
                .data = {.monster_info = {
                    .monster_0 = MONSTER_BIDOOF,
                }}
            }
        )
    },
    [TRACKER_PAGE_SIDE_PATH] = {
        .dungeon = DUNGEON_SIDE_PATH,
        .checks = TRACKER_CHECK_BUNDLE(
            {
                .check_str_id = ESCORT_INFO_STR_ID,
                .type = TRACKER_CHECK_MONSTER_INFO,
                .data = {.monster_info = {
                    .monster_0 = MONSTER_BIDOOF,
                }}
            }
        )
    },
    [TRACKER_PAGE_MT_HORN] = {
        .dungeon = DUNGEON_MT_HORN,
        .checks = TRACKER_CHECK_BUNDLE(
            {
                .check_str_id = ESCORT_INFO_STR_ID,
                .type = TRACKER_CHECK_MONSTER_INFO,
                .data = {.monster_info = {
                    .monster_0 = MONSTER_BIDOOF,
                }}
            }
        )
    },
    [TRACKER_PAGE_ROCK_PATH] = {
        .dungeon = DUNGEON_ROCK_PATH,
        .checks = TRACKER_CHECK_BUNDLE(
            {
                .check_str_id = ESCORT_INFO_STR_ID,
                .type = TRACKER_CHECK_MONSTER_INFO,
                .data = {.monster_info = {
                    .monster_0 = MONSTER_BIDOOF,
                }}
            }
        )
    },
    [TRACKER_PAGE_FOGGY_FOREST] = {},
    [TRACKER_PAGE_FOREST_PATH] = {},
    [TRACKER_PAGE_STEAM_CAVE] = {
        .dungeon = DUNGEON_STEAM_CAVE,
        .checks = TRACKER_CHECK_BUNDLE(
            {
                .check_str_id = BAG_UPGRADE_CHECK_STR_ID,
                .type = TRACKER_CHECK_NUMBERED_SUBX,
                .data = {.numbered_subx_check = {
                    .subx_bit = 3, .number = 4
                }}
            },
            {
                .check_str_id = BOSS_INFO_STR_ID,
                .type = TRACKER_CHECK_MONSTER_INFO,
                .data = {.monster_info = {
                    .monster_0 = MONSTER_GROUDON,
                }}
            },
            {
                .check_str_id = BOSS_GIFT_CHECK_STR_ID,
                .type = TRACKER_CHECK_ID_VAL_SUBX,
                .data = {.id_val_subx_check = {
                    .subx_bit = 25,
                    .id_val = MONSTER_UXIE,
                }},
                .is_check_active_func = IsDarkraiGoal,
            }
        )
    },
    [TRACKER_PAGE_AMP_PLAINS] = {
        .dungeon = DUNGEON_AMP_PLAINS,
        .checks = TRACKER_CHECK_BUNDLE(
            {
                .check_str_id = BOSS_DUO_INFO_STR_ID,
                .type = TRACKER_CHECK_MONSTER_INFO,
                .data = {.monster_info = {
                    .monster_0 = MONSTER_MANECTRIC,
                    .monster_1 = MONSTER_ELECTRIKE,
                }}
            }
        )
    },
    [TRACKER_PAGE_NORTHERN_DESERT] = {},
    [TRACKER_PAGE_QUICKSAND_CAVE] = {
        .dungeon = DUNGEON_QUICKSAND_CAVE,
        .checks = TRACKER_CHECK_BUNDLE(
            {
                .check_str_id = BOSS_GIFT_CHECK_STR_ID,
                .type = TRACKER_CHECK_ID_VAL_SUBX,
                .data = {.id_val_subx_check = {
                    .subx_bit = 26,
                    .id_val = MONSTER_MESPRIT,
                }},
                .is_check_active_func = IsDarkraiGoal,
            }
        )
    },
    [TRACKER_PAGE_CRYSTAL_CAVE] = {},
    [TRACKER_PAGE_CRYSTAL_CROSSING] = {
        .dungeon = DUNGEON_CRYSTAL_CROSSING,
        .checks = TRACKER_CHECK_BUNDLE(
            {
                .check_str_id = BOSS_INFO_STR_ID,
                .type = TRACKER_CHECK_MONSTER_INFO,
                .data = {.monster_info = {
                    .monster_0 = MONSTER_GROVYLE,
                }}
            },
            // TODO: Add scripted loss info here?
            {
                .check_str_id = BOSS_GIFT_CHECK_STR_ID,
                .type = TRACKER_CHECK_ID_VAL_SUBX,
                .data = {.id_val_subx_check = {
                    .subx_bit = 27,
                    .id_val = MONSTER_AZELF,
                }},
                .is_check_active_func = IsDarkraiGoal,
            }
        )
    },
    [TRACKER_PAGE_CHASM_CAVE] = {},
    [TRACKER_PAGE_DARK_HILL] = {},
    [TRACKER_PAGE_SEALED_RUIN] = {
        .dungeon = DUNGEON_SEALED_RUIN,
        .checks = TRACKER_CHECK_BUNDLE(
            {
                .check_str_id = BOSS_INFO_STR_ID,
                .type = TRACKER_CHECK_MONSTER_INFO,
                .data = {.monster_info = {
                    .monster_0 = MONSTER_SPIRITOMB,
                }}
            }
        )
    },
    [TRACKER_PAGE_DUSK_FOREST] = {
        .dungeon = DUNGEON_DUSK_FOREST,
        .checks = TRACKER_CHECK_BUNDLE(
            {
                .check_str_id = ESCORT_INFO_STR_ID,
                .type = TRACKER_CHECK_MONSTER_INFO,
                .data = {.monster_info = {
                    .monster_0 = MONSTER_GROVYLE,
                }}
            }
        )
    },
    [TRACKER_PAGE_TREESHROUD_FOREST] = {
        .dungeon = DUNGEON_TREESHROUD_FOREST,
        .checks = TRACKER_CHECK_BUNDLE(
            {
                .check_str_id = ESCORT_INFO_STR_ID,
                .type = TRACKER_CHECK_MONSTER_INFO,
                .data = {.monster_info = {
                    .monster_0 = MONSTER_GROVYLE,
                }}
            }
        )
    },
    [TRACKER_PAGE_BRINE_CAVE] = {},
    [TRACKER_PAGE_SERENITY_RIVER] = {},
    [TRACKER_PAGE_LANDSLIDE_CAVE] = {},
    [TRACKER_PAGE_LUSH_PRAIRIE] = {},
    [TRACKER_PAGE_TINY_MEADOW] = {},
    [TRACKER_PAGE_LABYRINTH_CAVE] = {},
    [TRACKER_PAGE_ORAN_FOREST] = {},
    [TRACKER_PAGE_STAR_CAVE] = {},
    [TRACKER_PAGE_HIDDEN_LAND] = {},
    [TRACKER_PAGE_TEMPORAL_TOWER] = {},
    [TRACKER_PAGE_MYSTIFYING_FOREST] = {},
    [TRACKER_PAGE_BLIZZARD_ISLAND] = {},
    [TRACKER_PAGE_CREVICE_CAVE] = {},
    [TRACKER_PAGE_SURROUNDED_SEA] = {},
    [TRACKER_PAGE_MIRACLE_SEA] = {},
    [TRACKER_PAGE_AEGIS_CAVE] = {
        .name_str_id = AEGIS_CAVE_STR_ID,
    },
    [TRACKER_PAGE_MT_TRAVAIL] = {},
    [TRACKER_PAGE_THE_NIGHTMARE] = {},
    [TRACKER_PAGE_SPACIAL_RIFT] = {},
    [TRACKER_PAGE_CONCEALED_RUINS] = {},
    [TRACKER_PAGE_MARINE_RESORT] = {},
    [TRACKER_PAGE_BOTTOMLESS_SEA] = {},
    [TRACKER_PAGE_SHIMMER_DESERT] = {},
    [TRACKER_PAGE_MT_AVALANCHE] = {},
    [TRACKER_PAGE_GIANT_VOLCANO] = {},
    [TRACKER_PAGE_WORLD_ABYSS] = {},
    [TRACKER_PAGE_SKY_STAIRWAY] = {},
    [TRACKER_PAGE_MYSTERY_JUNGLE] = {},
    [TRACKER_PAGE_LAKE_AFAR] = {},
    [TRACKER_PAGE_HAPPY_OUTLOOK] = {},
    [TRACKER_PAGE_MY_MISTRAL] = {},
    [TRACKER_PAGE_SHIMMER_HILL] = {},
    [TRACKER_PAGE_LOST_WILDERNESS] = {},
    [TRACKER_PAGE_MIDNIGHT_FOREST] = {},
    [TRACKER_PAGE_1ST_STATION_PASS] = {},
    [TRACKER_PAGE_2ND_STATION_PASS] = {},
    [TRACKER_PAGE_3RD_STATION_PASS] = {},
    [TRACKER_PAGE_4TH_STATION_PASS] = {},
    [TRACKER_PAGE_5TH_STATION_PASS] = {},
    [TRACKER_PAGE_6TH_STATION_PASS] = {},
    [TRACKER_PAGE_7TH_STATION_PASS] = {},
    [TRACKER_PAGE_8TH_STATION_PASS] = {},
    [TRACKER_PAGE_9TH_STATION_PASS] = {},
    [TRACKER_PAGE_SKY_PEAK_SUMMIT_PASS] = {},
    [TRACKER_PAGE_SKY_PEAK_SUMMIT] = {},
    [TRACKER_PAGE_ZERO_ISLE_NORTH] = {},
    [TRACKER_PAGE_ZERO_ISLE_EAST] = {},
    [TRACKER_PAGE_ZERO_ISLE_WEST] = {},
    [TRACKER_PAGE_ZERO_ISLE_SOUTH] = {},
    [TRACKER_PAGE_ZERO_ISLE_CENTER] = {},
    [TRACKER_PAGE_DESTINY_TOWER] = {},
    [TRACKER_PAGE_OBLIVION_FOREST] = {},
    [TRACKER_PAGE_TREACHEROUS_WATERS] = {},
    [TRACKER_PAGE_SOUTHEASTERN_ISLANDS] = {},
    [TRACKER_PAGE_INFERNO_CAVE] = {},
    [TRACKER_PAGE_SE_BIDOOFS_WISH] = {},
    [TRACKER_PAGE_SE_IGGLYBUFF_THE_PRODIGY] = {},
    [TRACKER_PAGE_SE_TODAYS_OH_MY_GOSH] = {},
    [TRACKER_PAGE_SE_HERE_COMES_TEAM_CHARM] = {},
    [TRACKER_PAGE_SE_INTO_THE_FUTURE_OF_DARKNESS] = {},
    [TRACKER_PAGE_DARK_CRATER] = {},
};