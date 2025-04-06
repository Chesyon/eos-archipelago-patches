#define CONFIRMATION_STR_ID 1732 // 0x6C4
#define WHO_LIKE_TO_BE_STR_ID 2613
/* Starter Typesanity
Disabled (default): Hero and partner cannot share a type (vanilla behavior).
Enabled: Hero and partner can share types. Note that the game is not balanced around this!
Starter Options:
Choose: Skip quiz, choose hero. Choose partner as normal.
Override (default): Do quiz, with option to override quiz result for hero. Choose partner as normal.
Vanilla: Do quiz, be stuck with the hero quiz gives you. Choose partner as normal.
Random: Be randomly assigned a hero and partner from the starter pool.*/

#include <pmdsky.h>
#include <cot.h>
#include "extern.h"

typedef struct QuizData {
    // 0x0: something related to waiting for boxes state
    int8_t unk_0x0;
    int8_t unk_0x1;
    // 0x2: during the quiz questions?
    int8_t dialougeBox1Id;
    // 0x3: simple or advanced
    int8_t menuId;
    // 0x4
    int8_t dialougeBox2Id;
    // 0x5
    int8_t portraitBoxId;
    // 0x6: Specifically for Debug Quiz Window?
    int8_t textBoxId;
    // 0x7
    int8_t unk_0x7;
    // 0x8: pointer to questionBuffer?
    char *questionStrPtr;
    // 0xC: pointer to answerBuffers? uncertain if so because of the way the
    // uses some elements? maybe multipurpose?
    char *answerStrPtr[4];
    int unk_0x1C;
    // 0x20
    int state;
    // 0x24: 
    int currentQuestion;
    // 0x28: Countdown related to holding the stylus on the screen (Lifted state 0x3D)
    int countdownHold;
    // 0x2c
    int unk_0x2c;
    // 0x30: Next step after waiting? Not sure if also used for a countdown
    // from 4->0?
    int nextState;
    // 0x34: personality values
    uint8_t personalityValues[16];
    // 0x44: random number [0, 0x14) for highest values
    uint8_t personalityValuesTieBreaker[16];
    // 0x54: First always 0x41 and last is always 0x40
    uint8_t questionNums[10];
    // 0x5E: Why is this NOT???? The Scripting Variable inverses this value? WHY?
    // Is it NOT here?
    uint8_t notPlayedOldGame;
    // 0x5F
    uint8_t genderResult;
    // 0x60: probably?
    uint16_t currentQuestionString; 
    uint16_t unk_0x62;
    int unk_0x64;
    int unk_0x68;
    // 0x6C: exact size is a guess for now
    char questionBuffer[512];
    // 0x26C: uncertain of num buffers, but certain of buffer length
    // no question has more than 4 answers so logically you would think there
    // would only be 4 buffers for answers
    char answerBuffers[4][64];
    int unk_0x36C;
    // 0x370
    int numSelectablePartners;
    // 0x374: The base game only has 21 starters.
    // I suspect the list has space for 32 and extra entries go unused.
    struct monster_id_16 partners[32];
    // 0x3B4: 0x10 in size
    struct portrait_params portraitParams;
    // 0x3C4: Partner Name Final
    char partner_name_final[32];
    // 0x3E4: Partner Name Keyboard Input
    char partner_name_init[32];
    // 0x404: Partner Name Also Keyboard Input
    char partner_name_init_copy[32];
    // 0x424
    struct monster_id_16 partnerSpecies;
    // 0x426: pulled from ds firmware color 75%, mac 25%
    uint16_t auraColor;
} QuizData;

void __attribute__((naked)) TypesanityCheck() {
    asm("ldr r0,=apSettings");
    asm("ldrh r0,[r0,#0x0]");
    asm("tst r0,#0b0000000010000000");
    asm("bxne lr");
    asm("b SameTypeUnhook");
}

// Clever hook to initalize the question count to the last question and roll
// the player at the same time.
void __attribute__((naked)) ForcedPlayerCheck() {
    asm("ldr r1,=apSettings");
    asm("ldrh r1,[r1,#0x0]");
    asm("and r1,r1,#0b0000001100000000");
    asm("lsr r1,r1,#0x8");
    asm("cmp r1,#0b11");   // STARTER_OPTION_CHOOSE
    asm("cmpne r1,#0b01"); // STARTER_OPTION_RANDOM
    asm("moveq r3,#9"); // Only ask gender question.
    asm("movne r3,#0"); // Ask all questions.
    asm("ldr r1,[r0,#0x0]");
    asm("str r3,[r1,#0x24]");
    asm("b QuizForcedPlayerUnhook");
}

