#include "protocol_app_handler.h"
#include "protocol_app_const_def.h"
#include "object_describe.h"
#include "apdu_service_handler.h"

void exec_block_transmit_method(OBJECT_ATTR_DESC *oad,INT8U* param,objResponse *pResp,INT8U *resetFlag);
void exec_object_acq_ctrl_info_method(OBJECT_ATTR_DESC *oad,INT8U* param,objResponse *pResp);
void exec_object_meter_doc_method(OBJECT_ATTR_DESC *oad,INT8U* param,objResponse *pResp);
INT8U read_acq_plan_data_from_meter(objRequest *pRequest,objResponse *pResp,RSD *rsd,INT8U *rcsd,INT16U left_len,SEPARATE_FRAME *separate_frame);
INT8U read_acq_plan_data_from_flash(objRequest *pRequest,objResponse *pResp,RSD *rsd,INT8U *rcsd,INT16U left_len,SEPARATE_FRAME *separate_frame);
void exec_esam_method(OBJECT_ATTR_DESC *oad,INT8U* param,objResponse *pResp);
void exec_esam_oi_table_method(OBJECT_ATTR_DESC *oad,INT8U* param,objResponse *pResp);

#define  ASC_HIGH(o)   (((o>>4) & 0x0F) | 0x30)
#define  ASC_LOW(o)   ((o & 0x0F)| 0x30)


#define SOFT_VER_YEAR    0x19
#define SOFT_VER_MONTH   0x05
#define SOFT_VER_DAY     0x12

#define SOFT_VER_MAIN   '1'
#define SOFT_VER_MIN1   '0'

const INT8U dev_ver_info_default[]=
{
    DT_STRUCTURE,0x06,
    DT_VISIBLESTRING,4,'T','O','P','S',
    DT_VISIBLESTRING,4,'V',SOFT_VER_MAIN,'.',SOFT_VER_MIN1,
    DT_VISIBLESTRING,6,ASC_HIGH(SOFT_VER_YEAR),ASC_LOW(SOFT_VER_YEAR),ASC_HIGH(SOFT_VER_MONTH),ASC_LOW(SOFT_VER_MONTH),ASC_HIGH(SOFT_VER_DAY),ASC_LOW(SOFT_VER_DAY),
    DT_VISIBLESTRING,4,'1','.','0','0',
    DT_VISIBLESTRING,6,'1','7','1','0','2','1',
    DT_VISIBLESTRING,8,' ',' ',' ',' ',' ',' ',' ',' ',
};

INT8U process_client_apdu_get(objRequest *pRequest,objResponse *pResp)
{
    INT8U result=0;
	INT16U startsector =0;
	INT16U endsector = 0;
	INT16U page_idx = 0;
	INT8U  buffer[512] ={0};
	INT8U  bufferLoop =0;
    //���ÿ����룺DIR=1,PRM=0, FUNC=8  ��Ӧ���ģ��û�����
    pResp->frame[OOP_POS_CTRL] = CTRLFUNC_DIR_SET | CTRLFUNC_PRM_SET |  CTRL_FUNC_USERDATA;
    pResp->frame[pResp->pos++] =  SERVER_APDU_GET; //���Ӧ���������
    pResp->frame[pResp->pos++] =  pRequest->frame[pRequest->userdata_pos+1];        //���Ӧ��������

    switch(pRequest->frame[pRequest->userdata_pos+1])
    {
        case GET_REQUEST_NORMAL:                          //1)	��ȡһ���������ԣ�
            init_separate_frame_info(pRequest->frame[pRequest->userdata_pos+1]);
            result=GetRequestNormal(pRequest,pResp);
            break;
        case GET_REQUEST_NORMAL_LIST:                    //2)	��ȡ����������ԣ�
            init_separate_frame_info(pRequest->frame[pRequest->userdata_pos+1]);
            result=GetRequestNormalList(pRequest,pResp);
            break;
        case GET_REQUEST_RECORD:                    //3 ��ȡ1����¼�Ͷ�������
            init_separate_frame_info(pRequest->frame[pRequest->userdata_pos+1]);
        	result=GetRequestRecord(pRequest,pResp);
        	break;
		case GET_REQUEST_RECORD_LIST:            //4 ��ȡ�����¼�Ͷ�������
			init_separate_frame_info(pRequest->frame[pRequest->userdata_pos+1]);
			result=GetRequestRecordList(pRequest,pResp);
			break;			
        case GET_REQUEST_NEXT:                          //5)	��ȡ��֡��Ӧ����һ�����ݿ�
            result=GetRequestNext(pRequest,pResp);
            break;
		case GET_FLASH_CONTENT: //������Ϣ��0506000100010002    68 18 00 43 05 02 00 00 00 00 00 10 04 42 05 06 00 01 00 01 00 02 00 5D 4B 16    68 18 00 43 05 11 11 11 11 11 11 10 26 2A 05 06 00 01 00 00 00 01 00 8E 7D 16 
			pRequest->userdata_pos +=3;
			switch(pRequest->frame[pRequest->userdata_pos])
			{
				case 0x01:
					startsector = (pRequest->frame[pRequest->userdata_pos+1]<<8) +pRequest->frame[pRequest->userdata_pos+2];
					pRequest->userdata_pos +=3;
					endsector = (pRequest->frame[pRequest->userdata_pos]<<8) +pRequest->frame[pRequest->userdata_pos+1];

					tpos_mutexPend(&SIGNAL_DEBUG_UART);
					for(page_idx =startsector;page_idx<endsector;page_idx++)
					{
						for(bufferLoop =0;bufferLoop<8;bufferLoop++)
						{
							nor_flash_read_data(page_idx,0+512*bufferLoop,buffer,512);
							debug_uart_send_buf((uint8_t*)buffer,512);
						}
					}
					tpos_mutexFree(&SIGNAL_DEBUG_UART);
				result =1;  //�����ٽ�����ת�������
				break;
			}
			break;
    }
    return result;
}
INT8U GetRequestRecordList(objRequest *pRequest,objResponse *pResp)
{

}
INT8U GetRequestNormal(objRequest *pRequest,objResponse *pResp)
{
    INT8U result=0;
    INT16U data_pos;
    SEPARATE_FRAME *separate_frame=NULL;

    separate_frame=&separate_frame_mem;
    data_pos = pResp->pos;
    pResp->frame[pResp->pos++] = pRequest->frame[pRequest->userdata_pos+2];         //���PIID-ACD��todo������Ҫ����ACD
    pRequest->pos=pRequest->userdata_pos+3;
    separate_frame->pos=pRequest->pos;

    if(process_get_object_attribute(pRequest,pResp,separate_frame))
    {
        if(separate_frame->more_frame)
        {
            //�״η�֡ʱ�ȱ��������ģ��Ժ��ٱ���
            if(separate_frame->is_frist)
            {
                mem_cpy(separate_frame->frame,pRequest->frame,pRequest->frame_len);
                separate_frame->frame_len=pRequest->frame_len;
                separate_frame->is_frist = FALSE;
                separate_frame->userdata_pos =pRequest->userdata_pos;
            }

            //Ϊ����֡��Ϣ��׼������APDU���ݺ���4���ֽ�
            mem_cpy_right(pResp->frame+data_pos+6,pResp->frame+data_pos+1,pResp->pos-data_pos-1);
            pResp->pos+=5;
            //����֡��Ϣ��
            pResp->frame[data_pos-1]=GET_RESPONSE_NEXT; //���Ӧ���������
            pResp->frame[data_pos+0]=pRequest->frame[pRequest->userdata_pos+2];         //���PIID
            pResp->frame[data_pos+1]=0;//��֡��־,��֡���������֡
            pResp->frame[data_pos+2]=0;//�״η�֡����֡�����0
            pResp->frame[data_pos+3]=1;//�״η�֡����֡�����1
            pResp->frame[data_pos+4]=1;//��������
            pResp->frame[data_pos+5]=1;//�������
        }
        app_send_response_frame(pRequest,pResp,TRUE);
        result=1;
    }
    return result;
}

INT8U GetRequestNormalList(objRequest *pRequest,objResponse *pResp)
{
    INT16U idx,count,data_pos;
    INT16U start_pos,left_len;
    BOOLEAN is_next_object;
    SEPARATE_FRAME *separate_frame=NULL;

    separate_frame=&separate_frame_mem;
    data_pos = pResp->pos;

    pResp->frame[pResp->pos++] = pRequest->frame[pRequest->userdata_pos+2];         //���PIID
    pResp->frame[pResp->pos++] = pRequest->frame[pRequest->userdata_pos+3];         //���Ԫ�ظ���
    count=pRequest->frame[pRequest->userdata_pos+3];//Ԫ�ظ���
    pRequest->pos=pRequest->userdata_pos+4;

    separate_frame->batch_count=count;
    separate_frame->idx_count=0;
    for(idx=0;idx<count;idx++)
    {
        start_pos = pRequest->pos;
        left_len=pResp->max_reply_pos - pResp->pos-5;
        separate_frame->pos=pRequest->pos;
        if(process_get_object_attribute(pRequest,pResp,separate_frame))
        {
            is_next_object=(pRequest->pos>start_pos);
            if(separate_frame->more_frame)
            {
                //�״η�֡ʱ�ȱ��������ģ��Ժ��ٱ���
                if(separate_frame->is_frist)
                {
                    mem_cpy(separate_frame->frame,pRequest->frame,pRequest->frame_len);
                    separate_frame->frame_len=pRequest->frame_len;
                    separate_frame->is_frist = FALSE;
                    separate_frame->userdata_pos =pRequest->userdata_pos;
                    separate_frame->pos          =pRequest->pos;
                }
                //Ϊ����֡��Ϣ��׼������APDU���ݺ���5���ֽ�
                mem_cpy_right(pResp->frame+data_pos+5,pResp->frame+data_pos+1,pResp->pos-data_pos-1);
                pResp->pos+=4;
                //����֡��Ϣ��
                pResp->frame[data_pos-1]=GET_RESPONSE_NEXT; //����֡Ӧ��
                pResp->frame[data_pos+0]=pRequest->frame[pRequest->userdata_pos+2];         //���PIID-ACD
                pResp->frame[data_pos+1]=0;//��֡��־,��֡���������֡
                pResp->frame[data_pos+2]=0;//�״η�֡����֡�����0
                pResp->frame[data_pos+3]=1;//�״η�֡����֡�����1
                pResp->frame[data_pos+4]=1;//��������
                pResp->frame[data_pos+5]=is_next_object?(idx+1):idx;
            }
            else
            {
                separate_frame->idx_count++;
            }
        }
        else
        {
            return 0;
        }
    }
    app_send_response_frame(pRequest,pResp,TRUE);
    return 1;
}

INT8U GetRequestRecord(objRequest *pRequest,objResponse *pResp)
{
	INT8U result=0;
    BOOLEAN is_more_frame=FALSE;
	INT16U left_len,data_pos;
    SEPARATE_FRAME *separate_frame=NULL;

    separate_frame=&separate_frame_mem;
	
    data_pos = pResp->pos;
	pRequest->pos = pRequest->userdata_pos+3;
    pResp->frame[pResp->pos++] = pRequest->frame[pRequest->userdata_pos+2];         //���PIID
    left_len=pResp->max_reply_pos-pResp->pos;

    separate_frame->pos=pRequest->pos;
    if(process_req_record_object_attribute(pRequest,pResp,left_len,separate_frame))
    {
        if(separate_frame->more_frame)
        {
            if(separate_frame->is_frist)
            {
                mem_cpy(separate_frame->frame,pRequest->frame,pRequest->frame_len);
                separate_frame->frame_len=pRequest->frame_len;
                separate_frame->is_frist = FALSE;
                separate_frame->userdata_pos =pRequest->userdata_pos;
            }
            //Ϊ����֡��Ϣ��׼������APDU���ݺ���4���ֽ�
            mem_cpy_right(pResp->frame+data_pos+6,pResp->frame+data_pos+1,pResp->pos-data_pos-1);
            pResp->pos += 5;
            //����֡��Ϣ��
            pResp->frame[data_pos-1]=GET_RESPONSE_NEXT; //���Ӧ���������
            pResp->frame[data_pos+0]=pRequest->frame[pRequest->userdata_pos+2];         //���PIID
            pResp->frame[data_pos+1]=0;//��֡��־,��֡���������֡
            pResp->frame[data_pos+2]=0;//�״η�֡����֡�����0
            pResp->frame[data_pos+3]=1;//�״η�֡����֡�����1
            pResp->frame[data_pos+4]=2;//��¼�Ͷ�������
            pResp->frame[data_pos+5]=1;//��¼�Ͷ������
        }
        app_send_response_frame(pRequest,pResp,TRUE);
        result=1;
    }
    return result;
}

void request_object_list(objRequest *pRequest,SEPARATE_FRAME *separate_frame,objResponse *pResp,INT16U separate_flag_pos)
{

NEXT_OAD:
    process_get_object_attribute(pRequest,pResp,separate_frame);
    pResp->frame[separate_flag_pos+4]++;//�������
    if(separate_frame->more_frame)
    {
        return;
    }
    else
    {
       separate_frame->pos=pRequest->pos;
       separate_frame->idx_count++;
       if((separate_frame->idx_count < separate_frame->batch_count)&&(pResp->max_reply_pos>(pResp->pos+10)))
        {
            goto NEXT_OAD;
        }
    }
}

INT8U GetRequestNext(objRequest *pRequest,objResponse *pResp)
{
    INT16U frame_id,separate_flag_pos,left_len;
    BOOLEAN is_more_frame=FALSE;
    INT8U result;
    SEPARATE_FRAME *separate_frame=NULL;

    separate_frame=&separate_frame_mem;

    frame_id = pRequest->frame[pRequest->userdata_pos+3]*0x100+pRequest->frame[pRequest->userdata_pos+4];   //�����֡���
    pResp->frame[pResp->pos++] = pRequest->frame[pRequest->userdata_pos+2];                                 //���PIID
    separate_flag_pos=pResp->pos;
    pResp->frame[pResp->pos++] =  0;                        //��֡��־
    pResp->frame[pResp->pos++] = (frame_id+1)>>8;           //��֡���
    pResp->frame[pResp->pos++] = (frame_id+1);              //��֡���

    result=process_prepare_separate_frame_info(pRequest,pResp,separate_frame,frame_id);
    if(result==DAR_SECCESS)
    {
        left_len=pResp->max_reply_pos - pResp->pos;
        if(separate_frame->apdu_service_code==GET_REQUEST_RECORD)
        {
            process_req_record_object_attribute(pRequest,pResp,left_len,separate_frame);
        }
        else if(separate_frame->apdu_service_code==GET_REQUEST_NORMAL)
        {
            process_get_object_attribute(pRequest,pResp,separate_frame);
        }
        else if(separate_frame->apdu_service_code==GET_REQUEST_NORMAL_LIST)
        {
            request_object_list(pRequest,separate_frame,pResp,separate_flag_pos);
        }
        pResp->frame[separate_flag_pos]=separate_frame->more_frame?0:1;
    }
    else
    {
        pResp->frame[separate_flag_pos]=1;
        pResp->frame[separate_flag_pos+3]=0;
        pResp->frame[separate_flag_pos+4]=result;
    }

    app_send_response_frame(pRequest,pResp,TRUE);
    return 1;
}

