#include "ioapic.h"
#include "lapic.h"
#include "pic.h"
#include "idt.h"

void disable_interrupts();
void disable_interrupts_temporarily();
void prepare_interrupts();
void enable_interrupts();
void enable_idt();
void map_ioapic();