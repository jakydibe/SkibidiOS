[bits 32]          ; Set to 32-bit mode

section .text.entry
global _start      ; Make _start visible to the linker

_start:
    ; Call the kernel's entry point
    extern kernel_main  ; The C function that is the entry point of the kernel
    call kernel_main    ; Call the kernel's main function

    ; Hang if kernel returns
hang:
    jmp hang           ; Infinite loop