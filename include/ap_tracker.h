// ap_tracker.h
// This header manages all the information for the tracker window
// in game. If you are adding a check, that information should be
// put into the tracker_book array in the corresponding location
// that it can be obtainted. New pages should be added as an enum
// in enum tracker_page and then a new entry should be added to
// the tracker_book array defining any checks associated with the
// location.

#include <pmdsky.h>
#include <cot.h>
#include "ap_utils.h"

// TRACKER_CHECK_BUNDLE bundles a bunch of check with a null terminator check at the end.
#define TRACKER_CHECK_BUNDLE(...) {__VA_ARGS__ , {.type = TRACKER_CHECK_TERMINATOR}}
#define DUNGEON_NONE DUNGEON_TEST_DUNGEON
#define TRACKER_BOOK_PAGE_COUNT sizeof(tracker_book)/sizeof(tracker_book[0])

#define FALLBACK_ERROR_STR_ID 15438

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
#define BIDOOF_SE_STR_ID 370
#define IGGLYBUFF_SE_STR_ID 371
#define OH_MY_GOSH_SE_STR_ID 372
#define TEAM_CHARM_SE_STR_ID 373
#define FUTURE_SE_STR_ID 374

// Named Checks
#define BIDOOF_SE_LOC_CHECK_STR_ID 0
#define IGGLYBUFF_SE_LOC_CHECK_STR_ID 0
#define SUNFLORA_SE_LOC_CHECK_STR_ID 0
#define TEAM_CHARM_SE_LOC_CHECK_STR_ID 0
#define CONQUEST_ALL_DOJO_CHECK_STR_ID 0
#define GRANDPAS_TREASURE_CHECK_STR_ID 0
#define RECYCLE_SHOP_TREASURE_CHECK_STR_ID 0
#define LUDICOLO_DANCE_CHECK_STR_ID 0
#define SE_MAROWACK_DOJO_REVIVAL_CHECK_STR_ID 0
#define SNEASELS_GRATITUDE_CHECK_STR_ID 0
#define BLUE_GOOMI_1_CHECK_STR_ID 0
#define BLUE_GOOMI_2_CHECK_STR_ID 0

// Checks
#define SHOP_CHECK_STR_ID 0
#define BANK_CHECK_STR_ID 0
#define RANK_CHECK_STR_ID 0
#define TEAM_NAME_CHECK_STR_ID 0
#define BAG_UPGRADE_CHECK_STR_ID 0
#define BOSS_GIFT_CHECK_STR_ID 0

// Info
#define GRANDMASTER_OF_ALL_THINGS_BAD_BOSS_INFO_STR_ID 0
#define START_ITEM_INFO_STR_ID 0
#define BOSS_INFO_STR_ID 0
#define BOSS_DUO_INFO_STR_ID 0
#define ESCORT_INFO_STR_ID 0

/* tracker_page
    This lists all the pages in the tracker. If a page is added to
    this list please also add an entry into the tracker_book array
    below. Additionally, the order of the pages can be modified by
    changing their order in this enum without modifying the order
    in tracker_book. */
enum __attribute__((__packed__)) tracker_page {
    TRACKER_PAGE_SHOP,
    TRACKER_PAGE_BANK,
    TRACKER_PAGE_RANK,
    TRACKER_PAGE_GUILD,
    TRACKER_PAGE_DOJO,
    TRACKER_PAGE_CAFE,
    TRACKER_PAGE_HABITAT, // Aka Sharpedo Bluff
    TRACKER_PAGE_SE_BIDOOFS_WISH,
    TRACKER_PAGE_SE_IGGLYBUFF_THE_PRODIGY,
    TRACKER_PAGE_SE_TODAYS_OH_MY_GOSH,
    TRACKER_PAGE_SE_HERE_COMES_TEAM_CHARM,
    TRACKER_PAGE_SE_INTO_THE_FUTURE_OF_DARKNESS,
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
    // DUNGEON_ICE_AEGIS_CAVE is first in this dungeon group.
    TRACKER_PAGE_AEGIS_CAVE,
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
    TRACKER_PAGE_DARK_CRATER,
    TRACKER_PAGE_ZERO_ISLE_NORTH,      // Rule
    TRACKER_PAGE_ZERO_ISLE_EAST,       // Rule
    TRACKER_PAGE_ZERO_ISLE_WEST,       // Rule
    TRACKER_PAGE_ZERO_ISLE_SOUTH,      // Rule
    TRACKER_PAGE_ZERO_ISLE_CENTER,     // Rule
    TRACKER_PAGE_DESTINY_TOWER,        // Rule
    TRACKER_PAGE_OBLIVION_FOREST,      // Rule
    TRACKER_PAGE_TREACHEROUS_WATERS,   // Rule
    TRACKER_PAGE_SOUTHEASTERN_ISLANDS, // Rule
    TRACKER_PAGE_INFERNO_CAVE,         // Rule
    TRACKER_PAGE_ERROR = 0xFF
};

/* tracker_check_type
    A comprehensive enum of all kinds of checks (and useful
    information related to checks.) 
    If a check has a unique named and is not part of a set
    ie: (Shop Item #1, Shop Item #2, etc) it should use
    TRACKER_CHECK_NAMED (enum) and named_check (type).
    Otherwise, use one of the corresponding enums and the
    matching type. ie: (TRACKER_CHECK_BANK & bank_check).
    If the checks are part of a set that doesn't currently
    exist, feel free to add a new kind of tracker_check_type
    corresponding check_data for it into the union. */
enum __attribute__((__packed__)) tracker_check_type {
    TRACKER_CHECK_TERMINATOR = 0,
    // Info that may be useful for a check.
    // ie: 'This dungeon is unlocked from the start.'
    TRACKER_CHECK_INFO,
    // Info that shows the boss.
    // ie: 'Boss: Groudon'
    TRACKER_CHECK_BOSS_INFO,
    // Info that shows the bosses.
    // ie: 'Boss: Zubat & Koffing'
    TRACKER_CHECK_BOSS_DUO_INFO,
    // Info that shows an escort that joins the team.
    // ie: 'Escort: Bidoof'
    TRACKER_CHECK_ESCORT_INFO,
    // Info that shows the escorts that join the team.
    // ie: 'Escort Grovyle & Celebi'
    TRACKER_CHECK_ESCORT_DUO_INFO,
    // Check(s)/info that have a unique completion/drawing check.
    // ie: 'Recycle Shop Dungeons:\n X X X X X'
    TRACKER_CHECK_CUSTOM,
    // Check with a unique name that can't be grouped.
    // ie: 'Grandpa's Treasure: X'
    TRACKER_CHECK_NAMED,
    // Check for amount of money in bank.
    // ie: '100 G: X'
    TRACKER_CHECK_BANK,
    // Check for item buyable in a shop.
    // ie: 'Shop Item #3: X'
    TRACKER_CHECK_SHOP,
    // Check for swappable item in a shop.
    // ie: 'Swap Shop Item #2: X'
    TRACKER_CHECK_SWAP_SHOP,
    // Check for a bag upgrade.
    // ie: 'Bag Upgrade #4: X'
    TRACKER_CHECK_BAG_UPGRADE,
    // Check for a rank.
    // ie: 'Bronze Rank: X'
    TRACKER_CHECK_RANK,
    // Check for a gift.
    // ie: 'Palkia's Gift: X'
    TRACKER_CHECK_GIFT,
    // Check for an item.
    // ie: 'Sky Melodica: X'
    TRACKER_CHECK_ITEM,
    // Check for a 7 Treasure Mission.
    // ie: 'Aqua-Monica Mission: X'
    TRACKER_CHECK_SEVEN_TREASURE_MISSION,
    // Check for a dungeon being completed.
    // ie: 'Beach Cave: X'
    TRACKER_CHECK_DUNGEON_CONQUEST,
    // Check for a special episode dungeon being completed.
    // ie: 'SE Deep Star Cave: X'
    TRACKER_CHECK_SPECIAL_EPISODE_DUNGEON_CONQUEST,
    // Check for a dojo being completed.
    // ie: 'Dragon Maze: X'
    TRACKER_CHECK_DOJO_CONQUEST,
};

