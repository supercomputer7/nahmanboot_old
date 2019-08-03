#include "drivers/pci/enum.h"
#include "drivers/pci/device.h"
#include "drivers/pci/pcie.h"

void PCIenumeration(uint32_t list_addr, PCIDescriptor* pciTable)
{
    pciTable->devices = (PCIDeviceDescriptor*)(list_addr);
    pciTable->devices[0].uniq_id = 1;
 
    uint32_t count_devices = 0;
    uint32_t seg_entries;

    ACPIMCFG *ptr = (ACPIMCFG*)pciTable->mcfg;
    if(checkPCIeExisting())
        seg_entries = (ptr->h.Length - sizeof(ptr->h))/sizeof(PCIeSpaceConfig); // Enumeration PCIe using MCFG table
    else
        seg_entries = 1; // no PCIe, using one segment

    for(uint32_t seg=0; seg<seg_entries; ++seg)
    {
        uint8_t start_bus;
        uint8_t end_bus;
        if(checkPCIeExisting())
        {
            start_bus = ptr->spaces[seg].pci_start_bus;
            end_bus = ptr->spaces[seg].pci_end_bus;
        }
        else
        {
            start_bus = 0x0;
            end_bus = 0xFF;
        }
        for(uint8_t bus=start_bus; bus < end_bus; ++bus)
        {
            for(uint8_t device=0; device < DevicesPerBus; ++device)
            {
            for(uint8_t func=0; func < FunctionsPerDevice; ++func)
                {
                    if(getPCIVendorDevice(pciTable,seg,bus,device,func) != 0xffff && getPCIDeviceID(pciTable,seg,bus,device,func) != 0xffff)
                    {
                        pciTable->devices[count_devices].uniq_id = count_devices;

                        pciTable->devices[count_devices].segment = seg;
                        pciTable->devices[count_devices].bus = bus;
                        pciTable->devices[count_devices].device_number = device;
                        pciTable->devices[count_devices].function_number = func;

                        pciTable->devices[count_devices].vendor_id = getPCIVendorDevice(pciTable,seg,bus,device,func);
                        pciTable->devices[count_devices].device_id = getPCIDeviceID(pciTable,seg,bus,device,func);

                        pciTable->devices[count_devices].class_code = getPCIDeviceClassCode(pciTable,seg,bus,device,func);
                        pciTable->devices[count_devices].subclass = getPCIDeviceSubClass(pciTable,seg,bus,device,func);
                        pciTable->devices[count_devices].progif = getPCIDeviceProgIF(pciTable,seg,bus,device,func);
                        pciTable->devices[count_devices].revision = getPCIDeviceRevisionID(pciTable,seg,bus,device,func);
                        ++count_devices;
                    }
                }
            }
        }
    }
    pciTable->pci_devices_count = count_devices;
    
}