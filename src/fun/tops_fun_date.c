#include "../main_include.h"

//ϵͳʱ�䣺 �Ѿ���ȥ�˶�����
INT32U  second_elapsed(INT32U last_10ms_tick)
{
   last_10ms_tick =  system_get_tick10ms() - last_10ms_tick;

   last_10ms_tick = (last_10ms_tick ) / 100;

   return  last_10ms_tick;
}

//ϵͳʱ�䣺 �Ѿ���ȥ�˶���10����
INT32U  time_elapsed_10ms(INT32U last_10ms_tick)
{
   last_10ms_tick =  system_get_tick10ms() - last_10ms_tick;

   //last_10ms_tick = (last_10ms_tick ) / 100;

   return  last_10ms_tick;
}

INT32U  second_diff(INT32U cur_10ms_tick,INT32U begin_10ms_tick)
{
    cur_10ms_tick -= begin_10ms_tick;
    begin_10ms_tick =  cur_10ms_tick/ 100;

    return begin_10ms_tick;
}

//ϵͳʱ�䣺 �Ѿ���ȥ�˶��ٷ���, ���ﲻ�ܲ����������룬ȷ���������㹻�ġ�

INT32U  minute_elapsed(INT32U begin_10ms_tick)
{

   begin_10ms_tick =  system_get_tick10ms() - begin_10ms_tick;

    begin_10ms_tick /= 6000;
    return begin_10ms_tick;

}
//---------------------------------------------------------------------------
//  ������-��-�� ���������ڼ� 
//  �̶�����Ϊ20����
//  ������Ϊ0������1Ϊ1
//  2006-6-12 ����Ϊ����1Ϊ1��������Ϊ7
//---------------------------------------------------------------------------
INT8U weekDay(INT8U year,INT8U month,INT8U day)
{
    //���գ�Zeller����ʽ����w=y+[y/4]+[c/4]-2c+[26(m+1)/10]+d-1

    //��ʽ�еķ��ź������£�w�����ڣ�c�����ͣ�y���꣨��λ������
    //  m���£�m���ڵ���3��С�ڵ���14��
    // ���ڲ��չ�ʽ�У�ĳ���1��2��Ҫ������һ���13��14�������㣬
    //  ����2003��1��1��Ҫ����2002���13��1�������㣩��
    //  d���գ�[ ]����ȡ������ֻҪ�������֡�

    int tmpint;
    INT8U cent;


    cent = 20;


    if(month < 3)
    {
       month += 12;
       tmpint  = year + cent*100 -1;
       year = tmpint-cent*100;
       cent = tmpint/100;
    }

    tmpint = year;
    tmpint += (year>>2);
    tmpint += (cent >>2);
    tmpint += 26*(month+1)/10;

   tmpint += day -1;

   tmpint -= (cent <<1);

   //���������С��0�����ü�7�ķ��������ת��
   while(tmpint < 0) tmpint +=7;


   cent =  tmpint % 7;

   //2006-6-12 ,��������0����Ϊ������7
   if(cent == 0) cent = 7;

   return cent;

}
BOOLEAN   isLeapYear(INT16U year)
{
    BOOLEAN res;

    res = FALSE;
    if(0 ==  (year%4))
    {
         if(year%100!=0) res = TRUE;

         if(0 == (year % 400)) res = TRUE;
    }
    return res;
}

//
//   �õ�ĳ�µ�����
// 
INT8U getMonthDays(INT16U year,INT8U month)
{
    if(month==2)
    {
       if(TRUE == isLeapYear(year)) return 29;
       return 28;
    }    
    if((month==4)  || (month==6)  || (month==9)  ||   (month==11))
    {
       return 30;
    } 
    return 31;
}




