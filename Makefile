SRC_DIR = kernel
BUILD_DIR = build
OS_IMAGE = skibidiOS

BOOTLOADER_BIN = $(BUILD_DIR)/boot.bin
KERNEL_BIN = $(BUILD_DIR)/kernel.bin

PYTHON_SCRIPT = craft_image.py

# Compilation and linking flags
ASMOBJFLAGS = -f elf32
CFLAGS = -m32 -ffreestanding -nostdlib -fno-pic -fno-pie -static
LDFLAGS = -m elf_i386 -T linker.ld --oformat binary

# Tools
ASM = nasm
CC := $(shell command -v i686-elf-gcc >/dev/null 2>&1 && echo i686-elf-gcc || echo gcc)
LD := $(shell command -v i686-elf-ld >/dev/null 2>&1 && echo i686-elf-ld || echo ld)

# Find all .c and .asm files in the current directory and subdirectories
ASM_SRCS = $(shell find $(SRC_DIR) -type f -name "*.asm")
C_SRCS = $(shell find $(SRC_DIR) -type f -name "*.c")

# Convert .c and .asm file names to .o file names in the build directory
ASM_OBJS = $(patsubst $(SRC_DIR)/%.asm, $(BUILD_DIR)/%.o, $(ASM_SRCS))
C_OBJS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(C_SRCS))

# Define colors for echo output
COLOR_GREEN = \033[0;32m
COLOR_BLUE = \033[0;34m
COLOR_CYAN = \033[0;36m
COLOR_YELLOW = \033[0;33m
COLOR_RED = \033[0;31m
COLOR_RESET = \033[0m

# Default target: build the OS image
all: $(OS_IMAGE)
	@echo -e "${COLOR_GREEN}Build completed.${COLOR_RESET}"

# Rule to compile bootloader
$(BOOTLOADER_BIN): bootloader/boot.asm
	@mkdir -p $(dir $@)
	@echo -e "${COLOR_BLUE}Assembling bootloader/boot.asm...${COLOR_RESET}"
	@nasm -f bin bootloader/boot.asm -o $(BOOTLOADER_BIN)

# Rule for compiling .c files to .o files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	@echo -e "${COLOR_YELLOW}Compiling $<...${COLOR_RESET}"
	@$(CC) $(CFLAGS) -c $< -o $@
# Rule for compiling .asm files to .o files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.asm
	@mkdir -p $(dir $@)
	@echo -e "${COLOR_YELLOW}Assembling $<...${COLOR_RESET}"
	@$(ASM) $(ASMOBJFLAGS) $< -o $@

# Rule to build the kernel
$(KERNEL_BIN): $(C_OBJS) $(ASM_OBJS)
	@echo -e "${COLOR_CYAN}Linking kernel...${COLOR_RESET}"
	@$(LD) $(LDFLAGS) $(C_OBJS) $(ASM_OBJS) -o $(KERNEL_BIN)

# Rule to build the os image
$(OS_IMAGE): $(BOOTLOADER_BIN) $(KERNEL_BIN)
	@echo -e "${COLOR_BLUE}Creating OS image...${COLOR_RESET}"
	@python3 $(PYTHON_SCRIPT) $(BOOTLOADER_BIN) $(KERNEL_BIN) $(OS_IMAGE)

# Clean up object files and executable
clean:
	@echo -e "${COLOR_RED}Cleaning up...${COLOR_RESET}"
	@rm -rf $(BUILD_DIR) $(OS_IMAGE)
	@echo -e "${COLOR_GREEN}Clean completed.${COLOR_RESET}"