#ifndef PAGING_H
#define PAGING_H

// https://wiki.osdev.org/Paging
// pls mettete link dove prendete la roba ok

typedef struct __attribute__((packed)){
    union {
        struct {
            unsigned int pd : 10;  // Page Directory index
            unsigned int pt : 10;  // Page table index
        };
        unsigned int address : 20;  // Combined address
    };

    union {
        struct {
            unsigned int present : 1;          // Bit 0: Present flag
            unsigned int read_write : 1;       // Bit 1: Read/Write flag
            unsigned int user_supervisor : 1;  // Bit 2: User/Supervisor flag
            unsigned int reserved : 9;         // Bits 3-11: Reserved/Unused
        };
        unsigned int permissions : 12;         // Combined permissions as a single 12-bit integer
    };
} PAGE;

typedef PAGE PAGE_TABLE[1024];

extern PAGE_TABLE page_directory[1024];

void enablePaging(int *pd_addr);

//merda in C

void enable_paging(unsigned int page_directory_addr);
void *get_physaddr(void *virtualaddr);
void map_page(void *physaddr, void *virtualaddr, unsigned int flags);

#endif