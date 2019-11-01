/*
 * os.h
 *
 * Created: 2018/12/11 星期二 11:35:11
 *  Author: Administrator
 */ 


#ifndef _OS_H_
#define _OS_H_
#include "string.h"

#include "tpos_mem.h"
#include "tpos_service.h"
#include "tpos_time.h"
#include "tpos_typedef.h"
#include "tpos_include.h"
#include "tpos_cpu.h"

#include "sys_config.h"
#ifndef __SOFT_SIMULATOR__
#include "drv_timer.h"
#include "drv_gpio.h"
#include "drv_adc.h"
#include "drv_uart.h"
#include "drv_file_if.h"
#endif
#define tagDatetime DateTime
#define os_get_datetime tpos_datetime




#endif /* OS_H_ */