#include <stdint.h>
#include "strings.h"
#include "keyboard.h"
#include "interrupt/isr.h"
#include "io/io.h"


void kernel_main(){    
    char* stringa1 = "Welcome from the SkibidiOS kernel\n";
    char* stringa2 = "AntigigioGiGangster";

    clear_screen();
    
    isr_install();

    __asm__("sti");
    keyboard_handler_init();
    puts(stringa1);
    

    while(1){}
}
