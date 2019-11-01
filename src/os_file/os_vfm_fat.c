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
   功能：文件系统调用的页面擦除
---*/
BOOLEAN  os_flash_erase_page(INT16U page)
{
	return nor_flash_erase_page(page+VFM_MAX_PAGE_COUNT+FLADDR_VFM_PAGE_OFFSET);
}

/*+++
  功能：文件系统调用的写方法
  参数：
        INT16U page,               原页面
        INT16U page_offset,        原页面内偏移
        INT16U newPage,            新页面
        unsigned char *buf,        数据区
        INT16U len                 数据区长度
        CLUSTER_HEAD *cluster_head 页头信息
  返回:
        实际写入的长度
  描述：
        如果新旧两个页面不在同一片内，则需要进行数据转移，这就需要一个页面的内存执行这样的工作。
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



      //请求操作信号量
      OSMutexPend(&SIGNAL_FLASHSPI);

      //读取指定页面内容到缓冲区中
      nor_flash_read_data(page+VFM_MAX_PAGE_COUNT+FLADDR_VFM_PAGE_OFFSET,0,page_buffer,SIZE_OF_CLUSTER);

      //更新内容
      mem_cpy(page_buffer+page_offset,buf,write_len);
      mem_cpy(page_buffer,(INT8U *)cluster_head,sizeof(CLUSTER_HEAD));


      //擦除页面
      if(nor_flash_erase_page(newPage+VFM_MAX_PAGE_COUNT+FLADDR_VFM_PAGE_OFFSET) == FALSE)
      {
	        OSMutexFree(&SIGNAL_FLASHSPI);
            return 0x8000;
      }
      nor_flash_write_data(newPage+VFM_MAX_PAGE_COUNT+FLADDR_VFM_PAGE_OFFSET,0,page_buffer,SIZE_OF_CLUSTER);

      //为了确保写成功，增加写后验证机制,写入数据不正确时需要换页写入
    if(check_flash_data(newPage+VFM_MAX_PAGE_COUNT+FLADDR_VFM_PAGE_OFFSET,0,page_buffer,SIZE_OF_CLUSTER) == FALSE)
    {
         OSMutexFree(&SIGNAL_FLASHSPI);
         return 0x8000;
    }

      //释放信号量 
     OSMutexFree(&SIGNAL_FLASHSPI);

      return write_len;
}

/*+++
  功能：文件系统调用的截断函数
  参数：
        INT16U page,               原页面
        INT16U page_max_size,      页面最大大小
        INT16U newPage,            新页面
        CLUSTER_HEAD *cluster_head 页头信息
  返回:
        该页实际数据尺寸大小
  描述：
---*/
INT16U  ftrim_flashArray(INT16U page,INT16U page_max_size,INT16U newPage,CLUSTER_HEAD *cluster_head)
{
      INT16U  write_len;
      INT16U  page_left,page_offset;
      INT16U  idx;


      page_offset = sizeof(CLUSTER_HEAD);

      //请求操作信号量
      OSMutexPend(&SIGNAL_FLASHSPI);



      //读取指定页面内容到缓冲区中     
      nor_flash_read_data(page+VFM_MAX_PAGE_COUNT+FLADDR_VFM_PAGE_OFFSET,0,page_buffer,page_max_size+page_offset);
      mem_set(page_buffer+page_offset+page_max_size,SIZE_OF_CLUSTER-page_max_size-page_offset,0xFF);
      //更新内容
      mem_cpy(page_buffer,(INT8U *)cluster_head,sizeof(CLUSTER_HEAD));



      //擦除页面
      if(nor_flash_erase_page(newPage+VFM_MAX_PAGE_COUNT+FLADDR_VFM_PAGE_OFFSET) == FALSE)
      {
	        OSMutexFree(&SIGNAL_FLASHSPI); 
            return 0x8000;  
      }  
      //写入数据
      nor_flash_write_data(newPage+VFM_MAX_PAGE_COUNT+FLADDR_VFM_PAGE_OFFSET,0,page_buffer,SIZE_OF_CLUSTER);
	  

      if(check_flash_data(newPage+VFM_MAX_PAGE_COUNT+FLADDR_VFM_PAGE_OFFSET,0,page_buffer,SIZE_OF_CLUSTER) == FALSE)
      {
              OSMutexFree(&SIGNAL_FLASHSPI);
              return 0x8000;
      }
		

      //释放信号量 
     OSMutexFree(&SIGNAL_FLASHSPI);

      return page_max_size;
}


