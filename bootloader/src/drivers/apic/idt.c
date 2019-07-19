#include "drivers/apic/idt.h"
#include <string.h>

void create_idt()
{
    blank_idt();
    create_fault_handlers();
}
void blank_idt()
{
    IDTDescriptor *descriptors = (IDTDescriptor*)0x20000;
    for(int i=0; i< 256;++i)
    {
        descriptors[i].offset_1 = 0;
        descriptors[i].selector = GDT_CODE_SELECTOR;
        descriptors[i].type_attr = IDT_INTERRUPT_TYPE;
        descriptors[i].offset_2 = 0;
    }
}
void create_fault_handlers()
{
    //install_interrupt_service((uint32_t*)0x40000,2);
    //remove_interrupt_service(2);
    
    install_isrcode((uint32_t*)0x40000,(uint32_t*)&double_fault_exception);
    install_interrupt_service((uint32_t*)0x40000,DOUBLE_FAULT_VECTOR);

    install_isrcode((uint32_t*)0x50000,(uint32_t*)&nmi_exception);
    install_interrupt_service((uint32_t*)0x50000,NMI_VECTOR);
    //remove_interrupt_service(NMI_VECTOR);
    //remove_interrupt_service(DOUBLE_FAULT_VECTOR);

}

void double_fault_exception()
{
    uint16_t* vga = (uint16_t*)0xb8000;
    for (int i = 0; i < 2000;++i) // clear screen!
        vga[i] = 0;

    const short color = 0x0F00;
    const char* hello = "NahmanBOOT PANIC: Unhandled Excpetion. Abort!";
    vga = (uint16_t*)0xb8000;
    for (int i = 0; i < (int)strlen(hello);++i)
        vga[i] = color | hello[i];
    asm("cli");
    asm("hlt");
}

void nmi_exception()
{
    uint16_t* vga = (uint16_t*)0xb8000;
    for (int i = 0; i < 2000;++i) // clear screen!
        vga[i] = 0;

    const short color = 0x0F00;
    const char* hello = "NahmanBOOT PANIC: NMI Exception, Hardware failure. Abort!";
    vga = (uint16_t*)0xb8000;
    for (int i = 0; i < (int)strlen(hello);++i)
        vga[i] = color | hello[i];
    asm("cli");
    asm("hlt");
}

void install_isrcode(uint32_t *isr_addr,uint32_t *func_addr_ptr)
{
    ISRHandler *ptr = (ISRHandler*)isr_addr;
    ptr->call =  (uint16_t)CALL_MEMORY_OPCODE; // assembly call [memory] opcode
    ptr->call_addr = (uint32_t)(&ptr->func_addr);
    ptr->iret = ((uint8_t)IRETD_OPCODE); // assembly iretd
    ptr->func_addr = (uint32_t)func_addr_ptr;
}

void install_interrupt_service(uint32_t *pointer,uint16_t index)
{
    IDTDescriptor *descriptors = (IDTDescriptor*)0x20000;
    uint16_t lowptr = (uint16_t)((uint32_t)pointer & 0xffff);
    uint16_t highptr = (uint16_t)((uint32_t)pointer >> 16);
    if(index <= 256)
    {
        descriptors[index].offset_1 = lowptr;
        descriptors[index].selector = GDT_CODE_SELECTOR;
        descriptors[index].type_attr = IDT_INTERRUPT_PRESENT | IDT_INTERRUPT_TYPE;
        descriptors[index].offset_2 = highptr;
    }
}
void remove_interrupt_service(uint16_t index)
{
    IDTDescriptor *descriptors = (IDTDescriptor*)0x20000;
    if(index <= 256)
    {
        descriptors[index].type_attr = descriptors[index].type_attr & IDT_INTERRUPT_NOT_PRESENT;
    }
}