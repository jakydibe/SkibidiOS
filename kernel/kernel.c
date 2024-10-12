#include <stdint.h>
#include "strings.h"
#include "interrupt/isr.h"
#include "keyboard.h"

void initialize_interrupts(){
    isr_install();
    keyboard_handler_init();
    // other handlers init
    __asm__("sti");
}

void kernel_main(){    
    char* stringa1 = "Welcome from the SkibidiOS kernel\n";
    char* stringa2 = "AntigigioGiGangster";

    clear_screen();
    initialize_interrupts();
    puts(stringa1);
    
    while(1){}
}
