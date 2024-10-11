%ifndef PRINT_HEX_PM
%define PRINT_HEX_PM

[bits 32]
print_hex_pm:
    pusha
    mov edx, VIDEO_MEMORY ; Set edx to the start of vid mem.

    mov bx, ax ; save value in bx
    mov ah, WHITE_ON_BLACK ; Store the attributes in AH
    mov al, '0'
    mov [edx], ax
    add edx, 0x2
    mov al, 'x'
    mov [edx], ax
    add edx, 0x2

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
    ;mov al, ax ; Store the char at EBX in AL
    mov ah, WHITE_ON_BLACK ; Store the attributes in AH
    mov [edx], ax
    add edx, 0x2

    shl bx, 4 ; shifta di 4 verso sinistra
    cmp bx, 0
    jne .loop

    popa
    ret

%endif