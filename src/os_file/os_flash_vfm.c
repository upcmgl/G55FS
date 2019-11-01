//*****************************************************
//flash��������ϵͳ
//
//������
//   1).���õ���ƬFLASH�ĺ�2M����ǰ��30K��ʵ�ʷµĴ�С�͸�ֵ��һ�£�����
//   2��.Ŀǰʹ�ÿ�Ĵ�СΪ32�ֽڣ��ܹ���9600���飬������Ĵ�СΪ30720�ֽ�
//
//*****************************************************
#include "../main_include.h"

                                     
#define  VFM_FLASH_SIZE             2097152UL                         //FLASH��С			512*4096���������8M�ġ�2K*4K
#define  VFM_FLASH_PAGE_SIZE        4096                               //FLASHҳ��С
#define  VFM_FLASH_BLOCKDATA_SIZE   34                                //�����ݴ�С������2���ֽڿ�ͷ
#define  VFM_BLOCK_SIZE             32                                 //���С
#define  VFM_ARRANGE_PAGE_COUNT     8                                 //FLASHǰ������ʹ��
#define  VFM_FLASH_PAGE_BLOCK_COUNT 120                               //FLASHһҳ�����
#define  VFM_FLASH_PAGE_COUNT       VFM_MAX_PAGE_COUNT                 //FLASH��ҳ�� 512.
#define  VFM_FM_BLOCK_COUNT         (VFM_ARRANGE_PAGE_COUNT*VFM_FLASH_PAGE_BLOCK_COUNT)       //����960����	 8*120
#define  VFM_FM_SIZE                VFM_FM_BLOCK_COUNT*VFM_BLOCK_SIZE                            //�����С30720   32*960
#define  VFM_TEMP_PAGE_COUNT        2                                 //Ԥ������ʱ����ҳ��
#define  VFM_PAGE_SYS_USE_COUNT     4                                //��ʹ��״̬��¼ҳҳ��,�ɼ�¼����Ϊ65536 ,�ǲ���2��bit��ʾһ��?,Ҫ��û����ʾ�����
#define  VFM_USER_PAGE_COUNT        (VFM_FLASH_PAGE_COUNT-VFM_TEMP_PAGE_COUNT-VFM_PAGE_SYS_USE_COUNT-2)   //�û�����ʹ��
#define  VFM_FLASH_BLOCK_COUNT      (VFM_USER_PAGE_COUNT*VFM_FLASH_PAGE_BLOCK_COUNT)    //��60480����			  504*120

#define  VFM_INVALID_BLOCKIDX       0xFF                              //��Ч���
#define  VFM_FLAG_STR               "FMFA"                            //ϵͳ��ʶ������Ϊ4�ֽ�

#define  VFM_TRANSFER_LIMIT         3000                              //ʣ����п�ת����ֵ

//ҳ���ʹ�����
#define  VFM_PAGE_SYS               (VFM_FLASH_PAGE_COUNT-1)           //ϵͳ��ʶ��ҳ
#define  VFM_PAGE_SYS_ARRANGE       (VFM_FLASH_PAGE_COUNT-2)           //ϵͳ��������ҳ
#define  VFM_PAGE_SYS_USE           (VFM_FLASH_PAGE_COUNT-VFM_PAGE_SYS_USE_COUNT-2)	//	��ʹ��״̬��¼ҳҳ��,�ɼ�¼����Ϊ65536 ,�ǲ���2��bit��ʾһ��?,Ҫ��û����ʾ�����
#define  VFM_PAGE_TEMP              (VFM_PAGE_SYS_USE-VFM_TEMP_PAGE_COUNT)     //Ԥ������ʱ����

//VFM_PAGE_SYSϵͳҳ��ַʹ�����
#define  VFM_ADDR_SYSFLAG           0                                 //4,ϵͳ��ʶ��ҳ��־λ��

//��ʱҳʹ�����
//page0   ϵͳF4ʱת������,ǰ2���ֽ�Ϊ�����־��D0=0��ʾ��������  D1=0��ʾ�������

//ҳʹ��������£�
/*
.....VFM_USER_PAGE(1)........
............                .
............                . VFM_ARRANGE_PAGE
.....VFM_USER_PAGE(35).......
............
............
............
............
............
.....VFM_USER_PAGE(VFM_USER_PAGE_COUNT).......
.....VFM_PAGE_TEMP(1).......
.....VFM_PAGE_TEMP(2).......
.....VFM_PAGE_SYS_USE(1).......
............
............
.....VFM_PAGE_SYS_USE(30).......    ��ʹ��״̬��¼ҳ
.....VFM_PAGE_SYS_ARRANGE.......	����ҳ
.....VFM_PAGE_SYS.......	 ϵͳ��ʶ
*/

typedef struct
{
   INT8U  flash_block[3];
}FM_FLASH_BLOCK_MAP;


typedef struct
{
   INT32U    cur_free_block;             //��ǰ���п�����
   INT16U    cur_arrange_block;           //��ǰ����Ŀ�����
   INT16U    cur_transfer_block;          //��ǰת�ƵĿ�����
   INT8U     arrange_flag;               //�����־   0-δ����  1-����
   INT8U     transfer_flag;              //ת�Ʊ�־   0-δת��  1-ת��
   INT8U     buffer[66];
   INT8U     check_buffer[66];
//   INT8U     VFM_SIGNAL;			      //ʹ�ø����������ź���
}FLASH_MANAGE_INFO;

//flash��������ϵͳ�������
static FLASH_MANAGE_INFO OS_VFM_VAR;
//���������
static FM_FLASH_BLOCK_MAP OS_FM_BLOCK_LIST[VFM_FM_BLOCK_COUNT];//ռ�ô�С��4200*3=12600�ֽ�		,������960��



