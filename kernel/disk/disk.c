#include "disk.h"
#include "../io/io.h"
#include "../heap/heap.h"
#include "../utils/strings.h"
#include "../utils/mem.h"

#define IDE_DATA_PORT 0x1F0
#define IDE_SECTOR_COUNT_PORT 0x1F2
#define IDE_LBA_LOW_PORT 0x1F3
#define IDE_LBA_MID_PORT 0x1F4
#define IDE_LBA_HIGH_PORT 0x1F5
#define IDE_DRIVE_SELECT_PORT 0x1F6
#define IDE_COMMAND_PORT 0x1F7
#define IDE_STATUS_BUSY 0x80
#define IDE_STATUS_DRQ  0x08
#define IDE_STATUS_ERR  0x01
#define IDE_STATUS 0x1F7

#define SECTOR_SIZE 0x200

#define TIMEOUT 1000000  // Adjust based on your system's speed

DISK disk;

void disk_search_and_init()
{
    memset(&disk, 0, sizeof(disk));
    disk.type = SKIBIDIOS_DISK_TYPE_REAL;
    disk.sector_size = SKIBIDIOS_SECTOR_SIZE;
}

disk_status_t disk_read_sector(int lba, int total, void* buf)
{
    outb(IDE_DRIVE_SELECT_PORT, (lba >> 24) | 0xE0); // Select drive and LBA bits
    outb(IDE_SECTOR_COUNT_PORT, total);
    outb(IDE_LBA_LOW_PORT, (unsigned char)(lba & 0xff)); // LBA bits 0-7
    outb(IDE_LBA_MID_PORT, (unsigned char)(lba >> 8));   // LBA bits 8-15
    outb(IDE_LBA_HIGH_PORT, (unsigned char)(lba >> 16)); // LBA bits 16-23
    outb(IDE_COMMAND_PORT, 0x20);  // Send READ SECTORS command

    unsigned short* ptr = (unsigned short*) buf;
    for (int b = 0; b < total; b++)
    {
        int timeout = TIMEOUT;

        // Wait for the drive to be ready (not busy and DRQ set)
        while ((insb(IDE_STATUS) & (IDE_STATUS_BUSY | IDE_STATUS_DRQ)) != IDE_STATUS_DRQ) 
        {
            if (--timeout == 0)
            {
                return DISK_ERR_TIMEOUT;
            }
        }

        // Check for errors in the status register
        if (insb(IDE_STATUS) & IDE_STATUS_ERR)
        {
            return DISK_ERR_DRIVE_FAULT;
        }

        // Transfer the data (512 bytes per sector, 256 words)
        for (int i = 0; i < 256; i++)
        {
            *ptr = insw(IDE_DATA_PORT);
            ptr++;
        }

    }
    
    return DISK_SUCCESS;
}

disk_status_t disk_write_sector(int lba, int total, void* buf)
{
    // Seleziona il disco e imposta i bit dell'LBA
    outb(IDE_DRIVE_SELECT_PORT, (lba >> 24) | 0xE0);
    outb(IDE_SECTOR_COUNT_PORT, total);
    outb(IDE_LBA_LOW_PORT, (unsigned char)(lba & 0xff)); // LBA bits 0-7
    outb(IDE_LBA_MID_PORT, (unsigned char)(lba >> 8));   // LBA bits 8-15
    outb(IDE_LBA_HIGH_PORT, (unsigned char)(lba >> 16));  // LBA bits 16-23
    
    outb(IDE_COMMAND_PORT, 0x30);  // Comando WRITE SECTORS

    unsigned short* ptr = (unsigned short*) buf;
    for (int b = 0; b < total; b++)
    {
        // Wait for the disk to become ready (BSY should be 0, DRQ should be 1)
        int timeout = TIMEOUT;
        while (insb(IDE_STATUS) & IDE_STATUS_BUSY)  // Wait while BSY is set (busy)
        {
            if (--timeout == 0) return DISK_ERR_TIMEOUT;
        }

        // Check for error before proceeding (ERR bit set)
        if (insb(IDE_STATUS) & IDE_STATUS_ERR)  // Check ERR bit (bit 0 of status register)
        {
            return -2;  // Disk error occurred
        }

        // Wait for DRQ to be set (ready to transfer data)
        timeout = TIMEOUT;
        while (!(insb(IDE_STATUS) & IDE_STATUS_DRQ))  // Wait for DRQ to be set
        {
            if (--timeout == 0) return DISK_ERR_NO_DRQ;  // Timeout waiting for DRQ
        }

        // Transfer data to the disk (256 words = 512 bytes per sector)
        for (int i = 0; i < 256; i++)
        {
            outw(IDE_DATA_PORT, *ptr);  // Write one word (16 bits) to the data port
            ptr++;              // Move to the next word in the buffer
        }
    }
    for(int i=0;i<100000;i++); // wait or the next read won't work
    return DISK_SUCCESS;
}