/* tracker_check_built_layout
    This struct is used to draw and layout pages. It is
    not important for actually adding checks and pages.
    Used by the tracker to manage where to place checks
    in the window when it's time to actually draw them.
    Currently, the system goes top to bottom then left
    to right. (With a few exceptions for appearence).
    Exceptions include: TRACKER_CHECK_BOSS_INFO and
    TRACKER_CHECK_ESCORT_INFO which try to draw to the
    write side of the previous check if possible. */
struct tracker_check_built_layout {
    uint16_t x;
    uint16_t y;
    uint32_t spot_usage[11][2];
};

/* custom_tracker_element
    element_drawing_func: Used to draw the custom element
        into the tracker window. If there is NOT enough
        space in the layout struct, the element should
        return without drawing anything.
    element_checks_complete_func: Used to check if all
        the checks associated with the custom tracker 
        element are completed. */
struct custom_tracker_element {
    void (*element_drawing_func)(int, struct tracker_check_built_layout*);
    bool (*element_checks_complete_func)();
};

struct helpful_information {
    uint16_t str_id;
    uint16_t id_0;   // Used for string preprocessing if need be.
    uint16_t id_1;   // Used for string preprocessing if need be.
    uint16_t number; // Used for string preprocessing if need be.
};
typedef struct helpful_information general_info;
typedef struct helpful_information boss_info;
typedef struct helpful_information boss_duo_info;
typedef struct helpful_information escort_info;
typedef struct helpful_information escort_duo_info;

struct subx_check {
    uint16_t str_id;
    uint8_t subx_bit;
};
typedef struct subx_check named_check;

// number can be used to replace...
// [value:0]
// in the string preprocessing process
struct numbered_subx_check {
    uint32_t number; // Used for string preprocessing.
    uint8_t subx_bit;
};
typedef struct numbered_subx_check bank_check;
typedef struct numbered_subx_check shop_check;
typedef struct numbered_subx_check swap_shop_check;
typedef struct numbered_subx_check bag_upgrade_check;

// id can be used to replace...
// [kind:0], [dungeon:0], [item:0], [rank:0]
// in the string preprocessing process
struct id_subx_check {
    uint32_t id; // Used for string preprocessing.
    uint8_t subx_bit;
};
typedef struct id_subx_check rank_check;
typedef struct id_subx_check gift_check;
typedef struct id_subx_check item_check;
typedef struct id_subx_check seven_treasure_mission_check;

// Note: In game, there is no difference between a
// dungeon_conquest_check, dojo_conquest_check, and
// special_episode_dungeon_conquest_check. They are
// only made different in the tracker_book so it is
// readily apparent which its meant to be.
struct dungeon_conquest_check {
    struct dungeon_id_8 dungeon;
};
typedef struct dungeon_conquest_check dungeon_conquest_check;
typedef struct dungeon_conquest_check special_episode_dungeon_conquest_check;
typedef struct dungeon_conquest_check dojo_conquest_check;

union check_data {
    // Note: custom_tracker_elements should be used WISELY.
    struct custom_tracker_element custom_tracker_element;
    general_info general_info;
    boss_info boss_info;
    boss_duo_info boss_duo_info;
    escort_info escort_info;
    escort_duo_info escort_duo_info;
    named_check named_check;
    bank_check bank_check;
    shop_check shop_check;
    bag_upgrade_check bag_upgrade_check;
    rank_check rank_check;
    gift_check gift_check;
    item_check item_check;
    seven_treasure_mission_check seven_treasure_mission_check;
    dungeon_conquest_check dungeon_conquest_check;
    dojo_conquest_check dojo_conquest_check;
    special_episode_dungeon_conquest_check special_episode_dungeon_conquest_check;
};

/* tracker_page_entry
    type: The type of the check.
    data: A union with the corresponding data for the type
        listed above.
    is_check_active_func: When not NULL, a function that
        returns whether or not this check should be shown.*/
struct tracker_check {
    enum tracker_page page;
    enum tracker_check_type type;
    union check_data data;
    bool (*is_check_active_func)(); // NULL means check is always obtainable.
};

/* tracker_page_entry
    name_str_id: The string id to use from the text file
        as the name. If 0/blank, use the dungeon name below.
    dungeon: The dungeon associated with this page. This is
        used to draw the mission checks and if name_str_id
        is blank, the corresponding dungeon is used as the
        name.
    is_page_active_func: When not NULL, a function that returns
        whether or not this page should be shown. For example,
        DUNGEON_DARK_CRATER uses IsDarkraiGoal for this value
        since it's only active if Darkrai is the goal.
    checks: A list of tracker_checks terminated by one with
        TRACKER_CHECK_TERMINATOR for the type. The macro
        TRACKER_CHECK_BUNDLE can be used to automatically
        append this terminator to the end. Additionally, if
        there are no checks besides the missions for the
        corresponding dungeon, you can use the 
        TRACKER_EMPTY_CHECK_BUNDLE macro. */
struct tracker_page_entry {
    uint16_t name_str_id;
    struct dungeon_id_16 dungeon;
    bool (*is_page_active_func)(); // NULL means page is always shown.
    struct tracker_check *checks;
};

// Handle recycle shop dungeon checks.
void RecycleShopDungeonCheckDrawer(int idx, struct tracker_check_built_layout *layout);
bool IsAllRecycleShopDungeonChecksCompleted();

// Handle drink/drink events.
void DrinkAndDrinkEventCheckDrawer(int idx, struct tracker_check_built_layout *layout);
bool IsAllDrinkAndDrinkEventChecksCompleted();

