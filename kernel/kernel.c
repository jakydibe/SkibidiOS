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
    puts("\nPaging Enabled!!\n");

    //Map heap
    //map_virtual_to_physical(HEAP_BASE, HEAP_BASE, 0b11);
    
    unsigned int VA = 0x19000; //1 tabella ->1024 * 1024 * 4096
    unsigned int PA = 0x25000;
    unsigned int flags = 0b11;

    map_virtual_to_physical(VA, PA, flags);

    // unsigned int prova = get_physical_address(VA);
    // puts("PA ricavato:");
    // hexprint(prova);
    // putc('\n');
    // puts("PA orig:");
    // hexprint(PA);

    // if(get_physical_address(VA) == PA){
    //     puts("\nezez\n");
    // }

    while(1){}
}

