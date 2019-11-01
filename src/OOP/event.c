/*
 * event.c
 *
 * Created: 2019-1-24 14:52:43
 *  Author: mgl
 */ 
#include "event.h"
void powerOnOff()
{
	volatile DateTime datetime;
	volatile INT8U  time[7],tmpMin,tmpSec;
	volatile INT8U tmp;
	INT8U idx;
	tpos_datetime(&datetime);
	//mem_cpy(&time,datetime.century*100+datetime.year,2);
	time[0]= (datetime.century*100+datetime.year)/256;
	time[1]= (datetime.century*100+datetime.year)%256;
	time[2]=datetime.month;
	time[3]=datetime.day;
	time[4]=datetime.hour;
	time[5]=datetime.minute;
	time[6]=datetime.second;
	
	if(dev_power_lost()&& (TerminalPowerOnOffState==1))//检测到停电。
	{
		
		//		reportOnOffEvent(0);
		TerminalPowerOnOffState = 0;
		//fread_ertu_params(TERMINAL_POWER_ONOFF,&TerminalPowerOnOffEvent,sizeof(StrTerminalPowerOnOffEventBlock));
		if(TerminalPowerOnOffEvent.eventIndex>=14)
		{
			TerminalPowerOnOffEvent.eventIndex=14;
			for(idx=0;idx<14;idx++)
			{
				TerminalPowerOnOffEvent.event[idx+1].eventIndex[4] = idx;
				mem_cpy(&(TerminalPowerOnOffEvent.event[idx]),&(TerminalPowerOnOffEvent.event[idx+1]),sizeof(StrTerminalPowerOnOff));
			}
			
			TerminalPowerOnOffEvent.event[TerminalPowerOnOffEvent.eventIndex].eventIndex[0] = 0x06;
			mem_set(&(TerminalPowerOnOffEvent.event[TerminalPowerOnOffEvent.eventIndex].eventIndex[1]),3,0x00);
			TerminalPowerOnOffEvent.event[TerminalPowerOnOffEvent.eventIndex].eventIndex[4] = TerminalPowerOnOffEvent.eventIndex;
			TerminalPowerOnOffEvent.event[TerminalPowerOnOffEvent.eventIndex].beginTime[0] = 0x1C;  //开始时间
			mem_cpy(&(TerminalPowerOnOffEvent.event[TerminalPowerOnOffEvent.eventIndex].beginTime[1]),time,7);

			TerminalPowerOnOffEvent.event[TerminalPowerOnOffEvent.eventIndex].endTime[0] = 0x00;  //结束时间  //
			mem_set(&(TerminalPowerOnOffEvent.event[TerminalPowerOnOffEvent.eventIndex].endTime[1]),7,0x00);
			
			TerminalPowerOnOffEvent.event[TerminalPowerOnOffEvent.eventIndex].eventSource[0] = 0x16; //发生源
			TerminalPowerOnOffEvent.event[TerminalPowerOnOffEvent.eventIndex].eventSource[1] = 0;
			TerminalPowerOnOffEvent.event[TerminalPowerOnOffEvent.eventIndex].reportState[0] = 0x1;  //上报状态
			TerminalPowerOnOffEvent.event[TerminalPowerOnOffEvent.eventIndex].reportState[1] = 0;
			TerminalPowerOnOffEvent.event[TerminalPowerOnOffEvent.eventIndex].eventAttribute[0] = 0x04;
			TerminalPowerOnOffEvent.event[TerminalPowerOnOffEvent.eventIndex].eventAttribute[1] = 0x08;
			TerminalPowerOnOffEvent.event[TerminalPowerOnOffEvent.eventIndex].eventAttribute[2] = 0x80;
		}
		else
		{
			//	TerminalPowerOnOffEvent.event[TerminalPowerOnOffEvent.eventIndex].eventIndex = TerminalPowerOnOffEvent.eventIndex;
			TerminalPowerOnOffEvent.event[TerminalPowerOnOffEvent.eventIndex].eventIndex[0] = 0x06;
			mem_set(&(TerminalPowerOnOffEvent.event[TerminalPowerOnOffEvent.eventIndex].eventIndex[1]),3,0x00);
			TerminalPowerOnOffEvent.event[TerminalPowerOnOffEvent.eventIndex].eventIndex[4] = TerminalPowerOnOffEvent.eventIndex;
			TerminalPowerOnOffEvent.event[TerminalPowerOnOffEvent.eventIndex].beginTime[0] = 0x1C;  //开始时间
			mem_cpy(&(TerminalPowerOnOffEvent.event[TerminalPowerOnOffEvent.eventIndex].beginTime[1]),time,7);

			TerminalPowerOnOffEvent.event[TerminalPowerOnOffEvent.eventIndex].endTime[0] = 0x00;  //结束时间
			mem_set(&(TerminalPowerOnOffEvent.event[TerminalPowerOnOffEvent.eventIndex].endTime[1]),7,0x00);
			
			TerminalPowerOnOffEvent.event[TerminalPowerOnOffEvent.eventIndex].eventSource[0] = 0x16; //发生源
			TerminalPowerOnOffEvent.event[TerminalPowerOnOffEvent.eventIndex].eventSource[1] = 0;
			TerminalPowerOnOffEvent.event[TerminalPowerOnOffEvent.eventIndex].reportState[0] = 0x1;  //上报状态
			TerminalPowerOnOffEvent.event[TerminalPowerOnOffEvent.eventIndex].reportState[1] = 0;
			TerminalPowerOnOffEvent.event[TerminalPowerOnOffEvent.eventIndex].eventAttribute[0] = 0x04;
			TerminalPowerOnOffEvent.event[TerminalPowerOnOffEvent.eventIndex].eventAttribute[1] = 0x08;
			TerminalPowerOnOffEvent.event[TerminalPowerOnOffEvent.eventIndex].eventAttribute[2] = 0x80;
			
			TerminalPowerOnOffEvent.eventIndex++;
			system_debug_info("event is store 0           ");
		}
		fwrite_ertu_params(TERMINAL_POWER_ONOFF,&TerminalPowerOnOffEvent,sizeof(StrTerminalPowerOnOffEventBlock));
		//	fwrite_ertu_params(MONTH_BYTES,&gSystemInfo.month_bytes,8);
		//	fwrite_ertu_params(DAY_BYTES,&gSystemInfo.day_bytes,2);
		system_debug_info("event is store1           ");
	}
	else if(!dev_power_lost()&& (TerminalPowerOnOffState==0))
	{
		tpos_datetime(&datetime);
		fread_ertu_params(TERMINAL_POWER_ONOFF,&TerminalPowerOnOffEvent,sizeof(StrTerminalPowerOnOffEventBlock));
		tmpMin = TerminalPowerOnOffEvent.event[TerminalPowerOnOffEvent.eventIndex].beginTime[6]; //得到上次停电时分钟数，用来判断上电有效没
		tmpSec = TerminalPowerOnOffEvent.event[TerminalPowerOnOffEvent.eventIndex].beginTime[7]; //得到上次停电时分钟数，用来判断上电有效没
		reportOnOffEvent(1);
		TerminalPowerOnOffState = 1;
//		tmp =TerminalPowerOnOffEvent.eventIndex;
		if((TerminalPowerOnOffEvent.eventIndex)>=14)
		{
			//tmp =0;
			TerminalPowerOnOffEvent.eventIndex=14;
			for(idx=0;idx<14;idx++)
			{
				TerminalPowerOnOffEvent.event[idx+1].eventIndex[4] = idx;
				mem_cpy(&(TerminalPowerOnOffEvent.event[idx]),&(TerminalPowerOnOffEvent.event[idx+1]),sizeof(StrTerminalPowerOnOff));
			}
			tmpMin = TerminalPowerOnOffEvent.event[TerminalPowerOnOffEvent.eventIndex].beginTime[6]; //得到上次停电时分钟数，用来判断上电有效没
			tmpSec = TerminalPowerOnOffEvent.event[TerminalPowerOnOffEvent.eventIndex].beginTime[7]; //得到上次停电时分钟数，用来判断上电有效没
					
			TerminalPowerOnOffEvent.event[TerminalPowerOnOffEvent.eventIndex].eventIndex[0] = 0x06;
			mem_set(&(TerminalPowerOnOffEvent.event[TerminalPowerOnOffEvent.eventIndex].eventIndex[1]),3,0x00);
			TerminalPowerOnOffEvent.event[TerminalPowerOnOffEvent.eventIndex].eventIndex[4] = TerminalPowerOnOffEvent.eventIndex;
			TerminalPowerOnOffEvent.event[TerminalPowerOnOffEvent.eventIndex].beginTime[0] = 0x1C;  //开始时间
			mem_cpy(&(TerminalPowerOnOffEvent.event[TerminalPowerOnOffEvent.eventIndex].beginTime[1]),&(TerminalPowerOnOffEvent.event[TerminalPowerOnOffEvent.eventIndex-1].beginTime[1]),7);

			TerminalPowerOnOffEvent.event[TerminalPowerOnOffEvent.eventIndex].endTime[0] = 0x1C;  //结束时间
			mem_cpy(&(TerminalPowerOnOffEvent.event[TerminalPowerOnOffEvent.eventIndex].endTime[1]),time,7);
			
			TerminalPowerOnOffEvent.event[TerminalPowerOnOffEvent.eventIndex].eventSource[0] = 0x16; //发生源
			TerminalPowerOnOffEvent.event[TerminalPowerOnOffEvent.eventIndex].eventSource[1] = 1;
			TerminalPowerOnOffEvent.event[TerminalPowerOnOffEvent.eventIndex].reportState[0] = 0x1;  //上报状态
			TerminalPowerOnOffEvent.event[TerminalPowerOnOffEvent.eventIndex].reportState[1] = 0;
			
			TerminalPowerOnOffEvent.event[TerminalPowerOnOffEvent.eventIndex].eventAttribute[0] = 0x04;
			TerminalPowerOnOffEvent.event[TerminalPowerOnOffEvent.eventIndex].eventAttribute[1] = 0x08;
			
			if((((datetime.minute -tmpMin)*60+datetime.second -tmpSec)>60) ||( ((tmpMin - datetime.minute)*60+tmpSec-datetime.second)>60))
			TerminalPowerOnOffEvent.event[TerminalPowerOnOffEvent.eventIndex].eventAttribute[2] = 0xC0;
			else
			TerminalPowerOnOffEvent.event[TerminalPowerOnOffEvent.eventIndex].eventAttribute[2] = 0x80;
		}
		else
		{
			tmpMin = TerminalPowerOnOffEvent.event[TerminalPowerOnOffEvent.eventIndex-1].beginTime[6]; //得到上次停电时分钟数，用来判断上电有效没
			tmpSec = TerminalPowerOnOffEvent.event[TerminalPowerOnOffEvent.eventIndex-1].beginTime[7]; //得到上次停电时分钟数，用来判断上电有效没			
			//	TerminalPowerOnOffEvent.event[TerminalPowerOnOffEvent.eventIndex].eventIndex = TerminalPowerOnOffEvent.eventIndex;
			TerminalPowerOnOffEvent.event[TerminalPowerOnOffEvent.eventIndex].eventIndex[0] = 0x06;
			mem_set(&(TerminalPowerOnOffEvent.event[TerminalPowerOnOffEvent.eventIndex].eventIndex[1]),3,0x00);
			TerminalPowerOnOffEvent.event[TerminalPowerOnOffEvent.eventIndex].eventIndex[4] = TerminalPowerOnOffEvent.eventIndex;
			TerminalPowerOnOffEvent.event[TerminalPowerOnOffEvent.eventIndex].beginTime[0] = 0x1C;  //开始时间
			mem_cpy(&(TerminalPowerOnOffEvent.event[TerminalPowerOnOffEvent.eventIndex].beginTime[1]),&(TerminalPowerOnOffEvent.event[TerminalPowerOnOffEvent.eventIndex-1].beginTime[1]),7);

			TerminalPowerOnOffEvent.event[TerminalPowerOnOffEvent.eventIndex].endTime[0] = 0x1C;  //结束时间
			mem_cpy(&(TerminalPowerOnOffEvent.event[TerminalPowerOnOffEvent.eventIndex].endTime[1]),time,7);
			
			TerminalPowerOnOffEvent.event[TerminalPowerOnOffEvent.eventIndex].eventSource[0] = 0x16; //发生源
			TerminalPowerOnOffEvent.event[TerminalPowerOnOffEvent.eventIndex].eventSource[1] = 1;
			TerminalPowerOnOffEvent.event[TerminalPowerOnOffEvent.eventIndex].reportState[0] = 0x1;  //上报状态
			TerminalPowerOnOffEvent.event[TerminalPowerOnOffEvent.eventIndex].reportState[1] = 0;
			
			TerminalPowerOnOffEvent.event[TerminalPowerOnOffEvent.eventIndex].eventAttribute[0] = 0x04;
			TerminalPowerOnOffEvent.event[TerminalPowerOnOffEvent.eventIndex].eventAttribute[1] = 0x08;
			
			if((((datetime.minute -tmpMin)*60+datetime.second -tmpSec)>60) ||( ((tmpMin - datetime.minute)*60+tmpSec-datetime.second)>60))
			TerminalPowerOnOffEvent.event[TerminalPowerOnOffEvent.eventIndex].eventAttribute[2] = 0xC0;
			else
			TerminalPowerOnOffEvent.event[TerminalPowerOnOffEvent.eventIndex].eventAttribute[2] = 0x80;
			
			TerminalPowerOnOffEvent.eventIndex++;
		}
		fwrite_ertu_params(TERMINAL_POWER_ONOFF,&TerminalPowerOnOffEvent,sizeof(StrTerminalPowerOnOffEventBlock));
		system_debug_info("event is store2           ");
	}
	//fread_ertu_params(TERMINAL_POWER_ONOFF,&TerminalPowerOnOffEvent,sizeof(StrTerminalPowerOnOffEventBlock));
	//	system_debug_info("event is store3         ");
}

