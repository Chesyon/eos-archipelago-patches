.align
ExploreNewDungeonCheck:
    ldrb r0,[r4,#0x21] // check mission subtype
    cmp r0,#0x3 // is subtype 3? (EXPLORE_NEW_DUNGEON in mission_subtype_explore)
    ldrb r0,[r4,#0x20] // check mission type. also original instruction!
    bxne lr // if the subtype isn't 3, it can't be a new dungeon so we're good to return!
    cmp r0,#0x3 // is type MISSION_EXPLORE_WITH_CLIENT?
    bxne lr // if not, it can't be a new dungeon so we're good to return!
    b StartOfMissionTypeRoll // if we've gotten this far, this is a new dungeon and we should reroll the mission type
