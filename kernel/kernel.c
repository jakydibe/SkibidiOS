#include <stdint.h>
#include "strings.h"
#include "mem.h"
#include "interrupt/isr.h"
#include "keyboard.h"
#include "heap/heap.h"
#include "paging/paging.h"

void initialize_interrupts(){
    isr_install();
    keyboard_handler_init();
    // other handlers init
    __asm__ __volatile__("sti");
}

void kernel_main(){
    char* welcome = "Welcome from the SkibidiOS kernel\n\n";
    clear_screen();
    puts(welcome);
    
    initialize_interrupts();

    setup_paging();
    enablePaging((unsigned int) page_directory);
    
    puts("\n\n      Paging Enabled!!\n");

    while(1){}
}

