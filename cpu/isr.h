#include "common.h"

typedef struct registers {
    u32 ds; // data seg descriptor
    u32 edi, esi, ebp, esp, ebx, edx, ecx, eax; // pushed by pusha
    u32 int_no, err_code; // int no and err code
    u32 eip, cs, eflags, useresp, ss; // pushed by processor automatically
} registers_t;

typedef void (*isr_t)(registers_t);
void register_interrupt_handler(u8 n, isr_t handler);

//all the stuffs

#define IRQ0 32
#define IRQ1 33
#define IRQ2 34
#define IRQ3 35
#define IRQ4 36
#define IRQ5 37
#define IRQ6 38
#define IRQ7 39
#define IRQ8 40
#define IRQ9 41
#define IRQ10 42
#define IRQ11 43
#define IRQ12 44
#define IRQ13 45
#define IRQ14 46
#define IRQ15 47
