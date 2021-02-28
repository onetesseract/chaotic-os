#include "common.h"
#include "isr.h"
#include "idt.h"
#include "drivers/screen.h"
#include "libc/string.h"

isr_t interrupt_handlers[256];

void register_interrupt_handlers(u8 n, isr_t handler) {
    interrupt_handlers[n] = handler;
}
void isr_handler(registers_t regs) {
    kprint("recieved interrupt: ");
    char* regs_str;
    int_to_ascii(regs.int_no, regs_str);
    kprint(regs_str);
}

void irq_handler(registers_t regs) {
    // sends EOI to the slave if it was the slave PIC
    if (regs.int_no >= 40) {
        // reset the slave
        outb(0xA20, 0x20);
    }
    // reset master
    outb(0x20, 0x20);

    if(interrupt_handlers[regs.int_no] != 0) {
        isr_t handler = interrupt_handlers[regs.int_no];
        handler(regs);
    }
}