void vfm_init(INT8U isformat,void (*calluserfun)(void));//flash��������ϵͳ��ʼ������
void vfm_format();//flash��������ϵͳ��ʽ��
void vfm_page_arrange(void);//����ҳ����
INT32U vfm_get_free_block(void);//��ȡ���п�
void vfm_read_array(INT32U addr,INT8U *data,INT16U len);//������
void vfm_write_array(INT32U addr,INT8U *data,INT16U len);//д����
BOOLEAN vfm_check_flag(void);//���ϵͳ��ʶ��
BOOLEAN check_valid_block(FM_FLASH_BLOCK_MAP *block_info,INT8U block_seq);//�Աȼ�����������Ч��
INT8U calc_bit_count(INT8U* BS,INT8U byteLen,INT8U bitLen);//����λ�����ж��ٸ�0
BOOLEAN check_is_write(INT8U ch1,INT8U ch2);//�����ֽ��Ƿ��д
void vfm_transfer(void);//ת������
void vfm_arrange(INT8U step,BOOLEAN is_exec);//��������
BOOLEAN check_arrange(void);//�����������
void rebuild_block_list(void);//�ع�������
void get_page_pageoffset(INT32U flash_block,INT16U *page,INT16U *page_offset);//����FLASH�Ŀ��ڵڼ�ҳ��ҳ��ƫ���ֽ�
void erase_block_status_page(void);//�߳�ϵͳ��״̬ҳ
BOOLEAN use_block(INT32U flash_block);
void update_block_idx(INT8U *data,INT16U fm_block_idx);

#ifdef __SOFT_SIMULATOR__
void print_block_list(void);
#endif

/*+++
  ���ܣ���ʼ��Flash��������ϵͳ
  ������
      INT8U isformat �Ƿ��ʽ��
  ���أ�
      ��
  ������
      1).���Flash�Ƿ��Ѿ���ʽ�������û�����ʽ��
      2).����Flash�����ع�����ڴ����
      3).�������ڵ�����������Flash�ؿ������δ��ɵ�ת������
---*/
void vfm_init(INT8U isformat,void (*calluserfun)(void))
{

   //��ʼ��ȫ�ֱ���
   OS_VFM_VAR.cur_free_block=VFM_FLASH_BLOCK_COUNT-2;
   OS_VFM_VAR.cur_transfer_block=0;
   //OS_VFM_VAR.VFM_SIGNAL=0;
   //��ʼ��������
   mem_set(OS_FM_BLOCK_LIST,sizeof(OS_FM_BLOCK_LIST),0xFF);

   //���ϵͳ��ʶ��
   if(isformat)
   {
        vfm_format();
        if(calluserfun!=NULL)
        {
            calluserfun();
        }
   } 
   if(FALSE==vfm_check_flag())
   {
      vfm_format();
        if(calluserfun!=NULL)
        {
            calluserfun();
        }
      isformat=TRUE;
      if(FALSE==vfm_check_flag())
      {
         //���ϵͳ��ʶ����鲻�ܳɹ���ϵͳ�޷�����
//         record_sys_operate(OP_SYS_VFM_ERROR);
//         switch_off_flash();
         os_SoftReset();
         return;
      }
   }
   //������и���ʽ�����������ع�������ʼ�����
   if(isformat)return;

   //��ȡ��״̬���ع�ӳ������
   rebuild_block_list();

   //��������Ƿ���ɣ�û����ɼ�������
   if(check_arrange())return;//�����������������Ҫ�ع�����

}

/*+++
  ���ܣ���������ӳ���FLASH��
  ������
      ��
  ���أ�
  ������
---*/
void set_block_to_list(INT16U fm_block_idx,INT32U flash_block_idx)
{
   
   OS_FM_BLOCK_LIST[fm_block_idx].flash_block[0]=flash_block_idx & 0xFF;
   OS_FM_BLOCK_LIST[fm_block_idx].flash_block[1]=(flash_block_idx>>8) & 0xFF;
   OS_FM_BLOCK_LIST[fm_block_idx].flash_block[2]=(flash_block_idx>>16) & 0xFF;

}

/*+++
  ���ܣ���ȡ����ӳ���FLASH��
  ������
      ��
  ���أ�
  ������
---*/
INT32U get_block_from_list(INT16U fm_block_idx)
{
   INT32U ret;
   
   ret=OS_FM_BLOCK_LIST[fm_block_idx].flash_block[2]<<16;
   ret|=OS_FM_BLOCK_LIST[fm_block_idx].flash_block[1]<<8;
   ret|=OS_FM_BLOCK_LIST[fm_block_idx].flash_block[0];

   return ret;
}

/*+++
  ���ܣ��ع�����
  ������
      ��
  ���أ�
      TRUE:��鵽ϵͳ��ʶ��
      FALSE:û�м�鵽ϵͳ��ʶ��
  ������
      1).���ݿ��ʹ��״̬�ع�������ӳ��
      2).�õ���ǰ���п�
---*/
void rebuild_block_list(void)
{
   INT32U addr,flash_block_idx;
   INT16U page_idx,idx,page,page_offset,fm_block_idx;
   INT8U  byte_offset,bit_offset;
   BOOLEAN is_use,is_delete;

   //OS_VFM_VAR.cur_transfer_block=0xFFFF;

   for(page_idx=0;page_idx<VFM_PAGE_SYS_USE_COUNT;page_idx++)
   {
      //һ�ζ�ȡ64�ֽ�
      for(addr=0;addr<VFM_FLASH_PAGE_SIZE;addr+=64)
      {
         os_vfm_read_array(VFM_PAGE_SYS_USE+page_idx,addr,OS_VFM_VAR.buffer,64);
         //һ����״̬ռ��2��λ��һ���ֽ�4���飬64���ֽڹ�256����
         for(idx=0;idx<256;idx++)
         {
            byte_offset=idx/4;
            bit_offset=(idx%4)*2;
            flash_block_idx=page_idx*4;
            flash_block_idx*=VFM_FLASH_PAGE_SIZE;
            flash_block_idx+=addr*4+idx;

            is_use=(0==get_bit_value(OS_VFM_VAR.buffer+byte_offset,1,bit_offset));
            is_delete=(0==get_bit_value(OS_VFM_VAR.buffer+byte_offset,1,bit_offset+1));
            if(!is_use || is_delete)continue;//û��ʹ�û��Ѿ�ɾ������Ҫ��ȡ��ͷ��Ϣ
            OS_VFM_VAR.cur_free_block=flash_block_idx;
            //��ȡFLASH�洢�Ŀ�ͷ����
            get_page_pageoffset(flash_block_idx,&page,&page_offset);
            os_vfm_read_array(page,page_offset,(INT8U*)&fm_block_idx,2);
            //�ո�ʽ��fm_block_idxΪFFFF����fm_block_idx==flash_block_idx
            if((0xFFFF==fm_block_idx) && (flash_block_idx<VFM_FM_BLOCK_COUNT))
            {
                  fm_block_idx=flash_block_idx;
            }
            if(fm_block_idx<VFM_FM_BLOCK_COUNT)      //flash��ͷ�еĴ�ŵ�fm_block_idx��Ч
            {
               set_block_to_list(fm_block_idx,flash_block_idx);//��������ӳ���FLASH��
               /*//�ع�ʱ����ȡ��ת��Ŀ������ˣ�ת��ʱ�����´������м�⣬�������Ա�����Ϊ�������������⵼���޷�ת��
               if(0xFFFF==OS_VFM_VAR.cur_transfer_block)
               { 
                  if(get_block_from_list(fm_block_idx)<VFM_FM_BLOCK_COUNT)
                  { 
                     OS_VFM_VAR.cur_transfer_block=flash_block_idx;
                  }
               }         
            */         
             }
         }

      }
      
   }
   OS_VFM_VAR.cur_free_block++;

  #ifdef __SOFT_SIMULATOR__
   //print_block_list();
  #endif
}

