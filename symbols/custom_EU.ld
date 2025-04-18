/* ! For Arm9 */
GetCurrentBagCapacityHook = 0x200EE54;
GenerateKecleonItems1Hook = 0x2010B20;
GenerateKecleonItems2Hook = 0x2010E30;
IqScalingGroundHook = 0x20119FC;
NoLowercaseQTagBranch = 0x2022808;
LowercaseTagCodeError = 0x20234C4;
AfterLowercaseTagIsFound = 0x20234EC;
WriteSaveFileCallsite1 = 0x02049470;
WriteSaveFileCallsite2 = 0x02049498;
WriteSaveFileCallsite3 = 0x020494B0;
WriteSaveFileCallsite4 = 0x020494D8;
LoadSaveFileCallsite1 = 0x020496F4;
LoadSaveFileCallsite2 = 0x02049714;
ClearAdventureLogStructCallsite = 0x02048774;
BlockVersionCorruption = 0x204C794;
AllowDefaultPortaitInInit = 0x204DB04;
MiscIqScalingGroundHook = 0x20553D8;
StartOfMissionTypeRoll = 0x20628F8; /* this is the second instruction in the function actually */
MissionTypeRollHook = 0x2062948;
MissionTryOpenDungeon = 0x20682BC;
DeathLinkSendHookOld = 0x200C778;
MissionMultipurposeCheckAreaStart = 0x2062F74;
MissionMultipurposeCheckAreaEnd = 0x2062F94;
TAILORED_MISSION_DUNGEON = 0x2062F94;
TailoredMissionOverlapFloorsHook = 0x2063064;
GenerateMissionMinFloorRollHook = 0x206301C;
MessageSetWaitMode = 0x2035154;
CreateDefaultScriptEngineBox = 0x2035420;
ShowStringInDialogueBoxCallsite1 = 0x2035560;
IsValidPortraitCallsite = 0x2035814;
GuestMonsterToGroundMonsterCallsite = 0x0204FC84;
GetExpReturn = 0x2052DE0;
OptionsMenuItemListPtr = 0x209B684;

/* ! For Overlay1 */
ClearMarkHook = 0x2333D68;

/* ! For Overlay11 */
NameAutofillHook = 0x22E6B94;

/* Overlay 11: CRASS */
GroundMainLoopStuff = 0x22E98D4;
CancelRecoverStart = 0x22E981C;
GroundSupervisionExecuteRequestCancel = 0x22F7AE0;
GroundSupervisionExecuteRequestCancelCallsite = 0x22E9844;
RunNextOpcodeMainEnterDungeon = 0x22E2520;
RunNextOpcodeMainEnterGround = 0x22DED98;
RunNextOpcodeReturn = 0x22E2DB8;
ScriptEngineReturnTwo = 0x22E2DAC;
ScriptEngineReturnFive = 0x22E0264;
InitScriptRoutineFromCoroutineInfo = 0x22DD62C;
InitScriptRoutineFromCoroutineInfoCallsite = 0x22F8360;
GetSceneName = 0x22E4F40;
GetSceneNameCallsite = 0x22DF534;
GetCoroutineInfoCallsite = 0x22F8330;
DebugPrintCallsite = 0x22E9B08;
SelectPressBranchEqual = 0x22E980C;
OpcodeMainEnterDungeonBranchEqual = 0x22E23B8;
OpcodeMainEnterGroundBranch = 0x22DE958;
OpcodeEndBranchReturn = 0x22E0E08;
OpcodeMovementSpeed = 0x22E1970;
OpcodeSlidingSpeed = 0x22E19AC;
OpcodeHeightSpeed = 0x22E1E48;
OpcodeWaitSpeed = 0x22E0CBC;
OpcodeBgmWaitSpeed = 0x22E2A74;
OpcodeBgm2WaitSpeed = 0x22E2B4C;
OpcodeSetWaitModeStuff = 0x22E0234;
TurnOpcodeSwitchStatementSetup = 0x22E1228;
ShowStringInDialogueBoxCallsite2 = 0x230129C;
ShowStringInDialogueBoxCallsite3 = 0x2301624;

