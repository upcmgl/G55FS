//---------------------------------------------------------------------------


#pragma hdrstop

#include "main_include.h"
#include "Unit_vfm_memory.h"
#ifdef __USE_FLASH_DAT__
#define  OS_VFM_MEMORY_FILE "FLASH3.DAT"
#else
#define  OS_VFM_MEMORY_FILE "OS_VFM_MEMORY.DAT"
#endif


//---------------------------------------------------------------------------

#pragma package(smart_init)
#ifdef __32MX695F512L_H
static TFileStream  *FS_OS_VFM_MEMORY=NULL;
static HANDLE   hFS_OS_VFM_MEMORY=NULL;


static INT8 flash_buffer[4096];

//---------------------------------------------------------------------------
void open_os_vfm_memory(void)
{

   if(FS_OS_VFM_MEMORY==NULL)
   {
       if(FileExists(OS_VFM_MEMORY_FILE))
       {
          FS_OS_VFM_MEMORY = new TFileStream(OS_VFM_MEMORY_FILE,fmOpenReadWrite|fmShareDenyNone);
       }
       else
       {
          FS_OS_VFM_MEMORY = new TFileStream(OS_VFM_MEMORY_FILE,fmCreate|fmOpenReadWrite|fmShareDenyNone);
       }

   }
   if(hFS_OS_VFM_MEMORY==NULL)
   {
       //hFS_OS_VFM_MEMORY = CreateEvent(NULL,TRUE,TRUE,OS_VFM_MEMORY_FILE);
       hFS_OS_VFM_MEMORY = CreateMutex(NULL,FALSE,OS_VFM_MEMORY_FILE);
   }
}

//---------------------------------------------------------------------------
void close_os_vfm_memory(void)
{
   if(FS_OS_VFM_MEMORY!=NULL)
   {
      delete FS_OS_VFM_MEMORY;
      FS_OS_VFM_MEMORY = NULL;
   }

}


//---------------------------------------------------------------------------
INT16U  os_vfm_read_array(INT16U page,INT16U page_offset,unsigned char *buf,INT16U len)
{
      #ifdef __USE_FLASH_DAT__
      INT32U addr=(page+4096)*4096+page_offset;
      #else
      INT32U addr=(page+FLADDR_VFM_PAGE_OFFSET)*4096+page_offset;
      #endif
      WaitForSingleObject(hFS_OS_VFM_MEMORY,INFINITE);
      open_os_vfm_memory();

      FS_OS_VFM_MEMORY->Seek(addr,soFromBeginning);
      FS_OS_VFM_MEMORY->Read(buf,len);
      close_os_vfm_memory();

      //SetEvent(hFS_OS_VFM_MEMORY);
      ReleaseMutex(hFS_OS_VFM_MEMORY);
      return len;
}

//---------------------------------------------------------------------------

