#include <pmdsky.h>
#include <cot.h>

enum dungeon_check_type {
    DCT_OTHER = 0,
    DCT_EARLY = 1,
    DCT_LATE = 2,
    DCT_RULE = 3
};

enum dungeon_check_type GetDungeonCheckType(short dunId);

bool GetSubXBit(int bit);

int GetRemainingDungeonMissionChecks(enum dungeon_id dungeon, bool outlaw);

bool IsDarkraiGoal();

bool AreLongLocationsOn();

enum dungeon_mode GetDungeonModeWithSevenTreasureCheck(enum dungeon_id dungeon_id);

bool IsLookaLikeDungeonPlaceholder(enum dungeon_id dungeon_id);