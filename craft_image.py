import sys

# python $(PYTHON_SCRIPT) $(BOOTLOADER_BIN) $(KERNEL_BIN) $(OS_IMAGE)
def main():
    bootloader_bin = sys.argv[1]
    kernel_bin = sys.argv[2]
    os_image = sys.argv[3]
    
    # Read bootloader
    with open(bootloader_bin, 'rb') as f:
        bootloader = f.read()
        assert len(bootloader) == 512, "Bootloader must be 512 bytes"
        
        # Seek to the signature (last 2 bytes)
        f.seek(510)
        signature = f.read(2)
        assert signature == b"\x55\xAA", "Invalid bootloader signature"

    # Read kernel
    num_kernel_sectors = None
    kernel = None
    with open(kernel_bin, 'rb') as f:
        kernel = f.read()
        num_kernel_sectors = (len(kernel) + 511) // 512
        # padding
        kernel += b'\x00' * (512 - len(kernel) % 512)
    
    assert bootloader and kernel and num_kernel_sectors, "Error"
    paddingto1mb = (1024*1024)//512 - 2 - num_kernel_sectors
    assert paddingto1mb > 0, "kernel too big"

    # Create os-image
    with open(os_image, 'wb') as f:
        # write the bootloader
        f.write(bootloader)

        # write info sector
        f.write(num_kernel_sectors.to_bytes(2, byteorder='little') + b'\x00' * 510)

        # write kernel
        f.write(kernel)

        f.write(b'\x00' * (512 * paddingto1mb))

        f.write(b'\x00' * (1024*1024))

if __name__ == "__main__":
    main()