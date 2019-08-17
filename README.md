# nahmanboot

NahmanBoot is a bootloader intended for x86 PCs (only). Boots from Real Mode to 32 Bit Protected Mode.
Written in x86 assembly & C.

Check SUPPORT.md for support list (what is planned, what will not be implemented).

## Some thoughts in the right way
* Assume bare minimum working hardware - During development, any assumptions that can be made in High level programming are not valid here. As this code is interfacing hardware directly, sanity checks are a must. Relying on firmware should be a last resort in general, and code should check for bugs if any piece of firmware code is being used.

* Code is not perfect - If you read SUPPORT.md file, you might think to yourself that the code will be organized & coded perfectly. The truth is that it is not. I (the first "contributor") am coding and building this project for many purposes, mainly for fun and for learning about computers. No one is perfect, and because of that, one should not assume that his code (or of someone else) is perfect. Project's code will not use all the advanced methods when interfacing hardware, like super fast DMA transfers, fastest shiny graphics drawing etc. However, one shouldn't mistake to think that the produced code will use ugly hacks and lazy methods - the produced code will use general & reliable methods as much as possible, corresponding to "Assume bare minimum working hardware" principle.

* Bootmanager and that's it - This project, however fancy and advanced it might be in some point, will remain a Bootmanager, like GRUB. This project is not intended for being an Operating System and if you want to explore OSDev, I highly recommend to go https://wiki.osdev.org (I learned a lot from this website) and check the SerenityOS project at https://github.com/SerenityOS/serenity (which is a very nice project).

* Advanced Flexibility in Mind - The project will strive to produce a flexible code so changes can be made with ease.

* Minimum interference in boot sequence - Before transfering the execution to the kernel with appropriate parameters delievered, the bootloader will try to change back every crirical setting in hardware that has been changed while the bootloader loaded itself and other things.

## Installation

For now there is no userspace utility. Once it will be written, it is intended that you will invoke something like that:
However, since only GPT Partition table will be supported, you will need to create a BIOS boot partition with at least 256 MB of space.
Make sure you install os-prober package since the bootloader will use it to detect operating systems available on system.

The bootmanager can be installed in 3 main ways:
* Method 1: On GPT partitioned Storage, when there is a separate boot partition (most recommended).
* Method 2: On MBR partitioned Storage, when there is a separate boot partition.
* Method 3: On MBR partitioned Storage, when there is no separate boot partition (not recommended).

### Method 1:
```bash
nahmanboot /dev/sdXY --gpt
```

### Method 2:
```bash
nahmanboot /dev/sdXY --mbr
```

### Method 3:
```bash
nahmanboot /dev/sdX --mbr --no-seperate-partition
```

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