INT8U process_client_apdu_set(objRequest *pRequest,objResponse *pResp)
{
    INT8U result=0;

    //���ÿ����룺DIR=1,PRM=0, FUNC=8  ��Ӧ���ģ��û�����
    pResp->frame[OOP_POS_CTRL] = CTRLFUNC_DIR_SET | CTRLFUNC_PRM_SET |  CTRL_FUNC_USERDATA;
    pResp->frame[pResp->pos++] =  SERVER_APDU_SET; //���Ӧ���������
    pResp->frame[pResp->pos++] =  pRequest->frame[pRequest->userdata_pos+1];        //���Ӧ��������

    switch(pRequest->frame[pRequest->userdata_pos+1])
    {
        case SET_REQUEST_NORMAL:                          //1)	����һ���������ԣ�
            result=SetRequestNormal(pRequest,pResp);
            break;
        case SET_REQUEST_NORMA_LLIST:                    //2)	���ö���������ԣ�
            result=SetRequestNormalList(pRequest,pResp);
            break;
        case SET_THEN_GET_REQUEST_NORMAL_LIST:                        //3)���ú��ȡ��������������ԣ�
            result=SetThenGetRequestNormalList(pRequest,pResp);
            break;
    }
    return result;
}
INT8U SetRequestNormal(objRequest *pRequest,objResponse *pResp)
{
    INT16U start_pos;
    
    pResp->frame[pResp->pos++] = pRequest->frame[pRequest->userdata_pos+2];     //���PIID-ACD
    pRequest->pos=pRequest->userdata_pos+3;
    
    start_pos = pRequest->pos;

    if(process_set_object_attribute(pRequest,pResp))
    {
        pRequest->pos += get_object_attribute_item_len(pRequest->frame+start_pos+4,pRequest->frame[start_pos + 4]);
        app_send_response_frame(pRequest,pResp,TRUE);
        return 1;
    }
    else
    {
        return 0;
    }
}
INT8U SetRequestNormalList(objRequest *pRequest,objResponse *pResp)
{
    INT16U idx,start_pos,count;
    INT8U pos;

    pResp->frame[pResp->pos++] = pRequest->frame[pRequest->userdata_pos+2];     //���PIID-ACD

    pResp->frame[pResp->pos++] = pRequest->frame[pRequest->userdata_pos+3];     //���Ԫ�ظ���
    count=pRequest->frame[pRequest->userdata_pos+3];//Ԫ�ظ���
    pRequest->pos=pRequest->userdata_pos+4;

    pos = 0;
    for(idx=0;idx<count;idx++)
    {
        start_pos = pRequest->pos;

        if(process_set_object_attribute(pRequest,pResp))
        {
            pRequest->pos += get_object_attribute_item_len(pRequest->frame+start_pos+4,pRequest->frame[start_pos + 4]);
        }
        else
        {
            return 0;
        }
    }

    app_send_response_frame(pRequest,pResp,TRUE);
    return 1;
}
INT8U SetThenGetRequestNormalList(objRequest *pRequest,objResponse *pResp)
{
    INT16U idx,count;
    INT16U start_pos;
    INT8U pos;
    SEPARATE_FRAME *separate_frame=NULL;

    separate_frame=&separate_frame_mem;

    pResp->frame[pResp->pos++] = pRequest->frame[pRequest->userdata_pos+2];     //���PIID-ACD
    pResp->frame[pResp->pos++] = pRequest->frame[pRequest->userdata_pos+3];     //���Ԫ�ظ���
    count=pRequest->frame[pRequest->userdata_pos+3];//Ԫ�ظ���
    pRequest->pos=pRequest->userdata_pos+4;

    pos = 0;
    for(idx=0;idx<count;idx++)
    {
        start_pos = pRequest->pos;

        if(process_set_object_attribute(pRequest,pResp))
        {
            pRequest->pos += get_object_attribute_item_len(pRequest->frame+start_pos+4,pRequest->frame[start_pos + 4]);
            if(process_get_object_attribute(pRequest,pResp,separate_frame))
            {
                pRequest->pos+=4;
            }
            else
            {
                return 0;
            }
        }
        else
        {
            return 0;
        }
    }

    app_send_response_frame(pRequest,pResp,TRUE);
    return 1;
}
INT8U process_client_apdu_action(objRequest *pRequest,objResponse *pResp,INT8U *reset_flag)
{
    INT8U result=0;

    //���ÿ����룺DIR=1,PRM=0, FUNC=8  ��Ӧ���ģ��û�����
    pResp->frame[OOP_POS_CTRL] = CTRLFUNC_DIR_SET | CTRLFUNC_PRM_SET |  CTRL_FUNC_USERDATA;
    pResp->frame[pResp->pos++] =  SERVER_APDU_ACTION; //���Ӧ���������
    pResp->frame[pResp->pos++] =  pRequest->frame[pRequest->userdata_pos+1];        //���Ӧ��������

    switch(pRequest->frame[pRequest->userdata_pos+1])
    {
        case ACTION_REQUEST:                          //1)	����һ�����󷽷���
            result=ActRequestNormal(pRequest,pResp,reset_flag);
            break;
    }
    return result;
}
INT8U ActRequestNormal(objRequest *pRequest,objResponse *pResp,INT8U *reset_flag)
{
    INT16U obis,len;
    INT8U dar;
    OBJECT_ATTR_DESC *oad;
    
    pResp->frame[pResp->pos++] = pRequest->frame[pRequest->userdata_pos+2];     //���PIID-ACD
    pRequest->pos=pRequest->userdata_pos+3;

    oad = (OBJECT_ATTR_DESC*)(pRequest->frame+pRequest->pos);
    pRequest->pos+=4;
    obis = cosem_bin2_int16u(oad->value);
    switch(obis)
    {
    case OBIS_DEV_JZQ:
        if(check_safe_config(obis,OP_ACTION,pRequest->security_info)==FALSE)//����Ƿ�ﵽ��ȫ����������𲻵����򷵻ؾܾ���д
        {
            pResp->frame[pResp->pos++]=DAR_READ_WRITE_DENIED;//�ܾ���д
            pResp->frame[pResp->pos++] = DT_NULL;//������������          Data  OPTIONAL
            return 1;
        }
        len = get_object_attribute_item_len(pRequest->frame+pRequest->pos,pRequest->frame[pRequest->pos]);
        dar = process_action_dev_method(oad->attribute_id.attribute_idx,oad->index_id,pRequest->frame+pRequest->pos,len);
        if(dar == DAR_SECCESS)
        {
            (*reset_flag)=1;
        }
        mem_cpy(pResp->frame+pResp->pos,oad->value,4);
        pResp->pos+=4;
        pResp->frame[pResp->pos++]=dar;
		pResp->frame[pResp->pos++]=0;
        return 1;
    case OBIS_BLOCK_TRANSMIT:
        if(check_safe_config(obis,OP_ACTION,pRequest->security_info)==FALSE)//����Ƿ�ﵽ��ȫ����������𲻵����򷵻ؾܾ���д
        {
            pResp->frame[pResp->pos++]=DAR_READ_WRITE_DENIED;//�ܾ���д
            pResp->frame[pResp->pos++] = DT_NULL;//������������          Data  OPTIONAL
            return 1;
        }
        mem_cpy(pResp->frame+pResp->pos,oad->value,4);
        pResp->pos+=4;
        exec_block_transmit_method(oad,pRequest->frame+pRequest->pos,pResp,reset_flag);
        return 1;
    case OBIS_METER_TABLE:
        if(check_safe_config(obis,OP_ACTION,pRequest->security_info)==FALSE)//����Ƿ�ﵽ��ȫ����������𲻵����򷵻ؾܾ���д
        {
            pResp->frame[pResp->pos++]=DAR_READ_WRITE_DENIED;//�ܾ���д
            pResp->frame[pResp->pos++] = DT_NULL;//������������          Data  OPTIONAL
            return 1;
        }
        mem_cpy(pResp->frame+pResp->pos,oad->value,4);
        pResp->pos+=4;
        exec_object_meter_doc_method(oad,pRequest->frame+pRequest->pos,pResp);
        return 1;
    case OBIS_TASK_TABLE:
    case OBIS_NORMAL_TASK_TABLE:
    case OBIS_EVENT_TASK_TABLE:
        if(check_safe_config(obis,OP_ACTION,pRequest->security_info)==FALSE)//����Ƿ�ﵽ��ȫ����������𲻵����򷵻ؾܾ���д
        {
            pResp->frame[pResp->pos++]=DAR_READ_WRITE_DENIED;//�ܾ���д
            pResp->frame[pResp->pos++] = DT_NULL;//������������          Data  OPTIONAL
            return 1;
        }
        mem_cpy(pResp->frame+pResp->pos,oad->value,4);
        pResp->pos+=4;
        exec_object_acq_ctrl_info_method(oad,pRequest->frame+pRequest->pos,pResp);
        return 1;
    #ifdef __ENABLE_ESAM2__
    case OBIS_ESAM:
        if(check_safe_config(obis,OP_ACTION,pRequest->security_info)==FALSE)//����Ƿ�ﵽ��ȫ����������𲻵����򷵻ؾܾ���д
        {
            pResp->frame[pResp->pos++]=DAR_READ_WRITE_DENIED;//�ܾ���д
            pResp->frame[pResp->pos++] = DT_NULL;//������������          Data  OPTIONAL
            return 1;
        }
        mem_cpy(pResp->frame+pResp->pos,oad->value,4);
        pResp->pos+=4;    
        exec_esam_method(oad,pRequest->frame+pRequest->pos,pResp);
        return 1;
    case OBIS_ESAM_PARAM:
        if(check_safe_config(obis,OP_ACTION,pRequest->security_info)==FALSE)//����Ƿ�ﵽ��ȫ����������𲻵����򷵻ؾܾ���д
        {
            pResp->frame[pResp->pos++]=DAR_READ_WRITE_DENIED;//�ܾ���д
            pResp->frame[pResp->pos++] = DT_NULL;//������������          Data  OPTIONAL
            return 1;
        }    
        mem_cpy(pResp->frame+pResp->pos,oad->value,4);
        pResp->pos+=4;
        exec_esam_oi_table_method(oad,pRequest->frame+pRequest->pos,pResp);
        return 1;
    #endif
	//case:
		
		//break;
    default:
        return 0;
    }
}

void process_client_apdu_proxy(objRequest *pRequest,objResponse *pResp)
{
    //���ÿ����룺DIR=1,PRM=0, FUNC=8  ��Ӧ���ģ��û�����
    pResp->frame[OOP_POS_CTRL] = CTRLFUNC_DIR_SET | CTRLFUNC_PRM_SET |  CTRL_FUNC_USERDATA;
    pResp->frame[pResp->pos++] =  SERVER_APDU_PROXY; //���Ӧ���������
    pResp->frame[pResp->pos++] =  pRequest->frame[pRequest->userdata_pos+1];        //���Ӧ��������

    switch(pRequest->frame[pRequest->userdata_pos+1])
    {
    case PROXY_GET_REQUEST_LIST:                            //1)	�����ȡ����������Ķ��������������
        ProxyGetRequestList(pRequest,pResp);
        break;
    case PROXY_GET_REQUEST_RECORD:                         //2)	�����ȡһ����������һ����¼�Ͷ�����������
        ProxyGetRequestRecord(pRequest,pResp);
        break;
    case PROXY_TRANS_COMMAND_REQUEST:                    //7)����͸��ת����������
        ProxyTransCommandRequest(pRequest,pResp);
        break;
    case PROXY_SET_REQUEST_LIST:                           //3)�������ö���������Ķ��������������
    case PROXY_SET_THEN_GET_REQUEST_LIST:                  //4)	�������ú��ȡ����������Ķ��������������
    case PROXY_ACTION_REQUEST_LIST:                       //5)	�����������������Ķ�����󷽷�����
    case PROXY_ACTION_THEN_GET_REQUEST_LIST:              //6)	����������ȡ����������Ķ�����󷽷�����������
    default:
        send_error_response(pRequest,pResp,DAR_OTHER_REASON);
        break;
    }
}

void ProxyGetRequestList(objRequest *pRequest,objResponse *pResp)
{
    INT32U start_time;
    INT16U idx,count,time_out,idx_1;
    INT16U seq_tsa_pos;
    BOOLEAN time_out_flag = FALSE;
    INT8U server_addr_len,oad_count;

    pResp->frame[pResp->pos++] = pRequest->frame[pRequest->userdata_pos+2];     //���PIID-ACD��todo������Ҫ����ACD
    pRequest->pos=pRequest->userdata_pos+3;
    //��ȡִ�з�ʽ����ʱʱ��
    time_out=(pRequest->frame[pRequest->pos]<<8)+pRequest->frame[pRequest->pos+1];
    time_out*=100;
    pRequest->pos+=2;

    count=pRequest->frame[pRequest->pos++];//����
    seq_tsa_pos = pResp->pos;
    pResp->frame[pResp->pos++]=0;
    start_time = os_get_systick_10ms();
    //��������һִ��
    for(idx=0;idx<count;idx++)
    {
        if(!time_out_flag)
        {
            process_get_meter_attribute(pRequest,pResp);
        }
        pResp->frame[seq_tsa_pos]++;
        if((time_out>0) && (time_elapsed_10ms(start_time) >time_out))
        {
            if(!time_out_flag)
            {
                time_out_flag = TRUE;
                continue;
            }
            else
            {
                server_addr_len = pRequest->frame[pRequest->pos];
                server_addr_len++;
                mem_cpy(pResp->frame+pResp->pos,pRequest->frame+pRequest->pos,server_addr_len);
                pResp->pos += (server_addr_len);
                pRequest->pos += (server_addr_len);
                pRequest->pos += 2;
                oad_count = pRequest->frame[pRequest->pos++];
                pResp->frame[pResp->pos++] = oad_count;
                for(idx_1=0;idx_1<oad_count;idx_1++)
                {
                    mem_cpy(pResp->frame+pResp->pos,pRequest->frame+pRequest->pos,4);//����oad
                    pResp->pos += 4;
                    pRequest->pos += 4;
                    pResp->frame[pResp->pos++] = 0;//����
                    pResp->frame[pResp->pos++] = DAR_TIME_OUT;//���ճ�ʱ����
                }
            }
        }
    }
    app_send_response_frame(pRequest,pResp,TRUE);
}

void ProxyGetRequestRecord(objRequest *pRequest,objResponse *pResp)
{
    pResp->frame[pResp->pos++] = pRequest->frame[pRequest->userdata_pos+2];     //���PIID-ACD��todo������Ҫ����ACD
    pRequest->pos=pRequest->userdata_pos+3;
    process_get_meter_record(pRequest,pResp);
    app_send_response_frame(pRequest,pResp,TRUE);
}

void ProxyTransCommandRequest(objRequest *pRequest,objResponse *pResp)
{    
    INT16U idx,time_out;
    OBJECT_ATTR_DESC *oad;
    INT8U *cmd,*resp;
    INT8U head_size;
    INT16U cmd_len,max_resp_len;
    INT32U oad32;
    INT8U meter_no[6]={0};
    BOOLEAN err_flag = TRUE;

	INT8U head_len,resp_len;
    pResp->frame[pResp->pos++] = pRequest->frame[pRequest->userdata_pos+2];     //���PIID-ACD��todo������Ҫ����ACD
    pRequest->pos=pRequest->userdata_pos+3;
    //ת���˿�
    oad=(OBJECT_ATTR_DESC*)(pRequest->frame+pRequest->pos);
    oad32=cosem_bin2_int32u(oad->value);
    pRequest->pos+=sizeof(OBJECT_ATTR_DESC);

    mem_cpy(pResp->frame+pResp->pos,oad->value,4);//Ӧ��oad
    pResp->pos+=4;

    pRequest->pos+=5;
    
    time_out=(pRequest->frame[pRequest->pos]<<8)+pRequest->frame[pRequest->pos+1];//���յȴ����ĳ�ʱʱ�䣨�룩
    time_out*=100;//10ms
    pRequest->pos+=2;
    pRequest->pos+=2;

    cmd_len=get_cosem_data_len(pRequest->frame+pRequest->pos,&head_size);
    cmd=pRequest->frame+pRequest->pos+head_size;

    resp=pResp->frame+pResp->pos+4;
    max_resp_len=pResp->max_reply_pos-pResp->pos-4;
    if(err_flag)
    {
#ifdef __PROVICE_SHANDONG__
	mem_cpy(cmd+OOP_POS_ADDR+1+4,gSystemInfo.meter_no,gSystemInfo.devid_len);
	head_len=3+gSystemInfo.devid_len+2;
	fcs16(cmd+OOP_POS_LEN+4,head_len);
	//����FCSУ��λ
	fcs16(cmd+OOP_POS_LEN+4,cmd_len-4-4);	
#endif
        cmd_len=app_trans_send_meter_frame(cmd,cmd_len,resp,max_resp_len,time_out);
        if(cmd_len>0)
        {
            pResp->frame[pResp->pos++] = 1;//����
            pResp->pos+=set_cosem_data_len(pResp->frame+pResp->pos,cmd_len);
            mem_cpy(pResp->frame+pResp->pos,resp,cmd_len);
            pResp->pos+=cmd_len;
        }
        else
        {
            pResp->frame[pResp->pos++] = 0;//����
            pResp->frame[pResp->pos++] = DAR_OTHER_REASON;//δ֪����
        }
    }
    else
    {
        pResp->frame[pResp->pos++] = 0;
        pResp->frame[pResp->pos++] = DAR_OTHER_REASON;//δ֪����
    }
    app_send_response_frame(pRequest,pResp,TRUE);
}

INT8U check_param_valid(INT8U *buffer,INT16U len,const tagObject_Index *index_list,INT8U count)
{
    INT16U pos=0;
    INT8U idx,col_len;

    if(buffer[pos++]!=DT_STRUCTURE) return 0;
    if(buffer[pos++]!=count)    return 0;
    for(idx=0;idx<count;idx++)
    {
        col_len = get_object_attribute_item_len(buffer+pos,buffer[pos]);
        if((buffer[pos]!=((tagObject_Index*)&index_list[idx])->data_type)&&(buffer[pos]!=0)) return 0;
        if(col_len>((tagObject_Index*)&index_list[idx])->len) return 0;
        pos+=col_len;
    }
    return 1;
}

INT16U get_oad_value(INT16U obis,INT8U attri_id, INT8U index, INT8U *data,tagParamObject *object)
{
	INT32U offset,offset1;
    INT16U len,pos,data_pos=0;
    INT8U idx,count,col_len,fail_count,pos_id;
    tagObject_Attribute attri;
	
    if(obis==OBIS_CUSTOM_INFO)
	{
		pos_id = get_custom_info_id(attri_id);
	}
	else
	{
		pos_id = attri_id;
	}
    if(pos_id>=2)
    {
        offset = get_object_offset(obis);
        if(!get_param_attribute(pos_id-2,object->attri_list,object->attri_list_size,&attri))
        {
            data[data_pos++]=DT_NULL;
            return data_pos;
        }
        offset += get_attribute_offset(object->attri_list,object->attri_list_size,attri.Attri_Id);
        if((obis == OBIS_GPRS1)&&(attri_id==5))
        {
            data_pos = get_dev_ver_info(offset,index,data);
            return data_pos;
        }
        if(attri.index_list == NULL)
        {
            data[data_pos++]=1; //������
            fread_ertu_params(offset,data+data_pos,attri.len);
            if(data[data_pos]==attri.data_type)
            {
                len = get_object_attribute_item_len(data+data_pos,data[data_pos]);
                data_pos+=len;
            }
            else
            {
                data[data_pos++]=DT_NULL;
            }
        }
        else
        {
            count = attri.index_list_size/sizeof(tagObject_Index);
            if(index == 0)
            {
                data[data_pos++]=1; //������
                fail_count = 0;
                pos = data_pos;
                data[data_pos++]=DT_STRUCTURE;
                data[data_pos++]=count;
                for(idx=0;idx<count;idx++)
                {
                    offset1 = offset+get_index_offset(attri.index_list,idx,count);
                    fread_ertu_params(offset1,data+data_pos,((tagObject_Index*)&(attri.index_list)[idx])->len);
                    if(data[data_pos]==((tagObject_Index*)&(attri.index_list)[idx])->data_type)
                    {
                        col_len = get_object_attribute_item_len(data+data_pos,data[data_pos]);
                        data_pos+=col_len;
                    }
                    else
                    {
                        data[data_pos++]=DT_NULL;
                        fail_count++;
                    }
                }
                if(fail_count == count)
                {
                    data_pos = pos;
                    data[data_pos++]=DT_NULL;
                }
            }
            else
            {
                data[data_pos++]=1; //������
                offset1 = offset+get_index_offset(attri.index_list,index-1,count);
                fread_ertu_params(offset1,data+data_pos,((tagObject_Index*)&(attri.index_list)[index-1])->len);
                if(data[data_pos]==((tagObject_Index*)&(attri.index_list)[index-1])->data_type)
                {
                    col_len = get_object_attribute_item_len(data+data_pos,data[data_pos]);
                    data_pos+=col_len;
                }
                else
                {
                    data[data_pos++]=DT_NULL;
                }
            }
        }

    }
    else
    {
        data[data_pos++]=DT_NULL;
    }
	return data_pos;
}

INT16U get_acq_ctrl_info_table(INT16U obis,INT16U idx,INT8U *buffer,INT16U left_len,SEPARATE_FRAME *separate_frame)
{
    INT8U max_count;
    INT8U header[4];
    INT16U max_size;
    INT32U offset;

    switch(obis)
    {
    case OBIS_TASK_TABLE:
        offset=ACQ_TASK_INFO;
        max_count=MAX_TASK_COUNT;
        max_size=MAX_TASK_INFO_SIZE;
        break;
    case OBIS_NORMAL_TASK_TABLE:
        offset=ACQ_NORMAL_PLAN_INFO;
        max_count=MAX_NORMAL_PLAN_COUNT;
        max_size=MAX_NORMAL_PLAN_SIZE;
        break;
    case OBIS_EVENT_TASK_TABLE:
        offset=ACQ_EVENT_PLAN_INFO;
        max_count=MAX_EVENT_PLAN_COUNT;
        max_size=MAX_EVENT_PLAN_SIZE;
        break;
    }
    fread_ertu_params(offset+idx*max_size,header,4);
    if((header[0]!=DT_STRUCTURE)) //||(header[3]!=idx))
    {
        return 0;
    }
    if(max_size+5>left_len)
    {
        separate_frame->more_frame=TRUE;
        separate_frame->frame_id++;
        separate_frame->index_id[1]=idx;
        return 0;
    }
    fread_ertu_params(offset+ idx*max_size,buffer,max_size);

    switch(obis)
    {
    case OBIS_TASK_TABLE:
        if(!check_batch_add_task_format(buffer,1))
        {
            return 0;
        }
        break;
    case OBIS_NORMAL_TASK_TABLE:
        if(!check_batch_add_normal_format(buffer,1))
        {
            return 0;
        }
        break;
    case OBIS_EVENT_TASK_TABLE:
        if(!check_batch_add_event_format(buffer,1))
        {
            return 0;
        }
        break;
    }

    return get_object_attribute_item_len(buffer,buffer[0]);
}

