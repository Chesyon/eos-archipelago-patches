#define DUNGEON_GRID_CELL_BYTES 0x1E

#include <pmdsky.h>
#include <cot.h>

void __attribute__((naked)) CreateRoomInCell (int topLeftX, int topLeftY, int botRightX, int botRightY, int roomId, void* roomStuff, int bitflags) {
    asm("stmdb sp!,{r4,r5,r6,r7,r8,r9,lr}");
    asm("cmp   r0,r2");    // Check for correct order of arguments.
    asm("cmplt r1,r3");
    asm("ldmgeia sp!,{r4,r5,r6,r7,r8,r9,pc}");
    asm("cmp   r0,#0x0");  // Check lower bounds.
    asm("cmpge r1,#0x0");
    asm("ldmltia sp!,{r4,r5,r6,r7,r8,r9,pc}");
    asm("cmp   r2,#0x38"); // Check upper bounds.
    asm("cmplt r3,#0x20");
    asm("ldmgeia sp!,{r4,r5,r6,r7,r8,r9,pc}");
    asm("ldrb  r8,[sp,#0x1C]"); // Room #
    asm("mov   r4,r0"); // x0
    asm("mov   r9,r1"); // y0 
    asm("mov   r6,r2"); // x1
    asm("mov   r7,r3"); // y1
    asm("mov   r1,#0x1");
    asm("ldr   r0,[sp,#0x20]"); // Cell Ptr
    asm("strb  r1,[r0,#0xA]");  // is_room = TRUE
    asm("strb  r1,[r0,#0xB]");  // is_connected = TRUE
    asm("strh  r4,[r0,#0x0]");  // Cell Start X
    asm("strh  r9,[r0,#0x2]");  // Cell Start Y
    asm("strh  r6,[r0,#0x4]");  // Cell End X
    asm("strh  r7,[r0,#0x6]");  // Cell End Y
    asm("create_room_in_cell_x_loop:");
    asm("mov   r5,r9");
    asm("create_room_in_cell_y_loop:");
    asm("mov   r0,r4");
    asm("mov   r1,r5");
    asm("bl    GetTileSafe");
    asm("add   r5,r5,#0x1"); // Y Iterate
    asm("ldrh  r1,[r0,#0x0]");
    asm("bic   r1,r1,#0x3");
    asm("orr   r1,r1,#0x1");
    asm("strh  r1,[r0,#0x0]");
    asm("strb  r8,[r0,#0x7]");
    asm("cmp   r5,r7");
    asm("blt   create_room_in_cell_y_loop");
    asm("add   r4,r4,#0x1"); // X Iterate
    asm("cmp   r4,r6");
    asm("blt   create_room_in_cell_x_loop");
    asm("ldr   r0,=FLOOR_GENERATION_STATUS");
    asm("ldr   r0,[r0,#0x18]");
    asm("cmp   r0,#0x0");
    asm("moveq r8,#0x0");
    asm("beq   create_room_in_cell_imperfection_check");
    asm("mov   r0,#0x64");
    asm("bl    DungeonRandInt");
    asm("cmp   r0,#0x50");
    asm("movge r8,#0x0");
    asm("movlt r8,#0x1");
    asm("create_room_in_cell_imperfection_check:");
    asm("ldrb  r7,[sp,#0x24]");
    asm("tst   r7,#0x4"); // Check Imperfect Room Bit
    asm("moveq r9,#0x0");
    asm("beq   create_room_in_cell_store_check");
    asm("cmp   r8,#0x0");
    asm("mov   r9,#0x1");
    asm("beq   create_room_in_cell_store_check");
    asm("mov   r0,#0x64");
    asm("bl    DungeonRandInt");
    asm("cmp   r0,#0x32");
    asm("movlt r9,#0x0");
    asm("movge r8,#0x0");
    asm("create_room_in_cell_store_check:");
    asm("cmp   r8,#0x0");
    asm("cmpeq r9,#0x0");
    asm("ldmeqia sp!,{r4,r5,r6,r7,r8,r9,pc}");
    asm("ldr   r0,[sp,#0x20]"); // Cell Ptr
    asm("strb  r8,[r0,#0x1D]"); // Secondary Structures
    asm("strb  r9,[r0,#0x1C]"); // Imperfect
    asm("ldmia sp!,{r4,r5,r6,r7,r8,r9,pc}");
}

