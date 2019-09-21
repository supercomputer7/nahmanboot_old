#ifndef DISKIDE_H
#define DISKIDE_H

#include <stdint.h>
#include <stdbool.h>
#include "drivers/io/ioport.h"
#include "drivers/disk/ata.h"
#include "drivers/pci/defs.h"

#include "drivers/disk/defs.h"

#define PRIMARY_BUS_BASEIO 0x1f0
#define SECONDARY_BUS_BASEIO 0x170

#define IDE_ATA        0x00
#define IDE_ATAPI      0x01
 
#define ATA_MASTER     0x00
#define ATA_SLAVE      0x01

#define ATA_REG_DATA       0x00
#define ATA_REG_ERROR      0x01
#define ATA_REG_FEATURES   0x01
#define ATA_REG_SECCOUNT0  0x02
#define ATA_REG_LBA0       0x03
#define ATA_REG_LBA1       0x04
#define ATA_REG_LBA2       0x05
#define ATA_REG_HDDEVSEL   0x06
#define ATA_REG_COMMAND    0x07
#define ATA_REG_STATUS     0x07
#define ATA_REG_SECCOUNT1  0x08
#define ATA_REG_LBA3       0x09
#define ATA_REG_LBA4       0x0A
#define ATA_REG_LBA5       0x0B
#define ATA_REG_CONTROL    0x0C
#define ATA_REG_ALTSTATUS  0x0C
#define ATA_REG_DEVADDRESS 0x0D

#define      ATA_PRIMARY      0x00
#define      ATA_SECONDARY    0x01

bool IdentifyIDE_Drive(uint16_t port,bool slave, uint16_t* buf);
void ReadIDE_Drive(uint16_t port,bool slave, uint32_t lba_low,uint16_t* buf,uint16_t sectorcount);
void ExtReadIDE_Drive(uint16_t port,bool slave, uint32_t lba_low, uint32_t lba_high ,uint16_t* buf,uint16_t sectorcount);
void ide_400ns_delay(uint16_t port);

ERRORCODE ReadIDE_Sectors(PCIDescriptor* pciDesc,StorageDiskDescriptor* disk,LBA lba1,LBA lba2,SectorAmount amount,BufferAddr buf);
ERRORCODE ReadIDE_Identification(PCIDescriptor* pciDesc,StorageDiskDescriptor* disk,BufferAddr buf);

#endif