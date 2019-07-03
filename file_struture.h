#include<linux/ext2fs/ext2_fs.h>

//　超级块　super block
struct ext2_super_block
 {	__u32	s_inodes_count;	/* 文件系统中索引节点总数 */
	__u32	s_blocks_count;	/*文件系统中总块数 */
	__u32	s_r_blocks_count;	  /* 为超级用户保留的块数 */
	__u32	s_free_blocks_count;	/*文件系统中空闲块总数 */
	__u32	s_free_inodes_count;	/*文件系统中空闲索引节点总数*/
	__u32	s_first_data_block;	/* 文件系统中第一个数据块 */
	__u32	s_log_block_size;	 /* 用于计算逻辑块大小 */
	__s32	s_log_frag_size;	 /* 用于计算片大小 */
	__u32	s_blocks_per_group;	/* 每组中块数 */
	__u32	s_frags_per_group;  /* 每组中片数 */
	__u32	s_inodes_per_group;	/* 每组中索引节点数 */
	__u32	s_mtime;   /*最后一次安装操作的时间 */
	__u32	s_wtime;  /*最后一次对该超级块进行写操作的时间 */
	__u16	s_mnt_count;	/* 安装计数 */
	__s16	s_max_mnt_count;	 /* 最大可安装计数 */
	__u16	s_magic;  /* 用于确定文件系统版本的标志 */
	__u16	s_state;  /* 文件系统的状态*/
	__u16	s_errors;  /* 当检测到有错误时如何处理 */
	__u16	s_minor_rev_level; 	/* 次版本号 */
	__u32	s_lastcheck;	/* 最后一次检测文件系统状态的时间 */
	__u32	s_checkinterval; /* 两次对文件系统状态进行检测的间隔时间 */
	__u32	s_rev_level;	/* 版本号 */
	__u16	s_def_resuid;	 /* 保留块的默认用户标识号 */
	__u16	s_def_resgid;  /* 保留块的默认用户组标识号*/	
    __u32   s_first_ino;  /* 第一个非保留的索引节点 */
    __u16   s_inode_size;  /* 索引节点的大小 */
  	__u16   s_block_group_nr;  /* 该超级块的块组号 */
  	__u32   s_feature_compat;  /* 兼容特点的位图*/
 	 __u32   s_feature_incompat;  /* 非兼容特点的位图 */
  	__u32   s_feature_ro_compat;  /* 只读兼容特点的位图*/
  	__u8    s_uuid[16];  /* 128位的文件系统标识号*/
 	 char    s_volume_name[16];  /* 卷名 */
 	 char    s_last_mounted[64];  /* 最后一个安装点的路径名 */
  	__u32   s_algorithm_usage_bitmap;  /* 用于压缩*/
  	__u8    s_prealloc_blocks;  /* 预分配的块数*/
  	__u8    s_prealloc_dir_blocks;  /* 给目录预分配的块数 */
 	 __u16   s_padding1;  /* 填充 */
 	 __u32   s_reserved[204];  /* 用null填充块的末尾 */
};

// // 块组描述表　group descriptor table
// struct ext2_group_desc
// {
// 	__u32	bg_block_bitmap;	 /* 组中块位图所在的块号 */
// 	__u32	bg_inode_bitmap;  /* 组中索引节点位图所在块的块号 */
// 	__u32	bg_inode_table;  /*组中索引节点表的首块号 */
// 	__u16	bg_free_blocks_count;  /* 组中空闲块数 */
// 	__u16	bg_free_inodes_count;  /* 组中空闲索引节点数 */
// 	__u16	bg_used_dirs_count;	/*组中分配给目录的节点数 */
// 	__u16	bg_pad;  /*填充，对齐到字*/
// 	__u32   bg_reserved［3］;   /*用null填充12个字节*/
// };

//　块位图　block bitmap
struct block_bitmap
{
        bool bmap[1024] = {0};   
};

// inode位图　inode_bitmap
struct inode_bitmap
{
    bool imap[1024] = {0};
};

// inode 节点表
struct ext2_inode {	
    __u16	i_mode; /* 文件类型和访问权限 */
	__u16	i_uid;	/* 文件拥有者标识号*/
	__u32	i_size; /* 以字节计的文件大小 */
	__u32	i_atime; /* 文件的最后一次访问时间 */
	__u32	i_ctime; /* 该节点最后被修改时间 */
	__u32	i_mtime; /* 文件内容的最后修改时间 */
	__u32	i_dtime; /* 文件删除时间 */
	__u16	i_gid;	/* 文件的用户组标志符 */
	__u16	i_links_count; /* 文件的硬链接计数 */
	__u32	i_blocks; /* 文件所占块数（每块以512字节计）*/
	__u32	i_flags; /* 打开文件的方式 */
   union   /*特定操作系统的信息*/
   __u32	i_block[Ext2_N_BLOCKS];  /* 指向数据块的指针数组 */
	__u32	i_version;	  /* 文件的版本号（用于 NFS） */
	__u32	i_file_acl; /*文件访问控制表（已不再使用） */
	__u32	i_dir_acl;	 /*目录 访问控制表（已不再使用）*/
	 __u8	l_i_frag;	 /* 每块中的片数 */
	 __u32   i_faddr;  /* 片的地址 */
};

// data block 数据块
struct ext2_dir_entry_2 {
    __le32 inode; // 文件入口的inode号，0表示该项未使用
    __le16 rec_len; // 目录项长度
    __u8 name_len; // 文件名包含的字符数
    __u8 file_type; // 文件类型
    char name[255]; // 文件名
};  



