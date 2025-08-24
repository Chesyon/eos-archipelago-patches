.nds
.include "symbols.asm"

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