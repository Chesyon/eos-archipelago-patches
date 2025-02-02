.align 4
    AutofillName: // both sets up for truncation and sets the pointer for the
    ldr r1,=NameAutofillStringAddr // buffer truncate string by writing a null
    strb r0,[r1,#0x10]             // byte at the 11th character. r0 already
                                   // contains 0 from the original code.
    mov r2,#0 // original code
    bx lr     // return to original code