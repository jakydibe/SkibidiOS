OUTPUT_FORMAT("binary")
OUTPUT_ARCH(i386)

SECTIONS
{
    . = 0x1000; /* Starting address for kernel */
    
    entry_point : {
        KEEP(*(.text.entry))   /* Entry point section */
    }
    
    kernel : {
        KEEP(*(.text.kernel))  /* Kernel section */
        KEEP(*(.data.kernel))   /* Kernel data section */
    }
    
    _end = .; /* End of the output file */
}