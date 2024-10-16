#include "disk.h"
#include "../io/io.h"
#include <stddef.h>
#include <stdint.h>
#include "../strings.h"

struct disk disk;

int disk_write_sector(int lba, int total, void* buf)
{
    // Seleziona il disco e imposta i bit dell'LBA
    outb(0x1F6, (lba >> 24) | 0xE0);
    outb(0x1F2, total);
    outb(0x1F3, (unsigned char)(lba & 0xff)); // LBA bits 0-7
    outb(0x1F4, (unsigned char)(lba >> 8));   // LBA bits 8-15
    outb(0x1F5, (unsigned char)(lba >> 16));  // LBA bits 16-23
    
    outb(0x1F7, 0x30);  // Comando WRITE SECTORS

    unsigned short* ptr = (unsigned short*) buf;
    for (int b = 0; b < total; b++)
    {
        // Attendi che il buffer sia pronto
        while (insb(0x1F7) & 0x80);  // Aspetta che il bit BSY sia 0
        char c = insb(0x1F7);
        while(!(c & 0x08))  // Aspetta che DRQ sia impostato
        {
            
            c = insb(0x1F7);
        }
        // Scrivi i dati dal buffer al disco
        for (int i = 0; i < 256; i++)
        {
            outw(0x1F0, *ptr);
            ptr++;
        }
    }
    return 0;
}



int disk_read_sector(int lba, int total, void* buf)
{
    //magic
    outb(0x1F6, (lba >> 24) | 0xE0); // Select drive and LBA bits
    outb(0x1F2, total);
    outb(0x1F3, (unsigned char)(lba & 0xff)); // LBA bits 0-7
    outb(0x1F4, (unsigned char)(lba >> 8));   // LBA bits 8-15
    outb(0x1F5, (unsigned char)(lba >> 16)); // LBA bits 16-23
    outb(0x1F7, 0x20);  // Send READ SECTORS command

    unsigned short* ptr = (unsigned short*) buf;
    for (int b = 0; b < total; b++)
    {
        // Wait for the buffer to be ready
        char c = insb(0x1F7);
        while(!(c & 0x08))
        {
            c = insb(0x1F7);
        }

        // Copy from hard disk to memory
        for (int i = 0; i < 256; i++)
        {
            *ptr = insw(0x1F0);
            ptr++;
        }

    }
    return 0;
}

void disk_search_and_init()
{
    memset(&disk, 0, sizeof(disk));
    disk.type = SKIBIDIOS_DISK_TYPE_REAL;
    disk.sector_size = SKIBIDIOS_SECTOR_SIZE;
}

struct disk* disk_get(int index)
{
    if (index != 0)
        return 0;
    
    return &disk;
}

int disk_read_block(struct disk* idisk, unsigned int lba, int total, void* buf)
{
    if (idisk != &disk)
    {
        return -EIO;
    }

    return disk_read_sector(lba, total, buf);
}