void    os_vfm_write_array(INT16U page,INT16U page_offset,INT8U *buf,INT16U len)
{
      #ifdef __USE_FLASH_DAT__
      INT32U addr=(page+4096)*4096+page_offset;
      #else
      INT32U addr=(page+FLADDR_VFM_PAGE_OFFSET)*4096+page_offset;
      #endif
      WaitForSingleObject(hFS_OS_VFM_MEMORY,INFINITE);
      open_os_vfm_memory();

      if(addr>16*1024*1024)                             
      {

           int a=0;

      }

      FS_OS_VFM_MEMORY->Seek(addr,soFromBeginning);
      if(buf!=NULL)
      {
         FS_OS_VFM_MEMORY->Write(buf,len);
      }
      else
      {
         unsigned char ch=0;
         for(int i=0;i<len;i++) FS_OS_VFM_MEMORY->Write(&ch,1);
      }


      close_os_vfm_memory();

      //SetEvent(hFS_OS_VFM_MEMORY);
      ReleaseMutex(hFS_OS_VFM_MEMORY);
}
INT8U os_vfm_read_byte(INT16U page,INT16U page_offset)
{
      INT8U temp;
      #ifdef __USE_FLASH_DAT__
      INT32U addr=(page+4096)*4096+page_offset;
      #else
      INT32U addr=(page+FLADDR_VFM_PAGE_OFFSET)*4096+page_offset;
      #endif
      WaitForSingleObject(hFS_OS_VFM_MEMORY,INFINITE);
      open_os_vfm_memory();



      FS_OS_VFM_MEMORY->Seek(addr,soFromBeginning);
      FS_OS_VFM_MEMORY->Read(&temp,1);

      close_os_vfm_memory();

      //SetEvent(hFS_OS_VFM_MEMORY);
      ReleaseMutex(hFS_OS_VFM_MEMORY);
      return temp;
}
BOOLEAN os_vfm_write_bits(INT16U page,INT16U page_offset,INT8U bitdata,INT8U bitpos)
{
      INT8U temp;
      #ifdef __USE_FLASH_DAT__
      INT32U addr=(page+4096)*4096+page_offset;
      #else
      INT32U addr=(page+FLADDR_VFM_PAGE_OFFSET)*4096+page_offset;
      #endif
      WaitForSingleObject(hFS_OS_VFM_MEMORY,INFINITE);
      open_os_vfm_memory();

      if(addr>16*1024*1024)
      {

           int a=0;

      }


      FS_OS_VFM_MEMORY->Seek(addr,soFromBeginning);
      FS_OS_VFM_MEMORY->Read(&temp,1);
       if(0==bitdata)
          clr_bit_value(&temp,1,bitpos);
       else
          set_bit_value(&temp,1,bitpos);
      FS_OS_VFM_MEMORY->Position--;
      FS_OS_VFM_MEMORY->Write(&temp,1);

      close_os_vfm_memory();

      //SetEvent(hFS_OS_VFM_MEMORY);
      ReleaseMutex(hFS_OS_VFM_MEMORY);
      return 1;
}
//释放数据块，使用标志和删除标志都变为0.
void os_vfm_clr_2bits(INT16U page,INT16U page_offset,INT8U bitpos)
{
      INT8U temp;
      #ifdef __USE_FLASH_DAT__
      INT32U addr=(page+4096)*4096+page_offset;
      #else
      INT32U addr=(page+FLADDR_VFM_PAGE_OFFSET)*4096+page_offset;
      #endif
      WaitForSingleObject(hFS_OS_VFM_MEMORY,INFINITE);
      open_os_vfm_memory();

      if(addr>16*1024*1024)
      {

           int a=0;

      }


      FS_OS_VFM_MEMORY->Seek(addr,soFromBeginning);
      FS_OS_VFM_MEMORY->Read(&temp,1);
       clr_bit_value(&temp,1,bitpos);
       clr_bit_value(&temp,1,bitpos+1);
      FS_OS_VFM_MEMORY->Position--;
      FS_OS_VFM_MEMORY->Write(&temp,1);

      close_os_vfm_memory();

      //SetEvent(hFS_OS_VFM_MEMORY);
      ReleaseMutex(hFS_OS_VFM_MEMORY);
    //  return 1;
}

/*+++
 功能：擦除page
---*/
void os_vfm_erase_page(INT16U page)
{
      INT32U addr;
      INT8U   buffer[4096];

      memset(buffer,0xFF,SIZE_OF_CLUSTER);
      WaitForSingleObject(hFS_OS_VFM_MEMORY,INFINITE);
      open_os_vfm_memory();


      addr =  SIZE_OF_CLUSTER;
      #ifdef __USE_FLASH_DAT__
      addr *= (page+4096);
      #else
      addr *= (page+FLADDR_VFM_PAGE_OFFSET);
      #endif
      FS_OS_VFM_MEMORY->Seek(addr,soFromBeginning);
      FS_OS_VFM_MEMORY->Write(buffer,SIZE_OF_CLUSTER);

      close_os_vfm_memory();
      ReleaseMutex(hFS_OS_VFM_MEMORY);
}

/*+++
   功能：仿铁电使用的块擦除
   参数：
        INT16U  page   Block所在的page(第1页)
---*/
void os_vfm_erase_block(INT16U page)
{
      INT32U addr;
      INT8U   buffer[4096];
      INT8U idx;

      memset(buffer,0xFF,SIZE_OF_CLUSTER);
      WaitForSingleObject(hFS_OS_VFM_MEMORY,INFINITE);
      open_os_vfm_memory();

      for(idx=0;idx<16;idx++)
      {
      addr =  SIZE_OF_CLUSTER;
      #ifdef __USE_FLASH_DAT__
      addr *= (page+idx+4096);
      #else
      addr *= (page+FLADDR_VFM_PAGE_OFFSET+idx);
      #endif
      FS_OS_VFM_MEMORY->Seek(addr,soFromBeginning);
      FS_OS_VFM_MEMORY->Write(buffer,SIZE_OF_CLUSTER);
      }
      close_os_vfm_memory();
      ReleaseMutex(hFS_OS_VFM_MEMORY);
}
#endif