BOOLEAN get_time_period_depend_cur_time(INT8U *begin_time,INT8U *end_time,INT8U *cur_time,INT8U *acq_freq)
{
    INT8U time_begin[6],time_end[6],time_cur[6];
    INT32U value;
    BOOLEAN result=FALSE;

    get_datetime_from_cosem(begin_time,time_begin);
    get_datetime_from_cosem(cur_time,time_cur);

    value=cosem_bin2_int16u(acq_freq+1);
    switch(acq_freq[0])
    {
    case 1:
        time_begin[YEAR]=time_cur[YEAR];
        time_begin[MONTH]=time_cur[MONTH];
        time_begin[DAY]=time_cur[DAY];
        datetime_minus_seconds(&time_begin[YEAR],&time_begin[MONTH],&time_begin[DAY],&time_begin[HOUR],&time_begin[MINUTE],&time_begin[SECOND],3600);
        while(TRUE)
        {
            mem_cpy(time_end,time_begin,6);
            datetime_add_seconds(&time_end[YEAR],&time_end[MONTH],&time_end[DAY],&time_end[HOUR],&time_end[MINUTE],&time_end[SECOND],60*value);
            if((compare_string_reverse(time_cur,time_begin,6)>=0)&&(compare_string_reverse(time_cur,time_end,6)<0))
            {
                result=TRUE;
                break;
            }
            datetime_add_seconds(&time_begin[YEAR],&time_begin[MONTH],&time_begin[DAY],&time_begin[HOUR],&time_begin[MINUTE],&time_begin[SECOND],60*value);
        }
        break;
    case 2:
        time_begin[YEAR]=time_cur[YEAR];
        time_begin[MONTH]=time_cur[MONTH];
        date_minus_days(&time_begin[YEAR],&time_begin[MONTH],&time_begin[DAY],1);
        while(TRUE)
        {
            mem_cpy(time_end,time_begin,6);
            datetime_add_seconds(&time_end[YEAR],&time_end[MONTH],&time_end[DAY],&time_end[HOUR],&time_end[MINUTE],&time_end[SECOND],3600*value);
            if((compare_string_reverse(time_cur,time_begin,6)>=0)&&(compare_string_reverse(time_cur,time_end,6)<0))
            {
                result=TRUE;
                break;
            }
            datetime_add_seconds(&time_begin[YEAR],&time_begin[MONTH],&time_begin[DAY],&time_begin[HOUR],&time_begin[MINUTE],&time_begin[SECOND],3600*value);
        }
        break;
    case 3:
        time_begin[YEAR]=time_cur[YEAR];
        time_begin[MONTH]--;
        if(time_begin[MONTH]==0)
        {
            time_begin[MONTH]=12;
            time_begin[YEAR]--;
        }
        while(TRUE)
        {
            mem_cpy(time_end,time_begin,6);
            date_add_days(&time_end[YEAR],&time_end[MONTH],&time_end[DAY],value);
            if((compare_string_reverse(time_cur,time_begin,6)>=0)&&(compare_string_reverse(time_cur,time_end,6)<0))
            {
                result=TRUE;
                break;
            }
            date_add_days(&time_begin[YEAR],&time_begin[MONTH],&time_begin[DAY],value);
        }
        break;
    case 4:
        while(TRUE)
        {
            mem_cpy(time_end,time_begin,6);
            time_end[MONTH]+=value;
            if(time_end[MONTH]>12)
            {
                time_end[MONTH]-=12;
                time_end[YEAR]++;
            }
            if((compare_string_reverse(time_cur,time_begin,6)>=0)&&(compare_string_reverse(time_cur,time_end,6)<0))
            {
                result=TRUE;
                break;
            }
            time_begin[MONTH]+=value;
            if(time_begin[MONTH]>12)
            {
                time_begin[MONTH]-=12;
                time_begin[YEAR]++;
            }
        }
        break;
    default:
        break;
    }
    if(result)
    {
        set_cosem_datetime_s(begin_time,time_begin);
        set_cosem_datetime_s(end_time,time_end);
    }
    return result;
}


INT16U get_acq_task_monitor_table(INT16U obis,INT16U idx,INT8U *buffer,INT16U left_len,SEPARATE_FRAME *separate_frame)
{
    INT8U max_count,head_size;
    INT8U header[4];
    INT8U *data;
    INT16U max_size,col_len,pos;
    INT32U offset;
    INT8U begin_time[7]={0},end_time[7]={0},cur_time[7]={0},acq_freq[3]={0};


    offset=ACQ_TASK_INFO;
    max_count=MAX_TASK_COUNT;
    max_size=MAX_TASK_INFO_SIZE;

    fread_ertu_params(offset+(idx%max_count)*max_size,header,4);
    if((header[0]!=DT_STRUCTURE)||(header[3]!=idx))
    {
        return 0;
    }
    if(max_size+5>left_len)
    {
        separate_frame->more_frame=TRUE;
        separate_frame->frame_id++;
        separate_frame->index_id[1]=idx;
        return 0;
    }
    fread_ertu_params(offset+(idx%max_count)*max_size,buffer,max_size);
    if(!check_batch_add_task_format(buffer,1))
    {
        return 0;
    }

    data=NULL;
    col_len=get_object_attribute_item_value(buffer,buffer[0],FALSE,1,&data,&head_size);
    if((data!=NULL)&&(col_len==4)&&(data[0]==DT_TI))
    {
        mem_cpy(acq_freq,data+1,3);
    }
    else
    {
        return 0;
    }
    data=NULL;
    col_len=get_object_attribute_item_value(buffer,buffer[0],FALSE,2,&data,&head_size);
    if((data!=NULL)&&(col_len==2)&&(data[0]==DT_ENUM))
    {
        if((data[1]!=1)&&(data[1]!=2))
        {
            return 0;
        }
    }
    else
    {
        return 0;
    }
    data=NULL;
    col_len=get_object_attribute_item_value(buffer,buffer[0],FALSE,4,&data,&head_size);
    if((data!=NULL)&&(col_len==8)&&(data[0]==DT_DATETIME_S))
    {
        mem_cpy(begin_time,data+1,7);
    }
    else
    {
        return 0;
    }
    sys_datetime2cosem_datetime(cur_time);
    if(get_time_period_depend_cur_time(begin_time,end_time,cur_time,acq_freq))
    {
        pos=0;
        buffer[pos++]=DT_STRUCTURE;
        buffer[pos++]=8;
        buffer[pos++]=DT_UNSIGNED;
        buffer[pos++]=idx;
        buffer[pos++]=DT_ENUM;
        buffer[pos++]=1;
        buffer[pos++]=DT_DATETIME_S;
        mem_cpy(buffer+pos,begin_time,7);
        pos+=7;
        buffer[pos++]=DT_DATETIME_S;
        mem_cpy(buffer+pos,end_time,7);
        pos+=7;
        buffer[pos++]=DT_LONGUNSIGNED;
        pos+=cosem_int162_bin(1,buffer+pos);
        buffer[pos++]=DT_LONGUNSIGNED;
        pos+=cosem_int162_bin(1,buffer+pos);

        if(frame_receive>0)
        {
            buffer[pos++]=DT_LONGUNSIGNED;
            pos+=cosem_int162_bin(frame_receive,buffer+pos);
            buffer[pos++]=DT_LONGUNSIGNED;
            pos+=cosem_int162_bin(frame_receive,buffer+pos);
        }
        else
        {
            buffer[pos++]=DT_LONGUNSIGNED;
            pos+=cosem_int162_bin(1,buffer+pos);
            buffer[pos++]=DT_LONGUNSIGNED;
            pos+=cosem_int162_bin(1,buffer+pos);
        }
    }
    else
    {
        return 0;
    }

    return get_object_attribute_item_len(buffer,buffer[0]);
}


INT16U query_esam_oi_list(INT8U *resp,INT16U left_len,SEPARATE_FRAME* separate_frame)
{
    INT16U idx,pos,count;

    pos=0;
    count=0;

    resp[pos++]=DT_ARRAY;
    resp[pos++]=0;
    //tpos_enterCriticalSection();
    for(idx=separate_frame->index_id[1];idx<g_esam_oi_list.count;idx++)
    {
        if ((pos + 10) >= left_len)
        {
            //�Ų����ˣ�Ҫ��֡
            resp[1] = count;
            separate_frame->more_frame = 1;
            separate_frame->index_id[1] = idx;
            //tpos_leaveCriticalSection();
            return pos;
        }
        if(g_esam_oi_list.object_list[idx][4] & EXPLICIT_CONFIG_WORD)//����ǵ�������ֱ�����
        {
            resp[pos++]=DT_STRUCTURE;
            resp[pos++]=2;
            resp[pos++]=DT_OI;
            resp[pos++]=g_esam_oi_list.object_list[idx][0];
            resp[pos++]=g_esam_oi_list.object_list[idx][1];
            resp[pos++]=DT_LONGUNSIGNED;
            resp[pos++]=g_esam_oi_list.object_list[idx][2];
            resp[pos++]=g_esam_oi_list.object_list[idx][3];
            count++;
        }
    }
    //tpos_leaveCriticalSection();
    separate_frame->more_frame = 0;
    if(count > 0)
    {
        resp[1] = count;
    }
    else
    {
        resp[1] = 0;
        pos = 2;
    }
    return pos;
}

INT16U get_custom_oad_value(INT16U obis,INT8U attri_id, INT8U index, INT8U *security_info,INT8U *data,INT16U left_len, SEPARATE_FRAME *separate_frame)
{
    INT8U err_flag,count;
	INT16U pos,cnt_pos,idx,len;
	INT32U offset;
    tagObject_Attribute attri;
    tagParamObject object;
	INT32U  tmp;
	pos=0;
	switch(obis)
	{
    case OBIS_DEV_JZQ://�ն�
        if(check_safe_config(obis,OP_GET,security_info)==FALSE)//����Ƿ�ﵽ��ȫ����������𲻵����򷵻ؾܾ���д
        {
            data[pos++]=0;//������дΪ0
            data[pos++]=DAR_READ_WRITE_DENIED;//�ܾ���д
            return pos;
        }
        if(attri_id==3)
        {
            if(get_param_object(OBIS_GPRS1,&object))
            {
                offset = get_object_offset(OBIS_GPRS1);
                if(!get_param_attribute(3,object.attri_list,object.attri_list_size,&attri))
                {
                    data[pos++]=DT_NULL;
                    return pos;
                }
                offset += get_attribute_offset(object.attri_list,object.attri_list_size,attri.Attri_Id);
                pos = get_dev_ver_info(offset,index,data);
            }
            else
            {
                data[pos++]= 0;//������
                data[pos++]= 0xFF;
            }
        }
        else
        {
            data[pos++]= 0;//������
            data[pos++]= 0xFF;
        }
        break;
    case OBIS_METER_TABLE:
        if(check_safe_config(obis,OP_GET,security_info)==FALSE)//����Ƿ�ﵽ��ȫ����������𲻵����򷵻ؾܾ���д
        {
            data[pos++]=0;//������дΪ0
            data[pos++]=DAR_READ_WRITE_DENIED;//�ܾ���д
            return pos;
        }
        if((attri_id==2)&&(index==0))
        {
            data[pos++]=1;
            data[pos++]=DT_ARRAY;
            cnt_pos=pos;
            pos++;
            fread_ertu_params(ACQ_METER_DOC_INFO,data+pos,MAX_METER_DOC_SIZE);
            if(check_meter_doc_param(data+pos,0,&err_flag))
            {
                data[cnt_pos]=1;
                pos+=get_object_attribute_item_len(data+pos,data[pos]);
            }
            else
            {
                data[cnt_pos]=0;
            }
        }
        else
        {
            data[pos++]= 0;//������
            data[pos++]= 0xFF;
        }
        break;
    case OBIS_TASK_TABLE:
    case OBIS_NORMAL_TASK_TABLE:
    case OBIS_EVENT_TASK_TABLE:
        if(check_safe_config(obis,OP_GET,security_info)==FALSE)//����Ƿ�ﵽ��ȫ����������𲻵����򷵻ؾܾ���д
        {
            data[pos++]=0;//������дΪ0
            data[pos++]=DAR_READ_WRITE_DENIED;//�ܾ���д
            return pos;
        }
        if((attri_id==2)&&(index==0))
        {
            data[pos++]=1;
            data[pos++]=DT_ARRAY;
            cnt_pos=pos;
            data[cnt_pos]=0;
            pos++;
            count=0;
            for(idx=separate_frame->index_id[1];idx<20;idx++)
            {
                len=get_acq_ctrl_info_table(obis,idx,data+pos,left_len,separate_frame);
                if(separate_frame->more_frame)
                {
                    break;
                }
                if(len>0)
                {
                    count++;
                    pos+=len;
                    left_len-=len;
                }
            }
            data[cnt_pos]=count;
        }
        else
        {
            data[pos++]=0;//������дΪ0
            data[pos++]=DAR_READ_WRITE_DENIED;//�ܾ���д
            return pos;
        }
        break;
    case OBIS_ACQ_MONITOR_TABLE:
        if(check_safe_config(obis,OP_GET,security_info)==FALSE)//����Ƿ�ﵽ��ȫ����������𲻵����򷵻ؾܾ���д
        {
            data[pos++]=0;//������дΪ0
            data[pos++]=DAR_READ_WRITE_DENIED;//�ܾ���д
            return pos;
        }
        if((attri_id==2)&&(index==0))
        {
            data[pos++]=1;
            data[pos++]=DT_ARRAY;
            cnt_pos=pos;
            data[cnt_pos]=0;
            pos++;
            count=0;
            for(idx=separate_frame->index_id[1];idx<256;idx++)
            {
                len=get_acq_task_monitor_table(obis,idx,data+pos,left_len,separate_frame);
                if(separate_frame->more_frame)
                {
                    break;
                }
                if(len>0)
                {
                    count++;
                    pos+=len;
                    left_len-=len;
                }
            }
            data[cnt_pos]=count;
        }
        else
        {
            data[pos++]=0;//������дΪ0
            data[pos++]=DAR_READ_WRITE_DENIED;//�ܾ���д
            return pos;
        }
        break;
    #if defined __ENABLE_ESAM2__
    case OBIS_ESAM_PARAM:
        if(check_safe_config(obis,OP_GET,security_info)==FALSE)//����Ƿ�ﵽ��ȫ����������𲻵����򷵻ؾܾ���д
        {
            data[pos++]=0;//������дΪ0
            data[pos++]=DAR_READ_WRITE_DENIED;//�ܾ���д
            return pos;
        }
        if(attri_id==3)   //������2��
        {
            if(index==0)
            {
                pos+=query_esam_oi_list(data,left_len,separate_frame);
            }
            else
            {
                data[pos++]= 0;//������
                data[pos++]= 0xFF;
            }
        }
		if(attri_id == 2)
		{
			    data[pos++]= 0x01;//������
			    data[pos++]= 0x16;
				data[pos++]= 0x00;
		}
        break;
    case OBIS_ESAM:
        if(check_safe_config(obis,OP_GET,security_info)==FALSE)//����Ƿ�ﵽ��ȫ����������𲻵����򷵻ؾܾ���д
        {
            data[pos++]=0;//������дΪ0
            data[pos++]=DAR_READ_WRITE_DENIED;//�ܾ���д
            return pos;
        }
        pos+=req_object_attribute_esam(attri_id,index,data,left_len);
        break;
    #endif
	case OBIS_NETFLOW:
				data[pos++]= 0x01;//������
			    data[pos++]= 0x02;
				data[pos++]= 0x02;
				data[pos++]= 0x06;
				tmp = gSystemInfo.day_bytes;
				mem_cpy_reverse(data+pos,&tmp,4);
				pos+=4;
				data[pos++]= 0x06;
				tmp = gSystemInfo.month_bytes;
				mem_cpy_reverse(data+pos,&tmp,4);
				pos+=4;
		break;
   	}
	return pos;

}
/*
* @brief �γɶ���״̬
*/
void form_hold_state()
{
	INT8U idx=0;
	INT8U buffer[420];
	INT16U len;
	INT32U pos = 0;
	INT16U  size;
	INT8U csdIndex=0;
	volatile INT16U csdPos = 0; //��������RCD��λ�á�
	volatile INT8U  dayHoldFlag =0;
	volatile INT16U dayHoldtmp;
	SEPARATE_FRAME *separate_frame=NULL;
	separate_frame=&separate_frame_mem;
	for(idx =0;idx <10;idx ++)
	{
		pos =0;
		mem_set(buffer,420,0x00);
		len=get_acq_ctrl_info_table(OBIS_NORMAL_TASK_TABLE,idx,buffer,420,separate_frame);
		if(len!=0)  //�з���ֵ
		{	
			if((buffer[pos] == 2) && (buffer[pos+1] =0x6));
			{
				pos =2;
				while(buffer[1]>0) //�����еĶ���һ��
				{
					if(buffer[1]==3) //�ж϶������ͣ�Сʱ���ᣬ�ն��ᣬ�¶���
					{
						csdPos = 3;
						for(csdIndex = 0; csdIndex<buffer[pos+1];csdIndex++)
						{
							dayHoldtmp =(buffer[pos+csdPos+1]<<8)+buffer[pos+csdPos+2];
							switch(dayHoldtmp)
							{
								case 0x5003:
									break;
								case 0x5004: //�ն���
									 	dayHoldFlag =1;
									break;
								case 0x5006:
									break;
							}
							csdPos += 4;
							csdPos += 1;
							csdPos +=4*buffer[pos+csdPos];
							csdPos +=2;
						}
					
					}

					size = get_object_attribute_item_len(buffer+pos,buffer[pos]);
					pos += size;
					buffer[1]--;
					if((dayHoldFlag)&&(buffer[pos]==0x16)) //�ж��ն���ģʽ
					{
						gSystemInfo.dayHodeMode = buffer[pos+1];
						dayHoldFlag =0;
					}
					
				}
			}

		}

	}

}
INT8U process_get_object_attribute(objRequest *pRequest,objResponse *pResp,SEPARATE_FRAME *separate_frame)
{
    INT16U obis,pos,left_len;
    OBJECT_ATTR_DESC *oad;
    tagParamObject object;

    oad = (OBJECT_ATTR_DESC*)(pRequest->frame+pRequest->pos);
    mem_cpy(pResp->frame+pResp->pos, pRequest->frame+pRequest->pos,4); //���Ӧ��OAD
    pRequest->pos+=4;
    pResp->pos+=4;
    left_len=pResp->max_reply_pos-pResp->pos;

    obis=(oad->object_id[0]<<8)+oad->object_id[1];
	pos=get_custom_oad_value(obis,oad->attribute_id.attribute_idx,oad->index_id,pRequest->security_info,pResp->frame+pResp->pos,left_len,separate_frame);
	if(pos>0)
	{
		pResp->pos +=pos;
		return 1;
	}
	else if(get_param_object(obis,&object))
    {
        if(check_safe_config(obis,OP_GET,pRequest->security_info)==FALSE)//����Ƿ�ﵽ��ȫ����������𲻵����򷵻ؾܾ���д
        {
            pResp->frame[pResp->pos++]=0;//������дΪ0
            pResp->frame[pResp->pos++]=DAR_READ_WRITE_DENIED;//�ܾ���д
        }
        else
        {
            if(oad->attribute_id.attribute_idx>=2)
            {
            pResp->pos += get_oad_value(obis,oad->attribute_id.attribute_idx,oad->index_id,pResp->frame+pResp->pos,&object);
        }
            else
            {
                pResp->frame[pResp->pos++]=0;//������дΪ0
                pResp->frame[pResp->pos++]=DAR_OTHER_REASON;//�ܾ���д
            }
        }
        return 1;
    }
    return 0;
}


