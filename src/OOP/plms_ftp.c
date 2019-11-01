#include "../main_include.h"
#include "app_const_3761.h"
#include "protocol_app_handler.h"   
#include "plms_ftp.h"

#if 0
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
    if(header->filetype != 0xFD)
    {
        //Ŀǰֻ֧��g���ն˱������
        return FALSE;
    }
    return TRUE;
}

BOOLEAN check_update_header(DATHEADER *header,INT8U fileflag)
{
   if(fileflag == FILEFLAG_DEV_PROG)
   {
        if(header->filetype==0xFD)
       {
           return TRUE;
       }
       else
       {
           return TRUE;
       }
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

    *resetFlag = FALSE;
    resault = FALSE;
    
    check_md5 = check_datfile(header, buffer);
    if (check_md5)
    {
        mem_set(buffer,4,0xAA);
        fwrite_array_update_info(FLADDR_PROG_INFO_FLAG,buffer,4);
        mem_set(buffer,4,0x55);
        fwrite_array_update_info(FLADDR_PROG_INFO_FLAG+4,buffer,4);
        mem_set(buffer,4,0x99);
        fwrite_array_update_info(FLADDR_PROG_INFO_FLAG+8,buffer,4);
        mem_set(buffer,4,0x66);
        fwrite_array_update_info(FLADDR_PROG_INFO_FLAG+12,buffer,4);
        *resetFlag = TRUE;
        resault = TRUE;
    }
    return resault;
}
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
    #ifndef __SOFT_SIMULATOR__
    MD5_CTX md5_ctx;
    #endif
    INT16U  prog_page_size, ret;
    INT8U encrypt_idx, encrypt_len, k;
    INT8U TOPS_ENCRYPT_1_STR[] = { 'm','o','c','.','m','m','o','c','s','p','o','t','.','w','w','w' };
    INT8U  hash[16] = { 0 };
    static INT8U led_count = 0;
    INT8U temp_buffer[512]={0};

    //�����ļ����ȣ���ȡ�ļ��������ܣ�����MD5
    if (header->filesize == 0xFFFFFFFF) return FALSE;
    if (header->filesize > 0x100000) return FALSE;//�ļ����ȳ���1M�򷵻�ʧ��
    if(header->filetype != 0xFD) return FALSE;
    //header->encryptno

    progLen = header->filesize;
       //���ܲ�����MD5
       //ÿ�ζ�ȡ����ĳ���
    prog_page_size = 512;
    progStart = PIM_DATA;
    #ifndef __SOFT_SIMULATOR__
    MD5Init(&md5_ctx);
    #endif
    encrypt_idx = 0;

    while (progLen > 0)
    {
        if (progLen >= prog_page_size)
            len = prog_page_size;
        else
            len = progLen;
        ret = fread_array_update_data( progStart, buffer, len);
        mem_cpy(temp_buffer,buffer,512);
        if (ret != FILE_SUCCESS)
        {
            break;
        }
        //����
        //�ӽ����㷨1:ʹ��"moc.mmocspot.www"��ԭ�Ľ���ѭ�����
        if (header->encryptno == ENCRYPT_TOPS1)
        {
            encrypt_len = sizeof(TOPS_ENCRYPT_1_STR);
            for (i = 0;i < len;i++)
            {
                k = (progStart - PIM_DATA + i);
                k |= (progStart - PIM_DATA + i) >> 8;
                k |= (progStart - PIM_DATA + i) >> 16;
                k |= (progStart - PIM_DATA + i) >> 24;

                buffer[i] = buffer[i] ^ (TOPS_ENCRYPT_1_STR[encrypt_idx++] - 0x30) ^ k;
                if (encrypt_idx >= encrypt_len)
                {
                    encrypt_idx = 0;
                }
            }
        }
        #ifndef __SOFT_SIMULATOR__
        MD5Update(&md5_ctx, buffer, len);
        #endif
        progStart += len;
        progLen -= len;
        if ((led_count++) > 20)
        {
            led_count = 0;
        }
    }
    #ifndef __SOFT_SIMULATOR__
    MD5Final(hash, &md5_ctx);
    #endif
    for (i = 0;i < 16;i++)
    {
        if (header->md5[i] != hash[i])
        {
            return FALSE;
        }
    }
    return TRUE;

}
#endif