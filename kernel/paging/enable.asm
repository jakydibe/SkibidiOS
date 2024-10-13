; https://wiki.osdev.org/Paging#Enabling
[bits 32]
global enablePaging
extern hexprint
extern puts

enablePaging:
	mov eax, [esp + 4] ; Page addresses
    ; //push eax; Page directory
    ; call hexprint
	mov cr3, eax

	mov eax, cr0

    or eax, 0x80000001 ; Enable paging
	;mov cr0, eax
    ; jmp $
	ret



;  mov eax, page_directory
;  mov cr3, eax
 
;  mov eax, cr0
;  or eax, 0x80000001
;  mov cr0, eax
; questa funzione carica la page directory
; page_directory ce la possiamo caricare come ce pare no? credo di si, immagino abbia una struttura però
; pero' a sto punto sembra che la page directory deve seguire certe regole precise


; Enable Paging

; ;The final step is to actually enable paging. 
; First we tell the processor where to find our page directory by putting 
; it's address into the CR3 register. Because C code cannot directly access the 
; computer's registers, we will need to use assembly code to access CR3. 
; The following assembly is written for GAS. 
; ;If you use a different assembler then you will need to translate between 
; ;this assembly format and the format supported by your assembler. 


; enablePaging:
;     mov ebp, esp

;     xor eax, eax

;     mov eax, [ebp+8] ; cos'è page_directory? l' array che contiene l' array che contiene l' array
;     mov cr3, eax            ; dove è stato creato? ancora non l' abbiamo mica creato
    
;     mov eax, cr0
;     or eax, 0x80000001 ;protection (PE) bits of CR0
;     mov cr0, eax

;     pop ebp
