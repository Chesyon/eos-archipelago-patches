.nds
.include "symbols.asm"

.open "arm9.bin", arm9_start
    .org ClearAdventureLogStructCallsite
    .area 0x4
        bl CustomClearAdventureLogStruct
    .endarea

    .org WriteSaveFileCallsite1
    .area 0x4
        bl SaveMoreDataWrapper1
    .endarea

    .org WriteSaveFileCallsite2
    .area 0x4
        bl SaveMoreDataWrapper2
    .endarea

    .org WriteSaveFileCallsite3
    .area 0x4
        bl SaveMoreDataWrapper3
    .endarea

    .org WriteSaveFileCallsite4
    .area 0x4
        bl SaveMoreDataWrapper4
    .endarea
    
    .org LoadSaveFileCallsite1
    .area 0x4
        bl LoadMoreDataWrapper1
    .endarea
    
    .org LoadSaveFileCallsite2
    .area 0x4
        bl LoadMoreDataWrapper2
    .endarea
.close