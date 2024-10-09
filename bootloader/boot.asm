ORG 0x7c00
BITS 16

KERNEL_OFFSET equ  0x1000

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

    ; Set up parameters for INT 0x13
    mov ah, 2          ; Function: Read Sectors
    mov al, 2          ; Number of sectors to read
    mov bx, KERNEL_OFFSET     ; Load address of the KERNEL_OFFSET into BX
    mov ch, 0             ; Cylinder number (0)
    mov cl, 2             ; Sector number (start reading from sector 2)
    mov dh, 0             ; Head number (0)
    mov dl, [BOOT_DRIVE] ; Drive number (from data section)

    int 0x13 ; BIOS interrupt

    cmp al, 2 ; if AL ( sectors read ) != DH ( sectors expected ) print error
    je .kernel_loaded

    mov si, MSG_KERNEL_LOAD_ERROR ; display error message
    call print_string
    jmp .infinite_loop

.kernel_loaded:
    mov bx, 0x0
.loop_stampa:
    ; mov cx, bx
    ; add cx, KERNEL_OFFSET
    ; mov ax, [KERNEL_OFFSET + bx]
    ; ; call print_hex
    ; add bx, 2
    ; cmp bx, 0x20
    ; jne .loop_stampa

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

.infinite_loop:
    jmp $

%include "print/print_hex.asm"
%include "print/print_hex_pm.asm"
%include "print/print_string.asm"
%include "print/print_string_pm.asm"
%include "gdt/gdt32.asm"

BITS 32
protected_mode:
    ; Initialise registers and the stack once in PM.
    mov eax, DATA32_SEG ; Now in PM , our old segments are meaningless ,
    mov ds, eax ; so we point our segment registers to the
    mov ss, eax ; data selector we defined in our GDT
    mov es, eax
    mov fs, eax
    mov gs, eax
    mov ebp, 0x90000 ; Update our stack position so it is right
    mov esp, ebp ; at the top of the free space.

    mov ebx , MSG_PROT_MODE
    call print_string_pm ; Use our 32 - bit print routine.

    mov ax, 0x1234
    call print_hex_pm
    jmp $
    ; jmp 0x1000
    ; jmp KERNEL_OFFSET ; jmp in the kernel entry point
    ; jmp start.infinite_loop
    
; Global variables
BOOT_DRIVE : db 0
MSG_REAL_MODE db " Started in 16 - bit Real Mode",0x0A, 0x0D, 0
MSG_KERNEL_LOAD_ERROR: db ' Failed to read the disk! ',0x0A, 0x0D, 0
MSG_KERNEL_LOADED: db ' Kernel loaded in memory, switching to protected mode',0x0A, 0x0D,  0
MSG_PROT_MODE db " Successfully landed in 32 - bit Protected Mode " , 0

times 510 -($ - $$) db 0
dw 0xAA55

kernel_main:

    mov ebx, MSG_REAL_MODE
    call print_string_pm
.infinite_loop:
    jmp .infinite_loop
    
times 256 dw 0xdead
times 256 dw 0xbeef