INT8U process_req_record_object_attribute(objRequest *pRequest,objResponse *pResp,INT16U left_len,SEPARATE_FRAME *separate_frame)
{
    OBJECT_ATTR_DESC *oad;
    RSD *rsd;
    INT8U *rcsd,*count_pos;
    INT16U obis,max_len,len,node_count,meter_count;
	volatile INT8U  idx,idx2;
	volatile INT16U  oad2;
	volatile INT64U  startTime,endTime,eventTime;
	tagDatetime dt;
	INT8U pos;
	INT32U eventNumPos ;
    oad = (OBJECT_ATTR_DESC*)(pRequest->frame+pRequest->pos);
    mem_cpy(pResp->frame+pResp->pos,oad,4);
    pRequest->pos+=4;
    pResp->pos+=4;
    left_len-=4;
    rsd = (RSD*)(pRequest->frame+pRequest->pos);
    len=get_oop_data_type_len(rsd->value,DT_RSD);//��ȡRSD
    pRequest->pos+=len;
    rcsd=rsd->value+len;
    len=get_oop_data_type_len(rcsd,DT_RCSD);//��ȡRCSD
    mem_cpy(pResp->frame+pResp->pos,rcsd,len);
    pResp->pos+=len;
    pRequest->pos+=len;
    left_len-=len;

    obis=(oad->object_id[0]<<8)+oad->object_id[1];
    if(obis==OBIS_TASK_TABLE)
    {
        if(oad->attribute_id.value==3)
        {
            pResp->frame[pResp->pos++]=1;        //01����������
            count_pos=pResp->frame+pResp->pos;   //count_pos ��ʾ�ûظ�֡�ж�������¼��
            pResp->pos++;

	       //*count_pos=read_acq_plan_data_from_meter(pRequest,pResp,rsd,rcsd,left_len,separate_frame);

			*count_pos=read_acq_plan_data_from_flash(pRequest,pResp,rsd,rcsd,left_len,separate_frame);
	  
        }
        else
        {
            pResp->frame[pResp->pos++]=0;
            pResp->frame[pResp->pos++]=DAR_OTHER_REASON;
        }
        return 1;
    }
	else if(obis == OBIS_EVT_POWER_ONOFF)
	{
		pResp->frame[pResp->pos++] =0x01;
		eventNumPos = pResp->pos;
		pResp->frame[eventNumPos] =0;
		pResp->pos++;
		//pResp->frame[pResp->eventNumPos] =0x02;
		fread_ertu_params(TERMINAL_POWER_ONOFF,&TerminalPowerOnOffEvent,sizeof(StrTerminalPowerOnOffEventBlock));
		//����õ�ʱ�䷢����ʱ������
		if(rsd->choice ==2)
		{
			pos=1;
			dt.year = 19;
			pos+=2;
			dt.month = (rsd->choice_1_2_3.value[pos++] );
			dt.day = (rsd->choice_1_2_3.value[pos++] );
			dt.hour = (rsd->choice_1_2_3.value[pos++] );
			dt.minute = (rsd->choice_1_2_3.value[pos++] );
			dt.second = (rsd->choice_1_2_3.value[pos++] );
			startTime = getPassedSeconds(&dt,2000);
			pos +=3;
			dt.month = (rsd->choice_1_2_3.value[pos++] );
			dt.day = (rsd->choice_1_2_3.value[pos++] );
			dt.hour = (rsd->choice_1_2_3.value[pos++] );
			dt.minute = (rsd->choice_1_2_3.value[pos++] );
			dt.second = (rsd->choice_1_2_3.value[pos++] );			
			endTime = getPassedSeconds(&dt,2000);
		}
		for(idx2 =0;idx2<15;idx2++)
		{
			// ����flash���¼���ʱ��
			pos=1;
			dt.year = 19;
			pos+=2;
			dt.month = TerminalPowerOnOffEvent.event[idx2].beginTime[pos++];
			dt.day = TerminalPowerOnOffEvent.event[idx2].beginTime[pos++];
			dt.hour = TerminalPowerOnOffEvent.event[idx2].beginTime[pos++];
			dt.minute = TerminalPowerOnOffEvent.event[idx2].beginTime[pos++];
			dt.second = TerminalPowerOnOffEvent.event[idx2].beginTime[pos++];
			eventTime = getPassedSeconds(&dt,2000);
			if((eventTime>startTime)&&(eventTime<endTime))
			{//��ֵ��������
				(pResp->frame[eventNumPos])++;
				for(idx =0;idx <rcsd[0];idx ++)
				{
					//mem_cpy(oad2,rcsd+idx*5+2,2);
					oad2 = (*(rcsd+idx*5+2)<<8) +*(rcsd+idx*5+2+1);
					switch(oad2)
					{
						case 0x2022:
							mem_cpy(pResp->frame+pResp->pos,TerminalPowerOnOffEvent.event[idx2].eventIndex,5);
							//pResp->frame[pResp->pos++] =0x06;
							//mem_set(pResp->frame+pResp->pos,0x00,3);
							//pResp->pos+=3;
							//pResp->frame[pResp->pos++] =0x01;
							pResp->pos+=5;
						break;
						case 0x201E:
							mem_cpy(pResp->frame+pResp->pos,TerminalPowerOnOffEvent.event[idx2].beginTime,8);
							//pResp->frame[pResp->pos++] =0x1c;
							//mem_set(pResp->frame+pResp->pos,7,0x00);
							pResp->pos +=8;
						break;
						case 0x2020:
							if(TerminalPowerOnOffEvent.event[idx2].endTime[0] == 0x1C)
							{
								mem_cpy(pResp->frame+pResp->pos,TerminalPowerOnOffEvent.event[idx2].endTime,8);
								//pResp->frame[pResp->pos++] =0x1c;
								//mem_set(pResp->frame+pResp->pos,7,0x00);
								pResp->pos +=8;
							}
							else
							{
								*(pResp->frame+pResp->pos) =0;
								pResp->pos++;
							}
						break;
						case 0x3300:
					
						break;
						case 0x2024:
							mem_cpy(pResp->frame+pResp->pos,TerminalPowerOnOffEvent.event[idx2].eventSource,2);
							//pResp->frame[pResp->pos++] =0x16;
							//pResp->frame[pResp->pos++] =0x0;
							pResp->pos +=2;
						break;
						case 0x3309:
							mem_cpy(pResp->frame+pResp->pos,TerminalPowerOnOffEvent.event[idx2].eventAttribute,3);
							//pResp->frame[pResp->pos++] =0x04;
							//pResp->frame[pResp->pos++] =0x08;
							//pResp->frame[pResp->pos++] =0xC0;
							pResp->pos +=3;
						break;
					}
				}

			}
		}
		
	}
    else
    {
        return 0;
    }

}

INT8U get_custom_info_id(INT8U attri_id)
{
	INT8U id;
	
	switch(attri_id)
	{
		case 2:
			id = 2;			
			break;
		case 16:
			id = 3;
			break;
		default :
			id = 0;
	}
	return id;
} 

INT8U set_oad_value(INT16U obis, INT8U attri_id, INT8U index_id, INT8U *data, tagParamObject *object, INT8U *buffer)
{
    INT32U offset,offset1;
    INT16U len,pos;
    INT8U idx,count,col_len,dar,pos_id;
    tagObject_Attribute attri;
    tagObject_Index *index;
	    
	if(obis==OBIS_CUSTOM_INFO)
	{
		pos_id = get_custom_info_id(attri_id);
	}
	else
	{
		pos_id = attri_id;
	}
	
	if((pos_id>=2))
	{
		len = get_object_attribute_item_len(data,data[0]);
		offset = get_object_offset(obis);
        if(!get_param_attribute(pos_id-2,object->attri_list,object->attri_list_size,&attri))
        {
            return DAR_OTHER_REASON;
        }
        offset += get_attribute_offset(object->attri_list,object->attri_list_size,attri.Attri_Id);
        if(attri.right&READ_ONLY)
        {
            dar = DAR_READ_WRITE_DENIED;
            return dar;
        }
		if(attri.index_list == NULL)
		{
			if((len<=attri.len)&&((data[0]==attri.data_type)||(data[0]==DT_NULL)))
			{
				mem_set(buffer,attri.len,0xFF);
				mem_cpy(buffer,data,len);
				fwrite_ertu_params(offset,buffer,attri.len);
				dar=DAR_SECCESS;
			}
			else
			{
				dar=DAR_OTHER_REASON;
			}
		}
		else
		{
			count = (attri.index_list_size/sizeof(tagObject_Index));
			if(index_id == 0)
			{
				if(check_param_valid(data,len,attri.index_list,count))
				{
					pos=2;
					for(idx=0;idx<count;idx++)
					{
						col_len = get_object_attribute_item_len(data+pos,data[pos]);
						mem_set(buffer,col_len,0xFF);
						mem_cpy(buffer,data+pos,col_len);
						offset1 = offset+get_index_offset(attri.index_list,idx,count);
						fwrite_ertu_params(offset1,buffer,col_len);
						pos+=col_len;
					}
					dar=DAR_SECCESS;
				}
				else
				{
					dar=DAR_OTHER_REASON;
				}
			}
			else
			{
                if(index_id>count)
                {
                    return DAR_OTHER_REASON;
                }
				index = (tagObject_Index*)&(attri.index_list)[index_id-1];
				if((data[0]==(index->data_type)||(data[0]==DT_NULL))&&(len<=index->len))
				{
					mem_set(buffer,index->len,0xFF);
					mem_cpy(buffer,data,len);
					offset1 = offset+get_index_offset(attri.index_list,index_id-1,count);
					fwrite_ertu_params(offset1,buffer,index->len);
					dar=DAR_SECCESS;
				}
				else
				{
					dar=DAR_OTHER_REASON;
				}
			}
		}

	}
	else
	{
		dar=DAR_OTHER_REASON;
	}
	return dar;	
}

INT8U process_set_object_attribute(objRequest *pRequest,objResponse *pResp)
{
	INT16U obis;
    OBJECT_ATTR_DESC *oad;
	INT8U *buffer;
	INT8U dar;
    tagParamObject object;

    buffer = pResp->frame+SIZE_OF_APP_PROTOCOL_FRAME-300;
    oad = (OBJECT_ATTR_DESC*)(pRequest->frame+pRequest->pos);   
    mem_cpy(pResp->frame+pResp->pos, pRequest->frame+pRequest->pos,4); //���Ӧ��OAD
    pResp->pos+=4;
	pRequest->pos+=4;
	
	obis=(oad->object_id[0]<<8)+oad->object_id[1];
    if((check_safe_config(obis,OP_SET,pRequest->security_info)==FALSE)&&(obis!=OBIS_ESAM_PARAM))//����Ƿ�ﵽ��ȫ����������𲻵����򷵻ؾܾ���д
    {
        pResp->frame[pResp->pos++]=DAR_READ_WRITE_DENIED;//�ܾ���д
        return 1;
    }
	if(get_param_object(obis,&object))
	{	
		dar = set_oad_value(obis,oad->attribute_id.attribute_idx,oad->index_id,pRequest->frame+pRequest->pos,&object,buffer);
        if(dar == DAR_SECCESS)
        {
            if(obis==OBIS_CUSTOM_INFO)
            {
                load_ertu_param(obis,oad->attribute_id.attribute_idx,oad->index_id);
            }
            if(obis==OBIS_ESAM_PARAM)
            {
                if(oad->attribute_id.attribute_idx==2)
                {
                    if(pRequest->frame[pRequest->pos+1]==1)
                    {
                        set_system_flag(SYS_ESAM,SYS_FLAG_BASE);     //ʹ��ESAM��MACУ��
                        set_system_flag(SYS_ESAM_READMETER,SYS_FLAG_BASE);//ʹ��ESAM����
                    }
                    else
                    {
                        clr_system_flag(SYS_ESAM,SYS_FLAG_BASE);     //ʹ��ESAM��MACУ��
                        clr_system_flag(SYS_ESAM_READMETER,SYS_FLAG_BASE);//��ʹ��ESAM����
                    }
                }
                if(oad->attribute_id.attribute_idx==9)
                {
                    if(pRequest->frame[pRequest->pos+1]==1)
                    {
                        set_system_flag(SYS_ESAM_READMETER,SYS_FLAG_BASE);     //ʹ��ESAM����
                    }
                    else
                    {
                        clr_system_flag(SYS_ESAM_READMETER,SYS_FLAG_BASE);     //��ʹ��ESAM����
                    }
                }
            }
        }
		pResp->frame[pResp->pos++]=dar;
		return 1;
	}
	return 0;
}
void process_trans_frame_2_meter(objRequest *pRequest,objResponse *pResp)
{
    INT16U cmd_len=0;

    cmd_len = app_trans_send_meter_frame(pRequest->frame,pRequest->frame_len,pResp->frame,1200,6000);
    if(cmd_len>0)
    {
        pResp->frame_len = cmd_len;
        pResp->pos = cmd_len-1;
        app_send_ReplyFrame(pResp);
    }
    else
    {
        send_error_response(pRequest,pResp,DAR_TIME_OUT);
    }
}

BOOLEAN replace_oop_frame_addr(INT8U *frame,INT16U frame_len,INT8U *addr,INT8U *addr_len)
{
    INT16U f_len;
    INT8U len;
    
    frame=trim_oopframe_FE(frame);
    len = get_oopframe_addr_length(frame);
    *addr_len = ((frame[OOP_POS_ADDR] & 0x0F)+1);
    if(*addr_len != 6)  return FALSE;//������ɵ�ַ���Ȳ���6���ֽڣ���ִ���滻
    mem_cpy(addr,frame+OOP_POS_ADDR+1,*addr_len);
    if(check_is_all_ch(frame+5,6,0xAA)) return TRUE;
    //mem_cpy_reverse(frame+5,gSystemInfo.meter_no,*addr_len);//����Ϊ6���ֽ�
	mem_cpy(frame+5,gSystemInfo.meter_no,*addr_len);//����Ϊ6���ֽ�,���Է��ֲ��÷���
    fcs16(frame+OOP_POS_LEN,len+3);
    f_len = frame[OOP_POS_LEN] + ((frame[OOP_POS_LEN+1] )<<8);
    fcs16(frame+OOP_POS_LEN,f_len-2);
    return TRUE;
    
}

void restore_oop_addr(INT8U *frame,INT16U *frame_len,INT8U *addr,INT8U addr_len)
{
    INT16U f_len;
    INT8U len;
    
    frame=trim_oopframe_FE(frame);
    len = get_oopframe_addr_length(frame);
    if(((frame[OOP_POS_ADDR] & 0x0F)+1) != addr_len)
    {
        *frame_len = 0;
        return;
    }
    mem_cpy(frame+OOP_POS_ADDR+1,addr,addr_len);
    fcs16(frame+OOP_POS_LEN,len+3);
    f_len = frame[OOP_POS_LEN] + ((frame[OOP_POS_LEN+1] )<<8);
    fcs16(frame+OOP_POS_LEN,f_len-2);
}

void process_trans_frame_2_meter_ext(objRequest *pRequest,objResponse *pResp)
{
    #ifdef __PROVICE_SHANDONG__
    INT8U addr[10]={0};
    INT8U addr_len;
    
    if(!replace_oop_frame_addr(pRequest->frame,pRequest->frame_len,addr,&addr_len))
    {
        return;        
    }
    #endif
    process_trans_frame_2_meter(pRequest,pResp);
    
    #ifdef __PROVICE_SHANDONG__
    restore_oop_addr(pResp->frame,&(pResp->frame_len),addr,addr_len);
    #endif
}    


void load_ertu_param(INT16U obis, INT8U attri_id,INT8U index)
{
    INT8U paramcode;
    INT8U data[100];
    INT16U frame_ctrl,len;
    tagParamObject object;

    mem_set(data,100,0x00);
    if((!get_param_object(obis,&object))||(attri_id<2))
    {
        return;
    }
    len = get_oad_value(obis,attri_id,index,data,&object);
    if(len>1)
    {
        mem_cpy(data,data+1,len);
    }
    else
    {
        return;
    }
    if(obis == OBIS_CUSTOM_INFO)
    {        
        paramcode = get_custom_info_id(attri_id);
        switch(paramcode-2)
        {
            case OA_CFG_VENDO://���̴���
                break;
            case OA_FRAME_TYPE://��������
                if((len>0)&&(data[0]==DT_OCTETSTRING)&&(data[1]==2))
                {
                    frame_ctrl = bin2_int16u(data+2);
                    if(frame_ctrl&0x0001)
                    {
                        gSystemInfo.gprs_debug_flag = 1;  //��GPRS
                    }
                    else
                    {
                        gSystemInfo.gprs_debug_flag = 0;
                    }
                    if(frame_ctrl&0x0002)
                    {
                        gSystemInfo.meter_debug_flag = 1;   //�򿪵�������Ϣ
                    }
                    else
                    {
                        gSystemInfo.meter_debug_flag = 0;
                    }
                    if(frame_ctrl&0x0004)
                    {
                        //gSystemInfo.remote_app_debug_flag = 1;
                    }
                    else
                    {
                        gSystemInfo.remote_app_debug_flag = 0;
                    }
                    if(frame_ctrl&0x0008)
                    {
                        //gSystemInfo.debug_info_interface = 1;
                    }
                    else
                    {
                        gSystemInfo.debug_info_interface = 0;
                    }
                    if(frame_ctrl&0x0010)
                    {
                        gSystemInfo.clock_ready = 1;
                    }
                    else
                    {
                        gSystemInfo.clock_ready = 0;
                    }                
                }
                break;
        }
    }
}

INT8U process_action_dev_method(INT8U m_id,INT8U index,INT8U *data,INT16U len)
{
    INT8U dar;
    INT8U idx;
	uint32_t offset;
	INT16U max_size;
	INT8U max_count;
	INT8U tmp[MAX_NORMAL_PLAN_SIZE];
    dar = DAR_OTHER_REASON;
    switch(m_id)
    {
        case 1://����
            if((index == 0)&&(data[0]==0)&&(len == 1))
            {
                dar=DAR_SECCESS;
            }
            break;
        case 3://���ݳ�ʼ��
            if((index == 0)&&(data[0]==0)&&(len == 1))
            {
                dar=DAR_SECCESS;
            }
			delete_acq_data(FLASH_SECTOR_METER_DATA_START,FLASH_SECTOR_METER_DATA_START+MAX_ONE_METER_DATA_PAGE_COUNT);
			mem_set(&TerminalPowerOnOffEvent,sizeof(StrTerminalPowerOnOffEventBlock),0xFF);
			fwrite_ertu_params(TERMINAL_POWER_ONOFF,&TerminalPowerOnOffEvent,sizeof(StrTerminalPowerOnOffEventBlock));
            break;
        case 4://������ʼ��
            //if((index == 0)&&(data[0]==0)&&(len == 1))
			//if((index == 0)&&(data[0]==0))
            {
				//������ʼ����
                dar=DAR_SECCESS;
            }
			for(idx=0;idx<MAX_TASK_COUNT;idx++)
			{
				offset=ACQ_NORMAL_PLAN_INFO;
				max_size=MAX_NORMAL_PLAN_SIZE;
				max_count=MAX_NORMAL_PLAN_COUNT;
				fread_ertu_params(offset+(idx%max_count)*max_size,tmp,max_size);
				mem_set(tmp,MAX_NORMAL_PLAN_SIZE,0xFF);
				fwrite_ertu_params(offset+(idx%max_count)*max_size,tmp,max_size);
				
				offset=ACQ_EVENT_PLAN_INFO;
				max_size=MAX_EVENT_PLAN_SIZE;
				max_count=MAX_EVENT_PLAN_COUNT;
				fread_ertu_params(offset+(idx%max_count)*max_size,tmp,max_size);
				mem_set(tmp,MAX_EVENT_PLAN_SIZE,0xFF);
				fwrite_ertu_params(offset+(idx%max_count)*max_size,tmp,max_size);
			}
            break;
    }
    return dar;   
}

