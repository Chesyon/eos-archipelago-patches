.nds
.include "symbols.asm"

.open "overlay13.bin", overlay13_start
    .org SameTypeHook
    .area 0x4
        blne TypesanityCheck
    .endarea

    .org QuizInterceptStateHook
    .area 0x4
        b QuizCustomStateTrampoline
    .endarea

    .org QuizForcedPlayerHook
    .area 0x4
        b ForcedPlayerCheck
    .endarea

    .org QuizForcedPartnerHook
    .area 0x4
        bl ForcedPartnerCheck
    .endarea

    .org QuizForcedPartnerRollHook
    .area 0x4
        bl ForcedPartnerRollCheck
    .endarea

    .org QuizOverrideHook
    .area 0x4
        bl OverrideHeroCheck
    .endarea

    .org QuizHeroTweakHook
    .area 0x4
        bl HeroTweakCheck
    .endarea

    .org QuizChooseTweakHook
    .area 0x4
        bl ChooseTweakCheck
    .endarea
    
    .org QuizTypeHeroTweakHook
    .area 0x4
        bl TypeHeroTweak
    .endarea
.close