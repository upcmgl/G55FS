/*
   适用于2片8M NADN FLASH的简单文件系统

   1）一个簇大小为1056或4096字节，恰好是一个页面,对于AT45DB642D,页面是1056，MX25是4096字节。
   2）最大可用簇数为8192*2=16384，所以用14位表示一个簇,如果页面是4096，则簇数降低为8192个，另8192簇信息地址可做他用。
   3）FAT使用2字节表示簇的使用信息，用2位表示本簇擦写次数，用来做磨损平衡。
   4）文件名使用ID表示
   5) 文件不支持共享，即一个文件不能被同时打开。
   6) 一个簇在写时，根据数据内容判断是否需要写，还是擦写。如果是可以写，则不用更换簇。
   7)文件系统初始化时确定簇大小，即页面大小。

   事务控制区： 先填写事务类别，事务参数，最后填写事务标志,完成后,更新事务标志
       事务标志  2字节  55 AA  事务启动   66 99  事务完成  其他：无效
       事务类别  1字节
       事务参数：N字节  47字节

*/


// 需要一个事务性写铁电的函数，在单一的关键数据更新铁电操作中调用？？？？
// safe_write_fmArray();

/*
#define __TEST_FAT14__
#ifdef __TEST_FAT14__
#include "stdio.h"
void show_info(char * info);
char buffer[1024];
#endif

*/
#include "../main_include.h"

#define FAT_MAJOR_VER  0xA5
#define FAT_MINOR_VER  0x00


static FAT14_VAR   OS_FAT14;

void  check_file_transctrl(void);
void fat14_format(BOOLEAN  quick_format);
void  fat14_prefetch_free_clusters(void);
void detect_min_cluster_write_count(void);
BOOLEAN file_update_cluster_head(FAT14_FILE *file);
void safe_write_fmArray(INT32U addr,INT8U *data,INT16U len);
void  FAT14_cluster_check(void);
void  FAT14_file_check(void);

BOOLEAN fat14_checkflag(void);
BOOLEAN file_update_fat14(FAT14_FILE *file);
INT8U  file_move_to_cluster_seq(FAT14_FILE *file,INT16U cluster_seq);
INT8U  FAT14_Integrity_Check(void);
INT16U  get_a_free_cluster(INT8U open_file_seq);
INT16U  file_next_cluster(INT16U cur_cluster);
INT16U  file_prev_cluster(INT16U first_cluster,INT16U cur_cluster);
INT16U  cluster_to_page(INT16U cluster);
INT16U  system_free_cluster_count(void);
void fat14_var_check(void);
INT8U  file_delete(INT16U file_id);
INT8U  file_trim(INT16U file_id,INT32U max_size);
INT8U file_seek(FAT14_FILE *file,INT32U offset);

/*+++
  功能：初始化FAT14文件系统
  参数：
        无
  返回：
        TRUE  系统可用
        FALSE 系统不可用
  描述：
        1) 只能在操作系统启动时调用,一定不能在每个任务中都调用!
        2) 如果进行了文件系统格式化，则返回值中有格式化标志 

---*/
BOOLEAN  fat14_init(void)
{
    INT16U idx;
    INT8U  result;

    //文件系统操作信号量
    //OS_FAT14.SIGNAL = 0;

    result = FALSE;

    //确定设备中文件系统管理的簇数和簇页面尺寸。
    //OS_FAT14.max_fat14_clusters = gFlash.chip1.page_max + gFlash.chip2.page_max - FAT14_PAGE_OFFSET;
	OS_FAT14.max_fat14_clusters = get_flash_page_count() - 512 - FLADDR_VFM_PAGE_OFFSET;	     //2K 个页   文件系统使用的页数。
    OS_FAT14.per_cluster_size = SIZE_OF_CLUSTER;
    OS_FAT14.per_cluster_data_size =  OS_FAT14.per_cluster_size - sizeof(CLUSTER_HEAD);

    //2010-09-20 FAT14系统使用的簇数量一定不能超过16384,从第二片FLASH上空闲4个PAGE。
    if(OS_FAT14.max_fat14_clusters >= MAX_FAT14_CLUSTER)
    {
        OS_FAT14.max_fat14_clusters = MAX_FAT14_CLUSTER - 4;
    }

    //检查文件系统特征字
    if(fat14_checkflag() == FALSE)
    {
        fat14_format(TRUE);
//        record_sys_operate(OP_SYS_FAT_FORMAT);
        result |= FORMAT;
        if(fat14_checkflag() == FALSE) return FALSE;
    }
    //检查文件系统版本，不同版本，并根据版本做相应处理
    fat14_var_check();

    //初始化打开文件信息
    OS_FAT14.open_file_count = 0;
    for(idx=0;idx<MAX_OPEN_FILE;idx++)
    {
      OS_FAT14.opened_file_id[idx] = NONE_FILE_ID;
      OS_FAT14.in_using_free_clusters[idx]=INVALID_CLUSTER;
    }
    //初始化历史空闲列表
    for(idx=0;idx<MAX_HIS_FREE_CLUSTER;idx++)
    {

       OS_FAT14.his_free_cluster[idx] = INVALID_CLUSTER;
    }

    //系统当前空闲簇数
    OS_FAT14.free_cluster_count = system_free_cluster_count();


    //确定当前最小簇擦写轮次数
    detect_min_cluster_write_count();

    //预先提取空闲簇
    fat14_prefetch_free_clusters();


    //检查事务控制
    check_file_transctrl();


    //文件系统完整性检查
   // FAT14_Integrity_Check();

    result |= TRUE;
    return result;
}
/*+++
  功能:检查文件系统版本，不同版本，并根据版本做相应处理
  参数
       无
  返回:
       无
  描述:
        1)
---*/
void fat14_var_check(void)
{
//   INT32U      fmAddr;
//   INT16U      idx;
//   INT8U       buf[2];
//   FAT_ENTRY   entry;
//
//   read_fmArray(FMADDR_TRANSCTRL+44,buf,2);
//   首次使用版本，首次增加将多功能表FLASH纳入文件系统管理中，因此需要初始化多余的FAT表位空闲簇
//   if(((0xFF==buf[0]) && (0xFF==buf[1])) || ((0x00==buf[0]) && (0x00==buf[1])))
//   {
//        entry.write_count = 0;
//        entry.cluster = FILE_FREE_CLUSTER;
//        for(idx=gFlash.chip1.page_max + gFlash.chip2.page_max;idx<OS_FAT14.max_fat14_clusters;idx++)
//        {
//
//            初始化FAT表
//            fmAddr = FMADDR_FAT14+idx*sizeof(FAT_ENTRY);
//            write_fmArray(fmAddr,(INT8U *)&entry,sizeof(FAT_ENTRY));
//        }
//        buf[0]=FAT_MAJOR_VER;
//        buf[1]=FAT_MINOR_VER;
//        write_fmArray(FMADDR_TRANSCTRL+44,buf,2);
//   }


}
/*+++
  功能:检查文件系统事务控制,完成进行中的事务
  参数
       无
  返回:
       无
  描述:
        1)仅仅在初始化文件系统时被调用.
---*/
void  check_file_transctrl(void)
{
   INT32U addr;
   INT32U max_size;
   INT16U len;
   FAT14_FILE file;
   INT8U  transctrl_flag[2];  //事务控制标志
   INT8U  transctrl_class;    //事务类别
   INT8U  data[50];

   //读取事务控制标志
   read_fmArray(FMADDR_TRANSCTRL,transctrl_flag,2);
   if( (transctrl_flag[0]!=0x55)  || (transctrl_flag[1]!=0xAA) ) return;


   //读取事务控制类别
   transctrl_class = read_fmByte(FMADDR_TRANSCTRL+2);

   switch(transctrl_class)
   {
   case FILE_TRANSCTRL_UPDATE_FATFDT:
        //更新FAT，FDT
        read_fmArray(FMADDR_TRANSCTRL+3,(INT8U *)&file,sizeof(FAT14_FILE));
        file_update_fat14(&file);
        break;
   case FILE_TRANSCTRL_FMWRITE:
        //写关键数据到铁电
        read_fmArray(FMADDR_TRANSCTRL+3,(INT8U *)&addr,sizeof(INT16U));
        len = read_fmByte(FMADDR_TRANSCTRL+5);
        read_fmArray(FMADDR_TRANSCTRL+6,data,len);
        safe_write_fmArray(addr,data,len);
        break;
   case FILE_TRANSCTRL_DELETE_FILE:
        //读取未删除完成的文件ID到addr,然后继续删除
        read_fmArray(FMADDR_TRANSCTRL+3,(INT8U *)&addr,sizeof(INT16U));
        file_delete(addr);
        break;
   case FILE_TRANSCTRL_TRIM_FILE:
        //截断文件
        read_fmArray(FMADDR_TRANSCTRL+3,(INT8U *)&addr,sizeof(INT16U));
        read_fmArray(FMADDR_TRANSCTRL+5,(INT8U *)&max_size,sizeof(INT32U));
        file_trim(addr,max_size);
        break;
   default:
        break;
   }

   //事务控制标志结束
   transctrl_flag[0]=0;
   transctrl_flag[1]=0;
   write_fmArray(FMADDR_TRANSCTRL,transctrl_flag,2);
}


/*+++
  功能：检查文件系统特征字
  参数：
        无
  返回：
        TRUE  特征字OK
        FALSE 特征字ERR
  描述：
       铁电中前5个字节表示文件系统特征字 FAT14
---*/
BOOLEAN fat14_checkflag(void)
{
      INT8U buffer[5],fat_14[5];
      INT8U idx;

      fat_14[0]='F';
      fat_14[1]='A';
      fat_14[2]='T';
      fat_14[3]='1';
      fat_14[4]='4';

      for(idx=0; idx<3; idx++)
      {
         //读取文件系统特征字
         read_fmArray(FMADDR_FAT_FLAG,buffer,5);
         if(0 == compare_string(fat_14,buffer,5))   return TRUE;
      }
      return FALSE;

}


