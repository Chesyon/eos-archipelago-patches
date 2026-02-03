#pragma once

#include <pmdsky.h>
#include <cot.h>

// Some times the functions have returns. Sometimes they don't make them all
// void* because they have differing signatures.
typedef struct TopScreenMode {
    uint32_t thing;
    void* createFunction;
    void* closeFunction;
    void* function3;
    void* function4;
    void* function5;
    void* function6;
    void* function7;
    void* function8;
} TopScreenMode;

extern bool fadeOutAllActive;

extern struct vec2 PARTNER_SELECT_PORTRAIT_OFFSETS;
extern int MESSAGE_SET_WAIT_MODE_PARAMS[2];
extern void* UNIONALL_RAM_ADDRESS;

extern struct portrait_params SCRIPT_PORTRAIT_PARAMS;

extern uint32_t TAILORED_MISSION_DUNGEON;

void PlayEffectAnimationEntityWrapper(struct entity* entity, int effect_id);
void DetermineTileAppearence(int x, int y);
void DungeonModeSetupAndShowKeyboard(char* buf1, char* buf2, undefined thing);
int SomeDeathMsgCheckFun(union damage_source damage_source_or_result);
void MessageSetWaitMode(int speed1, int speed2);
void InitScriptRoutineFromCoroutineInfo(struct script_routine* routine, undefined4 param_2, struct coroutine_info* coroutine_info, int status);
void GetSceneName(char* dst, const char* src);
uint32_t GetStringWidth(char* str);

void UnkTopScreenFun1(void* num);
void UnkTopScreenFun2(uint32_t num);
void UnkTopScreenFun3(uint32_t num);
void UnkTopScreenFun4(uint32_t num);
void UnkTopScreenFun5(uint32_t num);
void UnkTopScreenFun6(uint32_t num);
void UnkTopScreenFun7(uint32_t num);
void UnkTopScreenFun8(void* num);
void LoadTopScreenBGPart1(void* thing, uint32_t other); // other is actually a pointer, but lazy
void LoadTopScreenBGPart2(void* thing, char* bg_string, uint32_t num);
void FreeTopScreenBG(void* thing);
void SomethingTopScreenBG(void* thing);
void SetupTopGroundMenuNext();
void SetupMenuNext(void* menu_thing);
void SetupGroundTopScreenFunctions(TopScreenMode* mode);
void SetupGroundTopScreenFunctions2(TopScreenMode* mode);
uint32_t GetTopScreenOptionType();
void InitBackgroundRegionDungeon(uint32_t* regionInfo);
void LoadBackgroundDungeon(void* thing, uint32_t* regionInfo, uint32_t num1, uint32_t num2);
void SetupBackgroundDungeon(void* thing);
void FreeTopScreenBGDungeon(void* thing);
void RemoveEmptyItemsInStorage();
void AnalyzeTextPreamble(undefined param_1, undefined param_2, undefined param_3, char* buffer);

static inline bool IsWithinRange(int x, int min, int max) { return min <= x && x <= max; }
