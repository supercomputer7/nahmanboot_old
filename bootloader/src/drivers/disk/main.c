#include "drivers/disk/main.h"
StorageDiskDescriptor* DiskEnumeration(PCIStorageControllerList* controllers,BumpAllocator* bump_allocator)
{
    StorageDiskDescriptor* tmp = (StorageDiskDescriptor*)bump_allocator->addr;

    for(int i=0; i< (int)controllers->count;++i)
    {
        bump_allocator->allocate(bump_allocator,sizeof(StorageDiskDescriptor));
        if(controllers->devices[i]->class_code == PCI_MassStroageController)
        {
            switch (controllers->devices[i]->subclass)
            {
                case IDE_StorageDevice:
                    tmp[i].operator.readData = &ReadIDE_Sectors;
                    tmp[i].header.controller = (PCIDeviceDescriptor*)controllers->devices[i];
                    tmp[i].header.storage_id = i;
                    break;
                case AHCI_StorageDevice:
                    //tmp[i].operator.readData = &readAHCI_Sectors;
                    tmp[i].header.controller = (PCIDeviceDescriptor*)controllers->devices[i];
                    tmp[i].header.storage_id = i;
                    break;
                case NVMe_StorageDevice:
                    break;
            }
        }
    }
    return tmp;
}