/*+++
  功能：格式化文件系统
  参数:
        BOOLEAN  quick_format   是否快速格式化
  返回:
        无

  描述:
       最大MAX_FAT14_CLUSTER
---*/
void fat14_format(BOOLEAN  quick_format)
{
    INT32U      fmAddr;
    INT16U      idx;
    FAT_ENTRY   entrys[16];
    INT8U       tmp[5];

    OSMutexPend(&SIGNAL_FLASHSPI);

    for(idx=0;idx<16;idx++)
    {
       entrys[idx].write_count = 0;
       entrys[idx].cluster = FILE_FREE_CLUSTER;
    }

    //初始化FAT表
    fmAddr = FMADDR_FAT14;
    for(idx = 0; idx < OS_FAT14.max_fat14_clusters; idx +=  16)
    {
        write_fmArray(fmAddr,(INT8U *)entrys,sizeof(FAT_ENTRY)*16);
        fmAddr += sizeof(FAT_ENTRY)*16;
    }

    //初始化根目录FDT,使用0xFF填充
    set_fmArray(FMADDR_FDT,0xFF,OS_LEN_FDT);

    //初始化事务控制区，最大50字节
    set_fmArray(FMADDR_TRANSCTRL,0x00,50);
    
//    set_fmArray(FMADDR_PLC_NODE_COUNT,0x00,2);

    //写文件系统版本号
    tmp[0]=FAT_MAJOR_VER;
    tmp[1]=FAT_MINOR_VER;
    write_fmArray(FMADDR_TRANSCTRL+44,tmp,2);
    
    //写文件系统标识
    tmp[0]='F';
    tmp[1]='A';
    tmp[2]='T';
    tmp[3]='1';
    tmp[4]='4';
    write_fmArray(FMADDR_FAT_FLAG,tmp,5);

    //如果是快速格式化，则不用处理NAND_FLASH的数据
    if(TRUE == quick_format) 
    {
       OSMutexFree(&SIGNAL_FLASHSPI);
       return;
    }

    //擦除NAND_FLASH数据
    //采用页擦除方式清除文件系统页面
    for(idx = 0; idx < OS_FAT14.max_fat14_clusters; idx++)
    {
        //转换簇号为NAND FLASH对应的页面地址fmAddr
        fmAddr = cluster_to_page(idx);          
        os_flash_erase_page(fmAddr); //目前不会执行，应该考虑哪片FLASH转换
    }
   OSMutexFree(&SIGNAL_FLASHSPI);
}


/*+++
  功能：确定当前最小簇擦写轮次数
  参数：
        无
  返回：
        无
  修改：
        OS_FAT.min_cluster_write_count
---*/
void detect_min_cluster_write_count(void)
{
    INT32U fmAddr;
    INT32U tmp_u32;
    FAT_ENTRY   fat_entry[16];  //16刚好是MAX_FAT14_CLUSTER的因数
    INT16U cluster_count;
    INT16U len;
    INT16U idx,u16;

    cluster_count = 0;
    fmAddr = FMADDR_FAT14;
    len  = sizeof(FAT_ENTRY)*16;
    OS_FAT14.min_cluster_write_count = 0xFF;
    do
    {
       read_fmArray(fmAddr,(unsigned char *)fat_entry,len);

       for(idx=0;idx<16;idx++)
       {
          //检查簇是否空闲
          cluster_count ++;
          if(cluster_count  > OS_FAT14.max_fat14_clusters) break; //必须放在前面执行。
          if(FILE_FREE_CLUSTER != fat_entry[idx].cluster) continue;
          if(fat_entry[idx].write_count < OS_FAT14.min_cluster_write_count)
          {
             OS_FAT14.min_cluster_write_count = fat_entry[idx].write_count;
          }
          //如果找到最小值为0，则可以不用再检查了！
          if(OS_FAT14.min_cluster_write_count == 0) return;
       }
       fmAddr += len;
       //cluster_count += 16;
    } while(cluster_count<OS_FAT14.max_fat14_clusters);



    if(OS_FAT14.min_cluster_write_count < MAX_CLUSTER_WRITE_LOOP) return;

    //如果最小擦写轮次等于MAX_CLUSTER_WRITE_LOOP，
    //则需要把所有空闲簇的擦写轮次复位为0
    
    read_fmArray(FMADDR_TRANSCTRL+46,(unsigned char *)&tmp_u32,4);
    if(tmp_u32==0xffffffff)
       tmp_u32 = 0;
    tmp_u32+=4;
    write_fmArray(FMADDR_TRANSCTRL+46,(unsigned char *)&tmp_u32,4);
    
    cluster_count = 0;
    fmAddr = FMADDR_FAT14;
    len  = sizeof(FAT_ENTRY)*16;
    do
    {
       read_fmArray(fmAddr,(unsigned char *)fat_entry,len);
       for(idx=0;idx<16;idx++)
       {
          //检查簇是否空闲
          cluster_count ++;
          if(cluster_count  > OS_FAT14.max_fat14_clusters) break; //必须放在前面执行。
          if(FILE_FREE_CLUSTER != fat_entry[idx].cluster) continue;

          //擦写轮次复位为0
          fat_entry[idx].write_count = 0;

          u16 = idx*sizeof(FAT_ENTRY);
          write_fmArray(fmAddr + u16 ,(unsigned char *)&fat_entry[idx],sizeof(FAT_ENTRY));
       }
       fmAddr += len;
      // cluster_count += 16;
    } while(cluster_count<OS_FAT14.max_fat14_clusters);

    OS_FAT14.min_cluster_write_count = 0;
}

/*+++
  功能：从FAT表中提取空闲簇
  参数：
        无
  返回：
        无
  修改：
        OS_FAT.free_clusters_count
        OS_FAT.free_clusters

---*/

void  fat14_prefetch_free_clusters(void)
{
    INT32U fmAddr;
    FAT_ENTRY   fat_entry[16];  //16刚好是MAX_FAT14_CLUSTER的因数
    INT16U cluster_count;
    INT16U cluster,len;
    INT16U idx,ii;

FETCH_FREE_CLUSTER:
    cluster_count = 0;
    fmAddr = FMADDR_FAT14;
    len  = sizeof(FAT_ENTRY)*16;
    OS_FAT14.free_clusters_count = 0;
    OS_FAT14.free_cluster_count = 0;
    do
    {
       read_fmArray(fmAddr,(unsigned char *)fat_entry,len);
       for(idx=0;idx<16;idx++)
       {
          //检查簇是否空闲
          cluster_count ++;
          if(cluster_count  > OS_FAT14.max_fat14_clusters) break; //必须放在前面执行。
          if(FILE_FREE_CLUSTER != fat_entry[idx].cluster) continue;

          OS_FAT14.free_cluster_count++;
          if(fat_entry[idx].write_count == OS_FAT14.min_cluster_write_count)
          {
              cluster = cluster_count-1;// + idx;

              //检查簇是否在当前新申请占用队列中
              for(ii=0;ii<MAX_OPEN_FILE;ii++)
              {
                 if(OS_FAT14.in_using_free_clusters[ii]==cluster)
                 {
                    cluster = INVALID_CLUSTER;
                    break;
                  }
              }
              if(cluster != INVALID_CLUSTER)
              {
                OS_FAT14.free_clusters[OS_FAT14.free_clusters_count ++] = cluster;
              }
              if(OS_FAT14.free_clusters_count==MAX_PRE_GET_CLUSTER) return;
          }

       }
       fmAddr += len;
       //cluster_count += 16;
    } while(cluster_count<OS_FAT14.max_fat14_clusters);

    //如果提取到的空闲簇数为0，并且空闲簇大于0，则执行重新探测最小值
    if(OS_FAT14.free_clusters_count == 0)
    {
       if(OS_FAT14.free_cluster_count == 0)
       {
//       record_sys_operate(OP_SYS_FAT_NOFREE_PAGE); 
		 	 return;
       }
       detect_min_cluster_write_count();
       goto FETCH_FREE_CLUSTER;
    }
}



