[bits 32]          ; Set to 32-bit mode

section .header       ; This places the code in the .header section */
global _start         ; Defines the entry point (used in the linker script) */

_start:
    ; Call the kernel's entry point
    extern kernel_main  ; The C function that is the entry point of the kernel
    call kernel_main    ; Call the kernel's main function

    ; Hang if kernel returns
hang:
    jmp hang           ; Infinite loop