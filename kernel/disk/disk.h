#ifndef DISK_H
#define DISK_H

// Represents a real physical hard disk
#define SKIBIDIOS_DISK_TYPE_REAL 0
#define SKIBIDIOS_SECTOR_SIZE 512
#define EIO 5

typedef unsigned int SKIBIDIOS_DISK_TYPE;

struct disk
{
    SKIBIDIOS_DISK_TYPE type;
    int sector_size;
};

int disk_read_sector(int lba, int total, void* buf);
int disk_write_sector(int lba, int total, void* buf);

void disk_search_and_init();
struct disk* disk_get(int index);
int disk_read_block(struct disk* idisk, unsigned int lba, int total, void* buf);

#endif