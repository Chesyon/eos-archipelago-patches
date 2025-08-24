.nds
.include "symbols.asm"

.open "arm9.bin", arm9_start
    ; A hook to determine how Guest Pokemon stats will be generated.
    .org GuestMonsterToGroundMonsterCallsite
    .area 0x4
        bl CustomGuestMonsterToGroundMonster
    .endarea
.close

.open "overlay29.bin", overlay29_start
    .org LevelScalingHook
    .area 0x4
        bl DoLevelScalingWrapper
    .endarea
.close