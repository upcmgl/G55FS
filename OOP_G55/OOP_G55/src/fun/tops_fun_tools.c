#include "../main_include.h"
#include "asf.h"

#define __METER_SIMPLE__

const INT8U  update_watermark[8] = { 'T','O','P','S','C','O','M','M' };

BOOLEAN read_datfile_header(DATHEADER *header, INT8U *buffer)
{
    INT16U datalen;
    INT16U pos;
    INT16U idx;
    INT16U len;
    INT8U  count;
    INT8U  cs;
    INT8U  tag;
    INT8U  taglen;

    //��ʼ���ļ�ͷ
    mem_set(header, sizeof(DATHEADER), 0xFF);

    //��ȡ�ļ�ͷ
    len = 512;
    //fread_array_dat(0,buffer,len,file_type);
   //1.������֤ˮӡ
    if (compare_string(buffer, (INT8U*)update_watermark, sizeof(update_watermark)) != 0)
    {
        return FALSE;
    }
    pos = sizeof(update_watermark);
    //2.�����֤�����
    datalen = bin2_int16u(buffer + pos);
    pos += 2;
    if (len < pos + datalen)
    {
        return FALSE;
    }
    cs = 0;
    for (idx = 0;idx < datalen + pos;idx++)
    {
        cs += buffer[idx];

    }
    if (buffer[datalen + pos] != cs)
    {
        return FALSE;
    }
    //���û���κοɴ���Ԫ�أ��򷵻���
    if (0 == datalen)
    {
        return FALSE;
    }
    //3.ö������tag�������tagֵ
    count = buffer[pos++];
    //���û���κοɴ���Ԫ�أ��򷵻���
    if (0 == count)
    {
        return FALSE;
    }
    for (idx = 0;idx < count;idx++)
    {
        tag = buffer[pos++];
        taglen = buffer[pos++];

        switch (tag)
        {
        case 0://�ļ����
            header->filetype = bin2_int16u(buffer + pos);
            break;
        case 1://�ļ��汾
            header->filever = bin2_int16u(buffer + pos);
            break;
        case 2://�ļ�����
            header->filesize = bin2_int32u(buffer + pos);
            break;
        case 3://�ļ�md5ֵ
            mem_cpy(header->md5, buffer + pos, taglen);//����TAGLEN
            break;
        case 4://�����㷨
            header->encryptno = buffer[pos];
            break;
        case 5://�����ļ����
            header->updatettype = buffer[pos];
            break;
        case 6://����������
            if (taglen <= sizeof(header->updatetspec))
            {
                mem_cpy(header->updatetspec, buffer + pos, taglen);//����TAGLEN
            }
            break;
        case 7://�����汾��
            if (taglen <= sizeof(header->updatetver))
            {
                mem_cpy(header->updatetver, buffer + pos, taglen);//����TAGLEN
            }
            break;
        case 8://�����汾����
            if (taglen <= sizeof(header->updatetdate))
            {
                mem_cpy(header->updatetdate, buffer + pos, taglen);//����TAGLEN
            }
            break;
        case 9://ʡ������
            if (taglen <= sizeof(header->areacode))
            {
                mem_cpy(header->areacode, buffer + pos, taglen);//����TAGLEN
            }
            break;
        case 11://�����ļ��ܴ�С
            if (taglen == 4)
            {
                header->attachsize = bin2_int32u(buffer + pos);
            }
            break;
        case 12://�����ļ��б�ÿ���ļ���С
            if (taglen == 20)
            {
                mem_cpy(header->attachfile, buffer + pos, taglen);//����TAGLEN
            }
            break;
        case 13://�豸����
            if (taglen <= sizeof(header->device_code_flag))
            {
                mem_cpy(header->device_code_flag, buffer + pos, taglen);//����TAGLEN
            }
            break;
        }
        pos += taglen;

    }	
    
    if(header->filetype != FILEFLAG_TYPE) return FALSE;

    return TRUE;
}

