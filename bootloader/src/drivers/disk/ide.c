#include "drivers/disk/ide.h"

ERRORCODE ReadIDE_Sectors(PCIDescriptor* pciDesc,StorageDiskDescriptor* disk,LBA lba1,LBA lba2,SectorAmount amount,BufferAddr buf)
{
    uint16_t port = 0;
    uint8_t id = disk->header.storage_id;
    if(id == 2 || id == 3) //  0 = Primary Master, 1 = Primary Slave, 2 = Secondary Master, 3 = Secondary Slave
    {
        port = pciDesc->readPCIreg(pciDesc,
            disk->header.controller->segment,
            disk->header.controller->bus,
            disk->header.controller->device_number,
            disk->header.controller->function_number,
            0x1c
        );
        if(port == 0 || port == 1)
        {
            port = SECONDARY_BUS_BASEIO;
        }
    }
    else
    {
        if(id == 0 || id == 1)
        {
            port = pciDesc->readPCIreg(pciDesc,
                disk->header.controller->segment,
                disk->header.controller->bus,
                disk->header.controller->device_number,
                disk->header.controller->function_number,
                0x10
            );
            if(port == 0 || port == 1)
            {
                port = PRIMARY_BUS_BASEIO;
            }
        }
    }
    uint16_t* buffer = (uint16_t*)(buf);
    bool slave = (id == 1) || (id == 3);
    if(lba2 == 0)
    {
        ReadIDE_Drive(port,slave, lba1,buffer,amount);
    }
    else
    {
        ExtReadIDE_Drive(port,slave,lba1,lba2,buffer,amount);
    }
    return 0;
}

ERRORCODE ReadIDE_Identification(PCIDescriptor* pciDesc,StorageDiskDescriptor* disk,BufferAddr buf)
{
    uint16_t port = 0;
    uint8_t id = disk->header.storage_id;
    if(id == 2 || id == 3) //  0 = Primary Master, 1 = Primary Slave, 2 = Secondary Master, 3 = Secondary Slave
    {
        port = pciDesc->readPCIreg(pciDesc,
            disk->header.controller->segment,
            disk->header.controller->bus,
            disk->header.controller->device_number,
            disk->header.controller->function_number,
            0x1c
        );
        if(port == 0 || port == 1)
        {
            port = SECONDARY_BUS_BASEIO;
        }
    }
    else
    {
        if(id == 0 || id == 1)
        {
            port = pciDesc->readPCIreg(pciDesc,
                disk->header.controller->segment,
                disk->header.controller->bus,
                disk->header.controller->device_number,
                disk->header.controller->function_number,
                0x10
            );
            if(port == 0 || port == 1)
            {
                port = PRIMARY_BUS_BASEIO;
            }
        }
    }
    uint16_t* buffer = (uint16_t*)(buf);
    bool slave = (id == 1) || (id == 3);
    if(IdentifyIDE_Drive(port,slave,buffer))
    {
        return 0;
    }
    else
    {
        return ERR_NO_DEVICE;
    }
}

