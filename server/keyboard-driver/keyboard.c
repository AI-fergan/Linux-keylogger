#include "keyboard.h"

//keyboard symbols list
char symbols[SYMBOLS_TABLE_TYPES][SYMBOLS_TABLE_SIZE];

//keyboard metadata
keyboard_mdata m_keyboard;

void keyboard_init() {
    init_base_symbols(symbols[0]);
    init_capital_symbols(symbols[1]);
    init_number_signs_symbols(symbols[2]);

    m_keyboard.is_alt_pressed = 0;
    m_keyboard.is_capslock_pressed = 0;
    m_keyboard.is_ctrl_pressed = 0;
    m_keyboard.is_shift_pressed = 0;
}

void init_base_symbols(char* symbols) {
    symbols[0x02] = '1'; 
    symbols[0x03] = '2'; 
    symbols[0x04] = '3';
    symbols[0x05] = '4'; 
    symbols[0x06] = '5'; 
    symbols[0x07] = '6';
    symbols[0x08] = '7'; 
    symbols[0x09] = '8'; 
    symbols[0x0A] = '9';
    symbols[0x0B] = '0';
    symbols[0x1E] = 'a'; 
    symbols[0x30] = 'b'; 
    symbols[0x2E] = 'c';
    symbols[0x20] = 'd';  
    symbols[0x12] = 'e'; 
    symbols[0x21] = 'f';
    symbols[0x22] = 'g'; 
    symbols[0x23] = 'h'; 
    symbols[0x17] = 'i';
    symbols[0x24] = 'j';
    symbols[0x25] = 'k';
    symbols[0x26] = 'l';  
    symbols[0x32] = 'm';
    symbols[0x31] = 'n'; 
    symbols[0x18] = 'o'; 
    symbols[0x19] = 'p'; 
    symbols[0x10] = 'q'; 
    symbols[0x13] = 'r'; 
    symbols[0x1F] = 's'; 
    symbols[0x14] = 't';
    symbols[0x16] = 'u'; 
    symbols[0x2F] = 'v';
    symbols[0x11] = 'w';
    symbols[0x2D] = 'x';
    symbols[0x15] = 'y';  
    symbols[0x2C] = 'z';
    symbols[0x0C] = '-'; 
    symbols[0x0D] = '=';
    symbols[0x1A] = '[';
    symbols[0x1B] = ']'; 
    symbols[0x1C] = '\n'; 
    symbols[0x27] = ';';
    symbols[0x28] = '\''; 
    symbols[0x29] = '~'; 
    symbols[0x33] = ','; 
    symbols[0x34] = '.'; 
    symbols[0x35] = '/';
    symbols[0x39] = ' ';
    symbols[0x0d] = '\0';
}

void init_capital_symbols(char* symbols){
    symbols[0x1E] = 'A'; 
    symbols[0x30] = 'B'; 
    symbols[0x2E] = 'C';
    symbols[0x20] = 'D';  
    symbols[0x12] = 'E'; 
    symbols[0x21] = 'F';
    symbols[0x22] = 'G'; 
    symbols[0x23] = 'H'; 
    symbols[0x17] = 'I';
    symbols[0x24] = 'J';
    symbols[0x25] = 'K';
    symbols[0x26] = 'L';  
    symbols[0x32] = 'M';
    symbols[0x31] = 'N'; 
    symbols[0x18] = 'O'; 
    symbols[0x19] = 'P'; 
    symbols[0x10] = 'Q'; 
    symbols[0x13] = 'R'; 
    symbols[0x1F] = 'S'; 
    symbols[0x14] = 'T';
    symbols[0x16] = 'U'; 
    symbols[0x2F] = 'V';
    symbols[0x11] = 'W';
    symbols[0x2D] = 'X';
    symbols[0x15] = 'Y';  
    symbols[0x2C] = 'Z';
}

void init_number_signs_symbols(char* symbols) {
    symbols[0x0] = ')';
    symbols[0x1] = '!';
    symbols[0x2] = '@';
    symbols[0x3] = '#';
    symbols[0x4] = '$';
    symbols[0x5] = '%';
    symbols[0x6] = '^';
    symbols[0x7] = '&';
    symbols[0x8] = '*';
    symbols[0x9] = '(';
}

int is_small_letter(char ch) {
    if(ch >= 'a' && ch <= 'z')
        return 1;
    return 0;
}

int is_number(char ch) {
    if(ch >= '0' && ch <= '9')
        return 1;
    return 0;
}

int is_special_char(char ch) {
    if(ch == '-' ||
       ch == '=' ||
       ch == '[' ||
       ch == ']' ||
       ch == '\n'||
       ch == ';' ||
       ch == '\\'||
       ch == '~' ||
       ch == ',' || 
       ch == '.' ||
       ch == '/' ||
       ch == ' ')
       return 1;
    return 0;
}

int update_metadata(unsigned char code) {
    /* Check if one of the control key is pressed: */
    if(code == CTRL_CODE)
        return m_keyboard.is_ctrl_pressed = 1;
    if(code == SHIFT_CODE)
        return m_keyboard.is_shift_pressed = 1;
    if(code == ALT_CODE)
        return m_keyboard.is_alt_pressed = 1;
    if(code == CAPSLOCK_CODE)
        return m_keyboard.is_capslock_pressed ^= 1; 
        /* the CAPSLOCK key is based on single press 
           each time so we dont wate to the relase we 
           wate to the next press. */
    
    /* Check if one of the control keys is released: */
    if(code == (CTRL_CODE | RELEASE_CODE))
        return !(m_keyboard.is_ctrl_pressed = 0);
    if(code == (SHIFT_CODE | RELEASE_CODE))
        return !(m_keyboard.is_shift_pressed = 0);
    if(code == (ALT_CODE | RELEASE_CODE))
        return !(m_keyboard.is_alt_pressed = 0);
    
    /* The key that was presed / released isnt control key */
    return 0;
}


key_event keyboard_handler(unsigned char code) {
    key_event event;
    event.scancode = -1;

    //back key
    if(code == 0x0E)
        return event;

    //update ctrl/shift/alt/capslock
    if(update_metadata(code))
        return event;

    if(is_small_letter(symbols[0][code]) || is_number(symbols[0][code]) || is_special_char(symbols[0][code])) {
        event.scancode = code; //get key scancode and store the key event
        event.data = m_keyboard;
    }

    return event;
}

char key_to_ascii(key_event key) {
    char sign = symbols[0][key.scancode];
    if(is_number(sign) && key.data.is_shift_pressed){
        return symbols[2][sign - '0'];        
    }
    if(is_small_letter(sign) && (key.data.is_capslock_pressed || key.data.is_shift_pressed))
        return symbols[1][key.scancode];

    return sign;
}
