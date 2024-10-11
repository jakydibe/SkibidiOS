%ifndef PRINT_HEX
%define PRINT_HEX 

[bits 16]
print_hex:
    pusha
    mov bx, ax ; save value in bx
    mov ah, 0eh ; Write Character (AH = 0x0E)
    mov al, '0'
    int 0x10 ; BIOS interrupt
    mov al, 'x'
    int 0x10 ; BIOS interrupt
.loop:
    mov ax, bx
    shr ax, 12
    and ax, 0x000F ; Mask the lower 4 bits to isolate the hex digit
    
    cmp ax, 9 ; check if is a digit
    jle .is_digit 
    
    add ax, 55 ; Convert 10-15 to ASCII 'A'-'F'
    jmp .end

.is_digit:
    add ax, 48 ; Convert 0-9 to ASCII '0'-'9'
.end:
    mov ah, 0eh ; Write Character (AH = 0x0E)
    int 0x10 ; BIOS interrupt
    
    shl bx, 4 ; shifta di 4 verso sinistra
    cmp bx, 0
    jne .loop

    mov al, 0x0A
    int 0x10 ; BIOS interrupt
    mov al, 0x0D
    int 0x10 ; BIOS interrupt
    popa
    ret

%endif