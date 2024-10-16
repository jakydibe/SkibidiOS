#include "keyboard.h"
#include "../utils/strings.h"
#include "../interrupt/isr.h"
#include "../io/io.h"

/* Can't do this with a loop because we need the address
 * of the function names */

// Example with handling Shift (just a simplified concept)
char scancode_to_ascii[] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b', // 0x00 - 0x0F
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',   // 0x10 - 0x1F
    0,  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',         // 0x20 - 0x2F
    0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0,            // 0x30 - 0x3F
    '*', 0, ' ' // Spacebar
};

// Function to convert scancode to ASCII
char convert_scancode_to_ascii(unsigned char scancode) {
    if (scancode > 0 && scancode < sizeof(scancode_to_ascii)) {
        return scancode_to_ascii[scancode];
    }
    return 0;
}

void keyboard_handler(registers_t r) {

    //unsigned char scancode = insb(0x64); //cosa e' 0x60? e' la porta del controller della tastiera
    unsigned char scancode = insb(0x60); //cosa e' 0x60? e' la porta del controller della tastiera
    char printable_char = convert_scancode_to_ascii(scancode);
    char mask = 0b10000000;
    if(mask & scancode )
        return;
    putc(printable_char);
}

void keyboard_handler_init(){
    puts("Registrato handler alla testiera: \n");
    register_interrupt_handler(33,(isr_t) keyboard_handler);
}