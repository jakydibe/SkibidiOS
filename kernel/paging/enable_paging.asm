; https://wiki.osdev.org/Paging#Enabling
[bits 32]
global enablePaging
extern hexprint
extern puts

enablePaging:
	mov eax, [esp +4] ; Page addresses
    ; //push eax; Page directory
    ; call hexprint
	mov cr3, eax
	mov eax, cr0

    or eax, 0x80000000 ; Enable paging

	mov cr0, eax
	ret


