#ifndef INTERRUPT_H
#define INTERRUPT_H

#define NUM_INTERRUPTS 256
#include <stdint.h>

struct idt_entry {
    uint16_t offset_low;   // Offset basso (bits 0-15) dell'handler dell'interrupt
    uint16_t selector;     // Codice di segmento (selector del segmento di codice nella GDT)
    uint8_t zero;          // Sempre 0
    uint8_t type_attr;     // Tipo e attributi (flags, come DPL e tipo di interrupt/gate)
    uint16_t offset_high;  // Offset alto (bits 16-31) dell'handler dell'interrupt
} __attribute__((packed));

struct idt_ptr {
    uint16_t limit;    // Dimensione della tabella IDT meno 1
    uint32_t base;     // Indirizzo di base della tabella IDT
} __attribute__((packed));

extern struct idt_entry idt[NUM_INTERRUPTS];
extern struct idt_ptr idtp;

void init_idt();
void set_idt_entry(int num, uint32_t base, uint16_t sel, uint8_t flags);
void isr32_handler();

#endif