/*+++
  功能:获取一个空闲簇
  参数:
        INT8U open_file_seq   文件打开的序号,指系统中同时打开的文件.
  返回:
        空闲簇序号
  描述:
       1)独占资源操作,需要使用文件系统信号量
       2)直接调用方法
             重新提取空闲簇: fat14_prefetch_free_clusters
       3)间接调用方法
             确定当前最小簇擦写轮次数: detect_min_cluster_write_count

---*/
INT16U  get_a_free_cluster(INT8U open_file_seq)
{
     INT16U  free_cluster;
     INT8U   idx,i;
     BOOLEAN is_fetch;

     free_cluster = INVALID_CLUSTER;

     if(open_file_seq >= MAX_OPEN_FILE) return INVALID_CLUSTER;

      //请求操作信号量
     //OSMutexPend(&OS_FAT14.SIGNAL);
     OSMutexPend(&SIGNAL_FLASHSPI);
     if(OS_FAT14.free_clusters_count <= 0)
     {
        //重新提取空闲簇
        fat14_prefetch_free_clusters();
     }
     free_cluster  = INVALID_CLUSTER;
     is_fetch      = TRUE;
     //如果没有提取到,说明没有空闲簇了
     if(OS_FAT14.free_clusters_count > 0)
     {
        GET_FREE_CLUSTER:
        //提取以前不同BLOCK的空闲簇
        for(idx=OS_FAT14.free_clusters_count-1;idx>0;idx--)
        {
            for(i=0;i<MAX_HIS_FREE_CLUSTER;i++)
            {
                if((cluster_to_page(OS_FAT14.free_clusters[idx])/CLUSTER_COUNT_OF_BLOCK==cluster_to_page(OS_FAT14.his_free_cluster[i])/CLUSTER_COUNT_OF_BLOCK)
                && (OS_FAT14.his_free_cluster[i] != INVALID_CLUSTER))
                {
                    break;
                }

            }
            //与以前提取过的都不相同
            if(i==MAX_HIS_FREE_CLUSTER)
            {
                free_cluster = OS_FAT14.free_clusters[idx];
                OS_FAT14.free_clusters[idx] = INVALID_CLUSTER;
                //重新排列free_clusters列表
                for(i=0;i<OS_FAT14.free_clusters_count-1;i++)
                {
                    if((OS_FAT14.free_clusters[i]==INVALID_CLUSTER))
                    {
                        OS_FAT14.free_clusters[i]=OS_FAT14.free_clusters[i+1];
                        OS_FAT14.free_clusters[i+1] = INVALID_CLUSTER;

                    }
                }
                OS_FAT14.free_clusters_count--;
        OS_FAT14.in_using_free_clusters[open_file_seq] = free_cluster;
                //存储历史空闲块
                for(i=0;i<MAX_HIS_FREE_CLUSTER-1;i++)
                {

                   OS_FAT14.his_free_cluster[i] = OS_FAT14.his_free_cluster[i+1];
                }
                OS_FAT14.his_free_cluster[MAX_HIS_FREE_CLUSTER-1] = free_cluster;
                break;
     }

        }
        if((free_cluster == INVALID_CLUSTER) && is_fetch)
     {
            is_fetch = FALSE;
        //重新提取空闲簇
        fat14_prefetch_free_clusters();
            goto GET_FREE_CLUSTER;

        }
        //如果已经提取的空闲簇还无法满足，该空闲块不在上2次空闲块的block中，则顺序提取。
        if((free_cluster == INVALID_CLUSTER) && !is_fetch)
        {

           free_cluster = OS_FAT14.free_clusters[--OS_FAT14.free_clusters_count];
           OS_FAT14.in_using_free_clusters[open_file_seq] = free_cluster;
                //存储历史空闲块
                for(i=0;i<MAX_HIS_FREE_CLUSTER-1;i++)
                {

                   OS_FAT14.his_free_cluster[i] = OS_FAT14.his_free_cluster[i+1];
        }
                OS_FAT14.his_free_cluster[MAX_HIS_FREE_CLUSTER-1] = free_cluster;


        }

     }

   //  OSMutexFree(&OS_FAT14.SIGNAL);
     OSMutexFree(&SIGNAL_FLASHSPI);
     return free_cluster;
}

/*+++
  功能：获取系统当前空闲簇数
  参数：
         无
  返回：
         INT16U

----*/
INT16U  system_free_cluster_count(void)
{
    INT32U fmAddr;
    FAT_ENTRY   fat_entry[16];  //16刚好是MAX_FAT14_CLUSTER的因数
    INT16U free_cluster_count,cluster_count;
    INT16U cluster,len;
    INT16U idx,ii;

    cluster_count = 0;
    free_cluster_count = 0;
    fmAddr = FMADDR_FAT14;
    len  = sizeof(FAT_ENTRY)*16;
    do
    {
       read_fmArray(fmAddr,(unsigned char *)fat_entry,len);
       for(idx=0;idx<16;idx++)
       {
          //检查簇是否空闲
          cluster_count ++;
          if(cluster_count  > OS_FAT14.max_fat14_clusters) break;  //必须放在前面执行。
          if(FILE_FREE_CLUSTER != fat_entry[idx].cluster) continue;
          free_cluster_count++;
       }
       fmAddr += len;
       //cluster_count += 16;
    } while(cluster_count<OS_FAT14.max_fat14_clusters);

    return free_cluster_count;
}

/*+++
  功能：创建文件
  参数:
        INT16U file_id
  返回:
        TRUE  成功
        FALSE 失败
---*/
INT8U  file_create(INT16U file_id)
{
    FDT_ENTRY  fdt_entry;
    INT32U  fmAddr;

    if(file_id >= MAX_FDT_COUNT) return FILE_ERROR_FILE_ID;

    fmAddr = FMADDR_FDT + file_id*sizeof(FDT_ENTRY);

    read_fmArray(fmAddr,(unsigned char *)&fdt_entry,sizeof(FDT_ENTRY));

    //文件已经创建了！
    if(fdt_entry.file_id == file_id) return SUCCESS;

    //新创建文件
    fdt_entry.file_id = file_id;
    fdt_entry.stamp_id = 0;
    fdt_entry.first_cluster = INVALID_CLUSTER;

    //以事务控制方式更新FDT
    safe_write_fmArray(fmAddr,(unsigned char *)&fdt_entry,sizeof(FDT_ENTRY));

    return SUCCESS;
}


/*+++
  功能:读取下一个簇号
  参数:
       INT16U cur_cluster 当前簇号
  返回:
       下一个簇号
---*/
INT16U  file_next_cluster(INT16U cur_cluster)
{
    INT32U    fmAddr;
    FAT_ENTRY  fat_entry;

    if(INVALID_CLUSTER == cur_cluster) return  INVALID_CLUSTER;
    if(cur_cluster >= OS_FAT14.max_fat14_clusters) return INVALID_CLUSTER;
     
    fmAddr = FMADDR_FAT14 + cur_cluster * sizeof(FAT_ENTRY);
    read_fmArray(fmAddr,(INT8U *)&fat_entry,sizeof(FAT_ENTRY));

    //如果下一个簇为结束簇,则返回无效簇
    if(FILE_END_CLUSTER == fat_entry.cluster) return INVALID_CLUSTER;

    //如果下一个簇为空闲簇,也是无效的
    if(FILE_FREE_CLUSTER == fat_entry.cluster) return INVALID_CLUSTER;

    return fat_entry.cluster;
}


/*+++
  功能：读取前一个簇号
  参数：
        INT16U first_cluster,    文件首簇号
        INT16U cur_cluster       当前簇号
  返回：
        前一个簇号
---*/
INT16U  file_prev_cluster(INT16U first_cluster,INT16U cur_cluster)
{
    INT16U cluster;
    INT16U counter;

    if(cur_cluster==first_cluster) return INVALID_CLUSTER;

    for(counter=0;counter<MAX_FILE_CLUSTERS;counter++)
    {
        cluster = file_next_cluster(first_cluster);
        if(cluster == cur_cluster) return first_cluster;
        first_cluster = cluster;
        if(INVALID_CLUSTER == cluster) break;
    }

    return  INVALID_CLUSTER;
}

/*+++
  功能:  打开文件
  参数:
        INT16U file_id,       [输入]  文件号
        FAT14_FILE *file      [输出]  文件句柄
  返回:
        操作结果
  描述:
        1)登记打开的文件时需要使用信号量
        2)文件不能同时被打开，也就是说文件不支持共享。
        3)如果30秒仍然不能打开文件，返回文件忙错误
---*/
INT8U file_open(INT16U file_id,FAT14_FILE *file)
{
    INT32U  fmAddr;
    INT8U   result;
    INT8U   open_file_seq;  //在已打开文件队列中的序号

    if(OS_FAT14.open_file_count >= MAX_OPEN_FILE) return FILE_MAX_OPEN_REACHED;
    if(file_id >= MAX_FDT_COUNT) return FILE_ERROR_FILE_ID;

    //
    //登记打开的文件
    //


    open_file_seq = 0;
    for(fmAddr = 0; fmAddr < MAX_OPEN_FILE; fmAddr++)
    {
       if(OS_FAT14.opened_file_id[fmAddr]==NONE_FILE_ID)
       {
          open_file_seq = fmAddr;
       }
       if(OS_FAT14.opened_file_id[fmAddr] == file_id)
       {
          //文件已经打开了，需要等待
          return FILE_IS_BUSY;
       } 
    }
    OS_FAT14.opened_file_id[open_file_seq] = file_id;
    OS_FAT14.open_file_count++;
    file->open_file_seq = open_file_seq;

    
    //读取文件FDT
    fmAddr = FMADDR_FDT + file_id*sizeof(FDT_ENTRY);
    read_fmArray(fmAddr,(unsigned char *)&(file->fdt_entry),sizeof(FDT_ENTRY));

    if(file->fdt_entry.file_id != file_id)
    {
     //       record_sys_operate(OP_SYS_FLASH_FDT_BAD);
     //       record_sys_operate(file_id);
     //       record_sys_operate(file->fdt_entry.file_id);
        result =  file_create(file_id);
        if(result != SUCCESS) return result;
        read_fmArray(fmAddr,(unsigned char *)&(file->fdt_entry),sizeof(FDT_ENTRY));
    }


    //文件打开后,当前位置在起始位置!
    //必须在登记打开文件后调用
    file->prev_cluster = INVALID_CLUSTER;
    file->cur_cluster = file->fdt_entry.first_cluster;
    file->cluster_seq = (file->cur_cluster == INVALID_CLUSTER) ? INVLAID_CLUSTER_SEQ : 0;
    file->next_cluster = file_next_cluster(file->cur_cluster);
	file->operate = FILE_OP_READ;
    file_seek(file,0);

    return SUCCESS;
}