BOOLEAN check_update_header(DATHEADER *header,INT8U fileflag)
{
   if(fileflag == FILEFLAG_DEV_PROG)
   {
        if(header->filetype == FILEFLAG_TYPE) return TRUE;
		return FALSE;
   }
   return TRUE;
}
void update_download_record(INT16U sec_count,INT16U cur_sec,INT16U sec_len)
{
    tagDatetime datetime;
    INT16U temp=0;
	INT8U val=0;

    //д�ܶ���
    fread_array_update_info(FLADDR_PROG_TOTAL_SEC_COUNT,(INT8U *)&temp,2);
    if(temp == 0xFFFF)
    {
        os_get_datetime(&datetime);
        fwrite_array_update_info(FLADDR_PROG_TOTAL_SEC_COUNT,(INT8U *)&sec_count,2);
        fwrite_array_update_info(FLADDR_UPDATE_TIME,datetime.value,6);
    }
    //��������һ���Σ���дĩ�εĳ��ȣ�����д�׶λ��м�γ��ȣ��׶Ρ��м�γ��ȹ̶���ĩ�γ��Ȳ��̶���
    if(cur_sec == sec_count-1 )
    {
        fread_array_update_info(FLADDR_PROG_END_FRAME_LEN,(INT8U *)&temp,2);
        if(temp == 0xFFFF)
        {
            fwrite_array_update_info(FLADDR_PROG_END_FRAME_LEN,(INT8U *)&sec_len,2);
        }
    }
    else
    {
        fread_array_update_info(FLADDR_PROG_FRAME_LEN,(INT8U *)&temp,2);
        if(temp == 0xFFFF)
        {
            fwrite_array_update_info(FLADDR_PROG_FRAME_LEN,(INT8U *)&sec_len,2);
        }
    }
    //дÿ�ε��������
    fread_array_update_info(FLADDR_PROG_DOWNLOAD_RECORD+cur_sec/8,&val,1);
    clr_bit_value(&val,1,cur_sec %8);
    fwrite_array_update_info(FLADDR_PROG_DOWNLOAD_RECORD+cur_sec/8,&val,1);
}
BOOLEAN check_download_record(INT8U *buffer)
{
    INT16U sec_count;
	INT16U idx;
    INT16U count;
    INT16U ps_count;

    sec_count = 0;
    //�ܶ���
    fread_array_update_info(FLADDR_PROG_TOTAL_SEC_COUNT,(INT8U *)&sec_count,2);
    
    if((sec_count == 0xFFFF) || (sec_count == 0x00) || (sec_count>4096))
    {
        return FALSE;
    }

    count=sec_count/8;
    //���ж����ر��
    fread_array_update_info(FLADDR_PROG_DOWNLOAD_RECORD,buffer,count+1);

    for(idx=0;idx<count;idx++)
    {
       if(buffer[idx] != 0x00)
       {
       		return FALSE;
       }
    }
    
    if(sec_count % 8 != 0)
    {
        ps_count=(sec_count % 8) ;
		for(idx=0;idx<ps_count;idx++)
        {
    		if(get_bit_value(buffer+count,1,idx))
            {
             	return FALSE;
            }
        }
    }
    return TRUE;
}
BOOLEAN datfile_update(DATHEADER *header, INT8U *resetFlag,INT8U *buffer)
{
    BOOLEAN check_md5, resault;
   union{
      INT8U info[32];
      INT32U u32[8];
      INT16U u16[16];
    }var;
    *resetFlag = FALSE;
    resault = FALSE;
    
    check_md5 = check_datfile(header, buffer);
    if (check_md5)
    {
        *resetFlag = TRUE;
        resault = TRUE;
    }
    return resault;
}
void toggle_LED_ERR(void);
/*+++
 ���ܣ� ����DAT�ļ�ͷ��Ϣ����֤�ļ��Ƿ�Ϸ����������֤����MD5�ļ�
 ������
        INT8U fileflag    �ļ���ʶ
        DATHEADER *header �ļ�ͷ�ṹ
        INT8U *buffer     ���ڶ�ȡ�ļ��Ļ���������������С�������2064�ֽ�
        INT8U file_type  0-�ն��ļ�  ����-usb�ļ�
����ֵ
    TRUE-�ļ���֤�ɹ� FALSE-�ļ���֤ʧ��
---*/
#define PIM_DATA                    512  
BOOLEAN check_datfile(DATHEADER *header, INT8U *buffer)
{
    INT32U   progLen, progStart;
    INT16U i, len;
    MD5_CTX md5_ctx;
    INT16U  prog_page_size, ret;
    INT8U  hash[16] = { 0 };
    static INT8U led_count = 0;

    //�����ļ����ȣ���ȡ�ļ��������ܣ�����MD5
    if (header->filesize == 0xFFFFFFFF) return FALSE;
    if (header->filesize > 0x100000) return FALSE;//�ļ����ȳ���1M�򷵻�ʧ��
 
    if(header->filetype != FILEFLAG_TYPE) return FALSE;
    
    //header->encryptno
    progLen = header->filesize;
       //���ܲ�����MD5
       //ÿ�ζ�ȡ����ĳ���
    prog_page_size = 512;
	progStart = PIM_DATA;

    MD5Init(&md5_ctx);

    while (progLen > 0)
    {
        if (progLen >= prog_page_size)
            len = prog_page_size;
        else
            len = progLen;
        ret = fread_array_update_data( progStart, buffer, len);
        if (ret != FILE_SUCCESS)
        {
            break;
        }
		
        //����
        //�ӽ����㷨1:ʹ��"moc.mmocspot.www"��ԭ�Ľ���ѭ�����
        if (header->encryptno == ENCRYPT_TOPS1)
        {
            decode_update(buffer, len, progStart-PIM_DATA);
            }
        MD5Update(&md5_ctx, buffer, len);
        progStart += len;
        progLen -= len;
        if ((led_count++) > 20)
        {
            led_count = 0;
			toggle_LED_ERR();
        }
    }
    MD5Final(hash, &md5_ctx);
    for (i = 0;i < 16;i++)
    {
        if (header->md5[i] != hash[i])
        {
            return FALSE;
        }
    }
    return TRUE;

}


