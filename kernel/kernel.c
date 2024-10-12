#include <stdint.h>
#include "strings.h"
#include "mem.h"
#include "interrupt/isr.h"
#include "keyboard.h"
#include "heap/heap.h"


void initialize_interrupts(){
    isr_install();
    keyboard_handler_init();
    // other handlers init
    __asm__("sti");
}

void kernel_main(){    
    char* welcome = "Welcome from the SkibidiOS kernel\n";
    // char* stringa2 = "AntigigioGiGangster";
    //memcpy(welcome, string3, 35);
    clear_screen();
    initialize_interrupts();
    char* stringa = (char*)malloc(21);
    //void memcpy(char *source, char *dest, int nbytes) {
    memcpy("AntigigioGiGangster\n", stringa, 21);
    hexprint(stringa);
    puts(stringa);

    putc('\n');
    
    free(stringa);

    char* string2 = (char*)malloc(35);
    memcpy(welcome, string2, 35);
    puts(string2);
    hexprint(string2);
    //prova free
    while(1){}
}

