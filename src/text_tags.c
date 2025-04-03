#include <pmdsky.h>
#include <cot.h>
#include "extern.h"

bool __attribute__((used)) HandleLowercaseQTag(const char* tag_string, char* buf, int tag_param_count, const char* tag_string_param)
{
    int index;
    if(StrcmpTag(tag_string, "qwoop"))
    {
        index = AtoiTag(tag_string_param);
        strcpy(buf, hintableItems.names[index]);
        CUSTOM_SAVE_AREA.hintedItems[index] = true;
        return true;
    }
    if(StrcmpTag(tag_string, "qho"))
    {
        strcpy(buf, CUSTOM_SAVE_AREA.dimensionalScreamInfo.who);
        return true;
    }
    if(StrcmpTag(tag_string, "qhat"))
    {
        strcpy(buf, CUSTOM_SAVE_AREA.dimensionalScreamInfo.what);
        return true;
    }
    if(StrcmpTag(tag_string, "qhere"))
    {
        strcpy(buf, CUSTOM_SAVE_AREA.dimensionalScreamInfo.where);
        CUSTOM_SAVE_AREA.dimensionalScreamInfo.recieved = true;
        return true;
    }
    return false;
}

void __attribute__((naked)) NoLowercaseQTagFound()
{
    asm("ldr r0,[r13,#0xB4]");
    asm("add r1,r13,#0x1C8");
    asm("mov r2,r6");
    asm("ldr r3,[r13,#0xB8]");
    asm("bl HandleLowercaseQTag");
    asm("cmp r0,#0");
    asm("beq LowercaseTagCodeError");
    asm("add r7,r13,#0x1C8");
    asm("b AfterLowercaseTagIsFound");
}