FSFILE * usb_open_file(INT8U file_type, FSFILE *file)
{
    char    *filename;
    FRESULT ret;

    switch(file_type)
    {
	    case FILEFLAG_DEV_PROG:   //�����ļ�
        {
            if(FILEFLAG_TYPE == FILEFLAG_SGP_GMTER_G55)//����G��
            {
                filename="0:G55_S_GMETER.dat";
            }
            else if(FILEFLAG_TYPE == FILEFLAG_TGP_GMTER_G55)//����G��
            {
                filename="0:G55_GMETER.dat";
            }
            else
            {
                filename=NULL;
            }
        }
            break;
        default:
            filename=NULL;
            break;
    }
    if(filename == NULL) 
    {
        return NULL;         
    }

    ret = f_open_app(file, filename, FA_READ);
    if(ret != FR_OK)
    {
        return NULL;
    }

     return file;
}
BOOLEAN usb_file_exist(INT8U file_type, FSFILE* file)
{
    FSFILE *myFile;
    myFile=usb_open_file(file_type, file);
    if(myFile != NULL)
    {
        f_close(myFile);
    }  
    
    return (myFile!=NULL);
}

void decode_update(INT8U *buffer,INT16U len,INT32U addr)
{
    INT8U TOPS_ENCRYPT_1_STR[] = { 'm','o','c','.','m','m','o','c','s','p','o','t','.','w','w','w' };
    INT8U encrypt_idx;
	INT8U  encrypt_len= sizeof(TOPS_ENCRYPT_1_STR), k;
	INT32U i;
    
    encrypt_idx= addr%encrypt_len;
    
    for (i = 0;i < len;i++)
    {
        k =  (addr+i);
        k |= (addr+i) >> 8;
        k |= (addr+i) >> 16;
        k |= (addr+i) >> 24;

        buffer[i] = buffer[i] ^ (TOPS_ENCRYPT_1_STR[encrypt_idx++] - 0x30) ^ k;
        if (encrypt_idx >= encrypt_len)
        {
            encrypt_idx = 0;
        }
    }
}

void usb_download_program(FSFILE * myFile,INT8U *data_buffer)
{
      INT32S  len;
      INT32U  datalen;
      INT8U   idx1,down_flag;
	  INT32U addr;
      
      UINT count;
      FRESULT ret;

        //ѭ����ȡ�ļ�������  
        datalen = 0;
        down_flag = 0;
        addr     = 0;
        do
        {         
           ClrTaskWdt();
           //len = FSfread(data_buffer,1,512,myFile);
           ret = f_read(myFile, data_buffer, 512, &count);
           if(ret != FR_OK)
           {
               f_close(myFile);
               return;
           } 
           len = count;
           if(len > 0)
           {
    		 if(addr==0)
			 {
			 	fwrite_array_update_data(FLADDR_PORG_HEADER,data_buffer,512);
			 }
             else
			 {
				//���ܴ���NOR_FLASH����д���ڲ�flash�ٽ���
				fwrite_array_update_data(addr,data_buffer,512);
			 }
             addr += len;
             datalen += len;
           }
           //�澯��һ����˸����ʾU���Ѿ������ˡ�
           down_flag ++;
           if( (down_flag&0x0F) == 0x0F)      
           {
                   //��˸�澯��
                LED_comm_toggle();
           } 
       }while(len > 0);

}
INT8U usb_download_dat(INT8U file_type, FSFILE * file)
{
    DATHEADER update_header;
    BOOLEAN sucess;
    INT8U resetFlag;
    
    resetFlag =FALSE;
    delete_update_file();

	update_object.usbFile=usb_open_file(file_type, file);
	usb_download_program(update_object.usbFile,update_object.buffer);
    //��ȡ�ļ�ͷ
	fread_array_update_data(FLADDR_PORG_HEADER,update_object.buffer,512);
    sucess=read_datfile_header(&update_header,update_object.buffer);

    //��֤MD5�����ļ�
    if(sucess )
    {        
		if(update_object.usbFile!=NULL)
		{
	        if(datfile_update(&update_header,&resetFlag,update_object.buffer)==FALSE)
			{

			}
		}
    }
    else
    {
    }
	if(update_object.usbFile!=NULL)
    {
			f_close(update_object.usbFile);
    }
    return resetFlag;
}
/*+++
 ���ܣ� ����U���ϵ������ļ�
 ������
        ��
---*/
BOOLEAN usb_file_process(void)
{
    INT8U   idx;
    FSFILE file;
    BOOLEAN  reset_flag=FALSE;

    for(idx = 0; idx<1; idx++)
    {
        switch(idx)
        {
        case 0:   //�����ļ�
                if(usb_file_exist(FILEFLAG_DEV_PROG, &file))
                {
                    //record_sys_operate(OP_SYS_MAIN_UPDATE);  
                    reset_flag=usb_download_dat(FILEFLAG_DEV_PROG, &file);
                }
                break;
        default:
                break;
        }
    }

    return reset_flag;
}
