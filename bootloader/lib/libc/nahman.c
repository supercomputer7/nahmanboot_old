#include "nahman.h"
#include "string.h"
void panic()
{
	uint16_t* vga = (uint16_t*)0xb8000;
    for (int i = 0; i < 2000;++i) // clear screen!
        vga[i] = 0;

    const short color = 0x0F00;
    const char* hello = "NahmanBOOT PANIC: Abort!";
    vga = (uint16_t*)0xb8000;
    for (int i = 0; i < (int)strlen(hello);++i)
        vga[i] = color | hello[i];

    asm("cli");
    asm("hlt");
}