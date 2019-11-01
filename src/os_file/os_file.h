#ifndef __OS_FILE_H__
#define __OS_FILE_H__


#ifdef __USE_FAT14__
#define os_SoftReset tpos_softReset
#ifndef __SOFT_SIMULATOR__
//#define ClrWdt  ClrTaskWdt
#else
#define ClrWdt()
#endif
#define switch_off_flash()
#define SIGNAL_FLASHSPI   SIGNAL_FLASH
#define OSMutexPend tpos_mutexPend
#define OSMutexFree tpos_mutexFree

#define VFM_MAX_PAGE_COUNT   512UL
#define SIZE_OF_CLUSTER    4096UL
#define FORMAT  0x02       //格式化标志
#define ATMEL_FLASH        0x1F
#define MACRONIX_FLASH     0xC2 

#define FAT14_PAGE_OFFSET   0

#define FLADDR_VFM_PAGE_OFFSET   130    //G55平台升级文件占130个块。
//MAX_FAT14_CLUSTER 必须小于16384，即0x4000, 因为FAT14中，0x3FFF,0x3FFE被用做特殊意义了。
//所以， MAX_FAT14_CLUSTER 最大可以为16380
//#define MAX_FAT14_CLUSTER   4096
#define MAX_FAT14_CLUSTER   2048
#define OS_LEN_FAT14        (MAX_FAT14_CLUSTER*2UL)

#define FLASH_CHIP_2        0x4000

//簇最大擦写循环轮次
#define MAX_CLUSTER_WRITE_LOOP  3

//定义根目录下文件的最大个数,这里考虑到测量点和其他,暂定为300个文件.
#define MAX_FDT_COUNT       300
#define OS_LEN_FDT          1800            //300*6

#if defined (__32MX695F512L_H)
#define FMADDR_TRANSCTRL     0                             //50字节,事务控制区,实际只能使用44字节，其中+44为版本号，+46为擦写次数
#define FMADDR_FAT14         50UL                            //FAT起始地址,	 ( file access table)
#define FMADDR_FDT           (FMADDR_FAT14 + OS_LEN_FAT14)  //FDT起始地址 (file directory table)
#define FMADDR_FAT_FLAG      (FMADDR_FDT + OS_LEN_FDT)                               //FAT文件系统标志
#define FMADDR_APP           (FMADDR_FAT_FLAG+5)      //应用程序可用的铁电起始地址
#else
#define FMADDR_FAT_FLAG      0                             //FAT文件系统标志
#define FMADDR_FAT14         5UL                            //FAT起始地址,
#define FMADDR_FDT           (FMADDR_FAT14 + OS_LEN_FAT14)  //FDT起始地址
#define FMADDR_TRANSCTRL     (FMADDR_FDT + OS_LEN_FDT)     //50字节,事务控制区,实际只能使用44字节，其中+44为版本号，+46为擦写次数
#define FMADDR_APP           (FMADDR_TRANSCTRL + 50)       //应用程序可用的铁电起始地址
#endif

//限制文件尺寸，规定文件最大占用的簇数,限制文件最大512K
#define  MAX_FILE_CLUSTERS   512

//可以同时打开的文件数量
#define  MAX_OPEN_FILE          5
#define  MAX_PRE_GET_CLUSTER   48

#define  MAX_HIS_FREE_CLUSTER  2             //保留历史空闲簇个数
#define  CLUSTER_COUNT_OF_BLOCK  16            //一个FLASH BLOCK包含的簇数

#define  INVALID_FILE_ID     0xFFFF          //无效文件
#define  DELETED_FILE_ID     0xFFFE          //被删除文件
#define  NONE_FILE_ID        0xFFFD          //无文件ID
//不存在的文件号
#define FILEID_NONE          0x7FFF


#define  INVALID_CLUSTER     0xFFFF          //无效的簇号
#define  FILE_END_CLUSTER    0x3FFF          //文件结束簇
#define  FILE_FREE_CLUSTER   0x3FFE          //空闲簇

#define  INVLAID_CLUSTER_SEQ  0xFFFF         //无效的文件簇序号

#define  SUCCESS                       0x00
#define  FILE_ERROR_FILE_ID            0x01
#define  FILE_NO_SPACE          2
#define  FILE_ERROR_CLUSTER_SEQ        3
#define  FILE_ERROR_FAT                4
#define  FILE_MAX_OPEN_REACHED         5
#define  FILE_IS_OPEN                  6
#define  FILE_NULL_BUFFER              7
#define  FILE_IS_BUSY                  8
#define  FILE_EOF                      9


//文件操作
#define  FILE_OP_READ             0x55
#define  FILE_OP_WRITE            0xAA


//文件操作事务类别定义
#define   FILE_TRANSCTRL_UPDATE_FATFDT   1
#define   FILE_TRANSCTRL_FMWRITE         2
#define   FILE_TRANSCTRL_DELETE_FILE     3
#define   FILE_TRANSCTRL_TRIM_FILE       4


//定义FAT表的簇
typedef struct
{
   INT16U    write_count:2;           //簇使用次数
   INT16U    cluster:14;              //簇号
}FAT_ENTRY;


//定义FDT的项目
typedef struct
{
   INT16U   file_id;           //文件ID, 0表示文件被删除
   INT16U   first_cluster;     //首簇
   INT16U   stamp_id;          //簇ID,文件创建时赋值为0,以后每使用一个簇加1
}FDT_ENTRY;