/*+++
  功能：关闭文件
  参数：
         FAT14_FILE *file,              [输入]文件句柄
  返回
        操作结果
  描述:
        1)登记打开的文件时需要使用信号量
---*/
INT8U  file_close(FAT14_FILE *file)
{
   INT16U idx;

   //
   //撤销文件打开登记标志
   //

     OSMutexPend(&SIGNAL_FLASHSPI);
    //请求操作信号量
   //OSMutexPend(&OS_FAT14.SIGNAL);
   OS_FAT14.opened_file_id[file->open_file_seq] = NONE_FILE_ID;
   OS_FAT14.open_file_count--;
   //释放信号量
  // OSMutexFree(&OS_FAT14.SIGNAL);
     OSMutexFree(&SIGNAL_FLASHSPI);

   file->fdt_entry.file_id = 0;
   file->fdt_entry.first_cluster = INVALID_CLUSTER;
   file->prev_cluster = INVALID_CLUSTER;
   file->cur_cluster = INVALID_CLUSTER;
   file->next_cluster = INVALID_CLUSTER;

   return SUCCESS;
}

/*+++
  功能: 文件定位,从起始位置算起
  参数:
        FAT14_FILE *file,              [输入]文件句柄
        INT32U  offset,                [输入]文件内偏移位置,从文件头算起
  返回:
        操作结果
  描述:
        需要把offset转换为FLASH地址


---*/
INT8U file_seek(FAT14_FILE *file,INT32U offset)
{
     INT32U  cluster;

     //当前位置所在的簇序号
     cluster = offset / OS_FAT14.per_cluster_data_size;
     file->offset = offset;
     file->cluster_offset = offset - cluster*OS_FAT14.per_cluster_data_size;

     //移动到指定的簇序号
     return file_move_to_cluster_seq(file,cluster);
}


/*+++
  功能: 移动到指定的簇序号
  参数:
         FAT14_FILE *file,
         INT16U cluster_seq

  返回:
         操作结果
  描述:
         如果达到文件末尾,需要申请空间

---*/
INT8U  file_move_to_cluster_seq(FAT14_FILE *file,INT16U cluster_seq)
{
    if(cluster_seq > OS_FAT14.max_fat14_clusters) return FILE_ERROR_CLUSTER_SEQ;

    if(file->cluster_seq ==INVLAID_CLUSTER_SEQ)
    {
	    //如果读文件，碰到文件末尾，返回EOF 
	    if(file->operate == FILE_OP_READ) return FILE_EOF;
		
       //新创建的文件
        while(1)
        {
        file->cur_cluster = get_a_free_cluster(file->open_file_seq);
        if(file->cur_cluster == INVALID_CLUSTER) return FILE_NO_SPACE;
        file->cluster_seq = 0;
           if(file_update_cluster_head(file) == TRUE)
              break;
        }  
        file->free_cluster = INVALID_CLUSTER;
        file_update_fat14(file);
    }

    if(file->cluster_seq > MAX_FILE_CLUSTERS) return FILE_ERROR_CLUSTER_SEQ;

    while(file->cluster_seq != cluster_seq)
    {
          if(file->cluster_seq < cluster_seq)
          {
              file->prev_cluster = file->cur_cluster;
              //move next
              if(file->next_cluster != INVALID_CLUSTER)
              {
                 //position to next cluster
                 file->cur_cluster = file->next_cluster;
                 file->next_cluster = file_next_cluster(file->next_cluster);
                 file->cluster_seq ++;
              }
              else
              {
			  	 //如果读文件，碰到文件末尾，返回EOF 
	             if(file->operate == FILE_OP_READ) return FILE_EOF;
				 
                 //need allocate a new cluster
                 while(1)
                 {
                 file->cur_cluster = get_a_free_cluster(file->open_file_seq);
                 if(file->cur_cluster == INVALID_CLUSTER) return FILE_NO_SPACE;
                 file->cluster_seq ++;
                    if(file_update_cluster_head(file) == TRUE)
                       break;
                 }
                 file->free_cluster = INVALID_CLUSTER;
                 file_update_fat14(file);
              }
          }
          else
          {
              //check if prev already is invlaid, it must be an fat error!
              if(file->prev_cluster == INVALID_CLUSTER) return  FILE_ERROR_FAT;

              //move prev
              file->next_cluster = file->cur_cluster;
              file->cur_cluster = file->prev_cluster;
              file->prev_cluster = file_prev_cluster(file->fdt_entry.first_cluster,file->cur_cluster);
              if(file->cluster_seq==0) return  FILE_ERROR_FAT;
              file->cluster_seq--;
          }
    }
    return SUCCESS;
}


/*+++
  功能：更新文件FAT表
  参数：
        FAT14_FILE *file
  返回：
        TRUE/FALSE
  描述：
        这里需要加上事务性处理？？？
---*/
BOOLEAN file_update_fat14(FAT14_FILE *file)
{
    FAT_ENTRY  fat_entry;
    INT32U   fmAddr;
    INT8U    trans_ctrl[2];


    trans_ctrl[0]=0x55;
    trans_ctrl[1]=0xAA;

    //请求操作信号量
    //OSMutexPend(&OS_FAT14.SIGNAL);
     OSMutexPend(&SIGNAL_FLASHSPI);


    //事务控制类别
    write_fmByte(FMADDR_TRANSCTRL+2,FILE_TRANSCTRL_UPDATE_FATFDT);
    //事务控制参数
    write_fmArray(FMADDR_TRANSCTRL+3,(INT8U *)file,sizeof(FAT14_FILE));
    //事务启动标志
    write_fmArray(FMADDR_TRANSCTRL,trans_ctrl,2);


    if(file->prev_cluster != INVALID_CLUSTER)
    {
      fmAddr = FMADDR_FAT14 + file->prev_cluster * sizeof(FAT_ENTRY);
      read_fmArray(fmAddr,(INT8U *)&fat_entry,sizeof(FAT_ENTRY));
      if(fat_entry.cluster != file->cur_cluster)
      {
         fat_entry.cluster = file->cur_cluster;
         write_fmArray(fmAddr,(INT8U *)&fat_entry,sizeof(FAT_ENTRY));
      }
    }
    else
    {
       if(file->fdt_entry.first_cluster != file->cur_cluster)
       {
           file->fdt_entry.first_cluster = file->cur_cluster;
           fmAddr = FMADDR_FDT + file->fdt_entry.file_id*sizeof(FDT_ENTRY);
           write_fmArray(fmAddr,(INT8U *)&(file->fdt_entry),sizeof(FDT_ENTRY));
       }
    }

    fmAddr = FMADDR_FAT14 + file->cur_cluster * sizeof(FAT_ENTRY);
    read_fmArray(fmAddr,(INT8U *)&fat_entry,sizeof(FAT_ENTRY));
    if(fat_entry.write_count < MAX_CLUSTER_WRITE_LOOP) fat_entry.write_count++;
    fat_entry.cluster = (file->next_cluster == INVALID_CLUSTER) ? FILE_END_CLUSTER : file->next_cluster;
    write_fmArray(fmAddr,(INT8U *)&fat_entry,sizeof(FAT_ENTRY));


    //检查是否需要释放簇
    if(file->free_cluster < OS_FAT14.max_fat14_clusters)
    {
        fmAddr =  FMADDR_FAT14 + file->free_cluster * sizeof(FAT_ENTRY);
        read_fmArray(fmAddr,(INT8U *)&fat_entry,sizeof(FAT_ENTRY));
        fat_entry.cluster = FILE_FREE_CLUSTER;
        write_fmArray(fmAddr,(INT8U *)&fat_entry,sizeof(FAT_ENTRY));
        file->free_cluster = INVALID_CLUSTER;
    }

    //事务完成标志
    trans_ctrl[0]=0x66;
    trans_ctrl[1]=0x99;
    write_fmArray(FMADDR_TRANSCTRL,trans_ctrl,2);

    //释放文件操作信号量
    //OSMutexFree(&OS_FAT14.SIGNAL);
     OSMutexFree(&SIGNAL_FLASHSPI);


    return TRUE;
}



/*+++
  功能：以事务方式写铁电内容
  参数：
        INT16U addr,         地址
        INT8U *data,         数据区
        INT16U len           数据区长度，目前限定长度不能超过46字节
---*/
void safe_write_fmArray(INT32U addr,INT8U *data,INT16U len)
{
    INT8U    trans_ctrl[2];


    trans_ctrl[0]=0x55;
    trans_ctrl[1]=0xAA;
    if(len > 44)
    {
       write_fmArray(addr,data,len);
    }
    else
    {
       //请求操作信号量
       //OSMutexPend(&OS_FAT14.SIGNAL);
       OSMutexPend(&SIGNAL_FLASHSPI);

       //事务控制类别
       write_fmByte(FMADDR_TRANSCTRL+2,FILE_TRANSCTRL_FMWRITE);
       //事务控制参数
       write_fmArray(FMADDR_TRANSCTRL+3,(INT8U *)&addr,2);
       write_fmByte(FMADDR_TRANSCTRL+5,len);
       write_fmArray(FMADDR_TRANSCTRL+6,data,len);
       //事务启动标志
       write_fmArray(FMADDR_TRANSCTRL,trans_ctrl,2);

       //执行写操作
       write_fmArray(addr,data,len);

       //事务完成标志
       trans_ctrl[0]=0x66;
       trans_ctrl[1]=0x99;
       write_fmArray(FMADDR_TRANSCTRL,trans_ctrl,2);

       //释放文件操作信号量
       //OSMutexFree(&OS_FAT14.SIGNAL);
        OSMutexFree(&SIGNAL_FLASHSPI);

    }
}


