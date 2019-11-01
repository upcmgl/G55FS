#ifndef __APP_PROTOCOL_ONENET_H__
#define	__APP_PROTOCOL_ONENET_H__
#include "../main_include.h"
#include "EdpKit.h"
#ifdef __cplusplus
extern "C" {
#endif
INT8U OneNetData[1024],oneNetstr[1024],oneNetstr2[1024];
INT8U edp_version[5];
void app_protocol_handler_edp(objRequest* Request,objResponse *Response);
void registerDevice();
INT8U  edp_ping();

BOOLEAN reportConfigToOnenet();
INT8U reportHoldDayEnergyToOnenet(INT8U flag);
INT8U reportyEnergyBlockToOnenet();
INT8U responseUpdataResToOnenet(INT8U flag);
void first_poweron_flash_rebuilt();
void handleUnsuccessReportDataRead( );
void handleUnsuccessReportDataWrite(INT8U *buf,INT16U len);
void powerOnRebuildWaningInfo();
void warning_report_handler(INT8U *warningMask);
INT8U report_warning_to_onenet(INT8U flag);
INT8U report645CmdResultToOnenet();
INT64U rand_wait_report_time_index[40];
void first_poweron_flash_wait_report_rebuilt();
void handle_wait_report_data_read();
void handle_wait_report_data_write(INT8U *buf,INT16U len);
INT16U getrand();  //得到随机数。
#ifdef __cplusplus
}
#endif

#endif

