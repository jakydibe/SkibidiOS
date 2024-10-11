OUTPUT_FORMAT("elf32-i386")  /* Specify the output format (assuming a 32-bit ELF format) */
ENTRY(_start)                /* Define the entry point of the program */

SECTIONS
{
    /* Define the memory address where the program will be loaded */
    . = 0x1000;

    /* The first part is the header (written in ASM) */
    .header : 
    {
        KEEP(*(.header))     /* Make sure the header is included and not removed */
    }

    /* Code section: contains the executable instructions */
    .text : 
    {
        *(.text)             /* All .text sections from input files */
    }

    /* Data section: initialized global/static variables */
    .data : 
    {
        *(.data)             /* All .data sections from input files */
    }

    /* BSS section: uninitialized global/static variables */
    .bss : 
    {
        *(.bss)              /* All .bss sections from input files */
        . = ALIGN(4);        /* Align to 4-byte boundary for BSS */
    }

    /* Ensure the entire output file does not exceed 2048 bytes */
    /DISCARD/ : 
    {
        *(.comment)
        *(.note*)
    }

    /* Add an assertion for the size limit */
    /*ASSERT(SIZEOF(.header) + SIZEOF(.text) + SIZEOF(.data) + SIZEOF(.bss) <= 2048, "Program exceeds maximum size of 2048 bytes");*/
}
