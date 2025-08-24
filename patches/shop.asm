.nds
.include "symbols.asm"

.open "arm9.bin", arm9_start
    .org GenerateKecleonItems1Hook
    .area 0x4
        b  GenerateShop1Tweak
    .endarea

    .org GenerateKecleonItems2Hook
    .area 0x4
        b  GenerateShop2Tweak
    .endarea
.close

.open "overlay22.bin", overlay22_start
    .org AddShopItemToInventoryHook1
    .area 0x4
        bl AddItemToBagWithMultiworldCheck
    .endarea

    .org AddShopItemToInventoryHook2
    .area 0x4
        bl AddItemToBagWithMultiworldCheck
    .endarea

    .org AddShopItemToInventoryHook3
    .area 0x4
        bl AddItemToBagWithMultiworldCheck
    .endarea

    .org AddShopItemToInventoryHook4
    .area 0x4
        bl AddItemToBagWithMultiworldCheck
    .endarea
.close