//每个簇的头部都有一个簇使用标志块
typedef struct{
   INT16U file_id;       //文件ID,表示本簇隶属的文件,这里表示有些冗余,目的是可以通过簇重建FAT及FDT
   INT16U stamp_id;      //簇ID,根据文件的簇ID递增形成.
   INT16U cluster_seq;   //本簇是文件的第几个簇,簇序号
   INT16U FCS;           //校验位
}CLUSTER_HEAD;


typedef struct{
   INT16U  max_fat14_clusters;       //文件系统管理的簇数量
   INT16U  free_cluster_count;      //文件系统中至少剩余的空闲簇数量.
   INT16U  per_cluster_size;        //单个簇尺寸，与存储区的页面大小相等。
   INT8U   open_file_count;		  //最大的打开文件的数量
   INT8U   SIGNAL;
   INT16U  per_cluster_data_size;   //单个簇的数据尺寸，等于簇尺寸减去簇头信息。
   INT8U   min_cluster_write_count;  //当前最小簇擦写轮次数
   INT8U   free_clusters_count;     //当前剩余的预提取空闲簇数量,如果为零，则重新从FAT表中查找空闲簇
   INT16U  free_clusters[MAX_PRE_GET_CLUSTER];       //空闲的簇号
   INT16U  in_using_free_clusters[MAX_OPEN_FILE];
   INT16U  opened_file_id[MAX_OPEN_FILE];
   INT16U  his_free_cluster[MAX_HIS_FREE_CLUSTER];       //历史获取到的空闲簇列表，用于控制每次新申请到的空闲簇不在一个FLASH BLCOK中
}FAT14_VAR;


//结构元素的组织要考虑四字节对齐
typedef struct{
   INT32U      offset;          //文件指针
   FDT_ENTRY   fdt_entry;       //文件FAT入口(应该是FDT）
   INT16U      cluster_offset;  //簇内偏移量
   INT16U      prev_cluster;    //上一个簇号
   INT16U      cur_cluster;     //当前簇号
   INT16U      next_cluster;    //下一个簇号
   INT16U      cluster_seq;     //当前簇序号
   INT16U      free_cluster;    //需要释放的簇号
   INT8U       open_file_seq;   //打开文件的序号 ，在打开的文件序列中的序号
   INT8U       operate;        //当前操作
}FAT14_FILE;

//FLASH虚拟铁电
void    os_vfm_erase_page(INT16U page);
void    os_vfm_write_array(INT16U page,INT16U page_offset,INT8U *buf,INT16U len);
INT16U  os_vfm_read_array(INT16U page,INT16U page_offset,unsigned char *buf,INT16U len);
BOOLEAN    os_vfm_write_bits(INT16U page,INT16U page_offset,INT8U bitdata,INT8U bitpos);
INT8U   os_vfm_read_byte(INT16U page,INT16U page_offset);
void    vfm_read_array(INT32U addr,INT8U *data,INT16U len);//读数据
void    vfm_write_array(INT32U addr,INT8U *data,INT16U len);//写数据
void    vfm_init(INT8U isformat,void (*calluserfun)(void));//flash虚拟铁电系统初始化工作
void    vfmflash_temp_page_erase(INT16U temp_page_idx);
void    vfmfalsh_temp_write_array(INT16U temp_page_idx,INT16U page_offset,INT8U *data,INT16U len);
void    vfmfalsh_temp_read_array(INT16U temp_page_idx,INT16U page_offset,INT8U *data,INT16U len);
INT16U  vfm_read_write_count(void);
void os_vfm_erase_block(INT16U block);
void check_sysf4_status(void);
void  os_vfm_clr_2bits(INT16U page,INT16U page_offset,INT8U bitpos);
void  os_spansion_erase_page(INT16U page,INT16U count);
BOOLEAN  check_flash_data(INT16U page,INT16U page_offset,INT8U *buffer,INT16U len);
INT16U get_flash_page_count(void);
void  read_fmArray(INT32U addr,void *buf,INT16U len);
unsigned char  read_fmByte(INT32U addr);
void  write_fmArray(INT32U addr,void *buf,INT16U len);
void set_fmArray(INT32U addr,unsigned char val,INT16U len);
BOOLEAN  os_flash_erase_page(INT16U page);
void  write_fmByte(INT32U addr,unsigned char ch);
INT16U  ftrim_flashArray(INT16U page,INT16U page_max_size,INT16U newPage,CLUSTER_HEAD *cluster_head);
BOOLEAN erase_write_pagehead(INT32U addr,unsigned char *buf,INT16U len);
INT16U  fupdate_flashArray(INT16U page,INT16U page_offset,INT8U *buf,INT16U len);
INT16U  fwrite_flashArray(INT16U page,INT16U page_offset,INT16U newPage,unsigned char *buf,INT16U len,CLUSTER_HEAD *cluster_head);
INT16U  fread_flashArray(INT16U page,INT16U page_offset,unsigned char *buf,INT16U len);
INT8U  fread_array(INT16U file_id,INT32U addr,INT8U *data,INT16U len);
INT8U  fwrite_array(INT16U file_id,INT32U addr,INT8U *data,INT16U len);
INT8U  file_delete(INT16U file_id);
BOOLEAN  fat14_init(void);
BOOLEAN  file_exist(INT16U file_id);

#endif
#endif