/*+++
  ���ܣ�����ָ��������1900-1-1�ȹ�ȥ������
  ������
        INT16U year,    ��λ�꣬>=1900
        INT8U month,    1-12
        INT8U day       1-31
  ���أ�
        INT16U  ���1900-1-1��ȥ������
  ������
        1���꡷2000��ֱ�Ӵ�2000�꿪ʼ���㡣
---*/
INT16U  getPassedDays(INT16U year,INT8U month,INT8U day)
{
   INT16U days,cur_year;

   days = 0;
   cur_year = 1900;
   if(year >= 2000)
   {
      days += 36525;            // ��1900��1999�������
      cur_year = 2000;
   }

   //�ۼƾ������������
   for(;cur_year < year; cur_year++)
   {
      if(TRUE == isLeapYear(cur_year)) days += 366;
      else   days += 365;
   }

   //�ۼӵ��µ�����
   month &= 0x0F;    //(���·����Ƶ����ֽ�)
   for(cur_year=1;cur_year<month;cur_year++)
   {
       days += getMonthDays(year,cur_year);
   }

   //�ۼ�����
   days += (day-1);

   return days;
}
/*+++
  ���ܣ�������1900-1-1 0��0��0 ���㾭�������ֵ
  ������
         INT8U    datetime[6]    HEX   ��-��-ʱ-��-��-��
         INT16U   centry        ����    1900,2000
---*/
INT64U   getPassedSeconds(tagDatetime *dt,INT16U centry)
{
    INT64U  passed_sec;
    INT16U  year;

    year = dt->year + centry;

    passed_sec  = dt->hour*3600;
	passed_sec += dt->minute*60;
	passed_sec += dt->second;

    passed_sec +=   (86400ULL) * getPassedDays(year,dt->month,dt->day);

    return passed_sec;
}
/*+++

 ����: ��������������
 ����: 1)���ӵ�����С��29��

---*/

void  date_add_days_ext(INT8U *year,INT8U *month,INT8U *day,INT8U days,INT16U centry)
{
    INT8U month_days;

     //����ֱ�Ӽ���Ҫ���ӵ�����
     *day += days;
    
     //�ж��Ƿ񳬹��˱��µ���ĩһ��
     month_days =  getMonthDays((INT16U)( centry+(*year)),*month);
     if(*day > month_days )
     {
          *day -= month_days;
          (*month)++;
          
           //����·��Ƿ񳬹���12��,�ﵽ����һ��
           if(*month > 12)
           {
               (*year)++;
               *month = 1;
           }
     }

}

/*+++

 ����: ��������������
 ����: 1)���ӵ�����С��29��

---*/

void  date_add_days(INT8U *year,INT8U *month,INT8U *day,INT8U days)
{
	date_add_days_ext(year,month,day,days,2000);
}
/*+++

 ����: ��ʱ�����ӹ̶�����

---*/
void datetime_add_seconds(INT8U *year,INT8U *month,INT8U *day,INT8U *hour,INT8U *minute,INT8U *second,INT32U value)
{
    INT32U residue;

    residue=value;

    //�����룬���������
    value=residue/60;
    residue=residue%60;

    *second+=residue;
    if(*second>=60)
    {
      *second-=60;
      value++;
    }

    //���ӷ��ӣ�����Сʱ��
    residue=value;
    value=residue/60;
    residue=residue%60;

    *minute+=residue;
    if(*minute>=60)
    {
      *minute-=60;
       value++;
    }

    //����Сʱ����������
    residue=value;
    value=residue/24;
    residue=residue%24;

    *hour+=residue;
    if(*hour>=24)
    {
      *hour-=24;
       value++;
    }
   //��������
   date_add_days(year,month,day,value);
}
/*+++

 ����: ��ʱ�����ӹ̶�������

---*/
void datetime_add_minutes(INT8U *year,INT8U *month,INT8U *day,INT8U *hour,INT8U *minute,INT32U value)
{
    INT32U residue;

    //���ӷ��ӣ�����Сʱ��
    residue=value;
    value=residue/60;
    residue=residue%60;

    *minute+=residue;
    if(*minute>=60)
    {
      *minute-=60;
       value++;
    }

    //����Сʱ����������
    residue=value;
    value=residue/24;
    residue=residue%24;

    *hour+=residue;
    if(*hour>=24)
    {
      *hour-=24;
       value++;
    }
   //��������
   date_add_days(year,month,day,value);
}
/*+++

 ����: ��������������
 ����: 1)���ӵ�����С��12

---*/

void  date_add_months(INT8U *year,INT8U *month,INT8U val)
{
    //INT8U month_days;

     *month += val;

     //����·��Ƿ񳬹���12��,�ﵽ����һ��
     if(*month > 12)
     {
         (*year)++;
         *month = 1;
     }

}