void __attribute__((naked)) GenerateMazeBacktrackingFloor (int gridSizexX, int gridSizeY, struct floor_properties *floor_props) {
    asm("stmdb sp!,{r3,r4,r5,r6,r7,r8,r9,r10,r11,lr}");
    asm("mov   r6,r2");   // r6 = param_3 (Floor Properties)
    asm("mov   r0,#0x3"); // Yes, this is a very poor practice, but because the
    asm("mov   r1,#0x3"); // maze algorithm is recursive, I need the stack space.
    asm("bl    GenerateMazeBacktracking");
    asm("sub   sp,sp,#0x1B00");
    asm("mov   r0,#0x0"); // Zero (Number)
    asm("add   r4,sp,#0xC");
    asm("mov   r5,#0x0");
    asm("maze_rb_floor_init_room_check_loop:");
    asm("    strb  r0,[r4,r5]");
    asm("    add   r5,r5,#0x1"); // Iterate
    asm("    cmp   r5,#0x80");
    asm("    blt   maze_rb_floor_init_room_check_loop");
    asm("mov   r0,#0x3");
    asm("bl    DungeonRandInt");
    asm("ldrsb r1,[r6,#0x1]"); // Room Density
    asm("cmp   r1,#0x0");
    asm("rsblt r10,r1,#0x0"); // If negative, use abs value.
    asm("addge r10,r1,r0");
    asm("cmp   r10,#0x4"); // Less than 4 rooms fail the check for enough room
    asm("movlt r10,#0x4"); // tiles. For some reason 3 doesn't work...
    asm("cmp   r10,#0x7"); // Could do more rooms, but that would require more work...
    asm("movgt r10,#0x7"); // and 15 is enough anyway I'm sure.
    asm("add   r0,sp,#0x94");
    asm("mov   r1,#0x1");
    asm("mov   r2,r10");
    asm("bl    InitDungeonGrid");
    asm("add   r4,sp,#0x94");
    asm("mov   r5,#0x0");
    asm("mov   r11,#0x0");
    asm("maze_rb_floor_room_loop:");
    asm("    mov   r0,#0x5");
    asm("    bl    DungeonRandInt");
    asm("    mov   r7,r0");
    asm("    mov   r0,#0x5");
    asm("    bl    DungeonRandInt");
    asm("    mov   r8,r0");
    asm("    add   r1,sp,r7");
    asm("    add   r1,r1,#0xC");
    asm("    ldrb  r2,[r1,r8, lsl #0x4]"); // sp + 0xC + x + (y * 16)
    asm("    cmp   r2,#0x0");
    asm("    beq   found_room");
    asm("    add   r11,r11,#0x1");
    asm("    cmp   r11,#40"); // Try to find a room 40 times. If we can't end.
    asm("    blt   maze_rb_floor_room_loop");
    asm("    b     failsafe_small_layout");
    asm("found_room:");
    asm("mov   r9,#0x1");
    asm("cmp   r7,#0x0");
    asm("subgt r0,r1,#0x1"); // Mark this 3x3 area, and the ones around it
    asm("movle r0,r1");      // that a cell can't be placed here.
    asm("cmp   r7,#0xF");    // Yes, for the edges I store the same value
    asm("addlt r2,r1,#0x1"); // multiple times into the same slot. It was just
    asm("movge r2,r1");      // a lot more complex to check for each cell
    asm("cmp   r8,#0x0");    // coordinate was along an edge and to not store.
    asm("subgt r3,r8,#0x1");
    asm("movle r3,r8");
    asm("cmp   r8,#0x7");
    asm("addlt r12,r8,#0x1");
    asm("movge r12,r8");
    asm("strb  r9,[r0,r3, lsl #0x4]");  // Left Top
    asm("strb  r9,[r0,r8, lsl #0x4]");  // Left Middle
    asm("strb  r9,[r0,r12, lsl #0x4]"); // Left Bottom
    asm("strb  r9,[r1,r3, lsl #0x4]");  // Middle Top
    asm("strb  r9,[r1,r8, lsl #0x4]");  // Middle Middle
    asm("strb  r9,[r1,r12, lsl #0x4]"); // Middle Bottom
    asm("strb  r9,[r2,r3, lsl #0x4]");  // Right Top
    asm("strb  r9,[r2,r8, lsl #0x4]");  // Right Middle
    asm("strb  r9,[r2,r12, lsl #0x4]"); // Right Bottom
    asm("mov   r3,#0x3");
    asm("mul   r0,r7,r3");
    asm("add   r0,r0,#0x2"); // (X * 3) + 2
    asm("mul   r1,r8,r3");
    asm("add   r1,r1,#0x2"); // (Y * 3) + 2
    asm("add   r2,r0,#0x3"); // (X * 3) + 2 + 3
    asm("add   r3,r1,#0x3"); // (Y * 3) + 2 + 3
    asm("strb  r5,[sp,#0x0]");
    asm("str   r4,[sp,#0x4]");
    asm("mov   r12,#0x0");
    asm("strb  r12,[sp,#0x8]"); // No, no, no! ROOM IMPERFECTIONS!
    asm("bl    CreateRoomInCell");
    asm("add   r4,r4,#0x1E"); // DUNGEON_GRID_CELL_BYTES
    asm("add   r5,r5,#0x1");
    asm("cmp   r5,r10");
    asm("blt   maze_rb_floor_room_loop");
    asm("failsafe_small_layout:");
    asm("ldr   r7,=FLOOR_GENERATION_STATUS");
    asm("add   r0,sp,#0x94");
    asm("ldrsh r3,[r7,#0x10]");
    asm("mov   r1,#0x1");
    asm("mov   r2,r10");
    asm("bl    GenerateMonsterHouse");
    asm("ldrb  r3,[r6,#0x13]");
    asm("add   r0,sp,#0x94");
    asm("mov   r1,#0x1");
    asm("mov   r2,r10");
    asm("bl    GenerateExtraHallways");
    asm("add   r0,sp,#0x94");
    asm("mov   r1,#0x1");
    asm("mov   r2,r10");
    asm("bl    GenerateSecondaryStructures");
    asm("add   sp,sp,#0x1B00");
    asm("ldmia sp!,{r3,r4,r5,r6,r7,r8,r9,r10,r11,pc}");
}

