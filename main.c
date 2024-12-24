#include <stdio.h>
#include "keyboard-driver/keyboard.h"

int main(void) {
    keyboard_init();
    putchar(key_to_ascii(keyboard_handler(0x1E)));
    return 0;
}
