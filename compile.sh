make all

DRIVE_PARAMS="-drive file=vm.img,if=none,id=drv0,format=raw"
NVME_PARAMS="-device nvme,drive=drv0,serial=foo"
LIVE_ISO="-cdrom ../archlinux-2019.01.01-x86_64.iso"
DEBUG_PARAMS="-no-shutdown"
BOOTSYSTEM_PARAMS="-boot d --enable-kvm -smp 2"
MONITOR_PARAMS="-monitor stdio"
CPU_PARAMS="-cpu Nehalem"
MACHINE_TYPE="-machine q35"
MEMORY_ALLOCATION="-m 1024M"
GRAPHICS_PARAMS="-vga std"
USB_DEVICE="-device qemu-xhci"
SOUND_DEVICE="-device AC97"
NETWORK_DEVICE="-device rtl8139"
SECONDARY_AHCI_CONTROLLER="-device ahci"
DEVICES_LIST="$GRAPHICS_PARAMS $DRIVE_PARAMS $NVME_PARAMS $USB_DEVICE $SOUND_DEVICE $NETWORK_DEVICE $SECONDARY_AHCI_CONTROLLER"

#qemu-system-x86_64 -machine q35 -drive if=mvme,file=vm.img,index=0,media=disk,format=raw -m 512M -cpu qemu64 -monitor stdio #
qemu-system-x86_64 $MACHINE_TYPE $MEMORY_ALLOCATION $DEVICES_LIST $CPU_PARAMS $MONITOR_PARAMS $LIVE_ISO $BOOTSYSTEM_PARAMS $DEBUG_PARAMS
echo ""