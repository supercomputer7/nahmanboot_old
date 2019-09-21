#ifndef PCIMAIN_H
#define PCIMAIN_H

#include "drivers/pci/defs.h"
#include "drivers/pci/enum.h"
#include "drivers/pci/pci.h"
#include "drivers/pci/pcie.h"

void PCIScan(PCIDescriptor* pciDescriptor, PCIDeviceDescriptor* list);
void PCIScanStorageDevices(PCIDescriptor* pciDescriptor,PCIStorageControllerList* list2);

#endif