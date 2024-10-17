#ifndef DISK_H
#define DISK_H

// Represents a real physical hard disk
#define SKIBIDIOS_DISK_TYPE_REAL 0
#define SKIBIDIOS_SECTOR_SIZE 512
#define EIO 5

typedef enum {
    DISK_SUCCESS = 0,
    DISK_ERR_TIMEOUT = -1,
    DISK_ERR_DRIVE_FAULT = -2,
    DISK_ERR_NO_DRQ = -3,
    DISK_ERR_UNKNOWN = -4
} disk_status_t;

typedef unsigned int SKIBIDIOS_DISK_TYPE;

typedef struct
{
    SKIBIDIOS_DISK_TYPE type;
    int sector_size;
} DISK;

void disk_search_and_init();

disk_status_t disk_read_sector(int lba, int total, void* buf);
disk_status_t disk_write_sector(int lba, int total, void* buf);

void disk_write_to_addr(int addr, unsigned int bufsize, void* buf);

void verbose_disk_read_sector(int lba, int total, void* buf);
void verbose_disk_write_sector(int lba, int total, void* buf);

void print_status(disk_status_t status);

#endif