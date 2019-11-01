#include "oop_type_def.h"
#include "object_describe.h"
#include "object_obis.h"

#define OBIS_SIZE_GPRS                    (150+60+86+50+20+30+20+3+20+20)
#define OBIS_SIZE_ADDR                    10
#define OBIS_SIZE_CUSTOM_INFO             100
#define OBIS_SIZE_ESAM_PARAM              20

const tagObject_Index OAD_GPRS_PARAM[]=
{
 /*���Ա��*/                  /*��������*/
{OA_WORK_MODE              ,	DT_ENUM             , 2  },//2������ģʽ
{OA_CONNECT_KIND           ,	DT_ENUM             , 2  },//2�����ӷ�ʽ
{OA_ONLINE_KIND            ,	DT_ENUM             , 2  },//2�����߷�ʽ
{OA_CONNECT_USE_MODE       ,	DT_ENUM             , 2  },//2�����ӷ�ʽӦ��ģʽ
{OA_LISTEN_PORT_LIST       ,    DT_ARRAY            , 23 },//23�������˿��б�
{OA_APN                    ,	DT_VISIBLESTRING    , 34 },//34��APN
{OA_USER                   ,	DT_VISIBLESTRING    , 34 },//34���û���
{OA_PASSWORD               ,	DT_VISIBLESTRING    , 34 },//34������
{OA_PROXY_IP               ,	DT_OCTETSTRING      , 8  },//8�������������ַ
{OA_PROXY_PORT             ,	DT_LONGUNSIGNED     , 3  },//3������˿�
{OA_TIMEOUT_RESEND_COUNT   ,	DT_UNSIGNED         , 3  },//3����ʱʱ�估�ط�����
{OA_HEART_CYCLE            ,	DT_LONGUNSIGNED     , 3  },//3����������
};

const tagObject_Index OAD_SMS_PARAM[]=
{
 /*���Ա��*/                  /*��������*/
{OA_SMS_CENTER_NO           ,	DT_VISIBLESTRING     , 18  },//18���������ĺ���
{OA_MSA_SIM_NO              ,	DT_ARRAY             , 34  },//38����վ����
{OA_DEST_NO                 ,	DT_ARRAY             , 34  },//������֪ͨĿ�ĺ���
};

//��վͨѶ����
const tagObject_Attribute OAD_MASTER_IP[]=
{
 /*���Ա��*/                  /*��������*/
{OA_MASTER_IP                       ,	DT_OCTETSTRING             },//8��IP
{OA_MASTER_PORT                     ,	DT_LONGUNSIGNED            },//3���˿�
};
INT16U get_OAD_MASTER_IP_size(void)
{
    return sizeof(OAD_MASTER_IP);
}

//���߹���ͨ�����Ա�
const tagObject_Attribute OAD_GPRS[]=
{
 /*���Ա��*/                  /*��������*/
{OA_GPRS_PARAM                       ,	DT_STRUCTURE        ,  150 ,   (tagObject_Index*)OAD_GPRS_PARAM ,sizeof(OAD_GPRS_PARAM)    ,0},//150��ͨ������
{OA_GPRS_MASTER_PARAM                ,  DT_ARRAY            ,  60  ,   NULL                             ,0                         ,0  },//60����վͨ�Ų�����
{OA_SMS_PARAM                        ,	DT_STRUCTURE        ,  86  ,   (tagObject_Index*)OAD_SMS_PARAM  ,sizeof(OAD_SMS_PARAM)     ,0  },//200������ͨ�Ų���
{OA_GPRS_VER                         ,	DT_STRUCTURE        ,  50  ,   NULL                             ,0                         ,READ_ONLY  },//50���汾��Ϣ
{OA_ALLOW_PROTOCOL                   ,	DT_VISIBLESTRING    ,  20  ,   NULL                             ,0                         ,0  },//20��֧�ֹ�Լ�б�
{OA_GRPS_CCID                        ,	DT_VISIBLESTRING    ,  30  ,   NULL                             ,0                         ,0  },//30��SIM����ICCID
{OA_GRPS_IMSI                        ,	DT_VISIBLESTRING    ,  20  ,   NULL                             ,0                         ,0  },//20��IMSI
{OA_GPRS_CSQ                         ,	DT_LONG             ,  3   ,   NULL                             ,0                         ,0  },//3���ź�ǿ��
{OA_GPRS_SIM_ID                      ,	DT_VISIBLESTRING    ,  20  ,   NULL                             ,0                         ,0  },//20��sim������
{OA_GPRS_IP                          ,	DT_OCTETSTRING      ,  20  ,   NULL                             ,0                         ,0  },//20������ip
};
#ifdef __PROVICE_SHANDONG__
const tagObject_Attribute OA_COMM_ADDR[]=
{
{OA_METER_ADDR                       ,	DT_OCTETSTRING     ,  10   ,   NULL                             ,0                         ,0  },//10������ַ
};
#else
const tagObject_Attribute OA_COMM_ADDR[]=
{
{OA_METER_ADDR                       ,	DT_OCTETSTRING     ,  10   ,   NULL                             ,0                         ,READ_ONLY  },//10������ַ
};
#endif
const tagObject_Attribute OA_CUSTOM_INFO[]=
{
{OA_CFG_VENDO                        ,	DT_OCTETSTRING     ,  4   ,   NULL                             ,0                         ,0  },//4����־����
{OA_FRAME_TYPE                       ,	DT_OCTETSTRING     ,  4   ,   NULL                             ,0                         ,0  },//4����־����
};
const tagObject_Attribute OA_ESAM_PARAM[]=
{
{OA_SAFE_MODE                        ,  DT_ENUM            ,  2   ,   NULL                             ,0                         ,0  },//2���Ƿ�����ȫģʽ
{OA_READ_MODE                        ,  DT_ENUM            ,  2   ,   NULL                             ,0                         ,0  },//2���Ƿ���ESAM����ģʽ
};


const tagParamObject   PARAM_OBJ_LIST[]=
{
{OBIS_GPRS1             ,OBIS_SIZE_GPRS             ,0   ,(tagObject_Attribute*)OAD_GPRS           ,sizeof(OAD_GPRS)       },  //GPRS��Ϣ
{OBIS_COMM_ADDR         ,OBIS_SIZE_ADDR             ,0   ,(tagObject_Attribute*)OA_COMM_ADDR       ,sizeof(OA_COMM_ADDR)   },  //����ַ
{OBIS_CUSTOM_INFO       ,OBIS_SIZE_CUSTOM_INFO      ,0   ,(tagObject_Attribute*)OA_CUSTOM_INFO     ,sizeof(OA_CUSTOM_INFO) },  //�����Զ������
{OBIS_ESAM_PARAM        ,OBIS_SIZE_ESAM_PARAM       ,0   ,(tagObject_Attribute*)OA_ESAM_PARAM      ,sizeof(OA_ESAM_PARAM)  },  //ESAM ����
	
};

BOOLEAN get_param_object(INT16U obis,tagParamObject *object)
{
    INT16U idx;

    for(idx=0;idx<sizeof(PARAM_OBJ_LIST)/sizeof(tagParamObject);idx++)
    {
        if(PARAM_OBJ_LIST[idx].obis==obis)
        {
            mem_cpy(object,(void*)&PARAM_OBJ_LIST[idx],sizeof(tagParamObject));
            return TRUE;
        }
    }
    return FALSE;
}

