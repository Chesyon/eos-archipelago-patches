/* ! For Arm9 */
GenerateKecleonItems1Hook = 0x2010B20;
GenerateKecleonItems2Hook = 0x2010E30;

/* ! For Overlay11 */
NameAutofillHook = 0x22E6B94;

/* ! For Overlay22 */
AddShopItemToInventoryHook1 = 0x238B0D8;
AddShopItemToInventoryHook2 = 0x238B138;
AddShopItemToInventoryHook3 = 0x238B724;
AddShopItemToInventoryHook4 = 0x238B784;

/* ! For Overlay29 */
LevelScalingHook = 0x22E80B0; /* In LoadMappaFileAttributes */

/* ! Archipelago Values For Overlay36 */
LevelScalingCheckAddr = 0x23DE0B8;
NameAutofillStringAddr = 0x23DE000;

/* Patch Constants */
MaxNumMultiworldItemsAtOnceKec1 = 7;
MaxNumMultiworldItemsAtOnceKec2 = 3;

/* Undocumented Functions 
   Note: You may to remove some of these definitions if they become defined
   in pmdsky-debug.*/
AddItemToKecleon1 = 0x2010B70;
AddItemToKecleon2 = 0x2010E74;

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
