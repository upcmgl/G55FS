#include "cjsonFramePack.h"
//#include "stdio.h"
#include "heap.h"

#ifdef __BEIJING_ONENET_EDP__

//设备配置信息
INT16U  cjsonDeviceConfigPack(INT8U * buf)
{
	cJSON *root, *datestreamsArray, *datestreamsContent, *datapoint, *datapointCotent, *value;
	char *out;
	INT16U resLen =0;
	INT8U tmpData[30];
	INT8S str_server_ip[20];
	root = cJSON_CreateObject();
	cJSON_AddItemToObject(root, "datastreams", datestreamsArray = cJSON_CreateArray());
	cJSON_AddItemToArray(datestreamsArray, datestreamsContent = cJSON_CreateObject());
	cJSON_AddStringToObject(datestreamsContent, "id", "event");
	cJSON_AddItemToObject(datestreamsContent, "datapoints", datapoint = cJSON_CreateArray());
	cJSON_AddItemToArray(datapoint, datapointCotent = cJSON_CreateObject());
	cJSON_AddItemToObject(datapointCotent, "value", value = cJSON_CreateObject());
	 mem_set(tmpData,30, 0x00);
	 sprintf(tmpData,"%s",gSystemInfo.managementNum);
	cJSON_AddStringToObject(value, "meterNum", tmpData); ///"1111111111111111111111");
	mem_set(tmpData,30, 0x00);
	sprintf(tmpData, "%.20s", gSystemInfo.myccid_info);
	cJSON_AddStringToObject(value, "iccid",tmpData); // "898602B8231700769443");
	cJSON_AddNumberToObject(value, "interval", 30);
	mem_set(tmpData,30, 0x00);
	sprintf(tmpData, "%.15s", gSystemInfo.imei_info);
	cJSON_AddStringToObject(value, "imei", tmpData); //"861477030064636");
	if(FILEFLAG_TYPE == FILEFLAG_TGP_GMTER_G55)
	{
		cJSON_AddNumberToObject(value, "meterType", 301);   //301代表三相，101代表单相。
	}
	else if(FILEFLAG_TYPE == FILEFLAG_SGP_GMTER_G55)
	{
		cJSON_AddNumberToObject(value, "meterType", 101);   //301代表三相，101代表单相。
	}	
     ip2str(gSystemInfo.ppp_ip, str_server_ip);
     mem_set(tmpData,30, 0x00);
     sprintf(tmpData, "%s", str_server_ip);	
	cJSON_AddStringToObject(value, "ip", tmpData); //"10.146.238.6");
	cJSON_AddNumberToObject(value, "moduleType", 2); //通信模块的类型（4G设置为1，NB设置为2，2G设置为3）
	mem_set(tmpData,30, 0x00);
	sprintf(tmpData, "%s", edp_version);
	cJSON_AddStringToObject(value, "version", tmpData);

	out = cJSON_PrintUnformatted(root);
	//printf("%s", out);
	cJSON_Delete(root);
	mem_cpy(buf,out,resLen = strlen(out));
	vPortFree(out);
	//	cJSON_Delete(out);
	return resLen;

}
//组过流告警字符串
void makeOverFlow(INT8U *buf,INT8U len)
{
	INT8U str1[200],str2[200],i =0,bufPos =0;
	INT32U AphaseOverFlowCnt =0,BphaseOverFlowCnt =0,CphaseOverFlowCnt =0;
	INT32U AphaseOverFlowTime =0,BphaseOverFlowTime =0,CphaseOverFlowTime =0;
	INT8U AphaseOverFlowTimeOccur[6] = {0},BphaseOverFlowTimeOccur[6] = {0},CphaseOverFlowTimeOccur[6] = {0};
////////////////////////////////////读A相数据//////////////////////////////////////////////////////////////		
	mem_set(str1,200,0xFF);
	mem_set(str2,200,0xFF); 
	app_read_his_item(0x19010001,str1,str2,255,0,2000);  //A相过流总次数 抄表的结果应该放在str1里。
	if(str1[0]!=0xFF)
		AphaseOverFlowCnt = (BCD2byte(str1[2])*10000) + (BCD2byte(str1[1])*100) +(BCD2byte(str1[0]));
	mem_set(str1,200,0xFF);
	mem_set(str2,200,0xFF);
	app_read_his_item(0x19010002,str1,str2,255,0,2000);  //A相过流总时间 抄表的结果应该放在str1里。
	if(str1[0]!=0xFF)
		AphaseOverFlowTime = (BCD2byte(str1[2])*10000) + (BCD2byte(str1[1])*100) +(BCD2byte(str1[0]));
	mem_set(str1,200,0xFF);
	mem_set(str2,200,0xFF);
	app_read_his_item(0x19010002,str1,str2,255,0,2000);  //A相发生过流时刻 抄表的结果应该放在str1里。
	if(str1[0]!=0xFF)
	{
		for(i =0;i<6;i++)
		{
			AphaseOverFlowTimeOccur[i] =str1[i];
		}
	}	
////////////////////////////////////读B相数据//////////////////////////////////////////////////////////////
	mem_set(str1,200,0xFF);
	mem_set(str2,200,0xFF);
	app_read_his_item(0x19020001,str1,str2,255,0,2000);  //A相过流总次数 抄表的结果应该放在str1里。
	if(str1[0]!=0xFF)
		BphaseOverFlowCnt = (BCD2byte(str1[2])*10000) + (BCD2byte(str1[1])*100) +(BCD2byte(str1[0]));
	mem_set(str1,200,0xFF);
	mem_set(str2,200,0xFF);
	app_read_his_item(0x19020002,str1,str2,255,0,2000);  //A相过流总时间 抄表的结果应该放在str1里。
	if(str1[0]!=0xFF)
		BphaseOverFlowTime = (BCD2byte(str1[2])*10000) + (BCD2byte(str1[1])*100) +(BCD2byte(str1[0]));
	mem_set(str1,200,0xFF);
	mem_set(str2,200,0xFF);
	app_read_his_item(0x19020002,str1,str2,255,0,2000);  //A相发生过流时刻 抄表的结果应该放在str1里。
	if(str1[0]!=0xFF)
	{
		for(i =0;i<6;i++)
		{
			BphaseOverFlowTimeOccur[i] =str1[i];
		}
	}	

	////////////////////////////////////读C相数据//////////////////////////////////////////////////////////////
	mem_set(str1,200,0xFF);
	mem_set(str2,200,0xFF);
	app_read_his_item(0x19030001,str1,str2,255,0,2000);  //A相过流总次数 抄表的结果应该放在str1里。
	if(str1[0]!=0xFF)
		CphaseOverFlowCnt = (BCD2byte(str1[2])*10000) + (BCD2byte(str1[1])*100) +(BCD2byte(str1[0]));
	mem_set(str1,200,0xFF);
	mem_set(str2,200,0xFF);
	app_read_his_item(0x19030002,str1,str2,255,0,2000);  //A相过流总时间 抄表的结果应该放在str1里。
	if(str1[0]!=0xFF)
		CphaseOverFlowTime = (BCD2byte(str1[2])*10000) + (BCD2byte(str1[1])*100) +(BCD2byte(str1[0]));
	mem_set(str1,200,0xFF);
	mem_set(str2,200,0xFF);
	app_read_his_item(0x19030002,str1,str2,255,0,2000);  //A相发生过流时刻 抄表的结果应该放在str1里。
	if(str1[0]!=0xFF)
	{
		for(i =0;i<6;i++)
		{
			CphaseOverFlowTimeOccur[i] =str1[i];
		}
	}
//	*(buf+bufPos++) ='0';*(buf+bufPos++) ='$';*(buf+bufPos++) ='A';*(buf+bufPos++) ='&';sprintf(buf,"%d",AphaseOverFlowCnt);
	sprintf(buf+strlen(buf),"0$",NULL);
	sprintf(buf+strlen(buf),"A&%d&%d",AphaseOverFlowCnt,AphaseOverFlowTime);
	if(AphaseOverFlowTimeOccur[5]!=0x00)
		sprintf(buf+strlen(buf),"&%d%d-%d%d-%d%d %d%d:%d%d:%d%d",AphaseOverFlowTimeOccur[5]/16,AphaseOverFlowTimeOccur[5]%16,AphaseOverFlowTimeOccur[4]/16,(AphaseOverFlowTimeOccur[4]%16),AphaseOverFlowTimeOccur[3]/16,(AphaseOverFlowTimeOccur[3]%16),AphaseOverFlowTimeOccur[2]/16,(AphaseOverFlowTimeOccur[2]%16),AphaseOverFlowTimeOccur[1]/16,(AphaseOverFlowTimeOccur[1]%16),AphaseOverFlowTimeOccur[0]/16,(AphaseOverFlowTimeOccur[0]%16));
	sprintf(buf+strlen(buf),";",NULL);
	sprintf(buf+strlen(buf),"B&%d&%d",BphaseOverFlowCnt,BphaseOverFlowTime);
	if(BphaseOverFlowTimeOccur[5]!=0x00)
		sprintf(buf+strlen(buf),"&%d%d-%d%d-%d%d %d%d:%d%d:%d%d",BphaseOverFlowTimeOccur[5]/16,BphaseOverFlowTimeOccur[5]%16,BphaseOverFlowTimeOccur[4]/16,(BphaseOverFlowTimeOccur[4]%16),BphaseOverFlowTimeOccur[3]/16,(BphaseOverFlowTimeOccur[3]%16),BphaseOverFlowTimeOccur[2]/16,(BphaseOverFlowTimeOccur[2]%16),BphaseOverFlowTimeOccur[1]/16,(BphaseOverFlowTimeOccur[1]%16),BphaseOverFlowTimeOccur[0]/16,(BphaseOverFlowTimeOccur[0]%16));
	sprintf(buf+strlen(buf),";",NULL);
	sprintf(buf+strlen(buf),"C&%d&%d",CphaseOverFlowCnt,CphaseOverFlowTime);
	if(CphaseOverFlowTimeOccur[5]!=0x00)
		sprintf(buf+strlen(buf),"&%d%d-%d%d-%d%d %d%d:%d%d:%d%d",CphaseOverFlowTimeOccur[5]/16,CphaseOverFlowTimeOccur[5]%16,CphaseOverFlowTimeOccur[4]/16,(CphaseOverFlowTimeOccur[4]%16),CphaseOverFlowTimeOccur[3]/16,(CphaseOverFlowTimeOccur[3]%16),CphaseOverFlowTimeOccur[2]/16,(CphaseOverFlowTimeOccur[2]%16),CphaseOverFlowTimeOccur[1]/16,(CphaseOverFlowTimeOccur[1]%16),CphaseOverFlowTimeOccur[0]/16,(CphaseOverFlowTimeOccur[0]%16));
	len =strlen(buf);
	
}
//组开表盖告警字符串
void makeOpenMeter(INT8U *buf,INT8U len)
{
	INT8U str1[200],str2[200],i =0,bufPos =0;
	INT16U openCnt =0;
	INT8U openTimeOccur[6] = {0};
	////////////////////////////////////读A相数据//////////////////////////////////////////////////////////////
	mem_set(str1,200,0xFF);
	mem_set(str2,200,0xFF);
	app_read_his_item(0x03300D00,str1,str2,255,0,2000);  //开盖总次数 抄表的结果应该放在str1里。
	if(str1[0]!=0xFF)
		openCnt = (BCD2byte(str1[2])*10000) + (BCD2byte(str1[1])*100) +(BCD2byte(str1[0]));
	mem_set(str1,200,0xFF);
	mem_set(str2,200,0xFF);
	app_read_his_item(0x03300D01,str1,str2,255,0,2000);  //上一次开盖时间 抄表的结果应该放在str1里。
	if(str1[0]!=0xFF)
	{
		for(i =0;i<6;i++)
		{
			openTimeOccur[i] =str1[i];
		}
	}
	sprintf(buf+strlen(buf),"1$",NULL);
	sprintf(buf+strlen(buf),"%d&",openCnt);
	sprintf(buf+strlen(buf),"&%d%d-%d%d-%d%d %d%d:%d%d:%d%d",openTimeOccur[5]/16,openTimeOccur[5]%16,openTimeOccur[4]/16,(openTimeOccur[4]%16),openTimeOccur[3]/16,(openTimeOccur[3]%16),openTimeOccur[2]/16,(openTimeOccur[2]%16),openTimeOccur[1]/16,(openTimeOccur[1]%16),openTimeOccur[0]/16,(openTimeOccur[0]%16));
	len =strlen(buf);
}
//组电表清零告警字符串
void makeCleanMeter(INT8U *buf,INT8U len)
{
	INT8U str1[200],str2[200],i =0,bufPos =0;
	INT16U cleanCnt =0;
	INT8U cleanTimeOccur[6] = {0};
	////////////////////////////////////读A相数据//////////////////////////////////////////////////////////////
	mem_set(str1,200,0xFF);
	mem_set(str2,200,0xFF);
	app_read_his_item(0x03300100,str1,str2,255,0,2000);  //开盖总次数 抄表的结果应该放在str1里。
	if(str1[0]!=0xFF)
	cleanCnt = (BCD2byte(str1[2])*10000) + (BCD2byte(str1[1])*100) +(BCD2byte(str1[0]));
	mem_set(str1,200,0xFF);
	mem_set(str2,200,0xFF);
	app_read_his_item(0x03300101,str1,str2,255,0,2000);  //上一次开盖时间 抄表的结果应该放在str1里。
	if(str1[0]!=0xFF)
	{
		for(i =0;i<6;i++)
		{
			cleanTimeOccur[i] =str1[i];
		}
	}
	sprintf(buf+strlen(buf),"2$",NULL);
	sprintf(buf+strlen(buf),"%d&",cleanCnt);
	sprintf(buf+strlen(buf),"&%d%d-%d%d-%d%d %d%d:%d%d:%d%d",cleanTimeOccur[5]/16,cleanTimeOccur[5]%16,cleanTimeOccur[4]/16,(cleanTimeOccur[4]%16),cleanTimeOccur[3]/16,(cleanTimeOccur[3]%16),cleanTimeOccur[2]/16,(cleanTimeOccur[2]%16),cleanTimeOccur[1]/16,(cleanTimeOccur[1]%16),cleanTimeOccur[0]/16,(cleanTimeOccur[0]%16));
	len =strlen(buf);
}
//事件告警信息
INT16U cjsonEventWarningPack(INT8U *buf,INT8U flag)
{
	cJSON *root, *datestreamsArray, *datestreamsContent, *datapoint, *datapointCotent, *value;
	char *out;
	INT16U resLen =0;
	INT8U buftmp[200] ={0},len =0;
	root = cJSON_CreateObject();
	cJSON_AddItemToObject(root, "datastreams", datestreamsArray = cJSON_CreateArray());
	cJSON_AddItemToArray(datestreamsArray, datestreamsContent = cJSON_CreateObject());
	cJSON_AddStringToObject(datestreamsContent, "id", "event");
	cJSON_AddItemToObject(datestreamsContent, "datapoints", datapoint = cJSON_CreateArray());
	cJSON_AddItemToArray(datapoint, datapointCotent = cJSON_CreateObject());
	cJSON_AddItemToObject(datapointCotent, "value", value = cJSON_CreateObject());
	switch(flag)
	{
		case 1:  //过流告警
			makeOverFlow(buftmp,len);
			break;
		case 2:  //开表盖告警
			makeOpenMeter(buftmp,len);
			break;
		case 3:   // 电能表清零告警
			makeCleanMeter(buftmp,len);
			break;
		default:
			break;
	}
//	cJSON_AddStringToObject(value, "topwarn", "2$1&17-04-01 13:05:47;B&0&0;C&0&0;");
	cJSON_AddStringToObject(value, "topwarn", buftmp);


	out = cJSON_PrintUnformatted(root);
	mem_cpy(buf,out,resLen = strlen(out));
	cJSON_Delete(root);
	vPortFree(out);
	return resLen;
}
//固件升级信息
INT16U cjsonFirmwareUpdatePack(INT8U *buf,INT8U flag)
{
	cJSON *root, *datestreamsArray, *datestreamsContent, *datapoint, *datapointCotent, *value;
	char *out;
	INT16U resLen = 0;
	root = cJSON_CreateObject();
	cJSON_AddItemToObject(root, "datastreams", datestreamsArray = cJSON_CreateArray());
	cJSON_AddItemToArray(datestreamsArray, datestreamsContent = cJSON_CreateObject());
	cJSON_AddStringToObject(datestreamsContent, "id", "event");
	cJSON_AddItemToObject(datestreamsContent, "datapoints", datapoint = cJSON_CreateArray());
	cJSON_AddItemToArray(datapoint, datapointCotent = cJSON_CreateObject());
	cJSON_AddItemToObject(datapointCotent, "value", value = cJSON_CreateObject());
	cJSON_AddNumberToObject(value, "fota", flag);

	out = cJSON_PrintUnformatted(root);
	mem_cpy(buf,out,resLen = strlen(out));
	cJSON_Delete(root);
	vPortFree(out);
	return resLen;
}
//组报文透传抄表数据。
INT16U cjsonReadMeter(INT8U *buf)
{
	cJSON *root, *datestreamsArray, *datestreamsContent, *datapoint, *datapointCotent, *value;
	char *out;
	INT16U resLen =0;
	INT8U buftmp[10] ={0},len =0,i=0;
	INT8U str1[200]={0},str2[400]={0};	
	root = cJSON_CreateObject();
	cJSON_AddItemToObject(root, "datastreams", datestreamsArray = cJSON_CreateArray());
	cJSON_AddItemToArray(datestreamsArray, datestreamsContent = cJSON_CreateObject());
	cJSON_AddStringToObject(datestreamsContent, "id", "event");
	cJSON_AddItemToObject(datestreamsContent, "datapoints", datapoint = cJSON_CreateArray());
	cJSON_AddItemToArray(datapoint, datapointCotent = cJSON_CreateObject());
	cJSON_AddItemToObject(datapointCotent, "value", value = cJSON_CreateObject());
	//cJSON_AddStringToObject(value, "topwarn", "2$1&17-04-01 13:05:47;B&0&0;C&0&0;");
	sprintf(buftmp,"%08x",cmd645);
	cJSON_AddStringToObject(value, "cmd", buftmp);
	
	len = app_read_his_item(cmd645,str1,str2,255,0,2000);//str1中存在着我们要读取的值；组合有功。
	mem_set(str2,400,0x00);
	for(i =0;i<len;i++)
	{
		if(((str1[i]>>4)&0x0F)>=0 &&((str1[i]>>4)&0x0F)<=9)
			str2[2*i] = ((str1[i]>>4)&0x0F) +'0';
		else if(((str1[i]>>4)&0x0F)>=10 &&((str1[i]>>4)&0x0F)<=15)
			str2[2*i] = ((str1[i]>>4)&0x0F) +'A';
			
		if(((str1[i])&0x0F)>=0 &&((str1[i])&0x0F)<=9)
			str2[2*i+1] = ((str1[i])&0x0F) +'0';
		else if(((str1[i])&0x0F)>=10 &&((str1[i])&0x0F)<=15)
			str2[2*i+1] = ((str1[i])&0x0F) +'A';
	}
	cJSON_AddStringToObject(value, "echo", str2);
	
	out = cJSON_PrintUnformatted(root);
	mem_cpy(buf,out,resLen = strlen(out));
	cJSON_Delete(root);
	vPortFree(out);
	return resLen;
}
//实时电量信息
INT16U cjsonEnergyInfoPack(char dayhold,INT8U *buf)
{
	cJSON *root, *datestreamsArray, *datestreamsContent, *datapoint, *datapointCotent, *value, *energy;
	char *out;
	INT8U tmpData[30] ={0};
    INT8U str1[100],str2[100];	
	INT32U energy_int[2];	
	tagDatetime  datetime;
	INT16U resLen =0;
	root = cJSON_CreateObject();
	cJSON_AddItemToObject(root, "datastreams", datestreamsArray = cJSON_CreateArray());
	cJSON_AddItemToArray(datestreamsArray, datestreamsContent = cJSON_CreateObject());
	cJSON_AddStringToObject(datestreamsContent, "id", "value");
	cJSON_AddItemToObject(datestreamsContent, "datapoints", datapoint = cJSON_CreateArray());
	cJSON_AddItemToArray(datapoint, datapointCotent = cJSON_CreateObject());
	cJSON_AddItemToObject(datapointCotent, "value", value = cJSON_CreateObject());
	//mem_set(tmpData,30, 0x00);
	//sprintf(tmpData,"%d",gSystemInfo.csq);	
	//cJSON_AddStringToObject(value, "SignalLevel", tmpData);
	cJSON_AddNumberToObject(value, "SignalLevel", gSystemInfo.csq);
	cJSON_AddItemToObject(value, "UserData", energy = cJSON_CreateObject());
	mem_set(tmpData,30, 0x00);
	sprintf(tmpData,"%s",gSystemInfo.managementNum);
	cJSON_AddStringToObject(energy, "meterNum", tmpData); ///"1111111111111111111111");
	
	os_get_datetime(&datetime);
	 //	str = "\"17-08-02 10:52:01\",";
	sprintf(tmpData,"%d-%d%d-%d%d %d%d:%d%d:%d%d",datetime.year,datetime.month/10,(datetime.month%10),datetime.day/10,(datetime.day%10),datetime.hour/10,(datetime.hour%10),datetime.minute/10,(datetime.minute%10),datetime.second/10,(datetime.second%10));
	cJSON_AddStringToObject(energy, "time", tmpData);
	{
		if(dayhold ==0)
		{
			app_read_his_item(0x0000FF00,str1,str2,255,0,2000);//str1中存在着我们要读取的值；组合有功。
		}
		else if(dayhold==1)
		{
			app_read_his_item(0x05060101,str1,str2,255,0,2000);//str1中存在着我们要读取的值；日冻结正向有功。
		}
		energy_int[0] = (BCD2byte(str1[3])*10000) +(BCD2byte(str1[2])*100)+(BCD2byte(str1[1]));
		energy_int[1] = BCD2byte(str1[0]);
	}	
	mem_set(tmpData,30, 0x00);	
    sprintf(tmpData,"%d.%02d",energy_int[0],energy_int[1]);	
	cJSON_AddStringToObject(energy, "kwh", tmpData);
	cJSON_AddNumberToObject(energy, "frozen", dayhold);
	

	out = cJSON_PrintUnformatted(root);
	
	mem_cpy(buf,out,resLen = strlen(out));
	cJSON_Delete(root);
	vPortFree(out);
	return resLen;

}
//电能量信息数据块
INT16U cjsonEnergyBlockPack(INT8U *buf)
{
	cJSON *root, *datestreamsArray, *datestreamsContent, *datapoint, *datapointCotent, *value, *energy;
	char *out;
	INT16U resLen =0;
    INT8U str1[100],str2[100];
    INT16U vol[6];  //转换用于电压
    INT16U cur[6];  //转换用于电流
    INT16U insP[8];  //瞬时有功  总+ABC
    INT16U insQ[8];  //瞬时无功   总+ABC
    INT16U pwrFactor[8];  //功率因数。 总+ABC	
	INT8U tmpData[30] ={0};
	root = cJSON_CreateObject();
	cJSON_AddItemToObject(root, "datastreams", datestreamsArray = cJSON_CreateArray());
	cJSON_AddItemToArray(datestreamsArray, datestreamsContent = cJSON_CreateObject());
	cJSON_AddStringToObject(datestreamsContent, "id", "value");
	cJSON_AddItemToObject(datestreamsContent, "datapoints", datapoint = cJSON_CreateArray());
	cJSON_AddItemToArray(datapoint, datapointCotent = cJSON_CreateObject());
	cJSON_AddItemToObject(datapointCotent, "value", value = cJSON_CreateObject());
	//mem_set(tmpData,30, 0x00);
	//sprintf(tmpData,"%d",gSystemInfo.csq);
	//cJSON_AddStringToObject(value, "SignalLevel", tmpData);
	cJSON_AddNumberToObject(value, "SignalLevel", gSystemInfo.csq);
	cJSON_AddItemToObject(value, "UserData", energy = cJSON_CreateObject());
if(FILEFLAG_TYPE == FILEFLAG_TGP_GMTER_G55)
{    
	{
	    app_read_his_item(0x0202FF00,str1,str2,255,0,2000);//str1中存在着我们要读取的值；电流数据块。
	    cur[0] = (BCD2byte(str1[2])*10) +(str1[1]>>4);
	    cur[1] = (str1[1]&0x0F)*100 +BCD2byte(str1[0]);
	    cur[2] = (BCD2byte(str1[5])*10) +(str1[4]>>4);
	    cur[3] = (str1[4]&0x0F)*100 +BCD2byte(str1[3]);
	    cur[4] = (BCD2byte(str1[8])*10) +(str1[7]>>4);
	    cur[5] = (str1[7]&0x0F)*100 +BCD2byte(str1[6]);
    }	
	mem_set(tmpData,30, 0x00);		
    sprintf(tmpData,"%d.%03d$%d.%03d$%d.%03d",cur[0],cur[1],cur[2],cur[3],cur[4],cur[5]);
	cJSON_AddStringToObject(energy, "cur", tmpData); //"000.000$FFF.FFF$FFF.FFF");
}
else if(FILEFLAG_TYPE == FILEFLAG_SGP_GMTER_G55)
{
	{
		app_read_his_item(0x0202FF00,str1,str2,255,0,2000);//str1中存在着我们要读取的值；电流数据块。
		cur[0] = (BCD2byte(str1[2])*10) +(str1[1]>>4);
		cur[1] = (str1[1]&0x0F)*100 +BCD2byte(str1[0]);
	}
	mem_set(tmpData,30, 0x00);
	sprintf(tmpData,"%d.%03d$FFF.FFF$FFF.FFF",cur[0],cur[1]);
	cJSON_AddStringToObject(energy, "cur", tmpData); //"000.000$FFF.FFF$FFF.FFF");
}
if(FILEFLAG_TYPE == FILEFLAG_TGP_GMTER_G55)
{
    {
	    app_read_his_item(0x0201FF00,str1,str2,255,0,2000);//str1中存在着我们要读取的值；电压数据块。
	    vol[0] =(BCD2byte(str1[1])*10) +(str1[0]>>4);vol[1] =str1[0]&0x0F;
	    vol[2] =(BCD2byte(str1[3])*10) +(str1[2]>>4);vol[3] =str1[2]&0x0F;
	    vol[4] =(BCD2byte(str1[5])*10) +(str1[4]>>4);vol[5] =str1[4]&0x0F;
    }
	mem_set(tmpData,30, 0x00);	
    sprintf(tmpData,"%d.%d$%d.%d$%d.%d",vol[0],vol[1],vol[2],vol[3],vol[4],vol[5]);	
	cJSON_AddStringToObject(energy, "vol", tmpData); //"219.4$FFF.F$FFF.F");
}
else if(FILEFLAG_TYPE == FILEFLAG_SGP_GMTER_G55)
{
	 {
		 app_read_his_item(0x0201FF00,str1,str2,255,0,2000);//str1中存在着我们要读取的值；电压数据块。
		 vol[0] =(BCD2byte(str1[1])*10) +(str1[0]>>4);vol[1] =str1[0]&0x0F;
	 }
	 mem_set(tmpData,30, 0x00);
	 sprintf(tmpData,"%d.%d$FFF.F$FFF.F",vol[0],vol[1]);
	 cJSON_AddStringToObject(energy, "vol", tmpData); //"219.4$FFF.F$FFF.F");
}
if(FILEFLAG_TYPE == FILEFLAG_TGP_GMTER_G55)
{
    {
	    app_read_his_item(0x0203FF00,str1,str2,255,0,2000);//str1中存在着我们要读取的值；瞬时有功功率。
	    insP[0] = BCD2byte(str1[2]);
	    insP[1] = BCD2byte(str1[1])*100+BCD2byte(str1[0]);
	    insP[2] = BCD2byte(str1[5]);
	    insP[3] = BCD2byte(str1[4])*100+BCD2byte(str1[3]);
	     
	    insP[4] = BCD2byte(str1[8]);
	    insP[5] = BCD2byte(str1[7])*100+BCD2byte(str1[6]);
	    insP[6] = BCD2byte(str1[11]);
	    insP[7] = BCD2byte(str1[10])*100+BCD2byte(str1[9]);
    }
	mem_set(tmpData,30, 0x00);	
    sprintf(tmpData, "%d.%04d$%d.%04d$%d.%04d$%d.%04d",insP[0],insP[1],insP[2],insP[3],insP[4],insP[5],insP[6],insP[7]);	
	cJSON_AddStringToObject(energy, "insP", tmpData); //"00.0000$00.0000$FF.FFFF$FF.FFFF");
}
else if(FILEFLAG_TYPE == FILEFLAG_SGP_GMTER_G55)
{
	 {
		 app_read_his_item(0x0203FF00,str1,str2,255,0,2000);//str1中存在着我们要读取的值；瞬时有功功率。
		 insP[0] = BCD2byte(str1[2]);
		 insP[1] = BCD2byte(str1[1])*100+BCD2byte(str1[0]);
		 insP[2] = BCD2byte(str1[5]);
		 insP[3] = BCD2byte(str1[4])*100+BCD2byte(str1[3]);
	 }
	 mem_set(tmpData,30, 0x00);
	 sprintf(tmpData, "%d.%04d$%d.%04d$FF.FFFF$FF.FFFF",insP[0],insP[1],insP[2],insP[3]);
	 cJSON_AddStringToObject(energy, "insP", tmpData); //"00.0000$00.0000$FF.FFFF$FF.FFFF");
}
if(FILEFLAG_TYPE == FILEFLAG_TGP_GMTER_G55)
{
    {
	    app_read_his_item(0x0204FF00,str1,str2,255,0,2000);//str1中存在着我们要读取的值；瞬时无功功率。
	    insQ[0] = BCD2byte(str1[2]);
	    insQ[1] = BCD2byte(str1[1])*100+BCD2byte(str1[0]);
	    insQ[2] = BCD2byte(str1[5]);
	    insQ[3] = BCD2byte(str1[4])*100+BCD2byte(str1[3]);
	    
	    insQ[4] = BCD2byte(str1[8]);
	    insQ[5] = BCD2byte(str1[7])*100+BCD2byte(str1[6]);
	    insQ[6] = BCD2byte(str1[11]);
	    insQ[7] = BCD2byte(str1[10])*100+BCD2byte(str1[9]);
    }
	mem_set(tmpData,30, 0x00);		
    sprintf(tmpData, "%d.%04d$%d.%04d$%d.%04d$%d.%04d",insQ[0],insQ[1],insQ[2],insQ[3],insQ[4],insQ[5],insQ[6],insQ[7]);	
	cJSON_AddStringToObject(energy, "insQ", tmpData); //"00.0000$00.0000$FF.FFFF$FF.FFFF");
}
else if(FILEFLAG_TYPE == FILEFLAG_SGP_GMTER_G55)
{
	{
		app_read_his_item(0x0204FF00,str1,str2,255,0,2000);//str1中存在着我们要读取的值；瞬时无功功率。
		insQ[0] = BCD2byte(str1[2]);
		insQ[1] = BCD2byte(str1[1])*100+BCD2byte(str1[0]);
		insQ[2] = BCD2byte(str1[5]);
		insQ[3] = BCD2byte(str1[4])*100+BCD2byte(str1[3]);
	}
	mem_set(tmpData,30, 0x00);
	sprintf(tmpData, "%d.%04d$%d.%04d$FF.FFFF$FF.FFFF",insQ[0],insQ[1],insQ[2],insQ[3]);
	cJSON_AddStringToObject(energy, "insQ", tmpData); //"00.0000$00.0000$FF.FFFF$FF.FFFF");
}
if(FILEFLAG_TYPE == FILEFLAG_TGP_GMTER_G55)
{
    {
	    app_read_his_item(0x0206FF00,str1,str2,255,0,2000);//str1中存在着我们要读取的值；功率因数。
	    pwrFactor[0] = BCD2byte(str1[1]>>4);
	    pwrFactor[1] = BCD2byte(str1[1]&0x0F)*100+BCD2byte(str1[0]);
	    pwrFactor[2] = BCD2byte(str1[3]>>4);
	    pwrFactor[3] = BCD2byte(str1[3]&0x0F)*100+BCD2byte(str1[2]);
	    
	    pwrFactor[4] = BCD2byte(str1[5]>>4);
	    pwrFactor[5] = BCD2byte(str1[5]&0x0F)*100+BCD2byte(str1[4]);
	    pwrFactor[6] = BCD2byte(str1[7]>>4);
	    pwrFactor[7] = BCD2byte(str1[7]&0x0F)*100+BCD2byte(str1[6]);
    }
	mem_set(tmpData,30, 0x00);	
    sprintf(tmpData, "%d.%03d$%d.%03d$%d.%03d$%d.%03d", pwrFactor[0], pwrFactor[1],pwrFactor[2], pwrFactor[3], pwrFactor[4], pwrFactor[5],pwrFactor[6], pwrFactor[7]);	
	cJSON_AddStringToObject(energy, "pwrF", tmpData); //"1.000$1.000$F.FFF$F.FFF");
}
else if(FILEFLAG_TYPE == FILEFLAG_SGP_GMTER_G55)
{
	{
		app_read_his_item(0x0206FF00,str1,str2,255,0,2000);//str1中存在着我们要读取的值；功率因数。
		pwrFactor[0] = BCD2byte(str1[1]>>4);
		pwrFactor[1] = BCD2byte(str1[1]&0x0F)*100+BCD2byte(str1[0]);
		pwrFactor[2] = BCD2byte(str1[3]>>4);
		pwrFactor[3] = BCD2byte(str1[3]&0x0F)*100+BCD2byte(str1[2]);
	}
	mem_set(tmpData,30, 0x00);
	sprintf(tmpData, "%d.%03d$%d.%03d$F.FFF$F.FFF", pwrFactor[0], pwrFactor[1],pwrFactor[2], pwrFactor[3]);
	cJSON_AddStringToObject(energy, "pwrF", tmpData); //"1.000$1.000$F.FFF$F.FFF");
}
	out = cJSON_PrintUnformatted(root);
	mem_cpy(buf,out,resLen = strlen(out));
	cJSON_Delete(root);
	vPortFree(out);
	return resLen;
}
#endif // __BEIJING_ONENET_EDP__
#ifdef __ZHEJIANG_ONENET_EDP__

#endif // DEBUG
