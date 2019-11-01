#include "../main_include.h"

static INT8U page_buffer[SIZE_OF_CLUSTER];
#ifdef __SOFT_SIMULATOR__
INT16U get_flash_page_count(void)
{
	return 2048;
}
#else
INT16U get_flash_page_count(void)
{
	return 2048;
}
#endif
/*+++
   ���ܣ��ļ�ϵͳ���õ�ҳ�����
---*/
BOOLEAN  os_flash_erase_page(INT16U page)
{
	return nor_flash_erase_page(page+VFM_MAX_PAGE_COUNT+FLADDR_VFM_PAGE_OFFSET);
}

/*+++
  ���ܣ��ļ�ϵͳ���õ�д����
  ������
        INT16U page,               ԭҳ��
        INT16U page_offset,        ԭҳ����ƫ��
        INT16U newPage,            ��ҳ��
        unsigned char *buf,        ������
        INT16U len                 ����������
        CLUSTER_HEAD *cluster_head ҳͷ��Ϣ
  ����:
        ʵ��д��ĳ���
  ������
        ����¾�����ҳ�治��ͬһƬ�ڣ�����Ҫ��������ת�ƣ������Ҫһ��ҳ����ڴ�ִ�������Ĺ�����
---*/
INT16U  fwrite_flashArray(INT16U page,INT16U page_offset,INT16U newPage,unsigned char *buf,INT16U len,CLUSTER_HEAD *cluster_head)
{
      INT16U  write_len;
      INT16U  page_left;
      INT16U  idx;


      if(buf==NULL) return 0;


      page_left = SIZE_OF_CLUSTER  - page_offset;
      if(len > page_left) write_len = page_left;
      else                write_len = len;
      if(write_len == 0) return 0;



      //��������ź���
      OSMutexPend(&SIGNAL_FLASHSPI);

      //��ȡָ��ҳ�����ݵ���������
      nor_flash_read_data(page+VFM_MAX_PAGE_COUNT+FLADDR_VFM_PAGE_OFFSET,0,page_buffer,SIZE_OF_CLUSTER);

      //��������
      mem_cpy(page_buffer+page_offset,buf,write_len);
      mem_cpy(page_buffer,(INT8U *)cluster_head,sizeof(CLUSTER_HEAD));


      //����ҳ��
      if(nor_flash_erase_page(newPage+VFM_MAX_PAGE_COUNT+FLADDR_VFM_PAGE_OFFSET) == FALSE)
      {
	        OSMutexFree(&SIGNAL_FLASHSPI);
            return 0x8000;
      }
      nor_flash_write_data(newPage+VFM_MAX_PAGE_COUNT+FLADDR_VFM_PAGE_OFFSET,0,page_buffer,SIZE_OF_CLUSTER);

      //Ϊ��ȷ��д�ɹ�������д����֤����,д�����ݲ���ȷʱ��Ҫ��ҳд��
    if(check_flash_data(newPage+VFM_MAX_PAGE_COUNT+FLADDR_VFM_PAGE_OFFSET,0,page_buffer,SIZE_OF_CLUSTER) == FALSE)
    {
         OSMutexFree(&SIGNAL_FLASHSPI);
         return 0x8000;
    }

      //�ͷ��ź��� 
     OSMutexFree(&SIGNAL_FLASHSPI);

      return write_len;
}

