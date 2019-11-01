#include "app_const_3761.h"
//#include "app_var_3761.h"
#include "app_fun_3761.h"


/*+++
  ���ܣ�����DATAFMT_01
---*/
void set_DATAFMT_01(DATAFMT_01 *data)
{
    INT8U year;
    INT8U idx;
    tagDatetime datetime;

    os_get_datetime(&datetime);
    for(idx=0;idx<6;idx++)
    {
       data->bcd_datetime[idx] = byte2BCD(datetime.value[idx]);
    }
//    year = datetime[CENTURY]*100;
//    year |= datetime[YEAR];
    year =datetime.year;
    data->bcd_datetime[4] |= weekDay(year ,datetime.month,datetime.day)<<5;//������
}

/*+++
  ���ܣ�����DATAFMT_15
---*/
//void set_DATAFMT_15(DATAFMT_15 *data)
//{
//    INT8U idx;
//    for(idx=0;idx<5;idx++)
//    {
//       data->value[idx] = byte2BCD(datetime[idx+1]);
//    }
//}

/*+++
  ���ܣ�  �����������ݸ�ʽ02
  ��ע��      BCD��ʮλ|BCD���λ
                G3G2G1S|BCD���λ
---*/
void double_to_fmt02(double dbVal,INT8U bcd2[2])
{
     int e;
     INT8U ch;
     INT8U s;
     int iVal;


     s = 0;
     if(dbVal < 0)
     {
         s=0x10;  //����λ
         dbVal = -dbVal;
     }
     //G3G2G1=100Ϊ1

     e=4;
     if(dbVal > 999)
     {
       while(dbVal >999)
       {
             dbVal/=10.0;
             e--;
             if(e==0) break;
       }
     }
     else
     {
        while(dbVal < 999)
        {
             if(dbVal * 10.0 > 999) break;
             dbVal*=10.0;
             e++;
             if(e==7) break;
        }
     }

     iVal = dbVal+0.49999;

     ch = iVal % 100;
     bcd2[0] =  byte2BCD(ch);
     ch = iVal / 100;
     bcd2[1] =  byte2BCD(ch);
     bcd2[1] |=  e<<5;
     bcd2[1] |= s;

}



//�ܼӹ��ʱ任Ϊ��ʽ02��  ��λ: 0.0001KW
// INT8U  e ָ��������ȡ8����ʾ10E-4�������𽥼�С��
void  ul2fmt02(INT32U ul, INT8U e,INT8U bcd2[2])
{
   INT8U ch;

   while( (e >= 8)   || (ul >= 1000) )
   {
       e--;
       ul =  (ul + 5) / 10;
       if(e == 0) break;
   }

   ch = ul % 100;
   bcd2[0] =  byte2BCD(ch);
   ch = ul / 100;
   bcd2[1] =  byte2BCD(ch);
   bcd2[1] |=  e<<5;
}
void  int32s2fmt02(INT32S int32s, INT8U e,INT8U bcd2[2])
{
   INT8U ch;
   INT8U sign;
   if(int32s < 0)
   {
       sign = 1;
       int32s = -int32s;
   }
   else
   {
       sign = 0;
   }

   while( (e >= 8)   || (int32s >= 1000) )
   {
       e--;
       int32s =  (int32s + 5) / 10;
       if(e == 0) break;
   }

   ch = int32s % 100;
   bcd2[0] =  byte2BCD(ch);
   ch = int32s / 100;
   bcd2[1] =  byte2BCD(ch)&0x0F;
   if(sign)
   {
       bcd2[1] |= 0x10;
   }
   bcd2[1] |=  e<<5;
}
////�ܼӹ��ʱ任Ϊ��ʽ02��  ��λ: 0.0001KW
//// INT8U  e ָ��������ȡ8����ʾ10E-4�������𽥼�С��
//void  INT32S2fmt02(INT32S value, INT8U e,INT8U bcd2[2])
//{
//   INT8U ch;
//   BOOLEAN sign;
//
//   if(value < 0)
//   {
//       sign = TRUE;
//       value = -value;
//   }
//   else
//   {
//       sign = FALSE;
//   }
//
//   while( (e >= 8)   || (value >= 1000) )
//   {
//       e--;
//       value =  (value + 5) / 10;
//       if(e == 0) break;
//   }
//
//   ch = value % 100;
//   bcd2[0] =  byte2BCD(ch);
//   ch = value / 100;
//   bcd2[1] =  byte2BCD(ch);
//   bcd2[1] |=  e<<5;
//   if(sign == TRUE)
//   {
//       bcd2[1] |=  0x10;
//   }
//}

