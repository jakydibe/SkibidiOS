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

print:
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