/*+++
  ���ܣ��ļ�ϵͳ���õĽضϺ���
  ������
        INT16U page,               ԭҳ��
        INT16U page_max_size,      ҳ������С
        INT16U newPage,            ��ҳ��
        CLUSTER_HEAD *cluster_head ҳͷ��Ϣ
  ����:
        ��ҳʵ�����ݳߴ��С
  ������
---*/
INT16U  ftrim_flashArray(INT16U page,INT16U page_max_size,INT16U newPage,CLUSTER_HEAD *cluster_head)
{
      INT16U  write_len;
      INT16U  page_left,page_offset;
      INT16U  idx;


      page_offset = sizeof(CLUSTER_HEAD);

      //��������ź���
      OSMutexPend(&SIGNAL_FLASHSPI);



      //��ȡָ��ҳ�����ݵ���������     
      nor_flash_read_data(page+VFM_MAX_PAGE_COUNT+FLADDR_VFM_PAGE_OFFSET,0,page_buffer,page_max_size+page_offset);
      mem_set(page_buffer+page_offset+page_max_size,SIZE_OF_CLUSTER-page_max_size-page_offset,0xFF);
      //��������
      mem_cpy(page_buffer,(INT8U *)cluster_head,sizeof(CLUSTER_HEAD));



      //����ҳ��
      if(nor_flash_erase_page(newPage+VFM_MAX_PAGE_COUNT+FLADDR_VFM_PAGE_OFFSET) == FALSE)
      {
	        OSMutexFree(&SIGNAL_FLASHSPI); 
            return 0x8000;  
      }  
      //д������
      nor_flash_write_data(newPage+VFM_MAX_PAGE_COUNT+FLADDR_VFM_PAGE_OFFSET,0,page_buffer,SIZE_OF_CLUSTER);
	  

      if(check_flash_data(newPage+VFM_MAX_PAGE_COUNT+FLADDR_VFM_PAGE_OFFSET,0,page_buffer,SIZE_OF_CLUSTER) == FALSE)
      {
              OSMutexFree(&SIGNAL_FLASHSPI);
              return 0x8000;
      }
		

      //�ͷ��ź��� 
     OSMutexFree(&SIGNAL_FLASHSPI);

      return page_max_size;
}


/*+++
  ���ܣ��ļ�ϵͳ���õ�д����,����д�ķ�ʽ��������
  ������
        INT16U page,               ԭҳ��
        INT16U page_offset,        ԭҳ����ƫ��
        unsigned char *buf,        ������
        INT16U len                 ����������
  ����:
        ʵ��д��ĳ���
        ���λ:  0,
                 1,������в�����ʽ��д 
 ������
---*/
INT16U  fupdate_flashArray(INT16U page,INT16U page_offset,INT8U *buf,INT16U len)
{
      INT16U  write_len;
      INT16U  page_left;
      INT16U  idx;
      BOOLEAN  need_update;

      if(buf==NULL) return 0;


      page_left = SIZE_OF_CLUSTER  - page_offset;
      if(len > page_left) write_len = page_left;
      else                write_len = len;
      if(write_len == 0) return 0;



      //��������ź���
      OSMutexPend(&SIGNAL_FLASHSPI);

      //��ȡָ��ҳ�����ݵ���������
      nor_flash_read_data(page+VFM_MAX_PAGE_COUNT+FLADDR_VFM_PAGE_OFFSET,0,page_buffer,SIZE_OF_CLUSTER);



      //���ȼ����������Ƿ���ͬ����ͬ����д�롣
      need_update = FALSE;
      for(idx=page_offset; idx < page_offset + write_len; idx++)
      {
         if(page_buffer[idx] != buf[idx-page_offset])
         {
            need_update = TRUE;
            break;
         }
      }
      if(FALSE == need_update)
      {
            //�ͷ��ź���           
             OSMutexFree(&SIGNAL_FLASHSPI);
             return write_len;
      }

      //������Ҫ���£���һ������дλ���Ƿ�Ϊȫ0xFF
      for(idx=page_offset; idx < page_offset + write_len; idx++)
      {
          if(page_buffer[idx] != 0xFF)
          {
             OSMutexFree(&SIGNAL_FLASHSPI);
             return 0x8000;
          }
      }
  
      //��������
      nor_flash_write_data(page+VFM_MAX_PAGE_COUNT+FLADDR_VFM_PAGE_OFFSET,page_offset,buf,write_len);

      //Ϊ��ȷ��д�ɹ�������д����֤���ƣ�����ʱ����Ҫ��֤��ǰд������,д�����ݲ���ȷʱ��Ҫ��ҳд��
      nor_flash_read_data(page+VFM_MAX_PAGE_COUNT+FLADDR_VFM_PAGE_OFFSET,0,page_buffer,SIZE_OF_CLUSTER);
      for(idx=0;idx<write_len;idx++)
      {
            if(page_buffer[page_offset+idx]!=buf[idx])
            {
                 OSMutexFree(&SIGNAL_FLASHSPI);
                 return 0x8000;
            }
      }

      //�ͷ��ź��� 
      OSMutexFree(&SIGNAL_FLASHSPI);

      return write_len;

}




