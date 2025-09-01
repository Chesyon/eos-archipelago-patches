.nds
.include "symbols.asm"

.open "arm9.bin", arm9_start
    .org GetCurrentBagCapacityHook
    .area 0x4
        bl GetCurrentBagCapacityTweak
    .endarea

    .org NoLowercaseQTagBranch
    .area 0x4
        b NoLowercaseQTagFound
    .endarea

    ; An Old Hook For Deathlink. We need to hook even earlier to avoid some
    ; bugs. Return this to its original instruction.
    .org DeathLinkSendHookOld
    .area 0x4
        bl GetDungeonResultMsg
    .endarea

    ; Set allow_default to true when initializing portraits
    .org AllowDefaultPortraitInInit
    .area 0x8
        mov r2,#1
        b AlwaysAllowPortraitDefault ; this is intentionally not bl
    .endarea

    ; Make $VERSION validity check always return true
    .org BlockVersionCorruption
    .area 0x20
        mov r0,#0x1
        nop
        nop
        nop
        nop
        nop
        nop
        nop
    .endarea

    .org ExplorerMazeUnlock
    .area 0x4
        nop
    .endarea

    .org HardcodedMissionUnlock1
    .area 0x4
        nop
    .endarea

    .org HardcodedMissionUnlock2
    .area 0x4
        nop
    .endarea

    .org HardcodedMissionUnlock3
    .area 0x4
        nop
    .endarea

	; .org SwapShopScenarioBalanceHook1
	; .org SwapShopScenarioBalanceHook2
	; To be used for Croagunk Shenanigans later. 
.close

.open "overlay1.bin", overlay1_start
    .org ClearMarkHook
    .area 0x4
        bl ClearMarkCheck
    .endarea
.close

.open "overlay11.bin", overlay11_start
    .org NameAutofillHook
    .area 0x4
        bl AutofillName
    .endarea
.close

.open "overlay19.bin", overlay19_start
    .org CancelSpindunlock
    .area 0x4
        nop
    .endarea
.close

.open "overlay28.bin", overlay28_start
    .org CreditsDisplayText
    .area 0x4
        bl PreprocessAndAnalyzeCreditsText
    .endarea
.close

.open "overlay29.bin", overlay29_start
    .org RecruitInstrumentHook
    .area 0x4
        beq ExtraInstrumentsCheck
    .endarea
        
    .org SpawnRateInstrumentHook
    .area 0x4
        bl ReduceSpawnInstrumentCheck
    .endarea
    
    .org DungeonTurnHook
    .area 0x4
        beq DungeonTurnEndChecksTrampoline
    .endarea
    
    .org DeathLinkDungeonInitHook
    .area 0x4
        bl ArchipelagoInitDungeon
    .endarea

    .org DeathLinkSendHook
    .area 0x4
        b DeathLinkSenderTrampoline
    .endarea

    .org WindCounterCheckHook
    .area 0x4
        bl WindCounterCheck
    .endarea

    .org GenerateFloorHook1
    .area 0x4
        b GenerateFloorCustomLayouts
    .endarea

    .org GenerateFloorHook2
    .area 0x4
        bl ApMazeTrapCheck
    .endarea
.close

.open "overlay31.bin", overlay31_start
    .org DungeonXMenuNameAddr
    .area 0x4
        orr r0,r0,#0x10000
    .endarea
.close
