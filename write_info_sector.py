import os, sys

def main():
    kernel_bin = sys.argv[1]
    os_image = sys.argv[2]
    
    size = os.path.getsize(kernel_bin)
    sectors = (size + 511) // 512

    print(f"Kernel is {sectors} sector{'s' if sectors > 1 else ''}")

    with open(os_image, 'ab') as f:
        f.write(sectors.to_bytes(2, byteorder='little') + b'\x00' * 510)

if __name__ == "__main__":
    main()