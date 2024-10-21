#ifndef BITMAP_H
#define BITMAP_H

#include "../heap/heap.h"

typedef char *Bitmap;

inline Bitmap alloc_bitmap(unsigned int bit_size){
    return malloc((bit_size+7)>>3);
}

inline void set(Bitmap bm, unsigned int i){
    bm[i>>3] |= 1 << (i%8);
}

inline char get(Bitmap bm, unsigned int i){
    return (bm[i>>3] & (1 << (i%8))) >> i;
}

#endif