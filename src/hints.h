#include <pmdsky.h>
#include <cot.h>

void hintcpy(int aspect, int id, void* outbuf);
enum HintAspect{
    who,
    what,
    where
};

#define BYTES_PER_WHO_STRING 17
#define BYTES_PER_WHAT_STRING 33
#define BYTES_PER_WHERE_STRING 33
#define WHO_START 0x4
#define WHAT_START 0x202
#define WHERE_START 0x5E0
