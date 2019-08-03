#ifndef DISKNVME_H
#define DISKNVME_H

#include <stdint.h>

struct NVMeControllerVersion
{
    uint8_t tertiary_version_number;
    uint8_t minor_version_number;
    uint16_t major_version_number;
} __attribute__((__packed__));
typedef struct NVMeControllerVersion NVMeVersionRegister;

struct NVMeControllerRegisters {
    
    uint32_t capabilities1;
    uint32_t capabilities2;

    NVMeVersionRegister version;

    uint32_t interrupt_mask_set;
    uint32_t interrupt_mask_clear;

    uint32_t controller_configuration;

    uint32_t reserved;

    uint32_t controller_status;
    uint32_t nvm_subsystem_reset;

    uint32_t admin_queue_attributes;

    uint32_t admin_submission_queue_base_addr1;
    uint32_t admin_submission_queue_base_addr2;

    uint32_t admin_completion_queue_base_addr1;
    uint32_t admin_completion_queue_base_addr2;

    uint32_t controller_memory_buffer_location;
    uint32_t controller_memory_buffer_size;

    uint32_t boot_partition_info;
    uint32_t boot_partition_read_select;

    uint32_t boot_partition_memory_buffer_location1;
    uint32_t boot_partition_memory_buffer_location2;

    uint32_t controller_memory_buffer_space_control1;
    uint32_t controller_memory_buffer_space_control2;

    uint32_t controller_memory_buffer_status;

    uint8_t reserved2[3492];

    uint32_t persistent_memory_capabilities;
    uint32_t persistent_memory_region_control;
    uint32_t persistent_memory_region_status;

    uint32_t persistent_memory_region_elasticity_buffer_size;
    uint32_t persistent_memory_region_sustained_write_throughtput;

    uint32_t persistent_memory_region_controller_space_control1;
    uint32_t persistent_memory_region_controller_space_control2;

    uint8_t command_set_specific[484];

    uint32_t submission_queue_0_tail_doorbell;


} __attribute__((__packed__));
typedef struct NVMeControllerRegisters NVMeControllerRegisters;

#endif