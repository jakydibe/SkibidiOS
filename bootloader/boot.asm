[org 0x7c00]
[bits 16]

KERNEL_OFFSET equ  0x1000
INFO_SECTOR equ 0x7c00 + 512

jmp start
times 4 db 0xAAAAAAAA

start:
    cli
    ; inizialize all registers to 0
    mov ax, 0x00
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x00
    sti

    mov si, MSG_REAL_MODE
    call print_string

    mov [BOOT_DRIVE], dl ; BIOS stores our boot drive in DL , so it ’s best to remember this for later.

    mov bx, INFO_SECTOR       ; destination address in RAM
    mov cx, 2                 ; starting sector (the second)
    push 1
    call read_disk
    pop ax

    mov bx, KERNEL_OFFSET
    mov cx, 3       ; starting sector (the second)
    mov ax, [INFO_SECTOR]
    push ax
    call read_disk
    pop ax

    mov si, MSG_KERNEL_LOADED
    call print_string      ; Print kernel load message

    ; Switch to protected mode
    cli     ; We must switch of interrupts until we have
            ; set - up the protected mode interrupt vector
            ; otherwise interrupts will run riot.


    lgdt[gdt32_descriptor]      ; Load our global descriptor table , which defines
                                ; the protected mode segments ( e.g. for code and data )
    mov eax, cr0 ; To make the switch to protected mode , we set
    or eax, 0x1 ; the first bit of CR0 , a control register
    mov cr0, eax

    jmp CODE32_SEG:protected_mode   ; Make a far jump ( i.e. to a new segment ) to our 32 - bit
                                    ; code. This also forces the CPU to flush its cache of
                                    ; pre - fetched and real - mode decoded instructions , which can
                                    ; cause problems.

%include "./bootloader/utils_16/print_hex.asm"
%include "./bootloader/utils_16/print_string.asm"
%include "./bootloader/utils_16/read_disk.asm"
%include "./bootloader/utils_32/print_string_pm.asm"
%include "./bootloader/gdt/gdt32.asm"

[bits 32]
protected_mode:
    ; Initialise registers and the stack once in PM.
    mov eax, DATA32_SEG ; Now in PM , our old segments are meaningless ,
    mov ds, eax ; so we point our segment registers to the
    mov ss, eax ; data selector we defined in our GDT
    mov es, eax
    mov fs, eax
    mov gs, eax
    ; mov ebp, 0x90000 ; Update our stack position so it is right
    mov ebp, 0x00200000
    mov esp, ebp ; at the top of the free space.

    ; Enable A20 line, l'ho trovato qui: https://wiki.osdev.org/A20_Line
    in al, 0x92
    or al, 2
    out 0x92, al

    mov ebx, MSG_PROT_MODE
    call print_string_pm

    jmp KERNEL_OFFSET
    
; Global variables
BOOT_DRIVE : db 0
MSG_REAL_MODE db " Started in 16 - bit Real Mode",0x0A, 0x0D, 0
MSG_KERNEL_LOADED: db ' Kernel loaded in memory, switching to protected mode',0x0A, 0x0D,  0
MSG_PROT_MODE db " Successfully landed in 32 - bit Protected Mode " , 0

times 510 -($ - $$) db 0
dw 0xAA55