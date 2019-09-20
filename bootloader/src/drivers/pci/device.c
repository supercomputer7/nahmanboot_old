#include "drivers/pci/device.h"
#include "drivers/pci/general.h"


bool checkPCIDeviceType(PCIDescriptor* pciTable,PCISegment seg,PCIBus bus,PCIDevice device,PCIFunction func,PCI_CLASSCODE class_code, PCI_SUBCLASS subclass,PCI_PROGIF progif)
{
    return (
            class_code == getPCIDeviceClassCode(pciTable,seg,bus,device,func) &&
            subclass == getPCIDeviceSubClass(pciTable,seg,bus,device,func) &&
            progif == getPCIDeviceProgIF(pciTable,seg,bus,device,func)
    );
}

PCI_REVISION_ID getPCIDeviceRevisionID(PCIDescriptor* pciTable,PCISegment seg,PCIBus bus,PCIDevice device,PCIFunction func)
{
    return (PCI_REVISION_ID)(ReadPCI(pciTable,seg,bus,device,func,8) & 0xFF);
}
PCI_CLASSCODE getPCIDeviceClassCode(PCIDescriptor* pciTable,PCISegment seg,PCIBus bus,PCIDevice device,PCIFunction func)
{
    return (PCI_CLASSCODE)((ReadPCI(pciTable,seg,bus,device,func,10) & 0xFF00) >> 8);
}
PCI_SUBCLASS getPCIDeviceSubClass(PCIDescriptor* pciTable,PCISegment seg,PCIBus bus,PCIDevice device,PCIFunction func)
{
    return (PCI_SUBCLASS)(ReadPCI(pciTable,seg,bus,device,func,10) & 0xFF);
}
PCI_PROGIF getPCIDeviceProgIF(PCIDescriptor* pciTable,PCISegment seg,PCIBus bus,PCIDevice device,PCIFunction func)
{
    return (PCI_PROGIF)((ReadPCI(pciTable,seg,bus,device,func,8) & 0xFF00) >> 8);
}
PCIHeaderType getPCIDeviceHeaderType(PCIDescriptor* pciTable,PCISegment seg,PCIBus bus,PCIDevice device,PCIFunction func)
{
    return (PCIHeaderType)(ReadPCI(pciTable,seg,bus,device,func,14) & 0xFF);
}

PCIVendor getPCIVendorDevice(PCIDescriptor* pciTable,PCISegment seg,PCIBus bus,PCIDevice device,PCIFunction func)
{
    return (PCIVendor)ReadPCI(pciTable,seg,bus,device,func,0);
}
PCIDeviceID getPCIDeviceID(PCIDescriptor* pciTable,PCISegment seg,PCIBus bus,PCIDevice device,PCIFunction func)
{
    return (PCIDeviceID)ReadPCI(pciTable,seg,bus,device,func,2);
}

PCICommandReg getPCICommandRegister(PCIDescriptor* pciTable,PCISegment seg,PCIBus bus,PCIDevice device,PCIFunction func)
{
    return (PCICommandReg)ReadPCI(pciTable,seg,bus,device,func,4);
}
void setPCICommandRegister(PCIDescriptor* pciTable,PCISegment seg,PCIBus bus,PCIDevice device,PCIFunction func, PCIRegisterValue value)
{
    WritePCI(pciTable,seg,bus,device,func,4,value);
}

bool isPCIDeviceInterruptEnabled(PCIDescriptor* pciTable,PCISegment seg,PCIBus bus,PCIDevice device,PCIFunction func)
{
    return (((uint16_t)getPCICommandRegister(pciTable,seg,bus,device,func) & (1<<10)) >> 10) == 0;
}

void enablePCIDeviceInterrupt(PCIDescriptor* pciTable,PCISegment seg,PCIBus bus,PCIDevice device,PCIFunction func)
{
    setPCICommandRegister(pciTable,
                          seg,
                          bus,
                          device,
                          func,
                          (PCIRegisterValue)(getPCICommandRegister(pciTable,seg,bus,device,func) & (~(1<<10)))
                          );
}
void disablePCIDeviceInterrupt(PCIDescriptor* pciTable,PCISegment seg,PCIBus bus,PCIDevice device,PCIFunction func)
{
    setPCICommandRegister(pciTable,
                          seg,
                          bus,
                          device,
                          func,
                          (PCIRegisterValue)(getPCICommandRegister(pciTable,seg,bus,device,func) & (1<<10))
                          );
}

PCIStatusReg getPCIStatusRegister(PCIDescriptor* pciTable,PCISegment seg,PCIBus bus,PCIDevice device,PCIFunction func)
{
    return (PCIStatusReg)ReadPCI(pciTable,seg,bus,device,func,6);
}
bool isPCIDevice66MHzCapable(PCIDescriptor* pciTable,PCISegment seg,PCIBus bus,PCIDevice device,PCIFunction func)
{
    return (((uint16_t)getPCIStatusRegister(pciTable,seg,bus,device,func) & (1<<5)) >> 5) == 1;
}
bool isPCIDeviceHasCapabilitiesList(PCIDescriptor* pciTable,PCISegment seg,PCIBus bus,PCIDevice device,PCIFunction func)
{
    return (((uint16_t)getPCIStatusRegister(pciTable,seg,bus,device,func) & (1<<4)) >> 4) == 1;
}