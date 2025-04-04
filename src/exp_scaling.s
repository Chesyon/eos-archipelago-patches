// EXP multiplier the last 4 bits of the settings struct.
// We can do this by loading the second byte and then bitshifting right 4.
// Keep in mind that the exp gained is in r0, so don't touch it except to multiply!
ExpScaling:
    ldr   r1,=apSettings+1 // load pointer for second byte of apSettings to r1
    ldrb  r1,[r1]          // get second byte of apSettings to r1
    lsr   r1,r1,#4         // bitshift r1 by 4 to get just the last 4 bits (which is our exp multiplier).
    mul   r0,r1,r0         // multiply our exp gained (r0) by our exp multiplier (r1)!
    ldmia sp!,{r4, pc}     // return