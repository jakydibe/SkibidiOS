#define TXT_BLANCO 0x07 //blanco sobre negro
#define VGA_MEMORY 0xB8000
#define VGA_WIDTH 80
#include <stdint.h>

typedef struct{
    uint8_t carattere;
    int8_t colore;
} VGA_CHAR;

int foo(){
    return 0;
}

int boo(){
    return 1;
}

void kernel_main() {

    VGA_CHAR *memoria_video = (VGA_CHAR *) VGA_MEMORY;

    uint8_t string[] = "HELLO";
    // clean_screen();

    
    for(int i = 80; i < 86; i++){
        memoria_video[i].carattere = (uint8_t) string[i-80];
        memoria_video[i].colore = TXT_BLANCO;
    }
    
    while(1){}
    
}

// void clean_screen(){
//     VGA_CHAR *memoria_video = (VGA_CHAR *) (VGA_MEMORY);

//     for(int i = 0; i < 500; i++){
//         memoria_video[i].carattere = 0x0 ;
//         memoria_video[i].colore = 0x0;
//     }
// }
//NON FUNZIONA