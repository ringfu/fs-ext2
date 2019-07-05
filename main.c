#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include "ext2struch" //定义了程序中用到的数据结构
#include "ext2init.h"

int main(){
	// define structure
    //...

    // init ext2 filesystem
    if(disk_alloc()){
        ext2fs_init();
    
    }

    return 0;
}　

/*
程序结构： 磁盘检查，文件系统初始化，提示符输出，命令输入到执行相应功能
*/