#ifndef PCI_PCIFUNC_H
#define PCI_PCIFUNC_H


#include "drivers/pci/defs.h"
#include "drivers/io/ioport.h"

void forcePCIMode(PCIDescriptor* pciTable);
bool isPCIMode(PCIDescriptor* pciTable);
PCIRegisterValue ReadPCIRegister(__attribute__((unused)) PCIDescriptor* pciTable,__attribute__((unused)) PCISegment seg,PCIBus bus,PCIDevice device,PCIFunction func, PCIOffsetSelector offset);
void WritePCIRegister(__attribute__((unused)) PCIDescriptor* pciTable,__attribute__((unused)) PCISegment seg,PCIBus bus,PCIDevice device,PCIFunction func,PCIOffsetSelector offset,PCIRegisterValue value);
PCIAddress CalcPCIAddress(PCIBus bus,PCIDevice device,PCIFunction func,PCIOffsetSelector offset);

#endif