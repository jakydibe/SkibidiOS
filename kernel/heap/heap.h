#ifndef HEAP_H
#define HEAP_H

#include <stdint.h>
#include <stddef.h>

#define HEAP_BASE 0x1000000        // Indirizzo di base della heap
#define HEAP_BLOCK_SIZE 4096         // Dimensione di ogni blocco della heap (4 KB)
#define HEAP_SIZE 52428800           // Dimensione totale della heap (50 MB)
#define HEAP_BLOCKS (HEAP_SIZE / HEAP_BLOCK_SIZE) // Calcola automaticamente il numero di blocchi

extern unsigned int num_malloc;
extern unsigned int block_allocated;

void init_heap();
void* malloc(size_t size);
void free(void *ptr);

#endif