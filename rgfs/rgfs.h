#ifndef RGFS_H
#define RGFS_H



typedef struct{
    //indirizzo kernel
    //size kernel

    void* root_directory;
    int grandezza_disco;
    // size root directory
    //indirizzo 

}header;


typedef struct{
    void * indirizzo;
    int size;

    allocazioni* next;
    allocazioni* prev;


} allocazioni;



typedef struct{
    //nome
    char nome[32];
    int grandezza;
    void* puntatore;
    int permssions;

} file;




typedef struct{
    file files[32];

    directory directories[32];

    directory* parent;
    directory* current; 
    int permssions;

} directory;




#endif