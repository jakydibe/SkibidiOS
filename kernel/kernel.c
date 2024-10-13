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
    char* welcome = "Welcome from the SkibidiOS kernel\n";

    clear_screen();
    initialize_interrupts();
    hexprint(sizeof(long));

    puts("\npd: ");
    hexprint(page_directory);
    puts("\nasm pd: ");
    enablePaging((int*)page_directory);
    map_page(0x1001000, 0xDEADBEEF, 0x003);
    void* addr = get_physaddr(0xDEADBEEF);
    hexprint(addr);
    puts("\n");
    while(1){}
}

