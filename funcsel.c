//　文件系统功能选择：cd, ls, mkdir, cat, open, exit, touch, rm, echo 

#include "funcsel.h"
#include "fileopt.h"

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
            creatFile(list, DIR_FILE, cur_path);  // 创建目录
            continue;
        }
        // if (0 == strcmp(command, TOUCH))
        // if(0 == strcmp(command,TOUCH))
        // {
        //     scanf("%s",list);
        //     creatFile(list,ORDINARY_FILE,cur_path);
        //     continue;
        // }
        //     scanf("%s", list);
        //     creatFile(list, ORDINARY_FILE,cur_path) // 创建空白文件
        //     continue;
        // }
        if (0 == strcmp(command, LS))
        {
            listFile(cur_path);
            continue;
        }
        if (0 == strcmp(command, CD))
        {
            scanf("%s", list);
            changePath(cur_path, list);
            continue;
        }
        if (0 == strcmp(command, RM))
        {
            scanf("%s", list);
            deleteFile(list, cur_path);
            continue;
        }
        if (0 == strcmp(command, CAT))
        {
            scanf("%s", list);
            readFile(list, cur_path);
            continue;
        }
        // if (0 == strcmp(command, ECHO))
        // {
        //     scanf("%s", list);
        //     ext2_echo(list, cur_path);
        //     continue;
        // }
        if (0 == strcmp(command, OPEN))
        {
            scanf("%s", list);
            openFile(list, cur_path);
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