/*+++

 ����: ��ʱ�����ӹ̶�Сʱ��

---*/
void datetime_add_hours(INT8U *year,INT8U *month,INT8U *day,INT8U *hour,INT32U value)
{
    INT32U residue;


    //����Сʱ����������
    residue=value;
    value=residue/24;
    residue=residue%24;

    *hour+=residue;
    if(*hour>=24)
    {
      *hour-=24;
       value++;
    }
   //��������
   date_add_days(year,month,day,value);
}
void cosem_datetime_add_ti(INT8U *cosem_bin,INT8U ti[3])
{
    INT8U datetime_bin[6]={0};
    INT8U pos=0;
    INT16U year;
    INT32U seconds;

    year=cosem_bin[0]*0x100+cosem_bin[1];
    datetime_bin[YEAR]       = year-2000;
    datetime_bin[MONTH]      = cosem_bin[2];
    datetime_bin[DAY]        = cosem_bin[3];
    datetime_bin[HOUR]       = cosem_bin[4];
    datetime_bin[MINUTE]     = cosem_bin[5];
    datetime_bin[SECOND]     = cosem_bin[6];

    switch(ti[0])
    {
    case 0:
        seconds=cosem_bin2_int16u(ti+1);
        datetime_add_seconds(&datetime_bin[YEAR],&datetime_bin[MONTH],&datetime_bin[DAY],&datetime_bin[HOUR],&datetime_bin[MINUTE],&datetime_bin[SECOND],seconds);
        break;
    case 1:
        seconds=(cosem_bin2_int16u(ti+1))*60;
        datetime_add_seconds(&datetime_bin[YEAR],&datetime_bin[MONTH],&datetime_bin[DAY],&datetime_bin[HOUR],&datetime_bin[MINUTE],&datetime_bin[SECOND],seconds);
        break;
    case 2:
        seconds=(cosem_bin2_int16u(ti+1))*3600;
        datetime_add_seconds(&datetime_bin[YEAR],&datetime_bin[MONTH],&datetime_bin[DAY],&datetime_bin[HOUR],&datetime_bin[MINUTE],&datetime_bin[SECOND],seconds);
        break;
    case 3:
        seconds=cosem_bin2_int16u(ti+1);
        date_add_days(&datetime_bin[YEAR],&datetime_bin[MONTH],&datetime_bin[DAY],seconds);
        break;
    case 4:
        seconds=cosem_bin2_int16u(ti+1);
        datetime_bin[MONTH]++;
        if(datetime_bin[MONTH]>12)
        {
            datetime_bin[MONTH]=1;
            datetime_bin[YEAR]++;
        }
        break;
    default:
        return;
    }

    year=2000+datetime_bin[YEAR];
    cosem_bin[pos++] = year>>8;
    cosem_bin[pos++] = year;
    cosem_bin[pos++] = datetime_bin[MONTH];
    cosem_bin[pos++] = datetime_bin[DAY];
    cosem_bin[pos++] = datetime_bin[HOUR];
    cosem_bin[pos++] = datetime_bin[MINUTE];
    cosem_bin[pos++] = datetime_bin[SECOND];
}

void date_minus_days(INT8U *year,INT8U *month,INT8U *day,INT8U days)
{
     INT16S what_day;
     INT8U month_days;

     //����ֱ�Ӽ�ȥ������,���ܳ��ָ�ֵ
     what_day = *day;
     what_day -= days;

     if(what_day >= 1)
     {
         *day = what_day;
         return;
     }

     //���ڼ������ϸ���
     (*month) --;
     if((*month) == 0x00)
     {
         //��һ���12��
         *month = 12;
         if((*year) > 0) (*year)--;
     }
     month_days =  getMonthDays((INT16U)(2000+(*year)),*month);
    
     *day = month_days + what_day;
}

void   date_minus_months(INT8U *year,INT8U *month,INT8U months)
{
     INT16S what_month;

     what_month = *month;
     what_month -= months;

     if((*month) == 0x00)
     {
         //��һ���12��
         *month = 12;
         if((*year) > 0) (*year)--;
     }
     *month = what_month;
}

