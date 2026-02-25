#include <pmdsky.h>
#include <cot.h>

bool debug_mode = false;



enum dungeon_check_type {
    DCT_OTHER = 0,
    DCT_EARLY = 1,
    DCT_LATE = 2,
    DCT_RULE = 3
};

/* GetDungeonCheckType
    Returns the type of missions checks that this dungeon has.

    dungeon_id: dungeon to get the type of mission checks
    return: dungeon_check_type */
enum dungeon_check_type GetDungeonCheckType(short dungeon_id);

/* GetSubXBit
    Returns the status of the xth bit in the corresponding
    VAR_SCENARIO_SUBX scripting variable there are 8
    VAR_SCENARIO_SUBX variables with two indices and each
    indices stores 1 byte (or 8 bits). So, if we wanted to
    obtain the 33rd bit, we would get it by checking
    VAR_SCENARIO_SUB3[0] & 0b00000010.


    bit: 0-127 bit to check
    return: value of the corresponding bit */
bool GetSubXBit(int bit);

/* GetRemainingDungeonMissionChecks
    Calculates how many missions checks remain of a certain
    type for a given dungeon.

    dungeon: dungeon to check remaining mission count for
    bool: whether to check outlaw missions instead of normal missions
    return: number of uncompleted mission checks */
int GetRemainingDungeonMissionChecks(enum dungeon_id dungeon, bool outlaw);

/* IsDarkraiGoal
    return: Is Goal Darkrai? */
bool IsDarkraiGoal();

/* IsDarkraiGoal
    return: If long locations like Zero Isle North and 9999999 money
            are enabled. */
bool AreLongLocationsOn();

/* GetDungeonModeWithSevenTreasureCheck
    If the dungeon is not a Seven Treasure dungeon, just
    returns the DMODE. However, if it a Seven Treasure dungeon
    also check the corresponding SUBX bit for sending the
    Seven Treasure mission acquired check. This check has been
    added to ensure that the Seven Treasure missions can still
    be obtained in the Cafe even when the dungeon has already
    been unlocked.

    dungeon_id: the dungeon to get the DMODE of
    return: DMODE of the dungeon */
enum dungeon_mode GetDungeonModeWithSevenTreasureCheck(enum dungeon_id dungeon_id);

/* IsLookaLikeDungeonPlaceholder
    To get look-alike dungeons to be near their corresponding
    dungeon in the list at the crossroads requires renaming certain
    dungeons that can't normally show up in the list. ie:
    Beach Cave Pit AND setting their DMODE to
    DMODE_OPEN_AND_REQUEST. Because of this, we need to add
    some checks in places to not do stuff like generate missions
    for DUNGEON_BEACH_CAVE_PIT and not put the dungeon in the
    curated mission menu list.

    dungeon_id: the dungeon to check
    return: is a lookalike dungeon? */
bool IsLookaLikeDungeonPlaceholder(enum dungeon_id dungeon_id);

/* GetDebugRomFlag
    Checks if the debug cheat is active by checking if the
    instruction has been changed.

    return: debug cheat on? */
bool GetDebugRomFlag();

/* DrawVariableInfoInWindow
    Draws the value for a variable into the window passed and
    accounts for ALL types.
    
    idx: the window to draw in
    x: x location to start drawing
    y: y location to start drawing
    max_width: max_width to use after x starting before new line
    buffer: a buffer to temporarily write strings into
    buffer_len: length of buffer
    var: the variable to draw the info for */
void DrawVariableInfoInWindow(
    int idx,
    int x,
    int y,
    int max_width,
    char* buffer,
    int buffer_len,
    enum script_var_id var
);