// TODO: Display current shop listings.

// TODO: Display current bank info.

// TODO: Display swap-shop info.

// TODO: Display mission board info.

struct tracker_check empty_check = {.type = TRACKER_CHECK_TERMINATOR};

struct tracker_check shop_checks[] = TRACKER_CHECK_BUNDLE(
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
);

struct tracker_check bank_checks[] = TRACKER_CHECK_BUNDLE(
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
);

struct tracker_check rank_checks[] = TRACKER_CHECK_BUNDLE(
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
);

struct tracker_check guild_checks[] = TRACKER_CHECK_BUNDLE(
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
    },
    {
        .type = TRACKER_CHECK_NAMED,
        .data = {.named_check = {
            .str_id = BLUE_GOOMI_1_CHECK_STR_ID,
            .subx_bit = 20
        }},
        .is_check_active_func = IsDarkraiGoal
    }
);

struct tracker_check dojo_checks[] = TRACKER_CHECK_BUNDLE(
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
    {
        .type = TRACKER_CHECK_DOJO_CONQUEST,
        .data = {.dojo_conquest_check = {
            .dungeon = DUNGEON_FINAL_MAZE
        }}
    },
    {
        .type = TRACKER_CHECK_NAMED,
        .data = {.named_check = {
            .str_id = CONQUEST_ALL_DOJO_CHECK_STR_ID, 
            .subx_bit = 67
        }}
    },
    {
        .type = TRACKER_CHECK_NAMED,
        .data = {.named_check = {
            .str_id = GRANDPAS_TREASURE_CHECK_STR_ID,
            .subx_bit = 68
        }}
    }
);

struct tracker_check cafe_checks[] = TRACKER_CHECK_BUNDLE(
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
);

struct tracker_check habitat_checks[] = TRACKER_CHECK_BUNDLE(
    {
        .type = TRACKER_CHECK_NAMED,
        .data = {.named_check = {
            .str_id = BLUE_GOOMI_2_CHECK_STR_ID,
            .subx_bit = 21
        }}
    },
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
);

struct tracker_check beach_cave_checks[] = TRACKER_CHECK_BUNDLE(
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
);
// TODO: Add starting information.

struct tracker_check mt_bristle_checks[] = TRACKER_CHECK_BUNDLE(
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
);

struct tracker_check apple_woods_checks[] = TRACKER_CHECK_BUNDLE(
    {
        .type = TRACKER_CHECK_BAG_UPGRADE,
        .data = {.bag_upgrade_check = {
            .number = 3, .subx_bit = 2
        }}
    }
);

struct tracker_check bidoof_escort_info[] = TRACKER_CHECK_BUNDLE(
    {
        .type = TRACKER_CHECK_ESCORT_INFO,
        .data = {.escort_info = {
            .id_0 = MONSTER_BIDOOF,
        }}
    }
);

struct tracker_check steam_cave_checks[] = TRACKER_CHECK_BUNDLE(
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
        .type = TRACKER_CHECK_GIFT,
        .data = {.gift_check = {
            .id = MONSTER_UXIE,
            .subx_bit = 25
        }},
        .is_check_active_func = IsDarkraiGoal,
    },
    {
        .type = TRACKER_CHECK_BOSS_INFO,
        .data = {.boss_info = {
            .id_0 = MONSTER_UXIE,
        }},
        .is_check_active_func = IsDarkraiGoal,
    }
);

struct tracker_check amp_plains_checks[] = TRACKER_CHECK_BUNDLE(
    {
        .type = TRACKER_CHECK_BOSS_DUO_INFO,
        .data = {.boss_duo_info = {
            .id_0 = MONSTER_MANECTRIC,
            .id_1 = MONSTER_ELECTRIKE,
        }}
    }
);

struct tracker_check quicksand_cave_checks[] = TRACKER_CHECK_BUNDLE(
    {
        .type = TRACKER_CHECK_GIFT,
        .data = {.gift_check = {
            .id = MONSTER_MESPRIT,
            .subx_bit = 26
        }},
        .is_check_active_func = IsDarkraiGoal,
    },
    {
        .type = TRACKER_CHECK_BOSS_INFO,
        .data = {.boss_info = {
            .id_0 = MONSTER_MESPRIT,
        }},
    }
);

struct tracker_check crystal_crossing_checks[] = TRACKER_CHECK_BUNDLE(
    {
        .type = TRACKER_CHECK_BOSS_INFO,
        .data = {.boss_info = {
            .id_0 = MONSTER_GROVYLE,
        }},
    },
    // TODO: Add scripted loss info here?
    {
        .type = TRACKER_CHECK_GIFT,
        .data = {.gift_check = {
            .id = MONSTER_AZELF,
            .subx_bit = 26
        }},
        .is_check_active_func = IsDarkraiGoal,
    },
    {
        .type = TRACKER_CHECK_BOSS_INFO,
        .data = {.boss_info = {
            .id_0 = MONSTER_AZELF,
        }},
        .is_check_active_func = IsDarkraiGoal,
    }
);

struct tracker_check sealed_ruin_checks[] = TRACKER_CHECK_BUNDLE(
    {
        .type = TRACKER_CHECK_BOSS_INFO,
        .data = {.boss_info = {
            .id_0 = MONSTER_SPIRITOMB,
        }}
    }
);

struct tracker_check grovyle_escort_info[] = TRACKER_CHECK_BUNDLE(
    {
        .type = TRACKER_CHECK_ESCORT_INFO,
        .data = {.escort_info = {
            .id_0 = MONSTER_GROVYLE,
        }}
    }
);

struct tracker_check dusk_forest_checks[] = TRACKER_CHECK_BUNDLE(
    {
        .type = TRACKER_CHECK_ESCORT_DUO_INFO,
        .data = {.escort_duo_info = {
            .id_0 = MONSTER_GROVYLE,
            .id_1 = MONSTER_CELEBI
        }}
    }
);

struct tracker_check brine_cave_checks[] = TRACKER_CHECK_BUNDLE(
    {
        .type = TRACKER_CHECK_BOSS_DUO_INFO,
        .data = {.boss_duo_info = {
            .id_0 = MONSTER_KABUTOPS,
            .id_1 = MONSTER_OMASTAR,
        }}
    }
);
// TODO: Add conditional Chatot escort.

struct tracker_check labyrinth_cave_checks[] = TRACKER_CHECK_BUNDLE(
    {
        .type = TRACKER_CHECK_BOSS_INFO,
        .data = {.boss_info = {
            .id_0 = MONSTER_GABITE,
        }}
    }
);

struct tracker_check mystifying_forest_checks[] = TRACKER_CHECK_BUNDLE(
    {
        .type = TRACKER_CHECK_INFO,
        .data = {.general_info = {
            .str_id = GRANDMASTER_OF_ALL_THINGS_BAD_BOSS_INFO_STR_ID,
        }}
    },
    {
        .type = TRACKER_CHECK_BAG_UPGRADE,
        .data = {.bag_upgrade_check = {
            .number = 5,
            .subx_bit = 4,
        }}
    }
);