/*+++
  功能：检查文件是否存在
  参数：
        INT16U file_id
  返回：
         TRUE / FALSE

---*/
BOOLEAN  file_exist(INT16U file_id)
{
   INT32U     fmAddr;
   FDT_ENTRY  fdt_entry;

   if(file_id > MAX_FDT_COUNT) return FALSE;

   fmAddr = FMADDR_FDT + file_id*sizeof(FDT_ENTRY); 
   read_fmArray(fmAddr,(INT8U *)&fdt_entry,sizeof(FDT_ENTRY));

   if(fdt_entry.file_id != file_id) return FALSE;

   return TRUE;
}

/*+++
  功能：删除文件
  参数：
        INT16U  file_id
  返回：
        操作结果
  描述：
       1）加入事务性处理，并且需要占用系统操作信号量
       2）自后向前依次删除每个簇，最后删除FDT
---*/
INT8U  file_delete(INT16U file_id)
{
   INT32U fmAddr;
   INT16U idx,count;
   INT16U cluster;
   INT16U fat_clusters[64];
   INT16U prev_cluster;       //分组删除的前组最末的簇号
   FDT_ENTRY  fdt_entry;
   FAT_ENTRY fat_entry;
   INT8U trans_ctrl[2];

   //不存在的文件,删除直接返回成功!
   if(file_exist(file_id)==FALSE) return  SUCCESS;

   //检查文件打开登记标志
   for(idx=0;idx<MAX_OPEN_FILE;idx++)
   {
       if(OS_FAT14.opened_file_id[idx]==file_id)
       {
          return FILE_IS_OPEN;
       }
   }

   if(file_id > MAX_FDT_COUNT) return FILE_ERROR_FILE_ID;

   #ifdef __PROVICE_JIANGSU__
	if(file_id==1)
	{
		record_sys_operate(OP_SYS_DEL_FILE1);
	}
   #endif
   //请求操作信号量
   //OSMutexPend(&OS_FAT14.SIGNAL);
     OSMutexPend(&SIGNAL_FLASHSPI);

   //事务控制类别
   write_fmByte(FMADDR_TRANSCTRL+2,FILE_TRANSCTRL_DELETE_FILE);
   //事务控制参数
   write_fmArray(FMADDR_TRANSCTRL+3,(INT8U *)&file_id,2);
   //事务启动标志
   trans_ctrl[0] = 0x55;
   trans_ctrl[1] = 0xAA;
   write_fmArray(FMADDR_TRANSCTRL,trans_ctrl,2);

   fmAddr = FMADDR_FDT + file_id*sizeof(FDT_ENTRY);

   read_fmArray(fmAddr,(INT8U *)&fdt_entry,sizeof(FDT_ENTRY));

   //首先删除FAT链表，需要执行事务性操作
   //为了使事务最小，采取从后删除的方法。
   //为了提高效率,采用每次调入最后64个簇的方法

   do{

      //调入文件的最后64簇号
      cluster = fdt_entry.first_cluster;
      for(idx=0;idx<64;idx++) fat_clusters[idx]=INVALID_CLUSTER;
      idx=0;
      count=0;
      prev_cluster = cluster;
      while(cluster!=INVALID_CLUSTER)
      {
          count++;
          if(idx>=64)
          {
            prev_cluster = fat_clusters[63];
            idx=0;
          }
          fat_clusters[idx++] = cluster;
          cluster = file_next_cluster(cluster);
      }

      //依次释放调入的后面的簇
      while(idx>0)
      {
          idx--;

          //释放簇 
          fmAddr =  FMADDR_FAT14+fat_clusters[idx]*sizeof(FAT_ENTRY);
          read_fmArray(fmAddr,(INT8U *)&fat_entry,sizeof(FAT_ENTRY));
          fat_entry.cluster = FILE_FREE_CLUSTER;
          write_fmArray(fmAddr,(INT8U *)&fat_entry,sizeof(FAT_ENTRY));


          //需要分组删除时，需要把前组的最末簇设置为最后的簇
          if( (idx==0) && (prev_cluster != fdt_entry.first_cluster))
          {
              fmAddr =  FMADDR_FAT14+prev_cluster*sizeof(FAT_ENTRY);
              read_fmArray(fmAddr,(INT8U *)&fat_entry,sizeof(FAT_ENTRY));
              fat_entry.cluster = FILE_END_CLUSTER;
              write_fmArray(fmAddr,(INT8U *)&fat_entry,sizeof(FAT_ENTRY));
          }
      }
   }while(fat_clusters[0] != fdt_entry.first_cluster);

   //删除更新FDT
   fdt_entry.file_id = DELETED_FILE_ID;
   fdt_entry.first_cluster = INVALID_CLUSTER;
   fdt_entry.stamp_id = 0;
   fmAddr =  FMADDR_FDT + file_id*sizeof(FDT_ENTRY);
   write_fmArray(fmAddr,(INT8U *)&fdt_entry,sizeof(FDT_ENTRY));

   //事务完成标志
   trans_ctrl[0]=0x66;
   trans_ctrl[1]=0x99;
   write_fmArray(FMADDR_TRANSCTRL,trans_ctrl,2);

   //释放信号量
   //OSMutexFree(&OS_FAT14.SIGNAL);
     OSMutexFree(&SIGNAL_FLASHSPI);



   return SUCCESS;
}
///*+++
//  功能：删除文件
//  参数：
//        INT16U  file_id
//  返回：
//        操作结果
//  描述：
//       1）加入事务性处理，并且需要占用系统操作信号量
//       2）自后向前依次删除每个簇，最后删除FDT
//---*/
//INT8U  file_delete_exec(INT16U file_id)
//{
//  INT8U result;
//
//  OSMutexPend(&SIGNAL_FLASHSPI);
//  
//  result =file_do_delete(file_id);
//
//  #ifdef __IMPORTANT_PARAM_BACKUP_RESTORE__
//  //同步写入备份参数文件和档案文件
//  if((file_id>=1) && (file_id<=MAX_METER_COUNT))
//  {
//        backup_clear_meter_param(file_id,0);
//        backup_clear_meter_param(file_id,1);
//  }
//  #endif
//  OSMutexFree(&SIGNAL_FLASHSPI);
//  
//  return result;
//}