void __attribute__((naked)) GenerateMazeBacktracking (int x, int y) {
    asm("stmdb sp!,{r4,r5,r6,r7,r8,r9,lr}");
    asm("mov   r4,r0");
    asm("mov   r5,r1");
    asm("mov   r0,#0x4");
    asm("bl    DungeonRandInt");
    asm("mov   r6,r0");   // Random Starting Direction
    asm("mov   r9,#0x0"); // Counter
    asm("maze_backtracking_loop:");
    asm("mov   r7,r4");
    asm("mov   r8,r5");
    asm("cmp   r6,#0x1");
    asm("addlt r8,r8,#0x3"); // I considered using the directions array
    asm("addeq r7,r7,#0x3"); // however, I only use the four cardinal
    asm("cmp   r6,#0x2");    // directions and have to add the number 3
    asm("subeq r7,r7,#0x3"); // which can't be shifted from the original
    asm("subgt r8,r8,#0x3"); // offset of 1. Could probably use my own
    asm("cmp   r7,#0x2");    // array of offsets for X/Y? But meh.
    asm("cmpge r8,#0x2");
    asm("blt   maze_backtracking_loop_iter");
    asm("cmp   r7,#17");
    asm("cmplt r8,#17");
    asm("bge   maze_backtracking_loop_iter");
    asm("mov   r0,r7");
    asm("mov   r1,r8");
    asm("bl    GetTileSafe");
    asm("ldrh  r1,[r0,#0x0]");
    asm("and   r1,r1,#0x3");
    asm("cmp   r1,#0x0");
    asm("bne   maze_backtracking_loop_iter");
    asm("bic   r1,r1,#0x3");
    asm("orr   r1,r1,#0x1");
    asm("strh  r1,[r0,#0x0]");
    asm("mov   r0,r4");
    asm("mov   r1,r5");
    asm("cmp   r6,#0x1");
    asm("addlt r1,r1,#0x2");
    asm("addeq r0,r0,#0x2");
    asm("cmp   r6,#0x2");
    asm("subeq r0,r0,#0x2");
    asm("subgt r1,r1,#0x2");
    asm("bl    GetTileSafe");
    asm("ldrh  r1,[r0,#0x0]");
    asm("bic   r1,r1,#0x3");
    asm("orr   r1,r1,#0x1");
    asm("strh  r1,[r0,#0x0]");
    asm("mov   r0,r4");
    asm("mov   r1,r5");
    asm("cmp   r6,#0x1");
    asm("addlt r1,r1,#0x1");
    asm("addeq r0,r0,#0x1");
    asm("cmp   r6,#0x2");
    asm("subeq r0,r0,#0x1");
    asm("subgt r1,r1,#0x1");
    asm("bl    GetTileSafe");
    asm("ldrh  r1,[r0,#0x0]");
    asm("bic   r1,r1,#0x3");
    asm("orr   r1,r1,#0x1");
    asm("strh  r1,[r0,#0x0]");
    asm("mov   r0,r4");
    asm("mov   r1,r5");
    asm("bl    GetTileSafe");
    asm("ldrh  r1,[r0,#0x0]");
    asm("bic   r1,r1,#0x3");
    asm("orr   r1,r1,#0x1");
    asm("strh  r1,[r0,#0x0]");
    asm("mov   r0,r7");
    asm("mov   r1,r8");
    asm("bl    GenerateMazeBacktracking");
    asm("maze_backtracking_loop_iter:");
    asm("add   r6,r6,#0x1");
    asm("and   r6,r6,#0x3");
    asm("add   r9,r9,#0x1");
    asm("cmp   r9,#0x4");
    asm("blt   maze_backtracking_loop");
    asm("ldmia sp!,{r4,r5,r6,r7,r8,r9,pc}");
}

