#include "drivers/fs/partition.h"

void partition_read(struct partition* partition,PCIDescriptor* pci_desc, StorageDiskDescriptor* descriptor,uint32_t block,uint16_t block_count,uint16_t* buf)
{
    descriptor->operator.readData(pci_desc,descriptor,partition->lba_start+block,0,block_count,buf);
}