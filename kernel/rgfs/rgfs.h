#ifndef RGFS_H
#define RGFS_H

#define MAX_FILES 256
#define MAX_SUBDIRS 256
#define MAX_NAME_LENGTH 256
#define BLOCK_SIZE 4096 // Maybe 512 like a sector?
#define HDD_SIZE 4096*1024*5 //20MB

typedef unsigned int Permission;
typedef unsigned int Data_ptr; // Bo possiamo creare una struct per l'header del file dove salvare i dati forse

// Forward declaration of structs
typedef struct _DIRECTORY DIRECTORY, *Directory;
typedef struct _FILE FILE, *File;

typedef struct _FILE {
    char name[MAX_NAME_LENGTH];
    Permission permission;

    Directory parent_dir;
    
    unsigned int data_size;
    Data_ptr data_addr;
} FILE, *File;

typedef struct _DIRECTORY {
    char name[MAX_NAME_LENGTH];
    Permission permission;

    Directory parent_dir;

    unsigned int n_subdirs;
    Directory sub_dirs[MAX_SUBDIRS];

    unsigned int n_files;
    File files[MAX_FILES];

} DIRECTORY, *Directory;

extern Directory root_dir;

void load_fs(); // non so se in reata' vada caricato tutto il fs in ram o lo carichi quando ti serve
void save_fs();

// the fs how is saved on disk?
/*
    unsigned int n_dirs;
    dir1, dir2, dir3 ...    -> dir  : [index_parent_dir, len name, name, permissions]
    unsigned int n_files
    file1, file2, file3 ... -> file : [index_parent_dir, len name, name, permissions, data_addr, data_size]
*/

#include "bitmap.h"
extern Bitmap blockmap; //Qua ci salviamo cosa è libero/occupato nella memoria?

// TO DO
// File file_open(char path[], unsigned int path_len); // se non esiste lo crea?
// void file_close(File f);

// void file_delete(File f);
// void file_save(File f);

// Directory dir_create(char path[], unsigned int path_len);
// void dir_delete(Directory d);

#endif