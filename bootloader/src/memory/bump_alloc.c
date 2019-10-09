#include "memory/bump_alloc.h"

uint32_t AllocateMemory(BumpAllocator* allocator, uint32_t bytesToAllocate)
{
    uint32_t address = (uint32_t)allocator->addr;
    allocator->addr = (uint32_t)((uint32_t)allocator->addr + bytesToAllocate);
    return address;
}
void CleanMemory(BumpAllocator* allocator)
{
    allocator->addr = allocator->default_addr;
}
void InitAllocator(BumpAllocator* allocator,e820map* mem_map)
{
    //allocator->default_addr =
}