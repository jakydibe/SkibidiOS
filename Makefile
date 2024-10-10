kernel_dir=./kernel
bootloader_dir=./bootloader

all: $(kernel_dir)/kernel.o $(bootloader_dir)/boot.bin
	ld -o $(kernel_dir)/kernel.bin -Ttext 0x1000 $(kernel_dir)/kernel.o --oformat binary
	cat $(bootloader_dir)/boot.bin $(kernel_dir)/kernel.bin > os-image	

clean:
	rm -f $(kernel_dir)/*.o $(kernel_dir)/*.bin $(bootloader_dir)/*.bin os-image

$(bootloader_dir)/boot.bin: $(bootloader_dir)/boot.asm 
	nasm -f bin $(bootloader_dir)/boot.asm -o $(bootloader_dir)/boot.bin

$(kernel_dir)/kernel.o: $(kernel_dir)/kernel.c
	gcc -ffreestanding -c $(kernel_dir)/kernel.c -o $(kernel_dir)/kernel.o