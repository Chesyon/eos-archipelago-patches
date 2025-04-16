// Ban Explore New Dungeons
.align
ExploreNewDungeonCheck:
    ldrb r0,[r4,#0x21]          // check mission subtype
    cmp  r0,#0x3                // is subtype 3? (EXPLORE_NEW_DUNGEON in mission_subtype_explore)
    ldrb r0,[r4,#0x20]          // check mission type. also original instruction!
    bxne lr                     // if the subtype isn't 3, it can't be a new dungeon so we're good to return!
    cmp  r0,#0x3                // is type MISSION_EXPLORE_WITH_CLIENT?
    bxne lr                     // if not, it can't be a new dungeon so we're good to return!
    b    StartOfMissionTypeRoll // if we've gotten this far, this is a new dungeon and we should reroll the mission type

// Allow Mission To Be Generated w/ Ealy Floors
EarlyMissionFloorsCheck:
    ldr   r3,=apSettings
    ldrh  r3,[r3,#0x0]
    tst   r3,#0b0000000000010000 // apSettings->earlyMissionFloors
    movne r0,#2
    str   r0,[sp,#0x8]           // original instruction
    bx    lr
    
// When missions are being tailored, allow the floors to be duplicates.
TailoredMissionBoardCheck:
    ldr   r1,=TAILORED_MISSION_DUNGEON
    ldr   r2,[r1,#0x0]
    cmp   r4,r2
    movne r0,#0x0 // original instruction
    bx    lr
    
