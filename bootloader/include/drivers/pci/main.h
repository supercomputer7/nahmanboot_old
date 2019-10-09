#ifndef PCIMAIN_H
#define PCIMAIN_H

#include "drivers/pci/defs.h"
#include "drivers/pci/enum.h"
#include "drivers/pci/pci.h"
#include "drivers/pci/pcie.h"

#include "memory/bump_alloc.h"

PCIDeviceDescriptor* PCIScan(PCIDescriptor* pciDescriptor, BumpAllocator* bump_allocator);
PCIStorageControllerList* PCIScanStorageDevices(PCIDescriptor* pciDescriptor,BumpAllocator* bump_allocator);

#endif