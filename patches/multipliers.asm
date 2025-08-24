.nds
.include "symbols.asm"

.open "arm9.bin", arm9_start
    .org IqScalingGroundHook
    .area 0x4
        bl IqScalingGround
    .endarea

    .org MiscIqScalingGroundHook
    .area 0x4
        bl IqScalingGroundMisc
    .endarea

    ; EXP scaling
    .org GetExpReturn
    .area 0x4
        bl ExpScaling
    .endarea
.close

.open "overlay29.bin", overlay29_start
    .org IqScalingDungeonHook
    .area 0x4
        bl IqScalingDungeon
    .endarea

    .org IqScalingDungeonGummiHook
    .area 0x4
        bl IqScalingDungeonGummi
    .endarea
.close
