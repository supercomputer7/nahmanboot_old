void disable_paging()
{
    asm("mov $0x7fffffff,%eax\n\t"
            "mov %cr0,%ebx\n\t"
            "and %eax,%ebx\n\t"
            "mov %ebx,%cr0");
}