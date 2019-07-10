// 外存inode节点初始化，创建根目录以及root目录
BOOL out_inode_table_init()
{
    // 定义根目录的权限
    __u16 permit = 0x1111;
    root_disk.i_type = DIR_FILE;
    root_disk.i_mode = permit;
    root_disk.i_uid = 0;                // 文件拥有者0
    
    // 获取当前时间戳
    __u32 current_time;
    time_t t = time(NULL);
    current_time = time(&t);
    root_disk.i_atime = current_time;
    root_disk.i_ctime = current_time;
    root_disk.i_mtime = current_time;
    root_disk.i_dtime = 0xFFFF;

    root_disk.i_gid = 0;                        // 文件用户组标识符
    root_disk.i_links_count = 2;                // 硬链接计数，初始值为0，创建root目录之后为1
    root_disk.i_blocks = 1;                     // 文件所占块数，初始化为1
    root_disk.i_size = 2;
    root_disk.i_flags = 3;                      // 设置文件打开方式为读写
    root_disk.i_block[0] = 0;                   // 设置文件系统的第一个数据块

    // 设置索引位图当中的某一位
    // 获取第一个inode节点，讲其编号为1，注意，inode节点编号为0的不用
    // 设置索引位图当中的某一位
    __u16 inode_number = get_one_free_index_bitmap();
    if (inode_number && set_one_bit_of_index_bitmap(inode_number, BLOCK_INDEX_IN_USE, 0))
    {   
        struct ext2_dir_entry_2 root_disk_dentry_one_dot, root_disk_dentry_another_dot;
        root_disk_dentry_one_dot.inode = inode_number;
        root_disk_dentry_one_dot.rec_len = 256;             // 目录项所占空间大小为256个字节
        root_disk_dentry_one_dot.file_type = DIR_FILE;      // 定义文件类型为目录文件
        strcpy(root_disk_dentry_one_dot.name, ".");
        root_disk_dentry_one_dot.name_len = 1;

        root_disk_dentry_another_dot.inode = inode_number;
        root_disk_dentry_another_dot.rec_len = 256;             // 目录项所占空间大小为256个字节
        root_disk_dentry_another_dot.file_type = DIR_FILE;      // 定义文件类型为目录文件
        strcpy(root_disk_dentry_another_dot.name, "..");
        root_disk_dentry_another_dot.name_len = 2;

        // 创建root目录
        __u16 root_inode_number = get_one_free_index_bitmap();
        if (root_inode_number && set_one_bit_of_index_bitmap(root_inode_number, BLOCK_INDEX_IN_USE, 0))
        {
            struct ext2_out_inode root_dir;
            root_dir.i_type = 0x1111;
            root_dir.i_mode = permit;
            root_dir.i_uid = 1;

            __u32 root_dir_make_time;
            time_t temp = time(NULL);
            root_dir_make_time = time(&temp);
            root_dir.i_atime = root_dir_make_time;
            root_dir.i_ctime = root_dir_make_time;
            root_dir.i_mtime = root_dir_make_time;
            root_dir.i_dtime = 0xFFFF;

            root_dir.i_gid = 0;
            root_dir.i_links_count = 2;
            root_dir.i_blocks = 1;
            root_dir.i_size = 2;
            root_dir.i_flags = 3;
            root_dir.i_block[0] = 1;

            root_disk.i_size++;
            root_disk.i_links_count++;

            struct ext2_dir_entry_2 root_dir_dentry;
            root_dir_dentry.inode = root_inode_number;
            root_dir_dentry.rec_len = 256;             // 目录项所占空间大小为256个字节
            root_dir_dentry.file_type = DIR_FILE;      // 定义文件类型为目录文件
            strcpy(root_dir_dentry.name, "root");
            root_dir_dentry.name_len = 4;

            // 设置root的根目录
            struct ext2_dir_entry_2 root_dir_dentry_one_dot, root_dir_dentry_another_dot;
            root_dir_dentry_one_dot.inode = root_inode_number;
            root_dir_dentry_one_dot.rec_len = 256;             // 目录项所占空间大小为256个字节
            root_dir_dentry_one_dot.file_type = DIR_FILE;      // 定义文件类型为目录文件
            strcpy(root_dir_dentry_one_dot.name, ".");
            root_dir_dentry_one_dot.name_len = 1;

            root_dir_dentry_another_dot.inode = inode_number;
            root_dir_dentry_another_dot.rec_len = 256;             // 目录项所占空间大小为256个字节
            root_dir_dentry_another_dot.file_type = DIR_FILE;      // 定义文件类型为目录文件
            strcpy(root_dir_dentry_another_dot.name, "..");
            root_dir_dentry_another_dot.name_len = 2;

            if ((disk = fopen(DISK, "r+")) != NULL)
            {
                
                printf("\n\n%d, %d\n\n", inode_number, root_inode_number);

                // 向磁盘写入inode节点
                fseek(disk, HOME_LENGTH + FIRST_INODE_BLOCK*EVERY_BLOCK + OUT_INODE_LENGTH*(inode_number - 1), SEEK_SET);
                fwrite(&root_disk, OUT_INODE_LENGTH, 1, disk);
                fseek(disk, HOME_LENGTH + FIRST_INODE_BLOCK*EVERY_BLOCK + OUT_INODE_LENGTH*(root_inode_number - 1), SEEK_SET);
                fwrite(&root_dir, OUT_INODE_LENGTH, 1, disk);
                
                // 向磁盘写入根目录的dentry目录
                fseek(disk, HOME_LENGTH + FIRST_DATA_BLOCK*EVERY_BLOCK, SEEK_SET);
                fwrite(&root_disk_dentry_one_dot, DIR_DENTRY_LENGTH, 1, disk);
                fseek(disk, HOME_LENGTH + FIRST_DATA_BLOCK*EVERY_BLOCK + DIR_DENTRY_LENGTH, SEEK_SET);
                fwrite(&root_disk_dentry_another_dot, DIR_DENTRY_LENGTH, 1, disk);
                fseek(disk, HOME_LENGTH + FIRST_DATA_BLOCK*EVERY_BLOCK + 2*DIR_DENTRY_LENGTH, SEEK_SET);
                fwrite(&root_dir_dentry, DIR_DENTRY_LENGTH, 1, disk);
                // 向磁盘写入root目录的dentry目录
                fseek(disk, HOME_LENGTH + (FIRST_DATA_BLOCK + 1)*EVERY_BLOCK, SEEK_SET);
                fwrite(&root_dir_dentry_one_dot, DIR_DENTRY_LENGTH, 1, disk);
                fseek(disk, HOME_LENGTH + (FIRST_DATA_BLOCK + 1)*EVERY_BLOCK + DIR_DENTRY_LENGTH, SEEK_SET);
                fwrite(&root_dir_dentry_another_dot, DIR_DENTRY_LENGTH, 1, disk);

                // 设置数据块被占用的标志
                printf("空闲块数目：%d\n", get_one_free_block_bitmap());
                set_one_bit_of_block_bitmap(FIRST_DATA_BLOCK + 1, BLOCK_INDEX_IN_USE, 0);
                printf("空闲块数目：%d\n", get_one_free_block_bitmap());
                set_one_bit_of_block_bitmap(FIRST_DATA_BLOCK + 2, BLOCK_INDEX_IN_USE, 0);
                printf("空闲块数目：%d\n", get_one_free_block_bitmap());
                printf("文件系统初始化完成，根目录以及root目录已经创建！\n");
                fclose(disk);
                return IS_TRUE;
            }
        }
    }    
    printf("硬盘操作失败！\n");
    return IS_FALSE;
}

