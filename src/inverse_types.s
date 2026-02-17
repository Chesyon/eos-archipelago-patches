.align 4
GhostImmunityIsActiveFlip:
    ldr   r3,=CUSTOM_SAVE_AREA
    ldrb  r3,[r3,#0x2C0]
    cmp   r3,#0
    movne r0,#0
    popne {r3,pc}   
    ldr   r3,[r1,#0xb4] // original instruction
    bx    lr

IsTypeIneffectiveAgainstGhostFlip:
    ldr  r0,=CUSTOM_SAVE_AREA
    ldrb r0,[r0,#0x2C0] 
    eor  r0,r0,#1 // !flip
    bx   lr

GetTypeMatchupFlip:
  ldr   r1,=CUSTOM_SAVE_AREA
  ldrb  r1,[r1,#0x2C0] // or some other valid immediate; i just picked the first free byte in the save area.
  cmp   r1,#0
  popeq {r3-r7,pc}
  cmp   r0,#2    // eq: neutral -> neutral, no change needed
  addlt r0,r0,#2 // immune -> neutral, NVE -> SE
  subgt r0,r0,#2 // SE -> NVE
  pop   {r3-r7,pc}