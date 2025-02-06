.nds
.include "symbols.asm"

.open "arm9.bin", arm9_start
    .org GenerateKecleonItems1Hook
        b  GenerateShop1Tweak
    
    .org GenerateKecleonItems2Hook
        b  GenerateShop2Tweak

    .org IqScalingGroundHook
        bl IqScalingGround
.close

.open "overlay11.bin", overlay11_start
    .org NameAutofillHook
        bl AutofillName
.close

.open "overlay13.bin", overlay13_start
    .org SameTypeHook
        blne TypesanityCheck
    .org QuizInterceptStateHook
        b QuizCustomStateTrampoline
    .org QuizForcedPlayerHook
        b ForcedPlayerCheck
    .org QuizForcedPartnerHook
        bl ForcedPartnerCheck
.close

.open "overlay22.bin", overlay22_start
    .org AddShopItemToInventoryHook1
        bl AddItemToBagWithMultiworldCheck

    .org AddShopItemToInventoryHook2
        bl AddItemToBagWithMultiworldCheck

    .org AddShopItemToInventoryHook3
        bl AddItemToBagWithMultiworldCheck

    .org AddShopItemToInventoryHook4
        bl AddItemToBagWithMultiworldCheck
.close

.open "overlay29.bin", overlay29_start
    .org LevelScalingHook
        bl BranchLevel

    .org IqScalingDungeonHook
        bl IqScalingDungeon
.close