//�ܼӹ��ʱ任Ϊ��ʽ02��  ��λ: 0.0001KW
//  ul<0 return 0
BOOLEAN fmt02_to_ul(INT8U BCD2[2],INT32U *ul)
{
    INT32U times;

    if(is_valid_bcd(BCD2,1) == FALSE)
    //if(BCD2[0] > 0x99)
    {
       *ul = 0;
       return FALSE;
    }


    switch(BCD2[1]>>5)
    {
    case 0: times = 100000000; break;
    case 1: times =  10000000; break;
    case 2: times =   1000000; break;
    case 3: times =    100000; break;
    case 4: times =     10000; break;
    case 5: times =      1000; break;
    case 6: times =       100; break;
    case 7: times =        10; break;
	 default:times =         1; break;
    }

    *ul =  (BCD2[0] & 0x0F) +  (BCD2[0]>>4)*10 +  (BCD2[1] & 0x0F)*100;

    *ul *=times;

    if(BCD2[1] & 0x10) *ul = 0;

    return TRUE;

}
BOOLEAN fmt02_to_int32S(INT8U BCD2[2],INT32S *int32s)
{
    INT32U times;

    if(is_valid_bcd(BCD2,1) == FALSE)
    //if(BCD2[0] > 0x99)
    {
       *int32s = 0;
       return FALSE;
    }


    switch(BCD2[1]>>5)
    {
    case 0: times = 100000000; break;
    case 1: times =  10000000; break;
    case 2: times =   1000000; break;
    case 3: times =    100000; break;
    case 4: times =     10000; break;
    case 5: times =      1000; break;
    case 6: times =       100; break;
    case 7: times =        10; break;
	 default:times =         1; break;
    }

    *int32s =  (BCD2[0] & 0x0F) +  (BCD2[0]>>4)*10 +  (BCD2[1] & 0x0F)*100;

    *int32s *=times;

    if(BCD2[1] & 0x10) *int32s = -*int32s;

    return TRUE;

}
BOOLEAN fmt02_to_int32u(INT8U BCD2[2],INT32U *int32u)
{
    INT32U times;

    if(is_valid_bcd(BCD2,1) == FALSE)
    //if(BCD2[0] > 0x99)
    {
       *int32u = 0;
       return FALSE;
    }


    switch(BCD2[1]>>5)
    {
    case 0: times = 100000000; break;
    case 1: times =  10000000; break;
    case 2: times =   1000000; break;
    case 3: times =    100000; break;
    case 4: times =     10000; break;
    case 5: times =      1000; break;
    case 6: times =       100; break;
    case 7: times =        10; break;
	 default:times =         1; break;
    }

    *int32u =  (BCD2[0] & 0x0F) +  (BCD2[0]>>4)*10 +  (BCD2[1] & 0x0F)*100;

    *int32u *=times;

    if(BCD2[1] & 0x10) *int32u = 0;

    return TRUE;

}
////�ܼӹ��ʱ任Ϊ��ʽ02��  ��λ: 0.0001KW
////  ul<0 return 0
//BOOLEAN fmt02_to_INT32S(INT8U BCD2[2],INT32S *ul)
//{
//    INT32U times;
//
//    if(is_valid_bcd(BCD2,1) == FALSE)
//    //if(BCD2[0] > 0x99)
//    {
//       *ul = 0;
//       return FALSE;
//    }
//
//
//    switch(BCD2[1]>>5)
//    {
//    case 0: times = 100000000; break;
//    case 1: times =  10000000; break;
//    case 2: times =   1000000; break;
//    case 3: times =    100000; break;
//    case 4: times =     10000; break;
//    case 5: times =      1000; break;
//    case 6: times =       100; break;
//    case 7: times =        10; break;
//	 default:times =         1; break;
//    }
//
//    *ul =  (BCD2[0] & 0x0F) +  (BCD2[0]>>4)*10 +  (BCD2[1] & 0x0F)*100;
//
//    *ul *=times;
//
//    if(BCD2[1] & 0x10) *ul = -*ul;
//
//    return TRUE;
//
//}

