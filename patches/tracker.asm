.nds
.include "symbols.asm"

.open "arm9.bin", arm9_start
    ; AP Tracker
    .org OptionsMenuItemListPtr
    .area 0x4
        .pool
            .word newTopScreenOptionsList
    .endarea
.close

.open "overlay11.bin", overlay11_start
    .org ApTrackerSetupMenuHook
    .area 0x4
        b ApTrackerSetupMenuCheck
    .endarea

    .org ApTrackerTopScreenHook
    .area 0x4
        b ApTrackerTopScreenCheck
    .endarea

    .org ApTrackerTopScreenHook2
    .area 0x4
        b ApTrackerTopScreenCheck2
    .endarea

    .org GroundMenuItemListPtr
    .area 0x4
        .pool
            .word newTopGroundMenuList
    .endarea
.close

.open "overlay29.bin", overlay29_start
    .org TopScreenDungeonModeMode5Hook
    .area 0x4
        mov r0,#6
    .endarea

    .org TopScreenDungeonModeTrackerHook
    .area 0xC
        cmp r0,#0x4
        addls pc,pc,r0,lsl #0x2 ; untouched instruction
        b TopScreenDungeonModeTrackerCheck
    .endarea

    .org SetTopScreenTypeDungeonHook
    .area 0x4
        bne SetTopScreenTypeDungeonCheck
    .endarea

    .org DungeonTopScreenAdvanceUntilModeAddr
    .area 0xC
        .pool
        .halfword 0x2
        .halfword 0x1
        .halfword 0x0
        .halfword 0x3
        .halfword 0x4
        .halfword 0x5
    .endarea
.close
