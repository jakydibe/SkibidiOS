
void kernel_main(){
    char *video_memory = (char *) 0xB8000;

    video_memory[0] = 'S';
    video_memory[2] = 'K';
    video_memory[4] = 'I';
    video_memory[6] = 'B';
    video_memory[8] = 'i';
    video_memory[10] = 'D';
    video_memory[12] = 'i';
}

// $gcc -ffreestanding -c kernel.c -o kernel.o
// $ld -o kernel.bin -Ttext 0x9000 kernel.o --oformat binary
// cat boot.bin kernel.bin > os-image