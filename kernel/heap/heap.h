#ifndef HEAP_H
#define HEAP_H

#define HEAP_BASE 0x1000000        // Indirizzo di base della heap
#define HEAP_BLOCK_SIZE 4096         // Dimensione di ogni blocco della heap (4 KB)
#define HEAP_SIZE 52428800           // Dimensione totale della heap (50 MB)
#define HEAP_BLOCKS (HEAP_SIZE / HEAP_BLOCK_SIZE) // Calcola automaticamente il numero di blocchi

#define ALIGNMENT HEAP_BLOCK_SIZE
#define size_t unsigned int

//ahahha ce sta questo alignment
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~(ALIGNMENT-1))
#define SIZE_T_SIZE (ALIGN(sizeof(size_t))) // header size


void init_heap();
void* malloc(size_t size);
void free(void *ptr);

// typedef struct {
//     heap_table_entry* entries_array;
//     void* start_address;
// }heap_table;

// broder perchè 4096? come ve pare anche 1024 o 2048 o 512  o
// dobbiamo avere una dimensione fissa?

// 
// a nessuno interessa come implementiamo l'heap, la gente deve solo allocare
// e allora? non ho capito 
// chi importa heap.h importa la malloc, non sa come funziona però
// e quindi? // se lo metti i
//per comodita' allochiamo blocchi da lunghezza fissa. come fanno gli OS normali
//anche se vuoi solo 50byte te ne alloca 4096


#endif