// ul ��λ: KWh
BOOLEAN fmt03_to_ul(INT8U BCD2[4],INT32S *ul)
{
    if(is_valid_bcd(BCD2,3) == FALSE)
    //if(BCD2[0] > 0x99)
    {
       *ul = 0;
       return FALSE;
    }

    *ul =  (BCD2[0] & 0x0F)       +  (BCD2[0]>>4)*10;
    *ul += (BCD2[1] & 0x0F)*100   +  (BCD2[1]>>4)*1000 ;
    *ul += (BCD2[2] & 0x0F)*10000 +  (BCD2[2]>>4)*100000 ;
    *ul += (BCD2[3] & 0x0F)*1000000 ;

    if(BCD2[3] & 0x40) *ul *= 1000;

    if(BCD2[3] & 0x10) *ul *= -1;
    return TRUE;

}
//Ŀǰʹ��fmt03_to_ul�Ѿ��㹻�ˣ�����Ҫ��fmt03_to_int64s
/*
BOOLEAN fmt03_to_int64s(INT8U BCD2[4],INT64S *int64s)
{
    if(is_valid_bcd(BCD2,3) == FALSE)
    //if(BCD2[0] > 0x99)
    {
       *int64s = 0;
       return FALSE;
    }

    *int64s =  (BCD2[0] & 0x0F)       +  (BCD2[0]>>4)*10;
    *int64s += (BCD2[1] & 0x0F)*100   +  (BCD2[1]>>4)*1000 ;
    *int64s += (BCD2[2] & 0x0F)*10000 +  (BCD2[2]>>4)*100000 ;
    *int64s += (BCD2[3] & 0x0F)*1000000 ;

    if(BCD2[3] & 0x40) *int64s *= 1000;

    if(BCD2[3] & 0x10) *int64s *= -1;
    return TRUE;

}
 */
// ul   ��λ: KWh
BOOLEAN ul2fmt03(INT32S ul,INT8U bcd2[4])
{
 static    INT8U ch;
    INT32U  ul_u;

    if(ul < 0)
        ul_u = -ul;
    else
        ul_u = ul;
    
    ch = ul_u % 100;
    bcd2[0] = byte2BCD(ch);
    ul_u = ul_u / 100;

    ch = ul_u % 100;
    bcd2[1] =  byte2BCD(ch);
    ul_u = ul_u / 100;

    ch = ul_u % 100;
    bcd2[2] =  byte2BCD(ch);
    ul_u = ul_u / 100;

    ch = ul_u % 100;
    bcd2[3] =  byte2BCD(ch);

    if(ul < 0)  bcd2[3] |= 0x10;
    return TRUE;
}

