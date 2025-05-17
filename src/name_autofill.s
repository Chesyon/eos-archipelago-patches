.align 4
AutofillName:
    push  {r14}
    ldr   r0,=NameBuffer // We want to copy the name to the buffer
    ldr   r1,=slotData   // We want to get the name from slot data
    mov   r2,#10         // We want to copy ten characters
    bl    strncpy        
    mov   r1,r0          // Set the name menu to read from the buffer (r0 is the same at the end of strncpy, so it's still a pointer to NameBuffer)
    mov   r0,#0x0        // r0 was 0 in original code
    mov   r2,#0          // original code
    pop   {r15}          // return to original code
NameBuffer:
    .fill 0xB,0x0        // buffer for the name, so we don't overwrite the original info. last byte is reserved to be null
.pool