/* Overlay 11: Ap Tracker */
ApTrackerSetupMenuHook = 0x2301A7C;
ApTrackerTopScreenHook = 0x230DA34;
ApTrackerTopScreenUnhook = 0x230DA90;
ApTrackerTopScreenHook2 = 0x230D9BC;
ApTrackerTopScreenUnhook2 = 0x230DA18;
GroundMenuItemListPtr = 0x2301878;

/* ! For Overlay13 */
SameTypeHook = 0x238C48C;
SameTypeUnhook = 0x238C4B0;
QuizInterceptStateHook = 0x238B0E8;
QuizInterceptStateUnhook = 0x238C8B4;
QuizForcedPlayerHook = 0x238B250;
QuizForcedPlayerUnhook = 0x238B258;
QuizForcedPartnerHook = 0x238C44C;
QuizForcedPartnerUnhook = 0x238C4B0;
QuizForcedPartnerRollHook = 0x238C4B8;
QuizForcedPartnerRollUnhook = 0x238C450;
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
DeathLinkSendHook = 0x22ED8E0;
WindCounterCheckHook = 0x22ED53C;
/* Mode 5 represents the controls for keyboards in dungeon mode. Move it to 6. */
TopScreenDungeonModeMode5Hook = 0x22E911C;
TopScreenDungeonModeTrackerHook = 0x22E92AC;
TopScreenDungeonModeTrackerUnhook = 0x22E938C;
SetTopScreenTypeGroundHook = 0x22E9100;
SetTopScreenTypeGroundUnhook = 0x22E9110;


/* ! New Archipelago Values For Overlay36 */
archipelagoAreaStart = 0x23DE000;
apSlotName = 0x23DE000;
apSeed = 0x23DE010;
apSettings = 0x23DE018;
missionMaxes = 0x23DE01A;
macguffinMaxes = 0x23DE01E;
hintableItems = 0x23DE022;
cafeMaxes = 0x23DE1C6; /* See my comment on SP 105. */
startWithMainGameUnlocked = 0x23DE1C8;
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
DungeonModeSetupAndShowKeyboard = 0x234FC14;
SomeDeathMsgCheckFun = 0x200C63C;
GetStringWidth = 0x202688C;
SetupTopGroundMenuNext = 0x23016F8;
SetupMenuNext = 0x2034BD8;
SetupGroundTopScreenFunctions = 0x230DC0C;
SetupGroundTopScreenFunctions2 = 0x230DB18;
GetTopScreenOptionType = 0x204B1D8; /* Gets the number of the option selected for the top screen. */
CreateMode5TopScreenDungeon = 0x22E9000;
UpdateMode5TopScreenDungeon = 0x22E904C;
UnkMode5TopScreenDungeon = 0x22E9054;
CloseMode5TopScreenDungeon = 0x22E9058;
TopScreenModeSetDungeonMode = 0x22E9658;

/* Ap Tracker Top Screen Stuff */
UnkTopScreenFun1 = 0x22EC7EC;
UnkTopScreenFun2 = 0x22EB938;
UnkTopScreenFun3 = 0x22F50D4;
UnkTopScreenFun4 = 0x22EA5F0;
UnkTopScreenFun5 = 0x22EFE7C;
UnkTopScreenFun6 = 0x22EA5D8;
UnkTopScreenFun7 = 0x22EA650;
UnkTopScreenFun8 = 0x22F04B0;
LoadTopScreenBGPart1 = 0x22EC558; /* Might be actually for putting it on the top screen. */
LoadTopScreenBGPart2 = 0x22EE7A4; /* Might be more generic background load. */
FreeTopScreenBG = 0x22EC7EC; /* Might be more generic background free. */
SomethingTopScreenBG = 0x22F004C;
InitBackgroundRegionDungeon = 0x22C2BF8;
LoadBackgroundDungeon = 0x22C27D8;
SetupBackgroundDungeon = 0x22C2B30;

MESSAGE_SET_WAIT_MODE_PARAMS = 0x23259CC;
SCRIPT_PORTRAIT_PARAMS = 0x23259E4;

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
