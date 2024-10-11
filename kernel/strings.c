#include "strings.h"

#define TXT_BLANCO 0x0F //blanco sobre negro
#define VGA_MEMORY 0xB8000
#define VGA_WIDTH 80
#define VGA_HEIGHT 25

typedef struct{
    char chr;
    VGA_STYLE style;
} VGA_CHAR;

int cursorX;
int cursorY;

VGA_CHAR (*memoria_video)[VGA_WIDTH] = (VGA_CHAR (*)[VGA_WIDTH]) VGA_MEMORY;

int strlen(char* string){
    int i = 0;
    while(string[i] != '\0'){
        i++;
    }
    if (i == 0){
        return -1;
    } 
    else{
        return i;
    }
}

char digit_to_char(int digit){
    return digit + '0';
}

void putc(char chr){

    if (chr == '\n'){
        cursorY++;
        cursorX=0;
    }else{
        memoria_video[cursorY][cursorX].chr = chr;
        memoria_video[cursorY][cursorX].style = TXT_BLANCO;
        cursorX++;
    }
    
    
    if (cursorX > 80){
        cursorX = cursorX % 80;
        cursorY += 1;  
    }
    
}


void puts(char* string){
    for(int i = 0; string[i] != 0x0; cursorX++, i++){
        if (string[i] == '\n'){
            cursorY++;
            cursorX=-1;
            continue;
        }

        memoria_video[cursorY][cursorX].chr = string[i];
        memoria_video[cursorY][cursorX].style = TXT_BLANCO;
        
        if (cursorX > 80){
            cursorX = cursorX % 80;
            cursorY += 1;  
        }
    } 
}

void swag_print(char* string, VGA_STYLE style){
    for(int i = 0; string[i] != 0x0; cursorX++, i++){
        if (string[i] == '\n'){
            cursorY++;
            cursorX=-1;
            continue;
        }

        memoria_video[cursorY][cursorX].chr = string[i];
        memoria_video[cursorY][cursorX].style = style;
        
        if (cursorX > 80){
            cursorX = cursorX % 80;
            cursorY += 1;  
        }
    } 
}

void clear_screen(){
    for (int i = 0; i < VGA_HEIGHT; i++){
        for (int j = 0; j < VGA_WIDTH; j++){
            memoria_video[i][j].chr = ' ';
            memoria_video[i][j].style = 0x0;
        }
    }  
}


void int_to_ascii(int n, char str[]) {
    int i, sign;
    if ((sign = n) < 0) n = -n;
    i = 0;
    do {
        str[i++] = n % 10 + '0';
    } while ((n /= 10) > 0);

    if (sign < 0) str[i++] = '-';
    str[i] = '\0';

    /* TODO: implement "reverse" */
}
