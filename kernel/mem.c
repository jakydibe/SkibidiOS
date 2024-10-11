#include "mem.h"

int memset(void* addr, int value, unsigned int size){
    for(int i = 0; i < size; i++){
        *(char *) addr = value;
    }
}