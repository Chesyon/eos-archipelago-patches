#include <pmdsky.h>
#include <cot.h>
#include "extern.h"
#include "hints.h"

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
    char fullString[43];
    int offset;
    int length;
    switch(aspect){
        case who:
            offset = WHO_START + (id * BYTES_PER_WHO_STRING);
            length = BYTES_PER_WHO_STRING;
            strcpy(fullString, "[CS:N]");
            break;
        case what:
            offset = WHAT_START + (id * BYTES_PER_WHAT_STRING);
            length = BYTES_PER_WHAT_STRING;
            strcpy(fullString, "[CS:I]");
            break;
        case where:
            offset = WHERE_START + (id * BYTES_PER_WHERE_STRING);
            length = BYTES_PER_WHERE_STRING;
            strcpy(fullString, "[CS:P]");
            break;
        default:
            offset = 0;
            length = 0;
            break;
    }
    MemZero(buf, 33);
    CopyFromHintables(buf, offset, length);
    strcat(fullString, buf);
    strcat(fullString, "[CR]");
    strcpy(outbuf, fullString);
}