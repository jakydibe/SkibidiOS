
#include <stdint.h>  // Per tipi di dati come uint16_t
#define TXT_BLANCO 0x07 //blanco sobre negro
#define VGA_MEMORY (uint16_t*) 0xB8000
#define VGA_WIDTH 80

//bho non printa ma esegue (se tolgo il while crasha)

//non riesce a definire sta struct

//syntax error, si


// 0x000b8000: 'H', colour_for_H
// 0x000b8002: 'e', colour_for_e
// 0x000b8004: 'L', colour_for_L
// 0x000b8006: 'l', colour_for_l
// 0x000b8008: 'o', colour_for_o

typedef struct {
    char carattere;
    unsigned char colore;
} VGA_CHAR;

void kernel_main() {
    const char* string = "Hello, World!";


    char* memoria_video = VGA_MEMORY;

    *memoria_video = 'X';

    while(1){}
}

/*

ja a te non lo fa vero?
questo funziona nel chill ahahahhahahah //mo committo che funziona sucate
make                   2 ✘ 
gcc -ffreestanding -c kernel.c -o kernel.o
ld -o kernel.bin -Ttext 0x1000 kernel.o --oformat binary
ld: warning: cannot find entry symbol _start; defaulting to 0000000000001000
cat boot.bin kernel.bin > os-image

*/


// $gcc -ffreestanding -c kernel.c -o kernel.o
// $ld -o kernel.bin -Ttext 0x9000 kernel.o --oformat binary
// cat boot.bin kernel.bin > os-image