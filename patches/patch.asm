.nds
.include "symbols.asm"

.open "arm9.bin", arm9_start

    ; AP Shop Items
    .org GenerateKecleonItems1Hook
        b  GenerateShop1Tweak
    .org GenerateKecleonItems2Hook
        b  GenerateShop2Tweak

    .org IqScalingGroundHook
        bl IqScalingGround

    .org MiscIqScalingGroundHook
        bl IqScalingGroundMisc

    .org NoLowercaseQTagBranch
        b NoLowercaseQTagFound

    ; An Old Hook For Deathlink. We need to hook even earlier to avoid some
    ; bugs. Return this to its original instruction.
    .org DeathLinkSendHookOld
        bl GetDungeonResultMsg
    
    ; Mission Related Hooks
    .org MissionTypeRollHook
        bl ExploreNewDungeonCheck
    .org MissionTryOpenDungeon
        add r0,r0,#0x0 ; nop, the ds does not have a nop operator do this in case
    .org CrystalCaveCrossingMissionCheckAddr
        b CrystalCaveCrossingMissionSkipCheckAddr
    .org MissionMultipurposeCheckAreaStart
    .area MissionMultipurposeCheckAreaEnd - MissionMultipurposeCheckAreaStart ; 8 instruction size
        ldr r1,[tailor] ; This overwrites the crystal cave/crossing checks with
        cmp r1,#0x0     ; a check for tailored missions.
        beq MissionMultipurposeCheckAreaEnd
        cmp r5,r1
        movne r0,#0x0
        moveq r0,#0x1
        ldmia sp!,{r3,r4,r5,pc}
    .endarea
    .org MissionMultipurposeCheckAreaEnd
    .area 0x4
        .pool
            tailor:
             .word 0x0
    .endarea
    .org TailoredMissionOverlapFloorsHook
        blne TailoredMissionBoardCheck
    .org GenerateMissionMinFloorRollHook
        bl EarlyMissionFloorsCheck

    ; A hook to determine how Guest Pokemon stats will be generated.
    .org GuestMonsterToGroundMonsterCallsite
        bl CustomGuestMonsterToGroundMonster

    ; Save-related stuff
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

    ; Cutscene skip shenanigans
    .ifdef TryCutsceneSkipScan
    .org CreateDefaultScriptEngineBox
        bl CreateScriptEngineDialogueBox
    .org ShowStringInDialogueBoxCallsite1
        bl ShowScriptEngineStringInDialogueBox
    .org IsValidPortraitCallsite
        bl ShouldShowScriptEnginePortrait
    .endif
.close

.open "overlay11.bin", overlay11_start
    .org NameAutofillHook
        bl AutofillName
    
    ; More cutscene skip shenanigans
    .ifdef TryCutsceneSkipScan
    .org GroundSupervisionExecuteRequestCancelCallsite
        b FinalCutsceneSkipCheck
    .org GetCoroutineInfoCallsite
        bl GetRecoverCoroutineInfo
    .org InitScriptRoutineFromCoroutineInfoCallsite
        bl CustomInitScriptRoutineFromCoroutineInfo
    .org DebugPrintCallsite
    .area 0x8
        bl DebugPrintGameCancel
        nop
    .endarea
    .org GetSceneNameCallsite
        bl CustomGetSceneName
    .org SelectPressBranchEqual
        beq CheckSelectPressTrampoline
    .org OpcodeMainEnterDungeonBranchEqual
        beq HijackRunNextOpcodeMainEnterDungeon
    .org OpcodeMainEnterGroundBranch
        b HijackRunNextOpcodeMainEnterGround
    .org OpcodeEndBranchReturn ; end
        b HijackRunNextOpcodeControlStatement
    .org OpcodeMovementSpeed ; move
        bl GetMovementSpeedParam
    .org OpcodeSlidingSpeed ; slide
        bl GetMovementSpeedParam
    .org TurnOpcodeSwitchStatementSetup ; turn
        b TrySpeedUpTurnSpeedParamTrampoline
    .org OpcodeHeightSpeed ; height
        bl GetMovementSpeedParam
    .org OpcodeWaitSpeed ; wait
        bl GetWaitTime
    .org OpcodeBgmWaitSpeed ; bgm1
        bl GetWaitTime
    .org OpcodeBgm2WaitSpeed ; bgm2
        bl GetWaitTime
    .org OpcodeSetWaitModeStuff ; setwaitmode
    .area 0x2C
        ldrh r0,[r6,#0x2]
        bl ScriptParamToInt
        mov r11,r0
        ldrh r0,[r6,#0x0]
        bl ScriptParamToInt
        mov r1,r11
        bl TryMessageSetWaitMode
        nop
        nop
        nop
        nop
    .endarea
    .org ShowStringInDialogueBoxCallsite2
        bl ShowScriptEngineStringInDialogueBox
    .org ShowStringInDialogueBoxCallsite3
        bl ShowScriptEngineStringInDialogueBox
    .endif
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
    .org QuizForcedPartnerRollHook
        bl ForcedPartnerRollCheck
    .org QuizOverrideHook
        bl OverrideHeroCheck
    .org QuizHeroTweakHook
        bl HeroTweakCheck
    .org QuizChooseTweakHook
        bl ChooseTweakCheck
    .org QuizTypeHeroTweakHook
        bl TypeHeroTweak
.close

.open "overlay19.bin", overlay19_start
    .org CancelSpindunlock
        nop
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

    .org IqScalingDungeonGummiHook
        bl IqScalingDungeonGummi
        
    .org RecruitInstrumentHook
        beq ExtraInstrumentsCheck
        
    .org SpawnRateInstrumentHook
        bl ReduceSpawnInstrumentCheck
    
    .org DungeonTurnHook
        beq DungeonTurnEndChecksTrampoline
    
    .org DeathLinkDungeonInitHook
        bl DeathLinkInitDungeon
    .org DeathLinkSendHook
        b DeathLinkSenderTrampoline
.close
