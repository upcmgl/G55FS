/*
   ������2Ƭ8M NADN FLASH�ļ��ļ�ϵͳ

   1��һ���ش�СΪ1056��4096�ֽڣ�ǡ����һ��ҳ��,����AT45DB642D,ҳ����1056��MX25��4096�ֽڡ�
   2�������ô���Ϊ8192*2=16384��������14λ��ʾһ����,���ҳ����4096�����������Ϊ8192������8192����Ϣ��ַ�������á�
   3��FATʹ��2�ֽڱ�ʾ�ص�ʹ����Ϣ����2λ��ʾ���ز�д������������ĥ��ƽ�⡣
   4���ļ���ʹ��ID��ʾ
   5) �ļ���֧�ֹ�����һ���ļ����ܱ�ͬʱ�򿪡�
   6) һ������дʱ���������������ж��Ƿ���Ҫд�����ǲ�д������ǿ���д�����ø����ء�
   7)�ļ�ϵͳ��ʼ��ʱȷ���ش�С����ҳ���С��

   ����������� ����д���������������������д�����־,��ɺ�,���������־
       �����־  2�ֽ�  55 AA  ��������   66 99  �������  ��������Ч
       �������  1�ֽ�
       ���������N�ֽ�  47�ֽ�

*/


// ��Ҫһ��������д����ĺ������ڵ�һ�Ĺؼ����ݸ�����������е��ã�������
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
  ���ܣ���ʼ��FAT14�ļ�ϵͳ
  ������
        ��
  ���أ�
        TRUE  ϵͳ����
        FALSE ϵͳ������
  ������
        1) ֻ���ڲ���ϵͳ����ʱ����,һ��������ÿ�������ж�����!
        2) ����������ļ�ϵͳ��ʽ�����򷵻�ֵ���и�ʽ����־ 

---*/
BOOLEAN  fat14_init(void)
{
    INT16U idx;
    INT8U  result;

    //�ļ�ϵͳ�����ź���
    //OS_FAT14.SIGNAL = 0;

    result = FALSE;

    //ȷ���豸���ļ�ϵͳ����Ĵ����ʹ�ҳ��ߴ硣
    //OS_FAT14.max_fat14_clusters = gFlash.chip1.page_max + gFlash.chip2.page_max - FAT14_PAGE_OFFSET;
	OS_FAT14.max_fat14_clusters = get_flash_page_count() - 512 - FLADDR_VFM_PAGE_OFFSET;	     //2K ��ҳ   �ļ�ϵͳʹ�õ�ҳ����
    OS_FAT14.per_cluster_size = SIZE_OF_CLUSTER;
    OS_FAT14.per_cluster_data_size =  OS_FAT14.per_cluster_size - sizeof(CLUSTER_HEAD);

    //2010-09-20 FAT14ϵͳʹ�õĴ�����һ�����ܳ���16384,�ӵڶ�ƬFLASH�Ͽ���4��PAGE��
    if(OS_FAT14.max_fat14_clusters >= MAX_FAT14_CLUSTER)
    {
        OS_FAT14.max_fat14_clusters = MAX_FAT14_CLUSTER - 4;
    }

    //����ļ�ϵͳ������
    if(fat14_checkflag() == FALSE)
    {
        fat14_format(TRUE);
//        record_sys_operate(OP_SYS_FAT_FORMAT);
        result |= FORMAT;
        if(fat14_checkflag() == FALSE) return FALSE;
    }
    //����ļ�ϵͳ�汾����ͬ�汾�������ݰ汾����Ӧ����
    fat14_var_check();

    //��ʼ�����ļ���Ϣ
    OS_FAT14.open_file_count = 0;
    for(idx=0;idx<MAX_OPEN_FILE;idx++)
    {
      OS_FAT14.opened_file_id[idx] = NONE_FILE_ID;
      OS_FAT14.in_using_free_clusters[idx]=INVALID_CLUSTER;
    }
    //��ʼ����ʷ�����б�
    for(idx=0;idx<MAX_HIS_FREE_CLUSTER;idx++)
    {

       OS_FAT14.his_free_cluster[idx] = INVALID_CLUSTER;
    }

    //ϵͳ��ǰ���д���
    OS_FAT14.free_cluster_count = system_free_cluster_count();


    //ȷ����ǰ��С�ز�д�ִ���
    detect_min_cluster_write_count();

    //Ԥ����ȡ���д�
    fat14_prefetch_free_clusters();


    //����������
    check_file_transctrl();


    //�ļ�ϵͳ�����Լ��
   // FAT14_Integrity_Check();

    result |= TRUE;
    return result;
}
/*+++
  ����:����ļ�ϵͳ�汾����ͬ�汾�������ݰ汾����Ӧ����
  ����
       ��
  ����:
       ��
  ����:
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
//   �״�ʹ�ð汾���״����ӽ��๦�ܱ�FLASH�����ļ�ϵͳ�����У������Ҫ��ʼ�������FAT��λ���д�
//   if(((0xFF==buf[0]) && (0xFF==buf[1])) || ((0x00==buf[0]) && (0x00==buf[1])))
//   {
//        entry.write_count = 0;
//        entry.cluster = FILE_FREE_CLUSTER;
//        for(idx=gFlash.chip1.page_max + gFlash.chip2.page_max;idx<OS_FAT14.max_fat14_clusters;idx++)
//        {
//
//            ��ʼ��FAT��
//            fmAddr = FMADDR_FAT14+idx*sizeof(FAT_ENTRY);
//            write_fmArray(fmAddr,(INT8U *)&entry,sizeof(FAT_ENTRY));
//        }
//        buf[0]=FAT_MAJOR_VER;
//        buf[1]=FAT_MINOR_VER;
//        write_fmArray(FMADDR_TRANSCTRL+44,buf,2);
//   }


}
/*+++
  ����:����ļ�ϵͳ�������,��ɽ����е�����
  ����
       ��
  ����:
       ��
  ����:
        1)�����ڳ�ʼ���ļ�ϵͳʱ������.
---*/
void  check_file_transctrl(void)
{
   INT32U addr;
   INT32U max_size;
   INT16U len;
   FAT14_FILE file;
   INT8U  transctrl_flag[2];  //������Ʊ�־
   INT8U  transctrl_class;    //�������
   INT8U  data[50];

   //��ȡ������Ʊ�־
   read_fmArray(FMADDR_TRANSCTRL,transctrl_flag,2);
   if( (transctrl_flag[0]!=0x55)  || (transctrl_flag[1]!=0xAA) ) return;


   //��ȡ����������
   transctrl_class = read_fmByte(FMADDR_TRANSCTRL+2);

   switch(transctrl_class)
   {
   case FILE_TRANSCTRL_UPDATE_FATFDT:
        //����FAT��FDT
        read_fmArray(FMADDR_TRANSCTRL+3,(INT8U *)&file,sizeof(FAT14_FILE));
        file_update_fat14(&file);
        break;
   case FILE_TRANSCTRL_FMWRITE:
        //д�ؼ����ݵ�����
        read_fmArray(FMADDR_TRANSCTRL+3,(INT8U *)&addr,sizeof(INT16U));
        len = read_fmByte(FMADDR_TRANSCTRL+5);
        read_fmArray(FMADDR_TRANSCTRL+6,data,len);
        safe_write_fmArray(addr,data,len);
        break;
   case FILE_TRANSCTRL_DELETE_FILE:
        //��ȡδɾ����ɵ��ļ�ID��addr,Ȼ�����ɾ��
        read_fmArray(FMADDR_TRANSCTRL+3,(INT8U *)&addr,sizeof(INT16U));
        file_delete(addr);
        break;
   case FILE_TRANSCTRL_TRIM_FILE:
        //�ض��ļ�
        read_fmArray(FMADDR_TRANSCTRL+3,(INT8U *)&addr,sizeof(INT16U));
        read_fmArray(FMADDR_TRANSCTRL+5,(INT8U *)&max_size,sizeof(INT32U));
        file_trim(addr,max_size);
        break;
   default:
        break;
   }

   //������Ʊ�־����
   transctrl_flag[0]=0;
   transctrl_flag[1]=0;
   write_fmArray(FMADDR_TRANSCTRL,transctrl_flag,2);
}


