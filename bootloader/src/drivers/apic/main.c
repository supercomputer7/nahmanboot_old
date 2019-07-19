#include "drivers/apic/main.h"

void disable_interrupts()
{
    asm("cli");
    disablePIC();
    disableLAPIC();
    asm("sti");
}
void disable_interrupts_temporarily()
{
    asm("cli");
    disableLAPIC();
}
void prepare_interrupts()
{
    disable_interrupts();
    enable_idt();
    enable_interrupts();
}
void enable_idt()
{
    create_idt();
}
void enable_interrupts()
{
    asm("cli");
    enableLAPIC();
    asm("sti");
}
void map_ioapic()
{

}