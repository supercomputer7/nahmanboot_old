#include "drivers/kernels/elf.h"

void parseELF(Elf32MainHeader* main_header)
{
    Elf32ProgramHeader* progs = (Elf32ProgramHeader*)((uint32_t)main_header + (main_header->program_header_table_position));
    for(int i=0; i < main_header->program_header_table_entries_count ; ++i)
    {
        // zeroing p_memsz bytes at p_vaddr
        uint8_t* buf = (uint8_t*)(progs[i].p_vaddr);
        for(uint32_t j=0; j < progs[i].p_memsz; ++j)
        {
            buf[j] = 0;
        }
        
        // copy p_filesz bytes from p_offset to p_vaddr
        uint8_t* buf2 = (uint8_t*)((uint32_t)main_header + (progs[i].p_offset));
        for(uint32_t  j=0; j < progs[i].p_filesz; ++j)
        {
            buf[j] = buf2[j];
        }
    }

    uint8_t* buf3 = (uint8_t*)((uint32_t)main_header + main_header->section_header_table_position);
    uint8_t* buf4 = (uint8_t*)(0x400000);
    for(int i=0; i < main_header->section_header_table_entries_count ; ++i)
    {
        // copy p_filesz bytes from p_offset to p_vaddr
        for(uint32_t  j=0; j < main_header->section_header_table_entries_count*main_header->section_header_table_entry_size; ++j)
        {
            buf4[j] = buf3[j];
        }
    }
}