void __attribute__((naked)) GenerateSidewinderFloor (int gridSizexX, int gridSizeY, struct floor_properties *floor_props) {
    asm("stmdb sp!,{r3,r4,r5,r6,r7,r8,r9,r10,r11,lr}");
    asm("sub   sp,sp,#0xAE0");
    asm("sub   sp,sp,#0x1000");
    asm("add   r0,sp,#0x80");
    asm("mov   r1,#0x1");
    asm("mov   r2,#0x2");
    asm("bl    InitDungeonGrid");
    asm("mov   r0,#2");
    asm("mov   r1,#6");
    asm("mov   r2,#52");
    asm("mov   r3,#27");
    asm("bl    GenerateSidewinderMaze");
    asm("add   r4,sp,#0x80");
    asm("str   r4,[sp,#0x4]");
    asm("mov   r12,#0x0");
    asm("strb  r12,[sp,#0x0]");
    asm("strb  r7,[sp,#0x8]");
    asm("mov   r0,#2");
    asm("mov   r1,#2");
    asm("mov   r2,#53");
    asm("mov   r3,#5");
    asm("bl    CreateRoomInCell");
    asm("add   r4,r4,#0x1E");
    asm("str   r4,[sp,#0x4]");
    asm("mov   r12,#0x1");
    asm("strb  r12,[sp,#0x0]");
    asm("strb  r7,[sp,#0x8]");
    asm("mov   r0,#2");
    asm("mov   r1,#26");
    asm("mov   r2,#53");
    asm("mov   r3,#29");
    asm("bl    CreateRoomInCell");
    asm("mov   r0,#3");
    asm("mov   r1,#51");
    asm("bl    DungeonRandRange");
    asm("mov   r1,#2");
    asm("strb  r0,[sp,#0x0]");
    asm("strb  r1,[sp,#0x1]");
    asm("ldr   r2,=DUNGEON_PTR");
    asm("ldr   r2,[r2,#0x0]");
    asm("add   r12,r2,#0xCC00");
    asm("mov   r3,#28");
    asm("strh  r0,[r12,#0xE0]");
    asm("strh  r3,[r12,#0xE2]");
    asm("mov   r0,sp");
    asm("add   r1,r2,#0x4000");
    asm("add   r1,r1,#0x00C4");
    asm("mov   r2,#0x0");
    asm("bl    SpawnStairs");
    asm("add   sp,sp,#0xAE0");
    asm("add   sp,sp,#0x1000");
    asm("ldmia sp!,{r3,r4,r5,r6,r7,r8,r9,r10,r11,pc}");
}

