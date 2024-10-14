#include "paging.h"

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