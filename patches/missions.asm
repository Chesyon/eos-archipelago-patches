.nds
.include "symbols.asm"

.open "arm9.bin", arm9_start
    ; Mission Related Hooks
    .org MissionTypeRollHook
    .area 0x4
        bl ExploreNewDungeonCheck
    .endarea

    .org MissionTryOpenDungeon
    .area 0x4
        nop
    .endarea
    
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
    .area 0x4
        blne TailoredMissionBoardCheck
    .endarea

    .org GenerateMissionMinFloorRollHook
    .area 0x4
        bl EarlyMissionFloorsCheck
    .endarea

    .org SevenTreasureCafeMissionCheck1
    .area 0x4
        bl GetDungeonModeWithSevenTreasureCheck
    .endarea

	.org SevenTreasureCafeMissionCheck2
    .area 0x4
		bl GetDungeonModeWithSevenTreasureCheck
    .endarea
.close