/*+++
  ���ܣ����ϵͳ��ʶ��
  ������
      ��
  ���أ�
      TRUE:��鵽ϵͳ��ʶ��
      FALSE:û�м�鵽ϵͳ��ʶ��
  ������
---*/
BOOLEAN vfm_check_flag(void)
{
      INT8U buffer[5],flag[4];
      INT8U idx;

      flag[0]='F';
      flag[1]='M';
      flag[2]='F';
      flag[3]='L';

      for(idx=0; idx<3; idx++)
      {
         //��ȡϵͳ������
         os_vfm_read_array(VFM_PAGE_SYS,VFM_ADDR_SYSFLAG,buffer,4);
         if(0 == compare_string(flag,buffer,4))   return TRUE;
      }
      return FALSE;
}

/*+++
  ���ܣ���ʼ����ͷ����
  ������
  ���أ�
  ������Ҫ����дFLASHʧ�ܣ���������д
---*/
BOOLEAN init_blcok_idx(INT16U block_idx)
{
   INT16U  page,page_offset;
   INT8U   idx[2],new_idx[2];
   
   //����ԭҳ�ż�ҳƫ����
   get_page_pageoffset(block_idx,&page,&page_offset);
   //���¿�ͷ
   update_block_idx(idx,block_idx);
   //д���ͷ
   os_vfm_write_array(page,page_offset,idx,2);
   //��֤д���Ƿ���ȷ
   os_vfm_read_array(page,page_offset,new_idx,2);
   if(0 != compare_string(idx,new_idx,2))
   {
		return FALSE;
   }
      return  TRUE;
}

/*+++
  ���ܣ���ʼ�����ʹ��״̬
  ������BOOLEAN is_init_block_idx  �Ƿ��ʼ��Flash�Ŀ�ͷ��Ϣ
  ���أ�дFLASHʧ��,����FALSE,���ϼ����д���
  ������1).��ʽ������ã���Ҫ��ʼ��Flash�Ŀ�ͷ��Ϣ
        2).������ɺ���ã�����Ҫ��ʼ��Flash�Ŀ�ͷ��Ϣ
---*/
BOOLEAN init_block_status(BOOLEAN is_init_block_idx)
{
   INT16U idx,page,page_offset,count,block_idx;
   //��ʽ����ɺ�ӳ������Ŀ鵽FLASH��ǰ4200,����ʼ��ǰ4200����Ŀ�ͷ����
   for(idx=0;idx<VFM_FM_BLOCK_COUNT;idx++)
   {
      set_block_to_list(idx,idx);//��������ӳ���FLASH��
      if(is_init_block_idx)
      {
         if(init_blcok_idx(idx)==FALSE)
         {
         	return FALSE;
         }
      }
   }
   //����ǰ35ҳ���ʹ��״̬Ϊʹ��
   //һ�θ���64���ֽڹ�64*4=256����,һ��ҳ�洢4096*4=16384����,�����ܿ���Ϊ4200������һ��64���ֽ����ã�16������4096���飬��ʣ��4200-4096=104
   mem_set(OS_VFM_VAR.buffer,64,0xAA);
   count=(VFM_FM_BLOCK_COUNT/256)*256;
   for(idx=0;idx<count;idx+=256)
   {
      page=idx/(VFM_FLASH_PAGE_SIZE*4);
      page_offset=(idx%(VFM_FLASH_PAGE_SIZE*4))/4; 
      os_vfm_write_array(VFM_PAGE_SYS_USE+page,page_offset,OS_VFM_VAR.buffer,64);
      //��ȡ��֤д���Ƿ���ȷ
      os_vfm_read_array(VFM_PAGE_SYS_USE+page,page_offset,OS_VFM_VAR.check_buffer,64);
      if(0 != compare_string(OS_VFM_VAR.buffer,OS_VFM_VAR.check_buffer,64))
      {
           return FALSE;
      }
   }
   //����ʣ����״̬��ʣ������������4��������������Ҫ��λ����ʣ�µ�!!!
   count=(VFM_FM_BLOCK_COUNT%256)/4;
   if(count>0)
   {
      page=idx/(VFM_FLASH_PAGE_SIZE*4);//ע�⣺idxΪ����for�Ľ��
      page_offset=(idx%(VFM_FLASH_PAGE_SIZE*4))/4;
      os_vfm_write_array(VFM_PAGE_SYS_USE+page,page_offset,OS_VFM_VAR.buffer,count);
      //��ȡ��֤д���Ƿ���ȷ
      os_vfm_read_array(VFM_PAGE_SYS_USE+page,page_offset,OS_VFM_VAR.check_buffer,count);
      if(0 != compare_string(OS_VFM_VAR.buffer,OS_VFM_VAR.check_buffer,count))
      {
           return FALSE;
      }
      block_idx=page*VFM_FLASH_PAGE_SIZE*4+page_offset*4+count*4;
      count=(VFM_FM_BLOCK_COUNT%256)%4;
      for(idx=0;idx<count;idx++)
      {
         if(use_block(block_idx+idx) == FALSE)
         {
			return FALSE;
         }
      }
   }

   //���õ�ǰת��ָ��Ϊ��ʼ��ת�ƵĿ�
   OS_VFM_VAR.cur_transfer_block=0;
   //���õ�ǰ���п�ָ���������������һ�顣
   OS_VFM_VAR.cur_free_block=VFM_FM_BLOCK_COUNT;
   return TRUE;
}

