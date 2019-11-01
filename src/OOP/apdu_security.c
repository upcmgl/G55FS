#include "protocol_app_handler.h"
#include "protocol_app_const_def.h"
#include "object_describe.h"
#include "drv_esam_spi.h"
//#if defined __ENABLE_ESAM2__
#ifdef __ENABLE_ESAM2__

#define SID_MAC     0       //������֤��
#define RN          1       //�����
#define RN_MAC      2       //�����+����MAC
#define SID         3       //��ȫ��ʶ

#define RTU_DECODE_ERROR                   0x1001	/*�ն˽��ܴ���*/
#define RTU_VERI_SIGN_ERROR                0x1002	/*�ն���ǩʧ��*/
#define RTU_VERI_MAC_ERROR                 0x1003	/*�ն�MACУ��ʧ��*/
#define RTU_DEVCOUNT_ERROR                 0x1004	/*�Ự����������*/
#define LINK_CODE_UNIT_ERROR               0x1009   /*��·�豸���뵥Ԫ����*/


#define RTU_LINK_SESSION_ERROR             0x3005	/*��ǰ��·�ỰЭ��ʧ��*/


void sal_send_error_resp(objRequest  *pRequest,objResponse *pResp,INT16U err_code);
void app_protocol_handler(objRequest* pRequest,objResponse *pResp);

INT16S esam_send_cmd(INT8U *cmd_buffer,INT16U cmd_len,INT8U *recv_buffer,INT16U recv_max_len,INT16U *err_code)
{
    INT16S len;
    INT8U headdata[4];

    //��ȡ�ź���
    //tpos_mutexPend(&SIGNAL_EASM);
    drv_esam_spi_WriteArray(cmd_buffer,cmd_len);
    len = drv_esam_spi_ReadArray(recv_buffer,recv_max_len,headdata,err_code);
    //�ͷ��ź���
    //tpos_mutexFree(&SIGNAL_EASM);

    return len;
}
/*+++
  ���ܣ� ��վ���ͻ�ȡ T-ESAM ��Ϣ����

  ������
         INT8U *buffer  ������
  ���أ�
         ���ݳ���
  ������
      ���ͣ�803600FF0000
      ���أ�9000+LEN+ESAM���к�

 ---*/
INT16S esam_get_tesam_info(INT8U *buffer,INT16U max_len)
{
    INT16S len;
    INT16U pos;
    INT8U headdata[4];
    INT16U err_code;
    
    pos = 0;
    buffer[pos++] = 0x80;
    buffer[pos++] = 0x36;
    buffer[pos++] = 0x00;
    buffer[pos++] = 0xFF;
    buffer[pos++] = 0x00;
    buffer[pos++] = 0x00;
    //��ȡ�ź���
    //tpos_mutexPend(&SIGNAL_EASM);
    drv_esam_spi_WriteArray(buffer,pos);
    mem_set(buffer,pos,0x00);
    len = drv_esam_spi_ReadArray(buffer,max_len,headdata,&err_code);
    //�ͷ��ź���
    //tpos_mutexFree(&SIGNAL_EASM);

    return len;
}
//8002000060
INT16S esam_session_auth(INT8U *session_in,INT16U session_len,INT8U *buffer,INT16U max_len)
{
    INT16S len;
    INT16U pos;
    INT8U headdata[4];
    INT16U err_code;

    pos = 0;
    buffer[pos++] = 0x80;
    buffer[pos++] = 0x02;
    buffer[pos++] = 0x00;
    buffer[pos++] = 0x00;
    buffer[pos++] = 0x00;
    buffer[pos++] = 0x60;
    if(max_len-pos>0)
    {
        mem_cpy(buffer+pos,session_in,session_len);
        pos+=session_len;
    }
    //��ȡ�ź���
    //tpos_mutexPend(&SIGNAL_EASM);

    drv_esam_spi_WriteArray(buffer,pos);
    mem_set(buffer,pos,0x00);
    len = drv_esam_spi_ReadArray(buffer,max_len,headdata,&err_code);
    //tpos_mutexFree(&SIGNAL_EASM);
    return len;
}
INT16S req_object_attribute_esam(INT8U attri_id,INT8U index,INT8U *data,INT16U left_len)
{
    INT16U pos;
    INT16S len=0;
    INT8U esam_cmd[]={0x80,0x36,0x00,0x00,0x00,0x00};
    INT8U *text;
    INT16U err_code=0;
    INT8U chip_state;

    pos=0;
    switch(attri_id)
    {
    case 2: //ESAM���к�
		data[pos++]=1;
        data[pos++]=DT_OCTETSTRING;
        data[pos++]=8;
        esam_cmd[3]=0x02;
        mem_cpy(data+pos,esam_cmd,6);
        len=esam_send_cmd(data+pos,6,data+pos,32,&err_code);
        if(len!=8)
        {
            mem_set(data+pos,8,0x00);
        }
        pos+=8;
        break;
    case 3: //ESAM�汾��
		data[pos++]=1;
        data[pos++]=DT_OCTETSTRING;
        data[pos++]=4;
        esam_cmd[3]=0x03;
        mem_cpy(data+pos,esam_cmd,6);
        len=esam_send_cmd(data+pos,6,data+pos,32,&err_code);
        if(len!=4)
        {
            mem_set(data+pos,4,0x00);
        }
        pos+=4;
        break;
    case 4: //�Գ���Կ�汾16�ֽ�
		data[pos++]=1;
        data[pos++]=DT_OCTETSTRING;
        data[pos++]=1;
        esam_cmd[3]=0x04;
        mem_cpy(data+pos,esam_cmd,6);
        len=esam_send_cmd(data+pos,6,data+pos,32,&err_code);
        data[pos-1]=len;
        pos+=len;
        break;
    case 5: //�ỰʱЧ����
		data[pos++]=1;
        data[pos++]=DT_DOUBLELONGUNSIGNED;
        esam_cmd[3]=0x06;
        mem_cpy(data+pos,esam_cmd,6);
        len=esam_send_cmd(data+pos,6,data+pos,32,&err_code);
        if(len!=4)
        {
            mem_set(data+pos,4,0x00);
        }
        pos+=4;
        break;
    case 6: //�ỰʱЧʣ��ʱ��
		data[pos++]=1;
        data[pos++]=DT_DOUBLELONGUNSIGNED;
        esam_cmd[3]=0x07;
        mem_cpy(data+pos,esam_cmd,6);
        len=esam_send_cmd(data+pos,6,data+pos,32,&err_code);
        if(len!=4)
        {
            mem_set(data+pos,4,0x00);
        }
        pos+=4;
        break;
    case 7: //��ǰ������
		data[pos++]=1;
        data[pos++]=DT_STRUCTURE;
        data[pos++]=3;

        esam_cmd[3]=0x08;
        mem_cpy(data+pos,esam_cmd,6);
        len=esam_send_cmd(data+pos,6,data+pos,32,&err_code);
        if(len==12)
        {
            mem_cpy(g_esam_info.counter.single_addr_counter,data+pos,4);
            mem_cpy(g_esam_info.counter.report_counter,data+pos+4,4);
            mem_cpy(g_esam_info.counter.broadcast_no,data+pos+8,4);

            data[pos++]=DT_DOUBLELONGUNSIGNED;
            mem_cpy(data+pos,g_esam_info.counter.single_addr_counter,4);
            pos+=4;
            data[pos++]=DT_DOUBLELONGUNSIGNED;
            mem_cpy(data+pos,g_esam_info.counter.report_counter,4);
            pos+=4;
            data[pos++]=DT_DOUBLELONGUNSIGNED;
            mem_cpy(data+pos,g_esam_info.counter.broadcast_no,4);
            pos+=4;
        }

        if(len<=0)
        {
            data[pos++]=DT_DOUBLELONGUNSIGNED;
            mem_set(data+pos,4,0x00);
            pos+=4;
            data[pos++]=DT_DOUBLELONGUNSIGNED;
            mem_set(data+pos,4,0x00);
            pos+=4;
            data[pos++]=DT_DOUBLELONGUNSIGNED;
            mem_set(data+pos,4,0x00);
            pos+=4;
        }
        break;
    case 8: //֤��汾
		data[pos++]=1;
        data[pos++]=DT_STRUCTURE;
        data[pos++]=2;

        esam_cmd[3]=0x05;
        mem_cpy(data+pos,esam_cmd,6);
        len=esam_send_cmd(data+pos,6,data+pos,32,&err_code);
        if(len==2)
        {
            g_esam_info.cert_ver[1]=data[pos+1];
            g_esam_info.cert_ver[0]=data[pos];
            data[pos++]=DT_OCTETSTRING;
            data[pos++]=1;
            data[pos++]=g_esam_info.cert_ver[1];
            data[pos++]=DT_OCTETSTRING;
            data[pos++]=1;
            data[pos++]=g_esam_info.cert_ver[0];
        }

        if(len!=2)
        {
            data[pos++]=DT_OCTETSTRING;
            data[pos++]=1;
            data[pos++]=0;
            data[pos++]=DT_OCTETSTRING;
            data[pos++]=1;
            data[pos++]=0;
        }
        break;
    case 9: //�ն�֤�����к�
		data[pos++]=1;
        data[pos++]=DT_OCTETSTRING;
        data[pos++]=16;

        esam_cmd[3]=0x09;
        mem_cpy(data+pos,esam_cmd,6);
        len=esam_send_cmd(data+pos,6,data+pos,32,&err_code);
        if(len!=16)
        {
            mem_set(data+pos,16,0x00);
        }
        pos+=16;
        break;
    case 10: //�ն�֤��
        text=data+pos+4;
        text[0]=0x80;
        text[1]=0x36;
        text[2]=0x00;
        text[3]=0x0B;
        text[4]=0x00;
        text[5]=0x00;
        len=esam_send_cmd(text,6,text,left_len-5,&err_code);
		data[pos++]=1;
        data[pos++]=DT_OCTETSTRING;
        if(len>0)
        {
            pos+=set_cosem_data_len(data+pos,len);
            mem_cpy(data+pos,text,len);
            pos+=len;
        }
        else
        {
            data[pos++]=0;
        }
        break;
    case 11: //��վ֤�����к�
		data[pos++]=1;
        data[pos++]=DT_OCTETSTRING;
        data[pos++]=16;
        esam_cmd[3]=0x0A;
        mem_cpy(data+pos,esam_cmd,6);
        len=esam_send_cmd(data+pos,6,data+pos,32,&err_code);
        if(len!=16)
        {
            mem_set(data+pos,16,0x00);
        }
        pos+=16;
        break;
    case 12: //��վ֤��
        text=data+pos+4;
        text[0]=0x80;
        text[1]=0x36;
        text[2]=0x00;
        text[3]=0x0C;
        text[4]=0x00;
        text[5]=0x00;
        len=esam_send_cmd(text,6,text,left_len-5,&err_code);
		data[pos++]=1;
        data[pos++]=DT_OCTETSTRING;
        if(len>0)
        {
            pos+=set_cosem_data_len(data+pos,len);
            mem_cpy(data+pos,text,len);
            pos+=len;
        }
        else
        {
            data[pos++]=0;
        }
        break;
    case 13: //ESAM��ȫ�洢�����б�
		data[pos++]=1;
        data[pos++]=DT_ARRAY;
        data[pos++]=0;
        break;
    default:
		data[pos++]=1;
        data[pos++]=0;
        break;
    }
    return pos;
}

