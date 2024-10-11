#include "mem.h"



void memcpy(char *source, char *dest, int nbytes) {
    int i;
    for (i = 0; i < nbytes; i++) {
        *(dest + i) = *(source + i);
    }
}


int memset(void* addr, int value, unsigned int size){
    for(int i = 0; i < size; i++){
        *(char *) addr = value;
    }
}