#include "paging.h"
#include "../heap/heap.h"
#include "../utils/mem.h"

// Asm routine to enable paging
void enablePaging(unsigned int pd_addr);

page_directory_entry_t page_directory[NUM_ENTRIES] __attribute__((aligned(4096)));
page_table_entry_t first_page_table[NUM_ENTRIES] __attribute__((aligned(4096)));

// forse va spostata
void map_heap(){
    const uint32_t heap_pd_entry_number = (HEAP_SIZE+NUM_ENTRIES*PAGE_SIZE-1)>>22;
    const uint32_t heap_pd_index = HEAP_BASE>>22;

    page_table_entry_t *table;
    for (uint32_t i = 0; i < heap_pd_entry_number; i++) {
        table = malloc(NUM_ENTRIES * sizeof(page_table_entry_t));

        for(uint32_t j = 0; j < NUM_ENTRIES; j++){
            table[j].physical_addr = HEAP_BASE>>12 + NUM_ENTRIES*i + j;
            table[j].flags = PTE_RW | PTE_PRESENT; 
        }

        page_directory[heap_pd_index+i].page_table_addr = (uint32_t) table >> 12; 
        page_directory[heap_pd_index+i].flags = PDE_RW | PDE_PRESENT;
    }
}

void setup_paging() {
    // Inizializza il Page Directory
    memset(page_directory, 0, NUM_ENTRIES * sizeof(page_directory_entry_t)); // Disabilita tutte le entry all'inizio
    
    // Inizializza la prima Page Table (identity mapping, mappa 1:1 indirizzi fisici e virtuali)
    // Mapping of the first 0x400000 adresses
    for (uint32_t i = 0; i < NUM_ENTRIES; i++) {
        first_page_table[i].physical_addr = i;
        first_page_table[i].flags = PTE_RW | PTE_PRESENT;
    }

    // Collegare la prima Page Table al Page Directory
    page_directory[0].page_table_addr = (uint32_t) first_page_table >> 12; 
    page_directory[0].flags = PDE_RW | PDE_PRESENT;

    // Map heap
    map_heap();

    enablePaging((uint32_t) page_directory);
}

void map_virtual_to_physical(uint32_t virtual_addr, uint32_t physical_addr, uint32_t flags){
    uint32_t page_dir_index = (virtual_addr >> 22) & 0x3FF;
    uint32_t page_table_index = (virtual_addr >> 12) & 0x3FF;

    page_table_entry_t *page_table;
    if (!page_directory[page_dir_index].present){ // alloca
        page_table = (page_table_entry_t *) malloc(sizeof(page_table_entry_t));
        memset(page_table, 0, sizeof(page_table_entry_t));
        page_directory[page_dir_index].page_table_addr = (uint32_t) page_table >> 12;
        page_directory[page_dir_index].present = 1;
    } else {
        page_table = (page_table_entry_t *) (page_directory[page_dir_index].page_table_addr << 12);
    }

    page_table[page_table_index].physical_addr = physical_addr >> 12;
    page_table[page_table_index].flags = flags;
    page_table[page_table_index].present = 1;
}

void map_memory_region(uint32_t virtual_addr, uint32_t physical_addr, uint32_t size, uint32_t flags){
    for(int i = 0; i < size + PAGE_SIZE - 1; i+=PAGE_SIZE){
        map_virtual_to_physical(virtual_addr + i, physical_addr + i, flags);
    }
}

uint32_t get_physical_address(uint32_t virtual_addr){
    uint32_t page_dir_index = (virtual_addr >> 22) & 0x3FF;
    uint32_t page_table_index = (virtual_addr >> 12) & 0x3FF;

    if (!page_directory[page_dir_index].present){
        return 0;
    }

    page_table_entry_t *page_table = (page_table_entry_t *) (page_directory[page_dir_index].page_table_addr << 12);

    if (!page_table[page_table_index].present){
        return 0;
    }

    return (page_table[page_table_index].physical_addr << 12) | (virtual_addr & 0xFFF);
}