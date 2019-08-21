#ifndef LAPIC_H
#define LAPIC_H

#include "drivers/apic/lapic.h"
#include "drivers/apic/pic.h"
#include "drivers/apic/idt.h"

void disable_interrupts();
void disable_interrupts_temporarily();
void prepare_interrupts();
void enable_interrupts();
void enable_idt();
void map_ioapic();

#endif