/*+++
  ���ܣ�  ���ݸ�ʽ02��������
  ��ע��        BCD��ʮλ|BCD���λ
                G3G2G1S|BCD���λ
---*/
BOOLEAN  fmt02_to_double(INT8U BCD2[2],double *dbVal)
{
     double times;

     if(is_valid_bcd(BCD2,1) == FALSE)
     //if(BCD2[0] > 0x99)
     {
        *dbVal = 0;
        return FALSE;
     }


     switch(BCD2[1]>>5)
     {
     case 0: times = 10000; break;
     case 1: times =  1000; break;
     case 2: times =   100; break;
     case 3: times =    10; break;
     case 4: times =     1; break;
     case 5: times =   0.1; break;
     case 6: times =  0.01; break;
     case 7: times = 0.001; break;
     default: times =    1; break;
     }

     *dbVal =  (BCD2[0] & 0x0F) +  (BCD2[0]>>4)*10 +  (BCD2[1] & 0x0F)*100;

     *dbVal *=times;

     if(BCD2[1] & 0x10) *dbVal *= -1;

     return TRUE;
}



/*+++
    ���ܣ������ݸ�ʽ06�����ݸ�ʽ09   
        XX.XX   -> XX.XXXX
    ��ע����Ҫ���Ƕ���0xEEEE -> 0xEEEEEE
---*/
void  fromFMT06toFMT09(INT8U *fmt06,INT8U *fmt09)
{
   if((fmt06[0]!=0xEE)  || (fmt06[1] != 0xEE))
   {
      fmt09[0]=0x00;
   }
   else
   {
      fmt09[0]=0xEE;
   } 

   fmt09[1]=fmt06[0];
   fmt09[2]=fmt06[1];
}


/*+++
    ���ܣ������ݸ�ʽ11�����ݸ�ʽ14   
         XXXXXX.XX   -> XXXXXX.XXXX   
---*/
void  fromFMT11toFMT14(INT8U *fmt11,INT8U *fmt14)
{
   fmt14[0]=  (fmt11[0]<= 0x99) ? 0x00 : REC_DATA_IS_DEFAULT;

   fmt14[1]=fmt11[0];
   fmt14[2]=fmt11[1];
   fmt14[3]=fmt11[2];
   fmt14[4]=fmt11[3];
}

/*+++
  ���ܣ�fm07ת��Ϊ��ֵ
  ���������ݸ�ʽ07: byte1 (��λ��ʮ��λ)  byte2(��λ��ʮλ)
 
---*/
float  fmt07_to_float(INT8U *fmt07)
{
    INT32U uint;
    INT8U i,ch;

    if(fmt07[0]==0xEE)   
    {
         //�����BCD����         
         return 0.0;
    }

    uint=0;
    for(i=0;i<2;i++)
    {
       ch = fmt07[1-i];
 
       //����Ƿ�����ȷ��BCD����
       if(is_valid_bcd(&ch,1) == FALSE)
       //if(ch>0x99)
       {
            ch=0x00;
       }

       uint = uint*100+ (ch>>4)*10+(ch&0x0F);
    }
    
    return uint*0.1;
}


/*+++
  ���ܣ� ���ݸ�ʽ05 ת��Ϊ����.
  ������
         INT8U  fmt05[2]    %
  ���أ�
         INT16S     ǧ��֮һ

         SXX.XX
---*/
INT16S  fmt05_to_int(INT8U fmt05[2])
{
    INT16S  val;

    //�����ֵ�Ƿ�����
    if(is_valid_bcd(fmt05,1) == FALSE)
    //if(fmt05[0] > 0x99)
    {
        //û������,Ĭ��Ϊ15%
        #ifdef __PROVICE_NEIMENG__
        return 50;
        #else
        return 150;
        #endif
    }

    val = BCD2byte(fmt05[1] & 0x7F);
    val *= 100;
    val += BCD2byte(fmt05[0]);

    if(fmt05[1] & 0x80) val *= -1;
    #ifdef __PROVICE_NEIMENG__
    return 50;
    #else
    return val;
    #endif
}

INT16S  int_to_fmt05(INT8U fmt05[2],INT16S value)
{
    INT16S  val;

    if(value < 0) {val = 0x80;  value*=-1;  }
    else    val = 0x00;

    if(value > 7999) value = 7999;

    ul2bcd(value,fmt05,2);
    fmt05[1] |=  val;

    return val;
}