/*+++
  ���ܣ��߳����������ҳ
  ������
  ���أ�
  ������
---*/
void erase_arrange_page(void)
{
   INT16U  idx,count;
   count=(VFM_ARRANGE_PAGE_COUNT/16)*16;
   for(idx=0;idx<count;idx+=16)
   {
	  ClrWdt();
      os_vfm_erase_block(idx);
   }
   for(idx=count;idx<VFM_ARRANGE_PAGE_COUNT;idx++)
   {
	  ClrWdt();
      os_vfm_erase_page(idx);
   }
}

/*+++
  ���ܣ�flash��������ϵͳ��ʽ��
  ������
  ���أ�
  ������
---*/
void vfm_format(void)
{
   INT16U idx,page,page_offset;
   INT8U flag[4],check_flag[4];

   for(idx=0;idx<10;idx++)
   {
   //����ϵͳ��ʶ��ҳ
   os_vfm_erase_page(VFM_PAGE_SYS);
   //������״̬��¼ҳ
   erase_block_status_page();
   //����ϵͳ��������ҳ
   os_vfm_erase_page(VFM_PAGE_SYS_ARRANGE);
   //��������Ҫ��ҳ
   erase_arrange_page();

       //��ʼ��ʧ�ܣ������ִ�и�ʽ��
       if(FALSE==init_block_status(TRUE))
       {
       		continue;
       }
   //д���ʶ��
   flag[0]='F';
   flag[1]='M';
   flag[2]='F';
   flag[3]='L';
       //дʧ�ܺ�������ִ�и�ʽ��
   os_vfm_write_array(VFM_PAGE_SYS,VFM_ADDR_SYSFLAG,flag,4);
       os_vfm_read_array(VFM_PAGE_SYS,VFM_ADDR_SYSFLAG,check_flag,4);
       if(0 != compare_string(flag,check_flag,4))
       {
           continue;
       }
       break;
   }
    #ifdef __VFM_SELF_TEST__
    vfm_write_array(FMADDR_VFM_SELF_TEST,flag,4);
    #endif 
//    record_sys_operate(OP_SYS_VFM_FORMAT);
}

/*+++
  ���ܣ�����λ�����ж��ٸ�0
  ������
        INT8U *BS       λ��ָ��
        INT8U byteLen   λ�����ֽڳ���
        INT8U bitLen    λ����λ����
  ���أ�
      λ�����ж���λΪ0
  ������
---*/
INT8U calc_zero_count(INT8U *BS,INT8U byteLen,INT8U bitLen)
{
   INT8U idx,result;
   result=0;
   for(idx=0;idx<bitLen;idx++)
   {
      if(0==get_bit_value(BS,byteLen,idx))
         result++;

   }
   return result;

}

/*+++
  ���ܣ������ֽ��Ƿ��д
  ������
      INT8U ch1   ������ֽ�
      INT8U ch2   Ŀ���ֽ�
  ���أ�
      TRUE  ��д
      FALSE ����д
  ������
      ��λ�Աȣ�������ڽ�0��Ϊ1�򲻿�д�������д

      1)  0 ^ 0 = 0
          1 ^ 1 = 0
          ����  ch1^ch2 == 0�����ʾÿλ�����仯�����д��
      2)  1 ^ 0 = 1
          0 ^ 1 = 1
          ���� ��ch3 = ch1 ^ ch2 ��Ϊ�㣬���ʾ�в�ͬλ�仯������ı仯�Ǵ�1->0
          ���ԣ���ch3 & ch2, ��ch2λֵΪ0ʱ��ch3���Ϊ0����ʱ������ġ�
          ��֮�����ch2��λֵΪ1������Ҫ��ch1��0��Ϊch2��1����ʱch3��Ϊ1�����ǲ�����ġ� 
---*/
BOOLEAN check_is_write(INT8U ch1,INT8U ch2)
{
   INT8U ch3;
   ch3=ch1^ch2;
   if(0==ch3)return TRUE;
   if(0==(ch3 & ch2)) return TRUE;
   return FALSE;

}

/*+++
  ���ܣ����2�������������Ƿ���ͬ
  ������
      INT8U *data1  ������1
      INT8U *data2  ������2
      INT16U len  ��鳤��
  ���أ�
  ������
---*/
BOOLEAN chek_data_same(INT8U *data1,INT8U *data2,INT16U len)
{
   INT16U idx;
   for(idx=0;idx<len;idx++)
   {
      if(data1[idx]!=data2[idx])return FALSE;
   }
   return TRUE;
}

/*+++
  ���ܣ�ʹ��һ��ָ���Ŀ飬���ĸÿ��״̬λΪʹ��״̬
  ������
      INT32U flash_block    Flash������
      INT8U  pos  0-ʹ��λ  1-ɾ��λ
  ���أ�дflashʧ�ܣ���Ҫ����FALSE���ϼ����»��鴦��
  ������
---*/
BOOLEAN use_block(INT32U flash_block)
{
   INT16U page,page_offset,page_block,offset;

   //���ݿ�ţ���������ҳ��ƫ����
   page=flash_block/(VFM_FLASH_PAGE_SIZE*4);
   page_block=flash_block%(VFM_FLASH_PAGE_SIZE*4);
   page_offset=page_block/4;
   offset=page_block%4;
   //����λ״̬
   return os_vfm_write_bits(VFM_PAGE_SYS_USE+page,page_offset,0,offset*2);
}

/*+++
  ���ܣ��ͷ�ָ����FLASH��,���ĸÿ��״̬Ϊʹ�ú�ɾ��
  ������
      INT32U flash_block    Flash������
  ���أ�
  ������дFLASHδ�ɹ�����ִ�ж��
---*/
void release_block(INT32U flash_block)
{
   INT16U page,page_offset,page_block,offset;

   //���ݿ�ţ���������ҳ��ƫ����
   page=flash_block/(VFM_FLASH_PAGE_SIZE*4);
   page_block=flash_block%(VFM_FLASH_PAGE_SIZE*4);
   page_offset=page_block/4;
   offset=page_block%4;

   //����λ״̬
   os_vfm_clr_2bits(VFM_PAGE_SYS_USE+page,page_offset,offset*2);
}

