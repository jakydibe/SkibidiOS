/*
1) decidere indirizzo di inizio Heap, (0x01000000), blocchi 4096 bytes
2) tabella entry: 
    - se e' il primo
    - se e' l' ultimo
    - se e' free
    puntatore al blocco? forse ridondante dato che e' sequenziale
3) creare un array globale che tiene un

/*l' allocazione funziona iterando l' array di entries 
    1) ne troviamo una libera
    2) checkiamo se le N-1 successive ( con N numero di blocchi che ce servono) sono free
    3) ritorna 
*/

#include "heap.h"
#include "../strings.h"
#include "../mem.h"


typedef struct heap
{
    unsigned char is_used;
    unsigned char is_last;
} heap_entry;

heap_entry blocks[HEAP_BLOCKS] = {0};


void init_heap(){
    for (int i = 0; i < HEAP_BLOCKS; i++){
        blocks[i].is_used = 0;
        blocks[i].is_last = 0;
    }
}


/*
-----------------------------------------------------------------------------------------------------
|        |        |        |        |        |        |                                              |
|  4096  |        |        |        |        |        |     .   .   .                                |
|        |        |        |        |        |        |                                              |
-----------------------------------------------------------------------------------------------------

    ^
    |
----------------------------------------------------------------------------
    
---------------
|h_table_entry|
---------------
|   is_used   |
|   is_last   |
|             |
---------------

*/

/*l' allocazione funziona iterando l' array di entries 
    1) ne troviamo una libera
    2) checkiamo se le N-1 successive ( con N numero di blocchi che ce servono) sono free
    3) ritorna 
*/ //ok


void* malloc(size_t size){
    int needed_blocks = (size + HEAP_BLOCK_SIZE - 1) / HEAP_BLOCK_SIZE;
    for(int i = 0; i < (HEAP_BLOCKS - needed_blocks); i ++){
        int found = 0;
        if (!blocks[i].is_used){ 

            for (int x = 0; x < needed_blocks; x++){
                if (!blocks[i+x].is_used){
                    found++;
                }
            }
            if (found == needed_blocks){
                for (int x = 0; x < needed_blocks; x++){
                    blocks[i+x].is_used = 1;
                }

                //OCCHIO QUI CI VORREBBE -1 MA NON FUNZIONA CON -1
                blocks[i+needed_blocks -1].is_last = 1;
                return (void *) HEAP_BASE + i*HEAP_BLOCK_SIZE;
            }
        }
    }
    return (void *) 0;
}
void free(void *addr){
    int first_block = ((int)addr - HEAP_BASE) / HEAP_BLOCK_SIZE;
    int i = first_block;
    do {
        blocks[i].is_used = 0;
        blocks[i].is_last = 0;
        i++;
    }while(!blocks[i].is_last);
}