INT16U get_dev_ver_info(INT32U offset,INT8U index,INT8U *data)
{
    INT16U pos=0;
    INT8U temp[10],idx,count,index_temp;
    INT8U *ver_info;
    INT8U *dev_ver_info;    

    dev_ver_info = (INT8U*)dev_ver_info_default;

    mem_set(temp,10,0x00);
    data[pos++]=1; //������  
    ver_info=data+1+sizeof(dev_ver_info_default);
    mem_cpy(ver_info,(void*)dev_ver_info,sizeof(dev_ver_info_default));
    
    if(index == 0)
    {
        data[pos++] = DT_STRUCTURE;
        data[pos++] = 6;
        count = 6;
    }
    else
    {
        count = 1;
    }
    for(idx=0;idx<count;idx++)
    {
        if(index==0)
        {
            index_temp = idx+1;
        }
        else
        {
            index_temp = index;
        }
        switch(index_temp)
        {
            case 1:           
                fread_ertu_params(offset+DEV_VER_VENDOR_NO_POS,temp,6);
                if((temp[0]!=DT_VISIBLESTRING)||(temp[1]!=4)) 
                {
                    mem_cpy(data+pos,ver_info+DEV_VER_VENDOR_NO_POS,6);
                }
                else
                {
                    mem_cpy(data+pos,temp,6);
                }
                pos +=6;
                break;
            case 2:
                fread_ertu_params(offset+DEV_VER_SOFT_VER_POS,temp,6);
                if((temp[0]!=DT_VISIBLESTRING)||(temp[1]!=4)) 
                {
                    mem_cpy(data+pos,ver_info+DEV_VER_SOFT_VER_POS,6);
                }
                else
                {
                    mem_cpy(data+pos,temp,6);
                }
                pos +=6;            
                break;
            case 3:
                fread_ertu_params(offset+DEV_VER_SOFT_DATE_POS,temp,8);
                if((temp[0]!=DT_VISIBLESTRING)||(temp[1]!=6)) 
                {
                    mem_cpy(data+pos,ver_info+DEV_VER_SOFT_DATE_POS,8);
                }
                else
                {
                    mem_cpy(data+pos,temp,8);
                }
                pos +=8;
                break;
            case 4:
                fread_ertu_params(offset+DEV_VER_HARDWARE_VER_POS,temp,6);
                if((temp[0]!=DT_VISIBLESTRING)||(temp[1]!=4)) 
                {
                    mem_cpy(data+pos,ver_info+DEV_VER_HARDWARE_VER_POS,6);
                }
                else
                {
                    mem_cpy(data+pos,temp,6);
                }
                pos +=6;
                break;
            case 5:
                fread_ertu_params(offset+DEV_VER_HARDWARE_DATE_POS,temp,8);
                if((temp[0]!=DT_VISIBLESTRING)||(temp[1]!=6)) 
                {
                    mem_cpy(data+pos,ver_info+DEV_VER_HARDWARE_DATE_POS,8);
                }
                else
                {
                    mem_cpy(data+pos,temp,8);
                }
                pos +=8;            
                break;
            case 6:
                fread_ertu_params(offset+DEV_VER_VENDOR_CUSTOM_POS,temp,10);
                if((temp[0]!=DT_VISIBLESTRING)||(temp[1]!=8)) 
                {
                    mem_cpy(data+pos,ver_info+DEV_VER_VENDOR_CUSTOM_POS,10);
                }
                else
                {
                    mem_cpy(data+pos,temp,10);
                }
                pos +=10;            
                break;               
        }
    }
    return pos;
}

INT8U transmit_info_check(INT8U* param,objResponse *pResp)
{
    INT8U idx;
    INT8U size;
    INT16U nCount;
    INT32U size_of_file;
    INT16U size_of_block;
    //INT8U count_block[2];
    INT8U buffer[5];
    

    idx = 0;
    mem_set(buffer,5,0x00);
    if((param[idx] != DT_STRUCTURE)&& (param[idx+1] != 0x03)) return DAR_TYPE_UNMATCHED;
    idx+=2;
    size = get_object_attribute_item_len(param+idx,param[idx]);//��ȡ�ļ���Ϣ����
    //set_object_attribute_index(OBIS_BLOCK_TRANSMIT,0,param+idx);   //�洢�ļ���Ϣ
    (void)get_attribute_inbuffer(2,param+idx,buffer,5,TRUE,0xFF);//��ȡ�ļ���С
    size_of_file = cosem_bin2_int32u(buffer);
    fwrite_array_update_info(FLADDR_PROG_FILE_SIZE,(INT8U *)&size_of_file,4);
    idx+=size;
    if(param[idx] != DT_LONGUNSIGNED) return DAR_TYPE_UNMATCHED;
    size = 3;
    size_of_block = cosem_bin2_int16u(param+idx+1);//������С
    //mem_cpy(buffer,(INT8U *)&size_of_block,2);//�ȴ����buffer�У�������С
    idx+=size;
    size = get_object_attribute_item_len(param+idx,param[idx+1]);//��ȡУ��structure����
    mem_cpy(update_object.frame_crc,param+idx,size);//������structure�浽update_object->frame_crc�У������ȡ
    if((param[idx] != DT_STRUCTURE)&& (param[idx+1] != 0x02)) return DAR_TYPE_UNMATCHED;
    idx+=size;
    nCount = size_of_file/size_of_block;
    if((size_of_file%size_of_block) !=0)
    {
        nCount+=1;
    }
    //fwrite_array(FILEID_RUN_PARAM,FLADDR_PROG_TOTAL_SEC_COUNT,(INT8U*)&nCount,2);//�ܿ���
    update_object.total_sec_count=nCount;
    update_object.sec_size=size_of_block;
    //ĩ�δ�С
    //nCount=size_of_file-(nCount-1)*size_of_block;
    //mem_cpy(buffer+2,(INT8U *)&nCount,2);//ĩ�δ�С
    //fwrite_array(FILEID_RUN_PARAM,FLADDR_PROG_FRAME_LEN,buffer,4);
    mem_set(update_object.sec_status,sizeof(update_object.sec_status),0xFF);
    return DAR_SECCESS;
}

void start_transmit_info(INT8U* param,objResponse *pResp)
{
    INT8U dar_flag;
    dar_flag = transmit_info_check(param,pResp);
    pResp->frame[pResp->pos++] = dar_flag;
    pResp->frame[pResp->pos++] = DT_NULL;//������������          Data  OPTIONAL
    
}

INT8U write_transmit_file_check(INT8U* param,objResponse *pResp,INT8U *resetFlag)
{
    INT16U fileDataLen,len;
    INT32U progAddr;
    INT8U respflag;
    INT16U tag_cur_seg;
    INT8U fileFlag;
    BOOLEAN sucess;
    INT8U idx;
    INT8U head_size;
    
    respflag = DAR_SECCESS;
    idx = 0;
    if((param[idx] != DT_STRUCTURE)&& (param[idx+1] != 0x02)) return DAR_TYPE_UNMATCHED;
    idx+=2;
    if(param[idx] != DT_LONGUNSIGNED) return DAR_TYPE_UNMATCHED;
    idx+=1;
    tag_cur_seg = cosem_bin2_int16u(param+idx);//��ȡ��ǰ��
    idx+=2;
    if(param[idx] != DT_OCTETSTRING) return DAR_TYPE_UNMATCHED;
    idx+=1;
    fileDataLen = get_cosem_data_len(param+idx,&head_size);
    idx+=head_size;
    if(fileDataLen==0)
    {
        return DAR_READ_WRITE_DENIED;
    }
    if(update_object.total_sec_count==tag_cur_seg+1)
    {
        progAddr = (INT32U)tag_cur_seg * update_object.sec_size;
    }
    else
    {
        progAddr = (INT32U)tag_cur_seg * fileDataLen;
    }

    if(0==tag_cur_seg)
    {
        delete_update_file();
    }
	#ifdef __MX_UPDATE__
	if(progAddr<512)
	{
	    if(progAddr+fileDataLen>512)
		{
			len=512-progAddr;
			fwrite_array_update_data(FLADDR_PORG_HEADER+progAddr,param+idx,len);
	 		//��Ҫ�Ƚ���
			decode_update(param+idx+len,fileDataLen-len,progAddr+len-512);
		    fwrite_array_update_data(progAddr+len-512,param+idx+len,fileDataLen-len);
		}
		else
		{
			fwrite_array_update_data(FLADDR_PORG_HEADER+progAddr,param+idx,fileDataLen);
		}
	}
	else
	{
 		//��Ҫ�Ƚ���
		decode_update(param+idx,fileDataLen,progAddr-512);
	    fwrite_array_update_data(progAddr-512,param+idx,fileDataLen);
	}
	#else
    fwrite_array_update_data(progAddr,param+idx,fileDataLen);
	#endif
    //��֤�����汾�Ƿ����Ҫ��
    if(0==tag_cur_seg)
    {
        mem_cpy(update_object.buffer,param+idx,512);
        sucess=read_datfile_header(&update_header,update_object.buffer);
        fileFlag = (update_header.filetype);
        //�˴���δ�����ã�fileFlagδ���ж�ȡ����Ӧ�����ļ�ƫ�Ʋ�ȷ��
        if(!sucess ||(FALSE==check_update_header(&update_header,fileFlag)))
        {
			respflag = DAR_DATA_BLOCK_UNAVAILABLE;//��鷢���ļ�����
            delete_update_file();
        }
    }
    if(respflag == 0)
    {
        update_download_record(update_object.total_sec_count,tag_cur_seg,fileDataLen);
        //������ж��Ƿ��Ѿ����أ����������ɣ�����Ӧ����
        if(check_download_record(pResp->frame+pResp->pos))
        {
            if(datfile_update(&update_header,resetFlag,pResp->frame+pResp->pos)==FALSE)
            {
               respflag = DAR_LONG_WRITE_ABORTED;//��鷢���ļ�����
            }
        }
    }
    return  respflag;
}

void write_transmit_file(INT8U* param,objResponse *pResp,INT8U *resetFlag)
{
    INT8U dar_flag;
    dar_flag = write_transmit_file_check(param,pResp,resetFlag);
    pResp->frame[pResp->pos++] = dar_flag;
    pResp->frame[pResp->pos++] = DT_NULL;//������������          Data  OPTIONAL
}
/*
INT8U read_transmit_file_check(INT8U* param,objResponse *pResp,INT8U *cur_block_count,INT8U *block_len)
{
    INT32U filesize;
    INT16U nCount;
    INT8U idx,val;
    INT8U buffer[5];
    INT16U block_frame_count;
    
    idx=0;
    val=0;
    mem_set(buffer,sizeof(buffer),0x00);
    
    if(param[idx] != DT_STRUCTURE) return DAR_TYPE_UNMATCHED;
    idx+=2;
    if(param[idx] != DT_LONGUNSIGNED) return DAR_TYPE_UNMATCHED;
    block_frame_count = cosem_bin2_int16u(param+idx+1);
    mem_cpy_reverse(cur_block_count,param+idx+1,2);
    fwrite_array_update_info(FLADDR_PROG_FILE_SIZE,buffer,4);;
    filesize=bin2_int32u(buffer);
    nCount=filesize/1024;
    if(filesize%1024!=0)
    {
        nCount++;
    }
    //nCount = bin2_int16u(buffer+3);
    if(block_frame_count>nCount)  return DAR_DATA_BLOCK_NUMBER_INVALID;
    if(block_frame_count == nCount-1)
    {
        nCount=filesize-block_frame_count*1024;
        block_len[0] = nCount & 0xFF;
        block_len[1] = nCount>>8;
    }
    else
    {
        block_len[0]=0;
        block_len[1]=4;
    }
    //fread_array(FILEID_UPDATE_CTRL,FLADDR_PROG_DOWNLOAD_RECORD+block_frame_count/8,&val,1);
    //if(get_oop_bit_value(&val,1,block_frame_count%8)) return DAR_READ_WRITE_DENIED;
    return DAR_SECCESS;
}
*/
void read_transmit_file(INT8U* param,objResponse *pResp)
{
    INT8U dar_flag,idx=0;
    INT16U cur_block_count;
    //INT16U block_len;
    //INT8U buffer[2];
    INT16U frame_len;
    
    cur_block_count=0;
    if(param[idx] != DT_STRUCTURE)
    dar_flag = DAR_TYPE_UNMATCHED;
    idx+=2;
    if(param[idx] != DT_LONGUNSIGNED)
    dar_flag = DAR_TYPE_UNMATCHED;
    cur_block_count = cosem_bin2_int16u(param+idx+1);
    //dar_flag = read_transmit_file_check(param,pResp,(INT8U *)&cur_block_count,(INT8U *)&block_len);
    dar_flag = DAR_SECCESS;
    pResp->frame[pResp->pos++] = dar_flag;
    if(dar_flag != DAR_SECCESS)
    {
        pResp->frame[pResp->pos++] = DT_NULL;//������������          Data  OPTIONAL
    }
    else
    {
        pResp->frame[pResp->pos++] = 0x01;
        pResp->frame[pResp->pos++] = 0x02;
        pResp->frame[pResp->pos++] = 0x01;
        pResp->frame[pResp->pos++] = 0x09;
        pResp->frame[pResp->pos++] = 0x82;
        pResp->frame[pResp->pos++] = 0x01;
        pResp->frame[pResp->pos++] = 0x00;
        /*
        if(block_len>127)//����127���ֽ���������
        {
            if(block_len<256)
            {
                pResp->frame[pResp->pos++] = 0x81;//Ĭ�ϲ��ᳬ�������ֽڱ�ʾ�Ķ���
                pResp->frame[pResp->pos++] = block_len;
            }
            else
            {
                pResp->frame[pResp->pos++] = 0x82;//Ĭ�ϲ��ᳬ�������ֽڱ�ʾ�Ķ���
                cosem_int162_bin(block_len,pResp->frame+pResp->pos);
                pResp->pos+=2;
            }
        }
        else
        {
            pResp->frame[pResp->pos++] = block_len;
        }
        */
        //mem_set(buffer,2,0x00);
        //fread_array_update_info(FLADDR_PROG_FRAME_LEN,buffer,2);
        //frame_len = bin2_int16u(buffer);
        frame_len = 256;
        fread_array_update_data((INT32U)frame_len*cur_block_count,pResp->frame+pResp->pos,frame_len);
        pResp->pos+=frame_len;
    }
}


void exec_block_transmit_method(OBJECT_ATTR_DESC *oad,INT8U* param,objResponse *pResp,INT8U *resetFlag)
{
    switch(oad->attribute_id.value)
    {
        case 3:
            delete_update_file();
            pResp->frame[pResp->pos++] = 0;
            pResp->frame[pResp->pos++] = DT_NULL;//������������          Data  OPTIONAL            
            break;
        case 7:
            start_transmit_info(param,pResp);
            break;
        case 8:
            write_transmit_file(param,pResp,resetFlag);
            break;
        case 9:
            read_transmit_file(param,pResp);
            break;
        case 10:
            break;
        default:
            break;

    }
}

BOOLEAN check_meter_doc_param(INT8U *buffer,INT8U index,INT8U *err_flag)
{
    BOOLEAN result=TRUE,is_all_flag=FALSE;
    INT16U pos,meter_seq,data_pos;
    INT8U col_idx,col_cnt,port,len;
    INT32U port_32u;

    if(index>4)
    {
        return FALSE;
    }
    pos = 0;
    switch(index)
    {
    case 0:
        is_all_flag = TRUE;
        if((buffer[pos++]!=DT_STRUCTURE)||(buffer[pos++]!=4))
        {
            result = FALSE;
            *err_flag=DAR_TYPE_UNMATCHED;
            break;
        }
    case 1:
        if(buffer[pos++]!=DT_LONGUNSIGNED)
        {
            result = FALSE;
            *err_flag=DAR_TYPE_UNMATCHED;
        }
        else
        {
            meter_seq=cosem_bin2_int16u(buffer+pos);
            pos+=2;
            if((meter_seq>2000)||(meter_seq==0))
            {
                *err_flag = DAR_OTHER_REASON;
                result=FALSE;
            }
        }
        if((!result)||(!is_all_flag))
        {
            break;
        }
    case 2:
        if((buffer[pos++]!=DT_STRUCTURE)||(buffer[pos++]!=10))
        {
            result = FALSE;
            *err_flag=DAR_TYPE_UNMATCHED;
            break;
        }
        if(buffer[pos++]!=DT_TSA)
        {
            *err_flag=DAR_TYPE_UNMATCHED;
            result = FALSE;
            break;
        }
        len=(buffer[pos+1]+1);
        if((len!=6)||(buffer[pos]!=7))
        {
            *err_flag=DAR_OTHER_REASON;
            result = FALSE;
            break;
        }
        pos+=len+2;
        if((buffer[pos]!=DT_ENUM)||((buffer[pos+1]>10)&&(buffer[pos+1]!=0xFF)))
        {
            result = FALSE;
            *err_flag=DAR_TYPE_UNMATCHED;
            break;
        }
        pos+=2;
        if((buffer[pos]!=DT_ENUM)||(buffer[pos+1]>4))
        {
            result = FALSE;
            *err_flag=DAR_TYPE_UNMATCHED;
            break;
        }
        pos+=2;
        if(buffer[pos++]!=DT_OAD)
        {
            result = FALSE;
            *err_flag=DAR_TYPE_UNMATCHED;
            break;
        }
        pos+=4;
        if(buffer[pos++]!=DT_OCTETSTRING)
        {
            *err_flag=DAR_TYPE_UNMATCHED;
            result = FALSE;
            break;
        }
        len=(buffer[pos++]);
        if(len>10)
        {
            *err_flag=DAR_OTHER_REASON;
            result = FALSE;
            break;
        }
        pos+=len;
        if((buffer[pos]!=DT_UNSIGNED)||(buffer[pos+2]!=DT_UNSIGNED))
        {
            *err_flag=DAR_TYPE_UNMATCHED;
            result = FALSE;
            break;
        }
        pos+=4;
        if((buffer[pos]!=DT_ENUM)||(buffer[pos+1]>3))
        {
            *err_flag=DAR_TYPE_UNMATCHED;
            result = FALSE;
            break;
        }
        pos+=2;
        if((buffer[pos]!=DT_LONGUNSIGNED)||(buffer[pos+3]!=DT_LONGUNSIGNED))
        {
            *err_flag=DAR_TYPE_UNMATCHED;
            result = FALSE;
            break;
        }
        pos+=6;
        if(!is_all_flag)
        {
            break;
        }
    case 3:
        if((buffer[pos++]!=DT_STRUCTURE)||(buffer[pos++]!=4))
        {
            *err_flag=DAR_TYPE_UNMATCHED;
            result = FALSE;
            break;
        }
        if(buffer[pos++]!=DT_TSA)
        {
            *err_flag=DAR_TYPE_UNMATCHED;
            result = FALSE;
            break;
        }
        len=(buffer[pos+1]+1);
        if((len>6)||(buffer[pos]!=len+1))
        {
            *err_flag=DAR_OTHER_REASON;
            result = FALSE;
            break;
        }
        pos+=len+2;
        if(buffer[pos++]!=DT_OCTETSTRING)
        {
            *err_flag=DAR_TYPE_UNMATCHED;
            result = FALSE;
            break;
        }
        len=(buffer[pos++]);
        if(len>20)
        {
            *err_flag=DAR_OTHER_REASON;
            result = FALSE;
            break;
        }
        pos+=len;
        if((buffer[pos]!=DT_LONGUNSIGNED)||(buffer[pos+3]!=DT_LONGUNSIGNED))
        {
            *err_flag=DAR_TYPE_UNMATCHED;
            result = FALSE;
            break;
        }
        pos+=6;
        if(!is_all_flag)
        {
            break;
        }
    case 4:
        data_pos=pos;
        if(buffer[pos++]!=DT_ARRAY)
        {
            *err_flag=DAR_TYPE_UNMATCHED;
            result = FALSE;
            break;
        }
        col_cnt = buffer[pos++];
        for(col_idx=0;col_idx<col_cnt;col_idx++)
        {
            if((buffer[pos]!=DT_STRUCTURE)||(buffer[pos+1]!=2)||(buffer[pos+2]!=DT_OAD))
            {
                *err_flag=DAR_TYPE_UNMATCHED;
                result=FALSE;
                break;
            }
            else
            {
                pos += 7;
                pos += get_object_attribute_item_len(buffer+pos,buffer[pos]);
            }
        }
        len=pos-data_pos;
        if(len>50)
        {
            *err_flag=DAR_OTHER_REASON;
            result = FALSE;
            break;
        }
        break;
    }
    return result;
}

