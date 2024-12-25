#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdio.h>

#define SYMBOLS_TABLE_SIZE 256
#define SYMBOLS_TABLE_TYPES 3

#define CTRL_CODE 0x1D
#define SHIFT_CODE 0x2A
#define ALT_CODE 0x38
#define CAPSLOCK_CODE 0x3A

#define RELEASE_CODE 0x80

typedef struct __attribute__((packed)){
    unsigned char is_ctrl_pressed;
    unsigned char is_shift_pressed;
    unsigned char is_capslock_pressed;
    unsigned char is_alt_pressed;
} keyboard_mdata;

typedef struct __attribute__((packed)){
    unsigned char scancode;
    keyboard_mdata data;
} key_event;

//keyboard symbols list
extern char symbols[SYMBOLS_TABLE_TYPES][SYMBOLS_TABLE_SIZE];
extern keyboard_mdata m_keyboard;

void keyboard_init();
key_event keyboard_handler(unsigned char code);
void init_base_symbols(char* symbols);
void init_capital_symbols(char* symbols);
void init_number_signs_symbols(char* symbols);
int is_small_letter(char ch);
char key_to_ascii(key_event key);
int is_ascii(char ch);

#endif