void esam_get_info(void)
{
    uint8_t buffer[30]={0};
    uint8_t len;

    mem_set((INT8U*)&g_esam_info,sizeof(ESAM_INFO),0xFF);
    gSystemInfo.esam_type=0;
    mem_set(gSystemInfo.ESAM_DEVID,8,0xFF);
    gSystemInfo.ESAM_CERT_STATE = 0xFF;
    len = esam_get_esam_id(buffer,20);
    if(len == 8) 
    {
        mem_cpy_reverse(gSystemInfo.ESAM_DEVID,buffer,8);
        //gSystemInfo.esam_type=13;//13�淶esam
        len = esam_get_chip_state(buffer,20);
        if(len == 1) gSystemInfo.ESAM_CERT_STATE = buffer[0];
    }
    if(gSystemInfo.esam_type==0)
    {
        len = tesam_get_esam_id(buffer,20);
        if(len == 8)
        {
            mem_cpy_reverse(gSystemInfo.ESAM_DEVID,buffer,8);
            //gSystemInfo.esam_type=ESAM_OOP;//698esam
            len = tesam_get_chip_state(buffer,30);
            if(len == 16)
            {
                if(check_is_all_ch(buffer,16,0x00))
                {
                    gSystemInfo.ESAM_CERT_STATE=0x00;
                }
                else
                {
                    gSystemInfo.ESAM_CERT_STATE = 0x01;
                }
            }
        }
    }
    //if(gSystemInfo.esam_type==ESAM_OOP)//698esam
    {
        //���esam spiͨѶ����
        drv_esam_spi_high_Initialize();
    }
}
void update_esam_chip_state(void)
{
    uint8_t buffer[30]={0};
    uint8_t len;

    len = tesam_get_chip_state(buffer,30);
    if(len == 16)
    {
        if(check_is_all_ch(buffer,16,0x00))
        {
            gSystemInfo.ESAM_CERT_STATE=0x00;
        }
        else
        {
            gSystemInfo.ESAM_CERT_STATE = 0x01;
        }
    }
}
void exec_esam_method(OBJECT_ATTR_DESC *oad,INT8U* param,objResponse *pResp)
{
    INT8U sid[4],mac[4];
    INT16U pos,val16;
    INT16S len;
    INT16U  text_len,attach_len;
    INT8U *text,*attach_info,head_size;
    INT16U err_code;

    pos=0;
    text=NULL;
    attach_info=NULL;
    mem_set(sid,4,0x00);
    mem_set(mac,4,0x00);

    switch(oad->attribute_id.value)
    {
    case 3: //ESAM��ȡ
            pos++;//SID TYPE
            mem_cpy(sid,param+pos,4);
            pos+=4;
            attach_len=get_cosem_data_len(param+pos,&head_size);
            pos+=head_size;
            attach_info=param+pos;
            pos+=attach_len;
            
            text=pResp->frame+pResp->pos+8;
            mem_cpy(text,sid,4);
            mem_cpy(text+4,attach_info,attach_len);
            len=esam_send_cmd(text,4+attach_len,text,pResp->max_reply_pos-pResp->pos-8,&err_code);
            if(len>0)
            {
                pResp->frame[pResp->pos++] = DAR_SECCESS;
                pResp->frame[pResp->pos++] = 1;
                pResp->frame[pResp->pos++] = DT_OCTETSTRING;
                pResp->pos+=set_cosem_data_len(pResp->frame+pResp->pos,len);
                mem_cpy(pResp->frame+pResp->pos,text,len);
                pResp->pos+=len;
            }
            else
            {
                pResp->frame[pResp->pos++] = DAR_OTHER_REASON;
                pResp->frame[pResp->pos++] = 1;//Data  OPTIONAL
                pResp->frame[pResp->pos++] = DT_OCTETSTRING;
                pResp->frame[pResp->pos++] = 2;
                cosem_int162_bin(err_code,pResp->frame+pResp->pos);// esam ���ش������
                pResp->pos+=2;
                
            }
        break;
    case 4://���ݸ��£�������
        if((param[pos]==DT_STRUCTURE) && (param[pos+1]==2))
        {
            pos+=2;
            text_len=get_cosem_data_len(param+pos,&head_size);
            pos+=head_size;
            text=param+pos;
            pos+=text_len;

            pos++;//SID TYPE
            mem_cpy(sid,param+pos,4);
            pos+=4;
            attach_len=get_cosem_data_len(param+pos,&head_size);
            pos+=head_size;
            attach_info=param+pos;
            pos+=attach_len;
            pos++;
            mem_cpy(mac,param+pos,4);
            pos+=4;
            
            //���ͣ� sid1+AttachData1+Endata1
            mem_cpy(pResp->frame+pResp->pos+8,sid,4);
            mem_cpy(pResp->frame+pResp->pos+8+4,attach_info,attach_len);
            mem_cpy(pResp->frame+pResp->pos+8+4+attach_len,text,text_len);
            mem_cpy(pResp->frame+pResp->pos+8+4+attach_len+text_len,mac,4);
            len=esam_send_cmd(pResp->frame+pResp->pos+8,4+attach_len+text_len+4,pResp->frame+pResp->pos+8,pResp->max_reply_pos-pResp->pos-8,&err_code);
            if(len==0)
            {
                pResp->frame[pResp->pos++] = DAR_SECCESS;
                
            }
            else
            {
                pResp->frame[pResp->pos++] = DAR_OTHER_REASON;
                pResp->frame[pResp->pos++] =  1;//Data  OPTIONAL
                pResp->frame[pResp->pos++] = DT_OCTETSTRING;
                pResp->frame[pResp->pos++] = 2;
                cosem_int162_bin(err_code,pResp->frame+pResp->pos);// esam ���ش������
                pResp->pos+=2;
            }
        }
        else
        {
             pResp->frame[pResp->pos++] = DAR_OTHER_REASON;
            pResp->frame[pResp->pos++] = 0;
        }
        break;
    case 5: //Э��ʧЧ
        pResp->frame[pResp->pos++] = DAR_SECCESS;
        pResp->frame[pResp->pos++] = 1;
        pResp->frame[pResp->pos++] = DT_DATETIME_S;
        sys_datetime2cosem_datetime( pResp->frame+pResp->pos);
        pResp->pos+=7;
        break;
    case 7: //������Կ
        if((param[pos]==DT_STRUCTURE) && (param[pos+1]==2))
        {
            pos+=3;//DT_STRUCTURE��2��octet-string
            //����
            text_len=get_cosem_data_len(param+pos,&head_size);
            pos+=head_size;
            text=param+pos;
            pos+=text_len;
            //SID
            pos++;//SID data type
            mem_cpy(sid,param+pos,4);
            pos+=4;
            //������
            attach_len=get_cosem_data_len(param+pos,&head_size);
            pos+=head_size;
            attach_info=param+pos;
            pos+=attach_len;
            //MAC
            pos++;//mac data type
            mem_cpy(mac,param+pos,4);
            pos+=4;
            
            //���ͣ� sid1+AttachData1+Endata1
            mem_cpy(pResp->frame+pResp->pos+8,sid,4);
            mem_cpy(pResp->frame+pResp->pos+8+4,attach_info,attach_len);
            mem_cpy(pResp->frame+pResp->pos+8+4+attach_len,text,text_len);
            mem_cpy(pResp->frame+pResp->pos+8+4+attach_len+text_len,mac,4);
            len=esam_send_cmd(pResp->frame+pResp->pos+8,4+attach_len+text_len+4,pResp->frame+pResp->pos+8,pResp->max_reply_pos-pResp->pos-8,&err_code);
            if(len==0)
            {
                pResp->frame[pResp->pos++] = DAR_SECCESS;
                update_esam_chip_state();

            }
            else
            {
                pResp->frame[pResp->pos++] = DAR_OTHER_REASON;
                pResp->frame[pResp->pos++] =  1;//Data  OPTIONAL
                pResp->frame[pResp->pos++] = DT_OCTETSTRING;
                pResp->frame[pResp->pos++] = 2;
                cosem_int162_bin(err_code,pResp->frame+pResp->pos);// esam ���ش������
                pResp->pos+=2;
                
            }
        }
        else
        {
             pResp->frame[pResp->pos++] = DAR_OTHER_REASON;
            pResp->frame[pResp->pos++] = 0;
        }
        break;
    case 8: //֤�����
        if((param[pos]==DT_STRUCTURE) && (param[pos+1]==2))
        {
            pos+=3;
            text_len=get_cosem_data_len(param+pos,&head_size);
            pos+=head_size;
            text=param+pos;
            pos+=text_len;

            pos++;//SID TYPE
            mem_cpy(sid,param+pos,4);
            pos+=4;
            attach_len=get_cosem_data_len(param+pos,&head_size);
            pos+=head_size;
            attach_info=param+pos;
            pos+=attach_len;
            //���ͣ� sid1+AttachData1+Endata1
            mem_cpy(pResp->frame+pResp->pos+8,sid,4);
            mem_cpy(pResp->frame+pResp->pos+8+4,attach_info,attach_len);
            mem_cpy(pResp->frame+pResp->pos+8+4+attach_len,text,text_len);
            len=esam_send_cmd(pResp->frame+pResp->pos+8,4+attach_len+text_len,pResp->frame+pResp->pos+8,pResp->max_reply_pos-pResp->pos-8,&err_code);
            if(len==0)
            {
                pResp->frame[pResp->pos++] = DAR_SECCESS;
                update_esam_chip_state();
            }
            else
            {
                pResp->frame[pResp->pos++] = DAR_OTHER_REASON;
                pResp->frame[pResp->pos++] =  1;//Data  OPTIONAL
                pResp->frame[pResp->pos++] = DT_OCTETSTRING;
                pResp->frame[pResp->pos++] = 2;
                cosem_int162_bin(err_code,pResp->frame+pResp->pos);// esam ���ش������
                pResp->pos+=2;                
            }
        }
        else
        {
             pResp->frame[pResp->pos++] = DAR_OTHER_REASON;
             pResp->frame[pResp->pos++] = 0;
        }
        break;
    case 9://����Э��ʱЧ��������
        if((param[pos]==DT_STRUCTURE) && (param[pos+1]==2))
        {
            pos+=2;
            text_len=get_cosem_data_len(param+pos,&head_size);
            pos+=head_size;
            text=param+pos;
            pos+=text_len;

            pos++;//SID TYPE
            mem_cpy(sid,param+pos,4);
            pos+=4;
            attach_len=get_cosem_data_len(param+pos,&head_size);
            pos+=head_size;
            attach_info=param+pos;
            pos+=attach_len;
            //���ͣ� sid1+AttachData1+Endata1
            mem_cpy(pResp->frame+pResp->pos+8,sid,4);
            mem_cpy(pResp->frame+pResp->pos+8+4,attach_info,attach_len);
            mem_cpy(pResp->frame+pResp->pos+8+4+attach_len,text,text_len);
            len=esam_send_cmd(pResp->frame+pResp->pos+8,4+attach_len+text_len,pResp->frame+pResp->pos+8,pResp->max_reply_pos-pResp->pos-8,&err_code);
            if(len==0)
            {
                pResp->frame[pResp->pos++] = DAR_SECCESS;
            }
            else
            {
                pResp->frame[pResp->pos++] = DAR_OTHER_REASON;
                pResp->frame[pResp->pos++] =  1;//Data  OPTIONAL
                pResp->frame[pResp->pos++] = DT_OCTETSTRING;
                pResp->frame[pResp->pos++] = 2;
                cosem_int162_bin(err_code,pResp->frame+pResp->pos);// esam ���ش������
                pResp->pos+=2;                
            }
        }
        else
        {
             pResp->frame[pResp->pos++] = DAR_OTHER_REASON;
             pResp->frame[pResp->pos++] = 0;
        }
        break;
    }
}
void process_client_apdu_security_error(objRequest *pRequest,objResponse *pResp)
{
    
    pResp->frame[pResp->pos++]=APDU_SECURITY_RESPONSE;
    pResp->frame[pResp->pos++]=2;//����
    pResp->frame[pResp->pos++]=DAR_ESAM_VERIFY_FAIL;
    app_send_response_frame(pRequest,pResp,TRUE); 
}
void process_client_apdu_security(objRequest *pRequest,objResponse *pResp)
{

    INT16U pos,mac_len,text_len;
    INT8U  text_type,auth_type,cast_len;
    INT8U *text,sid[8],mac[4],head_size;
    INT16S len;
    INT16U err_code;

    pos=pRequest->userdata_pos+1;//���� apdu tag

    mem_set(sid,8,0x00);
    mem_set(mac,4,0x00);
    //��ȡapdu���Ļ�����
    text_type=pRequest->frame[pos++];
    text_len=get_cosem_data_len(pRequest->frame+pos,&head_size);
    pos+=head_size;
    text=pRequest->frame+pos;
    pos+=text_len;

    //������֤��Ϣ
    auth_type=pRequest->frame[pos++];
    pRequest->security_info[0]=text_type;
    pRequest->security_info[1]=SECURITYRESULT_OK;
    pRequest->security_info[2]=auth_type;
    switch(auth_type)
    {
        case SID_MAC://������֤��
            cast_len=0;
            //4�ֽ�SID
            mem_cpy(sid,pRequest->frame+pos,4);
            pos+=4;
            //������
            head_size=pRequest->frame[pos++];
            if(head_size==2)
            {
                mem_cpy(sid+4,pRequest->frame+pos,2);
            }
            else if(head_size==4)
            {
                mem_cpy(sid+4,pRequest->frame+pos,4);
            }
            pos+=head_size;
            //mac
            mac_len=pRequest->frame[pos++];
            if(mac_len==4)
            {
                mem_cpy(mac,pRequest->frame+pos,4);
            }
            mem_cpy_right(pRequest->frame+pRequest->userdata_pos+cast_len+6,text,text_len);
            mem_cpy(pRequest->frame+pRequest->userdata_pos,sid,6);
            mem_cpy(pRequest->frame+pRequest->userdata_pos+cast_len+6+text_len,mac,4);
            len=esam_send_cmd(pRequest->frame+pRequest->userdata_pos,text_len+10+cast_len,pRequest->frame+pRequest->userdata_pos,SIZE_OF_APP_PROTOCOL_FRAME-pRequest->userdata_pos,&err_code);
            if(len>0)
            {
                pRequest->frame_len=pRequest->userdata_pos+len+3;
                pRequest->frame[OOP_POS_LEN]=(pRequest->frame_len-2);
                pRequest->frame[OOP_POS_LEN+1]=(pRequest->frame_len-2)>>8;
                fcs16(pRequest->frame+OOP_POS_LEN,(pRequest->frame[OOP_POS_ADDR]&0x0F)+6);
                fcs16(pRequest->frame+OOP_POS_LEN,pRequest->frame_len-4);
                pRequest->frame[pRequest->frame_len-1] = 0x16;
                pRequest->is_security=TRUE;
                app_protocol_handler(pRequest,pResp);
            }
            else
            {
                pRequest->security_info[1]=SECURITYRESULT_NONSYMMETRY_ERR;
                pRequest->security_info[0]=SECURITYRESULT_NONSYMMETRY_ERR;
                process_client_apdu_security_error(pRequest,pResp);
            }
            break;
        case RN:    //�����
            head_size=pRequest->frame[pos++];
            mem_cpy(pRequest->frame+pRequest->userdata_pos,text,text_len);
            mem_cpy(pRequest->security_info+3,pRequest->frame+pos,head_size);
            pRequest->frame_len=pRequest->userdata_pos+text_len+3;
            pRequest->frame[OOP_POS_LEN]=(pRequest->frame_len-2);
            pRequest->frame[OOP_POS_LEN+1]=(pRequest->frame_len-2)>>8;
            fcs16(pRequest->frame+OOP_POS_LEN,(pRequest->frame[OOP_POS_ADDR]&0x0F)+6);
            fcs16(pRequest->frame+OOP_POS_LEN,pRequest->frame_len-4);
            pRequest->frame[pRequest->frame_len-1] = 0x16;
            pRequest->is_security=TRUE;
            app_protocol_handler(pRequest,pResp);
            break;
        case RN_MAC://�����+����MAC
                process_client_apdu_security_error(pRequest,pResp);
            break;
        case SID:  //��ȫ��ʶ
            //4�ֽ�SID
            mem_cpy(sid,pRequest->frame+pos,4);
            pos+=4;
            //������
            head_size=pRequest->frame[pos++];
            if(head_size==2)
            {
                mem_cpy(sid+4,pRequest->frame+pos,2);
            }
            else if(head_size==4)
            {
                mem_cpy(sid+4,pRequest->frame+pos,4);
            }
            pos+=head_size;
            mem_cpy_right(pRequest->frame+pRequest->userdata_pos+6,text,text_len);
            mem_cpy(pRequest->frame+pRequest->userdata_pos,sid,6);
            len=esam_send_cmd(pRequest->frame+pRequest->userdata_pos,text_len+6,pRequest->frame+pRequest->userdata_pos,SIZE_OF_APP_PROTOCOL_FRAME-pRequest->userdata_pos,&err_code);
            if(len>0)
            {
                pRequest->frame_len=pRequest->userdata_pos+len+3;
                pRequest->frame[OOP_POS_LEN]=(pRequest->frame_len-2);
                pRequest->frame[OOP_POS_LEN+1]=(pRequest->frame_len-2)>>8;
                fcs16(pRequest->frame+OOP_POS_LEN,(pRequest->frame[OOP_POS_ADDR]&0x0F)+6);
                fcs16(pRequest->frame+OOP_POS_LEN,pRequest->frame_len-4);
                pRequest->frame[pRequest->frame_len-1] = 0x16;
                pRequest->is_security=TRUE;
                app_protocol_handler(pRequest,pResp);
            }
            else
            {
                pRequest->security_info[1]=SECURITYRESULT_NONSYMMETRY_ERR;
                pRequest->security_info[0]=SECURITYRESULT_NONSYMMETRY_ERR;
                process_client_apdu_security_error(pRequest,pResp);
            }
            break;
        default:
            process_client_apdu_security_error(pRequest,pResp);
            break;            
    }
}