struct tracker_check crevice_cave_checks[] = TRACKER_CHECK_BUNDLE(
    {
        .type = TRACKER_CHECK_GIFT,
        .data = {.gift_check = {
            .id = MONSTER_FROSLASS,
            .subx_bit = 47
        }}
    },
    {
        .type = TRACKER_CHECK_BOSS_INFO,
        .data = {.boss_info = {
            .id_0 = MONSTER_FROSLASS,
        }}
    }
);

struct tracker_check surrounded_sea_checks[] = TRACKER_CHECK_BUNDLE(
    {
        .type = TRACKER_CHECK_GIFT,
        .data = {.gift_check = {
            .id = MONSTER_PHIONE,
            .subx_bit = 29
        }}
    },
    {
        .type = TRACKER_CHECK_BOSS_INFO,
        .data = {.boss_info = {
            .id_0 = MONSTER_GYARADOS,
        }}
    }
);

struct tracker_check aegis_cave_checks[] = TRACKER_CHECK_BUNDLE(
    {
        .type = TRACKER_CHECK_GIFT,
        .data = {.gift_check = {
            .id = MONSTER_REGICE,
            .subx_bit = 69
        }}
    },
    {
        .type = TRACKER_CHECK_BOSS_INFO,
        .data = {.boss_info = {
            .id_0 = MONSTER_REGICE,
        }}
    },
    {
        .type = TRACKER_CHECK_GIFT,
        .data = {.gift_check = {
            .id = MONSTER_REGIROCK,
            .subx_bit = 70
        }}
    },
    {
        .type = TRACKER_CHECK_BOSS_INFO,
        .data = {.boss_info = {
            .id_0 = MONSTER_REGIROCK,
        }}
    },
    {
        .type = TRACKER_CHECK_GIFT,
        .data = {.gift_check = {
            .id = MONSTER_REGISTEEL,
            .subx_bit = 71
        }}
    },
    {
        .type = TRACKER_CHECK_BOSS_INFO,
        .data = {.boss_info = {
            .id_0 = MONSTER_REGISTEEL,
        }}
    },
    {
        .type = TRACKER_CHECK_GIFT,
        .data = {.gift_check = {
            .id = MONSTER_REGIGIGAS,
            .subx_bit = 72
        }}
    },
    {
        .type = TRACKER_CHECK_BOSS_INFO,
        .data = {.boss_info = {
            .id_0 = MONSTER_REGIGIGAS,
        }}
    },
    {
        .type = TRACKER_CHECK_DUNGEON_CONQUEST,
        .data = {.dungeon_conquest_check = {
            .dungeon = DUNGEON_ICE_AEGIS_CAVE,
        }}
    },
    {
        .type = TRACKER_CHECK_DUNGEON_CONQUEST,
        .data = {.dungeon_conquest_check = {
            .dungeon = DUNGEON_REGICE_CHAMBER,
        }}
    },
    {
        .type = TRACKER_CHECK_DUNGEON_CONQUEST,
        .data = {.dungeon_conquest_check = {
            .dungeon = DUNGEON_ROCK_AEGIS_CAVE,
        }}
    },
    {
        .type = TRACKER_CHECK_DUNGEON_CONQUEST,
        .data = {.dungeon_conquest_check = {
            .dungeon = DUNGEON_REGIROCK_CHAMBER,
        }}
    },
    {
        .type = TRACKER_CHECK_DUNGEON_CONQUEST,
        .data = {.dungeon_conquest_check = {
            .dungeon = DUNGEON_STEEL_AEGIS_CAVE,
        }}
    },
    {
        .type = TRACKER_CHECK_DUNGEON_CONQUEST,
        .data = {.dungeon_conquest_check = {
            .dungeon = DUNGEON_REGISTEEL_CHAMBER,
        }}
    },
    {
        .type = TRACKER_CHECK_DUNGEON_CONQUEST,
        .data = {.dungeon_conquest_check = {
            .dungeon = DUNGEON_AEGIS_CAVE_PIT,
        }}
    },
    {
        .type = TRACKER_CHECK_DUNGEON_CONQUEST,
        .data = {.dungeon_conquest_check = {
            .dungeon = DUNGEON_REGIGIGAS_CHAMBER,
        }}
    }
);

struct tracker_check spacial_rift_checks[] = TRACKER_CHECK_BUNDLE(
    {
        .type = TRACKER_CHECK_GIFT,
        .data = {.gift_check = {
            .id = MONSTER_PALKIA,
            .subx_bit = 30
        }}
    },
    {
        .type = TRACKER_CHECK_BOSS_INFO,
        .data = {.boss_info = {
            .id_0 = MONSTER_PALKIA,
        }}
    }
);

struct tracker_check bottomless_sea_checks[] = TRACKER_CHECK_BUNDLE(
    {
        .type = TRACKER_CHECK_GIFT,
        .data = {.gift_check = {
            .id = MONSTER_KYOGRE,
            .subx_bit = 32
        }}
    },
    {
        .type = TRACKER_CHECK_BOSS_INFO,
        .data = {.boss_info = {
            .id_0 = MONSTER_KYOGRE,
        }}
    },
    {
        .type = TRACKER_CHECK_ITEM,
        .data = {.item_check = {
            .id = ITEM_AQUA_MONICA,
            .subx_bit = 31
        }}
    }
);

struct tracker_check shimmer_desert_checks[] = TRACKER_CHECK_BUNDLE(
    {
        .type = TRACKER_CHECK_GIFT,
        .data = {.gift_check = {
            .id = MONSTER_GROUDON,
            .subx_bit = 34
        }}
    },
    {
        .type = TRACKER_CHECK_BOSS_INFO,
        .data = {.boss_info = {
            .id_0 = MONSTER_GROUDON,
        }}
    },
    {
        .type = TRACKER_CHECK_ITEM,
        .data = {.item_check = {
            .id = ITEM_TERRA_CYMBAL,
            .subx_bit = 33
        }}
    }
);

struct tracker_check mt_avalanche_checks[] = TRACKER_CHECK_BUNDLE(
    {
        .type = TRACKER_CHECK_GIFT,
        .data = {.gift_check = {
            .id = MONSTER_ARTICUNO,
            .subx_bit = 36
        }}
    },
    {
        .type = TRACKER_CHECK_BOSS_INFO,
        .data = {.boss_info = {
            .id_0 = MONSTER_ARTICUNO,
        }}
    },
    {
        .type = TRACKER_CHECK_ITEM,
        .data = {.item_check = {
            .id = ITEM_ICY_FLUTE,
            .subx_bit = 35
        }}
    }
);

