//　文件操作功能实现
#include<system.h>
#include "ext2struc.h"

void ext2_mkdir(){

}

void ext2_ls(){

}

void ext2_rm(){

}

void ext2_open(){

}

void ext2_touch(){

}

void ext2_cat(){

}

void ext2_clear(){

}

void ext2_exit(){
    exit();
}
/*

待实现的功能：
1. mkdir, touch 调用 createFile, 通过　type 标志是文件还是目录
// 创建文件、目录
BOOL createFile(char* filename, __u8 type, char* current);

2. rm 调用 deleteFile 
// 删除文件
BOOL deleteFile(char* filename, char* current);

3. ls 调用　selectFile
// 列出此目录下的所有文件
BOOL selectFile(char* current);

4. open 调用打开文件
// 打开文件
BOOL openFile(char* filename, char* current);

5. echo 调用 modifyFile
// 写文件（修改文件）
BOOL modifyFile(char* filename, char* current);

6. cat 调用　readFile
// 读文件（将文件读出）
BOOL readFile(char* filename, char* current);

7. cd 调用 changeFile
// 修改当前工作目录
BOOL changeFile(char* current, char* filename);

8. clear 调用sys.clear

9. exit 调用 break　退出死循环
*/