// Force the game to use an unrandom order.
void __attribute__((naked)) ForcedPartnerCheck() {
    asm("mov r7,#0x0"); // Original Instruction
    asm("ldr r1,=apSettings");
    asm("ldrh r1,[r1,#0x0]");
    asm("and r1,r1,#0b0000001100000000");
    asm("lsr r1,r1,#0x8");
    asm("cmp r1,#0b01"); // STARTER_OPTION_RANDOM
    asm("moveq r6,#0x0");
    asm("bx lr");
}

void __attribute__((naked)) ForcedPartnerRollCheck() {
    asm("blt QuizForcedPartnerRollUnhook"); // originalish instruction
    asm("ldr r1,=apSettings");
    asm("ldrh r1,[r1,#0x0]");
    asm("and r1,r1,#0b0000001100000000");
    asm("lsr r1,r1,#0x8");
    asm("cmp r1,#0b01"); // STARTER_OPTION_RANDOM
    asm("bxne lr");
    asm("stmdb sp!,{lr}");
    asm("ldr r0,=apSlotName");
    asm("ldr r1,=apSeed");
    asm("ldr r2,[r1,#0x0]");
    asm("ror r2,r2,#0x7");
    asm("ldr r3,[r0,#0x0]");
    asm("ldr r1,[r0,#0x4]");
    asm("eor r3,r3,r1");
    asm("ldr r1,[r0,#0x8]");
    asm("and r3,r3,r1");
    asm("ldr r1,[r0,#0xC]");
    asm("eor r3,r3,r1");
    asm("eor r0,r3,r2");
    asm("ldr r2,=OVERLAY13_UNKNOWN_POINTER__NA_238CEA0");
    asm("ldr r2,[r2,#0x0]");
    asm("ldr r1,[r2,#0x370]"); // numSelectablePartners
    asm("bl _s32_div_f");
    asm("ldr r0,=OVERLAY13_UNKNOWN_POINTER__NA_238CEA0");
    asm("ldr r0,[r0,#0x0]");
    asm("add r0,r0,#0x300");
    asm("lsl r2,r1,#0x1");
    asm("ldr r3,=STARTERS_PARTNER_IDS");
    asm("ldrsh r1,[r3,r2]");
    asm("mov r7,#0");
    asm("force_partner_fill_entry_loop:");
    asm("add r3,r0,r7,lsl #0x1");
    asm("strh r1,[r3,#0x74]"); // #0x374
    asm("add r7,r7,#1");
    asm("cmp r7,#0x15");
    asm("blt force_partner_fill_entry_loop");
    asm("str r7,[r0,#0x70]"); // 0x370
    asm("ldmia sp!,{lr}");
}

// Check if override is active. If so, allow the quiz result to be overwritten.
void __attribute__((naked)) OverrideHeroCheck() {
    asm("mov r7,#0x0"); // Original Instruction
    asm("ldr r1,=apSettings");
    asm("ldr r1,[r1,#0x0]");
    asm("and r1,r1,#0b0000001100000000");
    asm("lsr r1,r1,#0x8");
    asm("cmp r1,#0b10"); // STARTER_OPTION_OVERRIDE
    asm("moveq r0,#0x42");
    asm("movne r0,#0x26");
    asm("bx lr");
}

void __attribute__((naked)) HeroTweakCheck() {
    asm("ldr r3,=apSettings");
    asm("ldr r3,[r3,#0x0]");
    asm("and r3,r3,#0b0000001100000000");
    asm("lsr r3,r3,#0x8");
    asm("cmp r3,#0b10"); // STARTER_OPTION_OVERRIDE
    asm("cmpne r3,#0b11"); // STARTER_OPTION_CHOOSE
    asm("cmpne r3,#0b11"); // STARTER_OPTION_RANDOM
    asm("ldrnesh r3,[r1,r0]"); // originalish instruction
    asm("bxne lr");
    asm("ldr r3,=OVERLAY13_UNKNOWN_POINTER__NA_238CEA0");
    asm("ldr r3,[r3,#0x0]");
    asm("ldr r3,[r3,#0x24]");
    asm("bx lr");
}

void __attribute__((naked)) ChooseTweakCheck() {
    asm("stmdb sp!,{r0,r1}");
    asm("ldr r3,=apSettings");
    asm("ldr r3,[r3,#0x0]");
    asm("and r3,r3,#0b0000001100000000");
    asm("lsr r3,r3,#0x8");
    asm("cmp r3,#0b11"); // STARTER_OPTION_CHOOSE
    asm("cmpne r3,#0b01"); // STARTER_OPTION_RANDOM
    asm("addne r2,r2,#1"); // originalish instruction
    asm("moveq r2,#0x42");
    asm("ldmia sp!,{r0,r1}");
    asm("bx lr");
}

