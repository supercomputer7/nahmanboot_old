#include "drivers/pci/pcie.h"

bool checkPCIeExisting()
{
    return GetACPITablePointer((void*)GetRSDP(), "MCFG") != NULL;
}

void forcePCIeMode(PCIDescriptor* pciTable)
{
    pciTable->pciMethod = PCIExpressMode;
    pciTable->readPCIreg = &ReadPCIeRegister;
    pciTable->writePCIreg = &WritePCIeRegister;
}
bool isPCIeMode(PCIDescriptor* pciTable)
{
    return queryPCIMode(pciTable) == PCIExpressMode;
}

// Basic PCI Functions

PCIRegisterValue ReadPCIeRegister(PCIDescriptor* pciTable,PCISegment seg,PCIBus bus,PCIDevice device,PCIFunction func, PCIOffsetSelector offset)
{
    if(pciTable->mcfg == NULL)
        return 0;

    // PCIe using MCFG table
    ACPIMCFG *ptr = (ACPIMCFG*)pciTable->mcfg;
    PCIeSpaceConfig *space = (PCIeSpaceConfig*)&ptr->spaces[seg];
    PCIAddress addr = CalcPCIeAddress(bus,device,func,offset);
    PCIRegisterValue *pcie_arr = (PCIRegisterValue*)(uint32_t)space->base_addr;
    return pcie_arr[addr];
}

PCIAddress CalcPCIeAddress(PCIBus bus,PCIDevice device,PCIFunction func,PCIOffsetSelector offset)
{
    return ((
             PCIeDeviceConfigSpaceSize * DevicesPerBus * FunctionsPerDevice * bus + 
             PCIeDeviceConfigSpaceSize * FunctionsPerDevice * device + 
             PCIeDeviceConfigSpaceSize * func + offset) >> 1 // divide by sizeof(uint16_t)
            ); 
}

void WritePCIeRegister(PCIDescriptor* pciTable,PCISegment seg,PCIBus bus,PCIDevice device,PCIFunction func,PCIOffsetSelector offset,PCIRegisterValue value)
{
    if(pciTable->mcfg != NULL)
    {
        // PCIe using MCFG table
        ACPIMCFG *ptr = (ACPIMCFG*)pciTable->mcfg;
        PCIeSpaceConfig *space = (PCIeSpaceConfig*)&ptr->spaces[seg];
        PCIAddress addr = CalcPCIeAddress(bus,device,func,offset);
        PCIRegisterValue *pcie_arr = (PCIRegisterValue*)(uint32_t)space->base_addr;
        pcie_arr[addr] = value;    
    }
}