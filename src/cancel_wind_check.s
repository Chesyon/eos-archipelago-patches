.align 4
    WindCounterCheck:
    poplt {r4,pc} // if turn counter is negative, don't run the rest of DecrementWindCounter
    // if not...
    subgt r1,r1,#0x1 // original instruction
    bx    lr // return to original code
