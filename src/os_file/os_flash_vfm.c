//*****************************************************
//flash虚拟铁电系统
//
//描述：
//   1).采用第三片FLASH的后2M仿以前的30K（实际仿的大小和该值不一致）铁电
//   2）.目前使用块的大小为32字节，总共仿9600个块，仿铁电的大小为30720字节
//
//*****************************************************
#include "../main_include.h"

                                     
#define  VFM_FLASH_SIZE             2097152UL                         //FLASH大小			512*4096，华邦的是8M的。2K*4K
#define  VFM_FLASH_PAGE_SIZE        4096                               //FLASH页大小
#define  VFM_FLASH_BLOCKDATA_SIZE   34                                //块数据大小，包括2个字节块头
#define  VFM_BLOCK_SIZE             32                                 //块大小
#define  VFM_ARRANGE_PAGE_COUNT     8                                 //FLASH前面整理使用
#define  VFM_FLASH_PAGE_BLOCK_COUNT 120                               //FLASH一页块个数
#define  VFM_FLASH_PAGE_COUNT       VFM_MAX_PAGE_COUNT                 //FLASH总页数 512.
#define  VFM_FM_BLOCK_COUNT         (VFM_ARRANGE_PAGE_COUNT*VFM_FLASH_PAGE_BLOCK_COUNT)       //铁电960个块	 8*120
#define  VFM_FM_SIZE                VFM_FM_BLOCK_COUNT*VFM_BLOCK_SIZE                            //铁电大小30720   32*960
#define  VFM_TEMP_PAGE_COUNT        2                                 //预留的临时区域页数
#define  VFM_PAGE_SYS_USE_COUNT     4                                //块使用状态记录页页数,可记录块数为65536 ,是不是2个bit表示一个?,要不没法表示这个多
#define  VFM_USER_PAGE_COUNT        (VFM_FLASH_PAGE_COUNT-VFM_TEMP_PAGE_COUNT-VFM_PAGE_SYS_USE_COUNT-2)   //用户可以使用
#define  VFM_FLASH_BLOCK_COUNT      (VFM_USER_PAGE_COUNT*VFM_FLASH_PAGE_BLOCK_COUNT)    //共60480个块			  504*120

#define  VFM_INVALID_BLOCKIDX       0xFF                              //无效块号
#define  VFM_FLAG_STR               "FMFA"                            //系统标识符，仅为4字节

#define  VFM_TRANSFER_LIMIT         3000                              //剩余空闲块转移限值

//页编号使用情况
#define  VFM_PAGE_SYS               (VFM_FLASH_PAGE_COUNT-1)           //系统标识符页
#define  VFM_PAGE_SYS_ARRANGE       (VFM_FLASH_PAGE_COUNT-2)           //系统整理事务页
#define  VFM_PAGE_SYS_USE           (VFM_FLASH_PAGE_COUNT-VFM_PAGE_SYS_USE_COUNT-2)	//	块使用状态记录页页数,可记录块数为65536 ,是不是2个bit表示一个?,要不没法表示这个多
#define  VFM_PAGE_TEMP              (VFM_PAGE_SYS_USE-VFM_TEMP_PAGE_COUNT)     //预留的临时区域

//VFM_PAGE_SYS系统页地址使用情况
#define  VFM_ADDR_SYSFLAG           0                                 //4,系统标识符页标志位置

//临时页使用情况
//page0   系统F4时转存数据,前2个字节为事务标志，D0=0表示开启事务  D1=0表示事务完成

//页使用情况如下：
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
.....VFM_PAGE_SYS_USE(30).......    块使用状态记录页
.....VFM_PAGE_SYS_ARRANGE.......	整理页
.....VFM_PAGE_SYS.......	 系统标识
*/

typedef struct
{
   INT8U  flash_block[3];
}FM_FLASH_BLOCK_MAP;


typedef struct
{
   INT32U    cur_free_block;             //当前空闲块索引
   INT16U    cur_arrange_block;           //当前整理的块索引
   INT16U    cur_transfer_block;          //当前转移的块索引
   INT8U     arrange_flag;               //整理标志   0-未整理  1-整理
   INT8U     transfer_flag;              //转移标志   0-未转移  1-转移
   INT8U     buffer[66];
   INT8U     check_buffer[66];
//   INT8U     VFM_SIGNAL;			      //使用该虚拟铁电信号量
}FLASH_MANAGE_INFO;

