#include "interrupt.h"
#include "strings.h"
#include "mem.h"

struct idt_entry idt[NUM_INTERRUPTS];
struct idt_ptr idtp;

void lidt(struct idt_ptr* idtp){
    __asm__ (
        "lidt (%0)"   // Istruzione assembly
        :
        : "r" (idtp)  // Specifica che 'idtp' è un input register //carica l'indirizzo in un registro del processore
    );
}

void set_idt_entry(int num, uint32_t base, uint16_t sel, uint8_t flags) {
    idt[num].offset_low = base & 0xFFFF;
    idt[num].selector = sel;
    idt[num].zero = 0;
    idt[num].type_attr = flags;
    idt[num].offset_high = (base >> 16) & 0xFFFF;
}

void init_idt() {
    idtp.limit = (sizeof(struct idt_entry) * NUM_INTERRUPTS) - 1;
    idtp.base = (uint32_t)&idt;

    memset(&idt, 0, sizeof(struct idt_entry) * NUM_INTERRUPTS);
    // Imposta tutte le voci IDT a zero

    //set_idt_entry(13, (uint32_t)isr32_handler, 0x08, 0x8E);  // Timer interrupt, ad esempio
    for(int i = 0; i < 256; i++){
        // Imposta le voci dell'IDT qui (ISR, interrupt hardware, ecc.)
        // if(i==13)
        //     continue;
        set_idt_entry(i, (uint32_t)isr32_handler, 0x06, 0x8E);  // Timer interrupt, ad esempio
    }
    
    // Carica la nuova IDT con l'istruzione "lidt"
    lidt(&idtp);
}


// global isr32
// isr32:
//     cli                     ; Disabilita gli interrupt
//     pusha                   ; Salva tutti i registri generali
//     call isr32_handler       ; Chiama il gestore C della ISR
//     popa                    ; Ripristina i registri
//     sti                     ; Riabilita gli interrupt
//     iret                    ; Ritorna dall'interrupt

//LEI E' UNA EMO DEL CAZZO, MI PROPONE COSE STRANE, MA IO PREDILIGO IL SESSO CLASSICO
void isr32_handler() {
    /* BLACK MAGIC – strongly discouraged! */
    __asm__("pusha");
    putc('C');
    __asm__("popa; leave; iret"); /* BLACK MAGIC! */
}
