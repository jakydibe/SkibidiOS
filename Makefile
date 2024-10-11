# Define the source file locations
BOOTLOADER_DIR = bootloader
KERNEL_DIR = kernel

# Define the names of your source files
BOOTLOADER = $(BOOTLOADER_DIR)/boot.asm
ENTRY = $(KERNEL_DIR)/entry.asm
KERNEL = $(KERNEL_DIR)/kernel.c
PYTHON_SCRIPT = write_info_sector.py

# Define the output directory for intermediate files
BUILD_DIR = build

# Define the output file names (stored in the build directory)
BOOTLOADER_BIN = $(BUILD_DIR)/boot.bin
ENTRY_OBJ = $(BUILD_DIR)/entry.o
KERNEL_OBJ = $(BUILD_DIR)/kernel.o
KERNEL_BIN = $(BUILD_DIR)/kernel.bin

# The final OS image will be in the root directory
OS_IMAGE = os-image

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

# Step 4: Link the entry and kernel code together into a single binary
$(KERNEL_BIN): $(ENTRY_OBJ) $(KERNEL_OBJ)
	$(LD) $(LDFLAGS) $(ENTRY_OBJ) $(KERNEL_OBJ) -o $(KERNEL_BIN)

# Step 5: Create the OS image by combining the bootloader and kernel
$(OS_IMAGE): $(BOOTLOADER_BIN) $(KERNEL_BIN)
	# Ensure the bootloader is exactly 512 bytes
	dd if=$(BOOTLOADER_BIN) of=$(OS_IMAGE) bs=512 count=1 conv=notrunc

	# Append info sector
	python $(PYTHON_SCRIPT) $(KERNEL_BIN) $(OS_IMAGE)

	# Append the kernel binary to the bootloader
	cat $(KERNEL_BIN) >> $(OS_IMAGE)

	@echo "Size of $(OS_IMAGE): $$(ls -l $(OS_IMAGE) | awk '{print $$5}') bytes"

	dd if=/dev/zero bs=1 count=$$(expr 4096 - $$(stat -c%s $(OS_IMAGE))) >> $(OS_IMAGE)

	@echo "Final size of $(OS_IMAGE): $$(ls -l $(OS_IMAGE) | awk '{print $$5}') bytes"

# Clean up the build directory and os-image
clean:
	rm -rf $(BUILD_DIR) $(OS_IMAGE)