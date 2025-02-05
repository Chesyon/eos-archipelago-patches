.align 4
    BranchLevel:
        // First things first, check archipelago setting. Skip all the code if level scaling is disabled
        stmdb sp!,{r0-r8, lr}
        stmdb sp!,{r0}
        ldr r0,=apSettings
        ldrb r0,[r0,#0x0]
        tst  r0,#0b0000000000001000 // Test ArchipelagoSettings->levelScaling
        ldmia sp!,{r0}
        beq return // if the value is 0, don't do level scaling
        // more checks for if level scaling should be used.
        cmp r0, #0 // i have literally no clue what this does lmaoooo- maybe this is what stops bossfights from scaling? regardless i'm leaving it in for now
        beq return
        // we don't want any special episode dungeons to be scaled! don't level scale if the dungeon id ranges from 123 to 164.
        ldr r4,=DUNGEON_PTR
        ldr r2,[r4]
        ldrb r2,[r2, #+0x748]
        cmp r2, #123 // Star Cave
        blt StartScale
        cmp r2, #164 // Spring Cave Pit
        bgt StartScale
        b return

    // Calcs max party level
    StartScale:
        mov r1,#0 // r1: Loop counter
    loopLevelScale:
        mov r0, r1
        // Validates the entity
        // push pop on r1-r4 because this function tends to mess up registers
        stmdb sp!,{r1-r4}
        bl GetActiveTeamMember
        ldmia sp!,{r1-r4}
        cmp r0,#0
        beq nextIterScaling
        ldrb r4, [r0]
        tst r4,#1
        beq nextIterScaling
        ldrb r2, [r0,#0x2]
        // r3 will keep your max party level
        cmp r1, #0
        moveq r3, #0
        cmp r2, r3
        movgt r3, r2
    nextIterScaling:
        add r1,r1,#1
        cmp r1,#0x3
        blt loopLevelScale

    // Checks if the level has to scale
    ContinueScale:
        mov r6, #0
        add r3, r3, r3 // level is stored as level x2 in the struct
        mov r7, r3
        // gets the first entry level in the spawn list
        ldr r4,=DUNGEON_PTR
        ldr r5, [r4]
        add r5, r5, #0x1
        add r5, r5, #0x164
        add r5, r5, #0x2C800

    LoopScale:
        ldrb r4, [r5]
        cmp r4, #2  // if level 0 or level 1, we shouldn't handle scaling
        beq CountScale
        b Difficult

    // different scaling models taken from Alpha are listed here
    // Alpha juggles between them with a few Performance flags
    // However writing and reading a byte in the save file
    // for that system sounds more convenient for that system

    // i removed all of them except difficult. owned

    // Difficult: eLevel = aLevel, always
    Difficult:
        mov r3, r7
        mov r4, r3
        strb r4, [r5]
    CountScale:
        add r5, r5,#0x8 // switch to the next entry level in list
        add r6, r6,#0x1 // switch to the next loop iteration
        cmp r6, #16 // 16 enemies per floor max, should iterate 16 times
        blt LoopScale

    return:
        ldmia sp!,{r0-r8, lr}
        b IsFullFloorFixedRoom // original instruction, hence the way we hook
