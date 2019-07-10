#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
//#include "ext2struc.h" //定义了程序中用到的数据结构
//#include "ext2init.h"
#include "ext2init.h" 
#include "fileopt.h" 
#include "funcsel.h" 
int main(){
    /*
    *    程序结构： 磁盘检查，文件系统初始化，提示符输出，命令输入到执行相应功能
    */
    char* cur_path = "/";
    // init ext2 filesystem
    if(disk_alloc()){
        //printf("disk alloc!\n");
        ext2fs_init();
        funcsel(cur_path);
    }else{
        printf("disk err!\n");
    }
    return 0;
}