INT32U get_object_offset(INT16U obis)
{
    INT32U size;
    INT16U idx;

    size=0;
    for(idx=0;idx<sizeof(PARAM_OBJ_LIST)/sizeof(tagParamObject);idx++)
    {
        if(PARAM_OBJ_LIST[idx].obis==obis)
        {
            return size;
        }
        size+=PARAM_OBJ_LIST[idx].attri_max_len;
    }
    return size;
}

BOOLEAN get_param_attribute(INT8U attri_id,const tagObject_Attribute *attr_list,INT8U attr_list_size,tagObject_Attribute *attri)
{
    INT8U idx;

	for(idx=0;idx<attr_list_size/sizeof(tagObject_Attribute);idx++)
	{
		if(attr_list[idx].Attri_Id == attri_id)
        {
            attri->Attri_Id=attr_list[idx].Attri_Id;
            attri->data_type=attr_list[idx].data_type;
            attri->len=attr_list[idx].len;
            attri->index_list=attr_list[idx].index_list;
            attri->index_list_size=attr_list[idx].index_list_size;
            attri->right=attr_list[idx].right;
            return TRUE;
        }
	}
    return FALSE;
}

INT16U get_attribute_offset(const tagObject_Attribute *attr_list,INT8U attr_list_size,INT8U attri_id)
{
	INT16U offset;
	INT8U idx;

	offset=0;
	for(idx=0;idx<attr_list_size/sizeof(tagObject_Attribute);idx++)
	{
		//if(((tagObject_Attribute*)&attr_list[idx])->Attri_Id == attri_id)
        if(attr_list[idx].Attri_Id==attri_id)
        {
            return offset;
        }
        offset += ((tagObject_Attribute*)&attr_list[idx])->len;
	}
	return offset;	
}

INT16U get_index_offset(const tagObject_Index *index_list,INT8U index,INT8U count)
{
	INT16U offset;
	INT8U idx;

	offset=0;
	for(idx=0;idx<count;idx++)
	{
		if(((tagObject_Index*)&index_list[idx])->index == index)
        {
            return offset;
        }
        offset += ((tagObject_Index*)&index_list[idx])->len;
	}
	return offset;
}

INT8U get_data_type_len(INT8U data_type)
{
    switch(data_type)
    {
    case DT_BOOLEAN:
    case DT_INTEGER:
    case DT_UNSIGNED:
    case DT_ENUM:
        return sizeof(COSEM_BOOLEAN);
    case DT_DOUBLELONG:
    case DT_DOUBLELONGUNSIGNED:
    case DT_FLOAT32:
        return sizeof(COSEM_UNSIGNED32);
    case DT_LONG:
    case DT_LONGUNSIGNED:
        return sizeof(COSEM_SIGNED16);
    case DT_LONG64:
    case DT_LONG64UNSIGNED:
    case DT_FLOAT64:
        return sizeof(COSEM_FLOAT64);
    case DT_DATETIME:
        return 1+12;
    case DT_DATE:
        return 1+5;
    case DT_TIME:
        return 1+4;
    case DT_OAD:
    case DT_OMD:
        return 1+4;
    default:
        return 0;
    }
    return 0;
}

