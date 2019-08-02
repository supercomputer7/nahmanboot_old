#ifndef PCI_PCIEFUNC_H
#define PCI_PCIEFUNC_H

#include "drivers/pci/defs.h"

bool checkPCIeExisting();
void forcePCIeMode(PCIDescriptor* pciTable);
bool isPCIeMode(PCIDescriptor* pciTable);
PCIRegisterValue ReadPCIeRegister(PCIDescriptor* pciTable,PCISegment seg,PCIBus bus,PCIDevice device,PCIFunction func, PCIOffsetSelector offset);
PCIAddress CalcPCIeAddress(PCIBus bus,PCIDevice device,PCIFunction func,PCIOffsetSelector offset);
void WritePCIeRegister(PCIDescriptor* pciTable,PCISegment seg,PCIBus bus,PCIDevice device,PCIFunction func,PCIOffsetSelector offset,PCIRegisterValue value);

#endif