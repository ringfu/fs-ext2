#include "fileopt.h"
#include "funcsel.h"
 
__u16 count = 0;
FILE *cur_disk;

BOOL creatFile(char* filename,__u8 type, char* cur_path){
    // filename　新建文件名，type　标志普通文件还是目录文件, cur_path　当前目录
    __u16 cur_inode_number = findInodeByName(cur_path);
    if(cur_inode_number == 0){ // 文件创建失败
        printf("creat file failed!\n");
        return IS_FALSE;
    }else{
        // 目录inode结构体
        struct ext2_inode cur_inode;
        if((cur_disk = fopen(DISK,"r+"))){
            fseek(cur_disk, FIRST_INODE_BLOCK*EVERY_BLOCK+OUT_INODE_LENGTH*(cur_inode_number-1), SEEK_SET);
            if(!fread(&cur_inode, OUT_INODE_LENGTH, 1, cur_disk)){
                printf("read file failed!\n");
                return IS_FALSE;
            }
            fclose(cur_disk);
            __u16 new_inode_number = get_free_inode(); // 获取空闲节点号
            printf("free inode: %d\n",new_inode_number);
            if(new_inode_number && set_inode_bitmap(new_inode_number,BLOCK_INDEX_IN_USE)){
                // 修改当前文件inode
                // 设置新建文件的inode
                struct ext2_inode new_inode; // 创建空闲节点
                __u16 permit = 0x1111;
                new_inode.i_type = type; //通过参数确定文件类型
                new_inode.i_mode = permit;
                new_inode.i_uid = 0; // 文件拥有者：系统
                // time
                __u32 cur_time;
                time_t t = time(NULL);
                cur_time = time(&t);
                new_inode.i_atime = cur_time; 
                new_inode.i_ctime = cur_time; // 创建时间
                new_inode.i_mtime = cur_time; //修改时间
                new_inode.i_dtime = 0xFFFF; // 文件删除时间

                new_inode.i_gid = 0;
                new_inode.i_flags = 3; //　文件打开方式
                // 申请数据块，如果是目录文件，则需要申请数据块存放'.' '..' 两个目录；
                // 普通文件不申请数据块，因为现在是创建文件，还没有写入文件内容
                if(type == DIR_FILE){
                    // 如果是目录文件，需要写入两个目录'.' '..'
                    __u16 free_block = get_free_block(); // 如果是新建目录，则需要申请数据块，普通文件不申请
                    set_block_bitmap(free_block+1, BLOCK_INDEX_IN_USE);
                    new_inode.i_blocks = 1;
                    new_inode.i_block[0] = free_block; // 填写新申请的空闲数据块标号
                    new_inode.i_size = 2;
                    new_inode.i_links_count = 2; // '.' '..' 两个目录
                    
                    // 写入　'.' '..'两个目录
                    struct ext2_dir_entry_2 cur_dir, parent_dir;
                    cur_dir.inode = new_inode_number; //当前目录inode节点号
                    cur_dir.rec_len = 256; //目录项所占空间
                    cur_dir.file_type = DIR_FILE;
                    strcpy(cur_dir.name, ".");
                    cur_dir.name_len = 1;

                    parent_dir.inode = cur_inode_number; // 当前目录的父目录节点号
                    parent_dir.rec_len = 256;
                    parent_dir.file_type = DIR_FILE;
                    strcpy(parent_dir.name, "..");
                    parent_dir.name_len = 2;

                    //　打开文件然后写入目录结构体
                    if((cur_disk = fopen(DISK,"r+")) == NULL){
                        printf("open file failed!\n");
                        return IS_FALSE;
                    }
                    // 写入 '.' 目录
                    fseek(cur_disk,(FIRST_DATA_BLOCK+free_block)*EVERY_BLOCK, SEEK_SET);
                    fwrite(&cur_dir, DIR_DENTRY_LENGTH, 1, cur_disk);
                    // 写入 '..' 目录
                    fseek(cur_disk, DIR_DENTRY_LENGTH, SEEK_CUR);
                    fwrite(&parent_dir, DIR_DENTRY_LENGTH, 1, cur_disk);
                }else{
                    printf("oridinary file!\n");
                    new_inode.i_blocks = 0;
                    new_inode.i_size = 0;
                    new_inode.i_links_count = 0;
                }
                printf("free inode:%d\n",new_inode_number);
                fclose(cur_disk);

                // 写入新建目录inode
                if((cur_disk = fopen(DISK,"r+")) == NULL){
                    printf("file open failed!\n");
                    return IS_FALSE;
                }
                fseek(cur_disk, FIRST_DATA_BLOCK*EVERY_BLOCK+OUT_INODE_LENGTH*(new_inode_number-1),SEEK_SET);
                fwrite(&new_inode,OUT_INODE_LENGTH,1,cur_disk);
                fclose(cur_disk);

                //新建目录目录项
                struct ext2_dir_entry_2 new_dentry;
                new_dentry.file_type = type;
                strcpy(new_dentry.name, filename); // 目录项的name
                new_dentry.name_len = strlen(filename);
                new_dentry.rec_len = 256;
                new_dentry.inode = new_inode_number; // 设置文件名，目的是能根据文件名找到inode

                //　将新建目录项写入当前目录的数据块，要考虑是否重新分块
                if(cur_inode.i_links_count % 4){ // %4 的原因：一个inode占256B, 一个数据块1024B
                    // 当前数据块空间有剩余，不申请
                    // 当前数据块块号
                    __u16 cur_dentry_block = FIRST_DATA_BLOCK + cur_inode.i_block[cur_inode.i_blocks - 1];
                    // 当前目录中最后一个目录项的块偏移
                    __u16 cur_dentry_number = cur_inode.i_size % 4;

                    if((cur_disk = fopen(DISK,"r+")) == NULL){
                        printf("open file failed!\n");
                        return IS_FALSE;
                    }
                    // 写入新建的目录项
                    if(fseek(cur_disk,cur_dentry_block*EVERY_BLOCK+cur_dentry_number*DIR_DENTRY_LENGTH, SEEK_SET)){
                        fwrite(&new_dentry,DIR_DENTRY_LENGTH,1,cur_disk);
                        printf("write in new dentry!\n");
                        fclose(cur_disk);
                        cur_inode.i_size++; // 目录项个数
                    }else{
                        printf("write in new dentry failed!\n");
                        return IS_FALSE;
                    }
                }else{
                    // 当前数据块满，需要申请数据块
                    cur_inode.i_size++;
                    // 申请数据块
                    __u16 new_block_number = get_free_block();
                    set_block_bitmap(new_block_number,BLOCK_INDEX_IN_USE); //　更新数据块位图

                    cur_inode.i_block[cur_inode.i_blocks] = new_block_number;
                    cur_inode.i_blocks++; // 当前inode数据块块数+1
                    if(cur_disk = fopen(DISK,"r+")){
                        // 写入新建目录项到磁盘
                        fseek(cur_disk,new_block_number*EVERY_BLOCK,SEEK_SET);
                        fwrite(&new_dentry,DIR_DENTRY_LENGTH,1,cur_disk);
                        cur_inode.i_links_count++; // 当前目录中所有目录项个数
                        printf("write in new dentry to disk!\n");
                        fclose(cur_disk);
                    }else{
                        printf("open file failed!\n");
                        return IS_FALSE;
                    }
                    // 将修改后的当前inode写回磁盘
                    if((cur_disk = fopen(DISK,"r+"))){
                        fseek(cur_disk,FIRST_DATA_BLOCK*EVERY_BLOCK+(cur_inode_number-1)*OUT_INODE_LENGTH,SEEK_SET);
                        fwrite(&cur_inode,OUT_INODE_LENGTH,1,cur_disk);
                        printf("write back cur inode to disk!\n");
                        return IS_TRUE;
                    }else{
                        printf("write back cur inode failed!\n");
                        return IS_FALSE;
                    }
                    return IS_FALSE;
                }

            }
        }
    }
    return IS_FALSE;
}