INT16U calculate_ms_size(INT8U *data,INT8U data_type)
{
    INT16U size=0,count;
    INT16U idx;
    
    if(data[2] & 0x80)
    {
        count=cosem_bin2_unit(data+3,data[2]& 0x7F);//��ȡ����
        for(idx=0;idx<count;idx++)
        {
            size+=get_oop_data_type_len(data+3+(data[2]& 0x7F)+size,data_type);
        }
        size+=(3+(data[2]& 0x7F));
    }
    else
    {
      for(idx=0;idx<data[2];idx++)
      {
          size+=get_oop_data_type_len(data+3+size,data_type);
      }
      size+=3;
    }
    return size;
}
INT16U calculate_ms_size_without_type(INT8U *data,INT8U data_type)
{
    INT16U size=0,count;
    INT16U idx;
    
    if(data[1] & 0x80)
    {
        count=cosem_bin2_unit(data+2,data[1]& 0x7F);//��ȡ����
        for(idx=0;idx<count;idx++)
        {
            size+=get_oop_data_type_len(data+2+(data[1]& 0x7F)+size,data_type);
        }
        size+=(2+(data[1]& 0x7F));
    }
    else
    {
        for(idx=0;idx<data[1];idx++)
        {
            size+=get_oop_data_type_len(data+2+size,data_type);
        }
        size+=2;
    }
    return size;
}
//��ȡ�����������ݳ���
//data              ����
//data_type         ��������
//����              ���ݳ���
INT16U get_object_attribute_item_len(INT8U *data,INT8U data_type)
{
    INT32U len;
    INT16U idx,size;
    INT16U num,count;
    if(data[0]!=data_type)
    {
        return 0;
    }
    len=0;
    switch(data_type)
    {
        case DT_DONT_CARE:
        case DT_NULL:   return 1;
        case DT_INTEGER:
        case DT_UNSIGNED:
        case DT_ENUM:
        case DT_BOOLEAN: return 2;
        case DT_LONG:
        case DT_LONGUNSIGNED:
        case DT_SCALER_UNIT:
        case DT_OI:           return 3;
        case DT_TI:
        case DT_TIME:         return 4;
        case DT_FLOAT32:
        case DT_OAD:
        case DT_OMD:
        case DT_DOUBLELONG:
        case DT_DOUBLELONGUNSIGNED: return 5;
        case DT_LONG64:
        case DT_FLOAT64:
        case DT_LONG64UNSIGNED: return 9;
        case DT_DATETIME:  return 11;
        case DT_COMDCB:
        case DT_DATE:      return 6;
        case DT_DATETIME_S:    return 8;
        //����Ϊ�䳤����Ҫ�����ݹ����
        case DT_ARRAY:  //������Ҫ���ǳ���Ϊ����ֽڵ����
            if(data[1] & 0x80)
            {
                num=cosem_bin2_unit(data+2,data[1]& 0x7F);
                data+=(data[1]& 0x7F)+2;
                len+=(data[1]& 0x7F)+2;
            }
            else
            {
                num=data[1];
                data+=2;
                len+=2;
            }
            for(idx=0;idx<num;idx++)
            {
                size=get_object_attribute_item_len(data,data[0]);
                if(size==0)//���͸�ʽ����ȷ
                {
                    return 0;
                }
                len+=size;
                data+=size;        
            }
            return len;
        case DT_STRUCTURE:
            num=data[1];
            data+=2;
            len+=2;
            if(num==0xFF)num=0;
            for(idx=0;idx<num;idx++)
            {
                size=get_object_attribute_item_len(data,data[0]);
                if(size==0)//���͸�ʽ����ȷ
                {
                    return 0;
                }
                len+=size;
                data+=size;
            }
            return len;
        case DT_BITSTRING:
            num=data[1]/8;
            if((data[1]%8) !=0 ) num++;
            return (num+2);
        case DT_EVENTMASK:
            num=data[2]/8;
            if((data[2]%8) !=0 ) num++;
            return (num+3);
        case DT_OCTETSTRING:
        case DT_VISIBLESTRING:
        case DT_UTF8STRING:
        case DT_TSA:
        case DT_MAC:
        case DT_RN:
            if(data[1] & 0x80)
            {
                num=cosem_bin2_unit(data+2,data[1]& 0x7F);
                num+=(data[1]& 0x7F)+2;
            }
            else
            {
                num=data[1]+2;
            }
            return num;
            //break;
        case DT_REGION://{���͡�����1������2}
            len+=2;
            data+=2;
            for(idx=0;idx<2;idx++)
            {
                size=get_object_attribute_item_len(data,data[0]);
                if(size==0)//���͸�ʽ����ȷ
                {
                    return 0;
                }
                len+=size;
                data+=size;
            }
            return len;
        case DT_ROAD:
            return data[5]*4+6;
        case DT_CSD:
            if(data[1]==0)
            {
                return 4+2;
            }
            else if(data[1]==1)
            {
                return data[6]*4+4+3;
            }
            else
            {
                return 0;
            }
            break;
        case DT_RCSD:
            size=2;
            for(idx=0;idx<data[1];idx++)
            {
                if(data[size]==0)
                {
                    size+=5;
                }
                else if(data[size]==1)
                {
                    size+=data[size+5]*4+6;
                }
                else
                {
                    return 0;
                }
            }
            return size;
            //break;
        case DT_RSD:
            size=2;
            switch(data[1])
            {
                case 0:
                    break;
                case 1: //��������������  OAD+DATA
                    size+=4;
                    len=get_object_attribute_item_len(data+size,data[size]);
                    if(len==0)//���͸�ʽ����ȷ
                    {
                        return 0;
                    }
                    size+=len;
                    break;
                case 2: //��������������  OAD+DATA+DATA+DATA
                    size+=4;
                    len=get_object_attribute_item_len(data+size,data[size]);
                    if(len==0)//���͸�ʽ����ȷ
                    {
                        return 0;
                    }
                    size+=len;
                    len=get_object_attribute_item_len(data+size,data[size]);
                    if(len==0)//���͸�ʽ����ȷ
                    {
                        return 0;
                    }
                    size+=len;
                    len=get_object_attribute_item_len(data+size,data[size]);
                    if(len==0)//���͸�ʽ����ȷ
                    {
                        return 0;
                    }
                    size+=len;
                    break;
                case 3:
                    count=data[size++];
                    for(idx=0;idx<count;idx++)
                    {
                        size+=4;
                        len=get_object_attribute_item_len(data+size,data[size]);
                        if(len==0)//���͸�ʽ����ȷ
                        {
                            return 0;
                        }
                        size+=len;
                        len=get_object_attribute_item_len(data+size,data[size]);
                        if(len==0)//���͸�ʽ����ȷ
                        {
                            return 0;
                        }
                        size+=len;
                        len=get_object_attribute_item_len(data+size,data[size]);
                        if(len==0)//���͸�ʽ����ȷ
                        {
                            return 0;
                        }
                        size+=len;
                    }
                    break;
                case 4:
                case 5:
                   size+=7;//DateTime_S,������Լ���������ݵĸ�ʽ�Ƿ���ȷ
                   size+=get_oop_data_type_len(data+size,DT_MS);//MS
                   break;
                case 6:
                case 7:
                case 8:
                   size+=7;//DateTime_S,������Լ���������ݵĸ�ʽ�Ƿ���ȷ
                   size+=7;//DateTime_S,������Լ���������ݵĸ�ʽ�Ƿ���ȷ
                   size+=3;//TI
                   size+=get_oop_data_type_len(data+size,DT_MS);//MS
                   break;
               case 9:
                   size++;//unsigned
                   break;
               case 10:
                   size++;//unsigned
                   size+=get_oop_data_type_len(data+size,DT_MS);//MS
                   break;
            }
            return size;
        case DT_MS:
            size=0;
            switch(data[1])
            {
                case 0:     //�޵��ܱ�
                    size=2;
                    break;
                case 1:    // ȫ���û���ַ
                    size=2;
                    break;
                case 2:   //һ���û�����
                    size = calculate_ms_size(data,DT_UNSIGNED);
                    break;
                case 3:  //һ���û���ַ
                    size = calculate_ms_size(data,DT_TSA);
                    break;
                case 4:   //һ���������
                    size = calculate_ms_size(data,DT_LONGUNSIGNED);
                    break;
                case 5:  //TODOһ���û���������
                case 6:  //TODOһ���û���ַ����
                case 7:  //TODOһ������������� 
                    size = calculate_ms_size(data,DT_REGION);
                    break;
                default:
                    size=0;
                    break;
            }
            return size;
            //break;
        case DT_SID:
            size=5;
            size+=data[size]+1;
            return size;       
            //break;
        case DT_SID_MAC:
            size=5;
            size+=data[size]+1;
            size+=data[size]+1;
            return size;
            //break;
        default:
            len=0;
            break;
    }
    return len;
}
//��ȡ����������ĳ��Ԫ�ص�������Ϣ
//data          ����������
//data_type     ��������
//only_value    �Ƿ�ֻ��ȡ���ݱ�־,������·��ص����ݽ�������������һ���ֽ�
//find_index    ��ȡԪ������
//retdata       ��������
//attr_head_size    ������������ͷ��С
INT16U get_object_attribute_item_value(INT8U *data,INT8U data_type,BOOLEAN only_value,INT8U find_index,INT8U **retdata,INT8U *attr_head_size)
{
    INT32U len;
    INT16U idx,size,head_size;
    INT16U num,count;
    if(data[0]!=data_type)
    {
        return 0;
    }
    len=0;
    *attr_head_size=0;
    head_size = 0;
    size = 0;
    *retdata=NULL;
    
    switch(data_type)
    {
        case DT_DONT_CARE:
        case DT_NULL:
            head_size =1;
            size=0;
            break;
        case DT_INTEGER:
        case DT_UNSIGNED:
        case DT_ENUM:
        case DT_BOOLEAN:
            head_size =1;
            size=1;
            break;
        case DT_LONG:
        case DT_LONGUNSIGNED:
        case DT_SCALER_UNIT:
        case DT_OI:
            head_size =1;
            size=2;
            break;
        case DT_TI:
        case DT_TIME:
            head_size =1;
            size=3;
            break;
        case DT_FLOAT32:
        case DT_OAD:
        case DT_OMD:
        case DT_DOUBLELONG:
        case DT_DOUBLELONGUNSIGNED:
            head_size =1;
            size=4;
            break;
        case DT_LONG64:
        case DT_FLOAT64:
        case DT_LONG64UNSIGNED:
            head_size =1;
            size=8;
            break;
        case DT_DATETIME:
            head_size =1;
            size=10;
            break;
        case DT_COMDCB:
        case DT_DATE:
            head_size =1;
            size=5;
            break;
        case DT_DATETIME_S:
            head_size =1;
            size=7;
            break;
        //����Ϊ�䳤����Ҫ�����ݹ����
        case DT_ARRAY:  //ʹ��index����������Ч���������Ϊ�˻�ȡ����
            if(data[1] & 0x80)
            {
                head_size=(data[1]& 0x7F)+2;
                num=cosem_bin2_unit(data+2,data[1]& 0x7F);
                data+=head_size;
                len+=head_size;
                head_size=1;
            }
            else
            {

                num=data[1];
                data+=2;
                len+=2;
                head_size=1;
            }
            for(idx=0;idx<num;idx++)
            {
                    size=get_object_attribute_item_value(data,data[0],only_value,0xFF,retdata,attr_head_size);
                if((idx==find_index) && (find_index!=0xFF))
                {
                    *retdata=data;
                    if((only_value) && (*attr_head_size>0) && (size>*attr_head_size))
                    {
                        *retdata+=*attr_head_size;
                        size-=*attr_head_size;
                    }
                    return size;
                }
                len+=size;
                data+=size;        
            }
            *attr_head_size=head_size;
            return len;
        case DT_STRUCTURE://ʹ��index�����Ի�ȡ�ṹ���Ա��
            head_size=1;
            num=data[1];
            data+=2;
            len+=2;
            if(num==0xFF)num=0;
            for(idx=0;idx<num;idx++)
            {
                size=get_object_attribute_item_value(data,data[0],only_value,0xFF,retdata,attr_head_size);
                if((idx==find_index) && (find_index!=0xFF))
                {
                    *retdata=data;
                    if((only_value) && (*attr_head_size>0) && (size>=*attr_head_size))
                    {
                        *retdata+=*attr_head_size;
                        size-=*attr_head_size;
                    }
                    return size;
                }
                len+=size;
                data+=size;        
            }
            *attr_head_size=head_size;
            return len;
        case DT_BITSTRING:
            head_size =2;
            size=data[1]/8;
            if(data[1]%8!=0)size++;
            break;
        case DT_EVENTMASK:
            head_size =1;
            size=data[2]/8+2;
            if(data[2]%8!=0)size++;
            break;
        case DT_OCTETSTRING:
        case DT_VISIBLESTRING:
        case DT_UTF8STRING:
        case DT_TSA:
        case DT_MAC:
        case DT_RN:
            if(data[1] & 0x80)
            {
                size=cosem_bin2_unit(data+2,data[1]& 0x7F);
                head_size=(data[1]& 0x7F)+2;
            }
            else
            {
                size=data[1];
                head_size=2;
            }
            break;
        case DT_REGION://{���͡�����1������2}
            head_size =1;
            size=1;
            data+=2;
            for(idx=0;idx<2;idx++)
            {
                len=get_object_attribute_item_len(data,data[0]);
                size+=len;
                data+=len;
            }
            break;
        case DT_ROAD:
            head_size =1;
            size=data[5]*4+5;
            break;
        case DT_CSD:
            head_size =1;
            if(data[1]==0)
            {
                size=4+1;
            }
            else
            {
                size=data[6]*4+4+2;
            }
            break;
        case DT_RCSD:
            head_size =1;
            size=2;
            for(idx=0;idx<data[1];idx++)
            {
                if(data[size]==0)
                {
                    size+=5;
                }
                else
                {
                    size+=data[size+5]*4+6;
                }
            }
            size--;//������ͷ
            *attr_head_size=head_size;
            return size;
            //break;
        case DT_RSD:
            head_size =1;
            size=2;
            switch(data[1])
            {
                case 0:
                    break;
                case 1: //��������������  OAD+DATA
                    size+=4;
                    len=get_object_attribute_item_len(data+size,data[size]);
                    size+=len;
                    break;
                case 2: //��������������  OAD+DATA+DATA+DATA
                    size+=4;
                    len=get_object_attribute_item_len(data+size,data[size]);
                    size+=len;
                    len=get_object_attribute_item_len(data+size,data[size]);
                    size+=len;
                    len=get_object_attribute_item_len(data+size,data[size]);
                    size+=len;
                    break;
                case 3:
                    count=data[size++];
                    for(idx=0;idx<count;idx++)
                    {
                        size+=4;
                        len=get_object_attribute_item_len(data+size,data[size]);
                        size+=len;
                        len=get_object_attribute_item_len(data+size,data[size]);
                        size+=len;
                        len=get_object_attribute_item_len(data+size,data[size]);
                        size+=len;
                    }
                    break;
                case 4:
                case 5:
                   size+=7;//DateTimeBCD
                   size+=get_oop_data_type_len(data+size,DT_MS);//MS
                   break;
                case 6:
                case 7:
                case 8:
                   size+=7;//DateTimeBCD
                   size+=7;//DateTimeBCD
                   size+=3;//TI
                   size+=get_oop_data_type_len(data+size,DT_MS);//MS
                   break;
               case 9:
                   size++;//unsigned
                   break;
               case 10:
                   size++;//unsigned
                   size+=get_oop_data_type_len(data+size,DT_MS);//MS
                   break;
            }
            size--;//������ͷ
            break;
        case DT_MS:
            head_size =1;
            size=0;
            switch(data[1])
            {
                case 0:     //�޵��ܱ�
                    size=2;
                    break;
                case 1:    // ȫ���û���ַ
                    size=2;
                    break;
                case 2:   //һ���û�����
                    size = calculate_ms_size(data,DT_UNSIGNED);
                    break;
                case 3:  //һ���û���ַ
                    size = calculate_ms_size(data,DT_TSA);
                    break;
                case 4:   //һ���������
                    size = calculate_ms_size(data,DT_LONGUNSIGNED);
                    break;
                case 5:  //TODOһ���û���������
                case 6:  //TODOһ���û���ַ����
                case 7:  //TODOһ������������� 
                    size = calculate_ms_size(data,DT_REGION);
                    break;
            }
            size--;//������ͷ
            break;
        case DT_SID:
            head_size =1;
            size=5;
            size+=data[size]+1;
            size--;//������ͷ
            break;
        case DT_SID_MAC:
            size=5;
            size+=data[size]+1;
            size+=data[size]+1;
            size--;//������ͷ
            break;
    }
    *attr_head_size=head_size;
    return head_size+size;
}

