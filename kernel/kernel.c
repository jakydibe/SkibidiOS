#include <stdint.h>
#include "utils/strings.h"
#include "utils/mem.h"
#include "interrupt/isr.h"
#include "keyboard/keyboard.h"
#include "heap/heap.h"
#include "paging/paging.h"
#include "disk/disk.h"

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

    disk_search_and_init();
    puts("\n\nReading from disk... ");
    int buffer[512];
    verbose_disk_read_sector(0x0, 0x1, buffer);
    
    hexprint(buffer[0]);
    puts("\n");
    hexprint(buffer[1]);
    puts("\n");
    hexprint(buffer[2]);

    int prova2[512] = {0xbeefdead , 0xbeefdead, 0xbeefdead};

    puts("\n\nWriting to disk... ");
    verbose_disk_write_sector(0x0, 0x1, prova2);

    int buffer2[512];
    puts("\nReading from disk... ");
    verbose_disk_read_sector(0x0, 0x1, buffer2);
    hexprint(buffer2[0]);
    puts("\n");
    hexprint(buffer2[1]);
    puts("\n");
    hexprint(buffer2[2]);
    
    while(1){}
}