void exec_object_meter_doc_method(OBJECT_ATTR_DESC *oad,INT8U* param,objResponse *pResp)
{

    INT8U err_flag = 0,head_size;
    INT8U *data,*buffer;
    INT16U len,param_len,col_len,meter_seq;
    COSEM_TSA *tsa;
    OBJECT_ATTR_DESC_TAG *port_info;

    switch(oad->attribute_id.value)
    {
        case 127://������Ӳɼ��������õ�Ԫ
            if(check_meter_doc_param(param,0,&err_flag))
            {
                len=get_object_attribute_item_len(param,param[0]);
                fwrite_ertu_params(ACQ_METER_DOC_INFO,param,len);
            }
            pResp->frame[pResp->pos++] = err_flag;
            pResp->frame[pResp->pos++] = DT_NULL;//������������          Data  OPTIONAL
            break;
        case 128: //������Ӳɼ��������õ�Ԫ
            if((param[0]!=DT_ARRAY)||(param[1]!=1))
            {
                err_flag=DAR_TYPE_UNMATCHED;
            }
            else
            {
                param+=2;
                if(check_meter_doc_param(param,0,&err_flag))
                {
                    len=get_object_attribute_item_len(param,param[0]);
                    fwrite_ertu_params(ACQ_METER_DOC_INFO,param,len);
                }
            }
            pResp->frame[pResp->pos++] = err_flag;
            pResp->frame[pResp->pos++] = DT_NULL;//������������          Data  OPTIONAL
            break;
        case 129: //�������õ�Ԫ�Ļ�����Ϣ����
            if((param[0]!=DT_STRUCTURE)||(param[1]!=0x02)||(param[2]!=DT_LONGUNSIGNED))
            {
                err_flag = DAR_TYPE_UNMATCHED;
            }
            else
            {
                meter_seq = param[3]<<8;
                meter_seq += param[4];
                buffer=pResp->frame+pResp->pos;
                fread_ertu_params(ACQ_METER_DOC_INFO,buffer,MAX_METER_DOC_SIZE);
                len=get_object_attribute_item_len(buffer,buffer[0]);
                if((check_meter_doc_param(buffer,0,&err_flag))&&(meter_seq==cosem_bin2_int16u(buffer+5)))
                {
                    param_len=get_object_attribute_item_len(param,param[0]);
                    col_len=get_object_attribute_item_value(buffer,DT_STRUCTURE,FALSE,1,&data,&head_size);
                    if((data!=NULL)&&(col_len>0))
                    {
                        mem_cpy(buffer+MAX_METER_DOC_SIZE,buffer,data-buffer);
                        mem_cpy(data+MAX_METER_DOC_SIZE,param,param_len);
                        mem_cpy(data+MAX_METER_DOC_SIZE+param_len,data+col_len,buffer+len-data-col_len);
                        len=get_object_attribute_item_len(buffer+MAX_METER_DOC_SIZE,buffer[MAX_METER_DOC_SIZE]);
                        fwrite_ertu_params(ACQ_METER_DOC_INFO,buffer+MAX_METER_DOC_SIZE,len);
                    }
                    else
                    {
                        err_flag=DAR_OTHER_REASON;
                    }
                }
                else
                {
                    err_flag=DAR_OTHER_REASON;
                }
            }
            pResp->frame[pResp->pos++] = err_flag;
            pResp->frame[pResp->pos++] = DT_NULL;//������������          Data  OPTIONAL
            break;
//        case 130: //�������õ�Ԫ����չ��Ϣ�Լ�������Ϣ������ΪNULL��ʾ�����¡�
//            update_meter_exinfo(param,&err_flag);
//            pResp->frame[pResp->pos++] = err_flag;
//            pResp->frame[pResp->pos++] = DT_NULL;//������������          Data  OPTIONAL
//            break;
        case 131: //�������ɾ������
            if(param[0] != DT_LONGUNSIGNED)
            {
                err_flag = DAR_TYPE_UNMATCHED;
            }
            else
            {
                meter_seq=cosem_bin2_int16u(param+1);
                buffer=pResp->frame+pResp->pos;
                fread_ertu_params(ACQ_METER_DOC_INFO,buffer,MAX_METER_DOC_SIZE);
                if((check_meter_doc_param(buffer,0,&err_flag))&&(meter_seq==cosem_bin2_int16u(buffer+5)))
                {
                    mem_set(buffer,MAX_METER_DOC_SIZE,0xFF);
                    fwrite_ertu_params(ACQ_METER_DOC_INFO,buffer,MAX_METER_DOC_SIZE);
                }
                else
                {
                    err_flag=DAR_OTHER_REASON;
                }
            }
            pResp->frame[pResp->pos++] = err_flag;
            pResp->frame[pResp->pos++] = DT_NULL;//������������          Data  OPTIONAL
            break;
//        case 132: //���ݵ���������Ϣɾ������
//            delete_meter_from_baseinfo(param,&err_flag);
//            pResp->frame[pResp->pos++] = err_flag;
//            pResp->frame[pResp->pos++] = DT_NULL;//������������          Data  OPTIONAL
//            break;
        case 133: //����ָ���˿ڵ�ĳ����ַɾ������
            if((param[0]==DT_STRUCTURE)&&(param[1]==2)&&(param[2]==DT_TSA))
            {
                tsa=(COSEM_TSA *)(param+2);
                if(param[4+tsa->len]==DT_OAD)
                {
                    err_flag = DAR_TYPE_UNMATCHED;
                }
                else
                {
                    port_info=(OBJECT_ATTR_DESC_TAG *)(param+4+tsa->len);
                    buffer=pResp->frame+pResp->pos;
                    fread_ertu_params(ACQ_METER_DOC_INFO+5,buffer,MAX_METER_DOC_SIZE);
                    col_len=get_object_attribute_item_len(buffer,buffer[0]);
                    if((compare_string(buffer,&tsa->tag,tsa->len+1)==0)&&(compare_string(buffer+col_len+4,port_info->value,5)==0))
                    {
                        mem_set(buffer,MAX_METER_DOC_SIZE,0xFF);
                        fwrite_ertu_params(ACQ_METER_DOC_INFO,buffer,MAX_METER_DOC_SIZE);
                    }
                    else
                    {
                        err_flag=DAR_OTHER_REASON;
                    }
                }
            }
            else
            {
                err_flag = DAR_TYPE_UNMATCHED;
            }
            pResp->frame[pResp->pos++] = err_flag;
            pResp->frame[pResp->pos++] = DT_NULL;//������������          Data  OPTIONAL
            break;
        case 134: //���ȫ������
            if(param[0] == 0)
            {
                buffer=pResp->frame+pResp->pos;
                mem_set(buffer,MAX_METER_DOC_SIZE,0xFF);
                fwrite_ertu_params(ACQ_METER_DOC_INFO,buffer,MAX_METER_DOC_SIZE);
                pResp->frame[pResp->pos++] = DAR_SECCESS;
            }
            else
            {
                pResp->frame[pResp->pos++] = DAR_OTHER_REASON;
            }
            pResp->frame[pResp->pos++] = DT_NULL;//������������          Data  OPTIONAL
            break;
        default:
            pResp->frame[pResp->pos++] = DAR_OTHER_REASON;
            pResp->frame[pResp->pos++] = DT_NULL;//������������          Data  OPTIONAL
            break;
    }
}

BOOLEAN check_batch_add_task_format(INT8U* param, INT8U count)
{
    INT16U pos;
    INT8U task_count;
    INT8U idx,idx_1;
    INT8U period_count;
    
    pos=0;

    for(idx=0;idx<count;idx++)
    {
        if((param[pos] != DT_STRUCTURE)||(param[pos+1] != 0x0C)||(param[pos+2] != DT_UNSIGNED)||(param[pos+4] != DT_TI)||(param[pos+5] > 5)||(0 == cosem_bin2_int16u(param+pos+6))
                ||(param[pos+8] != DT_ENUM)||(param[pos+9] == 0)||(param[pos+9]>5)||(param[pos+10] != DT_UNSIGNED)||(param[pos+12] != DT_DATETIME_S)
                ||(param[pos+20] != DT_DATETIME_S)||(param[pos+28] != DT_TI)||(param[pos+29] >5)||((param[pos+32] != DT_UNSIGNED)&&(param[pos+32] != DT_ENUM))
                ||(param[pos+34] != DT_ENUM)||((param[pos+35] !=1)&&(param[pos+35] !=2))||(param[pos+36] != DT_LONGUNSIGNED)||(param[pos+39] != DT_LONGUNSIGNED)
                ||(param[pos+42] != DT_STRUCTURE)||(param[pos+43] != 2)||(param[pos+44] != DT_ENUM)||(param[pos+45] > 3)||(param[pos+46] != DT_ARRAY)||(param[pos+47] == 0))
        {
            return FALSE;
        }
        pos += 47;
        period_count = param[pos];
        if(period_count > 7)
        {
            return FALSE;
        }
        pos++;
        for(idx_1=0;idx_1<period_count;idx_1++)
        {
            if((param[pos] != DT_STRUCTURE)||(param[pos+1] != 4)||(param[pos+2] != DT_UNSIGNED)||(param[pos+3] >23)||(param[pos+4] != DT_UNSIGNED)||(param[pos+5] >59)
                    ||(param[pos+6] != DT_UNSIGNED)||(param[pos+7] >23)||(param[pos+8] != DT_UNSIGNED)||(param[pos+9] >59))
            {
                return FALSE;
            }
            pos+=10;
        }
    }
    return TRUE;
}

BOOLEAN check_batch_add_normal_format(INT8U* param,INT8U scheme_count)
{
    INT16U pos=0;
    INT8U idx,csd_count;
    
    for(idx=0;idx<scheme_count;idx++)
    {
        if((param[pos]!=DT_STRUCTURE)||(param[pos+1]!=6)||(param[pos+2]!=DT_UNSIGNED)||(param[pos+4]!=DT_LONGUNSIGNED)||(bin2_int16u(param+pos+5) == 0)
                ||(param[pos+7]!=DT_STRUCTURE)||(param[pos+8]!=2)||(param[pos+9]!=DT_UNSIGNED)||(param[pos+10]>5))
        {
            return FALSE;
        }
        pos+=10;
        if((param[pos] == 0)||(param[pos] == 2)||(param[pos] == 5))
        {
            if(param[pos+1]!=DT_NULL)
            {
                return FALSE;
            }
            pos+=2;
        }
        else if(param[pos] == 1)
        {
            if(param[pos+1]!=DT_UNSIGNED)
            {
                return FALSE;
            }
            pos+=3;
        }
        else if(param[pos] == 3)
        {
            if((param[pos+1]!=DT_TI)||(param[pos+2]>5)||(0 == cosem_bin2_int16u(param+pos+3)))
            {
                return FALSE;
            }
            pos+=5;
        }
        else if(param[pos] == 4)
        {
            if((param[pos+1]!=DT_STRUCTURE)||(param[pos+1]!=2)||(param[pos+3]!=DT_TI)||(param[pos+4]>5)||(0 == cosem_bin2_int16u(param+pos+5))||(param[pos+7]!=DT_LONGUNSIGNED))
            {
                return FALSE;
            }
            pos+=10;
        }
        if(param[pos]!=DT_ARRAY)
        {
            return FALSE;
        }
        csd_count = param[pos+1];
        if(csd_count == 0)
        {
            return FALSE;
        }
        pos+=get_object_attribute_item_len(param+pos,param[pos]);
        if((param[pos]!=DT_MS)||(param[pos+1]>7))
        {
            return FALSE;
        }
        pos+=get_object_attribute_item_len(param+pos,param[pos]);
        if((param[pos]!=DT_ENUM)||(param[pos+1])>7)
        {
            return FALSE;
        }
        pos+=2;
    }
    return TRUE;
}

BOOLEAN check_batch_add_event_format(INT8U* param,INT8U scheme_count)
{
    INT16U pos=0;
    INT8U idx,csd_count;
    
    for(idx=0;idx<scheme_count;idx++)
    {
        if((param[pos]!=DT_STRUCTURE)||(param[pos+1]!=5)||(param[pos+2]!=DT_UNSIGNED)||(param[pos+4]!=DT_STRUCTURE)||(param[pos+5]!=2)||(param[pos+6]!=DT_UNSIGNED)||(param[pos+7]>2))
        {
            return FALSE;
        }
        pos+=7;
        if(param[pos] == 1)
        {
            if(param[pos+1]!=DT_NULL)
            {
                return FALSE;
            }
            pos+=2;
        }
        else if((param[pos] == 0)||(param[pos] == 2))
        {
            if((param[pos+1]!=DT_ARRAY)||(param[pos+2]==0))
            {
                return FALSE;
            }
            pos++;
            pos+=get_object_attribute_item_len(param+pos,param[pos]);
        }
        else
        {
            return FALSE;
        }
        if((param[pos]!=DT_MS)||(param[pos+1]>7))
        {
            return FALSE;
        }
        pos+=get_object_attribute_item_len(param+pos,param[pos]);
        if(param[pos]!=DT_BOOLEAN)
        {
            return FALSE;
        }
        pos+=2;
        if((param[pos]!=DT_LONGUNSIGNED)||(bin2_int16u(param+pos+1) == 0))
        {
            return FALSE;
        }
        pos+=3;
    }
    return TRUE;
}

BOOLEAN add_acq_ctrl_info(INT8U* param, INT16U len, INT16U max_size, INT8U max_count, uint32_t offset,INT8U acq_ctrl_flag)
{
    INT8U info_id,task_plan_idx = 0;
    uint32_t addr;
	DateTime cur_dt;
// 	INT8U tmp[250];
    info_id=param[3];
    if(len > max_size)
    {
        return FALSE;
    }

//�������Ҫ����һ�ַ���ȷ��addr�����ǲ��������ֶ�����������ȡ��ķ�������Ϊһ������Ƕ�20ȡ�࣬31��51���ᱻӳ�䵽ͬһ��λ���ϣ��ǿ϶��ǲ��Եġ�
/*
	| A | B |--|--|--|--|--|--|--|--|   <-----������
	| C | D |--|--|--|--|--|--|--|--|   <-----������
*/
	//gtask_plan_sequence;
	switch(acq_ctrl_flag)
	{
		case 1:
			for(task_plan_idx =0;task_plan_idx<MAX_TASK_COUNT;task_plan_idx++)
			{
				if(gtask_plan_sequence[0][task_plan_idx] == info_id)
				{
					break;
				}
				else if(gtask_plan_sequence[0][task_plan_idx] == 0x00)
				{
					gtask_plan_sequence[0][task_plan_idx] =info_id ;
					break;
				}
			}
			break;
		case 2:
			for(task_plan_idx =0;task_plan_idx<MAX_TASK_COUNT;task_plan_idx++)
			{
				if(gtask_plan_sequence[1][task_plan_idx] == info_id)
				{
					break;
				}
				else if(gtask_plan_sequence[1][task_plan_idx] == 0x00)
				{
					gtask_plan_sequence[1][task_plan_idx] =info_id ;
					break;
				}
			}		
			break;
		case 3:
			break;
		default:
			return FALSE;
	}	
    addr=offset+(task_plan_idx)*max_size;



    fwrite_ertu_params(addr,param,len);

	gSystemInfo.plan_task_chg =1;
	tpos_datetime(&cur_dt);
	mem_cpy(&gSystemInfo.dt,&cur_dt,sizeof(DateTime));
//  	fread_ertu_params(addr,tmp,len);   //�����������¿����Ƿ�д�����ȷ
    return TRUE;
}

BOOLEAN batch_add_acq_ctrl_info(INT16U obis,INT8U* param)
{
    uint32_t offset;
    INT16U pos,len,max_size;
    INT8U idx,count,max_count;
	INT8U  acq_ctrl_flag =0;
    pos = 0;
    count = 0;

    switch(obis)
    {
    case OBIS_TASK_TABLE:
        if((param[0]!=DT_ARRAY)||(param[1]==0)||(param[1]>MAX_TASK_COUNT))
        {
            return FALSE;
        }
        if(!check_batch_add_task_format(param+2,param[1]))
        {
            return FALSE;
        }
        offset=ACQ_TASK_INFO;
        max_size=MAX_TASK_INFO_SIZE;
        max_count=MAX_TASK_COUNT;
		acq_ctrl_flag =1;
        break;
    case OBIS_NORMAL_TASK_TABLE:
        if((param[0]!=DT_ARRAY)||(param[1]==0)||(param[1]>MAX_NORMAL_PLAN_COUNT))
        {
            return FALSE;
        }
        if(!check_batch_add_normal_format(param+2,param[1]))
        {
            return FALSE;
        }
        offset=ACQ_NORMAL_PLAN_INFO;
        max_size=MAX_NORMAL_PLAN_SIZE;
        max_count=MAX_NORMAL_PLAN_COUNT;
		acq_ctrl_flag =2;
        break;
    case OBIS_EVENT_TASK_TABLE:
        if((param[0]!=DT_ARRAY)||(param[1]==0)||(param[1]>MAX_EVENT_PLAN_COUNT))
        {
            return FALSE;
        }
        if(!check_batch_add_event_format(param+2,param[1]))
        {
            return FALSE;
        }
        offset=ACQ_EVENT_PLAN_INFO;
        max_size=MAX_EVENT_PLAN_SIZE;
        max_count=MAX_EVENT_PLAN_COUNT;
		acq_ctrl_flag =3;
        break;
    default:
        return FALSE;
    }
    count = param[1];
    pos += 2;
    if(count>max_count)
    {
        return FALSE;
    }
    for(idx=0;idx<count;idx++)
    {
        len=get_object_attribute_item_len(param+pos,param[pos]);
        if(add_acq_ctrl_info(param+pos,len,max_size,max_count,offset,acq_ctrl_flag) == FALSE)
        {
            return FALSE;
        }
        pos += len;
    }
    return TRUE;
}

BOOLEAN delete_acq_ctrl_info(INT16U obis,INT8U* param,BOOLEAN is_all)
{
    uint32_t offset;
    INT16U pos,len,max_size;
    INT8U idx,count,max_count,info_idx;
    INT8U buffer[10]={0};

    pos = 0;
    count = 0;

    if(!is_all)
    {
        if((param[0]!=DT_ARRAY)||(param[1]==0)||(param[1]>MAX_TASK_COUNT))
        {
            return FALSE;
        }
        for(idx=0;idx<param[1];idx++)
        {
            if(param[2+idx*2]!=DT_UNSIGNED)
            {
                return FALSE;
            }
        }
    }
    switch(obis)
    {
    case OBIS_TASK_TABLE:
        offset=ACQ_TASK_INFO;
        max_size=MAX_TASK_INFO_SIZE;
        max_count=MAX_TASK_COUNT;
        break;
    case OBIS_NORMAL_TASK_TABLE:
        offset=ACQ_NORMAL_PLAN_INFO;
        max_size=MAX_NORMAL_PLAN_SIZE;
        max_count=MAX_NORMAL_PLAN_COUNT;
        break;
    case OBIS_EVENT_TASK_TABLE:
        offset=ACQ_EVENT_PLAN_INFO;
        max_size=MAX_EVENT_PLAN_SIZE;
        max_count=MAX_EVENT_PLAN_COUNT;
        break;
    default:
        return FALSE;
    }
    if(is_all)
    {
        count=max_count;
    }
    else
    {
        count = param[1];
        pos = 2;
    }
    if(count>max_count)
    {
        return FALSE;
    }
    for(idx=0;idx<count;idx++)
    {
        if(is_all)
        {
            mem_set(buffer,10,0xFF);
            fwrite_ertu_params(offset+idx*max_size,buffer,10);
        }
        else
        {
            info_idx=param[pos+1];
            fread_ertu_params(offset+(info_idx%max_count)*max_size,buffer,10);
            if((buffer[0]!=DT_STRUCTURE)||(buffer[2]!=DT_UNSIGNED)||(buffer[3]!=info_idx))
            {
                continue;
            }
            else
            {
                mem_set(buffer,10,0xFF);
                fwrite_ertu_params(offset+(info_idx%max_count)*max_size,buffer,10);
            }
            pos += 2;
        }
    }
    return TRUE;
}

