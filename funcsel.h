#ifndef __FUNCSEL_H__
#define __FUNCSEL_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fileopt.h"

// 常用的命令
#define MKDIR   "mkdir"
#define TOUCH   "touch"
#define LS      "ls"
#define CD      "cd"
#define RM      "rm"
#define CAT     "cat"
#define ECHO    "echo"
#define OPEN    "open"
#define EXIT    "exit"
#define CLEAR   "clear"

// 展示输出, current表示当前目录
void funcsel(char* current);

// 清屏函数
void clean();

#endif