INT16U security_encode_apdu(objRequest *pRequest,objResponse *pResp)
{
    INT16U pos,idx,apdu_len;
    INT8U cmd[]={0x80,0x1C,0x00,0x11,0x00,0x00};
    INT8U mac[4];
    INT8U data[16];
    INT16U err_code;
    INT16S len;

    mem_set(mac,4,0x00);
    mem_set(data,16,0x00);
    pos=pResp->userdata_pos;

    len=pResp->pos-pResp->userdata_pos;
    apdu_len=len;
    idx=pResp->userdata_pos+10;

    if(pRequest->security_info[0]==0)//����
    {
        if(pRequest->security_info[2]==RN_MAC)//�ϱ�ʹ��
        {
            cmd[0]=0x80;
            cmd[1]=0x14;
            cmd[2]=0x01;
            cmd[3]=0x03;
        }
        else if(pRequest->security_info[2]==RN)           //��ȡ����
        {
            cmd[0]=0x80;
            cmd[1]=0x0E;
            cmd[2]=0x40;
            cmd[3]=0x02;
            len+=16;
            idx+=16;
        }
        else    //����+MAC,���ò���
        {

        }
    }
    else if(pRequest->security_info[0]==1)//����
    {
        cmd[3]=0x96;
        if(pRequest->security_info[2]==SID_MAC)
        {
            cmd[3]=0x97;
        }
    }
    else
    {
        pResp->frame[pos++]=APDU_SECURITY_RESPONSE;
        pResp->frame[pos++]=2;
        pResp->frame[pos++]=pRequest->security_info[1];
        return pos;
    }
    cosem_int162_bin(len,cmd+4);
    mem_cpy_right(pResp->frame+idx,pResp->frame+pResp->userdata_pos,len);
    if((pRequest->security_info[0]==0) && (pRequest->security_info[2]==RN))
    {
        mem_cpy(pResp->frame+pResp->userdata_pos+10,pRequest->security_info+3,16);
    }
    mem_cpy(pResp->frame+pResp->userdata_pos+4,cmd,6);

    if((pRequest->security_info[0]==0) && (pRequest->security_info[2]==RN_MAC))//�ϱ�ʹ��
    {
        mem_cpy(pRequest->frame+pRequest->userdata_pos,pResp->frame+idx,apdu_len);
        len=esam_send_cmd(pResp->frame+pResp->userdata_pos+4,len+6,pResp->frame+pResp->userdata_pos+4,SIZE_OF_APP_PROTOCOL_FRAME-pResp->userdata_pos-4,&err_code);
    }
    else
    {
        len=esam_send_cmd(pResp->frame+pResp->userdata_pos+4,len+6,pResp->frame+pResp->userdata_pos+4,SIZE_OF_APP_PROTOCOL_FRAME-pResp->userdata_pos-4,&err_code);
    }
    if(len>0)
    {
        pResp->frame[pos++]=APDU_SECURITY_RESPONSE;
        pResp->frame[pos++]=pRequest->security_info[0];
        if(pRequest->security_info[2]==SID_MAC)
        {
            pos+=set_cosem_data_len(pResp->frame+pos,len-4);
            mem_cpy(pResp->frame+pos,pResp->frame+pResp->userdata_pos+4,len);
            pos+=len;
            mem_cpy(mac,pResp->frame+pos-4,4);
            pos-=4;
            pResp->frame[pos++]=1;//mac OPIION
            pResp->frame[pos++]=0;//mac choice
            pResp->frame[pos++]=4;//mac len
            mem_cpy(pResp->frame+pos,mac,4);
            pos+=4;
        }
        else if(pRequest->security_info[2]==RN)//����+MAC
        {
            mem_cpy(mac,pResp->frame+pResp->userdata_pos+4,4);
            pos+=set_cosem_data_len(pResp->frame+pos,apdu_len);
            mem_cpy(pResp->frame+pos,pResp->frame+idx,apdu_len);
            pos+=apdu_len;
            pResp->frame[pos++]=1;//mac OPIION
            pResp->frame[pos++]=0;//mac choice
            pResp->frame[pos++]=4;//mac len
            mem_cpy(pResp->frame+pos,mac,4);
            pos+=4;
        }
        else if(pRequest->security_info[2]==RN_MAC)//�����+����MAC,�ϱ�ʹ��
        {
            pResp->frame[pos-2]=APDU_SECURITY_REQUEST;
            mem_cpy(data,pResp->frame+pResp->userdata_pos+4,12);
            mem_cpy(mac,pResp->frame+pResp->userdata_pos+16,4);
            pos+=set_cosem_data_len(pResp->frame+pos,apdu_len);
            mem_cpy(pResp->frame+pos,pRequest->frame+pRequest->userdata_pos,apdu_len);
            pos+=apdu_len;
            pResp->frame[pos++]=2;//�����+����MAC
            pResp->frame[pos++]=12;//RN len
            mem_cpy(pResp->frame+pos,data,12);
            pos+=12;
            pResp->frame[pos++]=4;//mac len
            mem_cpy(pResp->frame+pos,mac,4);
            pos+=4;
        }
        else
        {
            pos+=set_cosem_data_len(pResp->frame+pos,len);
            mem_cpy(pResp->frame+pos,pResp->frame+pResp->userdata_pos+4,len);
            pos+=len;
            pResp->frame[pos++]=0;//mac option
        }
    }
    else
    {
        pResp->frame[pos++]=APDU_SECURITY_RESPONSE;
        pResp->frame[pos++]=2;
        pResp->frame[pos++]=pRequest->security_info[1];
    }
    return pos;

}
//INT16U encode_readmeter_frame(INT8U *frame,INT16U len,INT8U rand[16])
//{
    //INT16S size,header_size;
    //INT16U userdata_pos,pos;
    //INT8U buffer[64],header[4];
    //INT16U err_code;
