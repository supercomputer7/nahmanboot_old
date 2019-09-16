#ifndef PCIMAIN_H
#define PCIMAIN_H

#include "drivers/pci/defs.h"
#include "drivers/pci/enum.h"
#include "drivers/pci/pci.h"
#include "drivers/pci/pcie.h"

void PCIScan(PCIDescriptor* pciDescriptor,uint32_t list_addr);


#endif