/* 
*/

#include <pmdsky.h>
#include <cot.h>
#include "ap_utils.h"

// TRACKER_CHECK_BUNDLE bundles a bunch of check with a null terminator check at the end.
#define TRACKER_CHECK_BUNDLE(...) {__VA_ARGS__ , {.type = TRACKER_CHECK_TERMINATOR}}
#define TRACKER_EMPTY_CHECK_BUNDLE() {{.type = TRACKER_CHECK_TERMINATOR}}
#define DUNGEON_NONE DUNGEON_TEST_DUNGEON

// TODO: Adapt these strings for NA using ifdefs.
// Locations
#define TOWN_STR_ID 16814
#define SHOP_STR_ID 16304
#define BANK_STR_ID 0
#define RANK_STR_ID 0
#define GUILD_STR_ID 16438
#define CAFE_STR_ID 16550
#define HABITAT_STR_ID 16439
#define DOJO_STR_ID 16562
#define AEGIS_CAVE_STR_ID 16880

// Named Checks
#define BIDOOF_SE_LOC_CHECK_STR_ID 0
#define IGGLYBUFF_SE_LOC_CHECK_STR_ID 0
#define SUNFLORA_SE_LOC_CHECK_STR_ID 0
#define TEAM_CHARM_SE_LOC_CHECK_STR_ID 0
#define CONQUEST_ALL_DOJO_CHECK_STR_ID 0
#define GRANDPAS_TREASURE_CHECK_STR_ID 0
#define RECYCLE_SHOP_TREASURE_CHECK_STR_ID 0
#define LUDICOLO_DANCE_CHECK_STR_ID 0

// Checks
#define SHOP_CHECK_STR_ID 0
#define BANK_CHECK_STR_ID 0
#define RANK_CHECK_STR_ID 0
#define TEAM_NAME_CHECK_STR_ID 0
#define BAG_UPGRADE_CHECK_STR_ID 0
#define BOSS_GIFT_CHECK_STR_ID 0

#define START_ITEM_INFO_STR_ID 0
#define BOSS_INFO_STR_ID 0
#define BOSS_DUO_INFO_STR_ID 0
#define ESCORT_INFO_STR_ID 0

