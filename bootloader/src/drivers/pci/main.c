#include "drivers/pci/main.h"

PCIDeviceDescriptor* PCIScan(PCIDescriptor* pciDescriptor,BumpAllocator* bump_allocator)
{
    PCIDeviceDescriptor* list = (PCIDeviceDescriptor*)bump_allocator->allocate(bump_allocator,sizeof(PCIDeviceDescriptor));
    pciDescriptor->mcfg = (ACPIMCFG*)GetACPITablePointer((void*)GetRSDP(), "MCFG");
    if(pciDescriptor->mcfg == NULL) { forcePCIMode(pciDescriptor); } else { forcePCIeMode(pciDescriptor); }
    PCIenumeration(list,pciDescriptor,bump_allocator);
    return list;
}
PCIStorageControllerList* PCIScanStorageDevices(PCIDescriptor* pciDescriptor,BumpAllocator* bump_allocator)
{

    PCIStorageControllerList* list2 = (PCIStorageControllerList*)bump_allocator->allocate(bump_allocator,sizeof(PCIStorageControllerList));

    int device_count=0;
    for(int i=0; i < (int)pciDescriptor->pci_devices_count; ++i)
    {
        if(pciDescriptor->devices[i].class_code == PCI_MassStroageController)
        {
            bump_allocator->allocate(bump_allocator,sizeof(PCIDeviceDescriptor*));
            list2->devices[device_count] = &pciDescriptor->devices[i];
            ++device_count;
        }     
    }
    
    list2->count = device_count;
    return list2;
}




