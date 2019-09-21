#ifndef ACPITABLE_H
#define ACPITABLE_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct acpi_info {
    void *rsdp;
    void *rsdt;
    void *xsdt;
    uint8_t rsdp_revision;
} ACPI_INFO;

typedef struct RSDPDescriptor {
 char Signature[8];
 uint8_t Checksum;
 char OEMID[6];
 uint8_t Revision;
 uint32_t RsdtAddress;
} __attribute__ ((packed)) RSDPDescriptor;

typedef struct RSDPDescriptor20 {
 RSDPDescriptor firstPart;
 
 uint32_t Length;
 uint32_t XsdtAddress1;
 uint32_t XsdtAddress2;
 uint8_t ExtendedChecksum;
 uint8_t reserved[3];
} __attribute__ ((packed)) RSDPDescriptor20;

typedef struct ACPISDTHeader {
  char Signature[4];
  uint32_t Length;
  uint8_t Revision;
  uint8_t Checksum;
  char OEMID[6];
  char OEMTableID[8];
  uint32_t OEMRevision;
  uint32_t CreatorID;
  uint32_t CreatorRevision;
} __attribute__((__packed__)) ACPITableHeader;

typedef struct RSDT {
  ACPITableHeader h;
  uint32_t PointerToOtherSDT[];
} __attribute__((__packed__)) RSDT;

typedef struct XSDT {
  ACPITableHeader h;
  uint64_t PointerToOtherSDT[];
} __attribute__((__packed__)) XSDT;

#define PCI_ADDR_SPACE 2
#define IO_ADDR_SPACE 1
#define MEMORY_ADDR_SPACE 0

typedef volatile struct MADT_ENTRY {
  uint8_t type;
  uint8_t length;
} __attribute__((__packed__)) MADT_ENTRY;

typedef volatile struct MADT_INT_OVERRIDE {
  MADT_ENTRY t;
  uint8_t bus;
  uint8_t irq;
  uint32_t gsi;
  uint16_t flags;
} __attribute__((__packed__)) MADT_INT_OVERRIDE;

typedef volatile struct MADT_LAPIC_ADDR_OVERRIDE {
  MADT_ENTRY t;
  uint16_t reserved;
  uint32_t addr1;
  uint32_t addr2;
} __attribute__((__packed__)) MADT_LAPIC_ADDR_OVERRIDE;

typedef volatile struct MADT_IOAPIC {
  MADT_ENTRY t;
  uint8_t id;
  uint8_t reserved;
  uint32_t addr;
  uint32_t gsi_base;
} __attribute__((__packed__)) MADT_IOAPIC;

typedef volatile struct MADT_LAPIC {
  MADT_ENTRY t;
  uint8_t acpi_id;
  uint8_t apic_id;
  uint32_t flags;
} __attribute__((__packed__)) MADT_LAPIC;

typedef volatile struct MADT
{
  ACPITableHeader h;
  uint32_t localapic_baseaddr;
  uint32_t flags;
  void* entries;
} __attribute__((__packed__)) MADT;

struct GenericAddressStructure
{
  uint8_t AddressSpace;
  uint8_t BitWidth;
  uint8_t BitOffset;
  uint8_t AccessSize;
  uint64_t Address;
} __attribute__((__packed__));
typedef struct GenericAddressStructure GenericAddressStructure;

typedef volatile struct FADT
{
    ACPITableHeader h;
    uint32_t FirmwareCtrl;
    uint32_t Dsdt;
 
    // field used in ACPI 1.0; no longer in use, for compatibility only
    uint8_t  Reserved;
 
    uint8_t  PreferredPowerManagementProfile;
    uint16_t SCI_Interrupt;
    uint32_t SMI_CommandPort;
    uint8_t  AcpiEnable;
    uint8_t  AcpiDisable;
    uint8_t  S4BIOS_REQ;
    uint8_t  PSTATE_Control;
    uint32_t PM1aEventBlock;
    uint32_t PM1bEventBlock;
    uint32_t PM1aControlBlock;
    uint32_t PM1bControlBlock;
    uint32_t PM2ControlBlock;
    uint32_t PMTimerBlock;
    uint32_t GPE0Block;
    uint32_t GPE1Block;
    uint8_t  PM1EventLength;
    uint8_t  PM1ControlLength;
    uint8_t  PM2ControlLength;
    uint8_t  PMTimerLength;
    uint8_t  GPE0Length;
    uint8_t  GPE1Length;
    uint8_t  GPE1Base;
    uint8_t  CStateControl;
    uint16_t WorstC2Latency;
    uint16_t WorstC3Latency;
    uint16_t FlushSize;
    uint16_t FlushStride;
    uint8_t  DutyOffset;
    uint8_t  DutyWidth;
    uint8_t  DayAlarm;
    uint8_t  MonthAlarm;
    uint8_t  Century;
 
    // reserved in ACPI 1.0; used since ACPI 2.0+
    uint16_t BootArchitectureFlags;
 
    uint8_t  Reserved2;
    uint32_t Flags;
 
    // 12 byte structure; see below for details
    GenericAddressStructure ResetReg;
 
    uint8_t  ResetValue;
    uint8_t  Reserved3[3];
 
    // 64bit pointers - Available on ACPI 2.0+
    uint64_t                X_FirmwareControl;
    uint64_t                X_Dsdt;
 
    GenericAddressStructure X_PM1aEventBlock;
    GenericAddressStructure X_PM1bEventBlock;
    GenericAddressStructure X_PM1aControlBlock;
    GenericAddressStructure X_PM1bControlBlock;
    GenericAddressStructure X_PM2ControlBlock;
    GenericAddressStructure X_PMTimerBlock;
    GenericAddressStructure X_GPE0Block;
    GenericAddressStructure X_GPE1Block;
} __attribute__((__packed__)) ACPIFADT;

struct PCIConfigurationSpaceDescriptor
{
  uint64_t base_addr;
  uint16_t pci_segment_number;
  uint8_t pci_start_bus;
  uint8_t pci_end_bus;
  uint8_t Reserved[4];
} __attribute__((__packed__));
typedef struct PCIConfigurationSpaceDescriptor PCIeSpaceConfig;

typedef volatile struct MCFG {
  ACPITableHeader h;
  uint8_t Reserved[8];
  PCIeSpaceConfig spaces[];
} __attribute__((__packed__)) ACPIMCFG;

void initACPI_INFO();
void *GetACPI_INFO();
uint32_t SearchRSDP(uint32_t pointer,uint16_t count);
uint32_t GetRSDP();
int GetRSDPRevision(RSDPDescriptor *rsdp);
void *GetACPITablePointer(RSDPDescriptor * rsdp, const char *signature);
bool ACPI_ValidateRebootSupport(ACPIFADT *fadt);
bool ACPI_ValidateRSDP(RSDPDescriptor *rsdp);
uint8_t ACPI_GetTableRevision(ACPITableHeader *pointer);
uint32_t ACPI_GetTableData(void *ptr,uint32_t offset);

#endif