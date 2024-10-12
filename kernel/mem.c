#include "mem.h"
#include "strings.h"


void memcpy(char *src, char *dest, int nbytes) {
    for (int i = 0; i < nbytes; i++) {
        dest[i] = src[i];
    }
}


int memset(void* addr, int value, unsigned int size){
    for(int i = 0; i < size; i++){
        *(char *) addr = value;
    }
}