#ifndef PAGING_H
#define PAGING_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

// https://wiki.osdev.org/Paging
// pls mettete link dove prendete la roba ok
#define NUM_ENTRIES 1024  // Ogni Page Directory e Page Table contiene 1024 voci
#define PAGE_SIZE 4096  // Dimensione di una pagina standard (4 KB)

typedef struct __attribute__((packed)){
    uint32_t present    : 1;  // Bit 0: 1 se la pagina è presente in memoria
    uint32_t rw         : 1;  // Bit 1: 1 se la pagina è leggibile/scrivibile, 0 per sola lettura
    uint32_t user       : 1;  // Bit 2: 1 se accessibile da tutti, 0 solo dal kernel
    uint32_t pwt        : 1;  // Bit 3: Page-level write-through
    uint32_t pcd        : 1;  // Bit 4: Page-level cache disable
    uint32_t accessed   : 1;  // Bit 5: 1 se la pagina è stata acceduta
    uint32_t reserved   : 1;  // Bit 6: Ignorato, ma dovrebbe essere 0
    uint32_t ps         : 1;  // Bit 7: Page size (0 per 4KB, 1 per 4MB)
    uint32_t ignored    : 4;  // Bit 8-11: Ignorati dal processore
    uint32_t page_table_base : 20; // Bit 12-31: Indirizzo base della Page Table
} page_directory_entry_t ;



typedef struct __attribute__((packed)){
    uint32_t present    : 1;  // Bit 0: 1 se la pagina è presente in memoria
    uint32_t rw         : 1;  // Bit 1: 1 se leggibile/scrivibile, 0 per sola lettura
    uint32_t user       : 1;  // Bit 2: 1 se accessibile da tutti, 0 solo dal kernel
    uint32_t pwt        : 1;  // Bit 3: Page-level write-through
    uint32_t pcd        : 1;  // Bit 4: Page-level cache disable
    uint32_t accessed   : 1;  // Bit 5: 1 se la pagina è stata acceduta
    uint32_t dirty      : 1;  // Bit 6: 1 se la pagina è stata scritta
    uint32_t pat        : 1;  // Bit 7: Page attribute table (per paging avanzato)
    uint32_t global     : 1;  // Bit 8: 1 se la pagina è globale (non svuotata da TLB flush)
    uint32_t ignored    : 3;  // Bit 9-11: Ignorati dal processore
    uint32_t page_base_addr : 20; // Bit 12-31: Indirizzo base della pagina fisica
} page_table_entry_t ;




// #define PAGING_CACHE_DISABLED  0b00010000
// #define PAGING_WRITE_THROUGH   0b00001000
// #define PAGING_ACCESS_FROM_ALL 0b00000100
// #define PAGING_IS_WRITEABLE    0b00000010
// #define PAGING_IS_PRESENT      0b00000001


// #define PAGING_TOTAL_ENTRIES_PER_TABLE 1024
// #define PAGING_PAGE_SIZE 4096


// struct paging_4gb_chunk
// {
//     uint32_t* directory_entry;
// };

// typedef unsigned int PAGE;

// typedef PAGE PAGE_TABLE[1024];

// extern PAGE_TABLE page_directory[1024] __attribute__((aligned(4096)));

// Page Directory
extern page_directory_entry_t page_directory[NUM_ENTRIES] __attribute__((aligned(4096)));

// Page Table
extern page_table_entry_t first_page_table[NUM_ENTRIES] __attribute__((aligned(4096)));

void enablePaging(int *pd_addr);

//merda in C
void setup_paging();
void enable_paging(unsigned int page_directory_addr);
//void *get_physaddr(void *virtualaddr);
//void map_page(void *physaddr, void *virtualaddr, unsigned int flags);

#endif