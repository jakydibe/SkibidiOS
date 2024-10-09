
all: kernel.o boot.bin
	ld -o kernel.bin -Ttext 0x1000 kernel.o --oformat binary
	cat boot.bin kernel.bin > os-image	
clean:
	rm ./*.o && rm ./*.bin

boot.bin: boot.asm 
	nasm -f bin boot.asm -o boot.bin
kernel.o: kernel.c
	gcc -ffreestanding -c kernel.c -o kernel.o