/*+++
  功能：文件系统调用的写方法,不擦写的方式更新数据
  参数：
        INT16U page,               原页面
        INT16U page_offset,        原页面内偏移
        unsigned char *buf,        数据区
        INT16U len                 数据区长度
  返回:
        实际写入的长度
        最高位:  0,
                 1,必须进行擦除方式的写 
 描述：
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



      //请求操作信号量
      OSMutexPend(&SIGNAL_FLASHSPI);

      //读取指定页面内容到缓冲区中
      nor_flash_read_data(page+VFM_MAX_PAGE_COUNT+FLADDR_VFM_PAGE_OFFSET,0,page_buffer,SIZE_OF_CLUSTER);



      //首先检查更新内容是否相同，相同则不用写入。
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
            //释放信号量           
             OSMutexFree(&SIGNAL_FLASHSPI);
             return write_len;
      }

      //内容需要更新，进一步检查待写位置是否为全0xFF
      for(idx=page_offset; idx < page_offset + write_len; idx++)
      {
          if(page_buffer[idx] != 0xFF)
          {
             OSMutexFree(&SIGNAL_FLASHSPI);
             return 0x8000;
          }
      }
  
      //更新内容
      nor_flash_write_data(page+VFM_MAX_PAGE_COUNT+FLADDR_VFM_PAGE_OFFSET,page_offset,buf,write_len);

      //为了确保写成功，增加写后验证机制，更新时仅需要验证当前写入数据,写入数据不正确时需要换页写入
      nor_flash_read_data(page+VFM_MAX_PAGE_COUNT+FLADDR_VFM_PAGE_OFFSET,0,page_buffer,SIZE_OF_CLUSTER);
      for(idx=0;idx<write_len;idx++)
      {
            if(page_buffer[page_offset+idx]!=buf[idx])
            {
                 OSMutexFree(&SIGNAL_FLASHSPI);
                 return 0x8000;
            }
      }

      //释放信号量 
      OSMutexFree(&SIGNAL_FLASHSPI);

      return write_len;

}




/*+++
  功能：文件系统调用的读方法
  参数：
        INT16U page,              原页面
        INT16U page_offset,       原页面内偏移
        unsigned char *buf,       数据区
        INT16U len                数据区长度
  返回:
        实际读取的长度
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

      //请求操作信号量
      OSMutexPend(&SIGNAL_FLASHSPI);

     
      //读取指定页面内容到缓冲区中
      nor_flash_read_data(page+VFM_MAX_PAGE_COUNT+FLADDR_VFM_PAGE_OFFSET,page_offset,buf,read_len);


      //释放信号量 
      OSMutexFree(&SIGNAL_FLASHSPI);

      return read_len;
}
/*+++
  功能：擦除页面，写入数据
  参数：
        INT32U addr,             线性地址，PAGE_SIZE的倍数
        unsigned char *buf,      写入数据
        INT16U len               数据长度
  返回：
        无
  描述：
        1）文件系统中，新取得一个簇，初始化簇信息。
---*/
BOOLEAN erase_write_pagehead(INT32U addr,unsigned char *buf,INT16U len)
{
      INT16U page;

      //请求操作信号量
      OSMutexPend(&SIGNAL_FLASHSPI);

      page = addr / SIZE_OF_CLUSTER;

      //擦除页面
      if(os_flash_erase_page(page) == FALSE)   //修改删除长度。
      {
             OSMutexFree(&SIGNAL_FLASHSPI);
             return FALSE;
      }        

      //写入簇头信息
      nor_flash_write_data(page+VFM_MAX_PAGE_COUNT+FLADDR_VFM_PAGE_OFFSET,0,buf,len);

      //释放信号量 
      OSMutexFree(&SIGNAL_FLASHSPI);
      return TRUE;

}
/*+++
  功能：仿铁电使用的FLASH读
---*/

INT16U  os_vfm_read_array(INT16U page,INT16U page_offset,unsigned char *buf,INT16U len)
{
      if(page>=VFM_MAX_PAGE_COUNT)
      {
      	return 0;
      }
      //读取指定页面内容到缓冲区中
      nor_flash_read_data(page+FLADDR_VFM_PAGE_OFFSET,page_offset,buf,len);

      return len;
}


/*+++
  功能：仿铁电使用的FLASH写
---*/

void os_vfm_write_array(INT16U page,INT16U page_offset,INT8U *buf,INT16U len)
{
      if(page>=VFM_MAX_PAGE_COUNT)
      {
      	return;
      }
      //写缓冲区数据到指定页面
       nor_flash_write_data(page+FLADDR_VFM_PAGE_OFFSET,page_offset,buf,len);


}

/*+++
   功能：仿铁电使用的页面擦除
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
   功能：仿铁电使用的块擦除
   参数：
        INT16U  page   Block所在的page(第1页)
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
   功能：仿铁电使用的FLASH读字节
---*/

INT8U  os_vfm_read_byte(INT16U page,INT16U page_offset)
{
   INT8U ch;
   os_vfm_read_array(page,page_offset,&ch,1);
   return ch;

}
/*+++
   功能：仿铁电使用的FLASH写位
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


//释放数据块，使用标志和删除标志都变为0.
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
    功能：读取铁电中指定位置的字节
---*/
unsigned char  read_fmByte(INT32U addr)
{
      unsigned char ch;

      vfm_read_array(addr,&ch,1);

      return ch;
}
/*+++
  功能： 写一个字节
---*/
void  write_fmByte(INT32U addr,unsigned char ch)
{
      vfm_write_array(addr,&ch,1);
}
/*+++
  功能：读取铁电，不考虑跨片读取
---*/
void  read_fmArray(INT32U addr,void *buf,INT16U len)
{
 
      vfm_read_array(addr,(INT8U*)buf,len);
}

/*+++
  功能：写铁电，不考虑跨片写
---*/
void  write_fmArray(INT32U addr,void *buf,INT16U len)
{
      vfm_write_array(addr,(INT8U*)buf,len);
}

///////////////////////////////////////////////////////////////////////////////
//
//从地址addr开始填充len个字节，字节取值为val, 不考虑跨片设置。
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


