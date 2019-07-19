#include "drivers/acpi/powermodes.h"
#include "drivers/io/ioport.h"

void rebootACPI()
{
    ACPI_INFO *acpi = (void*)GetACPI_INFO(); // find ACPI RSDP
    void *rsdp = (void*)acpi->rsdp;
    ACPIFADT *fadt = (ACPIFADT*)GetACPITablePointer(rsdp, "FACP"); // find FADT

    if(ACPI_GetTableRevision(fadt) >= 2) // check if FADT revision is 2 or above
    {
        if(ACPI_ValidateRebootSupport(fadt)) // validate ACPI reboot support
        {
            GenericAddressStructure *resetReg = &fadt->ResetReg;

            uint8_t value = fadt->ResetValue; //(uint8_t)ACPI_GetTableData(fadt,128);            
            uint32_t addr = resetReg->Address; //ACPI_GetTableData(fadt,120);

            if(resetReg->AddressSpace == IO_ADDR_SPACE)
            {
                uint16_t port = (uint16_t)addr;
                outb(port,value); // reboot through IO port

                asm("cli\n"
                    "hlt");
                
            }
            else
            {
                if(resetReg->AddressSpace == MEMORY_ADDR_SPACE)
                {
                    uint8_t *ptr = (uint8_t*)addr;
                    *ptr = value; // reboot through Memory addressing
                    asm("cli\n"
                    "hlt");
                }
                else if(resetReg->AddressSpace == PCI_ADDR_SPACE)
                {
                    
                }
            }
        }
    }
}
void poweroffACPI()
{

}