/*+++
  ���ܣ�����FLASH�Ŀ��ڵڼ�ҳ��ҳ��ƫ���ֽ�
  ������
      INT32U flash_block  FLASH������
      INT16U *page  ���,�ÿ����ڵ�ҳ�� 
      INT16U *page_offset  ���,�ÿ�����ҳ���ֽ�ƫ����
  ���أ�
  ������
---*/
void get_page_pageoffset(INT32U flash_block,INT16U *page,INT16U *page_offset)
{
   *page=flash_block/VFM_FLASH_PAGE_BLOCK_COUNT;
   *page_offset=(flash_block%VFM_FLASH_PAGE_BLOCK_COUNT)*VFM_FLASH_BLOCKDATA_SIZE;
}

/*+++
  ���ܣ���֤һ����������Ƿ���Ը���д��
  ������
      INT8U *buffer1 ԭ�黺����
      INT8U *buffer2 �¿黺����
      INT8U len     �Ƚϳ���
  ���أ�TRUE:���Ը���д��   FALSE:���ܸ���д��
  ������
---*/
BOOLEAN check_block_allow_write(INT8U *buffer1,INT8U *buffer2,INT8U len)
{
   INT8U idx;

   for(idx=0;idx<len;idx++)
   {
      if(!check_is_write(buffer1[idx],buffer2[idx]))return FALSE;
   }
   return TRUE;

}

/*+++
  ���ܣ�������Ÿ��µ���������
  ������
      INT8U *data    �黺����
      INT16U block_idx ��������
      INT8U len     �Ƚϳ���
  ���أ�
  ������
---*/
void update_block_idx(INT8U *data,INT16U fm_block_idx)
{
    if(fm_block_idx<VFM_FM_BLOCK_COUNT)
    {
        data[0] =(INT8U)fm_block_idx;
        data[1] =(INT8U)(fm_block_idx >>8);
    }
}

/*+++
  ���ܣ�������
  ������
      INT32U addr  ��ȡ���ݵĵ�ַ
      INT8U *data  ��ȡ���ݷ��ص�����
      INT16U len  ��ȡ���ݵĳ���
  ���أ�
  ������
---*/
void vfm_read_array(INT32U addr,INT8U *data,INT16U len)
{
   INT32U flash_block_idx;
   INT16U block_idx,block_offset,page,page_offset;
   INT8U datalen;

   
   if(NULL==data)return;

   OSMutexPend(&SIGNAL_FLASHSPI);
   while(len>0)
   {
     block_idx=addr/VFM_BLOCK_SIZE;
     block_offset=addr%VFM_BLOCK_SIZE;
     datalen=len>(VFM_BLOCK_SIZE-block_offset)?VFM_BLOCK_SIZE-block_offset:len;
     if(block_idx>=VFM_FM_BLOCK_COUNT)//���ʵ�ַ������ϵͳ֧�ִ�С
     {
        OSMutexFree(&SIGNAL_FLASHSPI);   
         mem_set(data,len,0xFF);
         return;
     }
     flash_block_idx=get_block_from_list(block_idx);//������ӳ���ȡ���ڵ�FLASH��
     //�����û��д�����ݻ�ÿ��Ѿ�ɾ�������ʼ��Ϊ0xFF
     if(flash_block_idx>=VFM_FLASH_BLOCK_COUNT)
     {
         mem_set(data,datalen,0xFF);

     }else
     {
          //����FLASH�Ŀ��ڵڼ�ҳ��ҳ��ƫ���ֽ�
          get_page_pageoffset(flash_block_idx,&page,&page_offset);
          page_offset+=block_offset+2;//���ӿ���ƫ�Ƽ���ͷ��С
          //��ȡ������
          os_vfm_read_array(page,page_offset,data,datalen);
     }
     len-=datalen;
     addr+=datalen;
     data+=datalen;
   }
   OSMutexFree(&SIGNAL_FLASHSPI);
}

