#include "main.h"
void main()
{
    welcome();
    
    prepare_interrupts(); // Disable LAPIC & PIC for a moment, install IDT & enable LAPIC
    
    BumpAllocator bump_allocator;
    bump_allocator.default_addr = (uint32_t)0x200000;
    bump_allocator.allocate = &AllocateMemory;
    bump_allocator.clean = &CleanMemory;
    bump_allocator.init = &InitAllocator;
    bump_allocator.clean(&bump_allocator);

    init(&bump_allocator);


    //const char cmd[] = "1024";
    //*(uint32_t*)(0x400000) = StringToInt(&cmd,4);

    /*
    HBA_MEM* hba = getAHCI_HBA(&pciDesc,0,0,31,2);
    HBA_PORT* port = getAHCI_PORT(hba,0);

    uint32_t *pointer = (uint32_t*)0x6000;
    pointer[0] = hba;
    pointer[1] = port;
    pointer[2] = &pciDesc;

    void *ptr = readAHCI_Sectors(port, 0,0, 4,(void *)0x1000);
    */


    // Reset HPET, LAPIC timer & RTC
    // Scan all partitions in disks
    // Scan all filesystems
    // Scan all devices, search for input devices
    // Show results to user

    //rebootACPI();

    

    // halt!
    
    
}

void init(BumpAllocator* bump_allocator)
{
    PCIDescriptor* pciDesc = (PCIDescriptor*)bump_allocator->allocate(bump_allocator,sizeof(PCIDescriptor));
    PCIScan(pciDesc,bump_allocator);
    PCIStorageControllerList* storages = (PCIStorageControllerList*)PCIScanStorageDevices(pciDesc,bump_allocator);

    StorageDiskDescriptor* disks = DiskEnumeration(storages,bump_allocator);
    
    boot_serenity(pciDesc,disks,bump_allocator);
    halt();
}

void boot_serenity(PCIDescriptor* pciDesc, StorageDiskDescriptor* disk,BumpAllocator* bump_allocator)
{
    // load Serenity!
    
    uint32_t addr = 0x300000;
    for(int i=0; i<2000; ++i)
    {  
        disk->operator.readData(pciDesc,disk,5954+i,0,1,addr); // pciDesc, Disk Desc, LBA0, LBA1, Sector count, Buffer
        addr = addr + 512;
        ide_400ns_delay(0x1f0);
        ide_400ns_delay(0x1f0);
        
    } 
    // do multiboot stuff

    const char cmd[] = "serial_debug root=/dev/hda2\0";
    char* cmd2 = (char*)0x600000;
    int f = 0;
    while(cmd[f] != 0x0)
    {
        cmd2[f] = cmd[f];
        ++f;
    }

    const char name[] = "NahmanOS Bootloader\0";
    char* name2 = (char*)0x600000+0x100;
    f = 0;
    while(name[f] != 0x0)
    {
        name2[f] = name[f];
        ++f;
    }

    multiboot_info_t* ptr = (multiboot_info_t*)0x1500;
    e820map_entry_t* mmap = (e820map_entry_t*)(0x700);

    //e820map_entry_t* messed_map = (e820map_entry_t*)mess_memory_map(mmap, bump_allocator);

    //e820map_entry_t* mmap2 = (e820map_entry_t*)build_real_memory_map(bump_allocator);

    multiboot_load(ptr,cmd2,mmap,6,name2);
    
    Elf32MainHeader* h = (Elf32MainHeader*)(0x300000);
    parseELF(h,0x400000);

    multiboot_loadelf(ptr,h);
    
    multiboot_jump(ptr,h->program_entry_position);
}

e820map_entry_t* mess_memory_map(e820map_entry_t* map,BumpAllocator* bump_allocator)
{
    e820map_entry_t* tmp = (e820map_entry_t*)bump_allocator->addr;
    for(int i=0; i<7;++i)
    {

            for(int j=0; j<2 ;++j)
            {
                e820map_entry_t* tmp2 = (e820map_entry_t*)bump_allocator->allocate(bump_allocator,sizeof(e820map_entry_t));
                tmp2->acpi3 = map[i].acpi3;
                tmp2->type = map[i].type;

                if(j%2==0)
                    tmp2->base_addr1 = map[i].base_addr1+((map[i].length1/2)*j)-188;
                else
                    tmp2->base_addr1 = map[i].base_addr1+((map[i].length1/2)*j);
                tmp2->length1 = map[i].length1/2;

            }
        
    }
    return tmp;
}

e820map_entry_t* build_real_memory_map(BumpAllocator* bump_allocator)
{
    e820map_entry_t* tmp = (e820map_entry_t*)bump_allocator->addr;
    bump_allocator->allocate(bump_allocator,sizeof(e820map_entry_t));
    tmp[0].base_addr1 = 0;
    tmp[0].length1 = 0x9fc00;
    tmp[0].type = 0x1;

    bump_allocator->allocate(bump_allocator,sizeof(e820map_entry_t));
    tmp[1].base_addr1 = 0x9fc00;
    tmp[1].length1 = 0x400;
    tmp[1].type = 0x2;
    
    bump_allocator->allocate(bump_allocator,sizeof(e820map_entry_t));
    tmp[2].base_addr1 = 0xe6000;
    tmp[2].length1 = 0x1a000;
    tmp[2].type = 0x2;

    bump_allocator->allocate(bump_allocator,sizeof(e820map_entry_t));
    tmp[3].base_addr1 = 0x100000;
    tmp[3].length1 = 0x1fd30000;
    tmp[3].type = 0x1;

    bump_allocator->allocate(bump_allocator,sizeof(e820map_entry_t));
    tmp[4].base_addr1 = 0x1fe30000;
    tmp[4].length1 = 0x11810;
    tmp[4].type = 0x4;

    bump_allocator->allocate(bump_allocator,sizeof(e820map_entry_t));
    //tmp[5].base_addr1 = 0x1fe42000;
    tmp[5].base_addr1 = 0x1fe41810;
    //tmp[5].length1 = 0xee7f0;
    tmp[5].length1 = 0x1000000;

    tmp[5].type = 0x1;

    bump_allocator->allocate(bump_allocator,sizeof(e820map_entry_t));
    tmp[6].base_addr1 = 0x01ff30000;
    tmp[6].length1 = 0x10000;
    tmp[6].type = 0x3;

    bump_allocator->allocate(bump_allocator,sizeof(e820map_entry_t));
    tmp[7].base_addr1 = 0x01ff40000;
    tmp[7].length1 = 0xb0000;
    tmp[7].type = 0x4;

    bump_allocator->allocate(bump_allocator,sizeof(e820map_entry_t));
    tmp[8].base_addr1 = 0x01fff0000;
    tmp[8].length1 = 0x10000;
    tmp[8].type = 0x2;
    
    bump_allocator->allocate(bump_allocator,sizeof(e820map_entry_t));
    tmp[9].base_addr1 = 0xfecf0000;
    tmp[9].length1 = 0x1000;
    tmp[9].type = 0x2;

    bump_allocator->allocate(bump_allocator,sizeof(e820map_entry_t));
    tmp[10].base_addr1 = 0xfed20000;
    tmp[10].length1 = 0x80000;
    tmp[10].type = 0x2;

    return tmp;
}

void welcome()
{
    const short color = 0x0F00;
    const char* hello = "NahmanOS Bootloader. Loading";
    short* vga = (short*)0xb8000;
    for (int i = 0; i < (int)strlen(hello);++i)
        vga[i] = color | hello[i];
}

void halt()
{
    asm("cli\n"
        "hlt");
}