/*+++
  功能：截断文件
  参数：
        INT16U  file_id     文件
        INT32U  max_size    截至长度
  返回：
        INT8U 操作结果
  描述：
       1）如果文件长度小于max_size,则不截断，也不扩展。
       2）需要事务控制
       3)
---*/
INT8U  file_trim(INT16U file_id,INT32U max_size)
{
   INT32U file_size;
   INT32U fmAddr;
   INT16U idx,count;
   INT16U cluster;
   INT16U prev_cluster;       //分组删除的前组最末的簇号
   INT16U cluster_seq;
   INT16U fat_clusters[64];
   INT16U page_max_size;
   INT16U page;
   INT16U newPage;
   FDT_ENTRY  fdt_entry;
   FAT_ENTRY fat_entry;
   CLUSTER_HEAD cluster_head;
   INT8U trans_ctrl[2];
   FAT14_FILE file;
   INT8U result;
   INT8U try_count;

   try_count = 0;

   //不存在的文件,直接返回成功!
   if(file_id > MAX_FDT_COUNT) return FILE_ERROR_FILE_ID;
   if(file_exist(file_id)==FALSE) return  SUCCESS;

   //文件最大尺寸小于一个簇，也直接返回成功！
   //if(max_size <= OS_FAT14.per_cluster_data_size) return SUCCESS;

   //请求操作信号量
   //OSMutexPend(&OS_FAT14.SIGNAL);
     OSMutexPend(&SIGNAL_FLASHSPI);

   //事务控制类别
   write_fmByte(FMADDR_TRANSCTRL+2,FILE_TRANSCTRL_TRIM_FILE);
   //事务控制参数
   write_fmArray(FMADDR_TRANSCTRL+3,(INT8U *)&file_id,sizeof(INT16U));
   write_fmArray(FMADDR_TRANSCTRL+5,(INT8U *)&max_size,sizeof(INT32U));
   //事务启动标志
   trans_ctrl[0] = 0x55;
   trans_ctrl[1] = 0xAA;
   write_fmArray(FMADDR_TRANSCTRL,trans_ctrl,2);

   fmAddr =  FMADDR_FDT + file_id*sizeof(FDT_ENTRY);
   read_fmArray(fmAddr,(INT8U *)&fdt_entry,sizeof(FDT_ENTRY));

   //为了提高效率,采用每次调入最后64个簇的方法
   do{

      //调入文件的最后64簇号
      cluster = fdt_entry.first_cluster;
      for(idx=0;idx<64;idx++) fat_clusters[idx]=INVALID_CLUSTER;
      idx=0;
      file_size=0;
      prev_cluster = cluster;
      while(cluster!=INVALID_CLUSTER)
      {
          file_size += OS_FAT14.per_cluster_data_size;
          if(idx>=64)
          {
            prev_cluster = fat_clusters[63];
            idx=0;
          }
          fat_clusters[idx++] = cluster;
          cluster = file_next_cluster(cluster);
      }


      //依次释放调入的后面的簇
      while(idx>0)
      {
          //继续
          idx--;

          //读取簇信息
          fmAddr =  FMADDR_FAT14+fat_clusters[idx]*sizeof(FAT_ENTRY);
          read_fmArray(fmAddr,(INT8U *)&fat_entry,sizeof(FAT_ENTRY));

          //更新文件尺寸
          file_size -= OS_FAT14.per_cluster_data_size;

          //判断是否已经满足要求了
          if(file_size <= max_size)
          {
              //设置文件结束簇，
              fat_entry.cluster = FILE_END_CLUSTER;
              write_fmArray(fmAddr,(INT8U *)&fat_entry,sizeof(FAT_ENTRY));
              break;
          }


          //释放当前簇
          fat_entry.cluster = FILE_FREE_CLUSTER;
          write_fmArray(fmAddr,(INT8U *)&fat_entry,sizeof(FAT_ENTRY));



          //需要分组删除时，需要把前组的最末簇设置为最后的簇
          if( (idx==0) && (prev_cluster != fdt_entry.first_cluster))
          {
              fmAddr =  FMADDR_FAT14+prev_cluster*sizeof(FAT_ENTRY);
              read_fmArray(fmAddr,(INT8U *)&fat_entry,sizeof(FAT_ENTRY));
              fat_entry.cluster = FILE_END_CLUSTER;
              write_fmArray(fmAddr,(INT8U *)&fat_entry,sizeof(FAT_ENTRY));
          }
      }

   }while(file_size > max_size);

      //更新最后一个簇，超出max_size大小部分为ff，方法为换页写入
      if(file_size+OS_FAT14.per_cluster_data_size>max_size)
      {
            page_max_size=max_size-file_size;
            result = file_open(file_id,&file);
            if(SUCCESS == result)
            {
               page = cluster_to_page(fat_clusters[idx]);
               PAGE_WRITE_AGAIN:
               //不能更新，必须换页面进行擦写
               cluster = get_a_free_cluster(file.open_file_seq);
               if((cluster != INVALID_CLUSTER) && (fat_clusters[idx]!=INVALID_CLUSTER))
               {
                 newPage = cluster_to_page(cluster);
                 cluster_head.file_id = file.fdt_entry.file_id;
                 cluster_head.cluster_seq = max_size/OS_FAT14.per_cluster_data_size;
                 cluster_head.stamp_id = ++ (file.fdt_entry.stamp_id);
                 if(0x8000==ftrim_flashArray(page,page_max_size,newPage,&cluster_head))
                 {
					//更新FAT为无效簇
                    fmAddr = FMADDR_FAT14 + cluster * sizeof(FAT_ENTRY);
                    fat_entry.cluster = INVALID_CLUSTER;
                    fat_entry.write_count =3;
                    write_fmArray(fmAddr,(INT8U *)&fat_entry,sizeof(FAT_ENTRY));
//                    record_sys_operate(OP_SYS_FLASH_PAGE_BAD);
//                    record_sys_operate(newPage | LOG_RECORD_MASK1);
                    if(try_count++>16)//换了3页都有问题，则复位集中器
                    {
					    //担心由于数据问题导致总是换页写入，还无法写成功，因此尝试16页后，完成事务
					    trans_ctrl[0]=0x66;
					    trans_ctrl[1]=0x99;
					    write_fmArray(FMADDR_TRANSCTRL,trans_ctrl,2);
                        switch_off_flash();
                        os_SoftReset();
                    }
                    goto PAGE_WRITE_AGAIN;
                 }
                 if(idx==0)
                 {
                     if(prev_cluster == fdt_entry.first_cluster)
                     {
                        file.prev_cluster=INVALID_CLUSTER;
                     }
                     else
                     {
                        file.prev_cluster=prev_cluster;
                     }
                 }
                 else
                 {
                    file.prev_cluster=fat_clusters[idx-1];
                 }
                 file.cur_cluster = cluster;
                 file.free_cluster = fat_clusters[idx];

                 if(file.prev_cluster!=INVALID_CLUSTER)
                 {
                    //更新FAT
                    fmAddr = FMADDR_FAT14 + file.prev_cluster * sizeof(FAT_ENTRY);
                    read_fmArray(fmAddr,(INT8U *)&fat_entry,sizeof(FAT_ENTRY));
                    fat_entry.cluster = file.cur_cluster;
                    write_fmArray(fmAddr,(INT8U *)&fat_entry,sizeof(FAT_ENTRY));
                 }
                 else
                 {
                    //更新FDT
                   file.fdt_entry.first_cluster = file.cur_cluster;
                   fmAddr = FMADDR_FDT + file.fdt_entry.file_id*sizeof(FDT_ENTRY);
                   write_fmArray(fmAddr,(INT8U *)&(file.fdt_entry),sizeof(FDT_ENTRY));
                 }

                //更新当前簇擦写次数
                fmAddr = FMADDR_FAT14 + file.cur_cluster * sizeof(FAT_ENTRY);
                read_fmArray(fmAddr,(INT8U *)&fat_entry,sizeof(FAT_ENTRY));
                if(fat_entry.write_count < MAX_CLUSTER_WRITE_LOOP) fat_entry.write_count++;
                fat_entry.cluster = FILE_END_CLUSTER;
                write_fmArray(fmAddr,(INT8U *)&fat_entry,sizeof(FAT_ENTRY));


                //释放空闲簇
                fmAddr =  FMADDR_FAT14 + file.free_cluster * sizeof(FAT_ENTRY);
                read_fmArray(fmAddr,(INT8U *)&fat_entry,sizeof(FAT_ENTRY));
                fat_entry.cluster = FILE_FREE_CLUSTER;
                write_fmArray(fmAddr,(INT8U *)&fat_entry,sizeof(FAT_ENTRY));

                
               }
            }
            file_close(&file);
     }


   //事务完成标志
   trans_ctrl[0]=0x66;
   trans_ctrl[1]=0x99;
   write_fmArray(FMADDR_TRANSCTRL,trans_ctrl,2);

   //释放信号量
   //OSMutexFree(&OS_FAT14.SIGNAL);
     OSMutexFree(&SIGNAL_FLASHSPI);

   return SUCCESS;
}



/*+++
  功能: 把簇转换为FLASH的页面地址
  参数:
        INT16U cluster  簇号
  返回:
        页面号
  描述：
---*/
INT16U  cluster_to_page(INT16U cluster)
{
    cluster += FAT14_PAGE_OFFSET;
    return cluster;
}

/*+++
  功能：更新文件簇的头信息
  参数：
        FAT14_FILE *file      文件句柄
  返回：
        无
  描述：
        是否需要事务控制：暂时不需要，这个过程不会影响FAT，FDT
---*/
BOOLEAN file_update_cluster_head(FAT14_FILE *file)
{
   INT32U addr;
   CLUSTER_HEAD  cluster_head;

   addr = cluster_to_page(file->cur_cluster);
   addr *= OS_FAT14.per_cluster_size;
   cluster_head.file_id = file->fdt_entry.file_id;
   cluster_head.cluster_seq = file->cluster_seq;
   cluster_head.stamp_id = ++ (file->fdt_entry.stamp_id);
   return erase_write_pagehead(addr,(INT8U *)&cluster_head,sizeof(CLUSTER_HEAD));

}


/*+++
  功能: 写文件
  参数:
        FAT14_FILE *file,              [输入]文件句柄
        INT8U *data,                   [输入]数据区
        INT16U datalen                 [输入]数据区长度
  返回:
        操作结果
---*/
INT8U  file_write(FAT14_FILE *file,INT8U *data,INT16U datalen)
{
   INT16U page,page_offset;
   INT16U newPage;
   INT16U write_len;
   INT16U cluster,old_cluster;
   INT32U offset,fmAddr;
   CLUSTER_HEAD  cluster_head;
   FAT_ENTRY fat_entry;
   INT8U try_count;

   if(data == NULL) return FILE_NULL_BUFFER;

   file->operate = FILE_OP_WRITE;
   
   while(datalen > 0)
   {
      page = cluster_to_page(file->cur_cluster);
      page_offset = file->cluster_offset + sizeof(CLUSTER_HEAD);
      try_count = 0;
     
      //首先按照不更改页面进行覆盖写
      write_len = fupdate_flashArray(page,page_offset,data,datalen);
      if(write_len & 0x8000)
      {
PAGE_WRITE_AGAIN:    
         try_count++;   
         //不能更新，必须换页面进行擦写
         cluster = get_a_free_cluster(file->open_file_seq);
         if(cluster == INVALID_CLUSTER) return FILE_NO_SPACE;
         newPage = cluster_to_page(cluster);
         cluster_head.file_id = file->fdt_entry.file_id;
         cluster_head.cluster_seq = file->cluster_seq;
         cluster_head.stamp_id = ++ (file->fdt_entry.stamp_id);
         write_len = fwrite_flashArray(page,page_offset,newPage,data,datalen,&cluster_head);
         if(write_len == 0x8000)
         {
			//更新FAT为无效簇
            fmAddr = FMADDR_FAT14 + cluster * sizeof(FAT_ENTRY);
            fat_entry.cluster = INVALID_CLUSTER;
            fat_entry.write_count =3;
            write_fmArray(fmAddr,(INT8U *)&fat_entry,sizeof(FAT_ENTRY));
//            record_sys_operate(OP_SYS_FLASH_PAGE_BAD);
//            record_sys_operate(newPage | LOG_RECORD_MASK1);

			if(try_count==15)
			{
			      //	nop();
			}
            if(try_count>16)//换了16页都有问题，则复位集中器
            {
			    //	system_debug_info("will to reset");
                switch_off_flash();
                os_SoftReset();
                break;     
            }
            goto PAGE_WRITE_AGAIN;
         }
         datalen -= write_len;
         data += write_len;

         //修改FAT表
         old_cluster = file->cur_cluster;
         file->cur_cluster = cluster;

         file->free_cluster = old_cluster;
         file_update_fat14(file);
      } 
      else
      {
         datalen -= write_len;
         data += write_len;
      }


      //文件当前指针前进
      offset = file->offset + write_len;
      file_seek(file,offset);
   }
   return SUCCESS;
}


