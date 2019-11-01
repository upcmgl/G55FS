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
#define FORMAT  0x02       //��ʽ����־
#define ATMEL_FLASH        0x1F
#define MACRONIX_FLASH     0xC2 

#define FAT14_PAGE_OFFSET   0

#define FLADDR_VFM_PAGE_OFFSET   130    //G55ƽ̨�����ļ�ռ130���顣
//MAX_FAT14_CLUSTER ����С��16384����0x4000, ��ΪFAT14�У�0x3FFF,0x3FFE���������������ˡ�
//���ԣ� MAX_FAT14_CLUSTER ������Ϊ16380
//#define MAX_FAT14_CLUSTER   4096
#define MAX_FAT14_CLUSTER   2048
#define OS_LEN_FAT14        (MAX_FAT14_CLUSTER*2UL)

#define FLASH_CHIP_2        0x4000

//������дѭ���ִ�
#define MAX_CLUSTER_WRITE_LOOP  3

//�����Ŀ¼���ļ���������,���￼�ǵ������������,�ݶ�Ϊ300���ļ�.
#define MAX_FDT_COUNT       300
#define OS_LEN_FDT          1800            //300*6

#if defined (__32MX695F512L_H)
#define FMADDR_TRANSCTRL     0                             //50�ֽ�,���������,ʵ��ֻ��ʹ��44�ֽڣ�����+44Ϊ�汾�ţ�+46Ϊ��д����
#define FMADDR_FAT14         50UL                            //FAT��ʼ��ַ,	 ( file access table)
#define FMADDR_FDT           (FMADDR_FAT14 + OS_LEN_FAT14)  //FDT��ʼ��ַ (file directory table)
#define FMADDR_FAT_FLAG      (FMADDR_FDT + OS_LEN_FDT)                               //FAT�ļ�ϵͳ��־
#define FMADDR_APP           (FMADDR_FAT_FLAG+5)      //Ӧ�ó�����õ�������ʼ��ַ
#else
#define FMADDR_FAT_FLAG      0                             //FAT�ļ�ϵͳ��־
#define FMADDR_FAT14         5UL                            //FAT��ʼ��ַ,
#define FMADDR_FDT           (FMADDR_FAT14 + OS_LEN_FAT14)  //FDT��ʼ��ַ
#define FMADDR_TRANSCTRL     (FMADDR_FDT + OS_LEN_FDT)     //50�ֽ�,���������,ʵ��ֻ��ʹ��44�ֽڣ�����+44Ϊ�汾�ţ�+46Ϊ��д����
#define FMADDR_APP           (FMADDR_TRANSCTRL + 50)       //Ӧ�ó�����õ�������ʼ��ַ
#endif

//�����ļ��ߴ磬�涨�ļ����ռ�õĴ���,�����ļ����512K
#define  MAX_FILE_CLUSTERS   512

//����ͬʱ�򿪵��ļ�����
#define  MAX_OPEN_FILE          5
#define  MAX_PRE_GET_CLUSTER   48

#define  MAX_HIS_FREE_CLUSTER  2             //������ʷ���дظ���
#define  CLUSTER_COUNT_OF_BLOCK  16            //һ��FLASH BLOCK�����Ĵ���

#define  INVALID_FILE_ID     0xFFFF          //��Ч�ļ�
#define  DELETED_FILE_ID     0xFFFE          //��ɾ���ļ�
#define  NONE_FILE_ID        0xFFFD          //���ļ�ID
//�����ڵ��ļ���
#define FILEID_NONE          0x7FFF


#define  INVALID_CLUSTER     0xFFFF          //��Ч�Ĵغ�
#define  FILE_END_CLUSTER    0x3FFF          //�ļ�������
#define  FILE_FREE_CLUSTER   0x3FFE          //���д�

#define  INVLAID_CLUSTER_SEQ  0xFFFF         //��Ч���ļ������

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


//�ļ�����
#define  FILE_OP_READ             0x55
#define  FILE_OP_WRITE            0xAA


//�ļ��������������
#define   FILE_TRANSCTRL_UPDATE_FATFDT   1
#define   FILE_TRANSCTRL_FMWRITE         2
#define   FILE_TRANSCTRL_DELETE_FILE     3
#define   FILE_TRANSCTRL_TRIM_FILE       4


