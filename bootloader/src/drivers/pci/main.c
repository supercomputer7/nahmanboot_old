#include "drivers/pci/main.h"

initPCIsubsystem()
{
    GetACPITablePointer(rsdp, "FACP");
}