/************************************************************************/
/* ���flash�д洢�����ݲ��õ�����ұյķ���                               */
/************************************************************************/
void  delete_acq_data(INT16U beginSector,INT16U endSector)
{
	INT16U idx=0;
	if(beginSector>endSector)
	{
		return;
	}
	for(idx =beginSector;idx<=endSector;idx++)
	{
		nor_flash_erase_page(idx);
	}
}


BOOLEAN update_acq_ctrl_info(INT16U obis,INT8U* param,INT8U *buffer)
{
    uint32_t offset;
    INT16U max_size,len,ms_len,data_len;
    INT8U max_count,info_idx,head_size;
    INT8U *data=NULL;

    if((param[0]!=DT_STRUCTURE)||(param[1]!=2)||(param[2]!=DT_UNSIGNED))
    {
        return FALSE;
    }
    info_idx=param[3];
    switch(obis)
    {
    case OBIS_TASK_TABLE:
        offset=ACQ_TASK_INFO;
        max_size=MAX_TASK_INFO_SIZE;
        max_count=MAX_TASK_COUNT;
        break;
    case OBIS_NORMAL_TASK_TABLE:
        offset=ACQ_NORMAL_PLAN_INFO;
        max_size=MAX_NORMAL_PLAN_SIZE;
        max_count=MAX_NORMAL_PLAN_COUNT;
        break;
    case OBIS_EVENT_TASK_TABLE:
        offset=ACQ_EVENT_PLAN_INFO;
        max_size=MAX_EVENT_PLAN_SIZE;
        max_count=MAX_EVENT_PLAN_COUNT;
        break;
    default:
        return FALSE;
    }
    fread_ertu_params(offset+(info_idx%max_count)*max_size,buffer,max_size);
    if((buffer[0]!=DT_STRUCTURE)||(buffer[2]!=DT_UNSIGNED)||(buffer[3]!=info_idx))
    {
        return FALSE;
    }
    if(OBIS_TASK_TABLE==obis)
    {
        if((param[4]!=DT_ENUM)||(param[5]==0)||(param[5]>2))
        {
            return FALSE;
        }
        data_len=get_object_attribute_item_value(buffer,DT_STRUCTURE,FALSE,8,&data,&head_size);
        if((data==NULL)||(data_len!=2)||(data[0]!=DT_ENUM))
        {
            return FALSE;
        }
        else
        {
            mem_cpy(data,param+4,2);
            data_len=get_object_attribute_item_len(buffer,buffer[0]);
            fwrite_ertu_params(offset+(info_idx%max_count)*max_size,buffer,data_len);
        }
    }
    if(OBIS_NORMAL_TASK_TABLE==obis)
    {
        if((param[4]!=DT_ARRAY)||(param[5]==0))
        {
            return FALSE;
        }
        len=get_object_attribute_item_len(param+4,param[4]);
        ms_len=get_object_attribute_item_value(buffer,DT_STRUCTURE,FALSE,4,&data,&head_size);
        if((data==NULL)||(data[0]!=DT_MS))
        {
            return FALSE;
        }
        else
        {
            mem_cpy(buffer+max_size,data,ms_len+2);
        }
        ms_len=get_object_attribute_item_value(buffer,DT_STRUCTURE,FALSE,3,&data,&head_size);
        if((data==NULL)||(data[0]!=DT_ARRAY))
        {
            return FALSE;
        }
        else
        {
            mem_cpy(data,param+4,len);
            mem_cpy(data+len,buffer+max_size,ms_len+2);
            data_len=get_object_attribute_item_len(buffer,buffer[0]);
            fwrite_ertu_params(offset+(info_idx%max_count)*max_size,buffer,data_len);
        }
    }
    if(OBIS_EVENT_TASK_TABLE==obis)
    {
        if(param[4]!=DT_BOOLEAN)
        {
            return FALSE;
        }
        data_len=get_object_attribute_item_value(buffer,DT_STRUCTURE,FALSE,4,&data,&head_size);
        if((data==NULL)||(data_len!=2)||(data[0]!=DT_BOOLEAN))
        {
            return FALSE;
        }
        else
        {
            mem_cpy(data,param+4,2);
            data_len=get_object_attribute_item_len(buffer,buffer[0]);
            fwrite_ertu_params(offset+(info_idx%max_count)*max_size,buffer,data_len);
        }
    }
    return TRUE;        
}

void exec_object_acq_ctrl_info_method(OBJECT_ATTR_DESC *oad,INT8U* param,objResponse *pResp)
{
    INT16U idx,obis;

    obis=cosem_bin2_int16u(oad->object_id);
    switch(oad->attribute_id.value)
    {
    case 127: //Add(Array of ...)
        if(batch_add_acq_ctrl_info(obis,param)==FALSE)
        {
            pResp->frame[pResp->pos++]=DAR_OTHER_REASON;
        }
        else
        {
            pResp->frame[pResp->pos++] = DAR_SECCESS;
        }
        pResp->frame[pResp->pos++] = DT_NULL;//������������          Data  OPTIONAL
        break;
    case 128: //Delete(Array of OAD)
        if(delete_acq_ctrl_info(obis,param,FALSE)==FALSE)
        {
            pResp->frame[pResp->pos++]=DAR_OTHER_REASON;
        }
        else
        {
            pResp->frame[pResp->pos++] = DAR_SECCESS;
        }
         pResp->frame[pResp->pos++] = DT_NULL;//������������          Data  OPTIONAL
        break;
    case 129: //Clear()
        if((param[0]==0)&&(delete_acq_ctrl_info(obis,param,TRUE)))
        {
            pResp->frame[pResp->pos++] = DAR_SECCESS;
        }
        else
        {
             pResp->frame[pResp->pos++] = DAR_OTHER_REASON;
        }
        pResp->frame[pResp->pos++] = DT_NULL;//������������          Data  OPTIONAL
        break;
    case 130:
        if(update_acq_ctrl_info(obis,param,pResp->frame+pResp->pos)==FALSE)
        {
            pResp->frame[pResp->pos++]=DAR_OTHER_REASON;
        }
        else
        {
            pResp->frame[pResp->pos++] = DAR_SECCESS;
        }
        pResp->frame[pResp->pos++] = DT_NULL;//������������          Data  OPTIONAL
        break;
    default:
        pResp->frame[pResp->pos++]=DAR_OTHER_REASON;
        pResp->frame[pResp->pos++] = DT_NULL;//������������          Data  OPTIONAL
        break;
        
    }
}

BOOLEAN add_esam_oi(INT8U* param,INT16U *len)
{
    INT8U data[4];
    if((param[0]!=DT_STRUCTURE) || (param[1]!=2) || (param[2]!=DT_OI)) return FALSE;

    data[0]=param[3];
    data[1]=param[4];
    data[2]=param[6];
    data[3]=param[7];

    update_esam_oi_list(data);
    *len=8;

    return TRUE;
}

BOOLEAN batch_add_esam_oi(INT8U* param)
{
    INT16U pos,len;
    INT8U idx,count;

    pos = 0;
    count = 0;

    if(param[0] == DT_ARRAY) count = param[1];
    pos += 2;

    for(idx=0;idx<count;idx++)
    {
        if (param[pos] == DT_STRUCTURE)
        {
            if (add_esam_oi(param+pos,&len) == FALSE) return FALSE;
            pos += len;
        }
        else
        {
            return FALSE;
        }
    }
    return TRUE;
}

BOOLEAN delete_esam_oi(INT8U* param)
{
    if(param[0] != DT_OI) return FALSE;

    delete_esam_oi_list(param+1);
    return TRUE;
}

void exec_esam_oi_table_method(OBJECT_ATTR_DESC *oad,INT8U* param,objResponse *pResp)
{
    INT8U data[100]={0};
    INT16U len,write_len;
    INT32U offset;

    switch(oad->attribute_id.value)
    {
    case 1: //Clear()
        len=MAX_ESAM_PARAM_SIZE;
        offset=ESAM_PARAM_START;
        while(len>0)
        {
            mem_set(data,sizeof(data),0xFF);
            if(len>sizeof(data))
            {
                write_len=sizeof(data);
            }
            else
            {
                write_len=len;
            }
            fwrite_ertu_params(offset,data,write_len);
            len-=write_len;
            offset+=write_len;
        }
        g_esam_oi_list.count=0;
        pResp->frame[pResp->pos++] = DAR_SECCESS;
        pResp->frame[pResp->pos++] = DT_NULL;//������������          Data  OPTIONAL
        break;
    case 127: //Add
        if(add_esam_oi(param,&len)==FALSE)
        {
            pResp->frame[pResp->pos++]=DAR_OTHER_REASON;
        }
        else
        {
            pResp->frame[pResp->pos++] = DAR_SECCESS;
            fwrite_ertu_params(ESAM_PARAM_START,(INT8U*)&g_esam_oi_list,g_esam_oi_list.count*5+4);
        }
         pResp->frame[pResp->pos++] = DT_NULL;//������������          Data  OPTIONAL
        break;
    case 128: //Delete
        if(delete_esam_oi(param)==FALSE)
        {
            pResp->frame[pResp->pos++]=DAR_OTHER_REASON;
        }
        else
        {
            pResp->frame[pResp->pos++] = DAR_SECCESS;
            fwrite_ertu_params(ESAM_PARAM_START,(INT8U*)&g_esam_oi_list,g_esam_oi_list.count*5+4);
        }
         pResp->frame[pResp->pos++] = DT_NULL;//������������          Data  OPTIONAL
        break;
    case 129: //Add(Array of ...)
        if(batch_add_esam_oi(param)==FALSE)
        {
            pResp->frame[pResp->pos++]=DAR_OTHER_REASON;
        }
        else
        {
            pResp->frame[pResp->pos++] = DAR_SECCESS;
            fwrite_ertu_params(ESAM_PARAM_START,(INT8U*)&g_esam_oi_list,g_esam_oi_list.count*5+4);
        }
        pResp->frame[pResp->pos++] = DT_NULL;//������������          Data  OPTIONAL
        break;
    }
}

//�ع�����ŷ��������� gtask_plan_sequence
void  rebuilt_task_plan_sequence(INT8U *tmp) //tmp ռ400���ֽ�
{
	INT8U idx =0;
	INT32U  offset =0,addr =0;
	INT16U pos=0;
	INT8U task_pos_idx =0,plan_pos_idx =0;
	for(idx=0;idx<MAX_TASK_COUNT;idx++)
	{
		offset = ACQ_TASK_INFO;
		pos=0;
		addr=offset+(idx)*MAX_TASK_INFO_SIZE;
		fread_ertu_params(addr,tmp,MAX_TASK_INFO_SIZE);
		if(*tmp == 0x2)
		{
			gtask_plan_sequence[0][task_pos_idx++] = *(tmp+3);
		}
		offset = ACQ_NORMAL_PLAN_INFO;
		pos=0;
		addr=offset+(idx)*MAX_NORMAL_PLAN_SIZE;
		fread_ertu_params(addr,tmp,MAX_NORMAL_PLAN_SIZE);	
		if(*tmp == 0x2)
		{
			gtask_plan_sequence[1][plan_pos_idx++] = *(tmp+3);
		}			
	}
}

/************************************************************************/
/* �ع���������                                                          */
/************************************************************************/
void rebuilt_task_plan_index()
{
	INT8U idx =0,idx_config = 0;
	INT32U addr;
	INT32U offset = 0;
	INT8U tmp[400]={0};
	INT16U len =0;
	INT16U pos=0;
	INT8U task_id = 0,plan_id = 0,task_index_pos =0; //��ʾ��ȫ�ֱ���gTaskPlanAttribute������λ��
	INT8U task_context_num =0;
	INT8U plan_context_num = 0;
	INT8U oad_num =0,oad_store_idx = 0;
	INT8U task_run_time_stru_cnt =0,task_run_time_stru_idx =0;
	INT8U search_plan_no_idx =0,search_plan_no_result =0;
	mem_set(&gTaskPlanAttribute,sizeof(gTaskPlanAttribute),0x00);
	rebuilt_task_plan_sequence(tmp);     //�ع�����ŷ���������
	for(idx=0;idx<MAX_TASK_COUNT;idx++)
	{
		if(gtask_plan_sequence[0][idx] !=0x00)
		{
			offset = ACQ_TASK_INFO;
			pos=0;
			addr=offset+(idx)*MAX_TASK_INFO_SIZE;
			fread_ertu_params(addr,tmp,MAX_TASK_INFO_SIZE);
			len = get_object_attribute_item_len(tmp,tmp[0]);
			if(tmp[pos++] != 0x2) continue;   //�������struct����ôֱ���˳�
			task_context_num = tmp[pos++];
			pos++;
			task_id =tmp[pos++];
	//		task_index_pos =task_id%MAX_TASK_COUNT;
			gTaskPlanAttribute[idx].task_id =task_id;
			for(idx_config = 1;idx_config<task_context_num;idx_config++)
			{
				len=get_object_attribute_item_len(tmp+pos,tmp[pos]);
				switch(idx_config)
				{
					case 1:
						gTaskPlanAttribute[idx].run_unit =tmp[pos+1];
						gTaskPlanAttribute[idx].run_frq = (tmp[pos+2]<<8)+tmp[pos+3];
						break;
					case 2:
						gTaskPlanAttribute[idx].task_type =tmp[pos+1];
						break;
					case 3:
						gTaskPlanAttribute[idx].plan_no =tmp[pos+1];
						break;
					case 11:
						//if(tmp[pos++] ==DT_STRUCTURE)  //��������ʱ�α�����struct��ͷ
						//{
							//task_run_time_stru_cnt = tmp[pos++];
							//for(task_run_time_stru_idx =0;task_run_time_stru_idx<task_run_time_stru_cnt;task_run_time_stru_idx++)
							//{
								//if(tmp[pos++] ==DT_ENUM)
								//{
									//gTaskPlanAttribute[task_id].task_running_time.task_running_type =tmp[pos++];
								//}
								//if(tmp[pos++] ==DT_ARRAY)
								//{
									//if(tmp[pos++]>=1)  //ʱ�α��е�ʱ�θ�������1
									//{
										//if(tmp[pos++] == DT_STRUCTURE)
										//{
											//pos+=2;
											//gTaskPlanAttribute[task_id].task_running_time.task_running_begin_hour = tmp[pos];
											//pos+=2;
											//gTaskPlanAttribute[task_id].task_running_time.task_running_begin_minute = tmp[pos];
											//pos+=2;
											//gTaskPlanAttribute[task_id].task_running_time.task_running_end_hour = tmp[pos];
											//pos+=2;
											//gTaskPlanAttribute[task_id].task_running_time.task_running_end_minute = tmp[pos];										
										//}
									//}
								//}
							//}
						//}
						break;
					
				} 
				pos += len;
			}
			search_plan_no_result = 0;
			for(search_plan_no_idx =0;search_plan_no_idx<MAX_TASK_COUNT;search_plan_no_idx++)
			{
				if(gtask_plan_sequence[1][search_plan_no_idx] ==gTaskPlanAttribute[idx].plan_no)
				{
					search_plan_no_result =1;
					break;
				}
			}
			if(search_plan_no_result ==0)
				continue;
			pos = 0;
			offset = ACQ_NORMAL_PLAN_INFO;
			addr = offset + (search_plan_no_idx)*MAX_NORMAL_PLAN_SIZE;  //������Ӧ�����ŵ����ݡ�
			fread_ertu_params(addr,tmp,MAX_NORMAL_PLAN_SIZE); 
			if(tmp[pos++] != 0x2) continue;   //�������struct����ôֱ���˳�
			plan_context_num =tmp[pos++];
			pos++;
			plan_id =tmp[pos++];
			for(idx_config = 1;idx_config<plan_context_num;idx_config++)
			{
				len=get_object_attribute_item_len(tmp+pos,tmp[pos]);
				switch(idx_config)
				{
					case 2:
						gTaskPlanAttribute[idx].acq_type =tmp[pos+3];
						switch(gTaskPlanAttribute[idx].acq_type)
						{
							case 0:
								gTaskPlanAttribute[idx].acq_content[0] = 0x00;  //eg 00 �ɵ�ǰ����
								break;
							case 1:
								gTaskPlanAttribute[idx].acq_content[0] =tmp[pos+5];  //����һ�εġ�
								break;
							case 2:
								gTaskPlanAttribute[idx].acq_content[0] = 0x00;  //eg 00 ������ʱ��ɼ�
								break;
							case 3:
								mem_cpy(gTaskPlanAttribute[idx].acq_content,tmp+pos+5,3);  //eg 54 01 00 0f ����TI���
								break;
						}
						break;
					case 3:
						pos+=3;
						if((tmp[pos]) ==0x01)
						{		pos++;
								mem_cpy(gTaskPlanAttribute[idx].master_oad,tmp+pos,4);
								pos+=4;
								oad_num = tmp[pos++];
								gTaskPlanAttribute[idx].oadcnt = oad_num;
								for(oad_store_idx =0;oad_store_idx<oad_num;oad_store_idx++)
								{
									mem_cpy(gTaskPlanAttribute[idx].planOad[oad_store_idx],tmp+pos,4);
									gTaskPlanAttribute[idx].planOad[oad_store_idx][3] = 0;
									pos+=4;
								}
							continue;
						}
						else if((tmp[pos]) ==0x00)
						{
							pos++;
							oad_num =tmp[pos-3];
							gTaskPlanAttribute[idx].oadcnt = oad_num;
							mem_set(gTaskPlanAttribute[idx].master_oad,4,0xFF);
							for(oad_store_idx =0;oad_store_idx<oad_num;oad_store_idx++)
							{
								mem_cpy(gTaskPlanAttribute[idx].planOad[oad_store_idx],tmp+pos,4);
								gTaskPlanAttribute[idx].planOad[oad_store_idx][3] = 0;
								pos+= 6;
							}
							continue;
						}
						break;
					case 5:
						gTaskPlanAttribute[idx].store_time_flag= tmp[pos+1];
						break;
				}
				pos += len;
			}
		}
			
	}
	
	//mem_set(&gPlanAttribute,0xFF,sizeof(PLAN_ATTRI_DESCRIBE));
	//offset = ACQ_NORMAL_PLAN_INFO;
	//for(idx =0;idx<MAX_NORMAL_PLAN_COUNT;idx++)
	//{
		//pos =0;
		//addr=offset+(idx)*MAX_NORMAL_PLAN_SIZE;
		//fread_ertu_params(addr,tmp,MAX_NORMAL_PLAN_COUNT);
		//if(tmp[pos++] != 0x2) continue;   //�������struct����ôֱ���˳�
		//plan_context_num =tmp[pos++];
		//pos++;
		//plan_id =tmp[pos++];
		//gPlanAttribute.tagPlanDescribe[plan_id].plan_id =plan_id;
		//for(idx_config = 1;idx_config<plan_context_num;idx_config++)
		//{
			//len=get_object_attribute_item_len(tmp+pos,tmp[pos]);
			//switch(idx_config)
			//{
				//case 2:
					//gPlanAttribute.tagPlanDescribe[plan_id].acq_type =tmp[pos+3];
					//break;
				//case 5:
					//gPlanAttribute.tagPlanDescribe[plan_id].store_time = tmp[pos+1];
			//}
			//pos += len;
		//}
	//}
}
// read_idx Ҫ��������idx��������������������Ǽ��������ͨ��getPassedDays()���������
INT16U make_meter_data_from_flash_minute_hold(INT8U hold_list_idx,INT32U *oad,INT8U oad_cnt,INT32U read_idx,INT8U *buffer,INT8U *time)  //���ǵ��������ݵ����⡣
{
	INT8U pot_idx =0;
	INT8U oad_idx =0;
	tagREADITEM read_item;
	INT8U data[200] ={0};
	INT16U valide_data_length =0;		
	INT8U data_pos = 0,data_len =0;
	for(oad_idx =0;oad_idx<oad_cnt;oad_idx++)   //����Ҫ���ٸ�oad�� read_item.offset
	{
		mem_set(data,200,0x00);
		data_pos =DATA_HEADER_SIZE;
		oad_search_in_hold_list(hold_list_idx,(INT8U*)(oad+oad_idx),&read_item);
		read_flash_data_from_save_idx(read_idx,read_item.offset , data, 200,
			read_item.block_data_len, FLASH_SECTOR_METER_DATA_START + read_item.page_id, read_item.page_count);
		//�ж�ʱ����Ч��
		
		//
		
		if(data[data_pos++] ==0x01)  //0x01 ���������ݡ�
		{
			data_len = get_object_attribute_item_len(data+data_pos,data[data_pos]);
			mem_cpy(buffer,data+data_pos,data_len);
			valide_data_length += data_len;
			buffer += data_len;
		}
		else
		{
			*(buffer++) =0x00;
			valide_data_length++;
		}
	}
	return valide_data_length;
}
//typedef union
//{
	//INT8U value[10];
	//struct
	//{
		//INT8U second;
		//INT8U minute;
		//INT8U hour;
		//INT8U day;
		//INT8U month;
		//INT8U year;
        //INT8U weekday;
		//INT8U msecond_h;
		//INT8U msecond_l;
		//INT8U century;
	//};