//
    //buffer[0]=0x80;
    //buffer[1]=0x04;
    //buffer[2]=0x00;
    //buffer[3]=0x10;
    //buffer[4]=0x00;
    //buffer[5]=0x00;
    //size=esam_send_cmd(buffer,6,buffer,64,&err_code);
    //if(size!=16)return 0;
    //mem_cpy(rand,buffer,16);
    //userdata_pos=get_oopframe_addr_length(frame)+6;
    //if(len<userdata_pos+3) return 0;
    //len-=userdata_pos+3;
    //header_size=set_cosem_data_len(header,len);
    //mem_cpy_right(frame+userdata_pos+header_size+2,frame+userdata_pos,len);
    //frame[userdata_pos]=0x10;
    //frame[userdata_pos+1]=0x00;
    //mem_cpy(frame+userdata_pos+2,header,header_size);
    //pos=userdata_pos+header_size+len+2;
    //frame[pos++]=0x01;//�����
    //frame[pos++]=size;
    //mem_cpy(frame+pos,buffer,size);
    //pos+=size;
    //
    //frame[OOP_POS_LEN] = (pos+2-1);
    //frame[OOP_POS_LEN+1] = ((pos+2-1)>>8);
    ////����HCSУ��λ
    //fcs16(frame+OOP_POS_LEN,11);
    ////����FCSУ��λ
    //fcs16(frame+OOP_POS_LEN,pos-1);
    //pos += 2;
    //frame[pos++]=0x16;
    //return pos;
