# SkibidiOS

An attempt to make a 32-bit OS.

Things implemented until now:
  1) Bootloader
      a) print something from bootloader using BIOS interrupts
      b) switch from 16-bit Real Mode to 32-bit Protected mode
      c) Read from disk, set up GDT and load Kernel in Memory

  2) Kernel:
      a) print functions using Video memory at 0xB8000
      b) leggi e scrivi da terminale
      c) Heap, (malloc() e free()
      d) Paging
