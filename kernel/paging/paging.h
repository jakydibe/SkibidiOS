#ifndef PAGING_H
#define PAGING_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define NUM_ENTRIES 1024  // Ogni Page Directory e Page Table contiene 1024 voci
#define PAGE_SIZE 4096    // Dimensione di una pagina standard (4 KB)

typedef enum {
    PDE_PRESENT    = 1 << 0,  // Bit 0 - Present
    PDE_RW         = 1 << 1,  // Bit 1 - Read/Write
    PDE_USER       = 1 << 2,  // Bit 2 - User/Supervisor
    PDE_PWT        = 1 << 3,  // Bit 3 - Write-Through
    PDE_PCD        = 1 << 4,  // Bit 4 - Cache Disable
    PDE_ACCESSED   = 1 << 5,  // Bit 5 - Accessed
    PDE_PS         = 1 << 7   // Bit 7 - Page Size (4MB if set, 4KB otherwise)
    // Bits 6, 8-11 are reserved/ignored
} PDE_FLAGS;

typedef enum {
    PTE_PRESENT    = 1 << 0,  // Bit 0 - Present
    PTE_RW         = 1 << 1,  // Bit 1 - Read/Write
    PTE_USER       = 1 << 2,  // Bit 2 - User/Supervisor
    PTE_PWT        = 1 << 3,  // Bit 3 - Write-Through
    PTE_PCD        = 1 << 4,  // Bit 4 - Cache Disable
    PTE_ACCESSED   = 1 << 5,  // Bit 5 - Accessed (set by the CPU when the page is accessed)
    PTE_DIRTY      = 1 << 6,  // Bit 6 - Dirty (set by the CPU when the page is written to)
    PTE_PAT        = 1 << 7,  // Bit 7 - Page Attribute Table (affects memory caching behavior)
    PTE_GLOBAL     = 1 << 8,  // Bit 8 - Global (TLB flush does not affect this page if set)
    // Bit 9-11 - Ignored (software use)
} PTE_FLAGS;

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
typedef union __attribute__((packed)) {
    struct {
        uint32_t present    : 1; 
        uint32_t rw         : 1;
        uint32_t user       : 1;
        uint32_t pwt        : 1;
        uint32_t pcd        : 1;
        uint32_t accessed   : 1;
        uint32_t reserved   : 1;
        uint32_t ps         : 1;
        uint32_t ignored    : 4;
        uint32_t : 20;
    };

    struct {
        uint32_t flags : 12;
        uint32_t page_table_addr : 20; // 20 bit alti dell'indirizzo della Page Table (12 bit bassi sono 0, la Page Table è allineata a 4096)
    };
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

void setup_paging();
void map_virtual_to_physical(uint32_t virtual_addr, uint32_t physical_addr, uint32_t flags);
void map_memory_region(uint32_t virtual_addr, uint32_t physical_addr, uint32_t size, uint32_t flags);
uint32_t get_physical_address(uint32_t virtual_addr);

#endif