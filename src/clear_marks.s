.align 4
ClearMarkCheck:
    push {lr}                            // setup for return at end
                                         // setup for LoadScriptVariableValueAtIndex. r0 doesn't matter since this isn't local...
    mov   r1,#0x4E                       // set variable to access to PERFORMANCE_PROGRESS_LIST
    add   r2,r7,#51                      // and the index should be mark_id+51.
    bl    LoadScriptVariableValueAtIndex // ok cool now load!!
    pop  {pc}                            // return