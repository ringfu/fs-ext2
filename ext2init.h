// 初始化文件系统中几个部分
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <time.h>
#include "ext2str(uc.h"

// super block init
bool super_block_init();

// group descriptor block
bool group_desc_block_init();

// block bitmap
bool block_bitmap_init();

// inode bitmap 
bool inode_bitmap_init();

// get free block 
__u16 get_free_block();

// set free block
bool set_block_bitmap();

// get free inode
__u16 get_free_inode();

// bool set inode bitmap
bool set_inode_bitmap();

// check disk and alloc disk
bool disk_alloc();

// 外存inode节点初始化，创建根目录以及root目录
bool root_inode_init();

// ext2 filesystem init
bool ext2fs_init();
