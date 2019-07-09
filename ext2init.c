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
struct ext2_inode disk_dir; //根目录 / 

FILE *disk;

// super block init
BOOL super_block_init(){
    // 初始化超级块
    super_block.s_inodes_count = 1024;          // 索引节点总数
    super_block.s_blocks_count = 1024;          // 文件系统总块数
    super_block.s_r_blocks_count = 8;          // 为超级块保留的块数    
    super_block.s_free_blocks_count = 892;     // 1024(总块数)-1(超级块)-1(GDT[1])-1(block bitmap)-1(inode bitmap)-128(inode table)空闲物理块数
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
    if((disk = fopen(DISK,"r+"))==NULL){
        printf("open dir faild!\n");
        return DIR_IS_FALSE;
    }else{
        fseek(disk,0,SEEK_SET);
        if(fwrite(&super_block,sizeof(struct ext2_super_block),1,disk)){ 
            /*  将内存中定义的super_blcok写到磁盘文件disk指向的区域
            */
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
    group_desc_table.GDT[1].bg_block_bitmap = 3;             // 块位图所在块号
    group_desc_table.GDT[1].bg_inode_bitmap = 4;             // 索引节点位图所在块号        group_desc_table.bg_inode_table = 5;              // 组中索引节点表的首块号
    group_desc_table.GDT[1].bg_free_blocks_count = 892;      // TODO 组中空闲块数
    group_desc_table.GDT[1].bg_free_inodes_count = 1024;     // TODO 组中空闲节点数目
    group_desc_table.GDT[1].bg_used_dirs_count = 64;         // TODO 组中分配给目录的节点数目
    group_desc_table.GDT[1].bg_pad = 0;
    //group_desc_table.padding;
                          // 填充，对齐到字
    
    if((disk=fopen(DISK,"r+"))==NULL){
        printf("GDT[1] initialize faild!\n");
        return IS_FALSE;
    }else
    {
        fseek(disk,SUPER_BLOCK_LENGTH,SEEK_SET);
        if(fwrite(&group_desc_table,sizeof(struct ext2_group_desc_table),1,disk)){
            fclose(disk);
            printf("GDT[1] initialized!\n");
            return IS_TRUE;
        }else
        {
            fclose(disk);
            printf("GDT[1] initialize failed!\n");
            return IS_FALSE;
        }
    }
}

// block bitmap
BOOL block_bitmap_init(){
    if((disk=fopen(DISK,"r+"))==NULL){
        printf("block bitmap initialize faild!\n");
        return IS_FALSE;
    }else
    {  
        int i,j;
        for(i = 0; i < 111; i++){
            bmap.block_bitmap[i] = 255;
        }
        bmap.block_bitmap[111] = 0b11110000;
        for(j = 0; j < 912; j++){
            bmap.block_bitmap[112+j] = 0x00;
        }
        // 我是怎么计算那些位该写０还是１的：
        /* 首先我的文件系统块大小是1KB, 共1024块；
        *   没有引导块；　超级块1KB , 块组描述符表1KB , 数据块位图1KB, inode位图1KB, inode节点表128B * 1024个inode;
        *   所以还剩892个空闲数据块，需要892b也就是111B + 4b的１来表示空闲；所以在数据块位图block bitmap中需要111B的255(11111111)＋0b11110000来表示
        *   空闲数据块；但是分配给block bitmap有1kB,　所以后面的912B用０来填充表示以占用，不能分配
        */
        fseek(disk,SUPER_BLOCK_LENGTH+GROUP_DESC_BLOCK_LENGTH,SEEK_SET);
       if(fwrite(&bmap, sizeof(struct ext2_block_bitmap),1,disk)){
            fclose(disk);
            printf("block bitmap initialized!\n");
            return IS_TRUE;
       }else{
           fclose(disk);
           printf("block bitmap initialize faild!\n");
           return IS_FALSE;
       }
        
    }
    
}

// inode bitmap 
BOOL inode_bitmap_init(){
    if((disk=fopen(DISK,"r+"))==NULL){
        printf("inode bitmap initialize faild!\n");
        return IS_FALSE;
    }else{
        int i,j;
        for(i=0; i<128; i++){
            imap.inode_bitmap[i] = 255;  
        }
        for(j=0; j<896; j++){
            imap.inode_bitmap[128+j] = 0;
        }
        /*  置位逻辑：共有1024个inode，所以只需要1024/8=128B, 而imap定义为 __u8 , 即1B,所以将前１２８Ｂ置位；但是inode bitmap分配1KB
        *   所以后896B需要清零，表示不能分配
        */
       fseek(disk,SUPER_BLOCK_LENGTH+GROUP_DESC_BLOCK_LENGTH+BLOCK_INDEX_BMP_SIZE,SEEK_CUR);
       if(fwrite(&imap,sizeof(struct ext2_inode_bitmap),1,disk)){
            fclose(disk);
            printf("inode bitmap initialized!\n");
       }else{
           fclose(disk);
           printf("inode bitmap initialize faild!\n");
       }
       
    }
}

// get free block; 通过查找block bitmap获取第一个空闲数据块块号(从0开始编号)，以块位单位分配，返回数据块编号
__u16 get_free_block(){
    __u32 index = SUPER_BLOCK_LENGTH+GROUP_DESC_BLOCK_LENGTH;
    __u32 cur = index;
    printf("search for free block...\n");
    if((disk=fopen(DISK,"r+"))==NULL){
        fseek(disk,cur,SEEK_SET);
        for(;cur < BIT_MAP_SIZE*8; ){
            __u8 cur_char = fgetc(disk);
            if(cur_char == 255){ //　这里有待理解
                cur += 8; 
            }else{
                while (cur_char &0x80)
                {
                    cur_char = cur_char << 1;
                    ++cur;
                }
                printf("cur index = %d\n",cur);
                printf("first free block munber: %d",cur-index);
                fclose(disk);
                return cur-index;
            }
        }
        printf("search free block failed!\n");
        fclose(disk);
        return 0;
    }
    printf("search free block failed!\n");
    fclose(disk);
    return 0;
}

// set free block,设置块位图，默认0号块
BOOL set_block_bitmap(int offset, int value){
    if(offset >= BIT_MAP_SIZE){
        printf("segment fault!\n");
        return IS_FALSE;
    }
    __u32 index = SUPER_BLOCK_LENGTH+GROUP_DESC_BLOCK_LENGTH;
    __u32 cur = index + offset/8;
    // 先取出要设置的位置和设置值对比，如果不同则设置，相同则置位
    if((disk=fopen(DISK,"r+")) != NULL){
        fseek(disk,cur,SEEK_SET);
        __u8 cur_char = fgetc(disk);
        __u8 tmp_char;
        if(offset % 8){
            tmp_char = cur_char >> (8 - offset%8 +1);
            cur_char = cur_char << (offset%8 - 1);
        }
        __u8 cur_bit = cur_char & 0x80;
        // 如果欲置位和当前位不同则置位
        if(value^cur_bit == 1){
            cur_char = value;
        }else{
            fclose(disk);
            printf("block bitmap set error!\n");
            return IS_FALSE;
        }
        if(offset%8){
            cur_char = cur_char >> (offset%8 -1 );
            tmp_char = tmp_char << (8 - offset%8 + 1);
            cur_char = cur_char + tmp_char;
        }
        // 写回磁盘； 且重新寻址
        fseek(disk,cur,SEEK_SET);
        if(fputc(cur_char,disk) != EOF){
            printf("set block bitmap!\n");
            super_block.s_free_blocks_count--; //表示的是申请一个block成功后，相应空闲块数-1
            group_desc_table.GDT[1].bg_free_blocks_count--;
            fclose(disk);
            return IS_FALSE;
        }
    } 
    fclose(disk);
    printf("set block bitmap faild!\n");
    return IS_FALSE;
}

// get free inode; 在创建新文件时会调用
__u16 get_free_inode(){
    __u32 index = SUPER_BLOCK_LENGTH+GROUP_DESC_BLOCK_LENGTH+BIT_MAP_SIZE;
    __u32 cur = index;
    if((disk=fopen(DISK,"r+")) != NULL){
        fseek(disk,cur,SEEK_SET);
        printf("free inode searching...\n");
        for(; cur < INDEX_MAP_SIZE*8; ){
            __u8 cur_char = fgetc(disk);
            if(cur_char == 255){
                cur += 8;
            }else{
                while (cur_char & 0x80)
                {
                    cur_char = cur_char << 1;
                    ++cur;
                }
                // 找到第一个空闲inode
                fclose(disk);
                printf("first free inode number: %d\n",cur-index);
                return cur-index;     
            }
        }
        fclose(disk);
        printf("inode search failed!\n");
        return 0;
    }
    fclose(disk);
    printf("inode search failed!\n");
    return 0;
}

//set inode bitmap
BOOL set_inode_bitmap(int offset, int value){
    // 越界检查
    if(offset >= INDEX_MAP_SIZE){
        printf("inode bitmap segment fault!\n");
        return IS_FALSE;
    }
    // 从inode bitmap中检查
    __u32 index = SUPER_BLOCK_LENGTH+GROUP_DESC_BLOCK_LENGTH+BLOCK_INDEX_BMP_SIZE;
    __u32 cur = index + offset/8;
    // 取出位图对应位，和欲设置的对比
    if((disk=fopen(DISK,"r+")) != NULL){
        fseek(disk,cur,SEEK_SET);
        __u8 cur_char = fgetc(disk);
        __u8 tmp_char;
        if(offset%8){
            tmp_char = cur_char >> (8 - offset%8 + 1);
            cur_char = cur_char << (offset%8 - 1);
        }
        __u8 cur_bit = cur_char & 0x80;
        if(value^cur_bit == 1){
            cur_char = value;
        }else{
            fclose(disk);
            printf("set inode bitmap failed!\n");
            return IS_FALSE;
        }

        if(offset%8){
            cur_char = cur_char >> (offset%8 - 1);
            tmp_char = tmp_char << (8 - offset%8 + 1);
            cur_char = cur_char + tmp_char;
        }
        //写回磁盘；重定位读写指针
        fseek(disk,cur,SEEK_SET);
        if(fputc(cur_char,disk) != EOF){
            super_block.s_free_inodes_count--;
            group_desc_table.GDT[1].bg_free_inodes_count--;
            fclose(disk);
            printf("set inode bitmap!\n");
            return IS_TRUE;
        }else{
            fclose(disk);
            printf("set inode bitmap failed!\n");
            return IS_FALSE;
        }
    }
}

// check disk and alloc disk
BOOL disk_alloc(){
    FILE *fp;
    if((fp=fopen(DISK,"r+")) == NULL){
        fclose(fp);
        printf("disk not exsit!\n");
        return IS_FALSE;
    }else{
        fclose(fp);
        printf("disk opend normally!\n");
        return IS_TRUE;
    }
}

// root inode
BOOL root_inode_init(){
    /*  disk_dir 是 "/" 系统根目录inode
    *       disk_entry 是系统根目录项
    *       disk_cur_dir_entry
    *   root_dir 是 "root" rooty用户目录inode
    *       root_entry 是root用户目录项
    */
    // 定义根目录的权限
    __u16 permit = 0x1111;
    disk_dir.i_type = DIR_FILE;
    disk_dir.i_mode = permit;
    disk_dir.i_uid = 0;                // 文件拥有者0

    // 获取当前时间戳
    __u32 current_time; 
    time_t t = time(NULL);
    current_time = time(&t);
    disk_dir.i_atime = current_time;
    disk_dir.i_ctime = current_time;
    disk_dir.i_mtime = current_time;
    disk_dir.i_dtime = 0xFFFF;

    disk_dir.i_gid = 0;                        // 文件用户组标识符
    disk_dir.i_links_count = 2;                // 硬链接计数，初始值为0，创建root目录之后为1
    disk_dir.i_blocks = 1;                     // 文件所占块数，初始化为1
    disk_dir.i_size = 2;
    disk_dir.i_flags = 3;                      // 设置文件打开方式为读写
    disk_dir.i_block[0] = 0;

    // 设置索引位图当中的某一位
    // 获取第一个inode节点，讲其编号为1，注意，inode节点编号为0的不用
    // 设置索引位图当中的某一位_
    __u16 disk_inode_number = get_free_inode(); // "/" inode节点
    if(disk_inode_number && set_block_bitmap(disk_inode_number,BLOCK_INDEX_IN_USE)){
    // "/" 目录项结构
    struct ext2_dir_entry_2 disk_entry;
    disk_entry.inode = disk_inode_number;
    disk_entry.rec_len = 256;
    disk_entry.file_type = DIR_FILE;
    strcpy(disk_entry.name, "/");
    disk_entry.name_len = 1;


    // 设置"/"目录中 "." (当前目录) 目录项结构
    struct ext2_dir_entry_2 disk_cur_dir_entry, disk_parent_dir_entry;
    disk_cur_dir_entry.inode = disk_inode_number; 
    disk_cur_dir_entry.rec_len = 256;
    disk_cur_dir_entry.file_type = DIR_FILE;
    strcpy(disk_cur_dir_entry.name, ".");
    disk_cur_dir_entry.name_len = 1;
    // 设置"/"目录中 ".." (父目录)目录项结构
    disk_parent_dir_entry.inode = disk_inode_number;
    disk_parent_dir_entry.rec_len = 256;
    disk_parent_dir_entry.file_type = DIR_FILE;
    strcpy(disk_parent_dir_entry.name, "..");
    disk_parent_dir_entry.name_len = 2;

    // 创建根目录"/"中 root 用户目录 "/root" iNode
    __u16 root_inode_number = get_free_inode();
    // 测试 root inode是否申请成功
    if(root_inode_number && set_inode_bitmap(root_inode_init,BLOCK_INDEX_IN_USE)){
        struct ext2_inode root_dir; // root目录inode节点
        root_dir.i_type = 0x1111; // 文件类型
        root_dir.i_mode = permit;
        root_dir.i_uid = 1; //所属用户id

        //time
        __u32 root_dir_time;
        time_t tmp = time(NULL);
        root_dir_time = time(&tmp);
        root_dir.i_atime = root_dir_time;
        root_dir.i_ctime = root_dir_time;
        root_dir.i_mtime = root_dir_time;
        root_dir.i_dtime = 0xffff;

        root_dir.i_gid = 0; // 组编号
        root_dir.i_links_count = 2; // 连接计数: 父目录中 . .. 
        root_dir.i_blocks = 1; // 所占数据块数
        root_dir.i_size = 2;
        root_dir.i_flags = 3; 
        root_dir.i_block[0] = 1;  //占用的数据块编号
        
        // 由于root目录的存在，"/"inode中需要修改如下
        disk_dir.i_size++;
        disk_dir.i_links_count++;

        // root目录中 ".", ".." 当前目录和父目录
        struct ext2_dir_entry_2 root_entry; //root目录结构体
        root_entry.inode = root_inode_number;
        root_entry.rec_len = 256;
        root_entry.file_type = DIR_FILE;
        strcpy(root_entry.name, "root"); //root目录名
        root_entry.name_len = 4;

        // root目录中 ".", ".." 目录项结构
        // "." 
        struct ext2_dir_entry_2 root_cur_dir_entry, root_parent_dir_entry;
        root_cur_dir_entry.inode = root_inode_number;
        root_cur_dir_entry.rec_len = 256;
        root_cur_dir_entry.file_type = DIR_FILE;
        strcpy(root_cur_dir_entry.name, ".");
        root_cur_dir_entry.name_len = 1;
        // ".."
        root_parent_dir_entry.inode = disk_inode_number; //root目录的父目录inode节点号
        root_parent_dir_entry.rec_len = 256;
        root_parent_dir_entry.file_type = DIR_FILE;
        strcpy(root_parent_dir_entry.name, "..");
        root_parent_dir_entry.name_len = 2;

        // 将 "/" 和 "root" inode节点和目录结构写回磁盘；
        if((disk=fopen(DISK, "r+")) != NULL)
            // 写入根节点和root目录的inode
            fseek(disk,FIRST_INODE_BLOCK*EVERY_BLOCK+OUT_INODE_LENGTH*(disk_inode_number - 1),SEEK_SET);
            fwrite(&disk_dir, OUT_INODE_LENGTH, 1, disk);
            fseek(disk,FIRST_INODE_BLOCK*EVERY_BLOCK+OUT_INODE_LENGTH*(root_inode_number - 1),SEEK_SET);
            fwrite(&root_dir, OUT_INODE_LENGTH,1,disk);

            // 写入根节点和root目录的dentry目录结构
            // "/" 目录项
            fseek(disk,FIRST_DATA_BLOCK*EVERY_BLOCK,SEEK_SET);
            fwrite(&disk_entry,DIR_DENTRY_LENGTH,1,disk);
            // "/." 目录项
            fseek(disk,EVERY_BLOCK,SEEK_CUR);
            fwrite(&disk_cur_dir_entry,DIR_DENTRY_LENGTH,1,disk);
            // "/.."目录项
            fseek(disk,DIR_DENTRY_LENGTH,SEEK_CUR);
            fwrite(&disk_parent_dir_entry,DIR_DENTRY_LENGTH,1,disk);

            // "root"目录项
            fseek(disk,(FIRST_DATA_BLOCK+1)*EVERY_BLOCK,SEEK_SET);
            fwrite(&root_entry,DIR_DENTRY_LENGTH,1,disk);
            // "root/."目录项
            fseek(disk,DIR_DENTRY_LENGTH,SEEK_CUR);
            fwrite(&root_cur_dir_entry,DIR_DENTRY_LENGTH,1,disk);
            // "root/.."目录项
            fseek(disk,DIR_DENTRY_LENGTH,SEEK_CUR);
            fwrite(&root_parent_dir_entry,DIR_DENTRY_LENGTH,1,disk);

            // 设置数据块位图和inode位图被占用标志位
            // 设置数据块被占用的标志
            printf("空闲块数目：%d\n", get_free_block());
            set_block_bitmap(FIRST_DATA_BLOCK + 1, BLOCK_INDEX_IN_USE);
            printf("空闲块数目：%d\n", get_free_block());
            set_block_bitmap(FIRST_DATA_BLOCK + 2, BLOCK_INDEX_IN_USE);
            printf("空闲块数目：%d\n", get_free_block());

            fclose(disk);
            printf("/ and root dir initialized!\n");
            printf("\n / inode number: %d\n", disk_inode_number);
            printf("\n root inode number: %d\n",root_inode_number);
        }else{
            fclose(disk);
            printf("/ and root dir initialize failed!\n");
            return IS_FALSE;
        }
    }
}

// ext2 重要结构体初始化成功
BOOL ext2fs_init(){
       if(super_block_init()){
           printf("超级块初始化成功!\n");
       }
       if(group_desc_table_init()){
           printf("组块描述符表初始化成功!\n");
       }
       if(block_bitmap_init()){
           printf("数据块位图初始化成功!\n");
       }
       if(inode_bitmap_init()){
           printf("索引节点表初始化成功!\n");
       }
       if(root_inode_init()){
           printf("根目录及root目录初始化成功!\n");
       }
}