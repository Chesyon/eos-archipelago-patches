#define VAR_SCENARIO_SUB1 #5
#define VAR_SCENARIO_SUB2 #6
#define NULL #0x0
#define MAX_MULTIWORLD_ITEMS_AT_ONCE_KEC1 #7;
#define MAX_MULTIWORLD_ITEMS_AT_ONCE_KEC2 #3;

.align
GenerateShop1Tweak:
    sub  sp,sp,#0x8
    mov  r0,NULL
    mov  r1,VAR_SCENARIO_SUB1
    mov  r2,#1
    bl   LoadScriptVariableValueAtIndex
    mov  r7,r0
    mov  r0,NULL
    mov  r1,VAR_SCENARIO_SUB2
    mov  r2,#0
    bl   LoadScriptVariableValueAtIndex
    lsl  r0,r0,#0x8
    orr  r0,r0,r7
    mov  r8,#0x0
    mov  r3,#0b0000000000000100
    mov  r7,#0x0
    check_item_flag_kec1_loop:
        tst    r0,r3
        streqb r7,[sp,r8]
        addeq  r8,#0x1
        add    r7,r7,#1
        lsl    r3,r3,#0x1
        cmp    r7,#7
        blt   check_item_flag_kec1_loop
    cmp   r8,MAX_MULTIWORLD_ITEMS_AT_ONCE_KEC1
    movlt r0,r8
    movge r0,MAX_MULTIWORLD_ITEMS_AT_ONCE_KEC1
    bl    RandIntSafe
    cmp   r8,#0
    addgt r7,r0,#1
    movle r7,r0
    kec_shop1_loop:
        mov r0,r5
        bl  RandIntSafe
        mov r4,r0
        mov r0,r5
        bl  RandIntSafe
        mov r2,r0
        mov r0,r6
        mov r1,r4
        bl  GetItemIdFromList
        bl  AddItemToKecleon1
        add r7,r7,#0x1
        cmp r7,#0x8
        blt kec_shop1_loop
    ldr r7,=kec1_multiworld_item_table
    kec_shop1_add_mulworld_items_loop:
        cmp  r8,#0
        ble  return_kec1_shop_tweak
        mov  r0,r8
        bl   RandIntSafe
        mov  r5,r0
        ldrb r4,[sp,r0]
        lsl  r4,r4,#0x1
        ldrh r0,[r7,r4]
        bl   AddItemToKecleon1
        sub  r8,r8,#1
        ldrb r1,[sp,r8]
        strb r1,[sp,r5]
        b    kec_shop1_add_mulworld_items_loop
    return_kec1_shop_tweak:
    bl  SortKecleonItems1
    add sp,sp,#0x8
    ldmia sp!,{r4,r5,r6,r7,r8,pc}
    
    .align
GenerateShop2Tweak:
    push {r8}
    sub  sp,sp,#0x8
    mov  r0,NULL
    mov  r1,VAR_SCENARIO_SUB2
    mov  r2,#0
    bl   LoadScriptVariableValueAtIndex
    lsl  r0,r0,#0x8
    mov  r1,NULL
    mov  r8,#0x0
    mov  r3,#0b0000001000000000
    mov  r7,#0x0
    check_item_flag_kec2_loop:
        tst    r0,r3
        streqb r7,[sp,r8]
        addeq  r8,#0x1
        add    r7,r7,#1
        lsl    r3,r3,#0x1
        cmp    r7,#3
        blt   check_item_flag_kec2_loop
    cmp   r8,MAX_MULTIWORLD_ITEMS_AT_ONCE_KEC2
    movlt r0,r8
    movge r0,MAX_MULTIWORLD_ITEMS_AT_ONCE_KEC2
    bl    RandIntSafe
    cmp   r8,#0
    addgt r7,r0,#1
    movle r7,r0
    kec_shop2_loop:
        mov r0,r5
        bl  RandIntSafe
        mov r4,r0
        mov r0,r5
        bl  RandIntSafe
        mov r2,r0
        mov r0,r6
        mov r1,r4
        bl  GetItemIdFromList
        bl  AddItemToKecleon2
        add r7,r7,#0x1
        cmp r7,#0x4
        blt kec_shop2_loop
    ldr r7,=kec2_multiworld_item_table
    kec_shop2_add_mulworld_items_loop:
        cmp  r8,#0
        ble  return_kec2_shop_tweak
        mov  r0,r8
        bl   RandIntSafe
        mov  r5,r0
        ldrb r4,[sp,r0]
        lsl  r4,r4,#0x1
        ldrh r0,[r7,r4]
        bl   AddItemToKecleon2
        sub  r8,r8,#1
        ldrb r1,[sp,r8]
        strb r1,[sp,r5]
        b    kec_shop2_add_mulworld_items_loop
    return_kec2_shop_tweak:
    bl  SortKecleonItems2
    add sp,sp,#0x8
    pop {r8}
    ldmia sp!,{r3,r4,r5,r6,r7,pc}
    
    .align
    AddItemToBagWithMultiworldCheck:
        stmdb sp!,{r4,lr}
        mov  r4,r0
        ldrh r0,[r0,#0x0]
        ldr  r1,=kec1_multiworld_item_table
        mov  r2,#0
        loop_check_for_multiworld_items_kec1:
            lsl     r3,r2,#1
            ldrh    r3,[r1,r3]
            cmp     r0,r3
            beq     set_multiworld_item_flag
            add     r2,r2,#1
            cmp     r2,#7
            blt     loop_check_for_multiworld_items_kec1
        ldr r1,=kec2_multiworld_item_table
        mov r2,#0
        loop_check_for_multiworld_items_kec2:
            lsl     r3,r2,#1
            ldrh    r3,[r1,r3]
            cmp     r0,r3
            addeq   r2,#7
            beq     set_multiworld_item_flag
            add     r2,r2,#1
            cmp     r2,#3
            blt     loop_check_for_multiworld_items_kec2
        mov r0,r4
        bl  SpecialProcAddItemToBag
        ldmia sp!,{r4,pc}
        set_multiworld_item_flag:
        mov   r4,r2
        cmp   r4,#6
        mov   r0,NULL
        movlt r1,VAR_SCENARIO_SUB1
        movge r1,VAR_SCENARIO_SUB2
        movlt r2,#1
        movge r2,#0
        bl    LoadScriptVariableValueAtIndex
        mov   r1,#0b00000001
        cmp   r4,#6
        addlt r2,r4,#2
        subge r2,r4,#6
        lsl   r1,r1,r2
        orr   r3,r0,r1
        mov   r0,NULL
        movlt r1,VAR_SCENARIO_SUB1
        movge r1,VAR_SCENARIO_SUB2
        movlt r2,#1
        movge r2,#0
        bl    SaveScriptVariableValueAtIndex
        ldmia sp!,{r4,pc}
    .pool
    .align
        kec2_multiworld_item_table:
            .hword 113
            .hword 236
            .hword 177
            .hword 400 // Just a way to see errors // padding.
    .align
        kec1_multiworld_item_table:
            .hword 11
            .hword 12
            .hword 98
            .hword 114
            .hword 166
            .hword 175
            .hword 184 // This is #9 now!
            .hword 400 // Just a way to see errors // padding.
