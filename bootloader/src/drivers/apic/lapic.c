#include "drivers/apic/lapic.h"
#include <stdint.h>

#define LAPIC_BASE_MMIO 0xfee00000
void disableLAPIC()
{
    uint32_t *reg = (uint32_t*)(LAPIC_BASE_MMIO+0xf0);
    uint32_t reg_value = *reg & 0xfffffeff;
    *reg = reg_value;
}
void enableLAPIC()
{
    uint32_t *reg = (uint32_t*)(LAPIC_BASE_MMIO+0xf0);
    uint32_t reg_value = *reg;
    *reg = reg_value | 0x100;
}

void EndInterrupt()
{
    uint32_t *reg = (uint32_t*)(LAPIC_BASE_MMIO+0xB0);
    *reg = 0;
}