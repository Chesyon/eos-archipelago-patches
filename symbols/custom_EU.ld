/* ! For Arm9 */
GenerateKecleonItems1Hook = 0x2010B20;
GenerateKecleonItems2Hook = 0x2010E30;
IqScalingGroundHook = 0x20119FC;
WriteSaveFileCallsite1 = 0x02049470;
WriteSaveFileCallsite2 = 0x02049498;
WriteSaveFileCallsite3 = 0x020494B0;
WriteSaveFileCallsite4 = 0x020494D8;
LoadSaveFileCallsite1 = 0x020496F4;
LoadSaveFileCallsite2 = 0x02049714;
ClearAdventureLogStructCallsite = 0x02048774;
CallGetDungeonResultMsg = 0x200C778;

/* ! For Overlay11 */
NameAutofillHook = 0x22E6B94;

/* ! For Overlay13 */
SameTypeHook = 0x238C48C;
SameTypeUnhook = 0x238C4B0;
QuizInterceptStateHook = 0x238B0E8;
QuizInterceptStateUnhook = 0x238C8B4;
QuizForcedPlayerHook = 0x238B250;
QuizForcedPlayerUnhook = 0x238B258;
QuizForcedPartnerHook = 0x238C44C;
QuizForcedPartnerUnhook = 0x238C4B0;
QuizOverrideHook = 0x238B908;
QuizHeroTweakHook = 0x238BED4;
QuizChooseTweakHook = 0x238B7BC;
QuizTypeHeroTweakHook = 0x238C3C0;
QuizTypeHeroTweakUnhook = 0x238C3E0;

/* ! For Overlay19 */
CancelSpindunlock = 0x238DE18;

/* ! For Overlay22 */
AddShopItemToInventoryHook1 = 0x238B0D8;
AddShopItemToInventoryHook2 = 0x238B138;
AddShopItemToInventoryHook3 = 0x238B724;
AddShopItemToInventoryHook4 = 0x238B784;

/* ! For Overlay29 */
LevelScalingHook = 0x22E80B0; /* In LoadMappaFileAttributes */
IqScalingDungeonHook = 0x22FAB68;
RecruitInstrumentHook = 0x230EA28;
RecruitInstrumentUnhook = 0x230EA48;
SpawnRateInstrumentHook = 0x230F174;
IqScalingDungeonGummiHook = 0x231DC08; /* In ApplyGummiBoostsDungeonMode */
DungeonTurnHook = 0x22E0468; /* In RunDungeon near RunFractionalTurn call. */
DungeonTurnUnhook = 0x22E0454;
DeathLinkDungeonInitHook = 0x22DF8EC; /* Overwrites A DebugPrint0 in RunDungeon.*/

/* ! New Archipelago Values For Overlay36 */
archipelagoAreaStart = 0x23DE000;
apSlotName = 0x23DE000;
apSeed = 0x23DE010;
apSettings = 0x23DE018;
missionMaxes = 0x23DE01A;
macguffinMaxes = 0x23DE01E;
deathLinkTracker = 0x23B0186; /* See CustomSaveArea in extern.h */
CUSTOM_SAVE_AREA = 0x23B0000;

/* Undocumented Functions 
   Note: You may to remove some of these definitions if they become defined
   in pmdsky-debug.*/
AddItemToKecleon1 = 0x2010B70;
AddItemToKecleon2 = 0x2010E74;
PARTNER_SELECT_MENU_OPTION_TRACKER = 0x238D9E4;
PARTNER_SELECT_MENU_OPTION_TIMER = 0x238D9E8;
PARTNER_SELECT_PORTRAIT_OFFSETS = 0x238CB54;
PlayEffectAnimationEntityWrapper = 0x22E4C98;
DetermineTileAppearence = 0x2337264;

/* ! Base CoT file overlay11 */
OpcodeCheck = 0x022DE714;
GetParameterCount = 0x022DE6CC;
ScriptEngineReturnTwo = 0x022E2DAC;
ScriptMenuRequestDefaultCase = 0x022E6A30;
ScriptMenuRequestFinalize = 0x022E71EC;
ScriptMenuUpdateDefaultCase = 0x022E7238;
ScriptMenuUpdateFinalize = 0x022E7798;
IS_BASE_GAME_MENU_FINISHED = 0x023257DC;

/* ! Base CoT file arm9 */
ShowKeyboardTypeCase3 = 0x02036C10;
ShowKeyboardTypeDefaultCase = 0x02036D3C;
ShowKeyboardReturn = 0x02036FA8;
PreprocessStringFromIdCallsite = 0x020396FC;