/*+++
  ���ܣ�����ļ�ϵͳ������
  ������
        ��
  ���أ�
        TRUE  ������OK
        FALSE ������ERR
  ������
       ������ǰ5���ֽڱ�ʾ�ļ�ϵͳ������ FAT14
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
         //��ȡ�ļ�ϵͳ������
         read_fmArray(FMADDR_FAT_FLAG,buffer,5);
         if(0 == compare_string(fat_14,buffer,5))   return TRUE;
      }
      return FALSE;

}


/*+++
  ���ܣ���ʽ���ļ�ϵͳ
  ����:
        BOOLEAN  quick_format   �Ƿ���ٸ�ʽ��
  ����:
        ��

  ����:
       ���MAX_FAT14_CLUSTER
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

    //��ʼ��FAT��
    fmAddr = FMADDR_FAT14;
    for(idx = 0; idx < OS_FAT14.max_fat14_clusters; idx +=  16)
    {
        write_fmArray(fmAddr,(INT8U *)entrys,sizeof(FAT_ENTRY)*16);
        fmAddr += sizeof(FAT_ENTRY)*16;
    }

    //��ʼ����Ŀ¼FDT,ʹ��0xFF���
    set_fmArray(FMADDR_FDT,0xFF,OS_LEN_FDT);

    //��ʼ����������������50�ֽ�
    set_fmArray(FMADDR_TRANSCTRL,0x00,50);
    
//    set_fmArray(FMADDR_PLC_NODE_COUNT,0x00,2);

    //д�ļ�ϵͳ�汾��
    tmp[0]=FAT_MAJOR_VER;
    tmp[1]=FAT_MINOR_VER;
    write_fmArray(FMADDR_TRANSCTRL+44,tmp,2);
    
    //д�ļ�ϵͳ��ʶ
    tmp[0]='F';
    tmp[1]='A';
    tmp[2]='T';
    tmp[3]='1';
    tmp[4]='4';
    write_fmArray(FMADDR_FAT_FLAG,tmp,5);

    //����ǿ��ٸ�ʽ�������ô���NAND_FLASH������
    if(TRUE == quick_format) 
    {
       OSMutexFree(&SIGNAL_FLASHSPI);
       return;
    }

    //����NAND_FLASH����
    //����ҳ������ʽ����ļ�ϵͳҳ��
    for(idx = 0; idx < OS_FAT14.max_fat14_clusters; idx++)
    {
        //ת���غ�ΪNAND FLASH��Ӧ��ҳ���ַfmAddr
        fmAddr = cluster_to_page(idx);          
        os_flash_erase_page(fmAddr); //Ŀǰ����ִ�У�Ӧ�ÿ�����ƬFLASHת��
    }
   OSMutexFree(&SIGNAL_FLASHSPI);
}


/*+++
  ���ܣ�ȷ����ǰ��С�ز�д�ִ���
  ������
        ��
  ���أ�
        ��
  �޸ģ�
        OS_FAT.min_cluster_write_count
---*/
void detect_min_cluster_write_count(void)
{
    INT32U fmAddr;
    INT32U tmp_u32;
    FAT_ENTRY   fat_entry[16];  //16�պ���MAX_FAT14_CLUSTER������
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
          //�����Ƿ����
          cluster_count ++;
          if(cluster_count  > OS_FAT14.max_fat14_clusters) break; //�������ǰ��ִ�С�
          if(FILE_FREE_CLUSTER != fat_entry[idx].cluster) continue;
          if(fat_entry[idx].write_count < OS_FAT14.min_cluster_write_count)
          {
             OS_FAT14.min_cluster_write_count = fat_entry[idx].write_count;
          }
          //����ҵ���СֵΪ0������Բ����ټ���ˣ�
          if(OS_FAT14.min_cluster_write_count == 0) return;
       }
       fmAddr += len;
       //cluster_count += 16;
    } while(cluster_count<OS_FAT14.max_fat14_clusters);



    if(OS_FAT14.min_cluster_write_count < MAX_CLUSTER_WRITE_LOOP) return;

    //�����С��д�ִε���MAX_CLUSTER_WRITE_LOOP��
    //����Ҫ�����п��дصĲ�д�ִθ�λΪ0
    
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
          //�����Ƿ����
          cluster_count ++;
          if(cluster_count  > OS_FAT14.max_fat14_clusters) break; //�������ǰ��ִ�С�
          if(FILE_FREE_CLUSTER != fat_entry[idx].cluster) continue;

          //��д�ִθ�λΪ0
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
  ���ܣ���FAT������ȡ���д�
  ������
        ��
  ���أ�
        ��
  �޸ģ�
        OS_FAT.free_clusters_count
        OS_FAT.free_clusters

---*/

