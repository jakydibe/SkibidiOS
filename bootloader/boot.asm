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

    mov si, message
    call print
    jmp $ ; this jumps to the current address, effectively creating an infinite loop

print_hex:
    mov ax, bx
    and ax, 0xf000 ; prendi le 4 prime cifre
    shr ax, 12
    cmp ax, 9 ;check if is a digit
    jle print_hex_digit 
    
    add ax, 55 ; aggiunge offset per cifre A-F
    jmp print_hex_end
print_hex_digit:
    add ax, 48 ; aggiunge offset ascii per cifre decimali
print_hex_end:
    call print_char
    
    shl bx, 4 ; shifta di 4 verso sinistra
    cmp bx, 0
    jne print_hex
    ret

print_string:
    mov bx, 0
loop:
    lodsb
    cmp al,0
    je done
    call print_char
    jmp loop
done:
    ret

print_char:
    mov ah, 0eh
    int 0x10
    ret


message: db 'Hello World!', 0
times 510 -($ - $$) db 0

dw 0xAA55