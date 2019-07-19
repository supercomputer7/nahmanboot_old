#include <stdint.h>

#define DOUBLE_FAULT_VECTOR 0x8
#define NMI_VECTOR 0x2
#define GDT_CODE_SELECTOR 0x8
#define IDT_INTERRUPT_TYPE 0xE
#define IDT_INTERRUPT_PRESENT 0x80
#define IDT_INTERRUPT_NOT_PRESENT 0x7F
#define CALL_MEMORY_OPCODE 0x15FF // assembly call [memory] opcode
#define IRETD_OPCODE 0xFC
#define CLI_HLT_OPCODES 0xF4FA
struct IDTDescr {
    uint16_t offset_1; // offset bits 0..15
    uint16_t selector; // a code segment selector in GDT or LDT
    uint8_t zero;      // unused, set to 0
    uint8_t type_attr; // type and attributes, see below
    uint16_t offset_2; // offset bits 16..31
} __attribute__((packed));
typedef struct IDTDescr IDTDescriptor;

struct InterruptHandler {
    uint16_t call;
    uint32_t call_addr;
    uint8_t iret;
    uint32_t func_addr;
} __attribute__((packed));
typedef struct InterruptHandler ISRHandler;

void create_idt();
void blank_idt();
void create_fault_handlers();
void double_fault_exception();
void nmi_exception();
void install_isrcode(uint32_t *isr_addr,uint32_t *func_addr_ptr);
void install_interrupt_service(uint32_t *pointer,uint16_t index);
void remove_interrupt_service(uint16_t index);

 
