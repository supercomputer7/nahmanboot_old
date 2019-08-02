#include "drivers/pci/general.h"

PCIRegisterValue ReadPCI(PCIDescriptor* pciTable,PCISegment seg,PCIBus bus,PCIDevice device,PCIFunction func, PCIOffsetSelector offset)
{
    return (pciTable->readPCIreg)(pciTable,seg,bus,device,func,offset);
}

void WritePCI(PCIDescriptor* pciTable,PCISegment seg,PCIBus bus,PCIDevice device,PCIFunction func,PCIOffsetSelector offset,PCIRegisterValue value)
{
    (pciTable->writePCIreg)(pciTable,seg,bus,device,func,offset,value);
}
PCIMode queryPCIMode(PCIDescriptor* pciTable)
{
    return (PCIMode)pciTable->pciMethod;
}