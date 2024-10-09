ORG 0x0
BITS 16

start:
    cli
    mov ax, 0x7C0
    mov ds, ax
    mov es, ax
    mov ax, 0x00
    mov ss, ax
    mov sp, 0x7C00
    sti

    mov si, message ; display error message
    call print_string

    mov [BOOT_DRIVE], dl ; BIOS stores our boot drive in DL , so it ’s best to remember this for later.

    ; Set up parameters for INT 0x13
    mov ah, 2          ; Function: Read Sectors
    mov al, 2          ; Number of sectors to read
    mov bx, 0x9000        ; Load address of the buffer into BX
    mov ch, 0             ; Cylinder number (0)
    mov cl, 2             ; Sector number (start reading from sector 2)
    mov dh, 0             ; Head number (0)
    mov dl, [BOOT_DRIVE] ; Drive number (from data section)

    int 0x13 ; BIOS interrupt

    cmp al, 2 ; if AL ( sectors read ) != DH ( sectors expected ) print error
    jne .read_error
    
    mov ax, [0x9000] ; first sector 0xdada
    call print_hex

    mov ax, [0x9200] ; second sector 0xface
    call print_hex

    jmp .end

.read_error:
    call print_hex ; check the number of sectors read 
    mov si, error_message ; display error message
    call print_string

.end:
    mov si, end_message
    call print_string

    jmp $ ; this jumps to the current address, effectively creating an infinite loop

%include "./print/print_hex.asm"
%include "./print/print_string.asm"

; Global variables
BOOT_DRIVE : db 0

message: db 'Hello World!',0x0A, 0x0D, 0
error_message: db ' Failed to read the disk! ',0x0A, 0x0D, 0
end_message: db ' End ',0x0A, 0x0D, 0

times 510 -($ - $$) db 0
dw 0xAA55

times 256 dw 0xdada
times 256 dw 0xface