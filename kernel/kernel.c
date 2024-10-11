#include <stdint.h>
#include "strings.h"
#include "interrupt.h"


void kernel_main(){    
    char* stringa1 = "Welcome from the SkibidiOS kernel\n";
    char* stringa2 = "AntigigioGiGangster";

    clear_screen();
    
    init_idt();

    __asm__("sti");
    puts("casac");
    

    while(1){}
}
