#include "drivers/pci/pci.h"
#include "drivers/pci/general.h"

void forcePCIMode(PCIDescriptor* pciTable)
{
    pciTable->pciMethod = PCIconventionalMode;
    pciTable->readPCIreg = &ReadPCIRegister;
    pciTable->writePCIreg = &WritePCIRegister;
}
bool isPCIMode(PCIDescriptor* pciTable)
{
    return queryPCIMode(pciTable) == PCIconventionalMode;
}

// Basic PCI Functions
PCIRegisterValue ReadPCIRegister(__attribute__((unused)) PCIDescriptor* pciTable,__attribute__((unused)) PCISegment seg,PCIBus bus,PCIDevice device,PCIFunction func, PCIOffsetSelector offset)
{
    PCIAddress address = CalcPCIAddress(bus,device,func,offset);
    // Read using IO ports, legacy method
    uint16_t tmp;
    /* write out the address */
    outl(0xCF8, address);
    /* read in the data */
    tmp = (uint16_t)((inl(0xCFC) >> ((offset & 2) * 8)) & 0xffff);
    return (tmp);
}

void WritePCIRegister(__attribute__((unused)) PCIDescriptor* pciTable,__attribute__((unused)) PCISegment seg,PCIBus bus,PCIDevice device,PCIFunction func,PCIOffsetSelector offset,PCIRegisterValue value)
{
    PCIAddress address = CalcPCIAddress(bus,device,func,offset);
    // Read using IO ports, legacy method
    /* write out the address */
    outl(0xCF8, address);
    /* write in the data */
    outl(0xCFC,(uint32_t)(value & 0xffff));
}

PCIAddress CalcPCIAddress(PCIBus bus,PCIDevice device,PCIFunction func,PCIOffsetSelector offset)
{
    uint32_t lbus  = (uint32_t)bus;
    uint32_t ldevice = (uint32_t)device;
    uint32_t lfunc = (uint32_t)func;
    uint8_t loffset = (uint8_t)(offset & 0xff);
    
    // Read using IO ports, legacy method
    uint32_t address;
    
    /* create configuration address */
    address = (uint32_t)((lbus << 16) | (ldevice << 11) |
        (lfunc << 8) | (loffset & 0xfc) | ((uint32_t)0x80000000));   
    return address;
}