struct tracker_check giant_volcano_checks[] = TRACKER_CHECK_BUNDLE(
    {
        .type = TRACKER_CHECK_GIFT,
        .data = {.gift_check = {
            .id = MONSTER_HEATRAN,
            .subx_bit = 38
        }}
    },
    {
        .type = TRACKER_CHECK_BOSS_INFO,
        .data = {.boss_info = {
            .id_0 = MONSTER_HEATRAN,
        }}
    },
    {
        .type = TRACKER_CHECK_ITEM,
        .data = {.item_check = {
            .id = ITEM_FIERY_DRUM,
            .subx_bit = 37
        }}
    }
);

struct tracker_check world_abyss_checks[] = TRACKER_CHECK_BUNDLE(
    {
        .type = TRACKER_CHECK_GIFT,
        .data = {.gift_check = {
            .id = MONSTER_GIRATINA_ALTERED,
            .subx_bit = 40
        }}
    },
    {
        .type = TRACKER_CHECK_BOSS_INFO,
        .data = {.boss_info = {
            .id_0 = MONSTER_GIRATINA_ALTERED,
        }}
    },
    {
        .type = TRACKER_CHECK_ITEM,
        .data = {.item_check = {
            .id = ITEM_FIERY_DRUM,
            .subx_bit = 39
        }}
    }
);

struct tracker_check sky_stairway_checks[] = TRACKER_CHECK_BUNDLE(
    {
        .type = TRACKER_CHECK_GIFT,
        .data = {.gift_check = {
            .id = MONSTER_RAYQUAZA,
            .subx_bit = 42
        }}
    },
    {
        .type = TRACKER_CHECK_BOSS_INFO,
        .data = {.boss_info = {
            .id_0 = MONSTER_RAYQUAZA,
        }}
    },
    {
        .type = TRACKER_CHECK_ITEM,
        .data = {.item_check = {
            .id = ITEM_SKY_MELODICA,
            .subx_bit = 41
        }}
    }
);

struct tracker_check mystery_jungle_checks[] = TRACKER_CHECK_BUNDLE(
    {
        .type = TRACKER_CHECK_GIFT,
        .data = {.gift_check = {
            .id = MONSTER_MEW,
            .subx_bit = 42
        }}
    },
    {
        .type = TRACKER_CHECK_BOSS_INFO,
        .data = {.boss_info = {
            .id_0 = MONSTER_MEW,
        }}
    },
    {
        .type = TRACKER_CHECK_ITEM,
        .data = {.item_check = {
            .id = ITEM_GRASS_CORNET,
            .subx_bit = 41
        }}
    }
);

struct tracker_check first_station_pass_checks[] = TRACKER_CHECK_BUNDLE(
    {
        .type = TRACKER_CHECK_ESCORT_INFO,
        .data = {.escort_info = {
            .id_0 = MONSTER_SHAYMIN_LAND,
        }}
    }
);

struct tracker_check third_station_pass_checks[] = TRACKER_CHECK_BUNDLE(
    {
        .type = TRACKER_CHECK_ESCORT_INFO,
        .data = {.escort_info = {
            .id_0 = MONSTER_SHAYMIN_LAND,
        }}
    }
);

struct tracker_check fifth_station_pass_checks[] = TRACKER_CHECK_BUNDLE(
    {
        .type = TRACKER_CHECK_BOSS_INFO,
        .data = {.boss_info = {
            .id_0 = MONSTER_CARNIVINE,
        }}
    },
    {
        .type = TRACKER_CHECK_ESCORT_INFO,
        .data = {.escort_info = {
            .id_0 = MONSTER_SHAYMIN_LAND,
        }},
    }
);

struct tracker_check seventh_station_pass_checks[] = TRACKER_CHECK_BUNDLE(
    {
        .type = TRACKER_CHECK_ESCORT_INFO,
        .data = {.escort_info = {
            .id_0 = MONSTER_SHAYMIN_LAND,
        }}
    }
);

struct tracker_check eighth_station_pass_checks[] = TRACKER_CHECK_BUNDLE(
    {
        .type = TRACKER_CHECK_NAMED,
        .data = {.named_check = {
            .str_id = SNEASELS_GRATITUDE_CHECK_STR_ID,
            .subx_bit = 65
        }}
    },
    {
        .type = TRACKER_CHECK_ESCORT_INFO,
        .data = {.escort_info = {
            .id_0 = MONSTER_SHAYMIN_LAND,
        }},
    }
);

struct tracker_check ninth_station_pass_checks[] = TRACKER_CHECK_BUNDLE(
    {
        .type = TRACKER_CHECK_ESCORT_INFO,
        .data = {.escort_info = {
            .id_0 = MONSTER_SHAYMIN_LAND,
        }}
    }
);

struct tracker_check shaymin_escort_info[] = TRACKER_CHECK_BUNDLE(
    {
        .type = TRACKER_CHECK_ESCORT_INFO,
        .data = {.escort_info = {
            .id_0 = MONSTER_SHAYMIN_LAND,
        }}
    }
);

struct tracker_check sky_peak_summit_checks[] = TRACKER_CHECK_BUNDLE(
    {
        .type = TRACKER_CHECK_BOSS_DUO_INFO,
        .data = {.boss_info = {
            .id_0 = MONSTER_MUK,
            .id_1 = MONSTER_GRIMER
        }}
    },
    {
        .type = TRACKER_CHECK_GIFT,
        .data = {.gift_check = {
            .id = MONSTER_SHAYMIN_LAND,
            .subx_bit = 46
        }}
    },
    {
        .type = TRACKER_CHECK_ESCORT_INFO,
        .data = {.escort_info = {
            .id_0 = MONSTER_SHAYMIN_LAND,
        }},
    }
);

struct tracker_check se_bidoofs_wish_checks[] = TRACKER_CHECK_BUNDLE(
    {
        .type = TRACKER_CHECK_NAMED,
        .data = {.named_check = {
            .str_id = SE_MAROWACK_DOJO_REVIVAL_CHECK_STR_ID,
            .subx_bit = 66,
        }}
    },
    {
        .type = TRACKER_CHECK_SPECIAL_EPISODE_DUNGEON_CONQUEST,
        .data = {.special_episode_dungeon_conquest_check = {
            .dungeon = DUNGEON_DEEP_STAR_CAVE
        }}
    },
    {
        .type = TRACKER_CHECK_SPECIAL_EPISODE_DUNGEON_CONQUEST,
        .data = {.special_episode_dungeon_conquest_check = {
            .dungeon = DUNGEON_STAR_CAVE_PIT
        }}
    }
);

