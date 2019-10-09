#ifndef PCIENUM_H
#define PCIENUM_H

#include "drivers/pci/defs.h"
#include "memory/bump_alloc.h"

void PCIenumeration(PCIDeviceDescriptor* list_addr, PCIDescriptor* pciTable,BumpAllocator* bump_allocator);

#endif