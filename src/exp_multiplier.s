// Keep in mind that the exp gained is in r0, so don't touch it except to multiply!
ExpMultiplier:
    ldr   r1,=newApSettings // pointer to apSettings to r1
    ldrb  r1,[r1]           // load first byte of newApSettings (exp multiplier) to r1
    mul   r0,r1,r0          // multiply our exp gained (r0) by our exp multiplier (r1)!
    ldmia sp!,{r4, pc}      // return