BOOLEAN check_attribute_valid(INT8U *data,INT16U len,INT8U object_type,const tagObject_Attribute *attr_list,INT8U attr_list_size)
{
    INT16U count,size,idx;

    if(data[0]!=object_type)return FALSE;
    if(attr_list==NULL)return TRUE;

    if((data[0]==DT_STRUCTURE))
    {
        if(data[1] & 0x80)
        {
            count=cosem_bin2_unit(data+2,data[1]& 0x7F);
            data+=(data[1]& 0x7F)+2;
        }
        else
        {
            count=data[1];
            data+=2;
        }
        if(count!=attr_list_size/sizeof(tagObject_Attribute))
        {
            return FALSE;
        }
        for(idx=0;idx<count;idx++)
        {
            if(data[0]!=((tagObject_Attribute*)&attr_list[idx])->data_type)
            {
                return FALSE;
            }
            size=get_object_attribute_item_len(data,data[0]);
            data+=size;
        }
    }
    return TRUE;
}

INT16U get_oop_data_type_len(INT8U *data,INT8U data_type)
{
    INT32U len;
    INT16U idx,size,count;
    INT16U num;
    len=0;
    switch(data_type)
    {
        case DT_DONT_CARE:     //��û�����͵�APDU�����У�������255��0
        case DT_NULL:   return 0;
        case DT_INTEGER:
        case DT_UNSIGNED:
        case DT_ENUM:
        case DT_BOOLEAN: return 1;
        case DT_LONG:
        case DT_LONGUNSIGNED:
        case DT_SCALER_UNIT:
        case DT_OI:           return 2;
        case DT_TI:
        case DT_TIME:         return 3;
        case DT_FLOAT32:
        case DT_OAD:
        case DT_OMD:
        case DT_DOUBLELONG:
        case DT_DOUBLELONGUNSIGNED: return 4;
        case DT_LONG64:
        case DT_FLOAT64:
        case DT_LONG64UNSIGNED: return 8;
        case DT_DATETIME:  return 10;
        case DT_COMDCB:
        case DT_DATE:      return 5;
        case DT_DATETIME_S:    return 7;
        //����Ϊ�䳤����Ҫ�����ݹ����
        case DT_ARRAY:  //������Ҫ���ǳ���Ϊ����ֽڵ����
            if(data[0] & 0x80)
            {
                num=cosem_bin2_unit(data+1,data[0]& 0x7F);
                data+=(data[0]& 0x7F)+1;
                len+=(data[0]& 0x7F)+1;
            }
            else
            {
                num=data[0];
                data+=1;
                len+=1;
            }
            for(idx=0;idx<num;idx++)
            {
                size=get_object_attribute_item_len(data,data[0]);
                if(size==0)
                {
                    return 0;
                }
                len+=size;
                data+=size;
            }
            return len;
        case DT_STRUCTURE:
            num=data[0];
            data+=1;
            len+=1;
            if(num==0xFF)num=0;
            for(idx=0;idx<num;idx++)
            {
                size=get_object_attribute_item_len(data,data[0]);
                if(size==0)
                {
                    return 0;
                }
                len+=size;
                data+=size;
            }
            return len;
        case DT_BITSTRING:
            num=data[0]/8;
            if((data[0]%8) !=0 ) num++;
            return (num+1);
        case DT_EVENTMASK:
            num=data[1]/8;
            if((data[1]%8) !=0 ) num++;
            return (num+2);
        case DT_OCTETSTRING:
        case DT_VISIBLESTRING:
        case DT_UTF8STRING:
        case DT_TSA:
        case DT_MAC:
        case DT_RN:
            if(data[0] & 0x80)
            {
                num=cosem_bin2_unit(data+1,data[0]& 0x7F);
                num+=(data[0]& 0x7F)+1;
            }
            else
            {
                num=data[0]+1;
            }
            return num;
            //break;
        case DT_SID:
            len+=4;
            len+=get_oop_data_type_len(data+len,DT_OCTETSTRING);
            return len;
        case DT_SID_MAC:
            len+=get_oop_data_type_len(data+len,DT_SID);
            len+=get_oop_data_type_len(data+len,DT_MAC);
            return len;
        case DT_REGION://{���͡�����1������2}
            len+=1;
            data+=1;
            for(idx=0;idx<2;idx++)
            {
                size=get_object_attribute_item_len(data,data[0]);
                if(size==0)
                {
                    return 0;
                }
                len+=size;
                data+=size;
            }
            return len;
        case DT_ROAD:
            return data[4]*4+5;
        case DT_CSD:
            if(data[0]==0)
            {
                return 4+1;
            }
            else if(data[0]==1)
            {
                return data[5]*4+4+2;
            }
            else
            {
                return 0;
            }
            break;
        case DT_RCSD:
            size=1;
            for(idx=0;idx<data[0];idx++)
            {
                if(data[size]==0)
                {
                    size+=5;
                }
                else if(data[size]==1)
                {
                    size+=data[size+5]*4+6;
                }
                else
                {
                    return 0;
                }
            }
            return size;
            //break;
        case DT_RSD:
        {
            size=1;
            switch(data[0])
            {
                case 0:
                    break;
                case 1: //��������������  OAD+DATA
                    size+=4;
                    len=get_object_attribute_item_len(data+size,data[size]);
                    if(len==0)
                    {
                        return 0;
                    }
                    size+=len;
                    break;
                case 2: //��������������  OAD+DATA+DATA+DATA
                    size+=4;
                    len=get_object_attribute_item_len(data+size,data[size]);
                    if(len==0)
                    {
                        return 0;
                    }
                    size+=len;
                    len=get_object_attribute_item_len(data+size,data[size]);
                    if(len==0)
                    {
                        return 0;
                    }
                    size+=len;
                    len=get_object_attribute_item_len(data+size,data[size]);
                    if(len==0)
                    {
                        return 0;
                    }
                    size+=len;
                    break;
                case 3:
                    count=data[size++];
                    for(idx=0;idx<count;idx++)
                    {
                        size+=4;
                        len=get_object_attribute_item_len(data+size,data[size]);
                        if(len==0)
                        {
                            return 0;
                        }
                        size+=len;
                        len=get_object_attribute_item_len(data+size,data[size]);
                        if(len==0)
                        {
                            return 0;
                        }
                        size+=len;
                        len=get_object_attribute_item_len(data+size,data[size]);
                        if(len==0)
                        {
                            return 0;
                        }
                        size+=len;
                    }
                    break;
                case 4:
                case 5:
                   size=1;
                   size+=7;//DateTimeBCD
                   size+=get_oop_data_type_len(data+size,DT_MS);//MS
                   break;
                case 6:
                case 7:
                case 8:
                   size=1;
                   size+=7;//DateTimeBCD
                   size+=7;//DateTimeBCD
                   size+=3;//TI
                   size+=get_oop_data_type_len(data+size,DT_MS);//MS
                   break;
               case 9:
                   size=1;
                   size++;//unsigned
                   break;
               case 10:
                   size=1;
                   size++;//unsigned
                   size+=get_oop_data_type_len(data+size,DT_MS);//MS
                   break;
            }
            return size;
            //break;
        }
        case DT_MS:
            size=0;
            switch(data[0])
            {
                case 0:     //�޵��ܱ�
                    size=1;
                    break;
                case 1:    // ȫ���û���ַ
                    size=1;
                    break;
                case 2:   //һ���û�����
                    size=data[1]+2;
                    break;
                case 3:  //һ���û���ַ
                    size=2;
                    for(idx=0;idx<data[1];idx++)
                    {
                        size+=data[size]+1;
                    }
                    break;
                case 4:   //һ���������
                    size=data[1]*2+2;
                    break;
                case 5:  //TODOһ���û���������
                case 6:  //TODOһ���û���ַ����
                case 7:  //TODOһ�������������
                    size=calculate_ms_size_without_type(data,DT_REGION);
                    break;
            }
            return size;
            //break;
    }
    return len;


}

