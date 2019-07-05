#include "datatype.h"
#include "ext2init.h"
#include "ext2struc.h"
// 文件系统结构体初始化
// 实例化各种结构体
struct ext2_super_block super_block;
struct ext2_group_desc_table group_desc_table;
struct ext2_block_bitmap bmap;
struct ext2_inode_bitmap imap;
struct ext2_inode_table inode_table;
struct ext2_inode inode_root;

 FILE *disk;

// super block init
BOOL super_block_init(){
    // 初始化超级块
    super_block.s_inodes_count = 1024;          // 索引节点总数
    super_block.s_blocks_count = 1024;          // 文件系统总块数
    super_block.s_r_blocks_count = 8;          // 为超级块保留的块数    
    super_block.s_free_blocks_count = 892;     // 1024(总块数)-1(超级块)-1(GDT)-1(block bitmap)-1(inode bitmap)-128(inode table)空闲物理块数
    super_block.s_free_inodes_count = 1024;     // 空闲索引节点总数
    super_block.s_log_block_size = 0;           // 每一块的大小，此处为1024k
    super_block.s_first_data_block = FIRST_DATA_BLOCK;         // 文件系统中的第一个数据块, inode节点需要128个磁盘块，故开始的数据块为1+1+1+128 = 131块
    super_block.s_log_frag_size = 1;            // 片大小，此处不使用
    super_block.s_blocks_per_group = 1024;      // 每一个组中为1024个块，总共8个组
    super_block.s_frags_per_group = 0;          // 片数目，这里省略
    super_block.s_inodes_per_group = 0;         // 每个组中的索引节点数目为1024，同物理块保持一致
    super_block.s_mtime = 1;                    // TODO 赋值为当前时间
    super_block.s_wtime = 1;                    // TODO 最后一次安装操作的时间，也是当前时间
    super_block.s_mnt_count = 1;                // 安装计数
    super_block.s_max_mnt_count = 1;            // 最大可安装计数
    super_block.s_magic = 1;                    // 文件系统版本
    super_block.s_state = EXT2_IS_TRUE;        // 文件系统的状态，初始化为正常
    super_block.s_errors = 1;                   // TODO 检测到错误时候的出错处理
    super_block.s_minor_rev_level = 1;          // 次版本号
    super_block.s_lastcheck = 1;                // TODO 最后一次检查的时间，设置为当前时间
    super_block.s_checkinterval = 1000;         // TODO 两次对文件系统进行状态检查的间隔时间
    super_block.s_rev_level = 1;                // TODO 版本号
    super_block.s_def_resuid = 1;               // TODO 保留快的默认用户标识号
    super_block.s_def_resgid = 1;               // TODO 保留快的默认用户组标识号
    super_block.s_first_ino = 1;                // TODO 第一个非保留的索引节点
    super_block.s_inode_size = 1;               // TODO 索引节点的大小
    super_block.s_block_group_nr = 0;           // 该超级块的块组号
    super_block.s_feature_compat = 0;           // TODO 兼容特点的位图
    super_block.s_feature_incompat = 0;         // TODO 非兼容特点的位图
    super_block.s_uuid[0] = 1;                  // TODO 128位的文件系统标识号
    strcpy(super_block.s_volume_name, "/home");    // 卷名
    strcpy(super_block.s_last_mounted, DISK);   // 最后一个安装点的路径名
    super_block.s_algorithm_usage_bitmap = 1;   // TODO 用于压缩
    super_block.s_prealloc_blocks = 1;          // 预分配的块数
    super_block.s_prealloc_dir_blocks = 1;      // 给目录预分配的块数
    super_block.s_padding1 = 0;                 // 填充
    super_block.s_reserved[0] = 1;              // 全部用0填充完一块

   
    ; //读写打开文件 /home/ringfu/ext2
    if((disk = fopen(DISK,"rw"))==NULL){
        printf("open dir faild!\n");
        return DIR_IS_FALSE;
    }else{
        fseek(disk,0,0);
        if(fwrite(&super_block,sizeof(struct ext2_super_block),1,disk)){
            fclose(disk);
            printf("super block initialized!\n");
            return DIR_IS_TRUE;
        }else
        {
            fclose(disk);
            printf("super block initialize faild!\n");
        }
    }    

}

// group descriptor block
BOOL group_desc_table_init(){
    group_desc_table.GDT.bg_block_bitmap = 3;             // 块位图所在块号
    group_desc_table.GDT.bg_inode_bitmap = 4;             // 索引节点位图所在块号        group_desc_table.bg_inode_table = 5;              // 组中索引节点表的首块号
    group_desc_table.GDT.bg_free_blocks_count = 892;      // TODO 组中空闲块数
    group_desc_table.GDT.bg_free_inodes_count = 1024;     // TODO 组中空闲节点数目
    group_desc_table.GDT.bg_used_dirs_count = 64;         // TODO 组中分配给目录的节点数目
    group_desc_table.bg_pad = 0;                      // 填充，对齐到字
    
    if((disk=fopen(DISK,"rw"))==NULL){
        printf("GDT initialize faild!\n");
        return IS_FALSE;
    }else
    {
        fseek(disk,SUPER_BLOCK_LENGTH,SEEK_SET);
        if(fwrite(&group_desc_table,sizeof(struct ext2_group_desc_table),1,disk)){
            fclose(disk);
            printf("GDT initialized!\n");
            return IS_TRUE;
        }else
        {
            fclose(disk);
            printf("GDT initialize failed!\n");
            return IS_FALSE;
        }
    }
}

// block bitmap
BOOL block_bitmap_init(){
    if((disk=fopen(DISK,"rw"))==NULL){
        printf("block bitmap initialize faild!\n");
        return IS_FALSE;
    }else
    {
        
    }
    
}

// inode bitmap 
BOOL inode_bitmap_init(){

}

// get free block 
__u16 get_free_block(){

}

// set free block
BOOL set_block_bitmap(){

}

// get free inode
__u16 get_free_inode(){

}

// BOOL set inode bitmap
BOOL set_inode_bitmap(){

}

// check disk and alloc disk
BOOL disk_alloc(){

}

// ext2 filesystem init
BOOL ext2fs_init{
       
}