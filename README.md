# nahmanboot

NahmanBoot is a bootloader intended for x86 PCs (only). Boots from Real Mode to 32 Bit Protected Mode.
Written in x86 assembly & C (will use C for acpica probably)

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
* Full ACPI Support: AML parsing & power management (using ACPICA) and static table parsing (without ACPICA)
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

A side note about this section: any feature in this section can be implemented really after the basics are done. 
If any of the features here are present in the bootloader, that means that I implemented all of them (because they need each other).
Floppy Support is useful for debugging and testing environment, but not for real machines. When using floppies on real machines,
the floppy subsystem is very unreliable and will most likely fail to load big files in reasonable time. However, floppies can be used to boot small kernels,
and also can be used to load modules in runtime. So I'm still not very sure if I want to implement such subsystem. In case I decide to do so, it will be after everything else is written and behaves correctly.
Booting kernels over network if implemented, will be similiar to PXE network boot (i.e. using dhcp server+tftp to acquire IP address and then download the boot image).

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

## Installation

For now there is no userspace utility. Once it will be written, it is intended that you will invoke something like that:
However, since only GPT Partition table will be supported, you will need to create a BIOS boot partition with at least 256 MB of space.
Make sure you install os-prober package since the bootloader will use it to detect operating systems available on system.

```bash
nahmanboot -p /dev/sdXY
```

when you need to specify the partition you have created.

## Usage

Once you installed everything correctly, you can reboot and see this new bootloader starting.
Userspace utilities will be provided for major platforms (including source code, of course) so you can install and see the bootloader works on your PC.

Note: In order to compile the bootloader code, you must have a GCC Cross compiler. Do not try to bypass it by rewriting the cmake file as it will only bring problems obviously.

## License
[MIT](https://choosealicense.com/licenses/mit/)

Copyright (c) 2019 TheComputer

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.