//��ȡ����������ĳ��Ԫ�ص�����
//index             Ԫ��
//only_value        �Ƿ�ֻ��ȡ����
//parent_index      ��һ��Ԫ��
INT16U get_attribute_inbuffer(INT8U index,INT8U *data_buffer,INT8U *attribute,INT16U max_len,BOOLEAN only_value,INT8U parent_index)
{

    INT8U *data;
    INT16U len;
    INT8U head_len;

    if(parent_index!=0xFF)
    {
        len=get_object_attribute_item_value(data_buffer,data_buffer[0],FALSE,parent_index,&data,&head_len);
        if((len>0) && (data!=NULL))
        {
            data_buffer=data;
        }
        else
        {
            return 0;
        }
    }
    len=get_object_attribute_item_value(data_buffer,data_buffer[0],only_value,index,&data,&head_len);
    if(len>max_len)
    {
        len=max_len;
    }
    if((len>0) && (data!=NULL))
    {
        mem_cpy(attribute,data,len);
        return len;
    }
    
    return 0;
}

/*
 �������-���ȼ�           PIID��
 ������Ӧ�ò�Э��汾��    long-unsigned��
 ������Э��һ���Կ�        ProtocolConformance��
 �����Ĺ���һ���Կ�        FunctionConformance��
 �ͻ�������֡���ߴ�      long-unsigned��
 �ͻ�������֡���ߴ�      long-unsigned��
 �ͻ�������֡��󴰿ڳߴ�  unsigned��
 �ͻ������ɴ���APDU�ߴ� long-unsigned��
 ������Ӧ�����ӳ�ʱʱ��    double-long-unsigned��
 ��֤�������              ConnectMechanismInfo
*/
INT16U get_apdu_app_link_request_size(INT8U *data,INT16U max_len)
{
    INT16U pos;
//    INT16U size;


    pos=1;
    pos++;                       //PIID-ACD
    pos+=2;                     //������Ӧ�ò�Э��汾��
    pos+=8;                    //������Э��һ���Կ�
    pos+=16;                    //�����Ĺ���һ���Կ�
    pos+=2;                     //����������֡���ߴ�    long-unsigned
    pos+=2;                     //����������֡���ߴ�    long-unsigned��
    pos+=1;                     //���������ߴ�֡����    unsigned��
    pos+=2;                     //�ͻ������ɴ���APDU�ߴ�long-unsigned��
    pos+=4;                     //�̶���Ӧ�����ӳ�ʱʱ��  double-long-unsigned��
    switch(data[pos++])
    {
        case 0:
            //pos++;  //��������    [0] NullSecurity��
            break;
        case 1:
            pos+=data[pos]+1;  //PasswordSecurity��=visible-string
            break;
        case 2:             //�ԳƼ���    [2] SymmetrySecurity
            pos+=data[pos]+1;  //����1        octet-string��
            pos+=data[pos]+1;  //�ͻ���ǩ��1  octet-string
            break;
        case 3:             //����ǩ��    [3] SignatureSecurity
            pos+=data[pos]+1;  //����1        octet-string��
            pos+=data[pos]+1;  //�ͻ���ǩ��1  octet-string
            break;
    }
    return pos;
}
/*
LINK-Response��=SEQUENCE
{
  �������-���ȼ�  PIID��
  ���             Result��
  ����ʱ��         DataTime��
  �յ�ʱ��         DataTime��
  ��Ӧʱ��         DataTime
}
*/
INT16U get_apdu_link_response_size(INT8U *data,INT16U max_len)
{
    INT16U pos;
//    INT16U size;


    pos=1;
    pos++;                       //PIID-ACD
    pos++;                      //���             Result
    pos+=10;                     //����ʱ��         DataTime��
    pos+=10;                     //����ʱ��         DataTime��
    pos+=10;                     //��Ӧʱ��         DataTime
    return pos;
}
INT16U get_apdu_get_request_size(INT8U *data,INT16U max_len)
{
    INT16U pos,size;
    INT16U count,idx;
    INT16U count2,idx2;

    size=0;
    pos=1;
    pos++;                       //Ӧ�ò����  CHOICE
    pos++;                       //PIID-ACD
    switch(data[1])
    {
        case 1:
            pos+=4;              //OAD
            break;
        case 2://SEQUENCE OF OAD
            if(data[pos] & 0x80)
            {
                count=cosem_bin2_unit(data+pos+1,data[pos]& 0x7F);
                pos+=(data[pos]& 0x7F)+1;
                pos+=count*4;  //OAD  LIST
            }
            else
            {
            pos+=data[pos]*4+1;  //OAD  LIST
            }
            break;
        case 3:
            pos+=4;              //OAD
            size=get_oop_data_type_len(data+pos,DT_RSD);
            if(size==0)
            {
                return 0;
            }
            pos+=size;
            count=data[pos++];  //SEQUENCE OF,��Ҫ���Ǳ䳤
            for(idx=0;idx<count;idx++)
            {
                size=get_oop_data_type_len(data+pos,DT_CSD);
                if(size==0)
                {
                    return 0;
                }
                pos+=size;
            }
            break;
        case 4:
            count=data[pos++];
            for(idx=0;idx<count;idx++)
            {
                pos+=4;              //OAD
                size=get_oop_data_type_len(data+pos,DT_RSD);
                if(size==0)
                {
                    return 0;
                }
                pos+=size;
                count2=data[pos++];
                for(idx2=0;idx2<count2;idx2++)
                {
                    size=get_oop_data_type_len(data+pos,DT_CSD);
                    if(size==0)
                    {
                        return 0;
                    }
                    pos+=size;
                }
            }
            break;
        case 5:
            pos+=2;              //��ȷ���յ����һ�����ݿ����   long-unsigned
            break;
		case 6:
			pos =8;
			break;
    }
    return pos;
}
INT16U get_apdu_set_request_size(INT8U *data,INT16U max_len)
{
    INT16U pos,size;
    INT16U count,idx;

    size=0;
    pos=1;
    pos++;                       //Ӧ�ò����  CHOICE
    pos++;                       //PIID-ACD
    switch(data[1])
    {
        case 1:
            pos+=4;              //OAD
            size=get_object_attribute_item_len(data+pos,data[pos]);
            if(size==0)
            {
                return 0;
            }
            pos+=size;
            break;
        case 2:
            count=data[pos++];
            for(idx=0;idx<count;idx++)
            {
                pos+=4;              //OAD
                size=get_object_attribute_item_len(data+pos,data[pos]);
                if(size==0)
                {
                    return 0;
                }
                pos+=size;
            }
            break;
        case 3:
            count=data[pos++];
            for(idx=0;idx<count;idx++)
            {
                pos+=4;              //OAD
                size=get_object_attribute_item_len(data+pos,data[pos]);
                if(size==0)
                {
                    return 0;
                }
                pos+=size;
                pos+=4;              //OAD
                pos+=1;              //��ʱ��ȡʱ��         unsigned
            }
            break;
        default:
            pos=0;
            break;
    }
    return pos;
}
INT16U get_apdu_action_request_size(INT8U *data,INT16U max_len)
{
    INT16U pos,size;
    INT16U count,idx;

    size=0;
    pos=1;
    pos++;                       //Ӧ�ò����  CHOICE
    pos++;                       //PIID-ACD
    switch(data[1])
    {
        case 1:
            pos+=4;              //OAD
            size=get_object_attribute_item_len(data+pos,data[pos]);
            if(size==0)
            {
                return 0;
            }
            pos+=size;
            break;
        case 2:
            count=data[pos++];
            for(idx=0;idx<count;idx++)
            {
                pos+=4;              //OAD
                size=get_object_attribute_item_len(data+pos,data[pos]);
                if(size==0)
                {
                    return 0;
                }
                pos+=size;
            }
            break;
        case 3:
            count=data[pos++];
            for(idx=0;idx<count;idx++)
            {
                pos+=4;              //OAD
                size=get_object_attribute_item_len(data+pos,data[pos]);
                if(size==0)
                {
                    return 0;
                }
                pos+=size;
                pos+=4;              //OAD
                pos+=1;              //��ʱ��ȡʱ��         unsigned
            }
            break;
        default:
            pos=0;
            break;
    }
    return pos;
}
INT16U get_apdu_report_response_size(INT8U *data,INT16U max_len)
{
    INT16U pos;
//    INT16U count,idx;

    pos=2;
    pos++;                       //PIID-ACD
    switch(data[1])
    {
        case 1:
            pos+=data[3]*4+1;              //OAD
            break;
        case 2:
            pos+=data[3]*4+1;              //OAD
            break;
        default:
            pos=0;
            break;
    }
    return pos;
}
INT16U get_apdu_proxy_request_size(INT8U *data,INT16U max_len)
{
    INT16U pos,size;
    INT16U count,idx;
    INT16U count2,idx2;

    size=0;
    pos=2;
    pos++;                       //PIID-ACD
    switch(data[1])
    {
        case 1:
            pos+=2;                   //������������ĳ�ʱʱ��   long-unsigned
            count=data[pos++];
            for(idx=0;idx<count;idx++)
            {
                pos+=data[pos]+1;    //TSA
                pos+=2;              //����һ���������ĳ�ʱʱ��   long-unsigned
                pos+=data[pos]*4+1;  //OAD
            }
            break;
        case 2:
            pos+=2;                   //������������ĳ�ʱʱ��   long-unsigned
            pos+=data[pos]+1;    //TSA
            pos+=4;              //OAD
            size=get_oop_data_type_len(data+pos,DT_RSD);
            if(size==0)
            {
                return 0;
            }
            pos+=size;
            count=data[pos++];
            for(idx=0;idx<count;idx++)
            {
                size=get_oop_data_type_len(data+pos,DT_CSD);
                if(size==0)
                {
                    return 0;
                }
                pos+=size;
            }
            break;
        case 3:
            pos+=2;                   //������������ĳ�ʱʱ��   long-unsigned
            count=data[pos++];
            for(idx=0;idx<count;idx++)
            {
                pos+=data[pos]+1;    //TSA
                pos+=2;              //����һ���������ĳ�ʱʱ��   long-unsigned
                count2=data[pos++];
                for(idx2=0;idx2<count2;idx2++)
                {
                    pos+=4;              //OAD
                    size=get_object_attribute_item_len(data+pos,data[pos]);
                    if(size==0)
                    {
                        return 0;
                    }
                    pos+=size;
                }
            }
            break;
        case 4:
            pos+=2;                   //������������ĳ�ʱʱ��   long-unsigned
            count=data[pos++];
            for(idx=0;idx<count;idx++)
            {
                pos+=data[pos]+1;    //TSA
                pos+=2;              //����һ���������ĳ�ʱʱ��   long-unsigned
                count2=data[pos++];
                for(idx2=0;idx2<count2;idx2++)
                {
                    pos+=4;              //OAD
                    size=get_object_attribute_item_len(data+pos,data[pos]);
                    if(size==0)
                    {
                        return 0;
                    }
                    pos+=size;
                    pos+=4;              //OAD
                    pos+=1;              //��ʱ��ȡʱ��         unsigned
                }
            }
            break;
        case 5:
            pos+=2;                   //������������ĳ�ʱʱ��   long-unsigned
            count=data[pos++];
            for(idx=0;idx<count;idx++)
            {
                pos+=data[pos]+1;    //TSA
                pos+=2;              //����һ���������ĳ�ʱʱ��   long-unsigned
                count2=data[pos++];
                for(idx2=0;idx2<count2;idx2++)
                {
                    pos+=4;              //OAD
                    size=get_object_attribute_item_len(data+pos,data[pos]);
                    if(size==0)
                    {
                        return 0;
                    }
                    pos+=size;
                }
            }
            break;
        case 6:
            pos+=2;                   //������������ĳ�ʱʱ��   long-unsigned
            count=data[pos++];
            for(idx=0;idx<count;idx++)
            {
                pos+=data[pos]+1;    //TSA
                pos+=2;              //����һ���������ĳ�ʱʱ��   long-unsigned
                count2=data[pos++];
                for(idx2=0;idx2<count2;idx2++)
                {
                    pos+=4;              //OAD
                    size=get_object_attribute_item_len(data+pos,data[pos]);
                    if(size==0)
                    {
                        return 0;
                    }
                    pos+=size;
                    pos+=4;              //OAD
                    pos+=1;              //��ʱ��ȡʱ��         unsigned
                }
            }
            break;
        case 7:
            pos+=4;                   //����ת���˿�                  OAD
            pos+=5;                   //�˿�ͨ�ſ��ƿ�                COMDCB
            pos+=2;                  //���յȴ����ĳ�ʱʱ�䣨�룩   long-unsigned��
            pos+=2;                  //���յȴ��ֽڳ�ʱʱ�䣨���룩 long-unsigned��
            size=get_oop_data_type_len(data+pos,DT_OCTETSTRING);
            pos+=size;
            break;
        default:
            pos=0;
            break;
    }
    return pos;
}
INT16U get_apdu_security_request_size(INT8U *data,INT16U max_len)
{
    INT16U pos;
    //INT16U count,idx;

    pos=1;
    pos++;                       //Ӧ�����ݵ�Ԫ  CHOICE
    pos+=get_oop_data_type_len(data+pos,DT_OCTETSTRING);                       //����Ӧ�����ݵ�Ԫ   [0]  octet-string������Ӧ�����ݵ�Ԫ   [1]  octet-string
    
    switch(data[pos++])
    {
        case 0:
            pos+=get_oop_data_type_len(data+pos,DT_SID_MAC);              //����MAC         [0]  SID_MAC��
            break;
        case 1:
            pos+=get_oop_data_type_len(data+pos,DT_RN);                //�����          [1]  RN��
            break;
        case 2:
            pos+=get_oop_data_type_len(data+pos,DT_RN);             //�����+����MAC  [2]  RN_MAC
            pos+=get_oop_data_type_len(data+pos,DT_MAC);            //�����+����MAC  [2]  RN_MAC
            break;
        case 3:
            pos+=get_oop_data_type_len(data+pos,DT_SID);              //��ȫ��ʶ       [3]  SID
            break;
        default:
            pos=0;
            break;
    }
    return pos;
}
INT16U get_apdu_security_response_size(INT8U *data,INT16U max_len)
{
    INT16U pos;

    pos=1;
    switch(data[pos++])                       //Ӧ�����ݵ�Ԫ  CHOICE
    {
        case 0:
        case 1:
            pos+=get_oop_data_type_len(data+pos,DT_OCTETSTRING);                       //����Ӧ�����ݵ�Ԫ   [0]  octet-string������Ӧ�����ݵ�Ԫ   [1]  octet-string
            break;
        case 2:
            pos++;
        default:
            return 0;
    }
    if(data[pos++]!=0)
    {
        switch(data[pos++])
        {
            case 0:
                pos+=get_oop_data_type_len(data+pos,DT_SID_MAC);              //����MAC         [0]  SID_MAC��
                break;
            default:
                return 0;
        }
    }
    return pos;
}

