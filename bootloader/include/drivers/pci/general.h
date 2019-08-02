#ifndef PCIGENERAL_H
#define PCIGENERAL_H

#include "drivers/pci/defs.h"

PCIRegisterValue ReadPCI(PCIDescriptor* pciTable,PCISegment seg,PCIBus bus,PCIDevice device,PCIFunction func, PCIOffsetSelector offset);
void WritePCI(PCIDescriptor* pciTable,PCISegment seg,PCIBus bus,PCIDevice device,PCIFunction func,PCIOffsetSelector offset,PCIRegisterValue value);
PCIMode queryPCIMode(PCIDescriptor* pciTable);

#endif