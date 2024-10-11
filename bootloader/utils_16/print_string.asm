%ifndef PRINT_STRING
%define PRINT_STRING 

[bits 16]
print_string:
    mov bx, 0
.loop:
    lodsb ; Load byte from DS:SI into AL
    cmp al,0
    je .done
    
    mov ah, 0eh
    int 0x10
    
    jmp .loop
.done:
    ret

%endif