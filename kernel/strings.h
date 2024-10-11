#ifndef STRINGS_H
#define STRINGS_H

#define VGA_BLINK 0b10000000

enum FG_BYTE{
    BLACK         = 0,
    BLUE          = 1,
    GREEN         = 2,
    CYAN          = 3,
    RED           = 4,
    MAGENTA       = 5,
    BROWN         = 6,
    LIGHT_GRAY    = 7,
    DARK_GRAY     = 8,
    LIGHT_BLUE    = 9,
    LIGHT_GREEN   = 10,
    LIGHT_CYAN    = 11,
    LIGHT_RED     = 12,
    LIGHT_MAGENTA = 13,
    YELLOW        = 14,
    WHITE         = 15
};

enum BG_BYTE{
    BG_BLACK      = 0,
    BG_BLUE       = 1 << 4,
    BG_GREEN      = 2 << 4,
    BG_CYAN       = 3 << 4,
    BG_RED        = 4 << 4,
    BG_MAGENTA    = 5 << 4,
    BG_BROWN      = 6 << 4,
    BG_LIGHT_GRAY = 7 << 4
};



//  0     000 0000
// BLINK   BG  FG 

typedef char VGA_STYLE ; 

int strlen(char*);
char digit_to_char(int);
void clear_screen();
void print(char*);
void putc(char);
void swag_print(char* , VGA_STYLE);

#endif