void reportOnOffEvent(INT8U flag)
{

	//volatile INT8U event[100];
	//INT8U pos;
	//pos=14;
	//event[pos++] =0x88;
	//event[pos++] =0x02;
	//event[pos++] =0x02;
	//
	//event[pos++] =0x31;  //OAD
	//event[pos++] =0x06;
	//event[pos++] =0x02;
	//event[pos++] =0x00;
	//
	//event[pos++] =0x05;
	//
	//event[pos++] =0x00;
	//event[pos++] =0x20;
	//event[pos++] =0x22;
	//event[pos++] =0x02;
	//event[pos++] =0x00;
	//
	//event[pos++] =0x00;
	//event[pos++] =0x20;
	//event[pos++] =0x1E;
	//event[pos++] =0x02;
	//event[pos++] =0x00;
//
	//event[pos++] =0x00;
	//event[pos++] =0x20;
	//event[pos++] =0x24;
	//event[pos++] =0x02;
	//event[pos++] =0x00;
	//
	//event[pos++] =0x00;
	//event[pos++] =0x33;
	//event[pos++] =0x09;
	//event[pos++] =0x02;
	//event[pos++] =0x06;	
	//
	//event[pos++] =0x01;  //array
	//event[pos++] =0x01;  //1 hang
	//
	//event[pos++] =0x06;
	//event[pos++] =0x00;
	//event[pos++] =0x00;
	//event[pos++] =0x00;
	//event[pos++] =0x01;
	//event[pos++] =0x1C;  // start
	//mem_set(event+pos,7,0x00);
	//pos+=7;
	//event[pos++] =0x1C;  //结束
	//mem_set(event+pos,7,0x00);
	//pos+=7;
	//event[pos++] =0x16;  //发生源
	//event[pos++] =flag;  //
	//
	//event[pos++] =0x04;
	//event[pos++] =0x08;
	//event[pos++] =0xC0;
	
	INT8U buffer[200] ={0};
    INT8U pos = 0;
    tagDatetime datetime;
    INT8U head_len = 0;
    os_get_datetime(&datetime);
    buffer[pos++]=0x68;
    buffer[pos++]=0x00;
    buffer[pos++]=0x00;
    buffer[pos++]=CTRLFUNC_DIR_SET & CTRLFUNC_PRM_CLR |  CTRL_FUNC_USERDATA;
    buffer[pos++]=gSystemInfo.devid_len-1;
    mem_cpy_reverse(buffer + OOP_POS_ADDR+1,gSystemInfo.ertu_devid,gSystemInfo.devid_len); //这里我设置表号来读取
    // mem_cpy_reverse(pResp->frame+OOP_POS_ADDR+1,gSystemInfo.meter_no,gSystemInfo.devid_len);
    pos+=gSystemInfo.devid_len;
    buffer[pos++]=0x10;//主站地址
    pos+=2;
    buffer[pos++] =0x88;
    buffer[pos++] =0x02;
    buffer[pos++] =0x00;
    buffer[pos++] =0x01;
    mem_cpy(buffer+pos,"\x60\x12\x3\x0",4);
    pos+=4;
    buffer[pos++] =0x05;
    mem_cpy(buffer+pos,"\x0\x60\x40\x2\x0",5);
    pos+=5;
    mem_cpy(buffer+pos,"\x0\x60\x41\x2\x0",5);
    pos+=5;
    mem_cpy(buffer+pos,"\x0\x60\x42\x2\x0",5);
    pos+=5;
    mem_cpy(buffer+pos,"\x0\x40\x01\x2\x0",5);
    pos+=5;
    buffer[pos++] = 0x01;
    mem_cpy(buffer+pos,"\x31\x6\x2\x0",4);  //添加对应事件的oad。
    pos+=4;
    buffer[pos++] =0x02;
    //mem_cpy(buffer+pos,"\x20\x22\x2\x0",4);
    //pos+=4;
    mem_cpy(buffer+pos,"\x20\x1E\x2\x0",4);
    pos+=4;
    mem_cpy(buffer+pos,"\x20\x20\x2\x0",4);
    pos+=4;
    buffer[pos++] = 0x01;
    buffer[pos++] = 0x01;
    buffer[pos++] = 0x1C;
    buffer[pos++] = (datetime.century*100 +datetime.year)>>8;
    buffer[pos++] = (datetime.century*100 +datetime.year)&0xFF;
    buffer[pos++] = datetime.month;
    buffer[pos++] = datetime.day;
    buffer[pos++] = datetime.hour;
    buffer[pos++] = datetime.minute;
    buffer[pos++] = datetime.second;
    buffer[pos++] = 0x1C;
    buffer[pos++] = (datetime.century*100 +datetime.year)>>8;
    buffer[pos++] = (datetime.century*100 +datetime.year)&0xFF;
    buffer[pos++] = datetime.month;
    buffer[pos++] = datetime.day;
    buffer[pos++] = datetime.hour;
    buffer[pos++] = datetime.minute;
    buffer[pos++] = datetime.second;
    buffer[pos++] = 0x1C;
    buffer[pos++] = (datetime.century*100 +datetime.year)>>8;
    buffer[pos++] = (datetime.century*100 +datetime.year)&0xFF;
    buffer[pos++] = datetime.month;
    buffer[pos++] = datetime.day;
    buffer[pos++] = datetime.hour;
    buffer[pos++] = datetime.minute;
    buffer[pos++] = datetime.second;
    buffer[pos++] = DT_OCTETSTRING;
    buffer[pos++] = 0x06;
    mem_cpy_reverse(buffer + pos,gSystemInfo.meter_no,6);
    pos += 6;
    buffer[pos++] = 0x01;
    buffer[pos++] = 0x02;
//    buffer[pos++] = 0x06;
//    mem_cpy_reverse(buffer+pos,,4);
//    pos += 4;
    buffer[pos++] = 0x1C;
        buffer[pos++] = (datetime.century*100 +datetime.year)>>8;
        buffer[pos++] = (datetime.century*100 +datetime.year)&0xFF;
        buffer[pos++] = datetime.month;
        buffer[pos++] = datetime.day;
        buffer[pos++] = datetime.hour;
        buffer[pos++] = datetime.minute;
        buffer[pos++] = datetime.second;
    buffer[pos++] =0x00;
    buffer[pos++] =0x00;
    buffer[pos++] =0x00;
    
    buffer[OOP_POS_LEN] =  pos-1+2;
    buffer[OOP_POS_LEN+1] = (pos-1+2) >> 8;
    head_len= 12;
    fcs16(buffer+OOP_POS_LEN,head_len-1);
    //计算FCS校验位
    fcs16(buffer+OOP_POS_LEN,pos-1);
    pos += 2;
    buffer[pos++] = 0x16;
		
	mem_cpy(ResponseApp.frame,buffer,pos);
	//ResponseApp.frame =event;
	ResponseApp.pos =pos;
	ResponseApp.channel = PROTOCOL_CHANNEL_DEBUG;
	app_setFrameLenThenSendFrame_exec(&RequestRemote,&ResponseApp);
	
}