void __attribute__((naked)) TypeHeroTweak() {
    asm("ldr r1,=apSettings");
    asm("ldr r1,[r1,#0x0]");
    asm("and r1,r1,#0b0000001100000000");
    asm("lsr r1,r1,#0x8");
    asm("cmp r1,#0b10"); // STARTER_OPTION_OVERRIDE
    asm("cmpne r1,#0b11"); // STARTER_OPTION_CHOOSE
    asm("cmpne r1,#0b01"); // STARTER_OPTION_RANDOM
    asm("bne GetPersonality");
    asm("ldr r3,=OVERLAY13_UNKNOWN_POINTER__NA_238CEA0");
    asm("ldr r3,[r3,#0x0]");
    asm("ldr r4,[r3,#0x24]");
    asm("b QuizTypeHeroTweakUnhook");
}

enum monster_id MaleToFemaleForm(enum monster_id monId) {
    if (GetMonsterGender(monId) == GENDER_MALE && monId < 600) {
        if (GetMonsterGender(monId + 600) == GENDER_FEMALE) {
            return monId + 600;
        }
    }
    
    return monId;
}

struct preprocessor_flags preFlags = {.flags_1 = 0b10};
struct window_flags windowYesNoFlags = {.a_accept = true, .b_cancel = true,
    .se_on = true, .y_offset_end = true, .x_offset_end = true};
struct window_flags windowHeroSelectFlags = {.a_accept = true, .se_on = true, 
    .menu_lower_bar = true};