/*+++

 ����: ��ʱ���ȥ�̶�����

---*/
void datetime_minus_seconds(INT8U *year,INT8U *month,INT8U *day,INT8U *hour,INT8U *minute,INT8U *second,INT32U value)
{
    INT32U count;
    INT32U residue;
  
    residue=value;

    value = residue / 86400;
    residue = residue % 86400;
    count = (INT32U)*hour * 3600 + (INT32U)*minute * 60 + (INT32U)*second;
    if(count < residue) 
    {
        count += 86400;
        value ++; 
}

    count = count - residue;
    *hour = count / 3600;
    count = count - (INT32U)*hour * 3600;
    *minute = count / 60;
    *second = count - (INT32U)*minute * 60;

    //��ȥ����
    date_minus_days(year,month,day,value);
}
/*+++

 ����: ��ʱ���ȥ�̶�������

---*/
void datetime_minus_minutes(INT8U *year,INT8U *month,INT8U *day,INT8U *hour,INT8U *minute,INT32U value)
{
    INT32U count;
    INT32U residue;
  
    residue=value;

    value = residue / 1440;
    residue = residue % 1440;
    count = *hour * 60 + *minute;
    if(count < residue) 
    {
        count += 1440;
        value ++; 
    }

    count -= residue;
    *hour = count / 60;
    *minute = count - *hour * 60;

    //��ȥ����
    date_minus_days(year,month,day,value);
}
/*+++

 ����: ��ʱ���ȥ�̶�Сʱ��

---*/
void datetime_minus_hours(INT8U *year,INT8U *month,INT8U *day,INT8U *hour,INT32U value)
{
    INT32U count;
    INT32U residue;
  
    residue=value;

    value = residue / 24;
    residue = residue % 24;
    count = *hour;
    if(count < residue) 
    {
        count += 24;
        value ++; 
    }

    count -= residue;
    *hour = count;

    //��ȥ����
    date_minus_days(year,month,day,value);
}
//
//   ����ʱ��������Чʱ��,�����ܳ���1��ķ���
//

void commandDateAddMinute(CommandDate *cmdDate,INT8U minute)
{
    INT16U monthdays;

    cmdDate->minute += minute;

    if(cmdDate->minute >= 60)
    {
        cmdDate->hour += cmdDate->minute / 60;
        cmdDate->minute = cmdDate->minute % 60;
    }
    if(cmdDate->hour >= 24)
    {
        cmdDate->day +=  cmdDate->hour / 24;
        cmdDate->hour =  cmdDate->hour % 24;
    }

    //����Ƿ������ĩ���������ת�����µ�1��
    monthdays = getMonthDays(cmdDate->year,cmdDate->month);
    if(cmdDate->day > monthdays)
    {
        cmdDate->month++;
        cmdDate->day = 1;
    }
    if(cmdDate->month > 12)
    {
        cmdDate->month=1;
        cmdDate->year++;
    }
}

/*+++
  ���ܣ��õ�����
  ������
         INT8U  yesterday[3]  BIN  ��-��-��
  ���أ�
          ��
---*/
void get_yesterday(INT8U yesterday[3])
{
    tagDatetime datetime;
    
    os_get_datetime(&datetime);
    yesterday[0] = datetime.day;
    yesterday[1] = datetime.month;
    yesterday[2] = datetime.year;

    date_minus_days(&yesterday[2],&yesterday[1],&yesterday[0],1);
}

/*+++
  ���ܣ��õ�ǰ��
  ������
         INT8U  yesterday[3]  BIN  ��-��-��
  ���أ�
          ��
---*/
void get_before_yesterday(INT8U yesterday[3])
{
    tagDatetime datetime;
    
    os_get_datetime(&datetime);
    yesterday[0] = datetime.day;
    yesterday[1] = datetime.month;
    yesterday[2] = datetime.year;

    date_minus_days(&yesterday[2],&yesterday[1],&yesterday[0],2);
}

