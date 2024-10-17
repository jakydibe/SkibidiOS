#include <stdint.h>
#include "utils/strings.h"
#include "utils/mem.h"
#include "interrupt/isr.h"
#include "keyboard/keyboard.h"
#include "heap/heap.h"
#include "paging/paging.h"
#include "disk/disk.h"

#define START_DISK_EMPTY_SECTORS 2048

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

    // Testing disk
    disk_search_and_init();

    // char buffer[512];
    // verbose_disk_read_sector(START_DISK_EMPTY_SECTORS, 0x1, buffer);
    // puts("data:\n    ");
    // hexprint(*((int *) buffer));
    // puts("\n    ");
    // hexprint(*((int *) buffer + 1));
    // puts("\n    ");
    // hexprint(*((int *) buffer + 2));
    // puts("\n\n");

    // char data[512] = "ABCDEFGHILMNOPQRSTUVZ";
    // puts("writing ");
    // puts(data);
    // puts("\n");
    // verbose_disk_write_sector(START_DISK_EMPTY_SECTORS, 0x1, data);

    // verbose_disk_read_sector(START_DISK_EMPTY_SECTORS, 0x1, buffer);
    // puts("data:\n    ");
    // hexprint(*((int *) buffer));
    // puts("\n    ");
    // hexprint(*((int *) buffer + 1));
    // puts("\n    ");
    // hexprint(*((int *) buffer + 2));
    // puts("\n\n");

    int cacca_buffer[4][128];
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 128; j++){
            cacca_buffer[i][j] = 0xcacca;
        }
    }
    disk_write_sector(START_DISK_EMPTY_SECTORS, 0x4, cacca_buffer);

    int buffer_addr[3][128] = {
        {0xdeadbeef, 0xdeadbeef, 0xdeadbeef, 0xdeadbeef},
        {0xbadc0ffe, 0xbadc0ffe, 0xbadc0ffe, 0xbadc0ffe},
        {0xfaceface, 0xfaceface, 0xfaceface, 0xfaceface},
    };

    disk_write_to_addr(START_DISK_EMPTY_SECTORS*0x200 + 0x32, 0x600, buffer_addr);
    
    int buffer_check[4][128];
    disk_read_sector(START_DISK_EMPTY_SECTORS, 0x4, buffer_check);

    for(int sec = 0; sec < 4; sec++){
        for(int i = 0; i < 25; i++){
            hexprint(buffer_check[sec][i]);
            puts("-");
        }
        puts("\n");
    }
    
    while(1){}
}

