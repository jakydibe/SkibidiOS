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

    // write on index 0x25000
    unsigned int PA = 0xaaa000;
    *((int *) PA) = 0xdeadbeef;

    unsigned int VA = 0x600000; //1 tabella ->1024 * 1024 * 4096
    *((int *) VA) = 0xbadc0ffe;

    setup_paging();
    
    puts("\nPaging Enabled!!\n");

    unsigned int flags = 0b11;
    map_virtual_to_physical(VA, PA, flags);
    //asm volatile("mov %0, %%cr3" : : "r" (page_directory));
    //asm volatile("invlpg (%0)" ::"r" (VA) : "memory");

    // Verify that the virtual address maps to the correct physical address
    unsigned int prova = get_physical_address(VA);
    puts("PA ricavato:");
    hexprint(prova);
    putc('\n');
    puts("PA orig:");
    hexprint(PA);

    if (get_physical_address(VA) == PA) {
        puts("\nMapping successful!\n");
    } else {
        puts("\nMapping failed\n");
    }

    puts("Testing dereferencing of the virtual address\n");
    int value = *((int *) VA);
    hexprint(value);  // Should print 0xdeadbeef

    while(1){}
}

