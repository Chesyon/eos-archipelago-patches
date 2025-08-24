.nds
.include "symbols.asm"

.open "arm9.bin", arm9_start
    .ifdef TryCutsceneSkipScan
        .org CreateDefaultScriptEngineBox
        .area 0x4
            bl CreateScriptEngineDialogueBox
        .endarea
        
        .org ShowStringInDialogueBoxCallsite1
        .area 0x4
            bl ShowScriptEngineStringInDialogueBox
        .endarea

        .org IsValidPortraitCallsite
        .area 0x4
            bl ShouldShowScriptEnginePortrait
        .endarea
    .endif
.close

.open "overlay11.bin", overlay11_start
    .ifdef TryCutsceneSkipScan
        .org GroundSupervisionExecuteRequestCancelCallsite
        .area 0x4
            b FinalCutsceneSkipCheck
        .endarea

        .org GetCoroutineInfoCallsite
        .area 0x4
            bl GetRecoverCoroutineInfo
        .endarea

        .org InitScriptRoutineFromCoroutineInfoCallsite
        .area 0x4
            bl CustomInitScriptRoutineFromCoroutineInfo
        .endarea

        .org DebugPrintCallsite
        .area 0x8
            bl DebugPrintGameCancel
            nop
        .endarea

        .org GetSceneNameCallsite
        .area 0x4
            bl CustomGetSceneName
        .endarea

        .org SelectPressBranchEqual
        .area 0x4
            beq CheckSelectPressTrampoline
        .endarea

        .org OpcodeMainEnterDungeonBranchEqual
        .area 0x4
            beq HijackRunNextOpcodeMainEnterDungeon
        .endarea
        
        .org OpcodeMainEnterGroundBranch
        .area 0x4
            b HijackRunNextOpcodeMainEnterGround
        .endarea
        
        .org OpcodeEndBranchReturn ; end
        .area 0x4
            b HijackRunNextOpcodeControlStatement
        .endarea
        
        .org OpcodeMovementSpeed ; move
        .area 0x4
            bl GetMovementSpeedParam
        .endarea
        
        .org OpcodeSlidingSpeed ; slide
        .area 0x4
            bl GetMovementSpeedParam
        .endarea
        
        .org TurnOpcodeSwitchStatementSetup ; turn
        .area 0x4
            b TrySpeedUpTurnSpeedParamTrampoline
        .endarea
        
        .org OpcodeHeightSpeed ; height
        .area 0x4
            bl GetMovementSpeedParam
        .endarea
        
        .org OpcodeWaitSpeed ; wait
        .area 0x4
            bl GetWaitTime
        .endarea
        
        .org OpcodeBgmWaitSpeed ; bgm1
        .area 0x4
            bl GetWaitTime
        .endarea
        
        .org OpcodeBgm2WaitSpeed ; bgm2
        .area 0x4
            bl GetWaitTime
        .endarea
        
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
        .area 0x4
            bl ShowScriptEngineStringInDialogueBox
        .endarea
        
        .org ShowStringInDialogueBoxCallsite3
        .area 0x4
            bl ShowScriptEngineStringInDialogueBox
        .endarea
    .endif
.close