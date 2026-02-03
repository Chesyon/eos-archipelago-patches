#include <pmdsky.h>
#include <cot.h>
#include "custom_save_area.h"
#include "hints.h"

bool __attribute__((used)) HandleLowercaseQTag(const char* tag_string, char* buf, int tag_param_count, const char* tag_string_param)
{
    int index;
    if(StrcmpTag(tag_string, "qho"))
    {
        index = AtoiTag(tag_string_param);
        hintcpy((int)who, index, buf);
        CUSTOM_SAVE_AREA.hintedItems[index] = true;
        return true;
    }
    if(StrcmpTag(tag_string, "qhat"))
    {
        index = AtoiTag(tag_string_param);
        hintcpy((int)what, index, buf);
        return true;
    }
    if(StrcmpTag(tag_string, "qhere"))
    {
        index = AtoiTag(tag_string_param);
        hintcpy((int)where, index, buf);
        return true;
    }
    if(StrcmpTag(tag_string, "quag"))
    {
        int num;
        if(AtoiTag(tag_string_param) == 1) num = CUSTOM_SAVE_AREA.acquiredInstruments;
        else num = CUSTOM_SAVE_AREA.acquiredRelicFragmentShards;
        sprintf(buf, "%d", num);
        return true;
    }
    if(StrcmpTag(tag_string, "qbby"))
    {
        char temp_buf[33];
        MemZero(temp_buf, 33);
        uint32_t tag_param = AtoiTag(tag_string_param);
        enum script_var_id var = tag_param & 0xFF;
        uint8_t first_bit = (tag_param >> 8) & 0xFFF;
        uint8_t num_bits = (tag_param >> 20);
        for(int i = 0; i < num_bits; i++){
            if(LoadScriptVariableValueAtIndex(NULL, var, first_bit+i) != 0) temp_buf[i] = 0x31;
            else temp_buf[i] = 0x30;
        }
        strcpy(buf, temp_buf);
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