/*+++
  ���ܣ��õ�����
  ������
         INT8U  former_month[2]  BIN  ��-��
  ���أ�
          ��
---*/
void get_former_month(INT8U former_month[2])
{
    tagDatetime datetime;
    
    os_get_datetime(&datetime);
    if(datetime.month==1)
    {
        former_month[0] = 12;
        former_month[1] = datetime.year-1;
    }
    else
    {
        former_month[0] = datetime.month-1;
        former_month[1] = datetime.year;
    }
}

/*+++
  ���ܣ��õ�����������
  ������
         INT8U  former_month[2]  BIN  ��-��
  ���أ�
          ��
---*/
void get_former_month_from_param(INT8U former_month[2])
{
    if(former_month[0]==1)
    {
        former_month[0] = 12;
        former_month[1] = former_month[1]-1;
    }
    else
    {
        former_month[0] = former_month[0]-1;
    }
}

/*+++
  ���ܣ��ж��Ƿ�Ϊ����,������
  ������
        INT8U year,          BIN
        INT8U month,         BIN
        INT8U day            BIN
  ���أ�
        TRUE / FALSE
---*/
BOOLEAN is_yesterday(INT8U year,INT8U month,INT8U day)
{
   INT8U yesterday[3];

   get_yesterday(yesterday);

   if(  (day != yesterday[2]) ||  (month != yesterday[1])  || (year != yesterday[0]) )
   {
      return FALSE;
   }
   return TRUE;
}
//
//   ����ʱ��������Чʱ��
//

void commandDateMinusMinute(CommandDate *cmdDate,INT8U minute)
{
    INT8U monthdays;


    cmdDate->minute -= minute;
    if(cmdDate->minute < 0)
    {
        cmdDate->minute += 60;
        cmdDate->hour --;
    }
    if(cmdDate->hour < 0)
    {
        cmdDate->hour += 24;
        cmdDate->day--;
    }
    if(cmdDate->day <= 0)
    {
        if(cmdDate->month >1)
           monthdays = getMonthDays(cmdDate->year,cmdDate->month-1);
        else
           monthdays = 31;
        cmdDate->month--;
        cmdDate->day+=monthdays;
    }
    if(cmdDate->month <=0)
    {
       cmdDate->month=12;
       cmdDate->year--;
    }
}
//�ú���������ĺ��������Ǵ����޶������ģ�hour��Ҫ��24���ڣ������в�δ���ж����Ͻ�
void commandDateMinusHour(CommandDate *cmdDate,INT8U hour)
{
    INT8U monthdays;


    cmdDate->hour -= hour;
    if(cmdDate->hour < 0)
    {
        cmdDate->hour += 24;
        cmdDate->day--;
    }
    if(cmdDate->day <= 0)
    {
        if(cmdDate->month >1)
           monthdays = getMonthDays(cmdDate->year,cmdDate->month-1);
        else
           monthdays = 31;
        cmdDate->month--;
        cmdDate->day+=monthdays;
    }
    if(cmdDate->month <=0)
    {
       cmdDate->month=12;
       cmdDate->year--;
    }
}

//
//  �Ƚ������������ڵ��Ⱥ�  =0 ���  <0 cmdDate1 ��cmdDate2��  > 0 cmdDate1 ��cmdDate2��
// 
INT16S commandDateCompare(CommandDate *cmdDate1,CommandDate *cmdDate2)
{


    if(cmdDate1->year < cmdDate2->year) return -1;
    else if(cmdDate1->year > cmdDate2->year) return 1;
 
    if(cmdDate1->month < cmdDate2->month) return -1;
    else if(cmdDate1->month > cmdDate2->month) return 1;
    
    if(cmdDate1->day < cmdDate2->day) return -1;
    else if(cmdDate1->day > cmdDate2->day) return 1;
   
    if(cmdDate1->hour < cmdDate2->hour) return -1;
    else if(cmdDate1->hour > cmdDate2->hour) return 1;

    if(cmdDate1->minute < cmdDate2->minute) return -1;
    else if(cmdDate1->minute > cmdDate2->minute) return 1;
    return 0;
}
INT16S DateTimeCompare(tagDatetime *Date1,tagDatetime *Date2)
{

    if(Date1->year < Date2->year ) return -1;
    else if(Date1->year > Date2->year ) return 1;
 
    if(Date1->month < Date2->month ) return -1;
    else if(Date1->month > Date2->month ) return 1;
    
    if(Date1->day < Date2->day ) return -1;
    else if(Date1->day > Date2->day ) return 1;
   
    if(Date1->hour < Date2->hour ) return -1;
    else if(Date1->hour > Date2->hour ) return 1;

    if(Date1->minute < Date2->minute ) return -1;
    else if(Date1->minute > Date2->minute ) return 1;

    if(Date1->second < Date2->second ) return -1;
    else if(Date1->second > Date2->second ) return 1;

    return 0;
}

