#include "paging.h"
#include "../heap/heap.h"
#include "../mem.h"

page_directory_entry_t page_directory[NUM_ENTRIES] __attribute__((aligned(4096)));
page_table_entry_t first_page_table[NUM_ENTRIES] __attribute__((aligned(4096)));

void set_page_directory_entry(page_directory_entry_t* entry, uint32_t page_table_base_addr, int present, int rw, int user) {
    entry->present = present;
    entry->rw = rw;
    entry->user = user;
    entry->page_table_addr = page_table_base_addr >> 12;  // L'indirizzo è allineato a 4 KB, quindi sposta 12 bit a destra
}

// Funzione per configurare una singola Page Table Entry
void set_page_table_entry(page_table_entry_t* entry, uint32_t page_base_addr, int present, int rw, int user) {
    entry->present = present;
    entry->rw = rw;
    entry->user = user;
    entry->physical_addr = page_base_addr >> 12;  // L'indirizzo è allineato a 4 KB, quindi sposta 12 bit a destra
}

// Esempio di setup
void setup_paging() {
    // Inizializza il Page Directory
    for (int i = 0; i < NUM_ENTRIES; i++) {
        set_page_directory_entry(&page_directory[i], 0, 0, 0, 0);  // Disabilita tutte le entry all'inizio
    }
    
    // Inizializza la prima Page Table (identity mapping, mappa 1:1 indirizzi fisici e virtuali)
    for (int i = 0; i < NUM_ENTRIES; i++) {
        set_page_table_entry(&first_page_table[i], i * PAGE_SIZE, 1, 1, 0);  // Mappa ogni pagina fisica a se stessa
    }

    // Collegare la prima Page Table al Page Directory
    set_page_directory_entry(&page_directory[0], (uint32_t)first_page_table, 1, 1, 0);  // Prima entry del Page Directory
}

void map_virtual_to_physical(uint32_t virtual_addr, uint32_t physical_addr, uint32_t flags){
    uint32_t page_dir_index = (virtual_addr >> 22) & 0x3FF;
    uint32_t page_table_index = (virtual_addr >> 12) & 0xFFC00;

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

uint32_t get_physical_address(uint32_t virtual_addr){
    uint32_t page_dir_index = (virtual_addr >> 22) & 0x3FF;
    uint32_t page_table_index = (virtual_addr >> 12) & 0xFFC00;

    if (!page_directory[page_dir_index].present){
        return NULL;
    }

    page_table_entry_t *page_table = (page_table_entry_t *) (page_directory[page_dir_index].page_table_addr << 12);

    if (!page_table[page_table_index].present){
        return NULL;
    }

    return (page_table->physical_addr << 12) | (virtual_addr & 0xFFF);
}