void load_esam_oi_list(void)
{
    INT16U idx,pos,count,num;
    INT8U data[80];
    BOOLEAN is_break;
    
    is_break=FALSE;
    pos=0;
    num=0;
    g_esam_oi_list.count=0;
    mem_set(data,80,0x00);

    //fread_array(FILEID_ESAM_PARAM,FLADDR_ESAM_OI_LIST+pos,data,4);
    if((data[0]!=0x55)|| (data[1]!=0xAA)) return;
    count=bin2_int16u(data+2);
    pos+=4;
    for(;;)
    {
        //fread_array(FILEID_ESAM_PARAM,FLADDR_ESAM_OI_LIST+pos,data,80);
        pos+=80;
        num+=16;
        for(idx=0;idx<16;idx++)
        {
            if(!check_is_all_FF(data+idx*5,5) &&  !check_is_all_ch(data+idx*5,5,0x00))
            {
                if((g_esam_oi_list.count>=MAX_ESAM_OI_COUNT) || (g_esam_oi_list.count>=count))
                {
                    is_break=TRUE;
                    break;
                }
                mem_cpy(g_esam_oi_list.object_list[g_esam_oi_list.count++],data+idx*5,5);
            }
        }
        if(is_break || (num>=MAX_ESAM_OI_COUNT))
        {
            break;
        }
    }
    g_esam_oi_list.mark=0xAA55;
}

