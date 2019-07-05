#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <time.h>
#include "ext2struc.h"
#define IS_TRUE 1
#define IS_FALSE 0
// super block init
BOOL super_block_init();

// group descriptor block
BOOL group_desc_table_init();

// block bitmap
BOOL block_bitmap_init();

// inode bitmap 
BOOL inode_bitmap_init();

// inode table 
BOOL inode_table_init(); 

// get free block 
__u16 get_free_block();

// set block bitmap, 设置偏移位offset的bitmap位value
BOOL set_block_bitmap(int offset, int value);

// get free inode
__u16 get_free_inode();

// BOOL set inode bitmap
BOOL set_inode_bitmap(int offset, int value);


// check disk and alloc disk
BOOL disk_alloc();

// 外存inode节点初始化，创建根目录以及root目录
BOOL root_inode_init();

// ext2 filesystem init
BOOL ext2fs_init();
