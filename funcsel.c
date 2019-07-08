//　文件系统功能选择：cd, ls, mkdir, cat, open, exit, touch, rm, echo 

#include "print.h"

void funcsel(char* cur_path)
{
    for (; ;)
    {
        printf("[root@root-PC]:%s $", cur_path);
        char command[10]; // 命令
        char list[10]; // 命令参数
        scanf("%s", command);
        if (0 == strcmp(command, MKDIR))
        {   
            scanf("%s", list);
            ext2_mkdir(list, DIR_FILE, current);  // 创建目录
            continue;
        }
        if (0 == strcmp(command, TOUCH))
        {
            scanf("%s", list);
            ext2_touch(list, ORDINARY_FILE, current); // 创建空白文件
            continue;
        }
        if (0 == strcmp(command, LS))
        {
            ext2_ls(current);
            continue;
        }
        if (0 == strcmp(command, CD))
        {
            scanf("%s", list);
            ext2_cd(current, list);
            continue;
        }
        if (0 == strcmp(command, RM))
        {
            scanf("%s", list);
            ext2_rm(list, current);
            continue;
        }
        if (0 == strcmp(command, CAT))
        {
            scanf("%s", list);
            ext2_cat(list, current);
            continue;
        }
        if (0 == strcmp(command, ECHO))
        {
            scanf("%s", list);
            ext2_echo(list, current);
            continue;
        }
        if (0 == strcmp(command, OPEN))
        {
            scanf("%s", list);
            ext2_open(list, current);
            continue;
        }
        if (0 == strcmp(command, EXIT))
        {
            printf("退出系统！\n");
            break;
        } 
        if (0 == strcmp(command, CLEAR))
        {
            clean();
            continue;
        }
        printf("输入参数错误！\n");
    }
}

void clean()
{
    system("clear");
}