bool IdentifyIDE_Drive(uint16_t port,bool slave, uint16_t* buf)
{
    if(slave)
    {
        outb(port + ATA_REG_DATA, 0xB0);
    }
    else
    {
        outb(port + ATA_REG_DATA, 0xA0);
    }
    outb(port + ATA_REG_SECCOUNT0, 0);
    outb(port + ATA_REG_LBA0, 0);
    outb(port + ATA_REG_LBA1, 0);
    outb(port + ATA_REG_LBA2, 0);
    outb(port + ATA_REG_COMMAND, 0xEC);

    uint8_t input = inb(port + ATA_REG_STATUS);
    
    if (input != 0)
    {
        input = inb(port + ATA_REG_STATUS);
        if(((input & 0x1) != 0) || ((input & 0x20) != 0))
        {
            return false;
        }
        while(((input & 0x80) != 0) && ((input & 0x8) == 0))
        {
            if(((input & 0x1) != 0) || ((input & 0x20) != 0))
            {
                return false;
            }
            input = inb(port + ATA_REG_STATUS);
        }
        for(int i=0; i < 256;i++)
        {
            buf[i] = inw(port + ATA_REG_DATA);            
        }
        return true;
    }
    return false;
}
void ReadIDE_Drive(uint16_t port,bool slave, uint32_t lba_low ,uint16_t* buf,uint16_t sectorcount)
{
    if(slave)
    {
        outb(port + ATA_REG_DATA, 0xE0 | ((lba_low >> 24) & 0x0F));
    }
    else
    {
        outb(port + ATA_REG_DATA, 0xF0 | ((lba_low >> 24) & 0x0F));
    }
    outb(port+ 0x1, 0x00);
    outb(port + ATA_REG_SECCOUNT0,(uint8_t)sectorcount);
    outb(port + ATA_REG_LBA0, (uint8_t)(lba_low));
    outb(port + ATA_REG_LBA1, (uint8_t)(lba_low >> 8));
    outb(port + ATA_REG_LBA2, (uint8_t)(lba_low >> 16));
    outb(port + ATA_REG_COMMAND, ATA_CMD_READ_PIO);


    uint8_t input = inb(port + ATA_REG_STATUS);
    bool error = false;
    if (input)
    {
        input = inb(port + ATA_REG_STATUS);
        while(((input & 0x80) != 0) && ((input & 0x8) == 0)) // wait for DRQ to set, BSY to clear
        {
            input = inb(port + ATA_REG_STATUS);
            if(((input & 0x1) != 0) || ((input & 0x20) != 0)) // error happened!
            {
                error = true;
                break;
            }
        }
        if(!error)
        {
            int count = 0;
            for(int j=0; j<sectorcount; ++j)
            {
                for(int i=0; i < 256;i++)
                {
                    buf[count] = inw(port + ATA_REG_DATA); // transfer data
                    ++count;         
                }
                ide_400ns_delay(port);
                input = inb(port + ATA_REG_STATUS);
                while(((input & 0x80) != 0) && ((input & 0x8) == 0)) // wait for DRQ to set, BSY to clear
                {
                    input = inb(port + ATA_REG_STATUS);
                }
                for(int k=0; k<3; ++k)
                {
                    ide_400ns_delay(port);
                }
            }
        }
    }
}
void ExtReadIDE_Drive(uint16_t port,bool slave, uint32_t lba_low, uint32_t lba_high ,uint16_t* buf,uint16_t sectorcount)
{
    if(slave)
    {
        outb(port + ATA_REG_DATA, 0x50);
    }
    else
    {
        outb(port + ATA_REG_DATA, 0x40);
    }
    outb(port + ATA_REG_SECCOUNT0, (uint8_t)sectorcount >> 8);
    outb(port + ATA_REG_LBA0, (uint8_t)(lba_low >> 24));
    outb(port + ATA_REG_LBA1, (uint8_t)(lba_high));
    outb(port + ATA_REG_LBA2, (uint8_t)(lba_high >> 8));
    outb(port + ATA_REG_SECCOUNT0,(uint8_t)sectorcount);
    outb(port + ATA_REG_LBA0, (uint8_t)(lba_low));
    outb(port + ATA_REG_LBA1, (uint8_t)(lba_low >> 8));
    outb(port + ATA_REG_LBA2, (uint8_t)(lba_low >> 16));
    outb(port + ATA_REG_COMMAND, ATA_CMD_READ_PIO_EXT);

    int words = 256 * sectorcount;

    uint8_t input = inb(port + ATA_REG_STATUS);
    if (input)
    {
        
        input = inb(port + ATA_REG_STATUS);
        while(((input & 0x80) != 0) && ((input & 0x8) == 0))
        {
            input = inb(port + ATA_REG_STATUS);
        }

        for(int i=0; i < words;i++)
        {
            buf[i] = inw(port + ATA_REG_DATA);            
        }
    }
}

void ide_400ns_delay(uint16_t port)
{
	for(int i = 0;i < 4; i++)
		inb(port + ATA_REG_ALTSTATUS);
}