/*+++
  ���ܣ�д����
  ������
      INT32U addr  ��д���ݵĵ�ַ
      INT8U *data  ��д���ݵ�����
      INT16U len  ��д���ݵĳ���
  ���أ�
  ������
      1).д��������ԭ������ͬ����д��
      2).д������������Ը�д���򸲸�д�룬���򻻿�д��
      3).д������������ԭ���ݲ�һ�£������»�ҳд��
      4).������Ը���д��,��Ҫ����д�룬���ǰ�ʵ�ʴ�Сд��
      5)дFlashʧ�ܣ������������д��
---*/
void vfm_write_array(INT32U addr,INT8U *data,INT16U len)
{
   INT32U old_flash_block_idx,new_flash_block_idx;
   INT16U page,page_offset,block_idx,block_offset,offset,tmp_idx;
   INT8U *data_ptr;
   INT8U datalen,idx,write_len;
   BOOLEAN is_update;

   if(NULL==data)return;

   OSMutexPend(&SIGNAL_FLASHSPI);
   while(len>0)
   {
     block_idx=addr/VFM_BLOCK_SIZE;
     block_offset=addr%VFM_BLOCK_SIZE;
     datalen=len>(VFM_BLOCK_SIZE-block_offset)?VFM_BLOCK_SIZE-block_offset:len;
     if(block_idx>=VFM_FM_BLOCK_COUNT)
     {
      #ifdef __SOFT_SIMULATOR__
      snprintf(info,100,"vfm write array block_idx %d >=VFM_FM_BLOCK_COUNT !!!",block_idx);
      println_info(info);
      #endif
      OSMutexFree(&SIGNAL_FLASHSPI);
      return;

      }
     //������û�п��õĿ��п飨���п�����С��1����������
     vfm_arrange(0xEE,FALSE);
     //�õ�ԭ���
     old_flash_block_idx=get_block_from_list(block_idx);//������ӳ���ȡ���ڵ�FLASH��

     //����FLASH�Ŀ��ڵڼ�ҳ��ҳ��ƫ���ֽ�
     get_page_pageoffset(old_flash_block_idx,&page,&page_offset);
     //���ȶ�ȡԭ���ݣ��Ƚ�ԭ���ݺ������ݣ������д����ֱ��д�룬���򻻿�д��
     os_vfm_read_array(page,page_offset,OS_VFM_VAR.buffer,VFM_BLOCK_SIZE+2);
     //�����д�����ݺ�Ŀ��������ͬ������Ҫд��
     if(!chek_data_same(OS_VFM_VAR.buffer+block_offset+2,data,datalen))
     {
        //��֤ԭ�����������Ƿ���Ը���д��,����������򻻿�д�룬����Ҫ��װ���������������ݽ��кϲ�
        is_update=check_block_allow_write(OS_VFM_VAR.buffer+2+block_offset,data,datalen);

        //�ϲ����������ݵ�ԭ������,�ܹ�д��û�б�Ҫ�ϲ���ֻ��д��ʧ�ܺ����Ҫ�ϲ�
        mem_cpy(OS_VFM_VAR.buffer+block_offset+2,data,datalen);
       
        //�õ�д�����ݵĳ��Ⱥ���ʼ��ַ
        if(is_update)
        {
            //����Ǹ�ʽ�������ģ��п��״̬û�п�ͷ��Ϣ����ӿ�ͷλ�ÿ�ʼд
            if((OS_VFM_VAR.buffer[0]==0xFF) && (OS_VFM_VAR.buffer[1]==0xFF))
            {
                write_len=block_offset+datalen+2;
               data_ptr=OS_VFM_VAR.buffer;
               offset=page_offset;
            }
            else
            {
                write_len=datalen;
                data_ptr=data;
                offset=page_offset+block_offset+2;
            }
         }

         //���¿����
         update_block_idx(OS_VFM_VAR.buffer,block_idx);

         //Ĭ����ԭҳ�Ͻ���д������
         new_flash_block_idx=old_flash_block_idx;
         while(1)
         {
           //��ȡһ�����п�
           if(FALSE==is_update)
           {
               new_flash_block_idx=vfm_get_free_block();

               #ifdef __SOFT_SIMULATOR__
               if((new_flash_block_idx  <  old_flash_block_idx) && (old_flash_block_idx != 0x00FFFFFF))
               {
                    //snprintf(info,200,"error!,vfm write array block_idx = %d,old_flash_block_idx=%d,new_flash_block_idx=%d,task_no=%d",block_idx,old_flash_block_idx,new_flash_block_idx,os_cur_task);
                    //println_info(info);
               }
                #endif

              //����FLASH�Ŀ��ڵڼ�ҳ��ҳ��ƫ���ֽ�
              get_page_pageoffset(new_flash_block_idx,&page,&page_offset);
              //�����Ҫ��ҳд��������д�볤�Ⱥ���ʼָ��
              write_len=VFM_BLOCK_SIZE+2;
              data_ptr=OS_VFM_VAR.buffer;
              offset=page_offset;
           }
           //�������ݵ�FLASH
           #ifdef __SOFT_SIMULATOR__
           //snprintf(info,200,"vfm write array block_idx = %d,old_flash_block_idx=%d,new_flash_block_idx=%d,task_no=%d",block_idx,old_flash_block_idx,new_flash_block_idx,os_cur_task);
           //println_info(info);
           #endif
           //���д��ҳ��ַ�Ƿ���Ч���Ƿ���д��
           if(page<VFM_USER_PAGE_COUNT)
           {
              os_vfm_write_array(page,offset,data_ptr,write_len);
              //��֤д�������Ƿ���ȷ���������ȷ�����»�ȡһ���¿飬�ٴ�д��
              os_vfm_read_array(page,offset,OS_VFM_VAR.check_buffer,write_len);
              //��ȡ��ͷ������֤��ͷ�Ƿ���ȷ���������ȷ�����»�ȡһ���¿飬�ٴ�д��
              os_vfm_read_array(page,page_offset,(INT8U*)(&tmp_idx),2);
              if(chek_data_same(data_ptr,OS_VFM_VAR.check_buffer,write_len) && (tmp_idx==block_idx))
              {
                 if(FALSE==is_update)
                 { 
                    //���¿�ʹ�ñ��
                    if(use_block(new_flash_block_idx))
                    {
                        //���¿�ɾ�����
                        if(old_flash_block_idx<VFM_FLASH_BLOCK_COUNT)
                        {
                           release_block(old_flash_block_idx);
                        }

                        break;
                    }
                 }
                 else
                 {
                   break;
                 }
              }
           }

           //����Ǹ���ԭ�飬������ɾ��ԭ��
           if(FALSE == is_update)
           { 
               //���ڸÿ�д�����ݺ�ԭ���ݲ�һ�£���Ҫ��ҳ����д�룬��˽��ÿ����ɾ����־
               release_block(new_flash_block_idx);
               #ifdef __SOFT_SIMULATOR__
               snprintf(info,100,"vfm write array block_idx = %d write fail ,retry write!!!",block_idx);
               println_info(info);
               #endif
           } 
           is_update=FALSE;   
        }  // end of while(1)  

//         if(FALSE==is_update)
//         { 
//            //���¿�ʹ�ñ��
//            use_block(new_flash_block_idx);
//            //���¿�ɾ�����
//            if(old_flash_block_idx<VFM_FLASH_BLOCK_COUNT)
//            {
//               release_block(old_flash_block_idx);
//            }
//         }
         //���������FLASH�����
         set_block_to_list(block_idx,new_flash_block_idx);//��������ӳ���FLASH��
     }
     len-=datalen;
     addr+=datalen;
     data+=datalen;
   }
   //ÿ��д�����ݼ���Ƿ���Ҫת������
   vfm_transfer();
   OSMutexFree(&SIGNAL_FLASHSPI);
   
}

/*+++
  ���ܣ���ȡ���п�
  ������
  ���أ��Ѿ���ȡ���Ŀ��п����
  ������
---*/
INT32U vfm_get_free_block(void)
{
      INT16U page,page_offset;
      if(OS_VFM_VAR.cur_free_block<VFM_FLASH_BLOCK_COUNT-1)
      {
         //����ÿ�λ��һҳ�ĵ�һ���飬�����߳���ҳ
         page_offset=OS_VFM_VAR.cur_free_block%VFM_FLASH_PAGE_BLOCK_COUNT;
         if(0==page_offset)
         {
              page=OS_VFM_VAR.cur_free_block/VFM_FLASH_PAGE_BLOCK_COUNT;
               os_vfm_erase_page(page);
              #ifdef __SOFT_SIMULATOR__
              snprintf(info,100,"os_vfm_erase_page  %d",OS_VFM_VAR.cur_free_block/VFM_FLASH_PAGE_BLOCK_COUNT);
              println_info(info);
              #endif
         }
      }
      else
      {
         //û�п��п飬��Ҫ��������, �ӻ����Ͻ������ܣ���λ����
         switch_off_flash();
         os_SoftReset();

      }
      OS_VFM_VAR.cur_free_block++;

     return (OS_VFM_VAR.cur_free_block-1);
}