void disk_write_to_addr(int addr, unsigned int bufsize, void* buf){
    // puts("info\n    addr: ");
    // hexprint(addr);
    // puts("\n    bufsize: ");
    // hexprint(bufsize);
    // puts("\n    buf_addr: ");
    // hexprint(buf);

    unsigned int offset = addr%SECTOR_SIZE;
    if(offset){ // start in the middle of a sector
        char sec_buf[SECTOR_SIZE];
        int sector_begin = addr/SECTOR_SIZE;
        disk_read_sector(sector_begin, 1, sec_buf); // leggi il settore
        memcpy(buf, sec_buf+offset, SECTOR_SIZE-offset);           // modifica la parte di settore da modificare
        disk_write_sector(sector_begin, 1, sec_buf);// riscrivi il settore in memoria

        addr = addr + (SECTOR_SIZE-offset);
        bufsize = bufsize - (SECTOR_SIZE-offset);
        buf = ((char *) buf) + (SECTOR_SIZE-offset);
    }

    offset = bufsize%SECTOR_SIZE;
    if(offset){ // end in the middle of a sectorù
        char sec_buf[SECTOR_SIZE];
        int sector_end = (addr + bufsize + (SECTOR_SIZE - 1)) / SECTOR_SIZE;
        disk_read_sector(sector_end, 1, sec_buf);   // leggi il settore
        memcpy(buf+bufsize-offset, sec_buf, offset);        // modifica la parte di settore da modificare
        disk_write_sector(sector_end, 1, sec_buf);// riscrivi il settore in memoria
        
        bufsize = bufsize - offset;
    }

    // Edge case gia considerati, numero preciso di settori da scrivere
    int lba = addr/SECTOR_SIZE;
    int total = bufsize/SECTOR_SIZE;
    disk_write_sector(lba, total, buf);
}

void verbose_disk_read_sector(int lba, int total, void* buf){
    puts("\nReading from disk... ");
    print_status(disk_read_sector(lba, total, buf));
}

void verbose_disk_write_sector(int lba, int total, void* buf){
    puts("\nWriting to disk... ");
    print_status(disk_write_sector(lba, total, buf));
}

void print_status(disk_status_t status){
    switch (status) {
        case DISK_SUCCESS:
            puts("Operation successful.\n");
            break;
        case DISK_ERR_TIMEOUT:
            puts("Error: Operation timed out.\n");
            break;
        case DISK_ERR_DRIVE_FAULT:
            puts("Error: Drive fault occurred.\n");
            break;
        case DISK_ERR_NO_DRQ:
            puts("Error: DRQ not set, unable to transfer data.\n");
            break;
        case DISK_ERR_UNKNOWN:
            puts("Error: Unknown error occurred.\n");
            break;
        default:
            puts("Error: Unrecognized status code.\n");
            break;
    }
}