//}DateTime;
void change_time_forme(INT8U *time,tagDatetime *dt)
{
	dt->year = (*(INT16U *)time-2000);
	dt->month = *(time+2);
	dt->day =*(time+3);
	dt->hour =*(time+4);
	dt->minute =*(time+5);
}
// read_idx Ҫ��������idx��������������������Ǽ��������ͨ��getPassedDays()���������
INT16U make_meter_data_from_flash_hold(INT32U *master_oad,INT8U hold_list_idx,INT32U *oad,INT8U oad_cnt,INT32U read_idx,INT8U *buffer,INT8U *time,INT8U *task_start_time_frq)  //���ǵ��������ݵ����⡣
{
	INT8U pot_idx =0;
	INT8U oad_idx =0;
	tagREADITEM read_item;
	INT8U data[100] ={0};
	INT16U valide_data_length =0;
	INT8U data_pos = 0,data_len =0;
	INT8U oad_tmp[4],dividePhaseNum =0,dividePhaseIdx = 0;
	INT8U dt[7] ={0}; //�ñ����Ǵ�flash���õõ���ʱ�䡣
	volatile INT8U task_idx =0,task_id =0;
	tagDatetime dt_flash,dt_time; //ͨ��������ʱ���������Ƿ�����Чʱ����
	INT32U pass_minute_flash = 0,pass_minute_time =0;
	INT32U valide_time =0;  //��λ���ӣ������ж���Чʱ���
	static INT32U tmp =0;
	INT16U bufferPos =0;
#ifdef	__PROVINCE_JIBEI_FUHEGANZHI__
	INT8U equipment_cnt =0,equipment_idx =0;
	INT8U *realequipmentCntInMeter;

#endif
	//����master_oad�������ǰ������
	for(task_idx =0;task_idx<MAX_TASK_COUNT;task_idx++)
	{
		tmp = *((INT32U*)&(gTaskPlanAttribute[task_idx].master_oad));
		if( tmp == *master_oad) //�ҵ�����oad
		{
			switch (gTaskPlanAttribute[task_idx].run_unit)
			{
				case 1: //��
					valide_time = gTaskPlanAttribute[task_idx].run_frq;
					break;
				case 2://ʱ
					valide_time = gTaskPlanAttribute[task_idx].run_frq*60;
					break;
				case 3://��
					valide_time = gTaskPlanAttribute[task_idx].run_frq*60*24;
					break;
				case 4://��
					valide_time = gTaskPlanAttribute[task_idx].run_frq*60*24*30;
					break;
				case 5://��
					valide_time = gTaskPlanAttribute[task_idx].run_frq*60*24*365;
					break;
					
			}
			task_id =task_idx;
			break;
		}
	}
	for(oad_idx =0;oad_idx<oad_cnt;oad_idx++)   //����Ҫ���ٸ�oad�� read_item.offset
	{
		//����ٲ����ݶ���ʱ�䣬��ô�Լ�������Ϊflash��û�д洢
		{
			if(*(oad+oad_idx) ==0x00022120)   //����ʱ��
			{
				*(buffer+bufferPos++) = DT_DATETIME_S;
				mem_cpy(buffer+bufferPos+1,time,7);
				bufferPos += 7;
				continue;
			}
		}
#ifdef 	__PROVINCE_JIBEI_FUHEGANZHI__
		if((*((INT8U*)(oad)+5*oad_idx) ==0x00) && ( *((INT8U*)(oad)+5*oad_idx+1) ==0x46))    //������ô����Ϊ�˷��ն�50020200 ���Ӷ���ı��ġ�
		{
			mem_cpy(oad_tmp,(INT8U*)(oad)+5*oad_idx+1,4);
			equipment_cnt = oad_tmp[3] ;  //���ǰѷ������豸�����ᵽǰ��������
			//if(oad_tmp[3]!=0x00)
			//{
				//oad_tmp[3] =0x00;
			//} 
		}
		else
#endif	
		{   //������Ҫ����oadת�������⣬��Ϊ��վ�·��ı��Ŀ����Ƿ������ݣ���ô�ȶ����ܵ����ݣ�Ȼ���ٿ��Ƿ������ݡ�
			mem_cpy(oad_tmp,(INT8U*)(oad+oad_idx),4);
#ifdef	__PROVINCE_JIBEI_FUHEGANZHI__   
			equipment_cnt = oad_tmp[3] ;   //���ǰѷ������豸�����ᵽǰ��������
#endif			
			if(oad_tmp[3]!=0x00)
			{
				oad_tmp[3] =0x00;
			} 
		}//
		mem_set(data,100,0x00);
		
		if(oad_search_in_hold_list(hold_list_idx,oad_tmp,&read_item))
		{
			read_flash_data_from_save_idx(read_idx,read_item.offset , data, 100,
					read_item.block_data_len, FLASH_SECTOR_METER_DATA_START + read_item.page_id, read_item.page_count);
		}							
										
#ifdef	__PROVINCE_JIBEI_FUHEGANZHI__
	    if((oad_tmp[0]==0x46) && (oad_tmp[1] ==0x00))
		{
			oad_search_in_hold_list_2(oad_tmp,&read_item);
//			equipment_cnt = oad_tmp[3] ; //*((INT8U*)(oad+oad_idx)+3) ;
			if(equipment_cnt ==0)  //������Ҫ�������豸�����ݡ�
			{
				equipment_idx =0;
				equipment_cnt =ELECTRICAL_EQUIPMENT_NUM-1;
				*(buffer+ bufferPos++) = 0x01;
//				*(buffer++) = ELECTRICAL_EQUIPMENT_NUM;
				*(buffer+ bufferPos) =0;
				realequipmentCntInMeter = buffer+ (bufferPos++);
			}
			else
			{
				equipment_idx =equipment_cnt ;
				*(buffer+ bufferPos++) = 0x01;
				*(buffer+ bufferPos++) = 0x01;
			}	
			mem_set(data,100,0x00);
			for(equipment_idx ;equipment_idx<equipment_cnt+1;equipment_idx ++)
			{
				read_flash_data_from_save_idx(read_idx,read_item.offset , data, 100,read_item.block_data_len, SECTOR_ELECTRICAL_EQUIPMENT_MINUTE_DATA_START + read_item.page_id + equipment_idx*MAX_SECTOR_ELECTRICAL_EQUIPMENT_MINUTE_DATA, read_item.page_count);
				//�ж�ʱ����Ч�ԣ�˵�����ص���������ȷ�����١����ǵ�����ʱ���Ƿ�������ʱ�䡣 getPassedMinutes(tagDatetime *dt,INT16U centry)
				mem_cpy(dt,data+5,7);
				change_time_forme(dt,&dt_flash);
				change_time_forme(time,&dt_time);
				pass_minute_flash = getPassedMinutes(&dt_flash,2000);
				pass_minute_time = getPassedMinutes(&dt_time,2000);
				if(!(abs(pass_minute_time - pass_minute_flash)<=30))
				{
					continue;
				}//

				data_pos =DATA_HEADER_SIZE;
				if(data[data_pos++] ==0x01)  //0x01 ���������ݡ�
				{
					*realequipmentCntInMeter = *realequipmentCntInMeter+1 ;
					mem_cpy(task_start_time_frq,data+5,7);  //��������������ʱ�䴫���ϲ㡣					
					data_len = get_object_attribute_item_len(data+data_pos,data[data_pos]);
					mem_cpy(buffer+bufferPos,data+data_pos,data_len);
					valide_data_length += data_len;
					bufferPos += data_len;					
				}
				else   //û������
				{
					*(buffer+ bufferPos++) =0x00;
					valide_data_length++;
				}					
			}
//		return valide_data_length;
		}
		else
		{
#endif 										
		//�ж�ʱ����Ч�ԣ�˵�����ص���������ȷ�����١����ǵ�����ʱ���Ƿ�������ʱ�䡣 getPassedMinutes(tagDatetime *dt,INT16U centry)
		mem_cpy(dt,data+5,7);
		change_time_forme(dt,&dt_flash);
		change_time_forme(time,&dt_time);
		pass_minute_flash = getPassedMinutes(&dt_flash,2000);
		pass_minute_time = getPassedMinutes(&dt_time,2000);
		if(!(abs(pass_minute_time - pass_minute_flash)<=valide_time))
		{
			break;
		}//

		data_pos =DATA_HEADER_SIZE;
		if(data[data_pos++] ==0x01)  //0x01 ���������ݡ�
		{
			mem_cpy(task_start_time_frq,data+5,7);  //��������������ʱ�䴫���ϲ㡣
			mem_cpy(oad_tmp,(INT8U*)(oad+oad_idx),4);
			if(oad_tmp[3]==0x00)
			{
				data_len = get_object_attribute_item_len(data+data_pos,data[data_pos]);
				mem_cpy(buffer+bufferPos,data+data_pos,data_len);
				valide_data_length += data_len;
				bufferPos += data_len;
			}
			else  //�Է������ݽ��д���
			{
				if(data[data_pos++] ==0x01)
				{
					dividePhaseNum = data[data_pos++];
					data_len = get_object_attribute_item_len(data+data_pos,data[data_pos]);
					if(oad_tmp[3]>dividePhaseNum)
					{
						oad_tmp[3] =0x1;
					}
					data_pos += data_len*(oad_tmp[3] -1);
					
					mem_cpy(buffer+bufferPos,data+data_pos,data_len);
					valide_data_length += data_len;
					bufferPos += data_len;
				}
			}
		}
		else   //û������
		{
			*(buffer+ bufferPos++) =0x00;
			valide_data_length++;
		}	
#ifdef 	__PROVINCE_JIBEI_FUHEGANZHI__
		}
#endif
	}
	return valide_data_length;
}
// read_idx Ҫ��������idx��������������������Ǽ��������ͨ��getPassedDays()���������
INT16U make_meter_data_from_flash_cur(INT8U hold_list_idx,INT32U *oad,INT8U oad_cnt,INT32U read_idx,INT8U *buffer,INT8U *time,INT8U *task_start_time_frq)  //���ǵ��������ݵ����⡣
{
	INT8U pot_idx =0;
	INT8U oad_idx =0;
	tagREADITEM read_item;
	INT8U data[200] ={0};
	INT16U valide_data_length =0;
	INT8U data_pos = 0,data_len =0;
	INT8U oad_tmp[4],dividePhaseNum =0,dividePhaseIdx = 0;
	INT8U dt[7] ={0}; //�ñ����Ǵ�flash���õõ���ʱ�䡣
	volatile INT8U task_idx =0,task_id =0;
	tagDatetime dt_flash,dt_time; //ͨ��������ʱ���������Ƿ�����Чʱ����
	INT32U pass_minute_flash = 0,pass_minute_time =0;
	INT32U valide_time =0;  //��λ���ӣ������ж���Чʱ���
	static INT32U tmp =0;
	INT16U bufPos =0;

	for(oad_idx =0;oad_idx<oad_cnt;oad_idx++)   //����Ҫ���ٸ�oad�� read_item.offset
	{
		//����ٲ����ݶ���ʱ�䣬��ô�Լ�������Ϊflash��û�д洢
		//{
			//if(*(oad+oad_idx) ==0x00022120)   //����ʱ��
			//{
				//*buffer = DT_DATETIME_S;
				//mem_cpy(buffer+1,time,7);
				//buffer += 8;
				//continue;
			//}
		//}
		
		{   //������Ҫ����oadת�������⣬��Ϊ��վ�·��ı��Ŀ����Ƿ������ݣ���ô�ȶ����ܵ����ݣ�Ȼ���ٿ��Ƿ������ݡ�
			mem_cpy(oad_tmp,((INT8U*)oad+5*oad_idx+1),4);
			if(oad_tmp[3]!=0x00)
			{
				oad_tmp[3] =0x00;
			}
		}//
		
		if(!oad_search_in_cur_list(0,oad_tmp,&read_item))
		{
			continue;
		}
		
		mem_set(data,200,0x00);
		
    	//oad_search_in_hold_list(hold_list_idx,oad_tmp,&read_item);
		//read_flash_data_from_save_idx(read_idx,read_item.offset , data, 200,
		//read_item.block_data_len, FLASH_SECTOR_METER_DATA_START + read_item.page_id, read_item.page_count);
		
		read_flash_data(gSystemInfo.meter_no,read_item.offset,data,200,read_item.block_data_len, FLASH_SECTOR_METER_DATA_START + read_item.page_id, read_item.page_count);
		//�ж�ʱ����Ч�ԣ�˵�����ص���������ȷ�����١����ǵ�����ʱ���Ƿ�������ʱ�䡣 getPassedMinutes(tagDatetime *dt,INT16U centry)

		mem_cpy(task_start_time_frq,data+5,7);
		mem_cpy(time,data+5,7);
		data_pos =DATA_HEADER_SIZE;
		if(data[data_pos++] ==0x01)  //0x01 ���������ݡ�
		{
			mem_cpy(oad_tmp,((INT8U*)oad+5*oad_idx+1),4);
			if(oad_tmp[3]==0x00)
			{
				data_len = get_object_attribute_item_len(data+data_pos,data[data_pos]);
				mem_cpy(buffer+bufPos,data+data_pos,data_len);
				valide_data_length += data_len;
				bufPos += data_len;
			}
			else  //�Է������ݽ��д���
			{
				if(data[data_pos++] ==0x01)
				{
					dividePhaseNum = data[data_pos++];
					data_len = get_object_attribute_item_len(data+data_pos,data[data_pos]);
					if(oad_tmp[3]>dividePhaseNum)
					{
						oad_tmp[3] =0x1;
					}
					data_pos += data_len*(oad_tmp[3] -1);
					
					mem_cpy(buffer + bufPos,data+data_pos,data_len);
					valide_data_length += data_len;
					bufPos += data_len;
				}
			}
		}
		else   //û������
		{
			*(buffer+bufPos++) =0x00;
			valide_data_length++;
		}
	}
	return valide_data_length;
}
//��������ͨ��idx
INT16U make_meter_data_from_flash(INT32U *master_oad,INT32U *oad,INT8U oad_cnt,INT8U *time,INT8U *buffer,INT8U *task_start_time_frq,INT8U *DayHoldFlag,INT8U *data_type,INT8U *is_cur_data_req)
{
	INT32U read_idx;
	INT16U obis;	
	INT16U data_offset =0;
	INT8U idx_list =0,hold_list_idx =0;
	INT16U valide_data_length =0;
	INT8U hold_time[7];
	{
		hold_time[YEAR] = ((*time<<8)+*(time+1))-2000;
		hold_time[MONTH] = *(time+2);
		hold_time[DAY] = *(time+3);
		hold_time[HOUR] = *(time+4);
		hold_time[MINUTE] =*(time+5);
	}
	INT16U master_oad_trans = (*master_oad !=NULL)?(*(INT8U*)master_oad <<8) +(*((INT8U*)master_oad+1)):0x0;
	
 	read_idx = getPassedDays(2000+hold_time[YEAR],hold_time[MONTH],hold_time[DAY]);
	
	/* find the table */
	for(idx_list=0;idx_list<sizeof(hold_data_list)/sizeof(READ_LIST_PROP);idx_list++)   //����Ӵ���еõ������λ��
	{
		if(master_oad_trans == hold_data_list[idx_list].obis )
		{
			hold_list_idx = idx_list;
			break;
		}
		
	}
    if (hold_list_idx >= HOLD_LIST_COUNT) return;
	
		
	switch(master_oad_trans)
	{
		case 0x5002:  //���Ӷ���
			read_idx *= 96;
			read_idx += hold_time[HOUR] *4;
			read_idx += hold_time[MINUTE] /15;
			valide_data_length = make_meter_data_from_flash_hold(master_oad,hold_list_idx,oad,oad_cnt,read_idx,buffer,time,task_start_time_frq);
			break;
		case 0x5003:  //Сʱ����
			
			break;
		case 0x5004:  //�ն���
			valide_data_length = make_meter_data_from_flash_hold(master_oad,hold_list_idx,oad,oad_cnt,read_idx,buffer,time,task_start_time_frq);
			*DayHoldFlag =1;
			break;
		default:       //ʵʱ����
			valide_data_length = make_meter_data_from_flash_cur(hold_list_idx,oad,oad_cnt,read_idx,buffer,time,task_start_time_frq);
			*data_type =2;
			*is_cur_data_req =1;
			break;
			break;
	}


	return valide_data_length;
	//obis =  0x5002 ;
	//switch(obis)
	//{
	//case 0x5002: /* ���� */
		//save_idx *= 96;
		//save_idx += hold_time[HOUR] *4;
		//save_idx += hold_time[MINUTE] /15;
		//break;
	//case 0x5003: /* HOUR */
		//save_idx *= 24;
		//save_idx += hold_time[HOUR];
		//break;
	//case 0x5005: /* �� */
		//save_idx += hold_time[MONTH];
		//break;
	//default:
		//break;
	//}
	//
	//read_flash_data_from_save_idx(save_idx, data_offset, data, save_len, 
								 //params->save_size, read_param->start_page+params->page_id, params->page_cnt);
}