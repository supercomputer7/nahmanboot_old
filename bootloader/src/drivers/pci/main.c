#include "drivers/pci/main.h"
#include "drivers/io/ioport.h"

PCIDescriptor pciTable;

void PCIScan(uint32_t list_addr)
{
    pciTable.mcfg = (ACPIMCFG*)GetACPITablePointer((void*)GetRSDP(), "MCFG");
    if(pciTable.mcfg == NULL)
    {
        forcePCIMode();
    }
    else
    {
        forcePCIeMode();
    }
    uint32_t *ptr = (uint32_t*)0x800;
    ptr[0] = (uint32_t)&pciTable;
    PCIenumeration(list_addr);
}

// Abstracted PCI Subsystem Functions

bool checkPCIeExisting()
{
    return GetACPITablePointer((void*)GetRSDP(), "MCFG") != NULL;
}

void forcePCIeMode()
{
    pciTable.pciMethod = PCIExpressMode;
    pciTable.readPCIreg = &ReadPCIeRegister;
    pciTable.writePCIreg = &WritePCIeRegister;
}
void forcePCIMode()
{
    pciTable.pciMethod = PCIconventionalMode;
    pciTable.readPCIreg = &ReadPCIRegister;
    pciTable.writePCIreg = &WritePCIRegister;
}
PCIMode queryPCIMode()
{
    return (PCIMode)pciTable.pciMethod;
}
bool isPCIMode()
{
    return queryPCIMode() == PCIconventionalMode;
}
bool isPCIeMode()
{
    return queryPCIMode() == PCIExpressMode;
}

// Basic PCI Functions

PCIRegisterValue ReadPCI(PCISegment seg,PCIBus bus,PCIDevice device,PCIFunction func, PCIOffsetSelector offset)
{
    return (*pciTable.readPCIreg)(seg,bus,device,func,offset);
}
PCIRegisterValue ReadPCIeRegister(PCISegment seg,PCIBus bus,PCIDevice device,PCIFunction func, PCIOffsetSelector offset)
{
    if(pciTable.mcfg == NULL)
        return 0;

    // PCIe using MCFG table
    ACPIMCFG *ptr = (ACPIMCFG*)pciTable.mcfg;
    PCIeSpaceConfig *space = (PCIeSpaceConfig*)&ptr->spaces[seg];
    PCIAddress addr = CalcPCIeAddress(bus,device,func,offset);
    PCIRegisterValue *pcie_arr = (PCIRegisterValue*)(uint32_t)space->base_addr;
    return pcie_arr[addr];
}
PCIRegisterValue ReadPCIRegister(__attribute__((unused)) PCISegment seg,PCIBus bus,PCIDevice device,PCIFunction func, PCIOffsetSelector offset)
{
    PCIAddress address = CalcPCIAddress(bus,device,func,offset);
    // Read using IO ports, legacy method
    uint16_t tmp;
    /* write out the address */
    outl(0xCF8, address);
    /* read in the data */
    tmp = (uint16_t)((inl(0xCFC) >> ((offset & 2) * 8)) & 0xffff);
    return (tmp);
}


PCIAddress CalcPCIeAddress(PCIBus bus,PCIDevice device,PCIFunction func,PCIOffsetSelector offset)
{
    return ((
             PCIeDeviceConfigSpaceSize * DevicesPerBus * FunctionsPerDevice * bus + 
             PCIeDeviceConfigSpaceSize * FunctionsPerDevice * device + 
             PCIeDeviceConfigSpaceSize * func + offset) >> 1 // divide by sizeof(uint16_t)
            ); 
}
PCIAddress CalcPCIAddress(PCIBus bus,PCIDevice device,PCIFunction func,PCIOffsetSelector offset)
{
    uint32_t lbus  = (uint32_t)bus;
    uint32_t ldevice = (uint32_t)device;
    uint32_t lfunc = (uint32_t)func;
    uint8_t loffset = (uint8_t)(offset & 0xff);
    
    // Read using IO ports, legacy method
    uint32_t address;
    
    /* create configuration address */
    address = (uint32_t)((lbus << 16) | (ldevice << 11) |
        (lfunc << 8) | (loffset & 0xfc) | ((uint32_t)0x80000000));   
    return address;
}


void WritePCI(PCISegment seg,PCIBus bus,PCIDevice device,PCIFunction func,PCIOffsetSelector offset,PCIRegisterValue value)
{
    (*pciTable.writePCIreg)(seg,bus,device,func,offset,value);
}
void WritePCIeRegister(PCISegment seg,PCIBus bus,PCIDevice device,PCIFunction func,PCIOffsetSelector offset,PCIRegisterValue value)
{
    if(pciTable.mcfg != NULL)
    {
        // PCIe using MCFG table
        ACPIMCFG *ptr = (ACPIMCFG*)pciTable.mcfg;
        PCIeSpaceConfig *space = (PCIeSpaceConfig*)&ptr->spaces[seg];
        PCIAddress addr = CalcPCIeAddress(bus,device,func,offset);
        PCIRegisterValue *pcie_arr = (PCIRegisterValue*)(uint32_t)space->base_addr;
        pcie_arr[addr] = value;    
    }
}
void WritePCIRegister(__attribute__((unused)) PCISegment seg,PCIBus bus,PCIDevice device,PCIFunction func,PCIOffsetSelector offset,PCIRegisterValue value)
{
    PCIAddress address = CalcPCIAddress(bus,device,func,offset);
    // Read using IO ports, legacy method
    /* write out the address */
    outl(0xCF8, address);
    /* write in the data */
    outl(0xCFC,(uint32_t)(value & 0xffff));
}


