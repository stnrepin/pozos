#include "kern/kernel.h"
#include "kern/printf.h"

int user_main(void) {
    printf("Hello World\n");
    return 0;
}
