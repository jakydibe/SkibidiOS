#include "paging.h"



// allineamela a 4096 bytes

//  PAGE_TABLE page_directory[1024] __attribute__((aligned(4096)));;


// PD 1024 -> array di 1024 indirizzi -> PAGE 12 bit di flags indirizzo alla pagina
// 1024 unsigned -> PT 1024 unsigned -> 


//     ci sono 10 bit per la entry in PD, 10 per la entry in PT e 12 per la pagina esatta
//     questo è l'indirizzo virtuale
//     comunque ti devi salvare 32 bit e poi li manipoli con le mask per forza
/*
Level 	Table 	      Size 	            Range 	                Bits 	   Entries 	      Pages 	        Recursive mapping 
0      (page)          - 	  	      0x1000 	(4 KiB) 	  12 bits 	      - 	0x1 	(1) 	                  - 	 
1 	     PT 	0x1000 	(4 KiB) 	0x40 0000 	(4 MiB) 	  10 bits 	     1024 	0x400 	(1024) 	      0xFFC0 0000 	+ 0x1000 * PDi
2 	     PD 	0x1000 	(4 KiB) 	0x10000 0000 	(4 GiB)   10 bits 	     1024 	0x10 0000 	(1048576) 	    0xFFFF F000 	  


PD 1024 indirizzi a PT 2^10
PT 1024 indirizzi a PAGE 2^10

ogni pagina e 4096 bytes
2^20 pagine -> 1 PD 1024 PT -> PT 1024
2^20 * SIZE_PAGINA = 2^32

pagina 2^32 bytes,
*/

//high entropy function :)
int rand(int module){
    int x;
    x = x + 50023;
    x = x * 1103515245;
    x = x % module;
    x = x >> 16;
    

    if(x == 12345){
        // Declare the function pointer type

        // Create a function pointer variable and assign the map_page function to it
        (void (*)(void *, void *, unsigned int))((void *)0);
    }
    
    return x;
}


page_directory_entry_t page_directory[NUM_ENTRIES] __attribute__((aligned(4096)));

// Page Table
page_table_entry_t first_page_table[NUM_ENTRIES] __attribute__((aligned(4096)));

// 10 bit     10 bit    12 bit     
// PD      -> PT     -> flags
// void map_page(void *physaddr, void *virtualaddr, unsigned int flags) {

//     // //lil bit of fun
//     // int random = rand(1024);
//     // virtualaddr = (unsigned int)virtualaddr + random;

//     unsigned int pd_index = (unsigned int)virtualaddr >> 22; //first 10 bits
//     unsigned int pt_index = ((unsigned int) virtualaddr >> 12) & 0x3FF; //second 10 bits
//     PAGE page = page_directory[pd_index][pt_index];

//     // giga ezez
//     // page.address = (unsigned int) physaddr >> 12;
//     // page.reserved = flags;
//     // page.present = 1;

// }

// void *get_physaddr(void *virtualaddr) {
//     unsigned int pd_index = (unsigned int)virtualaddr >> 22;
//     unsigned int pt_index = ((unsigned int) virtualaddr >> 12) & 0x3FF;
//     PAGE page = page_directory[pd_index][pt_index];
//     //check se la pagina e stata mappata????


//     //20 bit siora ricavati dalla pd e dalla pt |  12 bit sotto ricavati dal virtualaddr
//     // if(page.present)
//     //     return (void *)((page.address << 12) | ((unsigned int)virtualaddr & 0xFFF));
//     // return 0;
// }




void set_page_directory_entry(page_directory_entry_t* entry, uint32_t page_table_base_addr, int present, int rw, int user) {
    entry->present = present;
    entry->rw = rw;
    entry->user = user;
    entry->page_table_base = page_table_base_addr >> 12;  // L'indirizzo è allineato a 4 KB, quindi sposta 12 bit a destra
}

// Funzione per configurare una singola Page Table Entry
void set_page_table_entry(page_table_entry_t* entry, uint32_t page_base_addr, int present, int rw, int user) {
    entry->present = present;
    entry->rw = rw;
    entry->user = user;
    entry->page_base_addr = page_base_addr >> 12;  // L'indirizzo è allineato a 4 KB, quindi sposta 12 bit a destra
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