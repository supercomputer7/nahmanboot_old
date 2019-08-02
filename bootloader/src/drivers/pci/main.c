#include "drivers/pci/main.h"
#include "drivers/pci/enum.h"
#include "drivers/pci/pci.h"
#include "drivers/pci/pcie.h"

PCIDescriptor pciTable;

void PCIScan(uint32_t list_addr)
{
    pciTable.mcfg = (ACPIMCFG*)GetACPITablePointer((void*)GetRSDP(), "MCFG");
    if(pciTable.mcfg == NULL)
    {
        forcePCIMode(&pciTable);
    }
    else
    {
        forcePCIeMode(&pciTable);
    }
    uint32_t *ptr = (uint32_t*)0x800;
    ptr[0] = (uint32_t)&pciTable;

    PCIenumeration(list_addr,&pciTable);
}





