%ifndef READ_DISK
%define READ_DISK 

[bits 16]
; bx destination address in RAM
; cx starting sector
read_disk:          ; prima tocca pushare il settore e poi la destinazione
    mov ax, [bp-0x2]    ; Sector to read

    ; Set up parameters for INT 0x13
    mov ah, 2                  ; Function: Read Sectors
    ; mov al, ax                 ; Number of sectors to read
    ; mov bx, KERNEL_OFFSET    ; Load address of the KERNEL_OFFSET into BX
    mov ch, 0                  ; Cylinder number (0)
    ; mov cl, 2                ; Sector number (start reading from sector 2)
    mov dh, 0                  ; Head number (0)
    mov dl, [BOOT_DRIVE]       ; Drive number (from data section)

    int 0x13 ; BIOS interrupt

    cmp al, [bp-0x2]; if AL ( sectors read ) != DH ( sectors expected ) print error
    jne .error
    ret

.error:
    mov si, MSG_FAIL_TO_READ_DISK ; display error message
    call print_string
    jmp $ ; hang


MSG_FAIL_TO_READ_DISK: db ' Failed to read the disk ',0x0A, 0x0D, 0

%endif