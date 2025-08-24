.nds
.include "symbols.asm"

.open "arm9.bin", arm9_start
    .org IqScalingGroundHook
        bl IqScalingGround

    .org MiscIqScalingGroundHook
        bl IqScalingGroundMisc

    ; EXP scaling
    .org GetExpReturn
    .area 0x4
        bl ExpScaling
    .endarea
.close

.open "overlay29.bin", overlay29_start
    .org IqScalingDungeonHook
        bl IqScalingDungeon

    .org IqScalingDungeonGummiHook
        bl IqScalingDungeonGummi
.close