//flash虚拟铁电系统管理变量
static FLASH_MANAGE_INFO OS_VFM_VAR;
//铁电块链表
static FM_FLASH_BLOCK_MAP OS_FM_BLOCK_LIST[VFM_FM_BLOCK_COUNT];//占用大小：4200*3=12600字节		,现在是960个



void vfm_init(INT8U isformat,void (*calluserfun)(void));//flash虚拟铁电系统初始化工作
void vfm_format();//flash虚拟铁电系统格式化
void vfm_page_arrange(void);//空闲页整理
INT32U vfm_get_free_block(void);//获取空闲块
void vfm_read_array(INT32U addr,INT8U *data,INT16U len);//读数据
void vfm_write_array(INT32U addr,INT8U *data,INT16U len);//写数据
BOOLEAN vfm_check_flag(void);//检查系统标识符
BOOLEAN check_valid_block(FM_FLASH_BLOCK_MAP *block_info,INT8U block_seq);//对比检查两个块的有效性
INT8U calc_bit_count(INT8U* BS,INT8U byteLen,INT8U bitLen);//计算位串中有多少个0
BOOLEAN check_is_write(INT8U ch1,INT8U ch2);//检查该字节是否可写
void vfm_transfer(void);//转移数据
void vfm_arrange(INT8U step,BOOLEAN is_exec);//整理数据
BOOLEAN check_arrange(void);//检查整理数据
void rebuild_block_list(void);//重构块链表
void get_page_pageoffset(INT32U flash_block,INT16U *page,INT16U *page_offset);//计算FLASH的块在第几页和页內偏移字节
void erase_block_status_page(void);//檫除系统块状态页
BOOLEAN use_block(INT32U flash_block);
void update_block_idx(INT8U *data,INT16U fm_block_idx);

#ifdef __SOFT_SIMULATOR__
void print_block_list(void);
#endif

/*+++
  功能：初始化Flash虚拟铁电系统
  参数：
      INT8U isformat 是否格式化
  返回：
      无
  描述：
      1).检查Flash是否已经格式化，如果没有则格式化
      2).根据Flash内容重构相关内存变量
      3).处理由于掉电可能引起的Flash重块问题和未完成的转移数据
---*/
void vfm_init(INT8U isformat,void (*calluserfun)(void))
{

   //初始化全局变量
   OS_VFM_VAR.cur_free_block=VFM_FLASH_BLOCK_COUNT-2;
   OS_VFM_VAR.cur_transfer_block=0;
   //OS_VFM_VAR.VFM_SIGNAL=0;
   //初始化块链表
   mem_set(OS_FM_BLOCK_LIST,sizeof(OS_FM_BLOCK_LIST),0xFF);

   //检查系统标识符
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
         //如果系统标识符检查不能成功，系统无法工作
//         record_sys_operate(OP_SYS_VFM_ERROR);
//         switch_off_flash();
         os_SoftReset();
         return;
      }
   }
   //如果进行个格式化，则无须重构链表，初始化完成
   if(isformat)return;

   //读取块状态，重构映射链表
   rebuild_block_list();

   //检查整理是否完成，没有完成继续整理
   if(check_arrange())return;//如果进行了整理，则不需要重构链表

}

/*+++
  功能：更新链表映射的FLASH块
  参数：
      无
  返回：
  描述：
---*/
void set_block_to_list(INT16U fm_block_idx,INT32U flash_block_idx)
{
   
   OS_FM_BLOCK_LIST[fm_block_idx].flash_block[0]=flash_block_idx & 0xFF;
   OS_FM_BLOCK_LIST[fm_block_idx].flash_block[1]=(flash_block_idx>>8) & 0xFF;
   OS_FM_BLOCK_LIST[fm_block_idx].flash_block[2]=(flash_block_idx>>16) & 0xFF;

}