//
//   HEX��  ��[0]-ʱ[1]-��[2]-��[3]-��[4]
void setCommandDate(CommandDate *cmdDate,INT8U *cmdDateBytes)
{

    cmdDate->year = 2000 + cmdDateBytes[4];
    cmdDate->month = cmdDateBytes[3];
    cmdDate->day =  cmdDateBytes[2];
    cmdDate->hour =  cmdDateBytes[1];
    cmdDate->minute =  cmdDateBytes[0];

}
//
//   BCD��  ��[0]-ʱ[1]-��[2]-��[3]-��[4]
void setCommandBCDDate(CommandDate *cmdDate,INT8U *cmdDateBytes)
{
    cmdDate->year = 2000 + BCD2byte(cmdDateBytes[4]);
    cmdDate->month = BCD2byte(cmdDateBytes[3]);
    cmdDate->day =  BCD2byte(cmdDateBytes[2]);
    cmdDate->hour =  BCD2byte(cmdDateBytes[1]);
    cmdDate->minute =  BCD2byte(cmdDateBytes[0]);

}

/*+++
 ���ܣ���������Ƿ�Ϊ���죬BCD   ��-��-��
---*/
BOOLEAN is_today_bcd(INT8U bcd_date[3])
{
    tagDatetime datetime;
    
    os_get_datetime(&datetime);
    if(BCD2byte(bcd_date[0]) != datetime.day)   return FALSE;
    if(BCD2byte(bcd_date[1]) != datetime.month) return FALSE;
    if(BCD2byte(bcd_date[2]) != datetime.year)  return FALSE;

    return TRUE;
}

BOOLEAN is_today(INT8U date[3])
{
    tagDatetime datetime;
    
    os_get_datetime(&datetime);
    if(date[0] != datetime.day)   return FALSE;
    if(date[1] != datetime.month) return FALSE;
    if(date[2] != datetime.year)  return FALSE;

    return TRUE;

}


void set_today_bcd(INT8U date[3])
{
    tagDatetime datetime;
    
    os_get_datetime(&datetime);
    date[0] = byte2BCD(datetime.day);
    date[1] = byte2BCD(datetime.month);
    date[2] = byte2BCD(datetime.year);

}

//����:
BOOLEAN set_today_bcd4(INT8U date[4])
{
    tagDatetime datetime;
    
    os_get_datetime(&datetime);
    date[0] = byte2BCD(datetime.minute);
    date[1] = byte2BCD(datetime.hour);
    date[2] = byte2BCD(datetime.day);
    date[3] = byte2BCD(datetime.month);
    return TRUE;
}
BOOLEAN is_former_month_bcd(INT8U date[2])
{
   INT8U for_month,for_year;
    tagDatetime datetime;
    
    os_get_datetime(&datetime);

   if(datetime.month == 1)
   {
      for_month = 12;
      for_year = datetime.year - 1;
   }
   else
   {
      for_month = datetime.month - 1;
      for_year = datetime.year;
   }
   if(BCD2byte(date[0]) != for_month) return FALSE;
   if(BCD2byte(date[1]) != for_year)  return FALSE;

   return TRUE;

}