void QuizCustomStateHandler(QuizData* quizData, int state) {
    switch (state) {
        case 0x42:; // Compute Starter List.
            // Build a list of all starters and include starters not normally
            // pickable in vanilla because of gender exclusivity or partner
            // exclusivity.
            int selectable = 0;
            struct monster_id_16 currMonId;
            uint8_t genderPicked = quizData->genderResult;
            int i;
            int j;
            for(i = 0; i < 32; i++) {
                if (genderPicked == 0) {
                    currMonId.val = FemaleToMaleForm(STARTERS_HERO_IDS[i].val);
                } else if (genderPicked == 1) {
                    currMonId.val = MaleToFemaleForm(STARTERS_HERO_IDS[i].val);
                } else {
                    currMonId.val = STARTERS_HERO_IDS[i].val;
                }
                bool notDuplicate = true;
                for(j = i; j >= 0; j--) {
                    if (currMonId.val == quizData->partners[j].val) {
                        notDuplicate = false;
                        break;
                    }
                }      
                if(notDuplicate) {
                    quizData->partners[selectable] = currMonId;
                    selectable += 1;
                }
            }
            for(i = 0 ; i < 21; i++) {
                if (selectable == 32) {
                    break; // The number of entries is full. Bail adding partners!
                }
                if (genderPicked == 0) {
                    currMonId.val = FemaleToMaleForm(STARTERS_PARTNER_IDS[i].val);
                } else if (genderPicked == 1) {
                    currMonId.val = MaleToFemaleForm(STARTERS_PARTNER_IDS[i].val);
                } else {
                    currMonId.val = STARTERS_HERO_IDS[i].val;
                }
                bool notDuplicate = true;
                for(j = selectable; j >= 0; j--) {
                    if (currMonId.val == quizData->partners[j].val) {
                        notDuplicate = false;
                        break;
                    }
                }      
                if(notDuplicate) {
                    quizData->partners[selectable] = currMonId;
                    selectable += 1;
                }
            }
            quizData->numSelectablePartners = selectable;
            // Reuse currentQuestion to store species.
            if (apSettings.starterOptions == STARTER_OPTION_OVERRIDE) {
                quizData->currentQuestion = (STARTERS_HERO_IDS[(GetPersonality()*2) + quizData->genderResult]).val;
                quizData->state = quizData->state + 1;
            } else if (apSettings.starterOptions == STARTER_OPTION_CHOOSE) {
                quizData->portraitBoxId = CreatePortraitBox(0, 3, 1);
                quizData->state = 0x46;
            } else if (apSettings.starterOptions == STARTER_OPTION_RANDOM) {
                quizData->portraitBoxId = CreatePortraitBox(0, 3, 1); // Gets closed right after but that's okay.
                uint32_t randomizedHeroNum = apSeed[0] ^ apSeed[1];
                randomizedHeroNum = randomizedHeroNum ^ (((uint32_t)apSlotName[0]));
                randomizedHeroNum = randomizedHeroNum ^ (((uint32_t)apSlotName[1]) << 1);
                randomizedHeroNum = randomizedHeroNum ^ (((uint32_t)apSlotName[2]) << 2);
                randomizedHeroNum = randomizedHeroNum ^ (((uint32_t)apSlotName[3]) << 3);
                randomizedHeroNum = randomizedHeroNum ^ (((uint32_t)apSlotName[4]) << 4);
                randomizedHeroNum = randomizedHeroNum ^ (((uint32_t)apSlotName[5]) << 5);
                randomizedHeroNum = randomizedHeroNum ^ (((uint32_t)apSlotName[6]) << 6);
                randomizedHeroNum = randomizedHeroNum ^ (((uint32_t)apSlotName[7]) << 7);
                randomizedHeroNum = randomizedHeroNum ^ (((uint32_t)apSlotName[8]) << 8);
                randomizedHeroNum = randomizedHeroNum ^ (((uint32_t)apSlotName[9]) << 9);
                randomizedHeroNum = randomizedHeroNum ^ (((uint32_t)apSlotName[10]) << 10);
                randomizedHeroNum = randomizedHeroNum ^ (((uint32_t)apSlotName[11]) << 11);
                randomizedHeroNum = randomizedHeroNum ^ (((uint32_t)apSlotName[12]) << 12);
                randomizedHeroNum = randomizedHeroNum ^ (((uint32_t)apSlotName[13]) << 13);
                randomizedHeroNum = randomizedHeroNum ^ (((uint32_t)apSlotName[14]) << 14);
                randomizedHeroNum = randomizedHeroNum ^ (((uint32_t)apSlotName[15]) << 15);
                randomizedHeroNum = _u32_div_f(randomizedHeroNum, selectable);
                register int finalHeroNum asm("r1"); // This gets modulo. Yes, it is cursed!
                quizData->currentQuestion = quizData->partners[finalHeroNum].val;
                quizData->state = 0x26;
            } // Breaks if we get here in Vanilla.
            break;
        case 0x43:; // Override Check Message
            struct preprocessor_args preArgs;
            InitPreprocessorArgs(&preArgs);
            preArgs.flag_vals[0] = quizData->currentQuestion;
            ShowDialogueBox(quizData->dialougeBox1Id);
            ShowStringIdInDialogueBox(quizData->dialougeBox1Id, preFlags, CONFIRMATION_STR_ID, &preArgs);
            quizData->state = quizData->state + 1;
            break;
        case 0x44:; // Override Check Yes/No Menu
            if(IsDialogueBoxActive(quizData->dialougeBox1Id) == false) {
                quizData->menuId = CreateSimpleMenuFromStringIds(&QUIZ_WINDOW_PARAMS_5, windowYesNoFlags, NULL,
                    QUIZ_MENU_ITEMS_1, 2);
                quizData->state = quizData->state + 1;
            }
            break;
        case 0x45:; // Override Check Response
            undefined menuResult = GetSimpleMenuResult(quizData->menuId);
            if(menuResult == 1) {
                CloseSimpleMenu(quizData->menuId);
                quizData->menuId = 0xFE;
                quizData->state = 0x26;
            } else if (menuResult == 2) {
                HidePortraitBox(quizData->portraitBoxId);
                CloseSimpleMenu(quizData->menuId);
                quizData->menuId = 0xFE;
                quizData->state = quizData->state + 1;
            }
            break;
        case 0x46:; // Make Hero Selection Menu
            ShowDialogueBox(quizData->dialougeBox1Id);
            ShowStringIdInDialogueBox(quizData->dialougeBox1Id, preFlags, WHO_LIKE_TO_BE_STR_ID, &preArgs);
            quizData->menuId = CreateAdvancedMenu(&QUIZ_WINDOW_PARAMS_6, windowHeroSelectFlags,
                NULL, GetOptionStringFromID, quizData->numSelectablePartners, 6);
            PARTNER_SELECT_MENU_OPTION_TRACKER = 0;
            PARTNER_SELECT_MENU_OPTION_TIMER = 0;
            InitPortraitParamsWithMonsterId(&(quizData->portraitParams),
                (quizData->partners[0]).val);
            SetPortraitEmotion(&quizData->portraitParams, PORTRAIT_NORMAL);
            SetPortraitLayout(&quizData->portraitParams, 4);
            SetPortraitOffset(&quizData->portraitParams, &PARTNER_SELECT_PORTRAIT_OFFSETS);
            ShowPortraitInPortraitBox(quizData->portraitBoxId, &quizData->portraitParams);
            quizData->state += 1;
            break;
        case 0x47:; // Player Is Picking Hero
            int selected = (int)GetAdvancedMenuCurrentOption(quizData->menuId);
            if (PARTNER_SELECT_MENU_OPTION_TRACKER == selected) {
                int currentCounter = PARTNER_SELECT_MENU_OPTION_TIMER;
                PARTNER_SELECT_MENU_OPTION_TIMER = currentCounter + 1;
                if (currentCounter == 0xF0) {
                    InitPortraitParamsWithMonsterId(&(quizData->portraitParams),
                    (quizData->partners[selected]).val);
                    SetPortraitEmotion(&quizData->portraitParams, PORTRAIT_NORMAL);
                    SetPortraitLayout(&quizData->portraitParams, 4);
                    SetPortraitOffset(&quizData->portraitParams, &PARTNER_SELECT_PORTRAIT_OFFSETS);
                    ShowPortraitInPortraitBox(quizData->portraitBoxId, &quizData->portraitParams);
                    PARTNER_SELECT_MENU_OPTION_TIMER = 0x00;
                } else if (currentCounter == 0xC0) {
                    InitPortraitParamsWithMonsterId(&(quizData->portraitParams),
                    (quizData->partners[selected]).val);
                    SetPortraitEmotion(&quizData->portraitParams, PORTRAIT_JOYOUS);
                    SetPortraitLayout(&quizData->portraitParams, 4);
                    SetPortraitOffset(&quizData->portraitParams, &PARTNER_SELECT_PORTRAIT_OFFSETS);
                    ShowPortraitInPortraitBox(quizData->portraitBoxId, &quizData->portraitParams);
                } else if (currentCounter == 0x90) {
                    InitPortraitParamsWithMonsterId(&(quizData->portraitParams),
                    (quizData->partners[selected]).val);
                    SetPortraitEmotion(&quizData->portraitParams, PORTRAIT_SURPRISED);
                    SetPortraitLayout(&quizData->portraitParams, 4);
                    SetPortraitOffset(&quizData->portraitParams, &PARTNER_SELECT_PORTRAIT_OFFSETS);
                    ShowPortraitInPortraitBox(quizData->portraitBoxId, &quizData->portraitParams);
                } else if (currentCounter == 0x60) {
                    InitPortraitParamsWithMonsterId(&(quizData->portraitParams),
                    (quizData->partners[selected]).val);
                    SetPortraitEmotion(&quizData->portraitParams, PORTRAIT_ANGRY);
                    SetPortraitLayout(&quizData->portraitParams, 4);
                    SetPortraitOffset(&quizData->portraitParams, &PARTNER_SELECT_PORTRAIT_OFFSETS);
                    ShowPortraitInPortraitBox(quizData->portraitBoxId, &quizData->portraitParams);
                } else if (currentCounter == 0x30) {
                    InitPortraitParamsWithMonsterId(&(quizData->portraitParams),
                    (quizData->partners[selected]).val);
                    SetPortraitEmotion(&quizData->portraitParams, PORTRAIT_HAPPY);
                    SetPortraitLayout(&quizData->portraitParams, 4);
                    SetPortraitOffset(&quizData->portraitParams, &PARTNER_SELECT_PORTRAIT_OFFSETS);
                    ShowPortraitInPortraitBox(quizData->portraitBoxId, &quizData->portraitParams);
                }
            } else {
                PARTNER_SELECT_MENU_OPTION_TRACKER = selected;
                PARTNER_SELECT_MENU_OPTION_TIMER = 0;
                InitPortraitParamsWithMonsterId(&(quizData->portraitParams),
                (quizData->partners[selected]).val);
                SetPortraitEmotion(&quizData->portraitParams, PORTRAIT_NORMAL);
                SetPortraitLayout(&quizData->portraitParams, 4);
                SetPortraitOffset(&quizData->portraitParams, &PARTNER_SELECT_PORTRAIT_OFFSETS);
                ShowPortraitInPortraitBox(quizData->portraitBoxId, &quizData->portraitParams);
            }
            if (IsAdvancedMenuActive(quizData->menuId) == false) {
                quizData->currentQuestion = quizData->partners[selected].val;
                CloseAdvancedMenu(quizData->menuId);
                quizData->menuId = 0xFE;
                HidePortraitBox(quizData->portraitBoxId);
                quizData->state = 0x43;
            }
            break;
    }
    return;
}
