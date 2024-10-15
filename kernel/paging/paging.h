#ifndef PAGING_H
#define PAGING_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define NUM_ENTRIES 1024  // Ogni Page Directory e Page Table contiene 1024 voci
#define PAGE_SIZE 4096    // Dimensione di una pagina standard (4 KB)

/* Page Directory Entry flags
                                               1                    0
    bit 0 : (P)   | Present         | presente in memoria  | non presente in memoria |
    bit 1 : (RW)  | Read/Write      | leggibile/scrivibile | sola lettura            |
    bit 2 : (US)  | User/Supervisor | accessibile da tutti | solo kernel             |
    bit 3 : (PWT) | Write-Trhough   |          ?           |            ?            |
    bit 4 : (PCD) | Cache Disable   |          ?           |            ?            |
    bit 5 : (A)   | Accessed        | è stata access       | non è stata accessa     |
    bit 6 :   -   |        -        |          -           |            -            |
    bit 7 : (PS)  | Page Size       | page size 4MB        | page size 4KB           |
    bit 8 :   -   |        -        |          -           |            -            |
    bit 9 :   -   |        -        |          -           |            -            |
    bit 10:   -   |        -        |          -           |            -            |
    bit 11:   -   |        -        |          -           |            -            |
*/
typedef struct __attribute__((packed)) {
    uint32_t present    : 1; 
    uint32_t rw         : 1;
    uint32_t user       : 1;
    uint32_t pwt        : 1;
    uint32_t pcd        : 1;
    uint32_t accessed   : 1;
    uint32_t reserved   : 1;
    uint32_t ps         : 1;
    uint32_t ignored    : 4;  
    uint32_t page_table_addr : 20; // 20 bit alti dell'indirizzo della Page Table (12 bit bassi sono 0, la Page Table è allineata a 4096)
} page_directory_entry_t;

/* Page Directory Entry flags
                                               1                    0
    bit 0 : (P)   | Present              | presente in memoria  | non presente in memoria |
    bit 1 : (RW)  | Read/Write           | leggibile/scrivibile | sola lettura            |
    bit 2 : (US)  | User/Supervisor      | accessibile da tutti | solo kernel             |
    bit 3 : (PWT) | Write-Trhough        |          ?           |            ?            |
    bit 4 : (PCD) | Cache Disable        |          ?           |            ?            |
    bit 5 : (A)   | Accessed             | è stata access       | non è stata accessa     |
    bit 6 : (D)   | Dirty                | pagina scritta       | non scritta             |
    bit 7 : (PAT) | Page Attribute Table |          ?           |            ?            |
    bit 8 : (G)   | Global               |          ?           |            ?            |
    bit 9 :   -   |          -           |          -           |            -            |
    bit 10:   -   |          -           |          -           |            -            |
    bit 11:   -   |          -           |          -           |            -            |
*/
typedef union __attribute__((packed)){
    struct {
        uint32_t present    : 1;
        uint32_t rw         : 1;
        uint32_t user       : 1;
        uint32_t pwt        : 1;
        uint32_t pcd        : 1;
        uint32_t accessed   : 1;
        uint32_t dirty      : 1;
        uint32_t pat        : 1;
        uint32_t global     : 1;  // 1 se la pagina è globale (non svuotata da TLB flush)
        uint32_t ignored    : 3;
        uint32_t : 20;
    };

    struct {
        uint32_t flags : 12;
        uint32_t physical_addr : 20; // Bit 12-31: Indirizzo base della pagina fisica
    };
} page_table_entry_t ;

//merda in C
void setup_paging();
void map_virtual_to_physical(uint32_t virtual_addr, uint32_t physical_addr, uint32_t flags);
void map_memory_region(uint32_t virtual_addr, uint32_t physical_addr, uint32_t size, uint32_t flags);
uint32_t get_physical_address(uint32_t virtual_addr);

#endif