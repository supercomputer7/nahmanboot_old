#ifndef DISKMAIN_H
#define DISKMAIN_H

#include "drivers/pci/defs.h"
#include "drivers/disk/defs.h"

#include "drivers/disk/ahci.h"
#include "drivers/disk/ide.h"

#include "memory/bump_alloc.h"

StorageDiskDescriptor* DiskEnumeration(PCIStorageControllerList* controllers,BumpAllocator* bump_allocator);

#endif