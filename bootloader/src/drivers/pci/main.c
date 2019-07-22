#include "drivers/pci/main.h"
#include "drivers/io/ioport.h"

PCIDescriptor pciTable;

void initPCIsubsystem()
{
    pciTable.mcfg = (ACPIMCFG*)GetACPITablePointer((void*)GetRSDP(), "MCFG");
    uint32_t *ptr = (uint32_t*)0x800;
    *ptr = &pciTable;
    PCIenumeration();
}

uint16_t ReadPCIregister(uint16_t seg,uint8_t bus,uint8_t device,uint8_t func, uint8_t offset)
{
    if(pciTable.mcfg != NULL)
    {
        // PCIe using MCFG table
        ACPIMCFG *ptr = (ACPIMCFG*)pciTable.mcfg;
        PCIeSpaceConfig *space = (PCIeSpaceConfig*)&ptr->spaces[seg];

        uint32_t addr = 0;

        addr = ((PCIeDeviceConfigSpaceSize * device * FunctionsPerDevice + PCIeDeviceConfigSpaceSize * func + offset) >> 1); // divide by sizeof(uint16_t)

        uint16_t *pcie_arr = (uint16_t*)(uint32_t)space->base_addr;

        return pcie_arr[addr];
    }
    else
    {
        uint32_t lbus  = (uint32_t)bus;
        uint32_t ldevice = (uint32_t)device;
        uint32_t lfunc = (uint32_t)func;
        // Read using IO ports, legacy method
        uint32_t address;
        
        uint16_t tmp = 0;
 
        /* create configuration address as per Figure 1 */
        address = (uint32_t)((lbus << 16) | (ldevice << 11) |
              (lfunc << 8) | (offset & 0xfc) | ((uint32_t)0x80000000));
 
        /* write out the address */
        outl(0xCF8, address);
        /* read in the data */
        tmp = (uint16_t)((inl(0xCFC) >> ((offset & 2) * 8)) & 0xffff);
        return (tmp);
    }
}
/* 
uint32_t WritePCIregister(uint8_t seg,uint8_t bus,uint8_t device,uint8_t func, uint8_t offset)
{
    if(pciTable.mcfg != NULL)
    {
        // Write to PCIe register using MCFG table
    }
    else
    {
        // Write using IO ports, legacy method
    }
}
*/
void PCIenumeration()
{
    pciTable.devices = (PCIDevice*)0x70000;
    pciTable.devices->uniq_id = 1;
    if(pciTable.mcfg != NULL)
    {
        // Enumeration PCIe using MCFG table
        ACPIMCFG *ptr = (ACPIMCFG*)pciTable.mcfg;
        uint32_t seg_entries = (ptr->h.Length - sizeof(ptr->h))/sizeof(PCIeSpaceConfig);

        uint32_t count_devices = 0;

        for(uint32_t seg=0; seg<seg_entries; ++seg)
        {
            uint8_t start_bus = ptr->spaces[seg].pci_start_bus;
            uint8_t end_bus = ptr->spaces[seg].pci_end_bus;
            for(uint8_t bus=start_bus; bus < end_bus; ++bus)
            {
                for(uint8_t device=0; device < DevicesPerBus; ++device)
                {
                    for(uint8_t func=0; func < FunctionsPerDevice; ++func)
                    {
                        if(ReadPCIregister(seg,bus,device,func,0) != 0xffff && ReadPCIregister(seg,bus,device,func,2) != 0xffff)
                        {
                            pciTable.devices[count_devices].uniq_id = count_devices;
                            pciTable.devices[count_devices].vendor_id = ReadPCIregister(seg,bus,device,func,0);
                            pciTable.devices[count_devices].device_id = ReadPCIregister(seg,bus,device,func,2);
                            //pciTable.devices[count_devices]->
                        }
                    }
                }
            }
        }
    }
    else
    {
        // Enumeration using IO ports, legacy method
        //for(int )
    }
}