#include "kern/string.h"

void *memset(void *dst, int ch, size_t count)
{
    size_t i;
    char *cdst = (char *)dst;

    for (i = 0; i < count; i++) {
        cdst[i] = ch;
    }
    return dst;
}