/*+++
  功能：获取链表映射的FLASH块
  参数：
      无
  返回：
  描述：
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
  功能：重构链表
  参数：
      无
  返回：
      TRUE:检查到系统标识符
      FALSE:没有检查到系统标识符
  描述：
      1).根据块的使用状态重构块链表映射
      2).得到当前空闲块
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
      //一次读取64字节
      for(addr=0;addr<VFM_FLASH_PAGE_SIZE;addr+=64)
      {
         os_vfm_read_array(VFM_PAGE_SYS_USE+page_idx,addr,OS_VFM_VAR.buffer,64);
         //一个块状态占用2个位，一个字节4个块，64个字节共256个块
         for(idx=0;idx<256;idx++)
         {
            byte_offset=idx/4;
            bit_offset=(idx%4)*2;
            flash_block_idx=page_idx*4;
            flash_block_idx*=VFM_FLASH_PAGE_SIZE;
            flash_block_idx+=addr*4+idx;

            is_use=(0==get_bit_value(OS_VFM_VAR.buffer+byte_offset,1,bit_offset));
            is_delete=(0==get_bit_value(OS_VFM_VAR.buffer+byte_offset,1,bit_offset+1));
            if(!is_use || is_delete)continue;//没有使用或已经删除不需要读取块头信息
            OS_VFM_VAR.cur_free_block=flash_block_idx;
            //读取FLASH存储的块头索引
            get_page_pageoffset(flash_block_idx,&page,&page_offset);
            os_vfm_read_array(page,page_offset,(INT8U*)&fm_block_idx,2);
            //刚格式化fm_block_idx为FFFF，则fm_block_idx==flash_block_idx
            if((0xFFFF==fm_block_idx) && (flash_block_idx<VFM_FM_BLOCK_COUNT))
            {
                  fm_block_idx=flash_block_idx;
            }
            if(fm_block_idx<VFM_FM_BLOCK_COUNT)      //flash块头中的存放的fm_block_idx有效
            {
               set_block_to_list(fm_block_idx,flash_block_idx);//更新链表映射的FLASH块
               /*//重构时不获取待转义的块索引了，转移时会重新从链表中检测，这样可以避免因为该索引出现问题导致无法转移
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
  功能：检查系统标识符
  参数：
      无
  返回：
      TRUE:检查到系统标识符
      FALSE:没有检查到系统标识符
  描述：
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
         //读取系统特征字
         os_vfm_read_array(VFM_PAGE_SYS,VFM_ADDR_SYSFLAG,buffer,4);
         if(0 == compare_string(flag,buffer,4))   return TRUE;
      }
      return FALSE;
}

/*+++
  功能：初始化块头索引
  参数：
  返回：
  描述：要考虑写FLASH失败，进行重新写
---*/
BOOLEAN init_blcok_idx(INT16U block_idx)
{
   INT16U  page,page_offset;
   INT8U   idx[2],new_idx[2];
   
   //计算原页号及页偏移量
   get_page_pageoffset(block_idx,&page,&page_offset);
   //更新块头
   update_block_idx(idx,block_idx);
   //写入块头
   os_vfm_write_array(page,page_offset,idx,2);
   //验证写入是否正确
   os_vfm_read_array(page,page_offset,new_idx,2);
   if(0 != compare_string(idx,new_idx,2))
   {
		return FALSE;
   }
      return  TRUE;
}

