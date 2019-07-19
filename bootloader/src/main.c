#include <stdint.h>
#include <string.h>

#include "drivers/acpi/powermodes.h"
#include "drivers/apic/main.h"

void welcome();
void halt();

void main()
{
    welcome();
    prepare_interrupts(); // Disable LAPIC & PIC for a moment, install IDT & enable LAPIC

    rebootACPI();

    //

    // Reset HPET, LAPIC timer & RTC
    // Initialize PCI & PCIe
    // Scan all disks
    // Scan all partitions in disks
    // Scan all filesystems
    // Scan all devices, search for input devices
    // Show results to user

    // halt!
    halt();
    
}
void welcome()
{
    const short color = 0x0F00;
    const char* hello = "Prepareing NahmanBOOT, Please Wait !";
    short* vga = (short*)0xb8000;
    for (int i = 0; i < (int)strlen(hello);++i)
        vga[i] = color | hello[i];
}
void halt()
{
    asm("cli");
    asm("hlt");
}

