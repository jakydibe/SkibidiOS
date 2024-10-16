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


//int disk_read_sector(int lba, int total, void* buf)
//int disk_read_block(struct disk* idisk, unsigned int lba, int total, void* buf)


    disk_search_and_init();
    puts("\n\nTrying to read disk\n");
    int buffer[512];
    disk_read_sector(0x0, 0x1, buffer);
    
    hexprint(buffer[0]);
    puts("\n");
    hexprint(buffer[1]);
    puts("\n");
    hexprint(buffer[2]);

    // hexprint(buffer[4]);
    // hexprint(buffer[5]);

    int prova2[512] = {0xbeefdead , 0xbeefdead, 0xbeefdead};
//void write_to_address(uint32_t byte_address, uint8_t* data, uint32_t data_size) {

    puts("\n\nTrying to write to disk\n");
    //write_to_address(0x0, &prova2, 4);
    disk_write_sector(0x0,0x1, prova2);

    int buffer2[512];
    puts("\n");
    disk_read_sector(0x0, 0x1, buffer2);
    hexprint(buffer2[0]);
    puts("\n");
    hexprint(buffer2[1]);
    puts("\n");
    hexprint(buffer2[2]);

    
    
    while(1){}
}

