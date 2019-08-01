cmake_minimum_required(VERSION 3.10)

project(NahmanBoot)

# Setup C
set(CMAKE_C_COMPILER /home/liav/opt/cross/bin/i686-elf-gcc)

# Setup using asm
enable_language(ASM)
set(CMAKE_ASM_COMPILER /usr/bin/nasm)
set(CMAKE_ASM_COMPILE_OBJECT "<CMAKE_ASM_COMPILER> <SOURCE> <FLAGS> -o <OBJECT>")

# Setup compilation flags
set(CMAKE_C_FLAGS "-std=gnu99 -ffreestanding -O2 -Wall -Wextra")
set(CMAKE_ASM_FLAGS "-f elf32")
set(CMAKE_EXE_LINKER_FLAGS "-T ${CMAKE_SOURCE_DIR}/linker.ld -nostdlib")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D__FILENAME__='\"$(subst ${CMAKE_SOURCE_DIR}/,,$(abspath $<))\"'")

# Libc source files
set(LIBC_SOURCE_FILES
        lib/libc/stdint.h
        lib/libc/string.c
        lib/libc/string.h
        lib/libc/nahman.h
        lib/libc/nahman.c
)

set(NAME_INCLUDE "default value" CACHE FILEPATH "description")

# Setup kernel sources
set(KERNEL_SOURCE_FILES
        src/boot/load.asm
        
        src/drivers/apic/main.c
        src/drivers/apic/lapic.c
        src/drivers/apic/pic.c
        src/drivers/apic/ioapic.c
        src/drivers/apic/idt.c

        
        src/drivers/acpi/table.c
        src/drivers/acpi/init.c
        src/drivers/acpi/powermodes.c

        src/drivers/pci/main.c

        src/main.c)

include_directories("lib/libc")
set(INCLUDE_DIR include/)
include_directories (${INCLUDE_DIR})     # .h all inside.

add_executable(core.bin ${KERNEL_SOURCE_FILES} ${LIBC_SOURCE_FILES})
