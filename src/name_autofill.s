.align 4
AutofillName: 
    ldr  r1,=slotData // both sets up for truncation and sets the pointer for the buffer
    strb r0,[r1,#10]  // truncate string by writing a null byte at the 11th character. r0 already contains 0 from the original code.
    mov  r2,#0        // original code
    bx   lr           // return to original code
