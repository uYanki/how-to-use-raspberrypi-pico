#include <stdio.h>
#include "pico/stdlib.h"

int main()
{
    stdio_init_all();

    while (1) {
        puts("Hello, world!");
        sleep_ms(1000);
    }

    return 0;
}