struct tracker_check se_igglybuff_the_prodigy_checks[] = TRACKER_CHECK_BUNDLE(
    {
        .type = TRACKER_CHECK_SPECIAL_EPISODE_DUNGEON_CONQUEST,
        .data = {.special_episode_dungeon_conquest_check = {
            .dungeon = DUNGEON_MURKY_FOREST
        }}
    },
    {
        .type = TRACKER_CHECK_SPECIAL_EPISODE_DUNGEON_CONQUEST,
        .data = {.special_episode_dungeon_conquest_check = {
            .dungeon = DUNGEON_EASTERN_CAVE
        }}
    },
    {
        .type = TRACKER_CHECK_SPECIAL_EPISODE_DUNGEON_CONQUEST,
        .data = {.special_episode_dungeon_conquest_check = {
            .dungeon = DUNGEON_FORTUNE_RAVINE_PIT
        }}
    }
);

struct tracker_check se_todays_oh_my_gosh_checks[] = TRACKER_CHECK_BUNDLE(
    {
        .type = TRACKER_CHECK_SPECIAL_EPISODE_DUNGEON_CONQUEST,
        .data = {.special_episode_dungeon_conquest_check = {
            .dungeon = DUNGEON_UPPER_SPRING_CAVE
        }}
    },
    {
        .type = TRACKER_CHECK_SPECIAL_EPISODE_DUNGEON_CONQUEST,
        .data = {.special_episode_dungeon_conquest_check = {
            .dungeon = DUNGEON_MIDDLE_SPRING_CAVE
        }}
    },
    {
        .type = TRACKER_CHECK_SPECIAL_EPISODE_DUNGEON_CONQUEST,
        .data = {.special_episode_dungeon_conquest_check = {
            .dungeon = DUNGEON_SPRING_CAVE_PIT
        }}
    }
);

struct tracker_check se_here_comes_team_charm_checks[] = TRACKER_CHECK_BUNDLE(
    {
        .type = TRACKER_CHECK_SPECIAL_EPISODE_DUNGEON_CONQUEST,
        .data = {.special_episode_dungeon_conquest_check = {
            .dungeon = DUNGEON_SOUTHERN_JUNGLE
        }}
    },
    {
        .type = TRACKER_CHECK_SPECIAL_EPISODE_DUNGEON_CONQUEST,
        .data = {.special_episode_dungeon_conquest_check = {
            .dungeon = DUNGEON_BOULDER_QUARRY_CLEARING
        }}
    },
    {
        .type = TRACKER_CHECK_SPECIAL_EPISODE_DUNGEON_CONQUEST,
        .data = {.special_episode_dungeon_conquest_check = {
            .dungeon = DUNGEON_RIGHT_CAVE_PATH
        }}
    },
    {
        .type = TRACKER_CHECK_SPECIAL_EPISODE_DUNGEON_CONQUEST,
        .data = {.special_episode_dungeon_conquest_check = {
            .dungeon = DUNGEON_LEFT_CAVE_PATH
        }}
    },
    {
        .type = TRACKER_CHECK_SPECIAL_EPISODE_DUNGEON_CONQUEST,
        .data = {.special_episode_dungeon_conquest_check = {
            .dungeon = DUNGEON_LIMESTONE_CAVERN_DEPTHS
        }}
    }
);

struct tracker_check se_into_the_future_of_darkness_checks[] = TRACKER_CHECK_BUNDLE(
    {
        .type = TRACKER_CHECK_SPECIAL_EPISODE_DUNGEON_CONQUEST,
        .data = {.special_episode_dungeon_conquest_check = {
            .dungeon = DUNGEON_BARREN_VALLEY_CLEARING
        }}
    },
    {
        .type = TRACKER_CHECK_SPECIAL_EPISODE_DUNGEON_CONQUEST,
        .data = {.special_episode_dungeon_conquest_check = {
            .dungeon = DUNGEON_DARK_WASTELAND
        }}
    },
    {
        .type = TRACKER_CHECK_SPECIAL_EPISODE_DUNGEON_CONQUEST,
        .data = {.special_episode_dungeon_conquest_check = {
            .dungeon = DUNGEON_TEMPORAL_SPIRE_SE5
        }}
    },
    {
        .type = TRACKER_CHECK_SPECIAL_EPISODE_DUNGEON_CONQUEST,
        .data = {.special_episode_dungeon_conquest_check = {
            .dungeon = DUNGEON_BLACK_SWAMP
        }}
    },
    {
        .type = TRACKER_CHECK_SPECIAL_EPISODE_DUNGEON_CONQUEST,
        .data = {.special_episode_dungeon_conquest_check = {
            .dungeon = DUNGEON_SPACIAL_CLIFFS
        }}
    },
    {
        .type = TRACKER_CHECK_SPECIAL_EPISODE_DUNGEON_CONQUEST,
        .data = {.special_episode_dungeon_conquest_check = {
            .dungeon = DUNGEON_DARK_ICE_MOUNTAIN_PINNACLE
        }}
    },
    {
        .type = TRACKER_CHECK_SPECIAL_EPISODE_DUNGEON_CONQUEST,
        .data = {.special_episode_dungeon_conquest_check = {
            .dungeon = DUNGEON_ICICLE_FOREST
        }}
    },
    {
        .type = TRACKER_CHECK_SPECIAL_EPISODE_DUNGEON_CONQUEST,
        .data = {.special_episode_dungeon_conquest_check = {
            .dungeon = DUNGEON_VAST_ICE_MOUNTAIN_PINNACLE
        }}
    }
);

