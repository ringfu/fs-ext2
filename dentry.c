#include "dentry.h"
#include"datatype.h"
#include "datatype.h"
#include "ext2init.h"
#include "ext2struc.h"
char delims[] = "/"; //　分隔符
FILE *file_disk;
// 根据目录找到文件所在节点
__u16 findInodeByName(char filename[]){
    
    __u16 final_inode;
    if(0 == strcmp(filename, "/")){
        return 2;
    }
    if((file_disk = fopen(DISK,"r+")) != NULL){
        //　寻找inode，　从１号节点开始找，0号节点不用
        fseek(file_disk,FIRST_INODE_BLOCK*EVERY_BLOCK+OUT_INODE_LENGTH,SEEK_SET);
        struct ext2_inode root_disk;
        struct ext2_dir_entry_2 dir_entry;
        __u8 flag = 0;
        if(fread(&root_disk, OUT_INODE_LENGTH,1,file_disk)){ // 读取了第一个inode
            // 分割路径名，每次返回第一个子串
            char* path = strtok(filename,delims);
            while (path != NULL)
            {
                int i;
                for(i=0; i<root_disk.i_links_count; i++){ //i_link_count　表示该目录下还有多少子目录
                    if(0 == fseek(file_disk,(FIRST_DATA_BLOCK+root_disk.i_block[i/4])*EVERY_BLOCK + DIR_DENTRY_LENGTH*(i%4),SEEK_SET)){
                        // i/4 的原因：一个目录项占256B, 一个数据块占1KB, 通过 i/4 和 i%4　就可以找到对应目录项在数据块中的位置
                        printf("find dir!\n"); // i_block[]存放的是inode指向数据块的指针,下标表示在那个数据块
                    }else{
                        printf("failed find dir\n");
                    }
                    // 读取目录项
                    fread(&dir_entry, DIR_DENTRY_LENGTH,1,file_disk);
                    if( 0 == strcmp(path, dir_entry.name)){
                        flag = 1;
                        printf("dir inode: %d\n", dir_entry.inode);
                        break; // 只有当欲寻址路径名和当前指向的路径名相匹配时才退出
                    }else{
                        continue; // 如果当前指向的路径名和欲寻址的路径名不匹配，指针移向下一个目录项
                    }
                    // 根据找到的inode重定位数据块
                    if(0 == fseek(file_disk,FIRST_INODE_BLOCK*EVERY_BLOCK+OUT_INODE_LENGTH*(dir_entry.inode - 1),SEEK_SET)){
                        printf("relocate data block succeed!\n");
                    }else{
                        printf("relocate data block failed!\n");
                    }
                    // 读取inode对应的数据块，解析数据结构
                    fread(&root_disk,OUT_INODE_LENGTH,1,file_disk);
                    path = strtok(NULL,delims);
                    if(flag && !path){ // flag=1表示找到对应inode，!path表示给定路径名搜索结束
                        fclose(file_disk); // 防止出现欲寻找 /root/usr/usr/file1，但是存在 /root/usr/file1这个文件
                        printf("file inode finded!\n");
                        return dir_entry.inode;
                    }else{
                        flag = 0;
                    }
                }
            }
            fclose(file_disk);
            return IS_FALSE;
        }
        return IS_FALSE;
    }
}