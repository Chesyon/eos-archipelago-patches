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

/*
typedef struct QuizData {
    // 0x0: something related to waiting for boxes and state?
    uint8_t unk_0x0;
    
    // 0x2: during the quiz questions?
    uint8_t dialougeBox1Id;
    // 0x3
    uint8_t simpleMenuId;
    // 0x4
    uint8_t dialougeBox2Id;
    
    // 0x6: Specifically for Debug Quiz Window?
    uint8_t textBoxId;
    
    // 0x8: pointer to 0x6C?
    int unk_0x8;
    
    // 0x14: moves to 0x18?
    int unk_0x14;
    // 0x18: moves to 0x1C? points to 0xC sometimes???
    int unk_0x18;
    // 0x1C:
    int unk_0x1C;
    // 0x20
    int state;
    // 0x24: 
    int currentQuestion;
    // 0x28: Countdown related to holding the stylus on the screen (Lifted state 0x3D)
    int countdownHold;
    
    // 0x30: Seems like a countdown for a delay after the quiz?
    // Set to 4? Then countdown?
    int countdown;
    // 0x34: personality values
    uint8_t personalityValues[16];
    // 0x44: random number [0, 0x14) for highest values
    uint8_t personalityValuesTieBreaker[16];
    
    // 0x54: First always 0x41 and last is always 0x40
    uint8_t questionNums[10]
    // 0x5E: Why is this NOT???? The Scripting Variable inverses this value? WHY?
    // Is it NOT here?
    uint8_t notPlayedOldGame;
    // 0x5F
    uint8_t genderResult;
    // 0x60: this is some weird value that uses like 0x24, 0x54, and quiz
    // question strings I have no idea what it's for
    uint16_t questionString; 
    
    // 0x6C: exact size is a guess for now
    char questionBuffer[30];
    
    // 0x26C: uncertain of num buffers, but certain of buffer length
    // no question has more than 4 answers so logically you would think there
    // would only be 4 buffers for answers
    char answerBuffers[4][64];
    
    // 0x3B4
    struct portrait_params portraitParams;
    
    // 0x426
    uint16_t unk_0x426;
} QuizData;

*/

void __attribute__((naked)) TypesanityCheck() {
    asm("ldr r0,=apSettings");
    asm("ldr r0,[r0,#0x0]");
    asm("tst r0,#0b0000000000010000");
    asm("bxne lr");
    asm("b SameTypeUnhook");
}

// Clever hook to initalize the question count to the last question and roll
// the player at the same time.
void __attribute__((naked)) ForcedPlayerCheck() {
    asm("ldr r1,=apSettings");
    asm("ldr r1,[r1,#0x0]");
    asm("and r1,r1,#0b0000000001100000");
    asm("lsr r1,r1,#0x5");
    asm("cmp r1,#0b01"); // STARTER_OPTION_RANDOM 
    asm("ldr r1,[r0,#0x0]");
    asm("movne r3,#0"); // 0 if normal
    asm("moveq r3,#9"); // Initalize to 9 questions if forced player.
    asm("str r3,[r1,#0x24]");
    asm("bne QuizForcedPlayerUnhook");
    asm("stmdb sp!,{r0,r1,r2,r3,r4,r5,r12}");
    asm("ldr r2,=apSlotName");
    asm("ldr r3,=apSeed");
    asm("ldr r4,[r2,#0x0]");
    asm("ldr r5,[r2,#0x4]");
    asm("eor r4,r4,r5");
    asm("lsl r4,r4,#0x2");
    asm("ldr r5,[r2,#0x8]");
    asm("eor r4,r4,r5");
    asm("ldr r5,[r2,#0xC]");
    asm("eor r4,r4,r5");
    asm("ldr r5,[r3,#0x0]");
    asm("add r4,r4,r5");
    asm("ldr r5,[r3,#0x4]");
    asm("eor r4,r4,r5");
    asm("and r4,r4,#0b1111"); // Personality 0-15
    asm("ldr r1,[r0,#0x0]");
    asm("add r1,r1,r4");
    asm("mov r3,#99"); // Set score for personality to be 99.
    asm("strb r3,[r1,#0x34]");
    asm("ldmia sp!,{r0,r1,r2,r3,r4,r5,r12}");
    asm("b QuizForcedPlayerUnhook");
}

// Lazy hook to force a specific partner by... replacing all entries with the
// same monster.
void __attribute__((naked)) ForcedPartnerCheck() {
    asm("mov r7,#0x0"); // Original Instruction
    asm("ldr r1,=apSettings");
    asm("ldr r1,[r1,#0x0]");
    asm("and r1,r1,#0b0000000001100000");
    asm("lsr r1,r1,#0x5");
    asm("cmp r1,#0b01"); // STARTER_OPTION_RANDOM
    asm("bxne lr");
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
    asm("eor r3,r3,r2");
    asm("and r3,r3,#0b1111");
    asm("ldr r0,=OVERLAY13_UNKNOWN_POINTER__NA_238CEA0");
    asm("ldr r0,[r0,#0x0]");
    asm("add r0,r0,#0x300");
    asm("lsl r2,r3,#0x1");
    asm("ldr r1,=STARTERS_PARTNER_IDS");
    asm("ldrsh r3,[r1,r2]");
    asm("force_partner_fill_entry_loop:");
    asm("add r1,r0,r7,lsl #0x1");
    asm("strh r3,[r1,#0x74]"); // #0x374
    asm("add r7,r7,#1");
    asm("cmp r7,#0x15");
    asm("blt force_partner_fill_entry_loop");
    asm("str r7,[r0,#0x70]"); // 0x370
    asm("b QuizForcedPartnerUnhook");
}

void QuizCustomStateHandler(void* quizData, int state) {
    switch (state) {
        case 0x42:; // Override Check
            struct preprocessor_args preArgs;
            InitPreprocessorArgs(&preArgs);
            
            break;
    }
    return;
}