void __attribute__((naked)) GenerateSidewinderMaze (int xStart, int yStart, int xEnd, int yEnd) {
    asm("stmdb sp!,{r3,r4,r5,r6,r7,r8,r9,r10,r11,lr}");
    asm("mov r4,r0");
    asm("mov r5,r1");
    asm("mov r6,r2");
    asm("mov r7,r3");
    asm("sidewinder_maze_loop_y:");
    asm("cmp r5,r7");
    asm("bge end_sidewinding");
    asm("mov r8,r4");
    asm("mov r9,r4");
    asm("sidewinder_maze_loop_x:");
    asm("cmp r8,r6");
    asm("bge sidewinder_carve_north");
    asm("mov r0,#0x3");
    asm("bl  DungeonRandInt");
    asm("cmp r0,#0x2");
    asm("blt sidewinder_carve_east");
    asm("sidewinder_carve_north:");
    asm("mov r0,r9");
    asm("add r1,r8,#0x1");
    asm("bl  DungeonRandRange");
    asm("and r0,r0,#0b11111110");
    asm("mov r10,r0");
    asm("mov r1,r5");
    asm("bl  GetTileSafe");
    asm("ldrh r1,[r0,#0x0]");
    asm("bic r1,r1,#0x3");
    asm("orr r1,r1,#0x1");
    asm("strh r1,[r0,#0x0]");
    asm("mov r0,r10");
    asm("sub r1,r5,#0x1");
    asm("bl  GetTileSafe");
    asm("ldrh r1,[r0,#0x0]");
    asm("bic r1,r1,#0x3");
    asm("orr r1,r1,#0x1");
    asm("strh r1,[r0,#0x0]");
    asm("mov r0,r10");
    asm("sub r1,r5,#0x2");
    asm("bl  GetTileSafe");
    asm("ldrh r1,[r0,#0x0]");
    asm("bic r1,r1,#0x3");
    asm("orr r1,r1,#0x1");
    asm("strh r1,[r0,#0x0]");
    asm("add r9,r8,#0x2");
    asm("b sidewinder_maze_loop_x_iter");
    asm("sidewinder_carve_east:");
    asm("mov r0,r8");
    asm("mov r1,r5");
    asm("bl  GetTileSafe");
    asm("ldrh r1,[r0,#0x0]");
    asm("bic r1,r1,#0x3");
    asm("orr r1,r1,#0x1");
    asm("strh r1,[r0,#0x0]");
    asm("add r0,r8,#0x1");
    asm("mov r1,r5");
    asm("bl  GetTileSafe");
    asm("ldrh r1,[r0,#0x0]");
    asm("bic r1,r1,#0x3");
    asm("orr r1,r1,#0x1");
    asm("strh r1,[r0,#0x0]");
    asm("add r0,r8,#0x2");
    asm("mov r1,r5");
    asm("bl  GetTileSafe");
    asm("ldrh r1,[r0,#0x0]");
    asm("bic r1,r1,#0x3");
    asm("orr r1,r1,#0x1");
    asm("strh r1,[r0,#0x0]");
    asm("sidewinder_maze_loop_x_iter:");
    asm("add r8,r8,#0x2");
    asm("cmp r8,r6");
    asm("ble sidewinder_maze_loop_x");
    asm("add r5,r5,#0x2");
    asm("b sidewinder_maze_loop_y");
    asm("end_sidewinding:");
    asm("ldmia sp!,{r3,r4,r5,r6,r7,r8,r9,r10,r11,pc}");
}