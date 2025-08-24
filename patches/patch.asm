.nds
.include "symbols.asm"

.open "arm9.bin", arm9_start
    .org GetCurrentBagCapacityHook
        bl GetCurrentBagCapacityTweak

    .org NoLowercaseQTagBranch
        b NoLowercaseQTagFound

    ; An Old Hook For Deathlink. We need to hook even earlier to avoid some
    ; bugs. Return this to its original instruction.
    .org DeathLinkSendHookOld
        bl GetDungeonResultMsg

    ; Set allow_default to true when initializing portraits
    .org AllowDefaultPortaitInInit
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

	; .org SwapShopScenarioBalanceHook1
	; .org SwapShopScenarioBalanceHook2
	; To be used for Croagunk Shenanigans later. 
.close

.open "overlay1.bin", overlay1_start
    .org ClearMarkHook
        bl ClearMarkCheck
.close

.open "overlay11.bin", overlay11_start
    .org NameAutofillHook
        bl AutofillName
.close

.open "overlay19.bin", overlay19_start
    .org CancelSpindunlock
        nop
.close

.open "overlay28.bin", overlay28_start
    .org CreditsDisplayText
        bl PreprocessAndAnalyzeCreditsText
.close

.open "overlay29.bin", overlay29_start
    .org RecruitInstrumentHook
        beq ExtraInstrumentsCheck
        
    .org SpawnRateInstrumentHook
        bl ReduceSpawnInstrumentCheck
    
    .org DungeonTurnHook
        beq DungeonTurnEndChecksTrampoline
    
    .org DeathLinkDungeonInitHook
        bl ArchipelagoInitDungeon
    .org DeathLinkSendHook
        b DeathLinkSenderTrampoline

    .org WindCounterCheckHook
        bl WindCounterCheck

    .org GeneateFloorHook1
        b GenerateFloorCustomLayouts
    .org GeneateFloorHook2
        bl ApMazeTrapCheck
.close

.open "overlay31.bin", overlay31_start
    .org DungeonXMenuNameAddr
    .area 0x4
        orr r0,r0,#0x10000
    .endarea
.close
