## Hardware Support (existing or planned):
* SATA & SATAPI Devices, USB 1,2 Storage Devices, IDE Hard drives & Optical Drives
* Floppy 1.44 MB Support
* NVMe Drives Support
* Basic VGA Capabilities
* PCI & PCI-Express Support
* Human Interface Devices: USB HID, PS2 Keyboard & mouse (only if not emulated)
* Modern hardware: IO-APIC & LAPIC
* Timing hardware: RTC & HPET

## Software Features (existing or planned):
* Booting Linux/Multiboot2/Multiboot kernels
* Booting kernels over CD/DVD-ROM/RAM
* MBR (Partial Support only) & GPT Partition table (Full Support)
* Parsing FAT12,16,32 & ext2,3,4 & ISO-9660
* Full ACPI Support: AML parsing (for PCI IRQ routing table) & power management (shutdown method) and static table parsing
* Modification & Configuration capabilities

## Some concepts/features that might be implemented, based on demand:
* Modules (drivers) insertion during runtime
* Basic Multitasking (Kernel mode only)
* Basic Networking hardware support & TCP/IP(v4/v6) Stack
* Some sort of x86 32 Bit paging mechanism
* USB 3 Support
* Basic shell (command line interface) with option for recovery tools (e.g. partition manager)
* UEFI Support (in other Repository)
* Booting kernels over Network
* LVM & Software RAID support

## This bootloader will not use:
* Obsolete hardware: PIT, PIC, PS/2 (if emulated)
* BIOS Services after initial setup (stage 1 & 1.5), including BIOS32 & BIOS APM 32 Protected mode services
* Multi core system or Multithreading system
* Any sort of advanced multitasking concept

### Notes & Explanations why not to implement some features
* "will not use" = will not support it, but this hardware/concepts/features can be still used by the OS later...
* MBR Lack of Support: Bootloader will halt if actually found only MBR table. However, protective MBR (a must for GPT table) can be used, and bootloader will warn the user if protective MBR is not found. The reason for lack of support for MBR is that no one really uses this type of partition table, and it is very limiting!
* Multi core system: Bootloader simply will not enable any other CPU core found in the system since OS will do it
* Advanced multitasking system: Bootloader does not need to multitask with User mode applications, as it will overcomplicate design...
* PS/2: If Bootloader detects a USB keyboard & mouse, it will check for USB Legacy Support. In case it found there is such support, it will disable interrupts from the PS/2 bus entirely, effectively disabling for the moment any support of PS/2 (but can be reenabled later by OS code).
* Any other obsolete hardware - bootloader will simply ignore that hardware, trying to disable it by masking it somehow.
* Bootloader will use BIOS services to locate e820 map, setup basic fraembuffer (and will list all VBE/VESA modes available) and to load stage 2 (core.bin). After switching to protected mode in stage 1.5 (boot32.bin), no further calls are made to BIOS. Basically, the bootloader will try to avoid usage of firmware capabilities as much as possible, to keep the code portable.