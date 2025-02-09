// access bits 7 8 9 10 of apSettings
// 0b 0 0 0 0 0 00 1111 0 0 0 0 0
// AND 0b1111
// this should leave us with just those bits, which is our multiplier!

IqScalingGround:
    ldr  r0,=apSettings
    ldrh r0,[r0]
    and  r0,r0,#0b1111
    mul  r4,r4,r0 // multiply IQ increase by the multiplier
    add  r0,r1,r4 // original instruction
    bx   lr // return
.align
IqScalingDungeon:
    push {r0}
    ldr  r0,=apSettings
    ldrh r0,[r0]
    and  r0,r0,#0b1111
    mul  r1,r1,r0 // multiply IQ increase by the multiplier
    pop  {r0}
    add  r1,r4,r1 // original instruction
    bx   lr // return
