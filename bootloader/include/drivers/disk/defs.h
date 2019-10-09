#ifndef DISKDEFS_H
#define DISKDEFS_H

#include <stdint.h>
#include "drivers/pci/defs.h"

#define LegacyHardDiskSectorByteSize 512

#define UndefinedStorageDevice 0
#define IDE_StorageDevice 1
#define AHCI_StorageDevice 6
#define NVMe_StorageDevice 8
#define USB_StorageDevice 10
#define Floopy_StorageDevice 70

#define NoPartitonTableDisk 0
#define GPTDisk 1
#define MBRDisk 2
#define BSDDisk 3
#define UndefinedPartitionTable 0xFF

#define ERR_NO_DEVICE 1
#define ERR_NO_DATA_AT_LBA 2
#define ERR_BAD_BLOCK 3
#define ERR_UNCORRECTABLE_DATA 4
#define ERR_DEVICE_BUSY 5
#define ERR_DEVICE_JAMMED 6
#define ERR_CONTROLLER_BUSY 7
#define ERR_CONTROLLER_JAMMED 8

typedef uint32_t DISK_ID;
typedef uint16_t STORAGE_TYPE;
typedef uint8_t  ERRORCODE;
typedef uint32_t STORAGE_DEVICE_ID;
typedef uint32_t LBA;
typedef uint16_t SectorAmount;
typedef uint32_t BufferAddr;

typedef struct StorageHeader {
    PCIDeviceDescriptor* controller;
    DISK_ID storage_id;
    STORAGE_TYPE storage_type;
    STORAGE_DEVICE_ID storage_device_id; // The number of device. For example, Disk number 2 in the AHCI controller

} StorageHeader;

typedef struct StorageDiskDescriptor StorageDiskDescriptor;

typedef struct StorageOperator {
    ERRORCODE (*readData)(PCIDescriptor*,StorageDiskDescriptor*,LBA,LBA,SectorAmount,BufferAddr);
    ERRORCODE (*writeData)(PCIDescriptor*,StorageDiskDescriptor*,LBA,LBA,SectorAmount,BufferAddr);
    ERRORCODE (*readIdentifyData)(PCIDescriptor*,StorageDiskDescriptor*,BufferAddr);
} StorageOperator;

typedef struct StorageStatus {
    bool MechanicalStorage;
    uint16_t rpm_speed;
    uint32_t maximum_lba;
    uint8_t partition_table_type;
} StorageStatus;

typedef struct StorageLock {
    bool locked;
    //ERRORCODE (*lockDevice)(StorageDescriptor*);
    //ERRORCODE (*unlockDevice)(StorageDescriptor*);
} StorageLock;


struct StorageDiskDescriptor
{
    StorageHeader header;
    PCIDeviceDescriptor* controller;
    StorageStatus status;
    StorageOperator operator;
};

#endif