void PCIenumeration(uint32_t list_addr)
{
    pciTable.devices = (PCIDeviceDescriptor*)(list_addr);
    pciTable.devices->uniq_id = 1;
    // Enumeration PCIe using MCFG table

    uint32_t count_devices = 0;
    uint32_t seg_entries;

    ACPIMCFG *ptr = (ACPIMCFG*)pciTable.mcfg;
    if(checkPCIeExisting())
        seg_entries = (ptr->h.Length - sizeof(ptr->h))/sizeof(PCIeSpaceConfig);
    else
        seg_entries = 1; // no PCIe, using one segment

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
                    if(ReadPCI(seg,bus,device,func,0) != 0xffff && ReadPCI(seg,bus,device,func,2) != 0xffff)
                    {
                        pciTable.devices[count_devices].uniq_id = count_devices;
                        pciTable.devices[count_devices].vendor_id = getPCIVendorDevice(seg,bus,device,func);
                        pciTable.devices[count_devices].device_id = getPCIDeviceID(seg,bus,device,func);
                        ++count_devices;
                        //pciTable.devices[count_devices]->
                    }
                }
            }
        }
    }
    pciTable.pci_devices_count = count_devices;
}

/// Functions for working with PCI Devices

bool checkPCIDeviceType(PCISegment seg,PCIBus bus,PCIDevice device,PCIFunction func,PCI_CLASSCODE class_code, PCI_SUBCLASS subclass,PCI_PROGIF progif)
{
    return (
            class_code == getPCIDeviceClassCode(seg,bus,device,func) &&
            subclass == getPCIDeviceSubClass(seg,bus,device,func) &&
            progif == getPCIDeviceProgIF(seg,bus,device,func)
    );
}

PCI_REVISION_ID getPCIDeviceRevisionID(PCISegment seg,PCIBus bus,PCIDevice device,PCIFunction func)
{
    return (PCI_REVISION_ID)(ReadPCI(seg,bus,device,func,8) & 0xFF);
}
PCI_CLASSCODE getPCIDeviceClassCode(PCISegment seg,PCIBus bus,PCIDevice device,PCIFunction func)
{
    return (PCI_CLASSCODE)((ReadPCI(seg,bus,device,func,8) & 0xFF00) >> 2);
}
PCI_SUBCLASS getPCIDeviceSubClass(PCISegment seg,PCIBus bus,PCIDevice device,PCIFunction func)
{
    return (PCI_SUBCLASS)(ReadPCI(seg,bus,device,func,10) & 0xFF);
}
PCI_PROGIF getPCIDeviceProgIF(PCISegment seg,PCIBus bus,PCIDevice device,PCIFunction func)
{
    return (PCI_PROGIF)((ReadPCI(seg,bus,device,func,10) & 0xFF00) >> 2);
}
PCIHeaderType getPCIDeviceHeaderType(PCISegment seg,PCIBus bus,PCIDevice device,PCIFunction func)
{
    return (PCIHeaderType)(ReadPCI(seg,bus,device,func,14) & 0xFF);
}

PCIVendor getPCIVendorDevice(PCISegment seg,PCIBus bus,PCIDevice device,PCIFunction func)
{
    return (PCIVendor)ReadPCI(seg,bus,device,func,0);
}
PCIDeviceID getPCIDeviceID(PCISegment seg,PCIBus bus,PCIDevice device,PCIFunction func)
{
    return (PCIDeviceID)ReadPCI(seg,bus,device,func,2);
}

PCICommandReg getPCICommandRegister(PCISegment seg,PCIBus bus,PCIDevice device,PCIFunction func)
{
    return (PCICommandReg)ReadPCI(seg,bus,device,func,4);
}
void setPCICommandRegister(PCISegment seg,PCIBus bus,PCIDevice device,PCIFunction func, PCIRegisterValue value)
{
    WritePCI(seg,bus,device,func,4,value);
}

bool isPCIDeviceInterruptEnabled(PCISegment seg,PCIBus bus,PCIDevice device,PCIFunction func)
{
    return (((uint16_t)getPCICommandRegister(seg,bus,device,func) & (1<<10)) >> 10) == 0;
}

void enablePCIDeviceInterrupt(PCISegment seg,PCIBus bus,PCIDevice device,PCIFunction func)
{
    setPCICommandRegister(seg,
                          bus,
                          device,
                          func,
                          (PCIRegisterValue)(getPCICommandRegister(seg,bus,device,func) & (~(1<<10)))
                          );
}
void disablePCIDeviceInterrupt(PCISegment seg,PCIBus bus,PCIDevice device,PCIFunction func)
{
    setPCICommandRegister(seg,
                          bus,
                          device,
                          func,
                          (PCIRegisterValue)(getPCICommandRegister(seg,bus,device,func) & (1<<10))
                          );
}

PCIStatusReg getPCIStatusRegister(PCISegment seg,PCIBus bus,PCIDevice device,PCIFunction func)
{
    return (PCIStatusReg)ReadPCI(seg,bus,device,func,6);
}
bool isPCIDevice66MHzCapable(PCISegment seg,PCIBus bus,PCIDevice device,PCIFunction func)
{
    return (((uint16_t)getPCIStatusRegister(seg,bus,device,func) & (1<<5)) >> 5) == 1;
}
bool isPCIDeviceHasCapabilitiesList(PCISegment seg,PCIBus bus,PCIDevice device,PCIFunction func)
{
    return (((uint16_t)getPCIStatusRegister(seg,bus,device,func) & (1<<4)) >> 4) == 1;
}