//����FAT��Ĵ�
typedef struct
{
   INT16U    write_count:2;           //��ʹ�ô���
   INT16U    cluster:14;              //�غ�
}FAT_ENTRY;


//����FDT����Ŀ
typedef struct
{
   INT16U   file_id;           //�ļ�ID, 0��ʾ�ļ���ɾ��
   INT16U   first_cluster;     //�״�
   INT16U   stamp_id;          //��ID,�ļ�����ʱ��ֵΪ0,�Ժ�ÿʹ��һ���ؼ�1
}FDT_ENTRY;


//ÿ���ص�ͷ������һ����ʹ�ñ�־��
typedef struct{
   INT16U file_id;       //�ļ�ID,��ʾ�����������ļ�,�����ʾ��Щ����,Ŀ���ǿ���ͨ�����ؽ�FAT��FDT
   INT16U stamp_id;      //��ID,�����ļ��Ĵ�ID�����γ�.
   INT16U cluster_seq;   //�������ļ��ĵڼ�����,�����
   INT16U FCS;           //У��λ
}CLUSTER_HEAD;


typedef struct{
   INT16U  max_fat14_clusters;       //�ļ�ϵͳ����Ĵ�����
   INT16U  free_cluster_count;      //�ļ�ϵͳ������ʣ��Ŀ��д�����.
   INT16U  per_cluster_size;        //�����سߴ磬��洢����ҳ���С��ȡ�
   INT8U   open_file_count;		  //���Ĵ��ļ�������
   INT8U   SIGNAL;
   INT16U  per_cluster_data_size;   //�����ص����ݳߴ磬���ڴسߴ��ȥ��ͷ��Ϣ��
   INT8U   min_cluster_write_count;  //��ǰ��С�ز�д�ִ���
   INT8U   free_clusters_count;     //��ǰʣ���Ԥ��ȡ���д�����,���Ϊ�㣬�����´�FAT���в��ҿ��д�
   INT16U  free_clusters[MAX_PRE_GET_CLUSTER];       //���еĴغ�
   INT16U  in_using_free_clusters[MAX_OPEN_FILE];
   INT16U  opened_file_id[MAX_OPEN_FILE];
   INT16U  his_free_cluster[MAX_HIS_FREE_CLUSTER];       //��ʷ��ȡ���Ŀ��д��б����ڿ���ÿ�������뵽�Ŀ��дز���һ��FLASH BLCOK��
}FAT14_VAR;


//�ṹԪ�ص���֯Ҫ�������ֽڶ���
typedef struct{
   INT32U      offset;          //�ļ�ָ��
   FDT_ENTRY   fdt_entry;       //�ļ�FAT���(Ӧ����FDT��
   INT16U      cluster_offset;  //����ƫ����
   INT16U      prev_cluster;    //��һ���غ�
   INT16U      cur_cluster;     //��ǰ�غ�
   INT16U      next_cluster;    //��һ���غ�
   INT16U      cluster_seq;     //��ǰ�����
   INT16U      free_cluster;    //��Ҫ�ͷŵĴغ�
   INT8U       open_file_seq;   //���ļ������ ���ڴ򿪵��ļ������е����
   INT8U       operate;        //��ǰ����
}FAT14_FILE;

//FLASH��������
void    os_vfm_erase_page(INT16U page);
void    os_vfm_write_array(INT16U page,INT16U page_offset,INT8U *buf,INT16U len);
INT16U  os_vfm_read_array(INT16U page,INT16U page_offset,unsigned char *buf,INT16U len);
BOOLEAN    os_vfm_write_bits(INT16U page,INT16U page_offset,INT8U bitdata,INT8U bitpos);
INT8U   os_vfm_read_byte(INT16U page,INT16U page_offset);
void    vfm_read_array(INT32U addr,INT8U *data,INT16U len);//������
void    vfm_write_array(INT32U addr,INT8U *data,INT16U len);//д����
void    vfm_init(INT8U isformat,void (*calluserfun)(void));//flash��������ϵͳ��ʼ������
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