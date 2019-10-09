#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>
#include <stdbool.h>
#include "drivers/disk/defs.h"

typedef enum
{
	SUBMENU		        = 0x01,
	BOOTSELECTOR	    = 0x02,
	SPECIALSELECTOR     = 0x03
} SELECTOR_TYPE;

typedef enum
{
	LINUX		        = 0x01,
	MULTIBOOT	        = 0x02,
	MULTIBOOT2          = 0x03
} BOOT_PROTOCOL;

typedef struct GeneralBootHeader {
    bool selection;
    uint32_t timeout;
    uint32_t default_selector;
    bool terminal_enabled;
} GeneralBootHeader;

typedef struct BootSelector {
    uint32_t protocol;
    StorageDiskDescriptor* storage;

    uint32_t kernel_lba1;
    uint32_t kernel_lba2;

    uint32_t kernel_size;

    uint32_t initrd_lba1;
    uint32_t initrd_lba2;

    uint32_t initrd_size;

    char* cmd[];
} BootSelector;

typedef struct Selector Selector;

typedef struct MenuSelector {
    uint32_t entries_count;
    Selector* selectors[];
} MenuSelector;

struct Selector{
    uint32_t type;
    union
    {
        BootSelector boot_selector;
        MenuSelector menu_selector;
    } u;
};

char* getEqualOffset(char* param);
char* getColonOffset(char* param);

uint32_t getParameterSuffixLength(char* param);
uint32_t getParameterPrefixLength(char* param);

bool parseBoolParameter(char* param);
uint32_t parseIntParameter(char* param);
char* parseStringParameter(char* param);

void parseBootSelector(BootSelector* bootselector,uint32_t buf);
void parseConfiguration(Selector* selector_list,uint32_t buf);

#endif