/*+++
  ���ܣ���ȡ��д����
  ������
  ���أ�
  ���������֧��4092�Σ�������ֵ������¼
---*/
INT16U vfm_read_write_count(void)
{
   INT16U len,addr,result;
   INT8U read_len,idx;
   INT8U readbuf[16];
   len=4092;
   addr=0;
   result=0;
   while(len>0)
   {
      read_len=len>16?16:len;
      os_vfm_read_array(VFM_PAGE_SYS,addr+4,readbuf,read_len);
      for(idx=0;idx<read_len*8;idx++)
      {
         if(get_bit_value(readbuf,read_len,idx))return result;
         result++;
      }
      addr+=read_len;
      len-=read_len;
   }
   return result;
}

/*+++
  ���ܣ����²�д����
  ������
  ���أ�
  ���������֧��4092�Σ�������ֵ������¼
---*/
void vfm_update_write_count(INT16U count)
{
   INT16U write_byte;
   INT8U write_bit;
   write_byte=count/8;
   write_bit=count%8;
   os_vfm_write_bits(VFM_PAGE_SYS,4+write_byte,0,write_bit);

}

/*+++
  ���ܣ��ƶ�������
  ������
  ���أ�ת��ʧ��,�򷵻�FALSE
  ������1).�����п�С��ĳֵʱ���ҿ������д���С��2048�Ŀ飬��ʼת��ǰ32ҳ�Ŀ�����
        2).ÿ�ν�ת��һ����
        
---*/
BOOLEAN move_blcok(INT32U old_block_idx,INT32U new_block_idx,INT16U fm_block_idx)
{
   INT16U  page,page_offset;
   INT8U buffer[VFM_FLASH_BLOCKDATA_SIZE];
   //����ԭҳ�ż�ҳƫ����
   get_page_pageoffset(old_block_idx,&page,&page_offset);
   //��ȡԭ������
   os_vfm_read_array(page,page_offset,OS_VFM_VAR.check_buffer,VFM_FLASH_BLOCKDATA_SIZE);
   //���¿�ͷ����Ϊ�������δʹ���򣬿�ͷΪFF
   update_block_idx(OS_VFM_VAR.check_buffer,fm_block_idx);
   //������ҳ�ż�ҳƫ����
   get_page_pageoffset(new_block_idx,&page,&page_offset);
   //д���¿�����
   os_vfm_write_array(page,page_offset,OS_VFM_VAR.check_buffer,VFM_FLASH_BLOCKDATA_SIZE);
   //д����֤
   os_vfm_read_array(page,page_offset,buffer,VFM_FLASH_BLOCKDATA_SIZE);
   if(0 != compare_string(buffer,OS_VFM_VAR.check_buffer,VFM_FLASH_BLOCKDATA_SIZE))
   {
       return FALSE;
   }
   return TRUE;
   
}

/*+++
  ���ܣ��߳���״̬ҳ
  ������
  ���أ�
  ������1).�ܹ�30��ҳ
---*/
void erase_block_status_page(void)
{
   INT16U idx;
   for(idx=0;idx<VFM_PAGE_SYS_USE_COUNT;idx++)
   {
      os_vfm_erase_page(VFM_PAGE_SYS_USE+idx);
	  ClrWdt();
   }
}

/*+++
  ���ܣ�����ת�ƣ���ǰ32ҳ����ת�Ƶ�32ҳ�Ժ�
  ������
  ���أ�
  ������1).�����п�С��ĳֵʱ���ҿ������д���С��2048�Ŀ飬��ʼת��ǰ32ҳ�Ŀ�����
        2).ÿ�ν�ת��һ����,��ȷ��ÿ�ζ��ܹ�ת��һ��
        3) �ú���ֻ������vfm_write_array�е��ã�!
        4)ת��ʧ��,�������¿�ת��
---*/
void vfm_transfer(void)
{
   INT32U old_block_idx,new_block_idx;
   INT16U idx;

   //����Ѿ�ת����ɣ�������ת��
   if(OS_VFM_VAR.cur_transfer_block>=VFM_FM_BLOCK_COUNT)return;

   //���п�ʣ��Ƚ϶�ʱ������ת��
   if((VFM_FLASH_BLOCK_COUNT-OS_VFM_VAR.cur_free_block)>VFM_TRANSFER_LIMIT)return;
   
   while(OS_VFM_VAR.cur_transfer_block<VFM_FM_BLOCK_COUNT)
   {
       //����ÿ鲻��ǰ32ҳ�򣬲�ת��
       old_block_idx=get_block_from_list(OS_VFM_VAR.cur_transfer_block);//������ӳ���ȡ���ڵ�FLASH��
       if(old_block_idx<VFM_FM_BLOCK_COUNT) break;
       OS_VFM_VAR.cur_transfer_block++;
   }

   if(OS_VFM_VAR.cur_transfer_block < VFM_FM_BLOCK_COUNT)
   { 
     //�õ���ǰҪת�ƵĿ�
      old_block_idx=get_block_from_list(OS_VFM_VAR.cur_transfer_block);//������ӳ���ȡ���ڵ�FLASH��
    
     if(old_block_idx<VFM_FM_BLOCK_COUNT)
     {
        for(idx=0;idx<3;idx++)
        {
        //��ȡһ�����п顢
        new_block_idx=vfm_get_free_block();

        //�ƶ����ݵ����п�
            if(FALSE==move_blcok(old_block_idx,new_block_idx,OS_VFM_VAR.cur_transfer_block))
            {
                DelayNmSec(100);
            	continue;
            }

        //�����¿�Ϊ��ʹ��
            if(FALSE==use_block(new_block_idx))
            {
                DelayNmSec(100);
            	continue;
            }
        //���¿�����
     set_block_to_list(OS_VFM_VAR.cur_transfer_block,new_block_idx);//��������ӳ���FLASH��
        //����ԭ��Ϊ��ɾ��
        release_block(old_block_idx);
        //�ƶ���ǰת��ָ��Ϊ��һ��
        OS_VFM_VAR.cur_transfer_block++;
            break;
        }
     } 
   }

   

}

