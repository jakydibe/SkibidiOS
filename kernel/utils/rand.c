#include "rand.h"

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