BOOL deleteFile(char* filename, char* cur_path){
    __u16 cur_inode_number = findInodeByName(cur_path);
    if(cur_inode_number){
        struct ext2_inode cur_inode; // 当前目录对应的inode
        struct ext2_dir_entry_2 dir_dentry; // 欲删除文件的目录项
        struct ext2_inode dir_inode;
        if((cur_disk = fopen(DISK,"r+"))){
            fseek(cur_disk, FIRST_INODE_BLOCK*EVERY_BLOCK+OUT_INODE_LENGTH*(cur_inode_number - 1), SEEK_SET);
            fread(&cur_inode, OUT_INODE_LENGTH, 1, cur_disk); //

            // 找到欲删除文件的数据块
            int i;
            for(i=0; i<cur_inode.i_links_count; i++){
                fseek(cur_disk,(FIRST_DATA_BLOCK+cur_inode.i_block[i/4])*EVERY_BLOCK+DIR_DENTRY_LENGTH*(i%4), SEEK_SET);
                fread(&dir_dentry,DIR_DENTRY_LENGTH,1,cur_disk); // 读取欲删除文件的目录项
                if(strcmp(dir_dentry.name, filename) == 0){ // 找到欲删除文件目录项
                    fseek(cur_disk,(FIRST_INODE_BLOCK+dir_dentry.inode-1)*EVERY_BLOCK,SEEK_SET);
                    fread(&dir_inode,OUT_INODE_LENGTH,1,cur_disk); // 读取欲删除文件的inode
                    
                    // 设置欲删除文件对应inode位图位为可用，但实际上数据并没有置位，只是设置这些存储区可用
                    set_inode_bitmap(dir_dentry.inode,BLOCK_INDEX_NOT_USE);
                    
                    // 设置文件对应数据块位图为可用 
                    int j;
                    for(j=0; j < dir_inode.i_blocks; j++){ // 考虑到一个文件可能占用多个数据块
                        set_block_bitmap(dir_inode.i_block[j],BLOCK_INDEX_NOT_USE); 
                    }
                    
                    // 修改当前目录inode相关信息
                    cur_inode.i_links_count--;
                    cur_inode.i_blocks--;
                    cur_inode.i_block[i] = 0; // 删除指向欲删除文件的指针
                    // 写回当前目录inode到磁盘
                    fseek(cur_disk, FIRST_INODE_BLOCK*EVERY_BLOCK+OUT_INODE_LENGTH*(cur_inode_number - 1), SEEK_SET);
                    fwrite(&cur_inode,OUT_INODE_LENGTH,1,cur_disk); // 写回inode到磁盘
                    fclose(cur_disk);
                    printf("delete file!\n");
                    return IS_TRUE;
                }
            }
        }else{
            printf("open file failed!\n");
        }
    }else{ 
        printf("not find specific inode!\n");
        return IS_FALSE;
    }
    return IS_FALSE;
}

