// IQ multiplier is the first 4 bits in the settings struct, so...
// access first byte
// bitwise AND 0b1111 to trim to just 4 bits

IqMultiplierGround:
    ldr  r0,=newApSettings // load pointer to settings struct
    ldrb r0,[r0,#1]        // load iq multiplier byte from settings struct
    mul  r2,r4,r0          // multiply IQ increase by the multiplier
    add  r0,r1,r2          // originalish instruction
    sub  r1,r0,r4          // commit jank to fix iq strings
    bx   lr // return
.align
IqMultiplierGroundMisc:
    // original instruction: mov r4, r0. iq increase in r1
    ldr  r4,=apSettings // load pointer to settings struct
    ldrb r4,[r4,#1]     // load first byte in settings struct
    mul  r1,r4,r1       // multiply IQ increase by the multiplier
    mov  r4,r0          // original instruction
    bx   lr             // return
.align
IqMultiplierDungeon:
    push {r0}              // we need a free register to work with so we free up r0
    ldr  r0,=newApSettings // load pointer to settings struct
    ldrb r0,[r0,#1]        // load iq multiplier byte from settings struct
    mul  r1,r0,r1          // multiply IQ increase by the multiplier
    pop  {r0}              // restore r0 to its value from vanilla
    add  r1,r4,r1          // original instruction
    bx   lr                // return
.align
IqMultiplierDungeonGummi:
    ldr   r2,=newApSettings // load pointer to settings struct
    ldrb  r2,[r2,#1]        // load iq multiplier byte from settings struct
    mul   r0,r11,r2         // multiply IQ increase by the multiplier
    add   r2,r6,r0          // originalish instruction
    ldr   r1,=#999
    cmp   r6,r1
    sublt r6,r2,r11         // commit jank to fix iq strings
    bx   lr //return