/* tracker_book
    A list of tracker_page_entries. Every value for the
    tracker_page enum should have an entry in the
    tracker_book. Because of the way the tracker_book
    is defined, the ordering of the pages can be shuffled
    around without modifying the decleration of
    tracker_book.
*/
struct tracker_page_entry tracker_book[] = {
    [TRACKER_PAGE_SHOP] = {
        .name_str_id = SHOP_STR_ID,
        .checks = &shop_checks,
    },
    [TRACKER_PAGE_BANK] = {
        .name_str_id = BANK_STR_ID,
        .checks = &bank_checks,
    },
    [TRACKER_PAGE_RANK] = {
        .name_str_id = RANK_CHECK_STR_ID,
        .checks = &rank_checks,
    },
    [TRACKER_PAGE_GUILD] = {
        .name_str_id = GUILD_STR_ID,
        .checks = &guild_checks,
    },
    [TRACKER_PAGE_DOJO] = {
        .name_str_id = DOJO_STR_ID,
        .checks = &dojo_checks,
    },
    [TRACKER_PAGE_CAFE] = {
        .name_str_id = CAFE_STR_ID,
        .checks = &cafe_checks,
    },
    [TRACKER_PAGE_HABITAT] = {
        .name_str_id = HABITAT_STR_ID,
        .is_page_active_func = IsDarkraiGoal,
        .checks = &habitat_checks,
    },
    [TRACKER_PAGE_BEACH_CAVE] = {
        .dungeon = DUNGEON_BEACH_CAVE,
        .checks = &beach_cave_checks,
    },
    [TRACKER_PAGE_DRENCHED_BLUFF] = {
        .dungeon = DUNGEON_DRENCHED_BLUFF,
        .checks = &empty_check,
    },
    [TRACKER_PAGE_MT_BRISTLE] = {
        .dungeon = DUNGEON_MT_BRISTLE,
        .checks = &mt_bristle_checks,
    },
    [TRACKER_PAGE_WATERFALL_CAVE] = {
        .dungeon = DUNGEON_WATERFALL_CAVE,
        .checks = &empty_check,
    },
    [TRACKER_PAGE_APPLE_WOODS] = {
        .dungeon = DUNGEON_APPLE_WOODS,
        .checks = &apple_woods_checks,
    },
    [TRACKER_PAGE_CRAGGY_COAST] = {
        .dungeon = DUNGEON_CRAGGY_COAST,
        .checks = &bidoof_escort_info,
    },
    [TRACKER_PAGE_SIDE_PATH] = {
        .dungeon = DUNGEON_SIDE_PATH,
        .checks = &bidoof_escort_info
    },
    [TRACKER_PAGE_MT_HORN] = {
        .dungeon = DUNGEON_MT_HORN,
        .checks = &bidoof_escort_info
    },
    [TRACKER_PAGE_ROCK_PATH] = {
        .dungeon = DUNGEON_ROCK_PATH,
        .checks = &bidoof_escort_info
    },
    [TRACKER_PAGE_FOGGY_FOREST] = {
        .dungeon = DUNGEON_FOGGY_FOREST,
        .checks = &empty_check,
    },
    [TRACKER_PAGE_FOREST_PATH] = {
        .dungeon = DUNGEON_FOREST_PATH,
        .checks = &empty_check,
    },
    [TRACKER_PAGE_STEAM_CAVE] = {
        .dungeon = DUNGEON_STEAM_CAVE,
        .checks = &steam_cave_checks,
    },
    [TRACKER_PAGE_AMP_PLAINS] = {
        .dungeon = DUNGEON_AMP_PLAINS,
        .checks = &amp_plains_checks,
    },
    [TRACKER_PAGE_NORTHERN_DESERT] = {
        .dungeon = DUNGEON_NORTHERN_DESERT,
        .checks = &empty_check,
    },
    [TRACKER_PAGE_QUICKSAND_CAVE] = {
        .dungeon = DUNGEON_QUICKSAND_CAVE,
        .checks = &quicksand_cave_checks,
    },
    [TRACKER_PAGE_CRYSTAL_CAVE] = {
        .dungeon = DUNGEON_CRYSTAL_CAVE,
        .checks = &empty_check,
    },
    [TRACKER_PAGE_CRYSTAL_CROSSING] = {
        .dungeon = DUNGEON_CRYSTAL_CROSSING,
        .checks = &crystal_crossing_checks,
    },
    [TRACKER_PAGE_CHASM_CAVE] = {
        .dungeon = DUNGEON_CHASM_CAVE,
        .checks = &empty_check,
    },
    [TRACKER_PAGE_DARK_HILL] = {
        .dungeon = DUNGEON_DARK_HILL,
        .checks = &empty_check,
    },
    [TRACKER_PAGE_SEALED_RUIN] = {
        .dungeon = DUNGEON_SEALED_RUIN,
        .checks = &sealed_ruin_checks,
    },
    [TRACKER_PAGE_DUSK_FOREST] = {
        .dungeon = DUNGEON_DUSK_FOREST,
        .checks = &grovyle_escort_info,
    },
    [TRACKER_PAGE_DEEP_DUSK_FOREST] = {
        .dungeon = DUNGEON_DUSK_FOREST,
        .checks = &dusk_forest_checks,
    },
    [TRACKER_PAGE_TREESHROUD_FOREST] = {
        .dungeon = DUNGEON_TREESHROUD_FOREST,
        .checks = &grovyle_escort_info,
    },
    [TRACKER_PAGE_BRINE_CAVE] = {
        .dungeon = DUNGEON_BRINE_CAVE,
        .checks = &brine_cave_checks,
    },
    [TRACKER_PAGE_SERENITY_RIVER] = {
        .dungeon = DUNGEON_SERENITY_RIVER,
        .checks = &empty_check,
    },
    [TRACKER_PAGE_LANDSLIDE_CAVE] = {
        .dungeon = DUNGEON_LANDSLIDE_CAVE,
        .checks = &empty_check,
    },
    [TRACKER_PAGE_LUSH_PRAIRIE] = {
        .dungeon = DUNGEON_LUSH_PRAIRIE,
        .checks = &empty_check,
    },
    [TRACKER_PAGE_TINY_MEADOW] = {
        .dungeon = DUNGEON_TINY_MEADOW,
        .checks = &empty_check,
    },
    [TRACKER_PAGE_LABYRINTH_CAVE] = {
        .dungeon = DUNGEON_LABYRINTH_CAVE,
        .checks = &labyrinth_cave_checks,
    },
    [TRACKER_PAGE_ORAN_FOREST] = {
        .dungeon = DUNGEON_ORAN_FOREST,
        .checks = &empty_check,
    },
    [TRACKER_PAGE_STAR_CAVE] = {
        .dungeon = DUNGEON_STAR_CAVE,
        .checks = &empty_check,
    },
    [TRACKER_PAGE_HIDDEN_LAND] = {
        .dungeon = DUNGEON_HIDDEN_LAND,
        .checks = &empty_check,
    },
    [TRACKER_PAGE_TEMPORAL_TOWER] = {
        .dungeon = DUNGEON_TEMPORAL_TOWER,
        .checks = &empty_check,
    },
    [TRACKER_PAGE_MYSTIFYING_FOREST] = {
        .dungeon = DUNGEON_MYSTIFYING_FOREST,
        .is_page_active_func = IsDarkraiGoal,
        .checks = &mystifying_forest_checks,
    },
    [TRACKER_PAGE_BLIZZARD_ISLAND] = {
        .dungeon = DUNGEON_DRENCHED_BLUFF,
        .is_page_active_func = IsDarkraiGoal,
        .checks = &empty_check,
    },
    [TRACKER_PAGE_CREVICE_CAVE] = {
        .dungeon = DUNGEON_CREVICE_CAVE,
        .checks = &crevice_cave_checks,
    },
    [TRACKER_PAGE_SURROUNDED_SEA] = {},
    [TRACKER_PAGE_MIRACLE_SEA] = {
        .dungeon = DUNGEON_MIRACLE_SEA,
        .is_page_active_func = IsDarkraiGoal,
        .checks = &surrounded_sea_checks,
    },
    [TRACKER_PAGE_AEGIS_CAVE] = {
        .name_str_id = AEGIS_CAVE_STR_ID,
        .is_page_active_func = IsDarkraiGoal,
        .checks = &aegis_cave_checks,
    },
    [TRACKER_PAGE_MT_TRAVAIL] = {},
    [TRACKER_PAGE_THE_NIGHTMARE] = {},
    [TRACKER_PAGE_SPACIAL_RIFT] = {
        .dungeon = DUNGEON_SPACIAL_RIFT,
        .is_page_active_func = IsDarkraiGoal,
        .checks = &spacial_rift_checks,
    },
    [TRACKER_PAGE_CONCEALED_RUINS] = {},
    [TRACKER_PAGE_MARINE_RESORT] = {},
    [TRACKER_PAGE_BOTTOMLESS_SEA] = {
        .dungeon = DUNGEON_BOTTOMLESS_SEA,
        .is_page_active_func = IsDarkraiGoal,
        .checks = &bottomless_sea_checks,
    },
    [TRACKER_PAGE_SHIMMER_DESERT] = {
        .dungeon = DUNGEON_SHIMMER_DESERT,
        .is_page_active_func = IsDarkraiGoal,
        .checks = &shimmer_desert_checks,
    },
    [TRACKER_PAGE_MT_AVALANCHE] = {
        .dungeon = DUNGEON_MT_AVALANCHE,
        .is_page_active_func = IsDarkraiGoal,
        .checks = &mt_avalanche_checks,
    },
    [TRACKER_PAGE_GIANT_VOLCANO] = {
        .dungeon = DUNGEON_GIANT_VOLCANO,
        .is_page_active_func = IsDarkraiGoal,
        .checks = &giant_volcano_checks,
    },
    [TRACKER_PAGE_WORLD_ABYSS] = {
        .dungeon = DUNGEON_WORLD_ABYSS,
        .is_page_active_func = IsDarkraiGoal,
        .checks = &world_abyss_checks,
    },
    [TRACKER_PAGE_SKY_STAIRWAY] = {
        .dungeon = DUNGEON_SKY_STAIRWAY,
        .is_page_active_func = IsDarkraiGoal,
        .checks = &sky_stairway_checks,
    },
    [TRACKER_PAGE_MYSTERY_JUNGLE] = {
        .dungeon = DUNGEON_MYSTERY_JUNGLE,
        .is_page_active_func = IsDarkraiGoal,
        .checks = &mystery_jungle_checks,
    },
    [TRACKER_PAGE_LAKE_AFAR] = {},
    [TRACKER_PAGE_HAPPY_OUTLOOK] = {},
    [TRACKER_PAGE_MY_MISTRAL] = {},
    [TRACKER_PAGE_SHIMMER_HILL] = {},
    [TRACKER_PAGE_LOST_WILDERNESS] = {},
    [TRACKER_PAGE_MIDNIGHT_FOREST] = {},
    [TRACKER_PAGE_1ST_STATION_PASS] = {
        .dungeon = DUNGEON_1ST_STATION_PASS,
        .is_page_active_func = IsDarkraiGoal,
        .checks = &first_station_pass_checks,
    },
    [TRACKER_PAGE_2ND_STATION_PASS] = {
        .dungeon = DUNGEON_2ND_STATION_PASS,
        .is_page_active_func = IsDarkraiGoal,
        .checks = &shaymin_escort_info,
    },
    [TRACKER_PAGE_3RD_STATION_PASS] = {
        .dungeon = DUNGEON_3RD_STATION_PASS,
        .is_page_active_func = IsDarkraiGoal,
        .checks = &third_station_pass_checks,
    },
    [TRACKER_PAGE_4TH_STATION_PASS] = {
        .dungeon = DUNGEON_4TH_STATION_PASS,
        .is_page_active_func = IsDarkraiGoal,
        .checks = &shaymin_escort_info,
    },
    [TRACKER_PAGE_5TH_STATION_PASS] = {
        .dungeon = DUNGEON_5TH_STATION_PASS,
        .is_page_active_func = IsDarkraiGoal,
        .checks = &fifth_station_pass_checks,
    },
    [TRACKER_PAGE_6TH_STATION_PASS] = {
        .dungeon = DUNGEON_6TH_STATION_PASS,
        .is_page_active_func = IsDarkraiGoal,
        .checks = &shaymin_escort_info,
    },
    [TRACKER_PAGE_7TH_STATION_PASS] = {
        .dungeon = DUNGEON_7TH_STATION_PASS,
        .is_page_active_func = IsDarkraiGoal,
        .checks = &seventh_station_pass_checks,
    },
    [TRACKER_PAGE_8TH_STATION_PASS] = {
        .dungeon = DUNGEON_8TH_STATION_PASS,
        .is_page_active_func = IsDarkraiGoal,
        .checks = &eighth_station_pass_checks,
    },
    [TRACKER_PAGE_9TH_STATION_PASS] = {
        .dungeon = DUNGEON_9TH_STATION_PASS,
        .is_page_active_func = IsDarkraiGoal,
        .checks = &shaymin_escort_info,
    },
    [TRACKER_PAGE_SKY_PEAK_SUMMIT_PASS] = {
        .dungeon = DUNGEON_SKY_PEAK_SUMMIT_PASS,
        .is_page_active_func = IsDarkraiGoal,
        .checks = &shaymin_escort_info,
    },
    [TRACKER_PAGE_SKY_PEAK_SUMMIT] = {
        .dungeon = DUNGEON_SKY_PEAK_SUMMIT,
        .is_page_active_func = IsDarkraiGoal,
        .checks = &sky_peak_summit_checks,
    },
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
    [TRACKER_PAGE_DARK_CRATER] = {},
    [TRACKER_PAGE_SE_BIDOOFS_WISH] = {
        .name_str_id = BIDOOF_SE_STR_ID,
        .is_page_active_func = NULL, // TODO: Function to check if SE are included.
        .checks = &se_bidoofs_wish_checks,
    },
    [TRACKER_PAGE_SE_IGGLYBUFF_THE_PRODIGY] = {
        .name_str_id = IGGLYBUFF_SE_STR_ID,
        .is_page_active_func = NULL, // TODO: Function to check if SE are included.
        .checks = &se_igglybuff_the_prodigy_checks,
    },
    [TRACKER_PAGE_SE_TODAYS_OH_MY_GOSH] = {
        .name_str_id = OH_MY_GOSH_SE_STR_ID,
        .is_page_active_func = NULL, // TODO: Function to check if SE are included.
        .checks = se_todays_oh_my_gosh_checks,
    },
    [TRACKER_PAGE_SE_HERE_COMES_TEAM_CHARM] = {
        .name_str_id = TEAM_CHARM_SE_STR_ID,
        .is_page_active_func = NULL, // TODO: Function to check if SE are included.
        .checks = &se_here_comes_team_charm_checks,
    },
    [TRACKER_PAGE_SE_INTO_THE_FUTURE_OF_DARKNESS] = {
        .name_str_id = FUTURE_SE_STR_ID,
        .is_page_active_func = NULL, // TODO: Function to check if SE are included.
        .checks = &se_into_the_future_of_darkness_checks,
    },
};