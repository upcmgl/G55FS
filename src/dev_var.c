#include "main_include.h"

objMutex SIGNAL_FLASH;
objMutex SIGNAL_METER_UART;
objMutex SIGNAL_DEBUG_UART;
objMutex SIGNAL_INFRA_UART;
objMutex SIGNAL_FILE_OP;
objMutex SIGNAL_REMOTE_UART;
objMutex SIGNAL_REMOTE_UART_SEND;
objMutex SIGNAL_RUN_DATA;
objMutex    SIGNAL_SYSTEMINFO;
objMutex    SIGNAL_METER;
objMutex    SIGNAL_EASM;
objMutex   SIGNAL_TEMP_BUFFER;
objMutex   SIGNAL_EVENT;

objRequest  RequestRemote,RequestRemote2;
objRequest  RequestRemoteServer;
objRequest  RequestUart;
objRequest  RequestDebug;
objRequest  RequestInfra;
objResponse ResponseApp;

SEPARATE_FRAME separate_frame_mem;

tagSystemInfo gSystemInfo;
tagAPP_RUN_DATA g_app_run_data;  //�������������洢NB-IOT���������ݡ�

//tagGlobalEVENT g_event;

uint8_t g_meter_set_flag;
#ifndef __SOFT_SIMULATOR__
tagUPDATE_OBJECT update_object;    //������������
DATHEADER update_header;                  //�����ļ�ͷ����
#endif
#ifdef __ONENET__
tagWirelessObj RequestOnenet,ReportOnenet;
TASK_INFO task;
#endif