/*+++
  ���ܣ�д�������������־
  ������
  ���أ�
  ������дʧ�ܣ�����ȡ����֤����Ҫ����
---*/
void update_arrange_trans(INT8U flag)
{
   INT8U temp[3];

   temp[0]=flag;
   temp[1]=flag;
   temp[2]=flag;
   
   os_vfm_write_array(VFM_PAGE_SYS_ARRANGE,0,temp,3);

}

/*+++
  ���ܣ���ȡ���������־
  ������
  ���أ�
  ��������λ3ȡ2
---*/
INT8U read_arrange_trans(void)
{
   INT8U temp[3],flag;
   
   os_vfm_read_array(VFM_PAGE_SYS_ARRANGE,0,temp,3);

   if((temp[0]==temp[1]) && (temp[1]==temp[2]))
   {
    	return temp[0];
   }
   flag=(temp[0] & temp[1]) | (temp[1] & temp[2]) | (temp[2] & temp[0]);
   
   return flag;
}

/*+++
  ���ܣ�����������33ҳ�����Ч��������ǰ33ҳ
  ������
  ���أ�
  ������1).�����п�С��ĳֵʱ���ҿ��������Ѿ�û��2046��ǰ����Ч��ʱ�����ÿ���ǰ33ҳ����ʼ����
        2).����ÿ������������Ч��
        3).����Ѿ���ʼ������û��������ʱ���磬�´ο����������߳�ǰ32ҳ��������
        4).����Ѿ�����������߳���8ҳ�Ŀ�״̬ʱ���磬����Ҫ������
        5).����ʧ�ܣ���������
---*/
void vfm_arrange(INT8U step,BOOLEAN is_exec)
{
   INT32U old_block_idx,new_block_idx;
   INT16U idx,page,page_offset,write_count;

   //���п�ʣ��Ƚ϶�ʱ������ת��
   if((VFM_FLASH_BLOCK_COUNT>OS_VFM_VAR.cur_free_block+2)&&(is_exec==FALSE)) return;
  #ifdef __SOFT_SIMULATOR__
  snprintf(info,100,"vfm_arrange !!!");
  println_info(info);
  #endif
   OSMutexPend(&SIGNAL_FLASHSPI);
   switch(step)
   {
     case 0xEE:
       //����ǰ�����߳���������ҳ
       os_vfm_erase_page(VFM_PAGE_SYS_ARRANGE);
       //д�����������־����ʼ����
       update_arrange_trans(0xEE);
       //�߳����������ҳ
       erase_arrange_page();
       //��������Ҫ��ҳ
       for(new_block_idx=0;new_block_idx<VFM_FM_BLOCK_COUNT;new_block_idx++)
       {
         //�õ���ǰ��Ҫ����Ŀ�
         old_block_idx=get_block_from_list(new_block_idx);//������ӳ���ȡ���ڵ�FLASH��
         //�ƶ����ݵ����п�
         if(FALSE == move_blcok(old_block_idx,new_block_idx,new_block_idx))
         {
   			OSMutexFree(&SIGNAL_FLASHSPI);
            return;
         }
         //���¿�����
         set_block_to_list(new_block_idx,new_block_idx);//��������ӳ���FLASH��
       }
       //д�����������־�����������
       update_arrange_trans(0xCC);
     case 0xCC:
        //�߳���״̬ҳ
       erase_block_status_page();
       //д�����������־���߳�״̬ҳ���
       update_arrange_trans(0x88);
     case 0x88:
       //��ʼ�����״̬
       if(FALSE==init_block_status(FALSE))
       {
       		erase_block_status_page();
   			OSMutexFree(&SIGNAL_FLASHSPI);
            return;

       }
       //������ɣ�����״̬Ϊ00
       update_arrange_trans(0x00);
       break;
     default:
       update_arrange_trans(0x00);
     	 break;	
    }
   //����FLASH��д����
   write_count=vfm_read_write_count();
   vfm_update_write_count(write_count);

   OSMutexFree(&SIGNAL_FLASHSPI);
}

/*+++
  ���ܣ�����������ݣ�ϵͳ��ʼ��ʱ����
  ������
  ���أ�
  ������1).��������Ƿ��Ѿ���ɣ�û����ɼ�������
        2).�������������־�����δ��ɵ�������
---*/
BOOLEAN check_arrange(void)
{
   INT8U flag;
   //��ȡ�����־
   //os_vfm_read_array(VFM_PAGE_SYS_ARRANGE,0,&flag,1);
   flag=read_arrange_trans();
   //�����������δ��ɣ����������
   if((0xFF!=flag) && (0x00!=flag))
   {
      vfm_arrange(flag,TRUE);
      return TRUE;
   }

  return FALSE;
}

/*+++
  ���ܣ��߳�ϵͳԤ����ҳ
  ������
  ���أ�
  ������
---*/
void vfmflash_temp_page_erase(INT16U temp_page_idx)
{
   if(temp_page_idx>=VFM_TEMP_PAGE_COUNT)return;
   os_vfm_erase_page(VFM_PAGE_TEMP+temp_page_idx);
}

/*+++
  ���ܣ���ϵͳԤ����ҳ����д����
  ������
  ���أ�
  ������
---*/
void vfmfalsh_temp_write_array(INT16U temp_page_idx,INT16U page_offset,INT8U *data,INT16U len)
{
   if(temp_page_idx>=VFM_TEMP_PAGE_COUNT)return;
   os_vfm_write_array(VFM_PAGE_TEMP+temp_page_idx,page_offset,data,len);
}

/*+++
  ���ܣ���ϵͳԤ����ҳ���ж�����
  ������
  ���أ�
  ������
---*/
void vfmfalsh_temp_read_array(INT16U temp_page_idx,INT16U page_offset,INT8U *data,INT16U len)
{
   if(temp_page_idx>=VFM_TEMP_PAGE_COUNT)return;
   os_vfm_read_array(VFM_PAGE_TEMP+temp_page_idx,page_offset,data,len);
}

#ifdef __SOFT_SIMULATOR__
void print_block_list(void)
{
  INT32U flash_block_idx;
  INT16U idx;
  for(idx=0;idx<VFM_FM_BLOCK_COUNT;idx++)
  {
      flash_block_idx=get_block_from_list(idx);
      {
        snprintf(info,100,"OS_FM_BLOCK_LIST[%d]=%d",idx,flash_block_idx);
        println_info(info);
      }

  }
}
#endif