/*+++
  功能：初始化块的使用状态
  参数：BOOLEAN is_init_block_idx  是否初始化Flash的块头信息
  返回：写FLASH失败,返回FALSE,由上级进行处理
  描述：1).格式化后调用，需要初始化Flash的块头信息
        2).整理完成后调用，不需要初始化Flash的块头信息
---*/
BOOLEAN init_block_status(BOOLEAN is_init_block_idx)
{
   INT16U idx,page,page_offset,count,block_idx;
   //格式化完成后，映射铁电的块到FLASH的前4200,并初始化前4200个块的块头索引
   for(idx=0;idx<VFM_FM_BLOCK_COUNT;idx++)
   {
      set_block_to_list(idx,idx);//更新链表映射的FLASH块
      if(is_init_block_idx)
      {
         if(init_blcok_idx(idx)==FALSE)
         {
         	return FALSE;
         }
      }
   }
   //更新前35页块的使用状态为使用
   //一次更新64个字节共64*4=256个块,一个页存储4096*4=16384个块,由于总块数为4200，而按一次64个字节设置，16次设置4096个块，还剩余4200-4096=104
   mem_set(OS_VFM_VAR.buffer,64,0xAA);
   count=(VFM_FM_BLOCK_COUNT/256)*256;
   for(idx=0;idx<count;idx+=256)
   {
      page=idx/(VFM_FLASH_PAGE_SIZE*4);
      page_offset=(idx%(VFM_FLASH_PAGE_SIZE*4))/4; 
      os_vfm_write_array(VFM_PAGE_SYS_USE+page,page_offset,OS_VFM_VAR.buffer,64);
      //读取验证写入是否正确
      os_vfm_read_array(VFM_PAGE_SYS_USE+page,page_offset,OS_VFM_VAR.check_buffer,64);
      if(0 != compare_string(OS_VFM_VAR.buffer,OS_VFM_VAR.check_buffer,64))
      {
           return FALSE;
      }
   }
   //设置剩余块的状态，剩余块数如果不是4的整数倍，还需要按位设置剩下的!!!
   count=(VFM_FM_BLOCK_COUNT%256)/4;
   if(count>0)
   {
      page=idx/(VFM_FLASH_PAGE_SIZE*4);//注意：idx为上面for的结果
      page_offset=(idx%(VFM_FLASH_PAGE_SIZE*4))/4;
      os_vfm_write_array(VFM_PAGE_SYS_USE+page,page_offset,OS_VFM_VAR.buffer,count);
      //读取验证写入是否正确
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

   //设置当前转移指针为初始待转移的块
   OS_VFM_VAR.cur_transfer_block=0;
   //设置当前空闲块指针铁电整理后的最后一块。
   OS_VFM_VAR.cur_free_block=VFM_FM_BLOCK_COUNT;
   return TRUE;
}

/*+++
  功能：檫除整理所需的页
  参数：
  返回：
  描述：
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
  功能：flash虚拟铁电系统格式化
  参数：
  返回：
  描述：
---*/
void vfm_format(void)
{
   INT16U idx,page,page_offset;
   INT8U flag[4],check_flag[4];

   for(idx=0;idx<10;idx++)
   {
   //擦除系统标识符页
   os_vfm_erase_page(VFM_PAGE_SYS);
   //擦除块状态记录页
   erase_block_status_page();
   //擦除系统整理事务页
   os_vfm_erase_page(VFM_PAGE_SYS_ARRANGE);
   //整理所需要的页
   erase_arrange_page();

       //初始化失败，则从新执行格式化
       if(FALSE==init_block_status(TRUE))
       {
       		continue;
       }
   //写入标识符
   flag[0]='F';
   flag[1]='M';
   flag[2]='F';
   flag[3]='L';
       //写失败后则重新执行格式化
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
  功能：计算位串中有多少个0
  参数：
        INT8U *BS       位串指针
        INT8U byteLen   位串的字节长度
        INT8U bitLen    位串的位长度
  返回：
      位串中有多少位为0
  描述：
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
  功能：检查该字节是否可写
  参数：
      INT8U ch1   待检查字节
      INT8U ch2   目的字节
  返回：
      TRUE  可写
      FALSE 不可写
  描述：
      按位对比，如果存在将0变为1则不可写，否则可写

      1)  0 ^ 0 = 0
          1 ^ 1 = 0
          所以  ch1^ch2 == 0，则表示每位都不变化，则可写。
      2)  1 ^ 0 = 1
          0 ^ 1 = 1
          所以 ，ch3 = ch1 ^ ch2 不为零，则表示有不同位变化，允许的变化是从1->0
          所以，用ch3 & ch2, 则ch2位值为0时，ch3会变为0，这时是允许的。
          反之，如果ch2的位值为1，则需要从ch1的0变为ch2的1，这时ch3仍为1，这是不允许的。 
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
  功能：检查2个缓冲区数据是否相同
  参数：
      INT8U *data1  缓冲区1
      INT8U *data2  缓冲区2
      INT16U len  检查长度
  返回：
  描述：
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
  功能：使用一个指定的块，更改该块的状态位为使用状态
  参数：
      INT32U flash_block    Flash块的序号
      INT8U  pos  0-使用位  1-删除位
  返回：写flash失败，需要返回FALSE，上级重新换块处理
  描述：
---*/
BOOLEAN use_block(INT32U flash_block)
{
   INT16U page,page_offset,page_block,offset;

   //根据块号，计算所在页及偏移量
   page=flash_block/(VFM_FLASH_PAGE_SIZE*4);
   page_block=flash_block%(VFM_FLASH_PAGE_SIZE*4);
   page_offset=page_block/4;
   offset=page_block%4;
   //更改位状态
   return os_vfm_write_bits(VFM_PAGE_SYS_USE+page,page_offset,0,offset*2);
}

/*+++
  功能：释放指定的FLASH块,更改该块的状态为使用和删除
  参数：
      INT32U flash_block    Flash块的序号
  返回：
  描述：写FLASH未成功，则执行多次
---*/
void release_block(INT32U flash_block)
{
   INT16U page,page_offset,page_block,offset;

   //根据块号，计算所在页及偏移量
   page=flash_block/(VFM_FLASH_PAGE_SIZE*4);
   page_block=flash_block%(VFM_FLASH_PAGE_SIZE*4);
   page_offset=page_block/4;
   offset=page_block%4;

   //更改位状态
   os_vfm_clr_2bits(VFM_PAGE_SYS_USE+page,page_offset,offset*2);
}

/*+++
  功能：计算FLASH的块在第几页和页內偏移字节
  参数：
      INT32U flash_block  FLASH块索引
      INT16U *page  输出,该块所在的页号 
      INT16U *page_offset  输出,该块所在页的字节偏移量
  返回：
  描述：
---*/
void get_page_pageoffset(INT32U flash_block,INT16U *page,INT16U *page_offset)
{
   *page=flash_block/VFM_FLASH_PAGE_BLOCK_COUNT;
   *page_offset=(flash_block%VFM_FLASH_PAGE_BLOCK_COUNT)*VFM_FLASH_BLOCKDATA_SIZE;
}

/*+++
  功能：验证一个块的数据是否可以覆盖写入
  参数：
      INT8U *buffer1 原块缓冲区
      INT8U *buffer2 新块缓冲区
      INT8U len     比较长度
  返回：TRUE:可以覆盖写入   FALSE:不能覆盖写入
  描述：
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
  功能：将块序号更新到缓冲区中
  参数：
      INT8U *data    块缓冲区
      INT16U block_idx 铁电块序号
      INT8U len     比较长度
  返回：
  描述：
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
  功能：读数据
  参数：
      INT32U addr  读取数据的地址
      INT8U *data  读取数据返回的内容
      INT16U len  读取数据的长度
  返回：
  描述：
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
     if(block_idx>=VFM_FM_BLOCK_COUNT)//访问地址，超出系统支持大小
     {
        OSMutexFree(&SIGNAL_FLASHSPI);   
         mem_set(data,len,0xFF);
         return;
     }
     flash_block_idx=get_block_from_list(block_idx);//从链表映射获取对于的FLASH块
     //如果还没有写入数据或该块已经删除，则初始化为0xFF
     if(flash_block_idx>=VFM_FLASH_BLOCK_COUNT)
     {
         mem_set(data,datalen,0xFF);

     }else
     {
          //计算FLASH的块在第几页和页內偏移字节
          get_page_pageoffset(flash_block_idx,&page,&page_offset);
          page_offset+=block_offset+2;//增加块内偏移及块头大小
          //读取块数据
          os_vfm_read_array(page,page_offset,data,datalen);
     }
     len-=datalen;
     addr+=datalen;
     data+=datalen;
   }
   OSMutexFree(&SIGNAL_FLASHSPI);
}

/*+++
  功能：写数据
  参数：
      INT32U addr  待写数据的地址
      INT8U *data  待写数据的内容
      INT16U len  待写数据的长度
  返回：
  描述：
      1).写入数据与原数据相同，则不写入
      2).写入数据如果可以覆写，则覆盖写入，否则换块写入
      3).写入的数据如果与原数据不一致，则重新换页写入
      4).如果可以覆盖写入,则不要整块写入，而是按实际大小写入
      5)写Flash失败，则重新申请块写入
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
     //检查如果没有可用的空闲块（空闲块数量小于1），则整理
     vfm_arrange(0xEE,FALSE);
     //得到原块号
     old_flash_block_idx=get_block_from_list(block_idx);//从链表映射获取对于的FLASH块

     //计算FLASH的块在第几页和页內偏移字节
     get_page_pageoffset(old_flash_block_idx,&page,&page_offset);
     //则先读取原数据，比较原数据和新数据，如果可写，则直接写入，否则换块写入
     os_vfm_read_array(page,page_offset,OS_VFM_VAR.buffer,VFM_BLOCK_SIZE+2);
     //如果待写入数据和目的数据相同，则不需要写入
     if(!chek_data_same(OS_VFM_VAR.buffer+block_offset+2,data,datalen))
     {
        //验证原块数据区域是否可以覆盖写入,如果不可以则换块写入，并需要先装载与数据与新数据进行合并
        is_update=check_block_allow_write(OS_VFM_VAR.buffer+2+block_offset,data,datalen);

        //合并，先有数据到原数据中,能够写入没有必要合并，只有写入失败后才需要合并
        mem_cpy(OS_VFM_VAR.buffer+block_offset+2,data,datalen);
       
        //得到写入数据的长度和起始地址
        if(is_update)
        {
            //如果是格式化产生的，有块的状态没有块头信息，则从块头位置开始写
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

         //更新块序号
         update_block_idx(OS_VFM_VAR.buffer,block_idx);

         //默认在原页上进行写入数据
         new_flash_block_idx=old_flash_block_idx;
         while(1)
         {
           //获取一个空闲块
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

              //计算FLASH的块在第几页和页內偏移字节
              get_page_pageoffset(new_flash_block_idx,&page,&page_offset);
              //如果需要换页写入则重置写入长度和起始指针
              write_len=VFM_BLOCK_SIZE+2;
              data_ptr=OS_VFM_VAR.buffer;
              offset=page_offset;
           }
           //更新数据到FLASH
           #ifdef __SOFT_SIMULATOR__
           //snprintf(info,200,"vfm write array block_idx = %d,old_flash_block_idx=%d,new_flash_block_idx=%d,task_no=%d",block_idx,old_flash_block_idx,new_flash_block_idx,os_cur_task);
           //println_info(info);
           #endif
           //检查写入页地址是否有效，非法则不写入
           if(page<VFM_USER_PAGE_COUNT)
           {
              os_vfm_write_array(page,offset,data_ptr,write_len);
              //验证写入数据是否正确，如果不正确，重新获取一个新块，再次写入
              os_vfm_read_array(page,offset,OS_VFM_VAR.check_buffer,write_len);
              //读取块头，并验证块头是否正确，如果不正确，重新获取一个新块，再次写入
              os_vfm_read_array(page,page_offset,(INT8U*)(&tmp_idx),2);
              if(chek_data_same(data_ptr,OS_VFM_VAR.check_buffer,write_len) && (tmp_idx==block_idx))
              {
                 if(FALSE==is_update)
                 { 
                    //更新块使用标记
                    if(use_block(new_flash_block_idx))
                    {
                        //更新块删除标记
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

           //如果是更新原块，则不能先删除原块
           if(FALSE == is_update)
           { 
               //由于该块写入数据和原数据不一致，需要换页重新写入，因此将该块打上删除标志
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
//            //更新块使用标记
//            use_block(new_flash_block_idx);
//            //更新块删除标记
//            if(old_flash_block_idx<VFM_FLASH_BLOCK_COUNT)
//            {
//               release_block(old_flash_block_idx);
//            }
//         }
         //更新链表的FLASH块序号
         set_block_to_list(block_idx,new_flash_block_idx);//更新链表映射的FLASH块
     }
     len-=datalen;
     addr+=datalen;
     data+=datalen;
   }
   //每次写完数据检测是否需要转移数据
   vfm_transfer();
   OSMutexFree(&SIGNAL_FLASHSPI);
   
}

/*+++
  功能：获取空闲块
  参数：
  返回：已经获取到的空闲块序号
  描述：
---*/
INT32U vfm_get_free_block(void)
{
      INT16U page,page_offset;
      if(OS_VFM_VAR.cur_free_block<VFM_FLASH_BLOCK_COUNT-1)
      {
         //如果该块位于一页的第一个块，则先檫除整页
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
         //没有空闲块，需要整理数据, 从机制上讲不可能，复位！！
         switch_off_flash();
         os_SoftReset();

      }
      OS_VFM_VAR.cur_free_block++;

     return (OS_VFM_VAR.cur_free_block-1);
}

/*+++
  功能：读取察写次数
  参数：
  返回：
  描述：最大支持4092次，超过该值不做记录
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
  功能：更新察写次数
  参数：
  返回：
  描述：最大支持4092次，超过该值不做记录
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
  功能：移动块数据
  参数：
  返回：转移失败,则返回FALSE
  描述：1).当空闲块小于某值时，且块链表中存在小于2048的块，则开始转移前32页的块数据
        2).每次仅转移一个块
        
---*/
BOOLEAN move_blcok(INT32U old_block_idx,INT32U new_block_idx,INT16U fm_block_idx)
{
   INT16U  page,page_offset;
   INT8U buffer[VFM_FLASH_BLOCKDATA_SIZE];
   //计算原页号及页偏移量
   get_page_pageoffset(old_block_idx,&page,&page_offset);
   //读取原块数据
   os_vfm_read_array(page,page_offset,OS_VFM_VAR.check_buffer,VFM_FLASH_BLOCKDATA_SIZE);
   //更新块头，因为如果初次未使用则，块头为FF
   update_block_idx(OS_VFM_VAR.check_buffer,fm_block_idx);
   //计算新页号及页偏移量
   get_page_pageoffset(new_block_idx,&page,&page_offset);
   //写入新块数据
   os_vfm_write_array(page,page_offset,OS_VFM_VAR.check_buffer,VFM_FLASH_BLOCKDATA_SIZE);
   //写后验证
   os_vfm_read_array(page,page_offset,buffer,VFM_FLASH_BLOCKDATA_SIZE);
   if(0 != compare_string(buffer,OS_VFM_VAR.check_buffer,VFM_FLASH_BLOCKDATA_SIZE))
   {
       return FALSE;
   }
   return TRUE;
   
}

/*+++
  功能：檫除块状态页
  参数：
  返回：
  描述：1).总共30个页
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
  功能：数据转移，将前32页数据转移到32页以后
  参数：
  返回：
  描述：1).当空闲块小于某值时，且块链表中存在小于2048的块，则开始转移前32页的块数据
        2).每次仅转移一个块,且确保每次都能够转移一块
        3) 该函数只允许在vfm_write_array中调用！!
        4)转移失败,则申请新块转移
---*/
void vfm_transfer(void)
{
   INT32U old_block_idx,new_block_idx;
   INT16U idx;

   //如果已经转移完成，则无须转移
   if(OS_VFM_VAR.cur_transfer_block>=VFM_FM_BLOCK_COUNT)return;

   //空闲块剩余比较多时，无须转移
   if((VFM_FLASH_BLOCK_COUNT-OS_VFM_VAR.cur_free_block)>VFM_TRANSFER_LIMIT)return;
   
   while(OS_VFM_VAR.cur_transfer_block<VFM_FM_BLOCK_COUNT)
   {
       //如果该块不在前32页则，不转移
       old_block_idx=get_block_from_list(OS_VFM_VAR.cur_transfer_block);//从链表映射获取对于的FLASH块
       if(old_block_idx<VFM_FM_BLOCK_COUNT) break;
       OS_VFM_VAR.cur_transfer_block++;
   }

   if(OS_VFM_VAR.cur_transfer_block < VFM_FM_BLOCK_COUNT)
   { 
     //得到当前要转移的块
      old_block_idx=get_block_from_list(OS_VFM_VAR.cur_transfer_block);//从链表映射获取对于的FLASH块
    
     if(old_block_idx<VFM_FM_BLOCK_COUNT)
     {
        for(idx=0;idx<3;idx++)
        {
        //获取一个空闲块、
        new_block_idx=vfm_get_free_block();

        //移动数据到空闲块
            if(FALSE==move_blcok(old_block_idx,new_block_idx,OS_VFM_VAR.cur_transfer_block))
            {
                DelayNmSec(100);
            	continue;
            }

        //更新新块为已使用
            if(FALSE==use_block(new_block_idx))
            {
                DelayNmSec(100);
            	continue;
            }
        //更新块链表
     set_block_to_list(OS_VFM_VAR.cur_transfer_block,new_block_idx);//更新链表映射的FLASH块
        //更新原块为已删除
        release_block(old_block_idx);
        //移动当前转移指针为下一个
        OS_VFM_VAR.cur_transfer_block++;
            break;
        }
     } 
   }

   

}

/*+++
  功能：写整理数据事务标志
  参数：
  返回：
  描述：写失败，有三取二保证不需要考虑
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
  功能：读取事务整理标志
  参数：
  返回：
  描述：按位3取2
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
  功能：数据整理，将33页后的有效数据整理到前33页
  参数：
  返回：
  描述：1).当空闲块小于某值时，且块链表中已经没有2046以前的有效块时，采用块察除前33页，开始整理
        2).整理每次整理所有有效块
        3).如果已经开始整理且没有整理完时掉电，下次开机后重新檫除前32页后，再整理
        4).如果已经整理完成且檫除了8页的块状态时掉电，则不需要再整理
        5).整理失败，重新整理
---*/
void vfm_arrange(INT8U step,BOOLEAN is_exec)
{
   INT32U old_block_idx,new_block_idx;
   INT16U idx,page,page_offset,write_count;

   //空闲块剩余比较多时，无须转移
   if((VFM_FLASH_BLOCK_COUNT>OS_VFM_VAR.cur_free_block+2)&&(is_exec==FALSE)) return;
  #ifdef __SOFT_SIMULATOR__
  snprintf(info,100,"vfm_arrange !!!");
  println_info(info);
  #endif
   OSMutexPend(&SIGNAL_FLASHSPI);
   switch(step)
   {
     case 0xEE:
       //整理前，先檫除整理事务页
       os_vfm_erase_page(VFM_PAGE_SYS_ARRANGE);
       //写入整理事务标志：开始整理
       update_arrange_trans(0xEE);
       //檫除整理所需的页
       erase_arrange_page();
       //整理所需要的页
       for(new_block_idx=0;new_block_idx<VFM_FM_BLOCK_COUNT;new_block_idx++)
       {
         //得到当前需要整理的块
         old_block_idx=get_block_from_list(new_block_idx);//从链表映射获取对于的FLASH块
         //移动数据到空闲块
         if(FALSE == move_blcok(old_block_idx,new_block_idx,new_block_idx))
         {
   			OSMutexFree(&SIGNAL_FLASHSPI);
            return;
         }
         //更新块链表
         set_block_to_list(new_block_idx,new_block_idx);//更新链表映射的FLASH块
       }
       //写入整理事务标志：导数据完成
       update_arrange_trans(0xCC);
     case 0xCC:
        //檫除块状态页
       erase_block_status_page();
       //写入整理事务标志：檫除状态页完成
       update_arrange_trans(0x88);
     case 0x88:
       //初始化块的状态
       if(FALSE==init_block_status(FALSE))
       {
       		erase_block_status_page();
   			OSMutexFree(&SIGNAL_FLASHSPI);
            return;

       }
       //整理完成，事务状态为00
       update_arrange_trans(0x00);
       break;
     default:
       update_arrange_trans(0x00);
     	 break;	
    }
   //更新FLASH檫写次数
   write_count=vfm_read_write_count();
   vfm_update_write_count(write_count);

   OSMutexFree(&SIGNAL_FLASHSPI);
}

/*+++
  功能：检查整理数据，系统初始化时调用
  参数：
  返回：
  描述：1).检查整理是否已经完成，没有完成继续整理
        2).根据整理事务标志，完成未完成的整理工作
---*/
BOOLEAN check_arrange(void)
{
   INT8U flag;
   //读取事务标志
   //os_vfm_read_array(VFM_PAGE_SYS_ARRANGE,0,&flag,1);
   flag=read_arrange_trans();
   //如果整理事务未完成，则继续整理
   if((0xFF!=flag) && (0x00!=flag))
   {
      vfm_arrange(flag,TRUE);
      return TRUE;
   }

  return FALSE;
}

/*+++
  功能：檫除系统预留的页
  参数：
  返回：
  描述：
---*/
void vfmflash_temp_page_erase(INT16U temp_page_idx)
{
   if(temp_page_idx>=VFM_TEMP_PAGE_COUNT)return;
   os_vfm_erase_page(VFM_PAGE_TEMP+temp_page_idx);
}

/*+++
  功能：对系统预留的页进行写数据
  参数：
  返回：
  描述：
---*/
void vfmfalsh_temp_write_array(INT16U temp_page_idx,INT16U page_offset,INT8U *data,INT16U len)
{
   if(temp_page_idx>=VFM_TEMP_PAGE_COUNT)return;
   os_vfm_write_array(VFM_PAGE_TEMP+temp_page_idx,page_offset,data,len);
}

/*+++
  功能：对系统预留的页进行读数据
  参数：
  返回：
  描述：
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





