#ifndef PCIMAIN_H
#define PCIMAIN_H

#include "drivers/acpi/table.h"

#define PCIeDeviceConfigSpaceSize 4096
#define FunctionsPerDevice 8
#define DevicesPerBus 32
#define PCIconventionalSegment 0

#define PCIExpressMode 2
#define PCIconventionalMode 1

struct PCIBasicHeader {
    uint16_t vendor_id;
    uint16_t device_id;
    
    uint16_t command;
    uint16_t status;

    uint8_t revision;
    uint8_t progif;
    uint8_t subclass;
    uint8_t class_code;

    uint8_t cache_line_size;
    uint8_t latency_timer;
    uint8_t header_type;
    uint8_t bist;
};
typedef struct PCIBasicHeader PCIBasicHeader;

struct PCIHeaderType0 {
    PCIBasicHeader h;

    uint32_t bar0;
    uint32_t bar1;
    uint32_t bar2;
    uint32_t bar3;
    uint32_t bar4;
    uint32_t bar5;
    uint32_t cardbus_cis_ptr;

    uint16_t subsystem_vendor_id;
    uint16_t subsystem_id;

    uint32_t expansion_rom_base_address;

    uint8_t capabilites_register;
    uint8_t reserved[3];

    uint32_t reserved2;

    uint8_t interrupt_line;
    uint8_t interrupt_pin;
    uint8_t min_grant;
    uint8_t max_latency;

} __attribute__((__packed__));

struct PCIHeaderType1 {
    PCIBasicHeader h;

    uint32_t bar0;
    uint32_t bar1;

    uint8_t primary_bus_number;
    uint8_t secondary_bus_number;
    uint8_t subordinate_bus_number;
    uint8_t secondary_latency_timer;

    uint8_t io_base;
    uint8_t io_limit;
    uint16_t secondary_status;

    uint16_t memory_base;
    uint16_t memory_limit;

    uint16_t prefetchable_memory_base;
    uint16_t prefetchable_memory_limit;

    uint32_t prefetchable_base_upper;
    uint32_t prefetchable_limit_upper;

    uint16_t io_base_upper;
    uint16_t io_limit_upper;

    uint32_t reserved;

    uint32_t expansion_rom_base_address;

    uint8_t interrupt_line;
    uint8_t interrupt_pin;
    uint16_t bridge_control;

} __attribute__((__packed__));

struct PCIHeaderType2 {
    PCIBasicHeader h;

    uint32_t cardbus_socket_base_address;

    uint8_t capabilites_list_offset;
    uint8_t reserved;
    uint16_t secondary_status;

    uint8_t pci_bus_number;
    uint8_t cardbus_bus_number;
    uint8_t subordinate_bus_number;
    uint8_t cardbus_latency_timer;

    uint32_t memory_base_addr0;
    uint32_t memory_limit0;
    uint32_t memory_base_addr1;
    uint32_t memory_limit1;

    uint32_t io_base_addr0;
    uint32_t io_limit0;
    uint32_t io_base_addr1;
    uint32_t io_limit1;

    uint8_t interrupt_line;
    uint8_t interrupt_pin;
    uint16_t bridge_control;

    uint16_t subsystem_device_id;
    uint16_t subsystem_vendor_id;

    uint32_t pc_card_16bit_mode_base_addr;

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
typedef struct PCIDeviceDescriptor PCIDeviceDescriptor;

typedef uint16_t PCIMode;

typedef uint16_t PCISegment;
typedef uint8_t PCIBus;
typedef uint8_t PCIDevice;
typedef uint8_t PCIFunction;

typedef uint16_t PCIRegisterValue;
typedef uint16_t PCIOffsetSelector;

typedef uint32_t PCIAddress;


struct pciSystemDescriptor {
    ACPIMCFG *mcfg;
    PCIMode pciMethod;
    PCIRegisterValue (*readPCIreg)(PCISegment,PCIBus,PCIDevice,PCIFunction,PCIOffsetSelector);
    void (*writePCIreg)(PCISegment,PCIBus,PCIDevice,PCIFunction,PCIOffsetSelector,PCIRegisterValue);
    uint32_t pci_devices_count;
    PCIDeviceDescriptor *devices;
} __attribute__((__packed__));
typedef struct pciSystemDescriptor PCIDescriptor;

void PCIScan();

PCIRegisterValue ReadPCIeRegister(PCISegment seg,PCIBus bus,PCIDevice device,PCIFunction func,PCIOffsetSelector offset);
PCIRegisterValue ReadPCIRegister(__attribute__((unused)) PCISegment seg,PCIBus bus,PCIDevice device,PCIFunction func,PCIOffsetSelector offset);
PCIRegisterValue ReadPCI(PCISegment seg,PCIBus bus,PCIDevice device,PCIFunction func, PCIOffsetSelector offset);

void WritePCIeRegister(PCISegment seg,PCIBus bus,PCIDevice device,PCIFunction func,PCIOffsetSelector offset,PCIRegisterValue value);
void WritePCIRegister(__attribute__((unused)) PCISegment seg,PCIBus bus,PCIDevice device,PCIFunction func,PCIOffsetSelector offset,PCIRegisterValue value);
void WritePCI(PCISegment seg,PCIBus bus,PCIDevice device,PCIFunction func,PCIOffsetSelector offset,PCIRegisterValue value);

void forcePCIMode();
void forcePCIeMode();

PCIAddress CalcPCIeAddress(PCIBus bus,PCIDevice device,PCIFunction func,PCIOffsetSelector offset);
PCIAddress CalcPCIAddress(PCIBus bus,PCIDevice device,PCIFunction func,PCIOffsetSelector offset);

PCIMode queryPCIMode();
bool isPCIMode();
bool isPCIeMode();

void PCIenumeration();


#endif
