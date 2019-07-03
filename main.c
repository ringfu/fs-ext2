#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include "file_struture.h" //定义了程序中用到的数据结构
#include "creat.h"  //文件创建
#include "access.h" //访问控制
#include "ballfre.h" //数据块的分配与回收
#include "close.h"  //关闭文件
#include "delete.h" //删除文件
#include "dir.h" //目录创建、删除，改变当前目录等
#include "format.h" //初始化数据块
#include "halt.h" //退出文件系统
#include "iallfre.h" //索引节点的分配与释放
#include "install.h" //安装
#include "log.h" //登录与注销操作
#include "name.h" //文件搜索
#include "open.h" //打开文件
#include "rdwt.h" //读写文件
#include "igetput.h" //获取或释放i节点
struct hinode hinode[NHINO];
struct dir dir;
struct file sys_ofile[SYSOPENFILE];
struct filsys filsys;
struct pwd pwd[PWDNUM];
struct user user[USERNUM];
FILE *fd;
struct inode *cur_path_inode;
int user_id;
/*kkkkk*/
unsigned short usr_id;
char usr_p[12];
char sel;
char temp_dir[12];
main( )
{
	unsigned short ab_fd1,ab_fd2,ab_fd3,ab_fd4,i,j;
	char *buf;
       int done=1;
	printf("\nDo you want to format the disk(y or n)?\n");
	if(getchar( )=='y')
 { printf("\nFormat will erase all context on the disk \n");
	printf("Formating...\n");	
	format( ); //初始化
	        printf("\nNow will install the fillsystem,please wait...\n");
	install( ); //安装文件系统
	        printf("\n----Login----\nPlease input your userid:");
	        scanf("%u",&usr_id);
	        printf("\nPlease input your password:");
	        scanf("%s",&usr_p);
              /*  printf("\nsuccess\n");*/
	  if(!login(usr_id,usr_p)) //判断用户登录是否正确
	     return;
   while(done)
  {
	        printf("\n Please Select Your Operating\n");
	        printf(" -1----ls\n -2----mkdir\n -3----change dir\n -4----create file\n -0----Logout\n");/* 注意*/
	sel=getchar( );
        sel=getchar( );
	switch(sel)
	{
	 case '1':                                       /*显示目录内容*/
	     _dir( );
	     break;
	 case '2':                                       /*创建目录*/
	     printf("please input dir name:");
	     scanf("%s",temp_dir);
	     mkdir(temp_dir);
	     break;
	 case '3':                                    /*改变当前目录*/
	     printf("please input dir name:");
	     scanf("%s",temp_dir);
	     chdir(temp_dir);
	     break;
	 case '4':                                    /*创建文件*/
	     printf("please input file name:");
	     scanf("%s",temp_dir);
	     ab_fd1=creat(2118,temp_dir,01777); //在这里设置创建文件的权限
	     buf=(char *)malloc(BLOCKSIZ*6+5);
	     write(ab_fd1,buf,BLOCKSIZ*6+5);
	     close(0,ab_fd1);
	     free(buf);
             break;
	 case '0':                               /*退出文件系统*/
	     logout(usr_id);
	     halt( );
	     done = 0;
         default:
             printf("error!\nNo such command,please try again.\nOr you can ask your teacher for help.\n");
             break;
	}
     }
  }
  else
  printf("User canseled\nGood Bye\n");
}