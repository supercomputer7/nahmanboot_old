#include "drivers/disk/nvme.h"
#include <stdbool.h>
#include <stdmath.h>

COMMANDSET getNVMeSupportedCommandSet(NVMeControllerRegisters* registers)
{
    return (COMMANDSET)(((registers->capabilities2) >> 5) & 0xff);
}
NVMeVersionRegister* getNVMeVersion(NVMeControllerRegisters* registers)
{
    return (NVMeVersionRegister*)&(registers->version);
}

uint16_t getNVMeMajorVersion(NVMeVersionRegister* reg)
{
    return reg->major_version_number;
}
uint8_t getNVMeMinorVersion(NVMeVersionRegister* reg)
{
    return reg->minor_version_number;
}
uint8_t getNVMeTertiaryVersion(NVMeVersionRegister* reg)
{
    return reg->tertiary_version_number;
}

uint16_t getNVMeMaximumSupportedQueueEntries(NVMeControllerRegisters* registers)
{
    return (uint16_t)((registers->capabilities1) & 0xffff);
}
bool isNVMeContiguousQueuesRequried(NVMeControllerRegisters* registers)
{
    return ((((registers->capabilities1) >> 16) & 0x1) == 1);
}
uint32_t getNVMeDoorbellStride(NVMeControllerRegisters* registers)
{
    uint8_t tmp = (uint8_t)(registers->capabilities2) & 0xf;
    return (uint32_t)power(2,(2+tmp));
}