//
//}

INT16U encode_readmeter_frame(INT8U *frame,INT16U len)
{
    INT16S size=16,header_size;
    INT16U userdata_pos,pos;
    INT8U buffer[64],header[4];
    INT16U err_code;

//    rs232_debug_info(frame,len);

    buffer[0]=0x80;
    buffer[1]=0x04;
    buffer[2]=0x00;
    buffer[3]=0x10;
    buffer[4]=0x00;
    buffer[5]=0x00;
//    size=esam_send_cmd(buffer,6,buffer,64,&err_code);
//    if(size!=16)return 0;
//    mem_cpy(rand,buffer,16);
//    rs232_debug_info("\xA4\xA6",2);
    userdata_pos=get_oopframe_addr_length(frame)+6;
    if(len<userdata_pos+3) return 0;
//    rs232_debug_info("\xA4\xA7",2);
    len-=userdata_pos+3;
    header_size=set_cosem_data_len(header,len);
    mem_cpy_right(frame+userdata_pos+header_size+2,frame+userdata_pos,len);
    frame[userdata_pos]=0x10;
    frame[userdata_pos+1]=0x00;
    mem_cpy(frame+userdata_pos+2,header,header_size);
    pos=userdata_pos+header_size+len+2;
    frame[pos++]=0x01;//�����
    frame[pos++]=size;
    mem_set(frame+pos,size,0x01);
    pos+=size;
    
    frame[OOP_POS_LEN] = (pos+2-1);
    frame[OOP_POS_LEN+1] = ((pos+2-1)>>8);
    //����HCSУ��λ
    fcs16(frame+OOP_POS_LEN,11);
    //����FCSУ��λ
    fcs16(frame+OOP_POS_LEN,pos-1);
    pos += 2;
    frame[pos++]=0x16;
//    rs232_debug_info(frame,pos);
    return pos;

}
//INT16U decode_readmeter_frame(INT8U *frame,INT16U len,INT8U rand[16])
INT16U decode_readmeter_frame(INT8U *frame,INT16U len)
{
    INT16S size;
    INT16U userdata_pos,pos,data_pos,text_len,idx,frame_len;
    INT8U buffer[64],meter_no[16],head_size;
    INT16U err_code;

    //ȥFE
    for(idx=0;idx<8;idx++)
    {
        if(*frame==0xFE)
        {
            frame++;
            len--;
        }
    }
    mem_set(meter_no,8,0x00);
    size=get_oopframe_addr_length(frame);

    if(size==0)return 0;
    mem_cpy(meter_no+2,frame+OOP_POS_ADDR+1,6);


    userdata_pos=get_oopframe_addr_length(frame)+6;
    
    if(frame[userdata_pos]!=0x90)return 0;//�ǰ�ȫ���䱨�Ĳ�����
    if(frame[userdata_pos+1]!=0x00)return 0;//�������ģ�Ŀǰû������

    frame_len = 0;
    frame_len += frame[1];
    frame_len += (frame[2]&0x3F) << 8;
    text_len=get_cosem_data_len(frame+userdata_pos+2,&head_size);//��ȡ���ĳ���
    if((text_len > (frame_len - userdata_pos - 7) )) return 0;//�ж��յ��ı����Ƿ���ȷ
//    mem_cpy(frame+userdata_pos+2+head_size+text_len,frame+userdata_pos+2+head_size+text_len+3,4);//��MAC��������ĩβ��ԭ�����м���3���ֽ�01 00 04

//    len=text_len+4;
//
//    data_pos=userdata_pos+6+8+16;//cmd+esam id+rand
//    mem_cpy_right(frame+data_pos,frame+userdata_pos+2+head_size,len);
//
//    len+=8+16;
//
//    pos=userdata_pos;
//    frame[pos++]=0x80;
//    frame[pos++]=0x0E;
//    frame[pos++]=0x48;
//    frame[pos++]=0x87;
//    frame[pos++]=len>>8;
//    frame[pos++]=len;
//    mem_cpy(frame+pos,meter_no,8);
//    pos+=8;
//
//    mem_cpy(frame+pos,rand,16);
////    pos+=16;
//
////    pos+=len;
//    
//    //rs232_debug_info(frame+userdata_pos,len+6,buffer,64);
////    size=esam_send_cmd(frame+userdata_pos,len+6,buffer,64,&err_code);
//    if(size<0)return 0;
    data_pos = userdata_pos+2+head_size ;
    mem_cpy(frame+userdata_pos,frame+data_pos,text_len);
    pos=userdata_pos+ text_len;

    frame[OOP_POS_LEN] = (pos+2-1);
    frame[OOP_POS_LEN+1] = ((pos+2-1)>>8);
    //����HCSУ��λ
    fcs16(frame+OOP_POS_LEN,11);
    //����FCSУ��λ
    fcs16(frame+OOP_POS_LEN,pos-1);
    pos += 2;
    frame[pos++]=0x16;

//    rs232_debug_info(frame,pos);
    return pos;
}
#endif

