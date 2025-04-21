#include <pmdsky.h>
#include <cot.h>
#include "extern.h"
#include "hints.h"

#define NUM_SHOP_ITEMS 10
#define NUM_DIMENSIONAL_SCREAMS 30
#define NUM_HINTABLES NUM_SHOP_ITEMS + NUM_DIMENSIONAL_SCREAMS
#define BYTES_PER_WHO_STRING 17
#define BYTES_PER_WHAT_STRING 33
#define BYTES_PER_WHERE_STRING 33
#define WHO_START 0x4
#define WHAT_START (WHO_START + (BYTES_PER_WHO_STRING * NUM_HINTABLES))
#define WHERE_START (WHAT_START + (BYTES_PER_WHAT_STRING * NUM_HINTABLES))

#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2
void CopyFromHintables(void* buf, int offset, int length){
    struct file_stream file;
    DataTransferInit();
    FileInit(&file);
    FileOpen(&file, "hintables.bin");
    FileSeek(&file, offset, SEEK_SET);
    FileRead(&file, buf, length);
    FileClose(&file);
    DataTransferStop();
}

void hintcpy(int aspect, int id, void* outbuf){
    char buf[33];
    int offset;
    int length;
    switch(aspect){
        case who:
            offset = WHO_START + (id * BYTES_PER_WHO_STRING);
            length = BYTES_PER_WHO_STRING;
            break;
        case what:
            offset = WHAT_START + (id * BYTES_PER_WHAT_STRING);
            length = BYTES_PER_WHAT_STRING;
            break;
        case where:
            offset = WHERE_START + (id * BYTES_PER_WHERE_STRING);
            length = BYTES_PER_WHERE_STRING;
            break;
        default:
            offset = 0;
            length = 0;
            break;
    }
    MemZero(buf, 33);
    CopyFromHintables(buf, offset, length);
    strcpy(outbuf, buf);
}