//---------------------------------------------------------------------------


#pragma hdrstop

#include "Unit_FLASH.h"
#include <Classes.hpp>
#include <stdio.h>

//---------------------------------------------------------------------------

#pragma package(smart_init)
 
#define FLASH_SECTOR_SIZE             (4096)  
#define FLASH_SECTOR_COUNT            (2048)

FILE     *fp_file_flash=NULL;

 String  str_file_flash = "DEV_FILE_FLASH.dat";

unsigned char flash_sector_buf[4096];

int sim_open_file_flash(void)
{
     if(  fp_file_flash )
     {
         fclose(fp_file_flash);
     }
     fp_file_flash = fopen(str_file_flash.c_str(),"r+b");
     if(fp_file_flash == NULL)
     {
        fp_file_flash = fopen(str_file_flash.c_str(),"w+b");
     }
     if(fp_file_flash == NULL)
     {
       return 0;
     }
     return 1;
}   
int sim_close_file_flash(void)
{
     if(  fp_file_flash )
     {
         fclose(fp_file_flash);
     }

     return 1;
}

unsigned short nor_flash_read_data(unsigned short sector,unsigned short offset,unsigned char *data,unsigned short len)
{
    unsigned int addr;

    if(sector > FLASH_SECTOR_COUNT)
    {
        return 0;
    }
    if(offset > FLASH_SECTOR_SIZE)
    {
        return 0;
    }
    if(fp_file_flash==NULL)
    {
      return 0;
    }
    tpos_mutexPend(&SIGNAL_FLASH);
    
    addr = sector;
    addr *= FLASH_SECTOR_SIZE;
    addr += offset;

    int err = fseek(fp_file_flash,addr,SEEK_SET);

    if(err == 0)
    {
        fread(data,len,1,fp_file_flash);
    }
    else
    {
        len = 0;
    }
    tpos_mutexFree(&SIGNAL_FLASH);
    return len;
}
unsigned short nor_flash_erase_page(unsigned short sector)
{
    unsigned int addr;
    
    if(sector > FLASH_SECTOR_COUNT)
    {
        return 0;
    }
    if(fp_file_flash==NULL)
    {
      return 0;
    }
    
    tpos_mutexPend(&SIGNAL_FLASH);
    
    addr = sector;
    addr *= FLASH_SECTOR_SIZE;

    int err = fseek(fp_file_flash,addr,SEEK_SET);

    if(err == 0)
    {
        memset(flash_sector_buf,0xFF,sizeof(flash_sector_buf));
        fwrite(flash_sector_buf,sizeof(flash_sector_buf),1,fp_file_flash);
    }
    else
    {
      tpos_mutexFree(&SIGNAL_FLASH);
      return 0;
    }

    tpos_mutexFree(&SIGNAL_FLASH);
    return 1;
}
unsigned short nor_flash_write_data(unsigned short sector,unsigned short offset,unsigned char *data,unsigned short len)
{
    unsigned short write_len;
    unsigned int addr;
    
    if(sector > FLASH_SECTOR_COUNT)
    {
        return 0;
    }
    if(offset > FLASH_SECTOR_SIZE)
    {
        return 0;
    }  
    if(fp_file_flash==NULL)
    {
      return 0;
    }
    
    tpos_mutexPend(&SIGNAL_FLASH);

    addr = sector;
    addr *= FLASH_SECTOR_SIZE;
    addr += offset;
    
    int err = fseek(fp_file_flash,addr,SEEK_SET);

    if(err == 0)
    {
        fwrite(data,len,1,fp_file_flash);
    }
    else
    {
      tpos_mutexFree(&SIGNAL_FLASH);
      return 0;
    }

    tpos_mutexFree(&SIGNAL_FLASH);
    return len;
}
BOOLEAN  check_flash_data(INT16U sector,INT16U offset,INT8U *buffer,INT16U len)
{
    unsigned int addr;
    unsigned char data[FLASH_SECTOR_SIZE];
    BOOLEAN result=FALSE;
    
    if(sector > FLASH_SECTOR_COUNT)
    {
        return 0;
    }
    if(offset > FLASH_SECTOR_SIZE)
    {
        return 0;
    }
    if(fp_file_flash==NULL)
    {
      return 0;
    }
    tpos_mutexPend(&SIGNAL_FLASH);
    
    addr = sector;
    addr *= FLASH_SECTOR_SIZE;
    addr += offset;

    int err = fseek(fp_file_flash,addr,SEEK_SET);

    if(err == 0)
    {
        fread(data,len,1,fp_file_flash);
        result=mem_is_same(data,buffer,len);
    }
    else
    {
        len = 0;
    }
    tpos_mutexFree(&SIGNAL_FLASH);
    return result;
}