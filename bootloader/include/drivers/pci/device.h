#ifndef PCIDEVICE_H
#define PCIDEVICE_H

#include "drivers/pci/defs.h"

bool checkPCIDeviceType(PCIDescriptor* pciTable,PCISegment seg,PCIBus bus,PCIDevice device,PCIFunction func,PCI_CLASSCODE class_code, PCI_SUBCLASS subclass,PCI_PROGIF progif);
PCI_REVISION_ID getPCIDeviceRevisionID(PCIDescriptor* pciTable,PCISegment seg,PCIBus bus,PCIDevice device,PCIFunction func);
PCI_CLASSCODE getPCIDeviceClassCode(PCIDescriptor* pciTable,PCISegment seg,PCIBus bus,PCIDevice device,PCIFunction func);
PCI_SUBCLASS getPCIDeviceSubClass(PCIDescriptor* pciTable,PCISegment seg,PCIBus bus,PCIDevice device,PCIFunction func);
PCI_PROGIF getPCIDeviceProgIF(PCIDescriptor* pciTable,PCISegment seg,PCIBus bus,PCIDevice device,PCIFunction func);
PCIHeaderType getPCIDeviceHeaderType(PCIDescriptor* pciTable,PCISegment seg,PCIBus bus,PCIDevice device,PCIFunction func);
PCIVendor getPCIVendorDevice(PCIDescriptor* pciTable,PCISegment seg,PCIBus bus,PCIDevice device,PCIFunction func);
PCIDeviceID getPCIDeviceID(PCIDescriptor* pciTable,PCISegment seg,PCIBus bus,PCIDevice device,PCIFunction func);
PCICommandReg getPCICommandRegister(PCIDescriptor* pciTable,PCISegment seg,PCIBus bus,PCIDevice device,PCIFunction func);
void setPCICommandRegister(PCIDescriptor* pciTable,PCISegment seg,PCIBus bus,PCIDevice device,PCIFunction func, PCIRegisterValue value);
bool isPCIDeviceInterruptEnabled(PCIDescriptor* pciTable,PCISegment seg,PCIBus bus,PCIDevice device,PCIFunction func);
void enablePCIDeviceInterrupt(PCIDescriptor* pciTable,PCISegment seg,PCIBus bus,PCIDevice device,PCIFunction func);
void disablePCIDeviceInterrupt(PCIDescriptor* pciTable,PCISegment seg,PCIBus bus,PCIDevice device,PCIFunction func);
PCIStatusReg getPCIStatusRegister(PCIDescriptor* pciTable,PCISegment seg,PCIBus bus,PCIDevice device,PCIFunction func);
bool isPCIDevice66MHzCapable(PCIDescriptor* pciTable,PCISegment seg,PCIBus bus,PCIDevice device,PCIFunction func);
bool isPCIDeviceHasCapabilitiesList(PCIDescriptor* pciTable,PCISegment seg,PCIBus bus,PCIDevice device,PCIFunction func);

#endif
