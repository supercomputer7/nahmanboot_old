#include "drivers/acpi/table.h"
initPCIsubsystem();

struct PCIfunctions {
    
} __attribute__((__packed__));

struct PCIDeviceDescriptor {
    uint32_t uniq_id;
    uint16_t flags;

    uint32_t segment;
    uint8_t bus;
    uint8_t device_number;
    uint8_t function_number;

    uint16_t vendor_id;
    uint16_t device_id;

    uint8_t class_code;
    uint8_t subclass;
    uint8_t progif;
    uint8_t revision;

} __attribute__((__packed__));
typedef struct PCIDeviceDescriptor PCIDevice;