void  fat14_prefetch_free_clusters(void)
{
    INT32U fmAddr;
    FAT_ENTRY   fat_entry[16];  //16�պ���MAX_FAT14_CLUSTER������
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
          //�����Ƿ����
          cluster_count ++;
          if(cluster_count  > OS_FAT14.max_fat14_clusters) break; //�������ǰ��ִ�С�
          if(FILE_FREE_CLUSTER != fat_entry[idx].cluster) continue;

          OS_FAT14.free_cluster_count++;
          if(fat_entry[idx].write_count == OS_FAT14.min_cluster_write_count)
          {
              cluster = cluster_count-1;// + idx;

              //�����Ƿ��ڵ�ǰ������ռ�ö�����
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

    //�����ȡ���Ŀ��д���Ϊ0�����ҿ��дش���0����ִ������̽����Сֵ
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
  ����:��ȡһ�����д�
  ����:
        INT8U open_file_seq   �ļ��򿪵����,ָϵͳ��ͬʱ�򿪵��ļ�.
  ����:
        ���д����
  ����:
       1)��ռ��Դ����,��Ҫʹ���ļ�ϵͳ�ź���
       2)ֱ�ӵ��÷���
             ������ȡ���д�: fat14_prefetch_free_clusters
       3)��ӵ��÷���
             ȷ����ǰ��С�ز�д�ִ���: detect_min_cluster_write_count

---*/
INT16U  get_a_free_cluster(INT8U open_file_seq)
{
     INT16U  free_cluster;
     INT8U   idx,i;
     BOOLEAN is_fetch;

     free_cluster = INVALID_CLUSTER;

     if(open_file_seq >= MAX_OPEN_FILE) return INVALID_CLUSTER;

      //��������ź���
     //OSMutexPend(&OS_FAT14.SIGNAL);
     OSMutexPend(&SIGNAL_FLASHSPI);
     if(OS_FAT14.free_clusters_count <= 0)
     {
        //������ȡ���д�
        fat14_prefetch_free_clusters();
     }
     free_cluster  = INVALID_CLUSTER;
     is_fetch      = TRUE;
     //���û����ȡ��,˵��û�п��д���
     if(OS_FAT14.free_clusters_count > 0)
     {
        GET_FREE_CLUSTER:
        //��ȡ��ǰ��ͬBLOCK�Ŀ��д�
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
            //����ǰ��ȡ���Ķ�����ͬ
            if(i==MAX_HIS_FREE_CLUSTER)
            {
                free_cluster = OS_FAT14.free_clusters[idx];
                OS_FAT14.free_clusters[idx] = INVALID_CLUSTER;
                //��������free_clusters�б�
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
                //�洢��ʷ���п�
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
        //������ȡ���д�
        fat14_prefetch_free_clusters();
            goto GET_FREE_CLUSTER;

        }
        //����Ѿ���ȡ�Ŀ��дػ��޷����㣬�ÿ��п鲻����2�ο��п��block�У���˳����ȡ��
        if((free_cluster == INVALID_CLUSTER) && !is_fetch)
        {

           free_cluster = OS_FAT14.free_clusters[--OS_FAT14.free_clusters_count];
           OS_FAT14.in_using_free_clusters[open_file_seq] = free_cluster;
                //�洢��ʷ���п�
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
  ���ܣ���ȡϵͳ��ǰ���д���
  ������
         ��
  ���أ�
         INT16U

----*/
INT16U  system_free_cluster_count(void)
{
    INT32U fmAddr;
    FAT_ENTRY   fat_entry[16];  //16�պ���MAX_FAT14_CLUSTER������
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
          //�����Ƿ����
          cluster_count ++;
          if(cluster_count  > OS_FAT14.max_fat14_clusters) break;  //�������ǰ��ִ�С�
          if(FILE_FREE_CLUSTER != fat_entry[idx].cluster) continue;
          free_cluster_count++;
       }
       fmAddr += len;
       //cluster_count += 16;
    } while(cluster_count<OS_FAT14.max_fat14_clusters);

    return free_cluster_count;
}

/*+++
  ���ܣ������ļ�
  ����:
        INT16U file_id
  ����:
        TRUE  �ɹ�
        FALSE ʧ��
---*/
INT8U  file_create(INT16U file_id)
{
    FDT_ENTRY  fdt_entry;
    INT32U  fmAddr;

    if(file_id >= MAX_FDT_COUNT) return FILE_ERROR_FILE_ID;

    fmAddr = FMADDR_FDT + file_id*sizeof(FDT_ENTRY);

    read_fmArray(fmAddr,(unsigned char *)&fdt_entry,sizeof(FDT_ENTRY));

    //�ļ��Ѿ������ˣ�
    if(fdt_entry.file_id == file_id) return SUCCESS;

    //�´����ļ�
    fdt_entry.file_id = file_id;
    fdt_entry.stamp_id = 0;
    fdt_entry.first_cluster = INVALID_CLUSTER;

    //��������Ʒ�ʽ����FDT
    safe_write_fmArray(fmAddr,(unsigned char *)&fdt_entry,sizeof(FDT_ENTRY));

    return SUCCESS;
}


/*+++
  ����:��ȡ��һ���غ�
  ����:
       INT16U cur_cluster ��ǰ�غ�
  ����:
       ��һ���غ�
---*/
INT16U  file_next_cluster(INT16U cur_cluster)
{
    INT32U    fmAddr;
    FAT_ENTRY  fat_entry;

    if(INVALID_CLUSTER == cur_cluster) return  INVALID_CLUSTER;
    if(cur_cluster >= OS_FAT14.max_fat14_clusters) return INVALID_CLUSTER;
     
    fmAddr = FMADDR_FAT14 + cur_cluster * sizeof(FAT_ENTRY);
    read_fmArray(fmAddr,(INT8U *)&fat_entry,sizeof(FAT_ENTRY));

    //�����һ����Ϊ������,�򷵻���Ч��
    if(FILE_END_CLUSTER == fat_entry.cluster) return INVALID_CLUSTER;

    //�����һ����Ϊ���д�,Ҳ����Ч��
    if(FILE_FREE_CLUSTER == fat_entry.cluster) return INVALID_CLUSTER;

    return fat_entry.cluster;
}


/*+++
  ���ܣ���ȡǰһ���غ�
  ������
        INT16U first_cluster,    �ļ��״غ�
        INT16U cur_cluster       ��ǰ�غ�
  ���أ�
        ǰһ���غ�
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
  ����:  ���ļ�
  ����:
        INT16U file_id,       [����]  �ļ���
        FAT14_FILE *file      [���]  �ļ����
  ����:
        �������
  ����:
        1)�ǼǴ򿪵��ļ�ʱ��Ҫʹ���ź���
        2)�ļ�����ͬʱ���򿪣�Ҳ����˵�ļ���֧�ֹ���
        3)���30����Ȼ���ܴ��ļ��������ļ�æ����
---*/
INT8U file_open(INT16U file_id,FAT14_FILE *file)
{
    INT32U  fmAddr;
    INT8U   result;
    INT8U   open_file_seq;  //���Ѵ��ļ������е����

    if(OS_FAT14.open_file_count >= MAX_OPEN_FILE) return FILE_MAX_OPEN_REACHED;
    if(file_id >= MAX_FDT_COUNT) return FILE_ERROR_FILE_ID;

    //
    //�ǼǴ򿪵��ļ�
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
          //�ļ��Ѿ����ˣ���Ҫ�ȴ�
          return FILE_IS_BUSY;
       } 
    }
    OS_FAT14.opened_file_id[open_file_seq] = file_id;
    OS_FAT14.open_file_count++;
    file->open_file_seq = open_file_seq;

    
    //��ȡ�ļ�FDT
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


    //�ļ��򿪺�,��ǰλ������ʼλ��!
    //�����ڵǼǴ��ļ������
    file->prev_cluster = INVALID_CLUSTER;
    file->cur_cluster = file->fdt_entry.first_cluster;
    file->cluster_seq = (file->cur_cluster == INVALID_CLUSTER) ? INVLAID_CLUSTER_SEQ : 0;
    file->next_cluster = file_next_cluster(file->cur_cluster);
	file->operate = FILE_OP_READ;
    file_seek(file,0);

    return SUCCESS;
}


