#ifndef MAINPROG_H
#define MAINPROG_H

#include <stdint.h>
#include <string.h>

#include "drivers/acpi/powermodes.h"
#include "drivers/apic/main.h"
#include "drivers/pci/main.h"

#include "drivers/disk/ahci.h"
#include "drivers/disk/ide.h"

#include "drivers/disk/defs.h"
#include "drivers/disk/main.h"

#include "drivers/kernels/multiboot.h"
#include "drivers/kernels/elf.h"
#include "drivers/kernels/e820map.h"
#include "drivers/fs/partition.h"
#include "drivers/fs/ext2.h"

#include "memory/bump_alloc.h"


#define BASE_PORT 0x1F0

void welcome();
void init(BumpAllocator* bump_allocator);

e820map_entry_t* mess_memory_map(e820map_entry_t* map,BumpAllocator* bump_allocator);
e820map_entry_t* build_real_memory_map(BumpAllocator* bump_allocator);


void boot_serenity(PCIDescriptor* pciDesc, StorageDiskDescriptor* disk,BumpAllocator* bump_allocator);
void halt();

#endif