/*+++
  ����: ��BCD��ʽʱ��תΪ�ֽڸ�ʽ
  ����: 
      INT8U *dt Ŀ��ת��ֵ
      INT8U *nValue ��ת��ֵ�������� �ӵ͵��߸�ʽΪ:��-��-ʱ-����-��-��-��
  ����: 
---*/
void RTCBCD2DateTime(tagDatetime *dt,INT8U *nValue)
{
     dt->second = BCD2byte(nValue[0]);
     dt->minute = BCD2byte(nValue[1]);
     dt->hour =   BCD2byte(nValue[2]);
     dt->weekday =BCD2byte(nValue[3]);	
     dt->day =    BCD2byte(nValue[4]);
     dt->month =  BCD2byte(nValue[5]);
     dt->year =   BCD2byte(nValue[6]);
}
void  DateTime2RTCBCD(tagDatetime *dt,INT8U *nValue)
{
     nValue[0]=byte2BCD(dt->second);
     nValue[1]=byte2BCD(dt->minute);
     nValue[2]=byte2BCD(dt->hour);
     nValue[3]=byte2BCD(dt->weekday);	
     nValue[4]=byte2BCD(dt->day);
     nValue[5]=byte2BCD(dt->month);
     nValue[6]=byte2BCD(dt->year);
}
void  DateTime2645BCD(tagDatetime *dt,INT8U *nValue)
{
     nValue[0]=byte2BCD(dt->second);
     nValue[1]=byte2BCD(dt->minute);
     nValue[2]=byte2BCD(dt->hour);
     nValue[3]=byte2BCD(dt->day);
     nValue[4]=byte2BCD(dt->month);
     nValue[5]=byte2BCD(dt->year);
}
//�Ƚ�����ʱ�䣬��������Ƿ����s��,s<60
BOOLEAN compare_time_second(tagDatetime *t1,tagDatetime *t2,INT8U s)
{
    INT32U   passed_min1;
    INT32U   passed_min2;
    INT16U   s1,s2;
    BOOLEAN is_t1;

    s1=t1->second;
    s2=t2->second;

    t1->second=0;
    t2->second=0;

    passed_min1 =  getPassedMinutes(t1,2000);
    passed_min2 =  getPassedMinutes(t2,2000);

    t1->second=s1;
    t2->second=s2;

    if(passed_min1 > passed_min2)
    {
       is_t1 = TRUE;
       passed_min1= (passed_min1 - passed_min2);
    }
    else
    {
       is_t1 = FALSE;
       passed_min1=(passed_min2 - passed_min1);
    }

    if(passed_min1 > 1) return TRUE;//��ô�϶�����60s

    if(passed_min1 == 0) //ֻ���벻�ȣ����������
    {
        if(t1->second>t2->second)
           s1=(INT16U)(t1->second-t2->second);
        else
           s1=(INT16U)(t2->second-t1->second);
    }
    else if(is_t1)  //t1�� ������t1-t2
    {
           s1=(INT16U)(t1->second+60-t2->second);
    }
    else   //t2�� ������t2-t1
    {
           s1=(INT16U)(t2->second+60-t1->second);
    }

    return (s1>s);

}
/*+++
  ���ܣ�������1900-1-1 0��0��0 ���㾭���ķ��Ӳ�ֵ
  ������
         INT8U    datetime[6]    HEX   ��-��-ʱ-��-��-��
         INT16U   centry        ����    1900,2000
---*/
INT32U   getPassedMinutes(tagDatetime *dt,INT16U centry)
{
    INT32U  passed_min;
    INT16U  year;

    year = (INT16U)(dt->year + centry);

    passed_min = dt->hour*60 + dt->minute;
    if(dt->second > 30)  passed_min ++;

    passed_min +=   (1440UL) * getPassedDays(year,dt->month,dt->day);

    return passed_min;
}
/*+++
  ���ܣ�������1900-1-1 0��0��0 ���㾭����Сʱ��ֵ
  ������
         INT8U    datetime[6]    HEX   ��-��-ʱ-��-��-��
         INT16U   centry        ����    1900,2000
---*/
INT32U   getPassedHours(tagDatetime *dt,INT16U centry)
{
    INT32U  passed_hour;
    INT16U  year;

    year = (INT16U)(dt->year + centry);

    passed_hour = dt->hour;

    passed_hour +=   (24UL) * getPassedDays(year,dt->month,dt->day);


    return passed_hour;
}
/*+++
  ���ܣ������������ڵ����
  ������
         INT8U dt1[6],
         INT8U dt2[6]
  ���أ�
         INT32U ���ֵ
---*/
INT32U   diff_sec_between_dt(tagDatetime *dt1,tagDatetime *dt2)
{
    INT64U   passed_sec1;
    INT64U   passed_sec2;

    passed_sec1 =  getPassedSeconds(dt1,2000);
    passed_sec2=  getPassedSeconds(dt2,2000);

    if(passed_sec1 > passed_sec2)
    {
       return (INT32U)(passed_sec1 - passed_sec2);
    }
    return (INT32U)(passed_sec2 - passed_sec1);
}
/*+++
  ���ܣ������������ڵķ��Ӳ�
  ������
         INT8U dt1[6],
         INT8U dt2[6]
  ���أ�
         INT32U ���Ӳ�ֵ
---*/
INT32U   diff_min_between_dt(tagDatetime *dt1,tagDatetime *dt2)
{
    INT32U   passed_min1;
    INT32U   passed_min2;

    passed_min1 =  getPassedMinutes(dt1,2000);
    passed_min2 =  getPassedMinutes(dt2,2000);

    if(passed_min1 > passed_min2)
    {
       return (passed_min1 - passed_min2);
    }
    return (passed_min2 - passed_min1);
}
/*+++
  ���ܣ�����cmdDate����td
---*/
void assign_td_value(CommandDate *cmdDate,INT8U *td,INT8U count)
{
       td[0] = byte2BCD(cmdDate->minute);
       td[1] = byte2BCD(cmdDate->hour);
       td[2] = byte2BCD(cmdDate->day);
       td[3] = byte2BCD(cmdDate->month);
       td[4] = byte2BCD((INT8U)(cmdDate->year-2000));
}

