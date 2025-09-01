.align 4
AutofillName:
    push  {r0,r14}
    add   r0,sp,#0xC0
    ldr   r1,=slotData+0x10 // We want to get the name from slot data
    mov   r2,#10            // We want to copy ten characters
    bl    strncpy
    add   r1,sp,#0xC0
    mov   r2,r1
    pop   {r0,r15}          // return to original code
.pool