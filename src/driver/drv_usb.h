/*
 * drv_usb.h
 *
 * Created: 2018/12/18 星期二 21:24:26
 *  Author: Administrator
 */ 


#ifndef DRV_USB_H_
#define DRV_USB_H_

#include "uhc.h"
#include "ff.h"
#include "tpos_typedef.h"

void ui_usb_mode_change(bool b_host_mode);
void ui_usb_vbus_change(bool b_vbus_present);
void ui_usb_vbus_error(void);
void main_usb_connection_event(uhc_device_t * dev, bool b_present);
void ui_usb_wakeup_event(void);
void main_usb_sof_event(void);
void ui_usb_enum_event(uhc_device_t *dev, uhc_enum_status_t status);
FRESULT f_open_app(
	FIL *fp,			/* Pointer to the blank file object */
	const TCHAR *path,	/* Pointer to the file name */
	BYTE mode			/* Access mode and file open mode flags */);
INT16U app_write_usb_data(INT8U* filename,INT32U offset,INT8U* data,INT16U len);
INT16U app_read_usb_data(INT8U* filename,INT32U offset,INT8U* data,INT16U len);
INT16U write_usb_data(INT8U* filename,INT32U offset,INT8U* data,INT16U len);
INT16U read_usb_data(INT8U* filename,INT32U offset,INT8U* data,INT16U len);


#endif /* DRV_USB_H_ */