/*+++
  ���ܣ��ر��ļ�
  ������
         FAT14_FILE *file,              [����]�ļ����
  ����
        �������
  ����:
        1)�ǼǴ򿪵��ļ�ʱ��Ҫʹ���ź���
---*/
INT8U  file_close(FAT14_FILE *file)
{
   INT16U idx;

   //
   //�����ļ��򿪵ǼǱ�־
   //

     OSMutexPend(&SIGNAL_FLASHSPI);
    //��������ź���
   //OSMutexPend(&OS_FAT14.SIGNAL);
   OS_FAT14.opened_file_id[file->open_file_seq] = NONE_FILE_ID;
   OS_FAT14.open_file_count--;
   //�ͷ��ź���
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
  ����: �ļ���λ,����ʼλ������
  ����:
        FAT14_FILE *file,              [����]�ļ����
        INT32U  offset,                [����]�ļ���ƫ��λ��,���ļ�ͷ����
  ����:
        �������
  ����:
        ��Ҫ��offsetת��ΪFLASH��ַ


---*/
INT8U file_seek(FAT14_FILE *file,INT32U offset)
{
     INT32U  cluster;

     //��ǰλ�����ڵĴ����
     cluster = offset / OS_FAT14.per_cluster_data_size;
     file->offset = offset;
     file->cluster_offset = offset - cluster*OS_FAT14.per_cluster_data_size;

     //�ƶ���ָ���Ĵ����
     return file_move_to_cluster_seq(file,cluster);
}


/*+++
  ����: �ƶ���ָ���Ĵ����
  ����:
         FAT14_FILE *file,
         INT16U cluster_seq

  ����:
         �������
  ����:
         ����ﵽ�ļ�ĩβ,��Ҫ����ռ�

---*/
INT8U  file_move_to_cluster_seq(FAT14_FILE *file,INT16U cluster_seq)
{
    if(cluster_seq > OS_FAT14.max_fat14_clusters) return FILE_ERROR_CLUSTER_SEQ;

    if(file->cluster_seq ==INVLAID_CLUSTER_SEQ)
    {
	    //������ļ��������ļ�ĩβ������EOF 
	    if(file->operate == FILE_OP_READ) return FILE_EOF;
		
       //�´������ļ�
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
			  	 //������ļ��������ļ�ĩβ������EOF 
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
  ���ܣ������ļ�FAT��
  ������
        FAT14_FILE *file
  ���أ�
        TRUE/FALSE
  ������
        ������Ҫ���������Դ�������
---*/
BOOLEAN file_update_fat14(FAT14_FILE *file)
{
    FAT_ENTRY  fat_entry;
    INT32U   fmAddr;
    INT8U    trans_ctrl[2];


    trans_ctrl[0]=0x55;
    trans_ctrl[1]=0xAA;

    //��������ź���
    //OSMutexPend(&OS_FAT14.SIGNAL);
     OSMutexPend(&SIGNAL_FLASHSPI);


    //����������
    write_fmByte(FMADDR_TRANSCTRL+2,FILE_TRANSCTRL_UPDATE_FATFDT);
    //������Ʋ���
    write_fmArray(FMADDR_TRANSCTRL+3,(INT8U *)file,sizeof(FAT14_FILE));
    //����������־
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


    //����Ƿ���Ҫ�ͷŴ�
    if(file->free_cluster < OS_FAT14.max_fat14_clusters)
    {
        fmAddr =  FMADDR_FAT14 + file->free_cluster * sizeof(FAT_ENTRY);
        read_fmArray(fmAddr,(INT8U *)&fat_entry,sizeof(FAT_ENTRY));
        fat_entry.cluster = FILE_FREE_CLUSTER;
        write_fmArray(fmAddr,(INT8U *)&fat_entry,sizeof(FAT_ENTRY));
        file->free_cluster = INVALID_CLUSTER;
    }

    //������ɱ�־
    trans_ctrl[0]=0x66;
    trans_ctrl[1]=0x99;
    write_fmArray(FMADDR_TRANSCTRL,trans_ctrl,2);

    //�ͷ��ļ������ź���
    //OSMutexFree(&OS_FAT14.SIGNAL);
     OSMutexFree(&SIGNAL_FLASHSPI);


    return TRUE;
}



/*+++
  ���ܣ�������ʽд��������
  ������
        INT16U addr,         ��ַ
        INT8U *data,         ������
        INT16U len           ���������ȣ�Ŀǰ�޶����Ȳ��ܳ���46�ֽ�
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
       //��������ź���
       //OSMutexPend(&OS_FAT14.SIGNAL);
       OSMutexPend(&SIGNAL_FLASHSPI);

       //����������
       write_fmByte(FMADDR_TRANSCTRL+2,FILE_TRANSCTRL_FMWRITE);
       //������Ʋ���
       write_fmArray(FMADDR_TRANSCTRL+3,(INT8U *)&addr,2);
       write_fmByte(FMADDR_TRANSCTRL+5,len);
       write_fmArray(FMADDR_TRANSCTRL+6,data,len);
       //����������־
       write_fmArray(FMADDR_TRANSCTRL,trans_ctrl,2);

       //ִ��д����
       write_fmArray(addr,data,len);

       //������ɱ�־
       trans_ctrl[0]=0x66;
       trans_ctrl[1]=0x99;
       write_fmArray(FMADDR_TRANSCTRL,trans_ctrl,2);

       //�ͷ��ļ������ź���
       //OSMutexFree(&OS_FAT14.SIGNAL);
        OSMutexFree(&SIGNAL_FLASHSPI);

    }
}


/*+++
  ���ܣ�����ļ��Ƿ����
  ������
        INT16U file_id
  ���أ�
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
  ���ܣ�ɾ���ļ�
  ������
        INT16U  file_id
  ���أ�
        �������
  ������
       1�����������Դ���������Ҫռ��ϵͳ�����ź���
       2���Ժ���ǰ����ɾ��ÿ���أ����ɾ��FDT
---*/
INT8U  file_delete(INT16U file_id)
{
   INT32U fmAddr;
   INT16U idx,count;
   INT16U cluster;
   INT16U fat_clusters[64];
   INT16U prev_cluster;       //����ɾ����ǰ����ĩ�Ĵغ�
   FDT_ENTRY  fdt_entry;
   FAT_ENTRY fat_entry;
   INT8U trans_ctrl[2];

   //�����ڵ��ļ�,ɾ��ֱ�ӷ��سɹ�!
   if(file_exist(file_id)==FALSE) return  SUCCESS;

   //����ļ��򿪵ǼǱ�־
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
   //��������ź���
   //OSMutexPend(&OS_FAT14.SIGNAL);
     OSMutexPend(&SIGNAL_FLASHSPI);

   //����������
   write_fmByte(FMADDR_TRANSCTRL+2,FILE_TRANSCTRL_DELETE_FILE);
   //������Ʋ���
   write_fmArray(FMADDR_TRANSCTRL+3,(INT8U *)&file_id,2);
   //����������־
   trans_ctrl[0] = 0x55;
   trans_ctrl[1] = 0xAA;
   write_fmArray(FMADDR_TRANSCTRL,trans_ctrl,2);

   fmAddr = FMADDR_FDT + file_id*sizeof(FDT_ENTRY);

   read_fmArray(fmAddr,(INT8U *)&fdt_entry,sizeof(FDT_ENTRY));

   //����ɾ��FAT������Ҫִ�������Բ���
   //Ϊ��ʹ������С����ȡ�Ӻ�ɾ���ķ�����
   //Ϊ�����Ч��,����ÿ�ε������64���صķ���

   do{

      //�����ļ������64�غ�
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

      //�����ͷŵ���ĺ���Ĵ�
      while(idx>0)
      {
          idx--;

          //�ͷŴ� 
          fmAddr =  FMADDR_FAT14+fat_clusters[idx]*sizeof(FAT_ENTRY);
          read_fmArray(fmAddr,(INT8U *)&fat_entry,sizeof(FAT_ENTRY));
          fat_entry.cluster = FILE_FREE_CLUSTER;
          write_fmArray(fmAddr,(INT8U *)&fat_entry,sizeof(FAT_ENTRY));


          //��Ҫ����ɾ��ʱ����Ҫ��ǰ�����ĩ������Ϊ���Ĵ�
          if( (idx==0) && (prev_cluster != fdt_entry.first_cluster))
          {
              fmAddr =  FMADDR_FAT14+prev_cluster*sizeof(FAT_ENTRY);
              read_fmArray(fmAddr,(INT8U *)&fat_entry,sizeof(FAT_ENTRY));
              fat_entry.cluster = FILE_END_CLUSTER;
              write_fmArray(fmAddr,(INT8U *)&fat_entry,sizeof(FAT_ENTRY));
          }
      }
   }while(fat_clusters[0] != fdt_entry.first_cluster);

   //ɾ������FDT
   fdt_entry.file_id = DELETED_FILE_ID;
   fdt_entry.first_cluster = INVALID_CLUSTER;
   fdt_entry.stamp_id = 0;
   fmAddr =  FMADDR_FDT + file_id*sizeof(FDT_ENTRY);
   write_fmArray(fmAddr,(INT8U *)&fdt_entry,sizeof(FDT_ENTRY));

   //������ɱ�־
   trans_ctrl[0]=0x66;
   trans_ctrl[1]=0x99;
   write_fmArray(FMADDR_TRANSCTRL,trans_ctrl,2);

   //�ͷ��ź���
   //OSMutexFree(&OS_FAT14.SIGNAL);
     OSMutexFree(&SIGNAL_FLASHSPI);



   return SUCCESS;
}
///*+++
//  ���ܣ�ɾ���ļ�
//  ������
//        INT16U  file_id
//  ���أ�
//        �������
//  ������
//       1�����������Դ���������Ҫռ��ϵͳ�����ź���
//       2���Ժ���ǰ����ɾ��ÿ���أ����ɾ��FDT
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
//  //ͬ��д�뱸�ݲ����ļ��͵����ļ�
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
  ���ܣ��ض��ļ�
  ������
        INT16U  file_id     �ļ�
        INT32U  max_size    ��������
  ���أ�
        INT8U �������
  ������
       1������ļ�����С��max_size,�򲻽ضϣ�Ҳ����չ��
       2����Ҫ�������
       3)
---*/
INT8U  file_trim(INT16U file_id,INT32U max_size)
{
   INT32U file_size;
   INT32U fmAddr;
   INT16U idx,count;
   INT16U cluster;
   INT16U prev_cluster;       //����ɾ����ǰ����ĩ�Ĵغ�
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

   //�����ڵ��ļ�,ֱ�ӷ��سɹ�!
   if(file_id > MAX_FDT_COUNT) return FILE_ERROR_FILE_ID;
   if(file_exist(file_id)==FALSE) return  SUCCESS;

   //�ļ����ߴ�С��һ���أ�Ҳֱ�ӷ��سɹ���
   //if(max_size <= OS_FAT14.per_cluster_data_size) return SUCCESS;

   //��������ź���
   //OSMutexPend(&OS_FAT14.SIGNAL);
     OSMutexPend(&SIGNAL_FLASHSPI);

   //����������
   write_fmByte(FMADDR_TRANSCTRL+2,FILE_TRANSCTRL_TRIM_FILE);
   //������Ʋ���
   write_fmArray(FMADDR_TRANSCTRL+3,(INT8U *)&file_id,sizeof(INT16U));
   write_fmArray(FMADDR_TRANSCTRL+5,(INT8U *)&max_size,sizeof(INT32U));
   //����������־
   trans_ctrl[0] = 0x55;
   trans_ctrl[1] = 0xAA;
   write_fmArray(FMADDR_TRANSCTRL,trans_ctrl,2);

   fmAddr =  FMADDR_FDT + file_id*sizeof(FDT_ENTRY);
   read_fmArray(fmAddr,(INT8U *)&fdt_entry,sizeof(FDT_ENTRY));

   //Ϊ�����Ч��,����ÿ�ε������64���صķ���
   do{

      //�����ļ������64�غ�
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


      //�����ͷŵ���ĺ���Ĵ�
      while(idx>0)
      {
          //����
          idx--;

          //��ȡ����Ϣ
          fmAddr =  FMADDR_FAT14+fat_clusters[idx]*sizeof(FAT_ENTRY);
          read_fmArray(fmAddr,(INT8U *)&fat_entry,sizeof(FAT_ENTRY));

          //�����ļ��ߴ�
          file_size -= OS_FAT14.per_cluster_data_size;

          //�ж��Ƿ��Ѿ�����Ҫ����
          if(file_size <= max_size)
          {
              //�����ļ������أ�
              fat_entry.cluster = FILE_END_CLUSTER;
              write_fmArray(fmAddr,(INT8U *)&fat_entry,sizeof(FAT_ENTRY));
              break;
          }


          //�ͷŵ�ǰ��
          fat_entry.cluster = FILE_FREE_CLUSTER;
          write_fmArray(fmAddr,(INT8U *)&fat_entry,sizeof(FAT_ENTRY));



          //��Ҫ����ɾ��ʱ����Ҫ��ǰ�����ĩ������Ϊ���Ĵ�
          if( (idx==0) && (prev_cluster != fdt_entry.first_cluster))
          {
              fmAddr =  FMADDR_FAT14+prev_cluster*sizeof(FAT_ENTRY);
              read_fmArray(fmAddr,(INT8U *)&fat_entry,sizeof(FAT_ENTRY));
              fat_entry.cluster = FILE_END_CLUSTER;
              write_fmArray(fmAddr,(INT8U *)&fat_entry,sizeof(FAT_ENTRY));
          }
      }

   }while(file_size > max_size);

      //�������һ���أ�����max_size��С����Ϊff������Ϊ��ҳд��
      if(file_size+OS_FAT14.per_cluster_data_size>max_size)
      {
            page_max_size=max_size-file_size;
            result = file_open(file_id,&file);
            if(SUCCESS == result)
            {
               page = cluster_to_page(fat_clusters[idx]);
               PAGE_WRITE_AGAIN:
               //���ܸ��£����뻻ҳ����в�д
               cluster = get_a_free_cluster(file.open_file_seq);
               if((cluster != INVALID_CLUSTER) && (fat_clusters[idx]!=INVALID_CLUSTER))
               {
                 newPage = cluster_to_page(cluster);
                 cluster_head.file_id = file.fdt_entry.file_id;
                 cluster_head.cluster_seq = max_size/OS_FAT14.per_cluster_data_size;
                 cluster_head.stamp_id = ++ (file.fdt_entry.stamp_id);
                 if(0x8000==ftrim_flashArray(page,page_max_size,newPage,&cluster_head))
                 {
					//����FATΪ��Ч��
                    fmAddr = FMADDR_FAT14 + cluster * sizeof(FAT_ENTRY);
                    fat_entry.cluster = INVALID_CLUSTER;
                    fat_entry.write_count =3;
                    write_fmArray(fmAddr,(INT8U *)&fat_entry,sizeof(FAT_ENTRY));
//                    record_sys_operate(OP_SYS_FLASH_PAGE_BAD);
//                    record_sys_operate(newPage | LOG_RECORD_MASK1);
                    if(try_count++>16)//����3ҳ�������⣬��λ������
                    {
					    //���������������⵼�����ǻ�ҳд�룬���޷�д�ɹ�����˳���16ҳ���������
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
                    //����FAT
                    fmAddr = FMADDR_FAT14 + file.prev_cluster * sizeof(FAT_ENTRY);
                    read_fmArray(fmAddr,(INT8U *)&fat_entry,sizeof(FAT_ENTRY));
                    fat_entry.cluster = file.cur_cluster;
                    write_fmArray(fmAddr,(INT8U *)&fat_entry,sizeof(FAT_ENTRY));
                 }
                 else
                 {
                    //����FDT
                   file.fdt_entry.first_cluster = file.cur_cluster;
                   fmAddr = FMADDR_FDT + file.fdt_entry.file_id*sizeof(FDT_ENTRY);
                   write_fmArray(fmAddr,(INT8U *)&(file.fdt_entry),sizeof(FDT_ENTRY));
                 }

                //���µ�ǰ�ز�д����
                fmAddr = FMADDR_FAT14 + file.cur_cluster * sizeof(FAT_ENTRY);
                read_fmArray(fmAddr,(INT8U *)&fat_entry,sizeof(FAT_ENTRY));
                if(fat_entry.write_count < MAX_CLUSTER_WRITE_LOOP) fat_entry.write_count++;
                fat_entry.cluster = FILE_END_CLUSTER;
                write_fmArray(fmAddr,(INT8U *)&fat_entry,sizeof(FAT_ENTRY));


                //�ͷſ��д�
                fmAddr =  FMADDR_FAT14 + file.free_cluster * sizeof(FAT_ENTRY);
                read_fmArray(fmAddr,(INT8U *)&fat_entry,sizeof(FAT_ENTRY));
                fat_entry.cluster = FILE_FREE_CLUSTER;
                write_fmArray(fmAddr,(INT8U *)&fat_entry,sizeof(FAT_ENTRY));

                
               }
            }
            file_close(&file);
     }


   //������ɱ�־
   trans_ctrl[0]=0x66;
   trans_ctrl[1]=0x99;
   write_fmArray(FMADDR_TRANSCTRL,trans_ctrl,2);

   //�ͷ��ź���
   //OSMutexFree(&OS_FAT14.SIGNAL);
     OSMutexFree(&SIGNAL_FLASHSPI);

   return SUCCESS;
}



/*+++
  ����: �Ѵ�ת��ΪFLASH��ҳ���ַ
  ����:
        INT16U cluster  �غ�
  ����:
        ҳ���
  ������
---*/
INT16U  cluster_to_page(INT16U cluster)
{
    cluster += FAT14_PAGE_OFFSET;
    return cluster;
}

/*+++
  ���ܣ������ļ��ص�ͷ��Ϣ
  ������
        FAT14_FILE *file      �ļ����
  ���أ�
        ��
  ������
        �Ƿ���Ҫ������ƣ���ʱ����Ҫ��������̲���Ӱ��FAT��FDT
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
  ����: д�ļ�
  ����:
        FAT14_FILE *file,              [����]�ļ����
        INT8U *data,                   [����]������
        INT16U datalen                 [����]����������
  ����:
        �������
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
     
      //���Ȱ��ղ�����ҳ����и���д
      write_len = fupdate_flashArray(page,page_offset,data,datalen);
      if(write_len & 0x8000)
      {
PAGE_WRITE_AGAIN:    
         try_count++;   
         //���ܸ��£����뻻ҳ����в�д
         cluster = get_a_free_cluster(file->open_file_seq);
         if(cluster == INVALID_CLUSTER) return FILE_NO_SPACE;
         newPage = cluster_to_page(cluster);
         cluster_head.file_id = file->fdt_entry.file_id;
         cluster_head.cluster_seq = file->cluster_seq;
         cluster_head.stamp_id = ++ (file->fdt_entry.stamp_id);
         write_len = fwrite_flashArray(page,page_offset,newPage,data,datalen,&cluster_head);
         if(write_len == 0x8000)
         {
			//����FATΪ��Ч��
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
            if(try_count>16)//����16ҳ�������⣬��λ������
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

         //�޸�FAT��
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


      //�ļ���ǰָ��ǰ��
      offset = file->offset + write_len;
      file_seek(file,offset);
   }
   return SUCCESS;
}


/*+++
  ����: ���ļ�
  ����:
        FAT14_FILE *file,              [����]�ļ����
        INT8U *data,                   [�����]������
        INT16U datalen                 [����]����������
  ����:
        �������
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
      if(read_len == 0x8000) //���ַ������ݳ���0x8000����ʱֱ�ӷ��أ��������ִ�лḴλ
      {
      	mem_set(data,datalen,0xFF);
      	return SUCCESS;		
      }
      datalen -= read_len;
      data += read_len;

      //�ļ���ǰָ��ǰ��
      offset = file->offset + read_len;
	  
	  //��������ļ�ĩβ������Ĭ��ֵ
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
  ���ܣ��ļ�ϵͳFAT14�������Լ��
  ������
         ��
  ���أ�
         �����
  ������
        1�������ü��
        2���ļ������Լ��
---*/
INT8U  FAT14_Integrity_Check(void)
{
   FAT14_file_check();

   FAT14_cluster_check();

   return 0;
}

/*+++
  ���ܣ��ļ�ϵͳFAT14�������Լ��:��Ӧ��״�����
  ������
         ��
  ���أ�
         �����
  ������
        1��ÿ�μ��100����
---*/

void  FAT14_cluster_check(void)
{
   INT32U fmAddr;
   INT16U file_id;
   INT16U idx;
   INT16U cur_clusters;   //��ǰ�������ʼ�غ�
   INT16U cluster_count;  //��ǰ��Ҫ���Ĵ�����
   INT16U file_cur_cluster;
   FAT_ENTRY fat_entry;
   FDT_ENTRY fdt_entry;
   INT16U fat_clusters[100];
   INT8U     fat_entry_usage[100];

   cur_clusters = 0;
   cluster_count = 0; 
   while(OS_FAT14.max_fat14_clusters > cur_clusters)
   {
       //��ȡ���100���ǿ��д�
       fmAddr = FMADDR_FAT14 + cur_clusters * sizeof(FAT_ENTRY);
       while(cur_clusters < OS_FAT14.max_fat14_clusters)
       {
          read_fmArray(fmAddr,(INT8U *)&fat_entry, sizeof(FAT_ENTRY));
          if(fat_entry.cluster != FILE_FREE_CLUSTER)
          {
             fat_clusters[cluster_count] = cur_clusters;
             fat_entry_usage[cluster_count] = 0;      //�����ô���
             cluster_count ++;
             if(cluster_count >= 100) break;
          }
          cur_clusters ++;
          fmAddr += sizeof(FAT_ENTRY);
       }

       //�����ļ�����ʹ��״̬���
       for(file_id=0; file_id < MAX_FDT_COUNT; file_id++)
       {
           //����ļ�������,���ӹ�!
           if(FALSE == file_exist(file_id)) continue;

           //��ȡ�ļ������Ŀ
           read_fmArray(FMADDR_FDT + file_id*sizeof(FDT_ENTRY),(INT8U *)&fdt_entry,sizeof(FDT_ENTRY));

           //��������ļ���ȫ����
           file_cur_cluster = fdt_entry.first_cluster;
           while(INVALID_CLUSTER != file_cur_cluster)
           {
              //����ļ���ǰ������ȡ�Ĵ������е����
              for(idx=0;idx<100;idx++)
              {
                 if(fat_clusters[idx]==file_cur_cluster)
                 {
                    fat_entry_usage[idx]++;

                    //�����Ƿ񱻶���ļ�����,��Ҫɾ���������õ��ļ�
                    if(fat_entry_usage[idx] > 1)
                    {
                       //ɾ����ǰ�ļ�,����ֹ��ǰ�ļ��Ĵؼ��
                       file_delete(file_id);
                       file_cur_cluster = INVALID_CLUSTER;
                       break;
                    }
                 }
              }
           }
       }

       //���ݴ�ʹ��ͳ����Ϣ��������
       for(idx = 0; idx < cluster_count; idx++)
       {
           //���ô���Ϊ��,�����ͷ�
           if(0 == fat_entry_usage[idx])
           {
                fmAddr = FMADDR_FAT14 + fat_clusters[idx] * sizeof(FAT_ENTRY);
                read_fmArray(fmAddr,(INT8U *)&fat_entry, sizeof(FAT_ENTRY));
                fat_entry.cluster = FILE_FREE_CLUSTER;
                safe_write_fmArray(fmAddr,(INT8U *)&fat_entry,sizeof(FAT_ENTRY));
           }
           //���ô�������1��,��ɾ���ļ�ʱ�Ѿ����ͷ���.
       }
   }

}

/*+++
  ���ܣ��ļ�ϵͳFAT14�������Լ��:�ļ�����Ϣ���
  ������
         ��
  ���أ�
         �����
  ������
        1������ļ�FDT��Ŀ�Ƿ���ȷ
        2) ����ļ��Ĵ����Ƿ���ȷ.
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

       //�ļ�ID��Ч,���һ����
       if(fdt_entry.file_id != file_id)
       {
         fdt_entry.file_id = FILEID_NONE;
         write_fmArray(fmAddr,(INT8U *)&fdt_entry,sizeof(FDT_ENTRY));
       }

       //����ļ��״���������Ƿ�һ��
       if(fdt_entry.first_cluster > OS_FAT14.max_fat14_clusters)
       {
          page =  cluster_to_page(fdt_entry.first_cluster);
          fread_flashArray(page,0,(INT8U *)&cluster_head,sizeof(CLUSTER_HEAD));
          if(cluster_head.file_id != fdt_entry.file_id)
          {
              //��Ч�ļ�
              fdt_entry.file_id = FILEID_NONE;
              write_fmArray(fmAddr,(INT8U *)&fdt_entry,sizeof(FDT_ENTRY));
       }
       }

       fmAddr += sizeof(FDT_ENTRY);
   }
}


/*+++
 ���ܣ�д�ļ�
 ������
       INT16U file_id,   �ļ���
       INT32U addr,      ƫ�Ƶ�ַ
       INT8U *data,      ������
       INT16U len        ���ݳ���
 ����:
       �������
 ������
       1������ļ���æ��������ȴ�ֱ����
       2�����ļ��������ʱ�ŵ�30��
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
  
  //?????�˴���������
  //if(file_id == 0)
  //{
  //    Nop();
  //    Nop();
  //} 
 
  //ת�������ն��ļ���ID��������Ϊ���ܹ����ļ�ID���.
  //FILEID_ERTU_NAMEֻ�� fwrite_ertu_params���á�
   if(file_id == FILEID_ERTU_NAME)
   {
      file_id = FILEID_ERTU;
   }


    //��������ź���
    OSMutexPend(&SIGNAL_FLASHSPI);

  //���ļ�,������ش���æ��������ȴ�
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
  //��λ
  file.operate = FILE_OP_WRITE;
  result = file_seek(&file,addr);
  if(result != SUCCESS)
  {
     file_close(&file);
     OSMutexFree(&SIGNAL_FLASHSPI);
     return result;
  }

  //д�ļ�
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
//  //ͬ��д�뱸�ݲ����ļ��͵����ļ�
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
 ���ܣ����ļ�
 ������
       INT16U file_id,   �ļ���
       INT32U addr,      ƫ�Ƶ�ַ
       INT8U *data,      ������
       INT16U len        ���ݳ���
 ����:
       �������
 ˵����
       �����ڵ��ļ�����ȡ���������0xFF
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

  //���ļ�,������ش���æ��������ȴ�
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


  //��λ
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

  //���ļ�
  result = file_read(&file,data,len);

  file_close(&file);

  OSMutexFree(&SIGNAL_FLASHSPI);
  return len;
}

/*+++
 ���ܣ����ļ�
 ������
       INT16U file_id,   �ļ���
       INT32U addr,      ƫ�Ƶ�ַ
       INT8U *data,      ������
       INT16U len        ���ݳ���
 ����:
       �������
 ˵����
       �����ڵ��ļ�����ȡ���������0xFF
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
//  //�����ȡ�����ļ�����ֹ��Ϊ���ڲ���������ĳ�����ļ��𻵻���֪��
//  if(++file_index>1)
//  {
//    file_index=0;
//  }
//  //ʹ����ȡ��ԭ�򣬶�ȡ�����ļ�����������ļ������ڣ��򲻽�����ȡ��
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
//			if(!is_same)//ǰ�����ļ�����ͬ����Ҫ��ȡ�������ļ�
//			{
//            read_backup_ertu_param(addr,buf2,size,1-file_index);
//	            //��ȡ��
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
//			//�������ļ���Ҫע�⣬PIM_METER_CUR_DATA���ڿ���3ȡ2������ֱ�Ӳ�����
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
//			if(!is_same)//ǰ�����ļ�����ͬ����Ҫ��ȡ�������ļ�
//			{
//            read_backup_meter_param(file_id,addr,buf2,size,1-file_index);
//	            //��ȡ��
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
 ���ܣ����ļ�,�߱���ع���
 ������
       INT16U file_id,   �ļ���
       INT32U addr,      ƫ�Ƶ�ַ
       INT8U *data,      ������
       INT16U len        ���ݳ���
 ����:
       �������
 ˵����
       �����ڵ��ļ�����ȡ���������0xFF
---*/
//INT8U  fread_array_monitor(INT16U file_id,INT32U addr,INT8U *data,INT16U len)
//{
////	�����ȡ���ǲ������ļ����ҿ����˲����㵵����ʼ����������Ҫ�ȳ�ʼ���ò�����
////	if((file_id>0) && (file_id<=MAX_METER_COUNT))
////	{
////		meter_doc_init(file_id);	
////	}
////	return fread_array_exec(file_id,addr,data,len);
// 	return 0;
//}
/*+++
 ���ܣ�д�ļ�,�߱���ع���
 ������
       INT16U file_id,   �ļ���
       INT32U addr,      ƫ�Ƶ�ַ
       INT8U *data,      ������
       INT16U len        ���ݳ���
 ����:
       �������
 ������
       1������ļ���æ��������ȴ�ֱ����
       2�����ļ��������ʱ�ŵ�30��
---*/

//INT8U fwrite_array_monitor(INT16U file_id,INT32U addr,INT8U *data,INT16U len)
//{
////	�����ȡ���ǲ������ļ����ҿ����˲����㵵����ʼ����������Ҫ�ȳ�ʼ���ò�����
////	if((file_id>0) && (file_id<=MAX_METER_COUNT))
////	{
////		meter_doc_init(file_id);	
////	}
////	return fwrite_array_exec(file_id,addr,data,len);
//	return 0;
//}
/*+++
  ���ܣ�ɾ���ļ�,�߱���ع���
  ������
        INT16U  file_id
  ���أ�
        �������
  ������
       1�����������Դ���������Ҫռ��ϵͳ�����ź���
       2���Ժ���ǰ����ɾ��ÿ���أ����ɾ��FDT
---*/
//INT8U  file_delete_monitor(INT16U file_id)
//{
////	INT8U ret;
////
////	ret = file_delete_exec(file_id);
////	//�����ȡ���ǲ������ļ����ҿ����˲����㵵����ʼ����������Ҫ�ȳ�ʼ���ò�����
////	if((file_id>0) && (file_id<=MAX_METER_COUNT))
////	{
////		meter_doc_init_status_clear(file_id);	
////	}
////	return ret;
//	return 0;
//}

//���ԣ�û�������ļ�ϵͳʹ��ҳ�����ҹ�ϵҳ����ӡҳͷ��Ϣ
/*
INT16U  printfile_pagehead(INT16U file_id,INT8U *buffer)
{
	INT16U idx,no,datalen;
    INT8U head[6];

    datalen  = 0;
	for(idx=0;idx<OS_FAT14.max_fat14_clusters;idx++)
	{
            fread_flashArray(idx,0,head,6);
    		no=bin2_int16u(head);//��ȡ�ļ�id
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
