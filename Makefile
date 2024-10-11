# Define the source file locations
BOOTLOADER_DIR = bootloader
KERNEL_DIR = kernel
INTERRUPT_DIR = kernel/interrupt
# Define the names of your source files
BOOTLOADER = $(BOOTLOADER_DIR)/boot.asm
ENTRY = $(KERNEL_DIR)/entry.asm
KERNEL = $(KERNEL_DIR)/kernel.c 
STRINGS = $(KERNEL_DIR)/strings.c
INTERRUPT = $(KERNEL_DIR)/interrupt.c
MEM = $(KERNEL_DIR)/mem.c
PYTHON_SCRIPT = craft_image.py

# Define the output directory for intermediate files
BUILD_DIR = build

# Define the output file names (stored in the build directory)
BOOTLOADER_BIN = $(BUILD_DIR)/boot.bin
ENTRY_OBJ = $(BUILD_DIR)/entry.o
KERNEL_OBJ = $(BUILD_DIR)/kernel.o
STRINGS_OBJ = $(BUILD_DIR)/strings.o
KERNEL_BIN = $(BUILD_DIR)/kernel.bin
INTERRUPT_OBJ = $(BUILD_DIR)/interrupt.o
MEM_OBJ = $(BUILD_DIR)/mem.o
# The final OS image will be in the root directory
OS_IMAGE = skibidiOS

# Tools for building
ASM = nasm
CC = gcc
LD = ld

# Compilation and linking flags
ASMFLAGS = -f bin# Bootloader needs to be in binary format
ASMOBJFLAGS = -f elf32# Entry ASM should be in ELF format for linking
CFLAGS = -m32 -ffreestanding -nostdlib -fno-pic -fno-pie -static
#LDFLAGS = -m elf_i386 -Ttext 0x1000 --oformat binary 
LDFLAGS = -m elf_i386 -T linker.ld --oformat binary

# Default target: build the OS image
all: $(OS_IMAGE)

# Ensure the build directory exists
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Step 1: Assemble the bootloader (512 bytes)
$(BOOTLOADER_BIN): $(BOOTLOADER) | $(BUILD_DIR)
	$(ASM) $(ASMFLAGS) $(BOOTLOADER) -o $(BOOTLOADER_BIN)

# Step 2: Assemble the entry point assembly code
$(ENTRY_OBJ): $(ENTRY) | $(BUILD_DIR)
	$(ASM) $(ASMOBJFLAGS) $(ENTRY) -o $(ENTRY_OBJ)

# Step 3: Compile the kernel C code
$(KERNEL_OBJ): $(KERNEL) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $(KERNEL) -o $(KERNEL_OBJ)

# compila il file strings.c per le funzioni di handling delle stringhe
$(STRINGS_OBJ): $(STRINGS) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $(STRINGS) -o $(STRINGS_OBJ)

# $(INTERRUPT_OBJ): $(INTERRUPT) | $(BUILD_DIR)
# 	$(CC) $(CFLAGS) -c $(INTERRUPT) -o $(INTERRUPT_OBJ)

$(BUILD_DIR)/idt.o : $(INTERRUPT_DIR)/idt.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $(INTERRUPT_DIR)/idt.c -o $(BUILD_DIR)/idt.o

$(BUILD_DIR)/isr.o : $(INTERRUPT_DIR)/isr.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $(INTERRUPT_DIR)/isr.c -o $(BUILD_DIR)/isr.o

$(BUILD_DIR)/interrupt.bin : $(INTERRUPT_DIR)/interrupt.asm | $(BUILD_DIR)
	$(ASM) $(ASMOBJFLAGS) $(INTERRUPT_DIR)/interrupt.asm -o $(BUILD_DIR)/interrupt.bin

$(MEM_OBJ): $(MEM) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $(MEM) -o $(MEM_OBJ)

# Step 4: Link the entry and kernel code together into a single binary
$(KERNEL_BIN): $(ENTRY_OBJ) $(KERNEL_OBJ) $(STRINGS_OBJ) $(MEM_OBJ) $(BUILD_DIR)/interrupt.bin $(BUILD_DIR)/isr.o $(BUILD_DIR)/idt.o
	$(LD) $(LDFLAGS) $(ENTRY_OBJ) $(KERNEL_OBJ) $(STRINGS_OBJ) $(MEM_OBJ) $(BUILD_DIR)/idt.o $(BUILD_DIR)/interrupt.bin $(BUILD_DIR)/isr.o -o $(KERNEL_BIN)

# Step 5: Create the OS image by combining the bootloader and kernel
$(OS_IMAGE): $(BOOTLOADER_BIN) $(KERNEL_BIN)
	python3 $(PYTHON_SCRIPT) $(BOOTLOADER_BIN) $(KERNEL_BIN) $(OS_IMAGE)

# Clean up the build directory and os-image
clean:
	rm -rf $(BUILD_DIR) $(OS_IMAGE)