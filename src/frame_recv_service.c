#include "main_include.h"

/*+++
  ���ܣ�  �ж��Ƿ��Ѿ����յ��˱���ͷ��
  ��ע��
          1)Ҫ��frame�в����������ֽڣ������ֽڱ�����ϱ�׼��
---*/

INT8U* trim_oopframe_FE(INT8U* frame)
{
    INT8U *old_frame;
    INT8U idx=0;

    old_frame=frame;
    while(idx<8)
    {
        if(*frame==0x68)
        {
            return frame;
        }
        frame++;
        idx++;
    }
    return old_frame;

}

INT8U get_oopframe_addr_length(INT8U* frame)
{
    INT8U addr_len;

    //frame=trim_oopframe_FE(frame);

    addr_len = 0;
    if (0x68 != frame[OOP_POS_BEGIN]) return addr_len;
    addr_len=(frame[OOP_POS_ADDR] & 0x0F)+3;
    return addr_len;
}

INT16U get_frame_header_length_gb_oop(INT8U* frame)
{
    INT8U addr_len;

    frame=trim_oopframe_FE(frame);
    addr_len = get_oopframe_addr_length(frame);
    if(addr_len == 0) return 0;

    return addr_len + 6;
}


INT8U check_frame_header_gb_oop(INT8U* frame)
{
    INT16U hcs;
    INT8U addr_len;

    //frame=trim_oopframe_FE(frame);
    addr_len = get_oopframe_addr_length(frame);
    if((addr_len == 0) || (addr_len>16)) return FALSE;
    //����HCSУ��λ
    hcs = fcs16_16u(frame+OOP_POS_LEN,addr_len+3);
    if (hcs != bin2_int16u(frame+OOP_POS_ADDR+addr_len)) 
    {
        return FALSE;
    }
    return TRUE;
}

INT8U check_frame_header_gb_oop_up(INT8U* frame)
{
    if(check_frame_header_gb_oop(frame))
    {
        if((frame[OOP_POS_CTRL]&0x80)==0x80)
        {
            return TRUE;
        }
    }
    return FALSE;
}

INT8U check_frame_header_gb_oop_down(INT8U* frame)
{
    if(check_frame_header_gb_oop(frame))
    {
        if((frame[OOP_POS_CTRL]&0x80)==0)
        {
            return TRUE;
        }
    }
    return FALSE;
}

INT8U  is_1376_1_frame_header(INT8U *frame, INT16U recv_len)
{
    //���376.1����ͷ
    if((0x68 == frame[0])  && (0x68 == frame[5]) )
    {
         //��������
         if( (frame[1]==frame[3]) && (frame[2]==frame[4]) )
         {
              return TRUE;
         }
    }
    return FALSE;
}
INT8U is_gb645_frame_header(INT8U *frame,INT16U recv_len)
{
    //���645����ͷ
    if((0x68 == frame[0])  && (0x68 == frame[7]) )
    {
         //���BCD����
        if(is_valid_bcd(frame+1,6) || check_is_all_ch(frame+1,6,0xAA))
        {
            //�������飿
            return TRUE;
        }
    }
    return FALSE;
}
INT8U is_onenet_frame_header(INT8U *frame,INT16U recv_len)
{
    INT16U pos=0;
    
    if(pos == str_find(frame,recv_len, (uint8_t *)"+MIPL", 5))
    {
        if(pos == str_find(frame,recv_len, (uint8_t *)"READ", 4))
    {
        return ONENET_READ;
    }   
        if(pos == str_find(frame,recv_len, (uint8_t *)"WRITE", 5))
    {
        return ONENET_WRITE;
    }  
        if(pos == str_find(frame,recv_len, (uint8_t *)"EVENT", 5))
        {
            return ONENET_EVENT_ID;
        }           
    }
    else
    {
        return 0;
    }
}
/*+++
  ���ܣ�  �ӱ���ͷ�еõ��������ĵĳ��ȣ��������屨�ĵĽ���
---*/
INT16U  get_1376_1_frame_len(INT8U *frame, INT16U recv_len)
{
       INT16U  frameLen;

       frameLen = frame[2];
       frameLen <<= 8;
       frameLen += frame[1];
       frameLen >>=2;

       frameLen += 8;

       return frameLen;
}
INT16U  get_gb645_frame_len(INT8U *frame, INT16U recv_len)
{
       INT16U  frameLen;

       frameLen = frame[9]+12;

       return frameLen;
}

INT16U  get_gboop_frame_len(INT8U *frame, INT16U recv_len)
{
       INT16U  frameLen;

       frameLen = frame[2];
       frameLen <<= 8;
       frameLen += frame[1];

       frameLen += 2;

       return frameLen;
}

/*+++
  ���ܣ�  ��ʼ�����Ľ��յ�У��ֵ���Ѿ����յ��������ı���ͷ
  ��ע��
          1����Ҫ���Ǳ����Ѿ�ȫ����������˵������
---*/
INT16U init_1376_1_frame_checksum(INT8U *frame, INT16U frame_len, INT16U recv_len)
{
   INT16U idx,count;
   INT16U  cs;

   //ȷ�����㳤��
   if(recv_len <= frame_len - 2)
   {
       count  = recv_len;
   }
   else
   {
      count = frame_len - 2;
   }

   cs = 0;   
   for(idx=6; idx < count ; idx++)
   {
      cs += frame[idx];
   }

   return cs;

}
INT16U init_gb645_frame_checksum(INT8U *frame, INT16U frame_len, INT16U recv_len)
{
   INT16U idx,count;
   INT16U  cs;

   //ȷ�����㳤��
   if(recv_len <= frame_len - 2)
   {
       count  = recv_len;
   }
   else
   {
      count = frame_len - 2;
   }

   cs = 0;   
   for(idx=0; idx < count ; idx++)
   {
      cs += frame[idx];
   }

   return cs;

}
/*+++
   ���ܣ�  ��鱨�Ľ����Ƿ��Ѿ�����ˣ����չ�������Ҫ����У��ֵ
---*/
INT8U  is_1376_1_frame_ready(INT8U *frame, INT16U frame_len, INT16U recv_len,INT16U *cs)
{
    if(recv_len >= frame_len)
    {
        if(  (((INT8U)(*cs)) == frame[frame_len-2])  &&  (0x16 == frame[frame_len-1])  )
        {
           return TRUE;
        }
        return FALSE;
    }
    else if(recv_len <= frame_len - 2)
    {
        *cs += frame[recv_len - 1];
    }

    return FALSE;
}
INT8U  is_gb645_frame_ready(INT8U *frame, INT16U frame_len, INT16U recv_len,INT16U *cs)
{
    if(recv_len >= frame_len)
    {
        if(  (((INT8U)(*cs)) == frame[frame_len-2])  &&  (0x16 == frame[frame_len-1])  )
        {
           return TRUE;
        }
        return FALSE;
    }
    else if(recv_len <= frame_len - 2)
    {
        *cs += frame[recv_len - 1];
    }

    return FALSE;
}
INT8U is_oop_protocol_frame_ready(INT8U *frame, INT16U frame_len, INT16U recv_len,INT16U *cs)
{
    INT16U fcs;
    if(recv_len >= frame_len)
    {
        if(0x16 == frame[frame_len-1])
        {
            //����Fcs
            *cs = fcs16_16u(frame+OOP_POS_LEN,frame_len-2-2);
            fcs = frame[frame_len-1-2] + 0x100*frame[frame_len-1-1];

            if ((*cs) == fcs) return TRUE;
        }
    }

    return FALSE;
}