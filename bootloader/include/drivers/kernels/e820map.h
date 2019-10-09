#ifndef E820MAP__H
#define E820MAP__H

#include <stdint.h>

typedef struct e820map {
    uint32_t map_addr;
    uint32_t entries_count;
} e820map;

typedef struct e820map_entry {
    uint32_t base_addr1;
    uint32_t base_addr2;

    uint32_t length1;
    uint32_t length2;

    uint32_t type;
    uint32_t acpi3;
} __attribute__((__packed__)) e820map_entry_t;

#endif