enum tracker_page {
    TRACKER_PAGE_SHOP,
    TRACKER_PAGE_BANK,
    TRACKER_PAGE_RANK,
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

// Force the enum to be as small as possible for better struct
// memory usage.
enum __attribute__((__packed__)) tracker_check_type {
    TRACKER_CHECK_TERMINATOR = 0,
    // Check(s) that have a unique drawing/completion check. ie: 'Recycle Shop Dungeons:\n X X X X X'
    TRACKER_CHECK_CUSTOM,
    // Info that may be useful for a check. ie: 'This dungeon is unlocked from the start.'
    TRACKER_CHECK_INFO,
    // Info that shows the boss. ie: 'Boss: Groudon'
    TRACKER_CHECK_BOSS_INFO,
    // Info that shows the bosses. ie: 'Boss: Zubat & Koffing'
    TRACKER_CHECK_BOSS_DUO_INFO,
    // Info that shows an escort that joins the team. ie: 'Escort: Bidoof'
    TRACKER_CHECK_ESCORT_INFO,
    // Check with a unique name: 'Grandpa's Treasure: X'
    TRACKER_CHECK_NAMED,
    // Check for amount of money in bank. ie: '100 G: X'
    TRACKER_CHECK_BANK,
    // Check for item buyable in a shop. ie: 'Shop Item #3: X'
    TRACKER_CHECK_SHOP,
    // Check for swappable item in a shop. ie: 'Swap Shop Item #2: X'
    TRACKER_CHECK_SWAP_SHOP,
    // Check for a bag upgrade. ie: 'Bag Upgrade #4: X'
    TRACKER_CHECK_BAG_UPGRADE,
    // Check for a rank. ie: 'Bronze Rank: X'
    TRACKER_CHECK_RANK,
    // Check for a gift. ie: 'Palkia's Gift: X'
    TRACKER_CHECK_GIFT,
    // Check for a 7 Treasure Mission. ie: 'Aqua-Monica Mission: X'
    TRACKER_CHECK_SEVEN_TREASURE_MISSION,
    // Check for a dungeon being completed: ie: 'Beach Cave: X'
    TRACKER_CHECK_DUNGEON_CONQUEST,
    // Check for a dojo being completed: ie: 'Dragon Maze: X'
    TRACKER_CHECK_DOJO_CONQUEST,
};

struct custom_tracker_element {
    void (*element_drawing_func)();
    bool (*element_checks_complete_func)();
};

struct helpful_information {
    uint16_t str_id;
    uint16_t id_0;   // Used for string preprocessing if need be.
    uint16_t id_1;   // Used for string preprocessing if need be.
    uint16_t number; // Used for string preprocessing if need be.
};
typedef helpful_information general_info;
typedef helpful_information boss_info;
typedef helpful_information boss_duo_info;
typedef helpful_information escort_info;

struct subx_check {
    uint16_t str_id;
    uint8_t subx_bit;
};
typedef subx_check named_check;

// number can be used to represent
// [value:0]
struct numbered_subx_check {
    uint32_t number;
    uint8_t subx_bit;
};
typedef numbered_subx_check bank_check;
typedef numbered_subx_check shop_check;
typedef numbered_subx_check swap_shop_check;
typedef numbered_subx_check bag_upgrade_check;

// id_val can be used to represent
// [kind:0], [dungeon:0], [item:0], [rank:0]
struct id_subx_check {
    uint32_t id;
    uint8_t subx_bit;
};
typedef id_subx_check rank_check;
typedef id_subx_check gift_check;
typedef id_subx_check seven_treasure_mission_check;

struct dungeon_conquest_check {
    struct dungeon_id_8 dungeon;
};
typedef dungeon_conquest_check dungeon_conquest_check;
typedef dungeon_conquest_check dojo_conquest_check;

union check_data {
    // Note: custom_tracker_elements should be used WISELY.
    // However, they generally should serve aesthetic or practical purpose.
    struct custom_tracker_element custom_tracker_element;
    general_info general_info;
    boss_info boss_info;
    boss_duo_info boss_duo_info;
    escort_info escort_info;
    named_check named_check;
    bank_check bank_check;
    shop_check shop_check;
    bag_upgrade_check bag_upgrade_check;
    rank_check rank_check;
    gift_check gift_check;
    seven_treasure_mission_check seven_treasure_mission_check;
    dungeon_conquest_check dungeon_conquest_check;
    dojo_conquest_check dojo_conquest_check;
};


struct tracker_check {
    enum tracker_check_type type;
    union check_data data;
    bool (*is_check_active_func)();
};

struct tracker_page_entry {
    uint16_t name_str_id;
    struct dungeon_id_16 dungeon;
    bool (*is_page_active_func)(); // NULL means page is always active.
    struct tracker_check checks[];
};

// Handle recycle shop dungeon checks.
void RecycleShopDungeonCheckDrawer();
bool IsAllRecycleShopDungeonChecksCompleted();

// Handle drink/drink events.
void DrinkAndDrinkEventCheckDrawer();
bool IsAllDrinkAndDrinkEventChecksCompleted();

struct tracker_page_entry tracker_book[] = {
    [TRACKER_PAGE_SHOP] = {
        .name_str_id = SHOP_STR_ID,
        .checks = TRACKER_CHECK_BUNDLE(
            {
                .type = TRACKER_CHECK_SHOP,
                .data = {.shop_check = {
                     .number = 1,
                     .subx_bit = 10
                }}
            },
            {
                .type = TRACKER_CHECK_SHOP,
                .data = {.shop_check = {
                     .number = 2,
                     .subx_bit = 11
                }}
            },
            {
                .type = TRACKER_CHECK_SHOP,
                .data = {.shop_check = {
                     .number = 3,
                     .subx_bit = 12
                }}
            },
            {
                .type = TRACKER_CHECK_SHOP,
                .data = {.shop_check = {
                     .number = 4,
                     .subx_bit = 13
                }}
            },
            {
                .type = TRACKER_CHECK_SHOP,
                .data = {.shop_check = {
                     .number = 5,
                     .subx_bit = 14
                }}
            },
            {
                .type = TRACKER_CHECK_SHOP,
                .data = {.shop_check = {
                     .number = 6,
                     .subx_bit = 15
                }}
            },
            {
                .type = TRACKER_CHECK_SHOP,
                .data = {.shop_check = {
                     .number = 7,
                     .subx_bit = 16
                }}
            },
            {
                .type = TRACKER_CHECK_SHOP,
                .data = {.shop_check = {
                     .number = 8,
                     .subx_bit = 17
                }}
            },
            {
                .type = TRACKER_CHECK_SHOP,
                .data = {.shop_check = {
                     .number = 9,
                     .subx_bit = 18
                }}
            },
            {
                .type = TRACKER_CHECK_SHOP,
                .data = {.shop_check = {
                     .number = 10,
                     .subx_bit = 19
                }}
            }
            // TODO: Display items in shop.
        )
    },
    [TRACKER_PAGE_BANK] = {
        .name_str_id = BANK_STR_ID,
        .checks = TRACKER_CHECK_BUNDLE(
            {
                .type = TRACKER_CHECK_BANK,
                .data = {.shop_check = {
                     .number = 100,
                     .subx_bit = 81
                }}
            },
            {
                .type = TRACKER_CHECK_BANK,
                .data = {.shop_check = {
                     .number = 5000,
                     .subx_bit = 82
                }}
            },
            {
                .type = TRACKER_CHECK_BANK,
                .data = {.shop_check = {
                     .number = 10000,
                     .subx_bit = 83
                }}
            },
            {
                .type = TRACKER_CHECK_BANK,
                .data = {.shop_check = {
                     .number = 20000,
                     .subx_bit = 84
                }}
            },
            {
                .type = TRACKER_CHECK_BANK,
                .data = {.shop_check = {
                     .number = 50000,
                     .subx_bit = 85
                }}
            },
            {
                .type = TRACKER_CHECK_BANK,
                .data = {.shop_check = {
                     .number = 100000,
                     .subx_bit = 86
                }}
            },
            {
                .type = TRACKER_CHECK_BANK,
                .data = {.shop_check = {
                     .number = 9999999,
                     .subx_bit = 87
                }}
            }
            // TODO: Display current bank money.
        )
    },
    [TRACKER_PAGE_RANK] = {
        .name_str_id = RANK_CHECK_STR_ID,
        .checks = TRACKER_CHECK_BUNDLE(
            {
                .type = TRACKER_CHECK_RANK,
                .data = {.rank_check = {
                    .id = RANK_BRONZE,
                    .subx_bit = 73
                }}
            },
            {
                .type = TRACKER_CHECK_RANK,
                .data = {.rank_check = {
                    .id = RANK_SILVER,
                    .subx_bit = 74
                }}
            },
            {
                .type = TRACKER_CHECK_RANK,
                .data = {.rank_check = {
                    .id = RANK_GOLD,
                    .subx_bit = 75
                }}  
            },
            {
                .type = TRACKER_CHECK_RANK,
                .data = {.rank_check = {
                    .id = RANK_DIAMOND,
                    .subx_bit = 76
                }}
            },
            {
                .type = TRACKER_CHECK_RANK,
                .data = {.rank_check = {
                    .id = RANK_SUPER,
                    .subx_bit = 77
                }}
            },
            {
                .type = TRACKER_CHECK_RANK,
                .data = {.rank_check = {
                    .id = RANK_ULTRA,
                    .subx_bit = 78
                }}
            },
            {
                .type = TRACKER_CHECK_RANK,
                .data = {.rank_check = {
                    .id = RANK_HYPER,
                    .subx_bit = 79
                }}
            },
            {
                .type = TRACKER_CHECK_RANK,
                .data = {.rank_check = {
                    .id = RANK_MASTER,
                    .subx_bit = 80
                }}
            },
            {
                .type = TRACKER_CHECK_RANK,
                .data = {.rank_check = {
                    .id = RANK_MASTER_1_STAR,
                    .subx_bit = 55
                }},
                .is_check_active_func = AreLongLocationsOn
            },
            {
                .type = TRACKER_CHECK_RANK,
                .data = {.rank_check = {
                    .id = RANK_MASTER_2_STARS,
                    .subx_bit = 56
                }},
                .is_check_active_func = AreLongLocationsOn
            },
            {
                .type = TRACKER_CHECK_RANK,
                .data = {.rank_check = {
                    .id = RANK_MASTER_3_STARS,
                    .subx_bit = 57
                }},
                .is_check_active_func = AreLongLocationsOn
            },
            {
                .type = TRACKER_CHECK_RANK,
                .data = {.rank_check = {
                    .id = RANK_GUILDMASTER,
                    .subx_bit = 58
                }},
                .is_check_active_func = AreLongLocationsOn
            }
            // TODO: Add an element to show current rank points and breakpoints.
            // TODO: Find a way to fit all 12 in?
        )
    },
    [TRACKER_PAGE_GUILD] = {
        .name_str_id = GUILD_STR_ID,
        .checks = TRACKER_CHECK_BUNDLE(
            {
                .type = TRACKER_CHECK_NAMED,
                .data = {.named_check = {
                    .str_id = BIDOOF_SE_LOC_CHECK_STR_ID,
                    .subx_bit = 5
                }}
            },
            {
                .type = TRACKER_CHECK_NAMED,
                .data = {.named_check = {
                    .str_id = IGGLYBUFF_SE_LOC_CHECK_STR_ID,
                    .subx_bit = 6
                }}
            },
            {
                .type = TRACKER_CHECK_NAMED,
                .data = {.named_check = {
                    .str_id = SUNFLORA_SE_LOC_CHECK_STR_ID,
                    .subx_bit = 7
                }}
            },
            {
                .type = TRACKER_CHECK_NAMED,
                .data = {.named_check = {
                    .str_id = TEAM_CHARM_SE_LOC_CHECK_STR_ID,
                    .subx_bit = 8
                }}
            }
            // TODO: Add information about when these checks are not in the guild.
            // TODO: Add new Croagunk Swap Shop checks?
            // TODO: Add a screen on the side that lists Croagunk's Inventory.
        )
    },
    [TRACKER_PAGE_DOJO] = {
        .name_str_id = DOJO_STR_ID,
        .checks = TRACKER_CHECK_BUNDLE(
            {
                .type = TRACKER_CHECK_DOJO_CONQUEST,
                .data = {.dojo_conquest_check = {
                    .dungeon = DUNGEON_NORMAL_FLY_MAZE
                }}
            },
            {
                .type = TRACKER_CHECK_DOJO_CONQUEST,
                .data = {.dojo_conquest_check = {
                    .dungeon = DUNGEON_DARK_FIRE_MAZE
                }}
            },
            {
                .type = TRACKER_CHECK_DOJO_CONQUEST,
                .data = {.dojo_conquest_check = {
                    .dungeon = DUNGEON_ROCK_WATER_MAZE
                }}
            },
            {
                .type = TRACKER_CHECK_DOJO_CONQUEST,
                .data = {.dojo_conquest_check = {
                    .dungeon = DUNGEON_GRASS_MAZE
                }}
            },
            {
                .type = TRACKER_CHECK_DOJO_CONQUEST,
                .data = {.dojo_conquest_check = {
                    .dungeon = DUNGEON_ELEC_STEEL_MAZE
                }}
            },
            {
                .type = TRACKER_CHECK_DOJO_CONQUEST,
                .data = {.dojo_conquest_check = {
                    .dungeon = DUNGEON_ICE_GROUND_MAZE
                }}
            },
            {
                .type = TRACKER_CHECK_DOJO_CONQUEST,
                .data = {.dojo_conquest_check = {
                    .dungeon = DUNGEON_FIGHT_PSYCH_MAZE
                }}
            },
            {
                .type = TRACKER_CHECK_DOJO_CONQUEST,
                .data = {.dojo_conquest_check = {
                    .dungeon = DUNGEON_POISON_BUG_MAZE
                }}
            },
            { // Placed here for formatting.
                .type = TRACKER_CHECK_NAMED,
                .data = {.named_check = {
                    .str_id = CONQUEST_ALL_DOJO_CHECK_STR_ID, 
                    .subx_bit = 67
                }}
            },
            {
                .type = TRACKER_CHECK_DOJO_CONQUEST,
                .data = {.dojo_conquest_check = {
                    .dungeon = DUNGEON_DRAGON_MAZE
                }}
            },
            {
                .type = TRACKER_CHECK_DOJO_CONQUEST,
                .data = {.dojo_conquest_check = {
                    .dungeon = DUNGEON_GHOST_MAZE
                }}
            },
            { // Placed here for formatting.
                .type = TRACKER_CHECK_NAMED,
                .data = {.named_check = {
                    .str_id = GRANDPAS_TREASURE_CHECK_STR_ID,
                    .subx_bit = 68
                }}
            },
            {
                .type = TRACKER_CHECK_DOJO_CONQUEST,
                .data = {.dojo_conquest_check = {
                    .dungeon = DUNGEON_FINAL_MAZE
                }}
            }
        )
    },
    [TRACKER_PAGE_CAFE] = {
        .name_str_id = CAFE_STR_ID,
        .checks = TRACKER_CHECK_BUNDLE(
            {
                .type = TRACKER_CHECK_SEVEN_TREASURE_MISSION,
                .data = {.seven_treasure_mission_check = {
                    .id = ITEM_AQUA_MONICA,
                    .subx_bit = 48
                }}
            },
            {
                .type = TRACKER_CHECK_SEVEN_TREASURE_MISSION,
                .data = {.seven_treasure_mission_check = {
                    .id = ITEM_TERRA_CYMBAL,
                    .subx_bit = 49
                }}
            },
            {
                .type = TRACKER_CHECK_SEVEN_TREASURE_MISSION,
                .data = {.seven_treasure_mission_check = {
                    .id = ITEM_ICY_FLUTE,
                    .subx_bit = 50
                }}
            },
            {
                .type = TRACKER_CHECK_SEVEN_TREASURE_MISSION,
                .data = {.seven_treasure_mission_check = {
                    .id = ITEM_FIERY_DRUM,
                    .subx_bit = 51
                }}
            },
            {
                .type = TRACKER_CHECK_SEVEN_TREASURE_MISSION,
                .data = {.seven_treasure_mission_check = {
                    .id = ITEM_ROCK_HORN,
                    .subx_bit = 52
                }}
            },
            {
                .type = TRACKER_CHECK_SEVEN_TREASURE_MISSION,
                .data = {.seven_treasure_mission_check = {
                    .id = ITEM_SKY_MELODICA,
                    .subx_bit = 53
                }}
            },
            {
                .type = TRACKER_CHECK_SEVEN_TREASURE_MISSION,
                .data = {.seven_treasure_mission_check = {
                    .id = ITEM_GRASS_CORNET,
                    .subx_bit = 54
                }}
            },
            {
                .type = TRACKER_CHECK_NAMED,
                .data = {.named_check = {
                    .str_id = RECYCLE_SHOP_TREASURE_CHECK_STR_ID,
                    .subx_bit = 59
                }}
            },
            {
                .type = TRACKER_CHECK_NAMED,
                .data = {.named_check = {
                    .str_id = LUDICOLO_DANCE_CHECK_STR_ID,
                    .subx_bit = 88
                }}
            },
            {
                .type = TRACKER_CHECK_CUSTOM,
                .data = {.custom_tracker_element = {
                    .element_drawing_func = DrinkAndDrinkEventCheckDrawer,
                    .element_checks_complete_func = IsAllDrinkAndDrinkEventChecksCompleted
                }}
            },
            {
                .type = TRACKER_CHECK_CUSTOM,
                .data = {.custom_tracker_element = {
                    .element_drawing_func = RecycleShopDungeonCheckDrawer,
                    .element_checks_complete_func = IsAllRecycleShopDungeonChecksCompleted
                }}
            }
        )
    },
    [TRACKER_PAGE_HABITAT] = {
        .name_str_id = HABITAT_STR_ID,
        .is_page_active_func = IsDarkraiGoal,
        .checks = TRACKER_CHECK_BUNDLE(
            {
                .type = TRACKER_CHECK_GIFT,
                .data = {.gift_check = {
                    .id = MONSTER_MANAPHY,
                    .subx_bit = 23
                }},
                .is_check_active_func = IsDarkraiGoal
            },
            {
                .type = TRACKER_CHECK_GIFT,
                .data = {.gift_check = {
                    .id = MONSTER_CRESSELIA,
                    .subx_bit = 45
                }},
                .is_check_active_func = IsDarkraiGoal
            }
        )
        // TODO: Add Manaphy's Discovery & Blue Goomi (2)
    },
    [TRACKER_PAGE_BEACH_CAVE] = {
        .dungeon = DUNGEON_BEACH_CAVE,
        .checks = TRACKER_CHECK_BUNDLE(
            {
                .type = TRACKER_CHECK_NAMED,
                .data = {.named_check = {
                    .str_id = TEAM_NAME_CHECK_STR_ID,
                    .subx_bit = 127
                }}
            },
            {
                .type = TRACKER_CHECK_BAG_UPGRADE,
                .data = {.bag_upgrade_check = {
                     .number = 1, .subx_bit = 0,
                }}
            }
        )
        // TODO: Add starting information.
    },
    [TRACKER_PAGE_DRENCHED_BLUFF] = {
        .dungeon = DUNGEON_DRENCHED_BLUFF,
        .checks = TRACKER_EMPTY_CHECK_BUNDLE(),
    },
    [TRACKER_PAGE_MT_BRISTLE] = {
        .dungeon = DUNGEON_MT_BRISTLE,
        .checks = TRACKER_CHECK_BUNDLE(
            {
                .type = TRACKER_CHECK_BAG_UPGRADE,
                .data = {.bag_upgrade_check = {
                    .number = 2, .subx_bit = 1,
                }}
            },
            {
                .type = TRACKER_CHECK_BOSS_INFO,
                .data = {.boss_info = {
                    .id_0 = MONSTER_DROWZEE,
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
                .type = TRACKER_CHECK_BAG_UPGRADE,
                .data = {.bag_upgrade_check = {
                    .number = 3, .subx_bit = 2
                }}
            }
        )
    },
    [TRACKER_PAGE_CRAGGY_COAST] = {
        .dungeon = DUNGEON_CRAGGY_COAST,
        .checks = TRACKER_CHECK_BUNDLE(
            {
                .type = TRACKER_CHECK_ESCORT_INFO,
                .data = {.escort_info = {
                    .id_0 = MONSTER_BIDOOF,
                }}
            }
        )
    },
    [TRACKER_PAGE_SIDE_PATH] = {
        .dungeon = DUNGEON_SIDE_PATH,
        .checks = TRACKER_CHECK_BUNDLE(
            {
                .type = TRACKER_CHECK_ESCORT_INFO,
                .data = {.escort_info = {
                    .id_0 = MONSTER_BIDOOF,
                }}
            }
        )
    },
    [TRACKER_PAGE_MT_HORN] = {
        .dungeon = DUNGEON_MT_HORN,
        .checks = TRACKER_CHECK_BUNDLE(
            {
                .type = TRACKER_CHECK_ESCORT_INFO,
                .data = {.escort_info = {
                    .id_0 = MONSTER_BIDOOF,
                }}
            }
        )
    },
    [TRACKER_PAGE_ROCK_PATH] = {
        .dungeon = DUNGEON_ROCK_PATH,
        .checks = TRACKER_CHECK_BUNDLE(
            {
                .type = TRACKER_CHECK_ESCORT_INFO,
                .data = {.escort_info = {
                    .id_0 = MONSTER_BIDOOF,
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
                .type = TRACKER_CHECK_BAG_UPGRADE,
                .data = {.bag_upgrade_check = {
                    .number = 4,
                    .subx_bit = 3
                }}
            },
            {
                .type = TRACKER_CHECK_BOSS_INFO,
                .data = {.boss_info = {
                    .id_0 = MONSTER_GROUDON,
                }}
            },
            {
                .type = TRACKER_CHECK_BOSS_INFO,
                .data = {.boss_info = {
                    .id_0 = MONSTER_UXIE,
                }},
                .is_check_active_func = IsDarkraiGoal,
            },
            {
                .type = TRACKER_CHECK_GIFT,
                .data = {.gift_check = {
                    .id = MONSTER_UXIE,
                    .subx_bit = 25
                }},
                .is_check_active_func = IsDarkraiGoal,
            }
        )
    },
    [TRACKER_PAGE_AMP_PLAINS] = {
        .dungeon = DUNGEON_AMP_PLAINS,
        .checks = TRACKER_CHECK_BUNDLE(
            {
                .type = TRACKER_CHECK_BOSS_DUO_INFO,
                .data = {.boss_duo_info = {
                    .id_0 = MONSTER_MANECTRIC,
                    .id_1 = MONSTER_ELECTRIKE,
                }}
            }
        )
    },
    [TRACKER_PAGE_NORTHERN_DESERT] = {},
    [TRACKER_PAGE_QUICKSAND_CAVE] = {
        .dungeon = DUNGEON_QUICKSAND_CAVE,
        .checks = TRACKER_CHECK_BUNDLE(
            {
                .type = TRACKER_CHECK_BOSS_INFO,
                .data = {.boss_info = {
                    .id_0 = MONSTER_MESPRIT,
                }},
            },
            {
                .type = TRACKER_CHECK_GIFT,
                .data = {.gift_check = {
                    .id = MONSTER_MESPRIT,
                    .subx_bit = 26
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
                .type = TRACKER_CHECK_BOSS_INFO,
                .data = {.boss_info = {
                    .id_0 = MONSTER_GROVYLE,
                }},
            },
            // TODO: Add scripted loss info here?
            {
                .type = TRACKER_CHECK_BOSS_INFO,
                .data = {.boss_info = {
                    .id_0 = MONSTER_AZELF,
                }},
                .is_check_active_func = IsDarkraiGoal,
            },
            {
                .type = TRACKER_CHECK_GIFT,
                .data = {.gift_check = {
                    .id = MONSTER_AZELF,
                    .subx_bit = 26
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
                .type = TRACKER_CHECK_BOSS_INFO,
                .data = {.boss_info = {
                    .id_0 = MONSTER_SPIRITOMB,
                }}
            }
        )
    },
    [TRACKER_PAGE_DUSK_FOREST] = {
        .dungeon = DUNGEON_DUSK_FOREST,
        .checks = TRACKER_CHECK_BUNDLE(
            {
                .type = TRACKER_CHECK_ESCORT_INFO,
                .data = {.escort_info = {
                    .id_0 = MONSTER_GROVYLE,
                }}
            }
        )
    },
    [TRACKER_PAGE_TREESHROUD_FOREST] = {
        .dungeon = DUNGEON_TREESHROUD_FOREST,
        .checks = TRACKER_CHECK_BUNDLE(
            {
                .type = TRACKER_CHECK_ESCORT_INFO,
                .data = {.escort_info = {
                    .id_0 = MONSTER_GROVYLE,
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