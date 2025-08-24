.nds
.include "symbols.asm"

.open "arm9.bin", arm9_start
    ; Mission Related Hooks
    .org MissionTypeRollHook
        bl ExploreNewDungeonCheck
    .org MissionTryOpenDungeon
        add r0,r0,#0x0 ; nop, the ds does not have a nop operator do this in case
    .org MissionMultipurposeCheckAreaStart
    .area TAILORED_MISSION_DUNGEON - MissionMultipurposeCheckAreaStart ; 7 instruction size
        ldr r1,[tailor] ; This overwrites the crystal cave/crossing checks with
        cmp r1,#0x0     ; a check for tailored missions.
        beq MissionMultipurposeCheckAreaEnd
        cmp r5,r1
        movne r0,#0x0
        moveq r0,#0x1
        ldmia sp!,{r3,r4,r5,pc}
    .endarea
    .org TAILORED_MISSION_DUNGEON
    .area 0x4
        .pool
            tailor:
            .word 0x0
    .endarea
    .org TailoredMissionOverlapFloorsHook
        blne TailoredMissionBoardCheck
    .org GenerateMissionMinFloorRollHook
        bl EarlyMissionFloorsCheck
    .org SevenTreasureCafeMissionCheck1
        bl GetDungeonModeWithSevenTreasureCheck
	.org SevenTreasureCafeMissionCheck2
		bl GetDungeonModeWithSevenTreasureCheck
.close