/*+++
  ���ܣ�fm09ת��Ϊ��ֵ
  ���������ݸ�ʽ09:   SXX.XXXX
            byte0 (ǧ��λ�����λ)
            byte2 (ʮ��λ���ٷ�λ)
            byte3 S(ʮλ����λ)
---*/
BOOLEAN  fmt09_to_double(INT8U *fmt09,double *db_val)
{
    INT32S value;
    BOOLEAN is_valid,sign;

    sign = (fmt09[2] & 0x80) ? TRUE : FALSE;
    fmt09[2] &= 0x7F;

    value = bcd2u32(fmt09,3,&is_valid);
    if(!is_valid) value = 0;
    if(sign) value *=-1;
 
    *db_val = value * 0.0001;

    return is_valid;
}


// fmt09ת��Ϊint32U, ��λ0.0001 ,���֮һ
INT32S fmt09_to_INT32S(INT8U *fmt09)
{
    INT32S value;
    BOOLEAN is_valid,sign;

    sign = (fmt09[2] & 0x80) ? TRUE : FALSE;
    fmt09[2] &= 0x7F;

    value = bcd2u32(fmt09,3,&is_valid);
    if(!is_valid) value = 0;
    if(sign)
    {
        value = -value;
    }
    return value;
}
//// fmt09ת��Ϊint32S, ��λ0.0001 ,���֮һ
//INT32S fmt09_to_INT32S(INT8U *fmt09)
//{
//    INT32S value;
//    BOOLEAN is_valid,sign;
//
//    sign = (fmt09[2] & 0x80) ? TRUE : FALSE;
//    fmt09[2] &= 0x7F;
//
//    value = bcd2u32(fmt09,3,&is_valid);
//    if(!is_valid) value = 0;
//
//    if(sign == TRUE)
//    {
//        value = -value;
//    }
//
//    return value;
//}

/*+++
  ���ܣ�����cmdDate����td
---*/
//TODO:'CommandDate' cannot start a parameter declaration
//void assign_td_value(CommandDate *cmdDate,INT8U *td,INT8U count)
//{
//       td[0] = byte2BCD(cmdDate->minute);
//       td[1] = byte2BCD(cmdDate->hour);
//       td[2] = byte2BCD(cmdDate->day);
//       td[3] = byte2BCD(cmdDate->month);
//       td[4] = byte2BCD(cmdDate->year-2000);
//}
//97���ĵ�ѹ��ʽתΪ376.1�����ݸ�ʽ07
void v_97_to_format07(INT8U voltage[2])
{
    INT8U data[2];
    //��ѹ       ��ʽ��XXX
    //���ݸ�ʽ07  XXX.X
    data[0] = (voltage[0] & 0x0F)<<4;
    data[1] = (voltage[0] & 0xF0)>>4;
    data[1] += (voltage[1] & 0x0F)<<4;
    mem_cpy(voltage,data,2);
}

//97���ĵ�����ʽתΪ376.1�����ݸ�ʽ25
void i_97_to_format25(INT8U i[3])
{
    INT8U data[3];
    // ����    ��ʽ  XX.XX
    //���ݸ�ʽ25  XXX.XXX
    data[0] = (i[0] & 0x0F)<<4;
    data[1] = (i[0] & 0xF0)>>4;
    data[1] += (i[1] & 0x0F)<<4;
    data[2] = (i[1] & 0xF0)>>4;
    mem_cpy(i,data,3);
}