//void process_client_apdu_connect(objRequest *pRequest,objResponse *pResp)
//{
//    extern const INT8U PROTOCOL_CONFORMANCE[];
//    extern const INT8U FUNCTION_CONFORMANCE[];
//    extern const INT8U dev_ver_info_default[];
//    extern const INT8U dev_ver_info_3_02[];
//
//    INT8U get_protocol_conformance_count();
//    INT8U get_function_conformance_count();
//    INT8U load_link_password(INT8U *password,INT8U max_len);
//
//    INT16U pos,idx,p_count,f_count,count,ciphertext_len,signature_len;
//    INT16S len;
//    INT32U protocol;
//    INT8U bit_info[16];
//    INT8U connect_mode,pwd_len,pwd_len2,head_size;
//    INT8U password[34];
//    INT8U *ciphertext,*signature,*buffer;
//    INT8U *dev_ver_info;
//
//    if(gSystemInfo.hardware_version == 0x02)
//    {
//        dev_ver_info = (INT8U*)dev_ver_info_3_02;
//    }
//    else
//    {
//        dev_ver_info = (INT8U*)dev_ver_info_default;
//    }
//    
//    mem_set(password,34,0x00);
//
//    //���ÿ����룺DIR=1,PRM=0, FUNC=8  ��Ӧ���ģ��û�����
//    pResp->frame[OOP_POS_CTRL] = CTRLFUNC_DIR_SET | CTRLFUNC_PRM_SET |  CTRL_FUNC_USERDATA;
//    pResp->frame[pResp->pos++] =  SERVER_APDU_CONNECT; //���Ӧ���������
//    pResp->frame[pResp->pos++] =  pRequest->frame[pRequest->userdata_pos+1];        //���Ӧ��������
//
//    pos=pRequest->userdata_pos+1;
//
//    pos++;//PIID
//    //���̰汾��Ϣ
//    mem_cpy(pResp->frame+pResp->pos,(INT8U*)dev_ver_info+4,4);
//    pResp->pos+=4;
//    mem_cpy(pResp->frame+pResp->pos,(INT8U*)dev_ver_info+10,4);
//    pResp->pos+=4;
//    mem_cpy(pResp->frame+pResp->pos,(INT8U*)dev_ver_info+16,6);
//    pResp->pos+=6;
//    mem_cpy(pResp->frame+pResp->pos,(INT8U*)dev_ver_info+24,4);
//    pResp->pos+=4;
//    mem_cpy(pResp->frame+pResp->pos,(INT8U*)dev_ver_info+30,6);
//    pResp->pos+=6;
//    mem_cpy(pResp->frame+pResp->pos,(INT8U*)dev_ver_info+38,8);
//    pResp->pos+=8;
//
//    //Э��汾��
//    protocol=cosem_bcd2_int16u(pRequest->frame+pos);
//    pos+=2;
//
//    if(protocol!=PROTOCOL_VER)
//    {
//        //Э�鲻��ͬ��Ŀǰ��������ֻ���Լ�Э�鴦��
//    }
//    cosem_int162_bcd(PROTOCOL_VER,pResp->frame+pResp->pos);
//    pResp->pos+=2;
//   //Э��һ���Կ�,ȡ����
//    count=64;
//    p_count=get_protocol_conformance_count();
//    mem_set(bit_info,16,0x00);
//    for(idx=0;idx<p_count;idx++)
//    {
//        if((PROTOCOL_CONFORMANCE[idx]<count) && get_oop_bit_value(pRequest->frame+pos,8,PROTOCOL_CONFORMANCE[idx]))
//        {
//            set_oop_bit_value(bit_info,8,PROTOCOL_CONFORMANCE[idx]);
//        }
//        else
//        {
//            clr_oop_bit_value(bit_info,8,PROTOCOL_CONFORMANCE[idx]);
//        }
//    }
//    pos+=count/8;
//
//    mem_cpy(pResp->frame+pResp->pos,bit_info,8);
//    pResp->pos+=8;
//
//   //����һ���Կ�,ȡ����
//    count=128;
//    f_count=get_function_conformance_count();
//    mem_set(bit_info,16,0x00);
//    for(idx=0;idx<f_count;idx++)
//    {
//        if((FUNCTION_CONFORMANCE[idx]<count) && get_oop_bit_value(pRequest->frame+pos,8,FUNCTION_CONFORMANCE[idx]))
//        {
//            set_oop_bit_value(bit_info,8,FUNCTION_CONFORMANCE[idx]);
//        }
//        else
//        {
//            clr_oop_bit_value(bit_info,8,FUNCTION_CONFORMANCE[idx]);
//        }
//    }
//    pos+=count/8;
//
//    mem_cpy(pResp->frame+pResp->pos,bit_info,16);
//    pResp->pos+=16;
//
//   //�ͻ�������֡���ߴ�
//    g_app_link_info.client_send_frame_size =cosem_bin2_int16u(pRequest->frame+pos);
//    pos+=2;
//     if(g_app_link_info.client_send_frame_size>MAX_SEND_FRAME_SIZE)
//     {
//        g_app_link_info.client_send_frame_size=MAX_SEND_FRAME_SIZE;
//     }
//    cosem_int162_bin(g_app_link_info.client_send_frame_size,pResp->frame+pResp->pos);
//    pResp->pos+=2;
//   //�ͻ�������֡���ߴ�
//    g_app_link_info.client_recv_frame_size =cosem_bin2_int16u(pRequest->frame+pos);
//    if(g_app_link_info.client_recv_frame_size>MAX_RECV_FRAME_SIZE)
//    {
//        g_app_link_info.client_recv_frame_size=MAX_RECV_FRAME_SIZE;
//    }
//    if(pRequest->pChannel!=NULL)
//    {
//        ((objProtocolChannel *)pRequest->pChannel)->max_send_frame_size = g_app_link_info.client_recv_frame_size;//���ķ���֡���ߴ�
//    }
//    pos+=2;
//    cosem_int162_bin(g_app_link_info.client_send_frame_size,pResp->frame+pResp->pos);
//    pResp->pos+=2;
//
//   //�ͻ����������ߴ�֡����
//   g_app_link_info.client_recv_frame_count=pRequest->frame[pos++];
//   if(g_app_link_info.client_recv_frame_count>MAX_RECV_FRAME_COUNT)
//   {
//    g_app_link_info.client_recv_frame_count=MAX_RECV_FRAME_COUNT;
//   }
//   pResp->frame[pResp->pos++]=g_app_link_info.client_recv_frame_count;
//
//   //�ͻ������ɴ���APDU�ߴ�
//    g_app_link_info.client_apdu_max_size =cosem_bin2_int16u(pRequest->frame+pos);
//    if(pRequest->pChannel!=NULL)
//    {
//         ((objProtocolChannel *)pRequest->pChannel)->max_apdu_size = cosem_bin2_int16u(pRequest->frame+pos); //����apdu���ն���֯���ͱ���ʱ��apdu�����ߴ�
//    }   
//    pos+=2;
//    if(g_app_link_info.client_apdu_max_size>MAX_APDU_SIZE)
//    {
//        g_app_link_info.client_apdu_max_size=MAX_APDU_SIZE;
//    }
//   cosem_int162_bin(g_app_link_info.client_apdu_max_size,pResp->frame+pResp->pos);
//   pResp->pos+=2;
//
//
//   //������Ӧ�����ӳ�ʱʱ��
//   g_app_link_info.link_time_out=cosem_bin2_int32u(pRequest->frame+pos);
//    if(pRequest->pChannel!=NULL)
//    {   
//        ((objProtocolChannel *)pRequest->pChannel)->link_start_timer = system_get_tick10ms();
//        ((objProtocolChannel *)pRequest->pChannel)->link_time_out = cosem_bin2_int32u(pRequest->frame+pos); //������Ӧ�����ӳ�ʱʱ��
//    }
//   pos+=4;
//   cosem_int32u2_bin(g_app_link_info.link_time_out,pResp->frame+pResp->pos);
//   pResp->pos+=4;
//   //��ȫ��֤�������
//   connect_mode=pRequest->frame[pos++];
//   //pResp->frame[pResp->pos++]=2;
//   switch(connect_mode)
//   {
//    case 0://��������    [0] NullSecurity��
//        pResp->frame[pResp->pos++]=SECURITYRESULT_OK;
//        pResp->frame[pResp->pos++]=0;
//        break;
//    case 1://һ������    [1] PasswordSecurity��
//        //��ȡ�ն�����
//        pwd_len2=read_custom_param(CONST_LINK_PWD,password);//load_link_password(password,34);
//        pwd_len=pRequest->frame[pos++];
//        if((pwd_len2==pwd_len) && (compare_string(pRequest->frame+pos,password,pwd_len)==0))
//        {
//            pResp->frame[pResp->pos++]=SECURITYRESULT_OK;
//        }
//        else
//        {
//            pResp->frame[pResp->pos++]=SECURITYRESULT_PWD_ERR;
//        }
//        pResp->frame[pResp->pos++]=0;
//        break;
//    #if defined __ENABLE_ESAM2__
//    case 2://�ԳƼ���    [2] SymmetrySecurity
//        ciphertext_len=get_cosem_data_len(pRequest->frame+pos,&head_size);
//        pos+=head_size;
//        ciphertext=pRequest->frame+pos;
//        pos+=ciphertext_len;
//        signature_len=get_cosem_data_len(pRequest->frame+pos,&head_size);
//        pos+=head_size;
//        signature=pRequest->frame+pos;
////        pos+=signature_len;
//        mem_cpy(ciphertext+ciphertext_len,signature,signature_len);
//
//
//        pResp->frame[pResp->pos++]=SECURITYRESULT_OK;//Ĭ����֤�ɹ�
//        pResp->frame[pResp->pos++]=1;//Ĭ����֤�ɹ�
//        buffer=pResp->frame+pResp->pos+1;
//        len=esam_session_auth(ciphertext,ciphertext_len+signature_len,buffer,pResp->max_reply_pos-pResp->frame_len-1);
//        if(len<112)
//        {
//            pResp->frame[pResp->pos-2]=SECURITYRESULT_NONSYMMETRY_ERR;
//            pResp->frame[pResp->pos-1]=0;
//        }
//        else
//        {
//            pResp->frame[pResp->pos++]=len-64;
//            pResp->pos+=len-64;
//            mem_cpy_right(pResp->frame+pResp->pos+1,pResp->frame+pResp->pos,64);
//            pResp->frame[pResp->pos++]=64;
//            pResp->pos+=64;
//        }
//        break;
//    case 3://����ǩ��    [3] SignatureSecurity
//        ciphertext_len=get_cosem_data_len(pRequest->frame+pos,&head_size);
//        pos+=head_size;
//        ciphertext=pRequest->frame+pos;
//        pos+=ciphertext_len;
//        
//        signature_len=get_cosem_data_len(pRequest->frame+pos,&head_size);
//        pos+=head_size;
//        signature=pRequest->frame+pos;
////        pos+=signature_len;
//        
//        pResp->frame[pResp->pos++]=SECURITYRESULT_OK;//Ĭ����֤�ɹ�
//        pResp->frame[pResp->pos++]=1;//Ĭ����֤�ɹ�
//        mem_cpy(ciphertext+ciphertext_len,signature,signature_len);
//        buffer=pResp->frame+pResp->pos+1;
//        len=esam_session_auth(ciphertext,ciphertext_len+signature_len,buffer,pResp->max_reply_pos-pResp->pos-1);
//        if(len<112)
//        {
//            pResp->frame[pResp->pos-2]=SECURITYRESULT_NONSYMMETRY_ERR;
//            pResp->frame[pResp->pos-1]=0;
//        }
//        else
//        {
//            pResp->frame[pResp->pos++]=len-64;
//            pResp->pos+=len-64;
//            mem_cpy_right(pResp->frame+pResp->pos+1,pResp->frame+pResp->pos,64);
//            pResp->frame[pResp->pos++]=64;
//            pResp->pos+=64;
//        }
//        break;
//    #endif
//    default:
//        pResp->frame[pResp->pos++]=SECURITYRESULT_PWD_ERR;
//        pResp->frame[pResp->pos++]=0;
//   }
////   rs232_debug_info(pResp->frame,pResp->pos);
//   app_send_response_frame(pRequest,pResp,TRUE);
//
//}
//void process_client_apdu_disconnect(objRequest *pRequest,objResponse *pResp)
//{
//    //���ÿ����룺DIR=1,PRM=0, FUNC=8  ��Ӧ���ģ��û�����
//    pResp->frame[OOP_POS_CTRL] = CTRLFUNC_DIR_SET | CTRLFUNC_PRM_SET |  CTRL_FUNC_USERDATA;
//    pResp->frame[pResp->pos++] =  SERVER_APDU_RELEASE_RESP; //���Ӧ���������
//    pResp->frame[pResp->pos++] =  pRequest->frame[pRequest->userdata_pos+1];        //���Ӧ��������
//    pResp->frame[pResp->pos++] =  0;//�Ͽ��ɹ�
//    
//   //�ͻ�������֡���ߴ�
//    g_app_link_info.client_send_frame_size=MAX_SEND_FRAME_SIZE;
//    if(pRequest->pChannel!=NULL)
//    {
//        ((objProtocolChannel *)pRequest->pChannel)->max_send_frame_size = g_app_link_info.client_send_frame_size;
//    }
//     //�ͻ�������֡���ߴ�
//    g_app_link_info.client_recv_frame_size =MAX_RECV_FRAME_SIZE;
//   //�ͻ����������ߴ�֡����
//    g_app_link_info.client_recv_frame_count=MAX_RECV_FRAME_COUNT;
//
//   //�ͻ������ɴ���APDU�ߴ�
//    g_app_link_info.client_apdu_max_size =MAX_APDU_SIZE;
//    if(pRequest->pChannel!=NULL)
//    {
//         ((objProtocolChannel *)pRequest->pChannel)->max_apdu_size = g_app_link_info.client_apdu_max_size; //����apdu���ն���֯���ͱ���ʱ��apdu�����ߴ�
//    }
//   //������Ӧ�����ӳ�ʱʱ��
//    g_app_link_info.link_time_out=0;
//    if(pRequest->pChannel!=NULL)
//    {
//        ((objProtocolChannel *)pRequest->pChannel)->link_time_out = g_app_link_info.link_time_out; //������Ӧ�����ӳ�ʱʱ��
//    }
//    app_setFrameLenThenSendFrame(pRequest,pResp);
//
//}
BOOLEAN check_safe_config(INT16U obis,INT8U operate,INT8U *security_info)
{
    INT16U config_word;
    INT8U safe_word,check_safe_word;
    BOOLEAN ret;

    if((security_info[0]==1) )//����
    {
        if((security_info[2]==0) || (security_info[2]==2))//MAC
        {
            check_safe_word=0x01;
        }
        else
        {
            check_safe_word=0x02;
        }
    }
    else
    {
        if((security_info[2]==0)|| (security_info[2]==1) || (security_info[2]==2))//MAC
        {
            check_safe_word=0x04;
        }
        else
        {
            check_safe_word=0x08;
        }
    }
    ret=FALSE;
    //��ȡ��ȫģʽ��
    config_word=get_esam_safe_config(obis);
    switch(operate)
    {
        case OP_GET:
            safe_word=(config_word>>12) & 0x0F;
            if(safe_word==0)
            {
                safe_word=0x08;
            }
            ret=check_safe_word<=safe_word;
            break;
        case OP_SET:
            safe_word=(config_word>>8) & 0x0F;;
            if(safe_word==0)
            {
                safe_word=0x08;
            }
            ret=check_safe_word<=safe_word;
            break;
        case OP_ACTION:
            safe_word=(config_word>>4) & 0x0F;
            if(safe_word==0)
            {
                safe_word=0x08;
            }
            ret=check_safe_word<=safe_word;
            break;
        case OP_PROXY://�ն˱��������ڴ�������ն˵����������Ŀǰû����
            safe_word=config_word & 0x0F;
            ret=check_safe_word<=safe_word;
            break;
        default:
            ret = TRUE;
    }
    if(get_system_flag(SYS_ESAM,SYS_FLAG_BASE))
    {
        return ret;
    }
    return TRUE;
}