/*+++
  功能: 读文件
  参数:
        FAT14_FILE *file,              [输入]文件句柄
        INT8U *data,                   [输入出]数据区
        INT16U datalen                 [输入]数据区长度
  返回:
        操作结果
---*/
INT8U  file_read(FAT14_FILE *file,INT8U *data,INT16U datalen)
{
   INT16U page,page_offset;
   INT16U read_len;
   INT32U offset;

   if(data == NULL) return FILE_NULL_BUFFER;

   file->operate = FILE_OP_READ;
   while(datalen > 0)
   {
      page = cluster_to_page(file->cur_cluster);
      page_offset = file->cluster_offset + sizeof(CLUSTER_HEAD);
      read_len = fread_flashArray(page,page_offset,data,datalen);
      if(read_len == 0x8000) //出现返回数据长度0x8000错误时直接返回，避免继续执行会复位
      {
      	mem_set(data,datalen,0xFF);
      	return SUCCESS;		
      }
      datalen -= read_len;
      data += read_len;

      //文件当前指针前进
      offset = file->offset + read_len;
	  
	  //如果到了文件末尾，返回默认值
      if( file_seek(file,offset) == FILE_EOF)
	  {
	     #ifdef __SGRID__
         mem_set(data,datalen,0xEE);
         #else
         mem_set(data,datalen,0xFF);
         #endif   
         break;		 
	  }
   }
   return SUCCESS;
}





/*+++
  功能：文件系统FAT14的完整性检查
  参数：
         无
  返回：
         检查结果
  描述：
        1）簇引用检查
        2）文件完整性检查
---*/
INT8U  FAT14_Integrity_Check(void)
{
   FAT14_file_check();

   FAT14_cluster_check();

   return 0;
}

/*+++
  功能：文件系统FAT14的完整性检查:簇应用状况检查
  参数：
         无
  返回：
         检查结果
  描述：
        1）每次检查100个簇
---*/

void  FAT14_cluster_check(void)
{
   INT32U fmAddr;
   INT16U file_id;
   INT16U idx;
   INT16U cur_clusters;   //当前处理的起始簇号
   INT16U cluster_count;  //当前需要检查的簇数量
   INT16U file_cur_cluster;
   FAT_ENTRY fat_entry;
   FDT_ENTRY fdt_entry;
   INT16U fat_clusters[100];
   INT8U     fat_entry_usage[100];

   cur_clusters = 0;
   cluster_count = 0; 
   while(OS_FAT14.max_fat14_clusters > cur_clusters)
   {
       //读取最多100个非空闲簇
       fmAddr = FMADDR_FAT14 + cur_clusters * sizeof(FAT_ENTRY);
       while(cur_clusters < OS_FAT14.max_fat14_clusters)
       {
          read_fmArray(fmAddr,(INT8U *)&fat_entry, sizeof(FAT_ENTRY));
          if(fat_entry.cluster != FILE_FREE_CLUSTER)
          {
             fat_clusters[cluster_count] = cur_clusters;
             fat_entry_usage[cluster_count] = 0;      //簇引用次数
             cluster_count ++;
             if(cluster_count >= 100) break;
          }
          cur_clusters ++;
          fmAddr += sizeof(FAT_ENTRY);
       }

       //遍历文件检查簇使用状态情况
       for(file_id=0; file_id < MAX_FDT_COUNT; file_id++)
       {
           //如果文件不存在,则掠过!
           if(FALSE == file_exist(file_id)) continue;

           //读取文件入口条目
           read_fmArray(FMADDR_FDT + file_id*sizeof(FDT_ENTRY),(INT8U *)&fdt_entry,sizeof(FDT_ENTRY));

           //遍历检查文件的全部簇
           file_cur_cluster = fdt_entry.first_cluster;
           while(INVALID_CLUSTER != file_cur_cluster)
           {
              //检查文件当前簇在提取的待检查簇中的情况
              for(idx=0;idx<100;idx++)
              {
                 if(fat_clusters[idx]==file_cur_cluster)
                 {
                    fat_entry_usage[idx]++;

                    //检查簇是否被多个文件引用,需要删除所有引用的文件
                    if(fat_entry_usage[idx] > 1)
                    {
                       //删除当前文件,并终止当前文件的簇检查
                       file_delete(file_id);
                       file_cur_cluster = INVALID_CLUSTER;
                       break;
                    }
                 }
              }
           }
       }

       //根据簇使用统计信息作出处理
       for(idx = 0; idx < cluster_count; idx++)
       {
           //引用次数为零,必须释放
           if(0 == fat_entry_usage[idx])
           {
                fmAddr = FMADDR_FAT14 + fat_clusters[idx] * sizeof(FAT_ENTRY);
                read_fmArray(fmAddr,(INT8U *)&fat_entry, sizeof(FAT_ENTRY));
                fat_entry.cluster = FILE_FREE_CLUSTER;
                safe_write_fmArray(fmAddr,(INT8U *)&fat_entry,sizeof(FAT_ENTRY));
           }
           //引用次数超过1的,在删除文件时已经被释放了.
       }
   }

}

/*+++
  功能：文件系统FAT14的完整性检查:文件簇信息检查
  参数：
         无
  返回：
         检查结果
  描述：
        1）检查文件FDT条目是否正确
        2) 检查文件的簇链是否正确.
---*/

void  FAT14_file_check(void)
{
   INT32U  fmAddr;
   INT16U  file_id;
   INT16U  page;
   FDT_ENTRY fdt_entry;
   CLUSTER_HEAD  cluster_head;

   fmAddr = FMADDR_FDT;
   for(file_id = 0; file_id < MAX_FDT_COUNT; file_id++)
   {
       read_fmArray(fmAddr,(INT8U *)&fdt_entry,sizeof(FDT_ENTRY));
       if(fdt_entry.file_id >= FILEID_NONE) continue;

       //文件ID有效,检查一致性
       if(fdt_entry.file_id != file_id)
       {
         fdt_entry.file_id = FILEID_NONE;
         write_fmArray(fmAddr,(INT8U *)&fdt_entry,sizeof(FDT_ENTRY));
       }

       //检查文件首簇与簇内容是否一致
       if(fdt_entry.first_cluster > OS_FAT14.max_fat14_clusters)
       {
          page =  cluster_to_page(fdt_entry.first_cluster);
          fread_flashArray(page,0,(INT8U *)&cluster_head,sizeof(CLUSTER_HEAD));
          if(cluster_head.file_id != fdt_entry.file_id)
          {
              //无效文件
              fdt_entry.file_id = FILEID_NONE;
              write_fmArray(fmAddr,(INT8U *)&fdt_entry,sizeof(FDT_ENTRY));
       }
       }

       fmAddr += sizeof(FDT_ENTRY);
   }
}


/*+++
 功能：写文件
 参数：
       INT16U file_id,   文件号
       INT32U addr,      偏移地址
       INT8U *data,      数据区
       INT16U len        数据长度
 返回:
       操作结果
 描述：
       1）如果文件打开忙，则继续等待直到打开
       2）打开文件的最大延时放到30秒
---*/

INT8U fwrite_array(INT16U file_id,INT32U addr,INT8U *data,INT16U len)
{
  FAT14_FILE file;
  INT8U result;

//  OS_give_rights_to_display_use_spi();

  if(addr > 0xA0000)
  {
     return 0;
  }
  
  //?????此处用做调试
  //if(file_id == 0)
  //{
  //    Nop();
  //    Nop();
  //} 
 
  //转换访问终端文件的ID，这样是为了能够做文件ID检查.
  //FILEID_ERTU_NAME只由 fwrite_ertu_params调用。
   if(file_id == FILEID_ERTU_NAME)
   {
      file_id = FILEID_ERTU;
   }


    //请求操作信号量
    OSMutexPend(&SIGNAL_FLASHSPI);

  //打开文件,如果返回错误忙，则继续等待
  while(TRUE)
  {
    result = file_open(file_id,&file);
    if(SUCCESS == result) break;
    if(FILE_IS_BUSY == result) 
    {
       OSMutexFree(&SIGNAL_FLASHSPI);
       DelayNmSec(200);
       OSMutexPend(&SIGNAL_FLASHSPI);
    } 
  }
  if(result != SUCCESS) 
  {
     OSMutexFree(&SIGNAL_FLASHSPI);  
     return result;
  }
  //定位
  file.operate = FILE_OP_WRITE;
  result = file_seek(&file,addr);
  if(result != SUCCESS)
  {
     file_close(&file);
     OSMutexFree(&SIGNAL_FLASHSPI);
     return result;
  }

  //写文件
  result = file_write(&file,data,len);

  file_close(&file);

  OSMutexFree(&SIGNAL_FLASHSPI);

  return result;
}
//INT8U fwrite_array_exec(INT16U file_id,INT32U addr,INT8U *data,INT16U len)
//{
//  INT8U result;
//  
//  OSMutexPend(&SIGNAL_FLASHSPI);
//
//  result = fwrite_array_data(file_id,addr,data,len);
//  #ifdef __IMPORTANT_PARAM_BACKUP_RESTORE__
//  //同步写入备份参数文件和档案文件
//  if((file_id == FILEID_ERTU)|| (file_id == FILEID_ERTU_NAME))
//  {
//        backup_ertu_param(addr,data,len,0);
//        backup_ertu_param(addr,data,len,1);
//  }
//  else if((file_id>=1) && (file_id<=MAX_METER_COUNT))
//  {
//        backup_meter_param(file_id,addr,data,len,0);
//        backup_meter_param(file_id,addr,data,len,1);
//  }
//  #endif
//  OSMutexFree(&SIGNAL_FLASHSPI);
//
//
//}

