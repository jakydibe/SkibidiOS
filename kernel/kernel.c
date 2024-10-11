
#include <stdint.h>
#include "strings.h"


__attribute__((section(".text")))
int foo(){
    return 0;
}

__attribute__((section(".text")))
int boo(){
    return 1;
}









__attribute__((section(".text")))
void kernel_main() {

    char* stringa = "Welcome to the SkibidiOS kernel";

    print(stringa);

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