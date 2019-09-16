#include "drivers/disk/ahci.h"
HBA_MEM* getAHCI_HBA(PCIDescriptor* pciDescriptor,PCISegment seg,PCIBus bus,PCIDevice device,PCIFunction func)
{
    return (HBA_MEM*)((uint32_t)(
        pciDescriptor->readPCIreg(pciDescriptor,seg,bus,device,func,ABAR_BASE) |
        pciDescriptor->readPCIreg(pciDescriptor,seg,bus,device,func,ABAR_BASE+2) << 16
        ));
}
HBA_PORT* getAHCI_PORT(HBA_MEM* pointer,uint8_t port)
{
    if(port > MAXIMUM_SATA_PORTS)
        return NULL;
	return &pointer->ports[port];
}
bool isAHCI_BIOS_OS_HANDOFF_SUPPORTED(HBA_MEM* pointer)
{
    return (pointer->cap2 & 0x1) == 1;
}
bool isAHCI_BIOSBusy(HBA_MEM* pointer)
{
    return ((pointer->bohc >> 4) & 0x1) == 1;
}
bool isAHCI_BIOSControl(HBA_MEM* pointer)
{
    return (pointer->bohc & 0x1) == 1;
}
bool isAHCI_OSControl(HBA_MEM* pointer)
{
    return ((pointer->bohc >> 1) & 0x1) == 1;
}
bool setAHCI_OSControl(HBA_MEM* pointer)
{
    if(isAHCI_BIOSBusy(pointer))
    {
        pointer->bohc = pointer->bohc | (1 << 1);
        if(isAHCI_OSControl(pointer) && !isAHCI_BIOSControl(pointer))
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}
bool probeAHCI_PORT(HBA_MEM* pointer,uint8_t port)
{
    return (((pointer->pi) >> port) & 0x1) == 1;
}
uint8_t checkAHCI_PORT_TYPE(HBA_PORT* port)
{
    uint32_t ssts = port->ssts;
	uint8_t ipm = (ssts >> 8) & 0x0F;
	uint8_t det = ssts & 0x0F;
	if (det != HBA_PORT_DET_PRESENT)	// Check drive status
		return AHCI_DEV_NULL;
	if (ipm != HBA_PORT_IPM_ACTIVE)
		return AHCI_DEV_NULL;
 
	switch (port->sig)
	{
	    case SATA_SIG_ATAPI:
		    return AHCI_DEV_SATAPI;
	    case SATA_SIG_SEMB:
		    return AHCI_DEV_SEMB;
	    case SATA_SIG_PM:
		    return AHCI_DEV_PM;
	    default:
		    return AHCI_DEV_SATA;
	}
}
uint8_t checkAHCI_PORT_SPEED(HBA_MEM* pointer,HBA_PORT *port)
{
    uint8_t iss  = (pointer->cap >> 20) & 0xF;
    uint8_t sctl = (port->sctl >> 4) & 0xF;
    switch(iss)
    {
        case 1:
            return SATA1_SPEED;
        case 2:
            switch(sctl)
            {
                case 0:
                    return SATA2_SPEED;
                case 1:
                    return SATA1_SPEED;
                case 2:
                    return SATA2_SPEED;
                case 3:
                    return SATA2_SPEED;
            }
        case 3:
            switch(sctl)
            {
                case 0:
                    return SATA3_SPEED;
                case 1:
                    return SATA1_SPEED;
                case 2:
                    return SATA2_SPEED;
                case 3:
                    return SATA3_SPEED;
            }

    }
}
int checkAHCI_PORT_FREESLOT(HBA_PORT *port)
{
    // If not set in SACT and CI, the slot is free
	uint32_t slots = (port->sact | port->ci);
	for (int i=0; i < MAXIMUM_COMMAND_SLOTS; i++)
	{
		if ((slots&1) == 0)
			return i;
		slots >>= 1;
	}
    return -1;
}
AHCI_REPORT_INFO* identifyAHCI_Device(HBA_PORT *port, void *buf)
{
    return NULL;
}
AHCI_REPORT_INFO* readAHCI_Sectors(HBA_PORT *port, uint32_t basel_sector,uint32_t baseh_sector, uint32_t count, void *buf)
{
	port->is = (uint32_t) -1;
	int spin = 0; // Spin lock timeout counter
	int slot = checkAHCI_PORT_FREESLOT(port);
	if (slot == -1)
		return NULL;
 
	HBA_CMD_HEADER *cmdheader = (HBA_CMD_HEADER*)port->clb;
	cmdheader += slot;
	cmdheader->command = sizeof(FIS_REG_H2D)/sizeof(uint32_t);	// Command FIS size
	cmdheader->prdtl = (uint16_t)((count-1)>>4) + 1;	// PRDT entries count
 
	HBA_CMD_TBL *cmdtbl = (HBA_CMD_TBL*)(cmdheader->ctba);
	int i=0;
	// 8K bytes (16 sectors) per PRDT
	for (i=0; i<cmdheader->prdtl-1; i++)
	{
		cmdtbl->prdt_entry[i].dba = (uint32_t) buf;
		cmdtbl->prdt_entry[i].info = (8*1024-1 & (0x3FFFFF)) | (1<<31);	// 8K bytes (this value should always be set to 1 less than the actual value)
		buf += 4*1024;	// 4K words
		count -= 16;	// 16 sectors
	}
	// Last entry
	cmdtbl->prdt_entry[i].dba = (uint32_t) buf;
	cmdtbl->prdt_entry[i].info = ((count<<9)-1 & (0x3FFFFF)) | (1<<31);	// 512 bytes per sector
 
	// Setup command
	FIS_REG_H2D *cmdfis = (FIS_REG_H2D*)(&cmdtbl->cfis);
 
	cmdfis->h.fis_type = FIS_TYPE_REG_H2D;
	cmdfis->h.attrs = 0x80;	// Command
	cmdfis->command = ATA_CMD_READ_DMA_EXT;
 
	cmdfis->lba0 = (uint8_t)basel_sector;
	cmdfis->lba1 = (uint8_t)(basel_sector>>8);
	cmdfis->lba2 = (uint8_t)(basel_sector>>16);
	cmdfis->device = 1<<6;	// LBA mode
 
	cmdfis->lba3 = (uint8_t)(basel_sector>>24);
	cmdfis->lba4 = (uint8_t)baseh_sector;
	cmdfis->lba5 = (uint8_t)(baseh_sector>>8);
 
	cmdfis->countl = count & 0xFF;
	cmdfis->counth = (count >> 8) & 0xFF;
 
	// The below loop waits until the port is no longer busy before issuing a new command
	while ((port->tfd & (ATA_DEV_BUSY | ATA_DEV_DRQ)) && spin < 1000000)
	{
		spin++;
		if (spin == 1000000)
		{
			return NULL;
		}
	}
	
	port->ci = 1<<slot;	// Issue command
 
	// Wait for completion
	while (1)
	{
		// In some longer duration reads, it may be helpful to spin on the DPS bit 
		// in the PxIS port field as well (1 << 5)
		if ((port->ci & (1<<slot)) == 0) 
			break;
		if (port->is & HBA_PxIS_TFES)	// Task file error
		{
			break;
		}
	}
 
	// Check again
	if (port->is & HBA_PxIS_TFES)
	{
		return NULL;
	}
	return NULL;
}
AHCI_REPORT_INFO* writeAHCI_Sectors(HBA_PORT *port, uint32_t base_sector, uint32_t count, void *buf)
{
    return NULL;
}
void resetAHCI_HBA(HBA_MEM* pointer)
{
    uint32_t tmp = pointer->ghc;
    tmp = tmp | 0x1;
    pointer->ghc = tmp;
    bool exit = ((pointer->ghc) & 0x1) == 1;
    while (exit)
    {
        exit = ((pointer->ghc) & 0x1) == 1;
    }
}
void resetAHCI_PORT(HBA_MEM* pointer)
{
    
}
void resetAHCI_SOFTWARE(HBA_MEM* pointer)
{

}