void default_esam_oi_list(INT16U obis,INT8U mask,INT16U safe_config)
{
    //tpos_enterCriticalSection();
    g_esam_oi_list_default.object_list[g_esam_oi_list_default.count][0]=obis>>8;
    g_esam_oi_list_default.object_list[g_esam_oi_list_default.count][1]=obis;
    g_esam_oi_list_default.object_list[g_esam_oi_list_default.count][2]=safe_config>>8;
    g_esam_oi_list_default.object_list[g_esam_oi_list_default.count][3]=safe_config;
    g_esam_oi_list_default.object_list[g_esam_oi_list_default.count][4]=mask;
    g_esam_oi_list_default.count++;
    g_esam_oi_list_default.mark=0xAA55;
    //tpos_leaveCriticalSection();
}

void update_esam_oi_list(INT8U data[4])
{
    INT16U idx;
    BOOLEAN found;

    found=FALSE;
    //tpos_enterCriticalSection();
    for(idx=0;idx<g_esam_oi_list.count;idx++)
    {
        if((g_esam_oi_list.object_list[idx][0]==data[0]) && (g_esam_oi_list.object_list[idx][1]==data[1]))
        {
            g_esam_oi_list.object_list[idx][2]=data[2];
            g_esam_oi_list.object_list[idx][3]=data[3];
            g_esam_oi_list.object_list[idx][4]=EXPLICIT_CONFIG_WORD;
            found=TRUE;
            break;
        }
    }
    if(!found)
    {
        mem_cpy(g_esam_oi_list.object_list[g_esam_oi_list.count],data,4);
        g_esam_oi_list.object_list[g_esam_oi_list.count][4]=EXPLICIT_CONFIG_WORD;
        g_esam_oi_list.count++;
    }
    g_esam_oi_list.mark=0xAA55;
    //tpos_leaveCriticalSection();
}
void delete_esam_oi_list(INT8U oi[2])
{
    INT16U idx,del_idx;
    BOOLEAN found;

    found=FALSE;
    //tpos_enterCriticalSection();
    for(idx=0;idx<g_esam_oi_list.count;idx++)
    {
        if((g_esam_oi_list.object_list[idx][0]==oi[0]) && (g_esam_oi_list.object_list[idx][1]==oi[1]))
        {
            found=TRUE;
            del_idx=idx;
            break;
        }
    }
    if(found)
    {
        if(g_esam_oi_list.count>1)
        {
            mem_cpy(g_esam_oi_list.object_list[del_idx],g_esam_oi_list.object_list[g_esam_oi_list.count-1],5);
        }
        g_esam_oi_list.count--;
        mem_set(g_esam_oi_list.object_list[g_esam_oi_list.count],5,0x00);

    }
    g_esam_oi_list.mark=0xAA55;
    //tpos_leaveCriticalSection();
}