/*+++
  ���ܣ��ļ�ϵͳ���õĶ�����
  ������
        INT16U page,              ԭҳ��
        INT16U page_offset,       ԭҳ����ƫ��
        unsigned char *buf,       ������
        INT16U len                ����������
  ����:
        ʵ�ʶ�ȡ�ĳ���
---*/
INT16U  fread_flashArray(INT16U page,INT16U page_offset,unsigned char *buf,INT16U len)
{
      INT16U  read_len;
      INT16U  page_left;

      if(buf==NULL) return 0;

      page_left = SIZE_OF_CLUSTER   - page_offset;
      if(len > page_left) read_len = page_left;
      else                read_len = len;

      if(read_len == 0) return 0;

      //��������ź���
      OSMutexPend(&SIGNAL_FLASHSPI);

     
      //��ȡָ��ҳ�����ݵ���������
      nor_flash_read_data(page+VFM_MAX_PAGE_COUNT+FLADDR_VFM_PAGE_OFFSET,page_offset,buf,read_len);


      //�ͷ��ź��� 
      OSMutexFree(&SIGNAL_FLASHSPI);

      return read_len;
}
/*+++
  ���ܣ�����ҳ�棬д������
  ������
        INT32U addr,             ���Ե�ַ��PAGE_SIZE�ı���
        unsigned char *buf,      д������
        INT16U len               ���ݳ���
  ���أ�
        ��
  ������
        1���ļ�ϵͳ�У���ȡ��һ���أ���ʼ������Ϣ��
---*/
BOOLEAN erase_write_pagehead(INT32U addr,unsigned char *buf,INT16U len)
{
      INT16U page;

      //��������ź���
      OSMutexPend(&SIGNAL_FLASHSPI);

      page = addr / SIZE_OF_CLUSTER;

      //����ҳ��
      if(os_flash_erase_page(page) == FALSE)   //�޸�ɾ�����ȡ�
      {
             OSMutexFree(&SIGNAL_FLASHSPI);
             return FALSE;
      }        

      //д���ͷ��Ϣ
      nor_flash_write_data(page+VFM_MAX_PAGE_COUNT+FLADDR_VFM_PAGE_OFFSET,0,buf,len);

      //�ͷ��ź��� 
      OSMutexFree(&SIGNAL_FLASHSPI);
      return TRUE;

}
/*+++
  ���ܣ�������ʹ�õ�FLASH��
---*/

INT16U  os_vfm_read_array(INT16U page,INT16U page_offset,unsigned char *buf,INT16U len)
{
      if(page>=VFM_MAX_PAGE_COUNT)
      {
      	return 0;
      }
      //��ȡָ��ҳ�����ݵ���������
      nor_flash_read_data(page+FLADDR_VFM_PAGE_OFFSET,page_offset,buf,len);

      return len;
}


/*+++
  ���ܣ�������ʹ�õ�FLASHд
---*/

void os_vfm_write_array(INT16U page,INT16U page_offset,INT8U *buf,INT16U len)
{
      if(page>=VFM_MAX_PAGE_COUNT)
      {
      	return;
      }
      //д���������ݵ�ָ��ҳ��
       nor_flash_write_data(page+FLADDR_VFM_PAGE_OFFSET,page_offset,buf,len);


}

