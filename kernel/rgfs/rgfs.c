#include "rgfs.h"
#include "../disk/disk.h"
#include "../heap/heap.h"

#define FS_SECTOR 0x0
#define FS_NUM_SECTOR 0x0

Directory root_dir;
Bitmap blockmap; //Qua ci salviamo cosa è libero/occupato nella memoria?

void load_fs(){
    char *fs = malloc(FS_NUM_SECTOR * SKIBIDIOS_SECTOR_SIZE);
    char *buffer_ptr = fs;
    disk_read_sector(FS_SECTOR, FS_NUM_SECTOR, fs);

    /*
    unsigned int n_dirs;
    dir1, dir2, dir3 ...    -> dir  : [index_parent_dir, name, permissions]
    unsigned int n_files
    file1, file2, file3 ... -> file : [index_parent_dir, name, permissions, data_addr, data_size]
    */

    // Built directory tree
    root_dir = malloc(sizeof(DIRECTORY));
    root_dir->name[0] = 0;
    root_dir->permission = 0;
    root_dir->parent_dir = NULL;
    root_dir->n_subdirs = 0;
    root_dir->n_files = 0;

    // read len data
    unsigned int n_dirs = *((unsigned int *) fs);
    fs+=sizeof(unsigned int);

    Directory dirs[n_dirs];
    dirs[0] = root_dir;

    unsigned int parent_dir_index;
    Directory parent_dir;
    // parse directories -> [index_parent_dir, name, permissions]
    for(int i = 0; i < n_dirs; i++){
        Directory d = malloc(sizeof(DIRECTORY));
        dirs[i+1] = d;

        parent_dir = dirs[*((unsigned int *) fs)];
        fs += sizeof(unsigned int);

        d->parent_dir = parent_dir;

        parent_dir->sub_dirs[parent_dir->n_subdirs] = d;
        parent_dir->n_subdirs++;

        int j = 0;
        while (*fs != 0){
            d->name[j] = *fs;
            fs++;
            j++;
        }

        d->permission = *((unsigned int *) fs);
        fs += sizeof(unsigned int);

        d->n_files = 0;
        d->n_subdirs = 0;
    }

    unsigned int n_files = *((unsigned int *) fs);
    
    // parse files -> [index_parent_dir, name, permissions, data_addr, data_size]
    for(int i = 0; i < n_files; i++){
        File f = malloc(sizeof(FILE));

        parent_dir = dirs[*((unsigned int *) fs)];
        fs += sizeof(unsigned int);

        f->parent_dir = parent_dir;

        parent_dir->files[parent_dir->n_files] = f;
        parent_dir->n_files++;

        int j = 0;
        while (*fs != 0){
            f->name[j] = *fs;
            fs++;
            j++;
        }

        f->permission = *((unsigned int *) fs);
        fs += sizeof(unsigned int);

        f->data_addr = *((unsigned int *) fs);
        fs += sizeof(unsigned int);

        f->data_size = *((unsigned int *) fs);
        fs += sizeof(unsigned int);
    }

    free(buffer_ptr);
}

// [index_parent_dir, name, permissions]
void dirs_to_mem_dfs(Directory d, char **buffer, unsigned int *dir_counter){
    unsigned int current_index = *dir_counter;
    for(int i = 0; i < d->n_subdirs; i++){
        // Write directory entry
        *((unsigned int *) *buffer) = current_index;
        *buffer += sizeof(unsigned int);
        
        int j = 0;
        while (d->sub_dirs[i]->name[j] != 0){
            **buffer = d->sub_dirs[i]->name[j];
            (*buffer)++;
            j++;
        }

        *((Permission *) *buffer) = d->sub_dirs[i]->permission;
        *buffer += sizeof(Permission);

        // recursive call
        (*dir_counter)++;
        dirs_to_mem_dfs(d->sub_dirs[i], buffer, dir_counter);
    }
}

// [index_parent_dir, name, permissions, data_addr, data_size]
unsigned files_to_mem_dfs(Directory d, char **buffer, unsigned int *dir_counter){
    unsigned int current_index = *dir_counter;
    
    unsigned int n_files = d->n_files;
    // write all the files in the directory
    for(int i = 0; i < d->n_files; i++){
        File f = d->files[i];

        *((unsigned int *) *buffer) = current_index;
        *buffer += sizeof(unsigned int);

        int j = 0;
        while (f->name[j] != 0){
            **buffer = f->name[j];
            (*buffer)++;
            j++;
        }

        *((Permission *) *buffer) = f->permission;
        *buffer += sizeof(Permission);

        *((unsigned int *) *buffer) = f->data_addr;
        *buffer += sizeof(unsigned int);

        *((unsigned int *) *buffer) = f->data_size;
        *buffer += sizeof(unsigned int);
    }
    
    // recursive calls
    for(int i = 0; i < d->n_subdirs; i++){
        (*dir_counter)++;
        n_files += files_to_mem_dfs(d->sub_dirs[i], buffer, dir_counter);
    }

    return n_files;
}

void save_fs(){
    char *fs = malloc(FS_NUM_SECTOR * SKIBIDIOS_SECTOR_SIZE);
    char *buffer_ptr = fs;
    /*
    unsigned int n_dirs;
    dir1, dir2, dir3 ...    -> dir  : [index_parent_dir, name, permissions]
    unsigned int n_files
    file1, file2, file3 ... -> file : [index_parent_dir, name, permissions, data_addr, data_size]
    */

    // Convert directory to mem representation
    unsigned int n_dirs = 0;
    unsigned int *n_dirs_ptr = (unsigned int *) fs;
    fs += sizeof(unsigned int); // skip the space for number of directories
    dirs_to_mem_dfs(root_dir, &fs, &n_dirs);
    *n_dirs_ptr = n_dirs;
    
    // Convert files to mem representation
    n_dirs = 0;
    unsigned int *n_files_ptr = (unsigned int *) fs;
    fs += sizeof(unsigned int); // skip the space for number of files
    *n_files_ptr = files_to_mem_dfs(root_dir, &fs, &n_dirs);

    disk_write_sector(FS_SECTOR, FS_NUM_SECTOR, buffer_ptr);
    free(buffer_ptr);
}