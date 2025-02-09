#define POISON_INSTRUMENT_ID 1390
#define TYPE_POISON 8
#define BUG_INSTRUMENT_ID 1391
#define TYPE_BUG 12
#define FIGHTING_INSTRUMENT_ID 1392
#define TYPE_FIGHTING 7
#define GHOST_INSTRUMENT_ID 1393
#define TYPE_GHOST 14
#define DARK_INSTRUMENT_ID 1394
#define TYPE_DARK
#define ELECTRIC_INSTRUMENT_ID 1395
#define TYPE_ELECTRIC 5
#define NORMAL_INSTRUMENT_ID 1396
#define TYPE_NORMAL 1
#define PSYCHIC_INSTRUMENT_ID 1397
#define TYPE_PSYCHIC 11
#define DRAGON_INSTRUMENT_ID 1398
#define TYPE_DRAGON 15
#define STEEL_INSTRUMENT_ID 1399
#define TYPE_STEEL 17
#define TYPE_INSTRUMENT_RECRUIT_BONUS 0xC8

#define TYPE_NONE 0
#define MULTITYPE_INSTRUMENT_ID 1387
#define MONOTYPE_INSTRUMENT_ID 1389
#define MINOR_INSTRUMENT_RECRUIT_BONUS 0x64
#define REDUCE_SPAWN_INSTRUMENT_ID 1388
#define REDUCE_SPAWN_INSTRUMENT_COUNTER 54

.align 4
    ExtraInstrumentsCheck:
        stmdb sp!,{r3,r4,r5,r6}
        mov r4,#0
        ldr r5,=instrument_type_generic_table
        ldr r6,=POISON_INSTRUMENT_ID
        check_for_custom_type_instrument_loop:
            mov r0,r8
            add r1,r6,r4
            bl  ItemIsActive
            cmp r0,#0x0
            beq check_for_custom_type_instrument_iter
            mov r0,r7
            ldrb r1,[r6,r4]
            bl MonsterIsType
            cmp r0,#0x0
            beq check_for_custom_type_instrument_iter
            add r6,r6,#TYPE_INSTRUMENT_RECRUIT_BONUS
            b instrument_found_return
            check_for_custom_type_instrument_iter:
            add r4,r4,#1
            cmp r4,#(STEEL_INSTRUMENT_ID - POISON_INSTRUMENT_ID)
            blt check_for_custom_type_instrument_loop
        ldr r1,[r7,#0xB4]
        ldrb r2,[r1,#0x16] // Check for Monotype or Not
        ldrb r3,[r1,#0x17]
        cmp r2,#TYPE_NONE
        cmpne r3,#TYPE_NONE
        ldreq r1,=#MONOTYPE_INSTRUMENT_ID
        ldrne r1,=#MULTITYPE_INSTRUMENT_ID
        mov r0,r8
        bl ItemIsActive
        cmp r0,#0x0
        addne r6,r6,#MINOR_INSTRUMENT_RECRUIT_BONUS
        instrument_found_return:   
        ldmia sp!,{r3,r4,r5,r6}
        b RecruitInstrumentUnhook
        .pool
        .align
            instrument_type_generic_table:
                .byte TYPE_POISON
                .byte TYPE_BUG
                .byte TYPE_FIGHTING
                .byte TYPE_GHOST
                .byte TYPE_DARK
                .byte TYPE_ELECTRIC
                .byte TYPE_NORMAL
                .byte TYPE_PSYCHIC
                .byte TYPE_DRAGON
                .byte TYPE_STEEL

.align 4
    ReduceSpawnInstrumentCheck:
        stmdb sp!,{r0,r1,r2,r3,r4,r5,r6,r7,r8,lr}
        mov r4,r1
        add r5,r0,#0x12400
        add r5,r5,#0x28 // DUNGEON + 0x12B28
        mov r6,#0x0
        reduce_spawn_instrument_loop:
            ldr r7,[r5,r6,lsl #0x4]
            mov r0,r7
            bl EntityIsValid
            cmp r0,#0x0
            beq reduce_spawn_instrument_iter
            mov r0,r7
            ldr r1,=#REDUCE_SPAWN_INSTRUMENT_ID
            bl ItemIsActive
            cmp r0,#0x0
            movne r0,#REDUCE_SPAWN_INSTRUMENT_COUNTER
            bne reduce_spawn_instrument_found
            reduce_spawn_instrument_iter:
            add r6,r6,#1
            cmp r6,#4
            blt reduce_spawn_instrument_loop
        ldrsh r0,[r4,#0x0] // original instruction
        reduce_spawn_instrument_found:
        ldmia sp!,{r0,r1,r2,r3,r4,r5,r6,r7,r8,pc}
        .pool
        .align