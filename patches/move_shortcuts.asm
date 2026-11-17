.nds
.include "symbols.asm"

.open "overlay29.bin", overlay29_start
    ; Move Shortcuts Related Hooks
    .org MessageLogShortcutButtonOneCheck
        bl CustomMessageLogShortcutButtonOneCheck
    .org MessageLogShortcutButtonTwoCheck
        bl CustomMessageLogShortcutButtonTwoCheck
    ; Before SetLeaderAction handles any input (touchscreen & buttons), intercept
    ; with our Move Shortcuts check. This is to avoid any 'quirky' behaviors that
    ; may occur with the touchscreen checks.
    .org MoveShortcutsMainCheckHook
    .area 0x8 ; 2 instructions
        str r8,[sp,#0x5C]  ; original instructions optimized
        b   TryHandleMoveShortcutsTrampoline
    .endarea
.close
