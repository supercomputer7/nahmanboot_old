#ifndef BUMP_ALLOC_H__
#define BUMP_ALLOC_H__

#include <stdint.h>
#include "drivers/kernels/e820map.h"

typedef struct bump_allocator {

    uint32_t default_addr;
    uint32_t addr;
    void (*clean)(struct bump_allocator*);
    uint32_t (*allocate)(struct bump_allocator*,uint32_t);
    void (*init)(struct bump_allocator*,e820map mem_map);

} BumpAllocator;

uint32_t AllocateMemory(BumpAllocator* allocator, uint32_t bytesToAllocate);
void CleanMemory(BumpAllocator* allocator);
void InitAllocator(BumpAllocator* allocator,e820map* mem_map);

#endif