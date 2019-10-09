#ifndef PARTITION_H__
#define PARTITION_H__

#include <stdint.h>
#include "drivers/disk/defs.h"
#include "drivers/pci/defs.h"

typedef struct partition {

    uint32_t lba_start;
    uint32_t lba_limit;
    StorageDiskDescriptor* desc;
    void (*read)(struct partition* partition,PCIDescriptor* pci_desc, StorageDiskDescriptor* descriptor,uint32_t block,uint16_t block_count,uint16_t* buf);
} Partition;

void partition_read(struct partition* partition,PCIDescriptor* pci_desc, StorageDiskDescriptor* descriptor,uint32_t block,uint16_t block_count,uint16_t* buf);

#endif