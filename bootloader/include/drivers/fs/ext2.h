#ifndef EXT2FS__H
#define EXT2FS__H

#include <stdint.h>
#include "memory/bump_alloc.h"
#include "drivers/fs/partition.h"

typedef struct ext2_superblock {
    uint32_t inodes_count;
    uint32_t blocks_count;
    uint32_t blocks_reserved_count;

    uint32_t unallocated_blocks_count;
    uint32_t unallocated_inodes_count;

    uint32_t block_contains_superblock;

    uint32_t block_size;
    uint32_t fragment_size;

    uint32_t block_group_blocks_count;
    uint32_t block_group_fragments_count;
    uint32_t block_group_inodes_count;

    uint32_t last_mount_time;
    uint32_t last_written_time;

    uint16_t mount_count;
    uint16_t mount_count_before_check;
    uint16_t fs_signature;
    uint16_t fs_state;
    uint16_t error_operation_vector;
    uint16_t minor_version_number;

    uint32_t posix_time_last_check;
    uint32_t interval_forced_checks;
    uint32_t os_id_fs_creator;
    uint32_t major_version_number;

    uint16_t userid_reserved_blocks;
    uint16_t groupid_reserved_blocks;
} __attribute__((__packed__)) ext2_superblock;

typedef struct ext2_extended_superblock {
    ext2_superblock basic_superblock;

    uint32_t first_nonreserved_inode;
    uint16_t inode_bytes_size;
    uint16_t blockgroup_this_superblock;

    uint32_t optional_features_present;
    uint32_t required_features_present;
    uint32_t write_required_features;
    
    uint32_t fs_id[4];
    char volume_name[16];
    char path_volume_mounted[64];

    uint32_t compression_algorithms_used;
    uint8_t preallocate_blocks_for_files_count;
    uint8_t preallocate_blocks_for_directories_count;

    uint16_t unused;
    uint32_t journal_id[4];
    uint32_t journal_inode;
    uint32_t journal_device;
    uint32_t orphan_inode_list_head;
    uint8_t unused2[1024-236];
} __attribute__((__packed__)) ext2_extended_superblock;

typedef enum
{
	FS_CLEAN    = 0x1,
	FS_ERROR	= 0x2
} Ext2FS_States;

typedef enum
{
	Ignore              = 0x1,
	Remount_Readonly	= 0x2,
    Kernel_Panic        = 0x3
} Ext2FS_Error_Handling_Methods;

typedef enum
{
	Linux               = 0x0,
	GNU_HURD        	= 0x1,
    MASIX               = 0x2,
    FreeBSD             = 0x3,
    OtherLites          = 0x4
} Ext2FS_Creator_ID;

typedef enum
{
	Preallocate_blocks_when_creating_new_dir = 0x1,
    AFS_server_inodes_exist                  = 0x2,
    File_system_has_a_journal                = 0x4,
    Inodes_have_extended_attributes          = 0x8,
    Filesystem_can_resize_larger_itself      = 0x10,
    Directories_use_hash_index               = 0x20
} Ext2FS_Optional_Feature_Flags;

typedef enum
{
	Compression_is_used                     = 0x1,
    Directory_entries_contain_type_field    = 0x2,
    Filesystem_needs_replay_journal         = 0x4,
    Filesystem_uses_journal_device          = 0x8
} Ext2FS_Required_Feature_Flags;

typedef enum
{
	Sparse_superblocks_group_descriptor_tables = 0x1,
    Filesystem_uses_64_bit_filesize            = 0x2,
    Directory_contents_stored_in_binarytree    = 0x4
} Ext2FS_ReadOnly_Feature_Flags;

typedef struct ext2_block_group_descriptor {
    uint32_t block_usage_bitmap_blkaddr;
    uint32_t inode_usage_bitmap_blkaddr;
    uint32_t inode_table_blkaddr;

    uint16_t unallocated_blocks_count;
    uint16_t unallocated_inodes_count;
    uint16_t directories_count;

    uint8_t unused[32-18];
} __attribute__((__packed__)) ext2_block_group_descriptor;

typedef struct ext2_inode {
    uint16_t type_permissions;
    uint16_t user_id;
    uint32_t sizel;
    uint32_t last_access_time;
    uint32_t creation_time;
    uint32_t last_modification_time;
    uint32_t deletion_time;
    uint16_t group_id;
    uint16_t hard_links_this_inode_count;
    uint32_t disk_sectors_used_count;
    uint32_t flags;
    uint32_t os_specific_value1;

    uint32_t direct_blk_ptr[12];
    uint32_t singly_indirect_blk_ptr;
    uint32_t doubly_indirect_blk_ptr;
    uint32_t triply_indirect_blk_ptr;

    uint32_t generation_number;
    uint32_t extended_attribute_block;
    uint32_t sizeh;

    uint32_t fragment_blk_addr;
    uint8_t os_specific_value2[12];
} __attribute__((__packed__)) ext2_inode;

typedef enum
{
	FIFO                = 0x1000,
    Character_device    = 0x2000,
    Directory           = 0x4000,
    Block_device        = 0x6000,
    Regular_file        = 0x8000,
    Symbolic_link       = 0xA000,
    Unix_socket         = 0xC000
} Ext2FS_Inode_Type;

typedef enum
{
	Other_Execute       = 0x001,
    Other_Write         = 0x002,
    Other_Read          = 0x004,
    Group_Execute       = 0x008,
    Group_Write         = 0x010,
    Group_Read          = 0x020,
    User_Execute        = 0x040,
    User_Write          = 0x080,
    User_Read           = 0x100,
    Sticky_Bit          = 0x200,
    Set_Group_ID        = 0x400,
    Set_User_ID         = 0x800
} Ext2FS_Inode_Permissions;

typedef enum
{
	Secure_deletion                 = 0x00000001,
    Keep_copy_of_data_on_delete     = 0x00000002,
    File_compression                = 0x00000004,
    Synchronous_updates             = 0x00000008,
    Immutable_file                  = 0x00000010,
    Append_only                     = 0x00000020,
    File_not_included_in_dump       = 0x00000040,
    Last_accessed_time_not_updating = 0x00000080,
    Hash_indexed_directory          = 0x00010000,
    AFS_directory                   = 0x00020000,
    Journal_file_data               = 0x00040000
} Ext2FS_Inode_Flags;

typedef struct ext2_directory_entry {
    uint32_t inode;
    uint16_t entry_size;
    uint8_t length;
    uint8_t type_indicator;
    char name[];
} __attribute__((__packed__)) ext2_directory_entry;
/*
typedef enum
{
    Unknown             = 0x0,
	Regular_file        = 0x1,
    Directory           = 0x2,
    Character_device    = 0x3,
    Block_device        = 0x4,
    FIFO                = 0x5,
    Socket              = 0x6,
    Symbolic_link       = 0x7
} Ext2FS_Directory_Entry_TypeIndicator;
*/
ext2_extended_superblock get_ext2_superblock(Partition* partition,BumpAllocator* bump_allocator);

#endif