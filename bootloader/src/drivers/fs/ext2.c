#include "drivers/fs/ext2.h"

ext2_extended_superblock get_ext2_superblock(Partition* partition,BumpAllocator* bump_allocator)
{
    ext2_extended_superblock* superblock = (ext2_extended_superblock*)bump_allocator->allocate(bump_allocator,sizeof(ext2_extended_superblock));
    partition->read(partition,partition->desc-);
}