#include "strings.h"

#define TXT_BLANCO 0x07 //blanco sobre negro
#define VGA_MEMORY 0xB8000
#define VGA_WIDTH 80


typedef struct{
    char carattere;
    char colore;
} VGA_CHAR;


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

void print(char* string){

    VGA_CHAR *memoria_video = (VGA_CHAR *) VGA_MEMORY;


    int lunghezza_stringa = strlen(string);

    for(int i = 80; i < (80 + lunghezza_stringa); i++){
        memoria_video[i].carattere = (char) string[i-80];
        memoria_video[i].colore = TXT_BLANCO;
    }
}