// 列出当前目录下所有文件
BOOL listFile(char* cur_path){
    __u16 cur_inode_number = findInodeByName(cur_path);
    if(cur_inode_number != 0){
        struct ext2_inode cur_inode;
        struct ext2_dir_entry_2 cur_dentry;
        if((cur_disk = fopen(DISK,"r+"))){
            fseek(cur_disk,FIRST_INODE_BLOCK*EVERY_BLOCK+(cur_inode_number-1)*OUT_INODE_LENGTH,SEEK_SET);
            fread(&cur_inode,OUT_INODE_LENGTH,1,cur_disk);

            // 读取当前文件inode指向的数据块，里面存放目录项
            int i;
            for(i=0; i<cur_inode.i_links_count; i++){
                fseek(cur_disk,(FIRST_INODE_BLOCK+cur_inode.i_block[i/4])*EVERY_BLOCK+(1%4)*DIR_DENTRY_LENGTH, SEEK_SET);
                fread(&cur_dentry,DIR_DENTRY_LENGTH,1,cur_disk);
                printf("%s ",cur_dentry.name); // 输出目录项文件名
            }
            fclose(cur_disk);
            return IS_TRUE;
        }else{
            printf("open file failed!\n");
            fclose(cur_disk);
            return IS_FALSE;
        }
    }else{
        printf("inode not find!\n");
    }
}


BOOL changePath(char* cur_path, char* aim_path){
    printf("change path******************\n");
    return IS_TRUE;
}

BOOL readFile(char* filename, char* cur_path){
    printf("read file******************\n");
    return IS_TRUE;
}

BOOL openFile(char* filename, char* cur_path){
    printf("open file******************\n");
    return IS_TRUE;
}

BOOL writeFile(char* filename, char* cur_path){
    printf("write file******************\n");
    return IS_TRUE;
}