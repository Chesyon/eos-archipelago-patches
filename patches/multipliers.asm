.nds
.include "symbols.asm"

.open "arm9.bin", arm9_start
    .org IqMultiplierGroundHook
    .area 0x4
        bl IqMultiplierGround
    .endarea

    .org MiscIqMultiplierGroundHook
    .area 0x4
        bl IqMultiplierGroundMisc
    .endarea

    ; EXP multiplier
    .org GetExpReturn
    .area 0x4
        bl ExpMultiplier
    .endarea
.close

.open "overlay29.bin", overlay29_start
    .org IqMultiplierDungeonHook
    .area 0x4
        bl IqMultiplierDungeon
    .endarea

    .org IqMultiplierDungeonGummiHook
    .area 0x4
        bl IqMultiplierDungeonGummi
    .endarea
.close