//97�������޹���ʽתΪ376.1�����ݸ�ʽ09
void pywg_97_to_format09(INT8U pwg[3])
{
    INT8U data[3];
    //97��ǰ�޹�����   XX.XX
    //���ݸ�ʽ09��SXX.XXXX
    data[0] = 0;
    mem_cpy(data+1,pwg,2);
    mem_cpy(pwg,data,3);
}
INT16U toDA(INT16U iVal)
{
   INT16U ret;
   INT8U low = (iVal-1) % 8;
   INT8U high = (iVal-1) /8;
   INT8U mask;
   ret=0;
   mask=1;

   if(iVal == 0 && low==0) return ret;
   ret=(high+1)<<8;
   while(low--)
   {
      mask<<=1;
   }
   ret|=mask;
   return ret;
}
INT16U DTtoInt(INT16U DT)
{
   INT16U high = (DT >> 8) & 0xFF;
   INT16U low = DT & 0xFF;

   high <<=3;  //����8
   do
   {
      high ++;
      low>>=1;
   }while(low > 0);
   return high;
}
//������ݸ�ʽA1�������Ƿ���ȷ������ȷ��������
void check_A1_week(INT8U *bcd)
{
    INT8U weekofday;
    if(check_is_all_ch(bcd,6,0xFF)) return;
    if(check_is_all_ch(bcd,6,0xEE)) return;
    if(check_is_all_ch(bcd,6,0x00)) return;

    weekofday=weekDay(BCD2byte(bcd[5]),BCD2byte(bcd[4] & 0x1F),BCD2byte(bcd[3]));
    bcd[4] &= 0x1F;
    bcd[4] |=(weekofday<<5);

}

/*+++
 ���ܣ� �Ӷ˿�ͨ�ſ������еõ������ʺ�У�鷽ʽ 
---*/
//void get_port_baudrate_parity(INT16U *BaudRate,INT8U *parity,PORT_COMM_CTRL port_ctrl)
//{
//    //���ü����˿ڵĲ�����
//    switch(port_ctrl.baudrate)
//    {
//    case 0: *BaudRate =   300; break;
//    case 1: *BaudRate =   600; break;
//    case 2: *BaudRate =  1200; break;
//    case 3: *BaudRate =  2400; break;
//    case 4: *BaudRate =  4800; break;
//    case 5: *BaudRate =  7200; break;
//    case 6: *BaudRate =  9600; break;
//    case 7:
//    default:*BaudRate = 19200; break;
//    }
//
//    //У��λ
//    if(port_ctrl.has_parity)
//    {
//        if(port_ctrl.parity)
//        {
//            *parity = UART_PARITY_ODD;
//        }
//        else
//        {
//            *parity = UART_PARITY_EVEN;
//        }
//    }
//    else
//    {
//        *parity = UART_PARITY_NONE;
//    }
//}


/*+++
  ���ܣ� �����¼�ʱ��  ��-ʱ-��-��-��
  ������ 
---*/
void   set_event_datetime(INT8U *event)
{
	tagDatetime dt;
    INT16U idx;
    tpos_datetime(&dt);
    for(idx=0; idx<=4;idx++)
    {
       event[idx] = byte2BCD(dt.value[idx+MINUTE]);      
    }
}

/*+++
  ���ܣ� �ն˲�����У�飬ģ255+0xAA�ļ��㷽ʽ
  ������
---*/

INT8U crc16_param(INT8U *data,INT16U datalen)
{
    INT16U idx;
    INT8U cs;

    cs = 0;

    for(idx = 0;idx < datalen;idx++)
    {
        cs += data[idx];
    }

    cs += 0xAA;

    return cs;
}

//��������97��ѷ�ƽ�ȼ�ת���ɼ��ƽ��
void zfpgj2zjfpg(INT8U *data,INT8U datalen,INT8U *fl_count)
{
    INT8U tmp[35]={0};
    if(*fl_count < 3) return;
    mem_cpy(tmp,data+datalen*1,datalen*3);
    if(*fl_count == 4)  //��ƽ�ȼ�ת���ɼ��ƽ��
    {
    mem_cpy(data+datalen*1,data+datalen*4,datalen);
    }
    else if(*fl_count == 3) //��ƽ��ת���ɼ��ƽ��
    {
        *fl_count = 4;
        mem_set(data+datalen*1,datalen,REC_DATA_IS_DENY);
    }
    mem_cpy(data+datalen*2,tmp,datalen*3);
}

