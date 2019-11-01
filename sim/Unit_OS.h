//---------------------------------------------------------------------------

#ifndef Unit_OSH
#define Unit_OSH
//---------------------------------------------------------------------------
#include <Classes.hpp>

#include  "main_include.h"
extern objSYSTEM  SYSTEM;
extern unsigned char info[];
extern TDateTime OS_DATETIME_OFFSET;  //系统时间偏差，用来模拟设备的时钟与系统时钟之差
#define MAX_RX_BUFFER_LENGTH   500
typedef struct{
    uint8_t *send_ptr;
    uint16_t send_len;
    uint16_t recv_pos;
    uint16_t read_pos;
    uint8_t  recv_buf[MAX_RX_BUFFER_LENGTH];
}tagUartObj;

void getDateTime(void);

void GetHexMessage(BYTE *frame,int FrameLen,char* msg);
void frameInfo(unsigned char *frame,unsigned char* info);
unsigned char* get_cur_task_name(void);
unsigned char get_cur_task_no(void);
void force_gprs_power_off(void);
void app_light_remote_rx_led(void);
void app_light_remote_tx_led(void);
void update_cy_adjust_param(void);
void output_led_test_status(void);
void tops_platform_set_eth_update_param(void);
void unlight_LED_ERR(void);
void  ertu_set_net_mode(unsigned char data);
unsigned short ger_ertu_average_voltage(void);
unsigned short get_ertu_vrate_voltage(void);
unsigned short ger_ertu_cur_voltage(void);
unsigned short tesam_get_esam_id(unsigned char *buffer,unsigned short max_len);
unsigned short tesam_get_chip_state(unsigned char *buffer,unsigned short max_len);
void drv_esam_spi_high_Initialize(void);
void debug_println(char *info);
void debug_println_hex(unsigned char *frame,unsigned short frame_len,char * type_text);
#endif
