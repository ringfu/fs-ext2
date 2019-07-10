#ifndef __DENTRY_H
#define __DENTRY_H
//#include "ext2fs.h"
#include "ext2struc.h"
// 通过文件名/目录名找到对应inode
__u16 findInodeByName(char name[]);

#endif