/*+++
   ���ܣ�������ʹ�õ�ҳ�����
---*/
void  os_vfm_erase_page(INT16U page)
{
	int tmp =VFM_MAX_PAGE_COUNT;
      if(page>=VFM_MAX_PAGE_COUNT)
      {
      	return;
      }
	(void)nor_flash_erase_page(page+FLADDR_VFM_PAGE_OFFSET);

}
/*+++
   ���ܣ�������ʹ�õĿ����
   ������
        INT16U  page   Block���ڵ�page(��1ҳ)
---*/
void  os_vfm_erase_block(INT16U page)
{
       INT16U idx;
       for(idx=0;idx<16;idx++)
       {
       		os_vfm_erase_page(page+FLADDR_VFM_PAGE_OFFSET+idx);
       }
}
/*+++
   ���ܣ�������ʹ�õ�FLASH���ֽ�
---*/

INT8U  os_vfm_read_byte(INT16U page,INT16U page_offset)
{
   INT8U ch;
   os_vfm_read_array(page,page_offset,&ch,1);
   return ch;

}
/*+++
   ���ܣ�������ʹ�õ�FLASHдλ
---*/
BOOLEAN  os_vfm_write_bits(INT16U page,INT16U page_offset,INT8U bitdata,INT8U bitpos)
{
   INT8U temp,check_temp;
   temp=os_vfm_read_byte(page,page_offset);

   if(0==bitdata)
      clr_bit_value(&temp,1,bitpos);
   else
      set_bit_value(&temp,1,bitpos);
   os_vfm_write_array(page,page_offset,&temp,1);
   os_vfm_read_array(page,page_offset,&check_temp,1);

   return (check_temp==temp);
}


//�ͷ����ݿ飬ʹ�ñ�־��ɾ����־����Ϊ0.
void os_vfm_clr_2bits(INT16U page,INT16U page_offset,INT8U bitpos)
{
   INT8U temp,check_temp,idx;

   temp=os_vfm_read_byte(page,page_offset);

   clr_bit_value(&temp,1,bitpos);
   clr_bit_value(&temp,1,bitpos+1);
   for(idx=0;idx<10;idx++)
   {
       os_vfm_write_array(page,page_offset,&temp,1);
       os_vfm_read_array(page,page_offset,&check_temp,1);
       if(check_temp==temp)
       {
            break;
       }
       DelayNmSec(100);
   }
}




/*+++
    ���ܣ���ȡ������ָ��λ�õ��ֽ�
---*/
unsigned char  read_fmByte(INT32U addr)
{
      unsigned char ch;

      vfm_read_array(addr,&ch,1);

      return ch;
}
/*+++
  ���ܣ� дһ���ֽ�
---*/
void  write_fmByte(INT32U addr,unsigned char ch)
{
      vfm_write_array(addr,&ch,1);
}
/*+++
  ���ܣ���ȡ���磬�����ǿ�Ƭ��ȡ
---*/
void  read_fmArray(INT32U addr,void *buf,INT16U len)
{
 
      vfm_read_array(addr,(INT8U*)buf,len);
}

/*+++
  ���ܣ�д���磬�����ǿ�Ƭд
---*/
void  write_fmArray(INT32U addr,void *buf,INT16U len)
{
      vfm_write_array(addr,(INT8U*)buf,len);
}

///////////////////////////////////////////////////////////////////////////////
//
//�ӵ�ַaddr��ʼ���len���ֽڣ��ֽ�ȡֵΪval, �����ǿ�Ƭ���á�
//
void set_fmArray(INT32U addr,unsigned char val,INT16U len)
{
   INT16U wlen;
   INT8U buffer[64];

   mem_set(buffer,64,val);
   while(len>0)
   {
       wlen = (len > 64) ? 64 : len;
       write_fmArray(addr,buffer,wlen);
       addr += wlen;
       len -= wlen;
   }
}
#ifdef __SOFT_SIMULATOR__
#else
BOOLEAN  check_flash_data(INT16U sector,INT16U offset,INT8U *buffer,INT16U len)
{
    unsigned int addr;
    static unsigned char data[FLASH_SECTOR_SIZE];
    BOOLEAN result=FALSE;
    
    if(sector > FLASH_SECTOR_COUNT)
    {
        return 0;
    }
    if(offset > FLASH_SECTOR_SIZE)
    {
        return 0;
    }
    nor_flash_read_data(sector,offset,data,len);
    result = mem_is_same(data,buffer,len);
    return result;
}
#endif


