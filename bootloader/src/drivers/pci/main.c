#include "drivers/pci/main.h"

PCIDescriptor pciTable;

void PCIScan(PCIDescriptor* pciDescriptor, PCIDeviceDescriptor* list)
{
    PCIDescriptor* tmp;
    if(pciDescriptor != NULL) { tmp = pciDescriptor; } else { tmp = &pciTable; }
    
    tmp->mcfg = (ACPIMCFG*)GetACPITablePointer((void*)GetRSDP(), "MCFG");

    if(tmp->mcfg == NULL) { forcePCIMode(tmp); } else { forcePCIeMode(tmp); }

    uint32_t *ptr = (uint32_t*)0x800;
    ptr[0] = (uint32_t)tmp;

    PCIenumeration(list,tmp);
}
void PCIScanStorageDevices(PCIDescriptor* pciDescriptor,PCIStorageControllerList* list2)
{
    int j=0;
    
    for(int i=0; i<pciDescriptor->pci_devices_count; ++i)
    {
        if(pciDescriptor->devices[i].class_code == PCI_MassStroageController)
        {
            list2->devices[j] = &pciDescriptor->devices[i];
            ++j;
        }     
    }
    
    list2->count = j;
    
}




