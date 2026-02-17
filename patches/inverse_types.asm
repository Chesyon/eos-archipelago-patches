.nds
.include "symbols.asm"

.open "overlay29.bin", arm9_start
    .org IsTypeIneffectiveAgainstGhostFlip
    .area 0x4
        beq IsTypeIneffectiveAgainstGhostFlip
    .endarea

    .org GhostImmunityIsActiveHook
    .area 0x4
        bl GhostImmunityIsActiveFlip
    .endarea

    .org GetTypeMatchupHook1
    .area 0x4
        beq GetTypeMatchupFlip
    .endarea
    .org GetTypeMatchupHook2
    .area 0x4
        b   GetTypeMatchupFlip
    .endarea
    .org GetTypeMatchupHook3
    .area 0x4
        bne GetTypeMatchupFlip
    .endarea
    .org GetTypeMatchupHook4
    .area 0x4
        b   GetTypeMatchupFlip
    .endarea
.close