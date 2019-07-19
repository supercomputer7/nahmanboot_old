#include "drivers/acpi/table.h"
#include <string.h>

void initACPI_INFO()
{

}

ACPI_INFO acpi;

void *GetACPI_INFO()
{
    uint32_t ptr = GetRSDP();

    uint32_t *p = (uint32_t*)0x4000;
    *p = ptr;

    acpi.rsdp = (void*)ptr;

    p = p+2;
    //*p = &acpi;    
    return &acpi;
}
uint32_t GetRSDP()
{
    uint16_t count = 64;
    uint32_t pointer = 0x80000;
    
    uint32_t p = SearchRSDP(pointer,count);
    if(p != 0)
        return p;

    count = 8191;
    pointer = 0xE0000;
    return SearchRSDP(pointer,count);
    
}

uint32_t SearchRSDP(uint32_t pointer,uint16_t count)
{
    void *p = (void*)pointer;
    for(int i=0; i<count; ++i)
    {
        if(strncmp((char*)p,"RSD PTR ",strlen("RSD PTR ")))
            return (uint32_t)p;
        p = p + 0x10;
    }
    return 0;
}

int GetRSDPRevision(void *rsdp)
{
    uint8_t *p = (uint8_t*)rsdp;
    uint8_t revision = p[15];
    return (int)revision;
}

void *GetACPITablePointer(void *rsdp, const char *signature)
{
    if(GetRSDPRevision(rsdp) == 0)
    {
        RSDPDescriptor *p = (RSDPDescriptor*)rsdp; // RSDP Pointer
        RSDT *rsdt = (RSDT*)p->RsdtAddress;

        int entries = (rsdt->h.Length - sizeof(rsdt->h)) / sizeof(uint32_t);
 
        for (int i = 0; i < entries; i++)
        {
            ACPITableHeader *h = (ACPITableHeader *) rsdt->PointerToOtherSDT[i];
            if (strncmp(h->Signature, signature, 4))
                return (void *) h;
        }
    }
    else
    {
        RSDPDescriptor20 *p = (RSDPDescriptor20*)rsdp; // RSDP Pointers
        XSDT *xsdt = (XSDT*)(p->XsdtAddress);

        int entries = (xsdt->h.Length - sizeof(xsdt->h)) / sizeof(uint64_t);
        for (int i = 0; i < entries; i++)
        {
            ACPITableHeader *h = (ACPITableHeader *) xsdt->PointerToOtherSDT[i];
            if (strncmp(h->Signature, signature, 4))
                return (void *) h;
        }
    }
    return NULL;
}

bool ACPI_ValidateRebootSupport(ACPIFADT *fadt)
{
    uint32_t flags = (uint32_t)fadt->Flags;
    uint8_t rebootSupport = ((flags >> 10) & 0x1);
    return rebootSupport == 1;
}

uint32_t ACPI_GetTableData(void *ptr,uint32_t offset)
{
    uint32_t *pointer = (uint32_t*)ptr;
    return pointer[offset/4];
}

uint8_t ACPI_GetTableRevision(void *pointer)
{
    ACPITableHeader *ptr = (ACPITableHeader*)pointer;
    return ptr->Revision;
}
bool ACPI_ValidateRSDP(RSDPDescriptor *rsdp)
{
    uint8_t checksum = rsdp->Checksum;
    uint8_t sum = 0;
    if(GetRSDPRevision(rsdp) == 0)
    {
        
        RSDPDescriptor* legacy_rsdp = (RSDPDescriptor*)rsdp;
        for(uint32_t i=0; i < sizeof(RSDPDescriptor); ++i)
        {
            sum += ((uint8_t*) legacy_rsdp)[i];
        }
        sum += checksum;
        return sum == 0;
    }
    else
    {
        RSDPDescriptor20* ext_rsdp = (RSDPDescriptor20*)rsdp;
        for(uint32_t i=0; i < ext_rsdp->Length; ++i)
        {
            sum += ((uint8_t*) ext_rsdp)[i];
        }
        sum += checksum;
        return sum == 0;
    }
}
