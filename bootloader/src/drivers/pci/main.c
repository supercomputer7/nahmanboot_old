#include "drivers/pci/main.h"

PCIDescriptor pciTable;

void PCIScan(PCIDescriptor* pciDescriptor, uint32_t list_addr)
{
    PCIDescriptor* tmp;
    if(pciDescriptor != NULL) { tmp = pciDescriptor; } else { tmp = &pciTable; }
    
    tmp->mcfg = (ACPIMCFG*)GetACPITablePointer((void*)GetRSDP(), "MCFG");

    if(tmp->mcfg == NULL) { forcePCIMode(tmp); } else { forcePCIeMode(tmp); }

    uint32_t *ptr = (uint32_t*)0x800;
    ptr[0] = (uint32_t)tmp;

    PCIenumeration(list_addr,tmp);
    
}