/*+++
 功能：读文件
 参数：
       INT16U file_id,   文件号
       INT32U addr,      偏移地址
       INT8U *data,      数据区
       INT16U len        数据长度
 返回:
       操作结果
 说明：
       不存在的文件，读取的数据填充0xFF
---*/
INT8U  fread_array(INT16U file_id,INT32U addr,INT8U *data,INT16U len)
{
  FAT14_FILE file;
  INT8U result;

  ClrTaskWdt();
  
   if(file_id == FILEID_ERTU_NAME)
   {
	   file_id = FILEID_ERTU;
   }

  if(file_id >= MAX_FDT_COUNT)
  {
     #ifdef __SGRID__
     mem_set(data,len,0xEE);
     #else
     mem_set(data,len,0xFF);
     #endif
     return SUCCESS;
  }

//  OS_give_rights_to_display_use_spi();

  if(addr > 0xA0000)
  {
     return 0;
  }

  OSMutexPend(&SIGNAL_FLASHSPI);

  if(FALSE == file_exist(file_id))
  {
     #ifdef __SGRID__
     mem_set(data,len,0xEE);
     #else
     mem_set(data,len,0xFF);
     #endif
     OSMutexFree(&SIGNAL_FLASHSPI);
     return SUCCESS;
  }

  //打开文件,如果返回错误忙，则继续等待
  while(TRUE)
  {
    result = file_open(file_id,&file);
    if(SUCCESS == result) break;
    if(FILE_IS_BUSY == result) 
    {
       OSMutexFree(&SIGNAL_FLASHSPI);
       DelayNmSec(200);
       OSMutexPend(&SIGNAL_FLASHSPI);
    } 
  }


  //定位
  file.operate = FILE_OP_READ;
  result = file_seek(&file,addr);
  if(result != SUCCESS)
  {
     file_close(&file);
     OSMutexFree(&SIGNAL_FLASHSPI);
     #ifdef __SGRID__
     mem_set(data,len,0xEE);
     #else
     mem_set(data,len,0xFF);
     #endif	 
     return result;
  }

  //读文件
  result = file_read(&file,data,len);

  file_close(&file);

  OSMutexFree(&SIGNAL_FLASHSPI);
  return len;
}

/*+++
 功能：读文件
 参数：
       INT16U file_id,   文件号
       INT32U addr,      偏移地址
       INT8U *data,      数据区
       INT16U len        数据长度
 返回:
       操作结果
 说明：
       不存在的文件，读取的数据填充0xFF
---*/
//INT8U  fread_array_exec(INT16U file_id,INT32U addr,INT8U *data,INT16U len)
//{
//  INT32U offset;
//  INT8U result;
//  INT8U idx;
//
//  OSMutexPend(&SIGNAL_FLASHSPI);
//  
//  result=fread_array_data(file_id,addr,data,len);
//    
//  #ifdef __IMPORTANT_PARAM_BACKUP_RESTORE__
//  INT16U datalen;
//  INT8U buf1[32];
//  INT8U buf2[32];
//  INT8U size,temp1,temp2,temp3;
//  BOOLEAN is_update,is_same;
//  static INT8U file_index =0;
//
//  //随机读取备份文件，防止因为长期不读，导致某备份文件损坏还不知道
//  if(++file_index>1)
//  {
//    file_index=0;
//  }
//  //使用三取二原则，读取参数文件，如果备份文件不存在，则不进行三取二
//  datalen=len;
//  offset =addr;
//  is_update = FALSE;
//  if(file_id == FILEID_ERTU)
//  {
//        while(len>0)
//        {
//            size=(len>32)?32:len;
//            read_backup_ertu_param(addr,buf1,size,file_index);
//			is_same= TRUE;
//			for(idx=0;idx<size;idx++)
//			{
//				if(buf1[idx]!=data[datalen-len+idx])
//				{
//					is_same =FALSE;
//					break;
//				}
//			}
//			if(!is_same)//前两个文件不相同才需要读取第三个文件
//			{
//            read_backup_ertu_param(addr,buf2,size,1-file_index);
//	            //三取二
//	            for(idx=0;idx<size;idx++)
//	            {
//	                temp1=buf1[idx];  
//	                temp2=buf2[idx];
//	                temp3=data[datalen-len+idx];  
//	                data[datalen-len+idx]=(temp1 & temp2) | (temp1 & temp3) | (temp2 & temp3);
//	                if((temp1!=temp2) || (temp1!=temp3) || (temp2!=temp3))
//	                {
//	                    is_update = TRUE;
//	
//	                }
//	            }
//			}
//            len-=size;
//            addr+=size;
//
//        }
//  }
//  else if((file_id>=1) && (file_id<=MAX_METER_COUNT))
//  {
//        if(FALSE == file_exist(file_id))
//        {
//			size = 32;
//            read_backup_meter_param(file_id,0,buf1,size,0);
//            read_backup_meter_param(file_id,0,buf2,size,1);
//            if(check_is_all_FF(buf1,size) || check_is_all_FF(buf2,size))
//            {
//  				OSMutexFree(&SIGNAL_FLASHSPI);
//                return result;
//            }
//        }
//        while(len>0)
//        {
//			//测量点文件需要注意，PIM_METER_CUR_DATA以内考虑3取2，否则直接不考虑
//			if(addr>=PIM_METER_CUR_DATA) break;
//            size=(len>32)?32:len;
//			if(addr+size>=PIM_METER_CUR_DATA)
//			{
//				size=PIM_METER_CUR_DATA-addr; 
//			}			
//            read_backup_meter_param(file_id,addr,buf1,size,file_index);
//			is_same= TRUE;
//			for(idx=0;idx<size;idx++)
//			{
//				if(buf1[idx]!=data[datalen-len+idx])
//				{
//					is_same =FALSE;
//					break;
//				}
//			}
//			if(!is_same)//前两个文件不相同才需要读取第三个文件
//			{
//            read_backup_meter_param(file_id,addr,buf2,size,1-file_index);
//	            //三取二
//	            for(idx=0;idx<size;idx++)
//	            {
//	                temp1=buf1[idx];  
//	                temp2=buf2[idx];
//	                temp3=data[datalen-len+idx];  
//	                data[datalen-len+idx]=(temp1 & temp2) | (temp1 & temp3) | (temp2 & temp3);
//                if((temp1!=temp2) || (temp1!=temp3) || (temp2!=temp3))
//	                {
//	                    is_update = TRUE;
//	
//	                }
//	            }
//			}
//            len-=size;
//            addr+=size;
//
//        }
//  }
//  if(is_update)
//  {
//        fwrite_array_exec(file_id,offset,data,datalen);
//        record_sys_operate(OP_SYS_FLASH_READ_ERROR);
//
//  }
//  #endif
//
//  OSMutexFree(&SIGNAL_FLASHSPI);
//
//  return result;
//}

/*+++
 功能：读文件,具备监控功能
 参数：
       INT16U file_id,   文件号
       INT32U addr,      偏移地址
       INT8U *data,      数据区
       INT16U len        数据长度
 返回:
       操作结果
 说明：
       不存在的文件，读取的数据填充0xFF
---*/
//INT8U  fread_array_monitor(INT16U file_id,INT32U addr,INT8U *data,INT16U len)
//{
////	如果读取的是测量点文件并且开启了测量点档案初始化事务，则需要先初始化该测量点
////	if((file_id>0) && (file_id<=MAX_METER_COUNT))
////	{
////		meter_doc_init(file_id);	
////	}
////	return fread_array_exec(file_id,addr,data,len);
// 	return 0;
//}
/*+++
 功能：写文件,具备监控功能
 参数：
       INT16U file_id,   文件号
       INT32U addr,      偏移地址
       INT8U *data,      数据区
       INT16U len        数据长度
 返回:
       操作结果
 描述：
       1）如果文件打开忙，则继续等待直到打开
       2）打开文件的最大延时放到30秒
---*/

//INT8U fwrite_array_monitor(INT16U file_id,INT32U addr,INT8U *data,INT16U len)
//{
////	如果读取的是测量点文件并且开启了测量点档案初始化事务，则需要先初始化该测量点
////	if((file_id>0) && (file_id<=MAX_METER_COUNT))
////	{
////		meter_doc_init(file_id);	
////	}
////	return fwrite_array_exec(file_id,addr,data,len);
//	return 0;
//}
/*+++
  功能：删除文件,具备监控功能
  参数：
        INT16U  file_id
  返回：
        操作结果
  描述：
       1）加入事务性处理，并且需要占用系统操作信号量
       2）自后向前依次删除每个簇，最后删除FDT
---*/
//INT8U  file_delete_monitor(INT16U file_id)
//{
////	INT8U ret;
////
////	ret = file_delete_exec(file_id);
////	//如果读取的是测量点文件并且开启了测量点档案初始化事务，则需要先初始化该测量点
////	if((file_id>0) && (file_id<=MAX_METER_COUNT))
////	{
////		meter_doc_init_status_clear(file_id);	
////	}
////	return ret;
//	return 0;
//}

//测试，没有所有文件系统使用页，查找关系页，打印页头信息
/*
INT16U  printfile_pagehead(INT16U file_id,INT8U *buffer)
{
	INT16U idx,no,datalen;
    INT8U head[6];

    datalen  = 0;
	for(idx=0;idx<OS_FAT14.max_fat14_clusters;idx++)
	{
            fread_flashArray(idx,0,head,6);
    		no=bin2_int16u(head);//获取文件id
			if(no==file_id)
			{
			mem_cpy(buffer+datalen,head,6);
			datalen+=6;
			if(datalen>900)break;
 		 	}	




	}
	return datalen;


}
*/