void set_byte2BCD(INT8U* data,INT16U len)
{
    INT16U idx;
    for(idx=0;idx<len;idx++)
    {
       data[idx] = byte2BCD(data[idx]);
    }
}
/*+++
  ���ܣ���1900-1-1 0��0��0 ���������ӹ̶������������
  ������
         INT8U    datetime[6]    ���   ��-��-ʱ-��-��-��
         INT64U   value     ����   ����
---*/
void  addSeconds_at1900(tagDatetime *dt,INT64U value)
{
    INT32U  residue;
    INT32U  val;    
    INT16U  year;
    INT8U   day,temp;    
 
	dt->second	=0;
	dt->minute	=0;
	dt->hour	=0;
	dt->day		=1;
	dt->month	=1;
	dt->year	=0;


    //�����룬���������
    val=value/60;
    temp=value%60;
    dt->second+=temp;

    //���ӷ��ӣ�����Сʱ��
    residue=val;
    val=residue/60;
    temp=residue%60;
    dt->minute+=temp;

    //����Сʱ����������
    residue=val;
    val=residue/24;
    temp=residue%24;
    dt->hour+=temp;

    
    year = 1900;
    if(val>36525)
	{
		val-=36525;   // ��1900��1999�������
		year = 2000;	
	}
    while(val>0)
	{
	    day=28;
		if(val<day)
		{
			day=val;
		}
		val-=day;
		//��������
	    date_add_days_ext(&dt->year,&dt->month,&dt->day,day,year);
		
 	}   
}

void sys_datetime2cosem_datetime(INT8U *cosem_bin)
{
    INT16U year;
    tagDatetime datetime;

    os_get_datetime(&datetime);
    year=datetime.century*100+datetime.year;
    cosem_bin[0]=year>>8;
    cosem_bin[1]=year;
    cosem_bin[2]=datetime.month;
    cosem_bin[3]=datetime.day;
    cosem_bin[4]=datetime.hour;
    cosem_bin[5]=datetime.minute;
    cosem_bin[6]=datetime.second;
}
INT16U set_cosem_datetime_s(INT8U *cosem_bin,INT8U *datetime_bin)
{
	INT16U pos,year;

	pos =0;
	year=2000+datetime_bin[YEAR];

	cosem_bin[pos++] = year>>8;
	cosem_bin[pos++] = year;
	cosem_bin[pos++] = datetime_bin[MONTH];
	cosem_bin[pos++] = datetime_bin[DAY];
	cosem_bin[pos++] = datetime_bin[HOUR];
	cosem_bin[pos++] = datetime_bin[MINUTE];
	cosem_bin[pos++] = datetime_bin[SECOND];
	
	return pos;
}
