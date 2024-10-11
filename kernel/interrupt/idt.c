#include "idt.h"

idt_gate_t idt[IDT_ENTRIES];
idt_register_t idt_reg;

void set_idt_gate(int n, unsigned int handler) {
    idt[n].low_offset = (unsigned short)((handler) & 0xFFFF);
    idt[n].sel = KERNEL_CS;
    idt[n].always0 = 0;
    idt[n].flags = 0x8E; 
    idt[n].high_offset = (unsigned short)(((handler) >> 16) & 0xFFFF);
}

void set_idt() {
    idt_reg.base = (unsigned int) &idt;
    idt_reg.limit = IDT_ENTRIES * sizeof(idt_gate_t) - 1;
    /* Don't make the mistake of loading &idt -- always load &idt_reg */
    __asm__ __volatile__("lidtl (%0)" : : "r" (&idt_reg));
}