INT16U get_esam_safe_config(INT16U obis)
{
    INT16U idx,found_idx,ret,obis_mask;
    INT16U mask[]={0x000F,0x00FF,0x0FFF};
    INT8U oi[2],num;

    oi[0]=obis>>8;
    oi[1]=obis;
    
    ret=0;
    found_idx=0xFFFF;
    //tpos_enterCriticalSection();
    //������ʽ��ȫģʽ���б��в���
    for(idx=0;idx<g_esam_oi_list.count;idx++)
    {
        if(g_esam_oi_list.object_list[idx][4] & EXPLICIT_CONFIG_WORD)
        {
            if((g_esam_oi_list.object_list[idx][0]==oi[0]) && (g_esam_oi_list.object_list[idx][1]==oi[1]))
            {
                found_idx=idx;
                break;
            }
        }
    }
    if(found_idx!=0xFFFF)
    {
        ret=g_esam_oi_list.object_list[found_idx][2];
        ret<<=8;
        ret|=g_esam_oi_list.object_list[found_idx][3];

    }
    //���û�ҵ�������Ĭ�ϰ�ȫģʽ���б��з�ͨ���в���,
    if(found_idx==0xFFFF)
    {
        for(idx=0;idx<g_esam_oi_list_default.count;idx++)
        {
            if(g_esam_oi_list_default.object_list[idx][4]==0)
            {
                if((g_esam_oi_list_default.object_list[idx][0]==oi[0]) && (g_esam_oi_list_default.object_list[idx][1]==oi[1]))
                {
                    found_idx=idx;
                    break;
                }
            }
        }
        if(found_idx!=0xFFFF)
        {
            ret=g_esam_oi_list_default.object_list[found_idx][2];
            ret<<=8;
            ret|=g_esam_oi_list_default.object_list[found_idx][3];

        }
    }
    //���û�ҵ�������Ĭ�ϰ�ȫģʽ���б���ͨ���в���
    if(found_idx==0xFFFF)
    {
        for(idx=0;idx<g_esam_oi_list_default.count;idx++)
        {
            num=g_esam_oi_list_default.object_list[idx][4];
            oi[0]=obis>>8;
            oi[1]=obis;
            if((num>0) && (num<4))
            {
                obis_mask=obis | mask[num-1];
                oi[0]=obis_mask>>8;
                oi[1]=obis_mask;
            }
            if((g_esam_oi_list_default.object_list[idx][0]==oi[0]) && (g_esam_oi_list_default.object_list[idx][1]==oi[1]))
            {
                found_idx=idx;
                break;
            }
        }
        if(found_idx!=0xFFFF)
        {
            ret=g_esam_oi_list_default.object_list[found_idx][2];
            ret<<=8;
            ret|=g_esam_oi_list_default.object_list[found_idx][3];

        }
    }
    //tpos_leaveCriticalSection();
    return ret;
}

INT32U get_system_flag(INT32U flag,INT8U type)
{
    INT32U ret;
    if(type>=MAX_SYSTEM_FLAG_TYPE)
    {
        return FALSE;
    }
    ret=(sys_flag[type] & flag);
    return ret;
}
void set_system_flag(INT32U flag,INT8U type)
{
    if(type>=MAX_SYSTEM_FLAG_TYPE)
    {
        return;
    }
    sys_flag[type] |= flag;
}
void  clr_system_flag(INT32U flag,INT8U type)
{
    if(type>=MAX_SYSTEM_FLAG_TYPE)
    {
        return;
    }
    sys_flag[type] &= ~flag;
}
void  clr_system_flag_type(INT8U type)
{
    if(type>=MAX_SYSTEM_FLAG_TYPE)
    {
        return;
    }
    sys_flag[type] = 0;
}
void  clr_system_flag_all()
{
    INT8U idx;
    for(idx=0;idx<MAX_SYSTEM_FLAG_TYPE;idx++)
    {
        sys_flag[idx] = 0;
    }
}
