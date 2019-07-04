// 文件系统结构体初始化
#include　"ext2init.h"
// 实例化各种结构体
struct ext2_super_block super_block;
struct ext2_group_desc gropu_desc;
struct ext2_block_bitmap bmap;
struct ext2_inode_bitmap imap;
struct ext2_inode inodes;


// super block init
bool super_block_init(){
    // 初始化超级块
}

// group descriptor block
bool group_desc_block_init(){

}

// block bitmap
bool block_bitmap_init(){

}

// inode bitmap 
bool inode_bitmap_init(){

}

// get free block 
__u16 get_free_block(){

}

// set free block
bool set_block_bitmap(){

}

// get free inode
__u16 get_free_inode(){

}

// bool set inode bitmap
bool set_inode_bitmap(){

}

// check disk and alloc disk
bool disk_alloc(){

}

// ext2 filesystem init
bool ext2fs_init{
    
}