//��������97��ѷ�ƽ�ȼ�ת���ɼ��ƽ��
void convert_phy_data_zfpgj2zjfpg(INT8U *data,INT8U datalen)
{
    INT8U tmp[35]={0};

    if(datalen>35) return;
    mem_cpy(tmp,data+(datalen+5)*4+5,datalen);
    mem_cpy(data+(datalen+5)*4+5,data+(datalen+5)*3+5,datalen);
    mem_cpy(data+(datalen+5)*3+5,data+(datalen+5)*2+5,datalen);
    mem_cpy(data+(datalen+5)*2+5,data+(datalen+5)*1+5,datalen);
    mem_cpy(data+(datalen+5)*1+5,tmp,datalen);
}

//��������97��ѷ��ת���ɼ��ƽ��
void convert_phy_data_zfg2zjfpg(INT8U *data,INT8U datalen)
{
    INT8U tmp[35]={0};

    if(datalen>35) return;
    mem_cpy(tmp,data+(datalen+5)*2+5,datalen);
    mem_cpy(data+(datalen+5)*2+5,data+(datalen+5)*1+5,datalen);
    mem_set(data+(datalen+5)*1+5,datalen,REC_DATA_IS_DENY);
    mem_cpy(data+(datalen+5)*4+5,tmp,datalen);

}


//�����������й�������ʾֵ�Ӻ� ������������data1��
void computer_zfxyg_add(INT8U *data1,INT8U *data2,INT8U fl,INT8U len)
{
    INT32U num1,num2;
    INT8U idx,idx1,pos;
    BOOLEAN is_valid1,is_valid2;

    pos = 0;
    if(len > 4) pos = len - 4;

    if(fl>10) fl = 4;
    
    for(idx = 0; idx < fl+1; idx++)
    {
        num1 = bcd2u32(data1+idx*len+pos,len-pos,&is_valid1);
        num2 = bcd2u32(data2+idx*len+pos,len-pos,&is_valid2);

        if(is_valid1 && is_valid2)
        {
            num1 += num2;

           //��ֹ�������������
            num2 = 1;
            for(idx1=0;idx1<len-pos;idx1++) num2 *= 100;
            num1 = num1 % num2;

            ul2bcd(num1,data1+idx*len+pos,len-pos);
        }
    }

}


/**************
����ת���ӿ�
 * format:  ת����ʽ��
 * pResp��  ����ת��������
 * param��  ���ݴ������ �޲�����ֵΪNULL
 * ����ֵΪ���ݳ���
**************/
/*
INT16U   convert_645_to_376(INT16U format,INT8U *pResp,INT8U *param)
{
    INT8U data[50]={0};

    if(!format)
    {
        return TRUE;
    }

    switch(format)
    {
        case REC_DATE:
            //set_event_datetime(pResp);
            break;
        case Energy_3761FMT14_BLOCK:
            for(;;)
            {
                for(i=0; i<4; i++)
                pResp[4-i] = pResp[3-i];
                pResp[0] = 0x00;
            }
            break;
        default:
            break;


    }
    return TRUE;

} */

//��ȡfnԤ�Ƴ���
INT16U  get_fn_len(objRequest *pRequest,tagFN_ITEM *pFn)
{

    INT16U len,pfn_num;
    INT8U idx;

    len =0;

    for(idx=0;idx<pRequest->item_count;idx++)
    {
        pFn = &((tagFN_ITEM*)(pRequest->fn))[idx];
        if(0xFF==pFn->num)
        {
            pfn_num = ((READ_CONTEXT *)(pRequest->context))->Td[6];
        }
        else
        {
            pfn_num = pFn->num;
        }
        len += pFn->len*pfn_num;
    }
    return len;
}
