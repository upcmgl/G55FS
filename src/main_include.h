#ifndef __MAIN_INCLUDE_H__
#define __MAIN_INCLUDE_H__

#include "stdint.h"
#ifndef __SOFT_SIMULATOR__
#include "stdbool.h"
#include "stddef.h"
#include "stdlib.h"
#include "stdio.h"
#else
#include "stdio.h"
#define println_info
#endif

#include "dev_const.h"
#include "dev_var.h"

//#include "os_hd200/os_typedef.h"
//#include "os_hd200/pins_def_hdv200.h"
//#include "os_hd200/os.h"
//#include "os_hd200/os_var.h"
//#include "os_hd200/os_device_var.h"
//#include "tpos/os_tpos.h"
//#include "tpos/tpos_service.h"

#include "os.h"
#include "MD5.h"



#include "fun/tops_fun_common.h"
#include "fun/tops_fun_convert.h"
#include "fun/tops_fun_date.h"
#include "fun/tops_fun_mem.h"
#include "fun/tops_fun_str.h"
#include "fun/tops_fun_tools.h"

#ifndef __SOFT_SIMULATOR__
#include "dev_driver.h"
#endif
#include "drv_flash.h"
#include "dev_file.h"

#include "os_file/os_file.h"
#include "frame_recv_service.h"

#include "task_1376_3.h"
#include "task_protocol_service.h"
#include "task_meter.h"

#ifdef __ONENET__
#include "task_nb_onenet.h"
#include "onenet/app_protocol_onenet.h"
#endif

#ifdef __OOP_G_METER__
#include "OOP/app_const_3761.h"
#include "OOP/app_var_3761.h"
#include "OOP/oop_type_def.h"
#include "OOP/object_describe.h"
#include "OOP/object_obis.h"
#include "OOP/protocol_app_const_def.h"
#include "OOP/protocol_app_handler.h"
#include "OOP/apdu_service_handler.h"
#include "OOP/apdu_security.h"
#include "OOP/app_read_meter_data.h"
#include "OOP/event.h"

#include "read_oop_meter/gb_oop.h"
#include "read_oop_meter/readmeter_oop_data.h"
#include "read_oop_meter/readmeter_oop_cur_data.h"
#include "read_oop_meter/readmeter_oop_hold_data.h"

#else

#include "Q1376_1/app_const_3761.h"
#include "Q1376_1/app_fun_3761.h"
#include "Q1376_1/app_var_3761.h"
#include "Q1376_1/plms_read_c2.h"
#include "Q1376_1/protocol_app_handler.h"
#include "Q1376_1/ErtuParam.h"
#include "Q1376_1/event.h"
#include "Q1376_1/readMeter_RS485.h"
#include "Q1376_1/variables.h"
#include "Q1376_1/option.h"
#endif

#ifdef __EDP__
#include "OneNet/Common.h"
#include "OneNet/app_protocol_onenet_edp.h"
#include "OneNet/cjsonFramePack.h"
#include "OneNet/FreeRTOS.h"
#include "OneNet/FreeRTOSConfig.h"
#include "OneNet/heap.h"
#include "OneNet/portable.h"
#include "OneNet/EdpKit.h"
#include "OneNet/cJSON.h"
#endif
#ifdef __SOFT_SIMULATOR__
void system_debug_data(uint8_t* data,uint16_t data_len);
INT8U meter_uart_send_buf(INT8U *frame,INT16U frame_len);
INT8U debug_uart_send_buf(INT8U *data,INT16U len);
void meter_uart_init (INT32U baudrate);
void   DelayNmSec(INT32U delay);
#endif
#endif