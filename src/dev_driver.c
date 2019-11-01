#include "main_include.h"
#include "asf.h"
#include "dev_driver.h"
#include "drv_uart.h"
#include "drv_gpio.h"

/*      LED Control     */
static INT32U LED_tmr_red=0, LED_tmr_local=0, LED_tmr_green, LED_CNT_green;
void LED_Task(void )
{
    //LED-remote LED-red
    if(ms_elapsed(LED_tmr_red) >= 500)
    {
        LED_tmr_red = OS_TIME;
        if(gSystemInfo.login_status)
        {
			LED_red_on(1);
        }
        else
        {
            if(gSystemInfo.csq > 15)
            {
                LED_red_toggle();
            }
            else
            {
                LED_red_on(0);
            }
        }
    }
	//LED-meter LED-local
    if(ms_elapsed(LED_tmr_local) >= 500)
    {
	    LED_tmr_local = OS_TIME;
	    if(gSystemInfo.local_status)
	    {
		    LED_local_on(1);
	    }
	    else
	    {
			LED_local_on(0);	
	    }
    }
		
    //LED-comm(USB, GPRS) LED-green
    if(LED_CNT_green && (ms_elapsed(LED_tmr_green) > 30) )
    {
        LED_tmr_green = OS_TIME;
        
        LED_CNT_green--;
        if(LED_CNT_green == 0)
        {
            LED_green_on(0);
        }
        else
        {
            LED_green_toggle();
        }
    }

}
void toggle_LED_ERR(void)   //use led_local as led-err
{
    LED_local_toggle();
}
void LED_local_on(INT8U on)
{
    if(on)
    {
        pio_set_pin_group_low(PORT_LED_LOCAL, PIN_LED_LOCAL);
    }
    else
    {
        pio_set_pin_group_high(PORT_LED_LOCAL, PIN_LED_LOCAL);
    }
}
void LED_local_toggle(void)
{
    pio_toggle_pin_group(PORT_LED_LOCAL, PIN_LED_LOCAL);
}
void LED_green_on(INT8U on)
{
    if(on)
    {
        pio_set_pin_group_low(PORT_LED_GREEN, PIN_LED_GREEN);
    }
    else
    {
        pio_set_pin_group_high(PORT_LED_GREEN, PIN_LED_GREEN);
    }
}
void LED_green_toggle(void)
{
    pio_toggle_pin_group(PORT_LED_GREEN, PIN_LED_GREEN);
}
void LED_comm_toggle(void)
{
    LED_green_toggle();
}
void LED_green_toggle_start(void)
{
    LED_CNT_green = 4;
}
void LED_red_on(INT8U on)
{
    if(on)
    {
        pio_set_pin_group_low(PORT_LED_RED, PIN_LED_RED);
    }
    else
    {
        pio_set_pin_group_high(PORT_LED_RED, PIN_LED_RED);
    }
}
void LED_red_toggle(void)
{
    pio_toggle_pin_group(PORT_LED_RED, PIN_LED_RED);
}

/*      GPRS PIN     */
void drv_pin_gprs_rst(uint8_t pin_level)
{
    if(pin_level)
    {
        pio_set_pin_group_high(PORT_GPRS_RST, PIN_GPRS_RST);
    }
    else
    {
        pio_set_pin_group_low(PORT_GPRS_RST, PIN_GPRS_RST);
    }
}
void drv_pin_gprs_onoff(uint8_t pin_level)
{
    if(pin_level)
    {
        pio_set_pin_group_high(PORT_GPRS_ONOFF, PIN_GPRS_ONOFF);
    }
    else
    {
        pio_set_pin_group_low(PORT_GPRS_ONOFF, PIN_GPRS_ONOFF);
    }
}

void drv_pin_gprs_heat_en(BOOLEAN en)
{
    if(en)
    {
        pio_set_pin_group_high(PORT_GPRS_ONOFF, PIN_GPRS_ONOFF);
    }
    else
    {
        pio_set_pin_group_low(PORT_GPRS_ONOFF, PIN_GPRS_ONOFF);
    } 
}

/*      SPI     */
void drv_flash_spi_init (void)
{   
    system_peripheral_clock_enable(ID_FLEXCOM5);
    pmc_enable_periph_clk(ID_FLEXCOM5);
    flexcom_enable(FLEXCOM5);
    flexcom_set_opmode(FLEXCOM5, FLEXCOM_SPI);
    
    pio_set_writeprotect(PORT_SPI_FLASH_CLK, FALSE);
    PORT_SPI_FLASH_CLK->PIO_OER = PIN_SPI_FLASH_CLK|PIN_SPI_FLASH_MOSI;
    PORT_SPI_FLASH_CLK->PIO_ODR = PIN_SPI_FLASH_MISO;
    pio_set_peripheral(PORT_SPI_FLASH_CLK, PIO_PERIPH_A, PIN_SPI_FLASH_CLK);
    pio_pull_up(PORT_SPI_FLASH_MOSI, PIN_SPI_FLASH_MOSI, 0);
    pio_set_peripheral(PORT_SPI_FLASH_MISO, PIO_PERIPH_A, PIN_SPI_FLASH_MISO);
	
    pio_set_peripheral(PORT_SPI_FLASH_MOSI, PIO_PERIPH_A, PIN_SPI_FLASH_MOSI); 
    pio_set_writeprotect(PORT_SPI_FLASH_CLK, TRUE); 
    
    
    spi_set_writeprotect(SPI5, FALSE);
    spi_disable(SPI5);
    
    spi_set_clock_polarity(SPI5, 0, 1);
    spi_set_clock_phase(SPI5, 0, 0);
    spi_set_bits_per_transfer(SPI5, 0, SPI_CSR_BITS_8_BIT);
    spi_set_baudrate_div(SPI5, 0, 6);//65.535M/1  
    
    spi_set_fixed_peripheral_select(SPI5);
    spi_disable_mode_fault_detect(SPI5);//we use software-NCS, disable mode-fault-detect
    spi_set_master_mode(SPI5);

    spi_enable(SPI5);
    spi_set_writeprotect(SPI5, TRUE);
}
void drv_spi_write(Spi *spi, INT8U byte)
{
    spi_put(spi, byte);
    while ( 0 == spi_is_tx_empty(spi));
    byte = spi_get(spi);
}
INT8U drv_spi_read(Spi *spi)
{
    INT8U byte;
    spi_put(spi, 0);
    while(0 == spi_is_tx_empty(spi));
    byte = (INT8U)spi_get(spi);
    
    return byte;
}
void drv_spi_clear_old_data(Spi *spi)
{
    INT8U byte;
    while(1 == spi_is_rx_full(spi))
    {
        byte = spi_get(spi);
    }
}
void drv_flash_spi_write_byte(uint8_t data)
{    
    drv_spi_write(SPI5, data);
}

void drv_flash_spi_write_array(uint8_t *data,uint16_t len)
{        
    uint16_t idx;
    uint8_t tmp;

    for(idx=0;idx<len;idx++)
    {
        drv_spi_write(SPI5, data[idx]);
    }
}
uint8_t drv_flash_spi_read_byte(void)
{
    return drv_spi_read(SPI5);
}
void drv_flash_spi_read_array(uint8_t *data,uint16_t len)
{        
    uint16_t idx;

    drv_spi_clear_old_data(SPI5);
    for(idx=0;idx<len;idx++)
    {
        data[idx]=drv_spi_read(SPI5);
    }
}


volatile tagUartObj  UartObjMeter,UartObjIr;
volatile tagUartObjGPRS UartObjGprs,UartObjDebug;


void ertu_month_bytes_add(INT32U bytes)
{
    INT32U month_bytes=0;
    tpos_enterCriticalSection();
    mem_cpy(&month_bytes,g_app_run_data.gprs_bytes.day_bytes,4);
    month_bytes += bytes;
    mem_cpy(g_app_run_data.gprs_bytes.day_bytes,&month_bytes,4);
    tpos_leaveCriticalSection();
}
int16_t gprs_uart_read_byte(void)
{
    int16_t recv;
    
    UartObjGprs.recv_pos = drv_uart_get_recv_pos(UART_ID_GPRS);
    if(UartObjGprs.read_pos == UartObjGprs.recv_pos)
    {
        return -1;
    }
    recv = UartObjGprs.recv_buf[UartObjGprs.read_pos];
    UartObjGprs.read_pos ++;
    ertu_month_bytes_add(1);   //这里注意的是每接受到一个字节就加1；
    if(UartObjGprs.read_pos >= sizeof(UartObjGprs.recv_buf))
    {
        UartObjGprs.read_pos = 0;
    }
    return recv;
}
uint8_t gprs_uart_send_buf(uint8_t *data,uint16_t len)
{
    if(gprs_uart_is_idle()==0)
    {
        return 0;
    }
    if(len == 0)
    {
        return 0;
    }
    UartObjGprs.send_ptr = data;
    UartObjGprs.send_len = len;
    ertu_month_bytes_add(len);   //这里注意的是每接受到一个字节就加len；
    
    drv_uart_lanch_tx(UART_ID_GPRS, data, len);
    
    if(gSystemInfo.gprs_debug_flag)
    {
		if(len>25)
		{
			system_debug_data(data,len);
		}
    }
    while(gprs_uart_is_idle()==0);
    return 1;
}
void uart_gprs_send_complete( void )
{
    UartObjGprs.send_len = 0;
}
uint8_t gprs_uart_is_idle(void)
{
    if(UartObjGprs.send_len)
    {
        return 0;
    }
    
    return 1;
}


int16_t meter_uart_read_byte(void)
{
    int16_t recv;
    
    UartObjMeter.recv_pos = drv_uart_get_recv_pos(UART_ID_METER);
    if(UartObjMeter.read_pos == UartObjMeter.recv_pos)
    {
        return -1;
    }
    recv = UartObjMeter.recv_buf[UartObjMeter.read_pos];
    UartObjMeter.read_pos ++;
    if(UartObjMeter.read_pos >= sizeof(UartObjMeter.recv_buf))
    {
        UartObjMeter.read_pos = 0;
    }
    return recv;
}
uint8_t meter_uart_send_buf(uint8_t *data,uint16_t len)
{
    if(meter_uart_is_idle()==0)
    {
        return 0;
    }
    if(len == 0)
    {
        return 0;
    }
    UartObjMeter.send_ptr = data;
    UartObjMeter.send_len = len;

    drv_uart_lanch_tx(UART_ID_METER, data, len);
        
    while(!meter_uart_is_idle());
    return 1;
}
void uart_meter_send_complete( void )
{
    UartObjMeter.send_len = 0;
}
uint8_t meter_uart_is_idle(void)
{
    if(UartObjMeter.send_len)
    {
        return 0;
    }
    return 1;
}

void system_debug_info(char* info)
{
    uint16_t info_len;
    
    info_len = str_cmdlen((uint8_t*)info);
    if(gSystemInfo.debug_info_interface == 0)
    {
        tpos_mutexPend(&SIGNAL_DEBUG_UART);
        debug_uart_send_buf((uint8_t*)info,info_len);
        tpos_mutexFree(&SIGNAL_DEBUG_UART);
    }
    else
    {
        tpos_mutexPend(&SIGNAL_METER_UART);
        meter_uart_send_buf((uint8_t*)info,info_len);
        tpos_mutexFree(&SIGNAL_METER_UART);
    }
}
void system_debug_data(uint8_t* data,uint16_t data_len)
{
    if(gSystemInfo.debug_info_interface == 0)
    {
        tpos_mutexPend(&SIGNAL_DEBUG_UART);
        debug_uart_send_buf(data,data_len);
        tpos_mutexFree(&SIGNAL_DEBUG_UART);
    }
    else
    {
        tpos_mutexPend(&SIGNAL_METER_UART);
        meter_uart_send_buf(data,data_len);
        tpos_mutexFree(&SIGNAL_METER_UART);
    }
}
int16_t debug_uart_read_byte(void)
{
    int16_t recv;
    
    UartObjDebug.recv_pos = drv_uart_get_recv_pos(UART_ID_DEBUG);
    if(UartObjDebug.read_pos == UartObjDebug.recv_pos)
    {
        return -1;
    }
    recv = UartObjDebug.recv_buf[UartObjDebug.read_pos];
    UartObjDebug.read_pos ++;
    if(UartObjDebug.read_pos >= sizeof(UartObjDebug.recv_buf))
    {
        UartObjDebug.read_pos = 0;
    }
    return recv;
}
uint8_t debug_uart_send_buf(uint8_t *data,uint16_t len)
{
    if(debug_uart_is_idle()==0)
    {
        return 0;
    }
    if(len == 0)
    {
        return 0;
    }
    UartObjDebug.send_ptr = data;
    UartObjDebug.send_len = len;
    // 启动发送
    drv_uart_lanch_tx(UART_ID_DEBUG, data, len);
    
    while(!debug_uart_is_idle());
    return 1;
}
void uart_debug_send_complete( void )
{
    UartObjDebug.send_len = 0;
}
uint8_t debug_uart_is_idle(void)
{
    if(UartObjDebug.send_len)
    {
        return 0;
    }
    return 1;
}
int16_t ir_read_byte(void)
{
    int16_t recv;
    
    UartObjIr.recv_pos = drv_uart_get_recv_pos(UART_ID_IR);
    if(UartObjIr.read_pos == UartObjIr.recv_pos)
    {
        return -1;
    }
    recv = UartObjIr.recv_buf[UartObjIr.read_pos];
    UartObjIr.read_pos ++;
    if(UartObjIr.read_pos >= sizeof(UartObjIr.recv_buf))
    {
        UartObjIr.read_pos = 0;
    }
    return recv;
}

uint8_t ir_send_buf(uint8_t *data,uint16_t len)
{
    if(ir_is_idle()==0)
    {
        return 0;
    }
    if(len == 0)
    {
        return 0;
    }
    UartObjIr.send_ptr = data;
    UartObjIr.send_len = len;
    //关闭接收
    ir_stop_rx();
    // 启动发送
    drv_ir_lanch_tx(1);
 
    while(!ir_is_idle());
    return 1;
}
void ir_send_complete( void )
{
    pio_set_writeprotect(PIOA, FALSE);
    pio_set_output(PORT_IR_TX, PIN_IR_TX, 0, 0, 0);
    
	
    ir_lanch_rx();
    UartObjIr.send_len = 0;

}
uint8_t ir_is_idle(void)
{
    if(UartObjIr.send_len)
    {
        return 0;
    }
    return 1;
}

uint8_t dev_power_lost(void)
{
    //if(gSystemInfo.lvd < 10000)
    //{
        //system_debug_data(&(gSystemInfo.lvd),2);
    //}
    if((gSystemInfo.lvd != 0xFFFF)&&(gSystemInfo.lvd < 8000))
    {
        return 1;
    }
    return 0;
}
static INT8U bat_cha_state;
void Task_bat_cha()
{
    if(bat_cha_state)
    {
        pio_toggle_pin_group(PORT_BAT_CHA_CTL,PIN_BAT_CHA_CTL);
    }
    else
    {
        pio_set_pin_group_low(PORT_BAT_CHA_CTL,PIN_BAT_CHA_CTL);
    }
}
void pin_battery_charge(uint8_t state)
{
    state = ~state;
    if(state)
    {
        if(bat_cha_state == 0)
        {
            bat_cha_state = 1;
            drv_bat_cha_start(bat_cha_state);
        }
    }
    else
    {
        if(bat_cha_state == 1)
        {
            bat_cha_state = 0;
            drv_bat_cha_start(bat_cha_state);
            pio_set_pin_group_low(PORT_BAT_CHA_CTL, PIN_BAT_CHA_CTL);
        }
    }
    //if(state)
    //{
        //LATCbits.LATC0 = 1;
    //}
    //else
    //{
        //LATCbits.LATC0 = 0;
    //}
}
void bat_charge_service(void)
{
    static uint8_t state=0;
    static uint32_t timer=0;
    INT16U voltage_battery;

    switch(state)
    {
        case 0:
            pin_battery_charge(1);//关闭电池充电
            timer = system_get_tick10ms();
            state ++;
            break;
        case 1:
            if((system_get_tick10ms() - timer) > 5)
            {
                voltage_battery = gSystemInfo.bat_mon;
                //system_debug_data(&voltage_battery,2);
                if( (gSystemInfo.lvd > 18000) || dev_power_lost() || (gSystemInfo.dev_temp >= 60) || (gSystemInfo.dev_temp <= -25))
                {
                    //已经掉电，或者温度不行，关闭充电
                    pin_battery_charge(1);
                    gSystemInfo.battery_charge = 0;
                    state ++;
                }
                else if(voltage_battery < 5150)
                {
                    //小于5.2v要充电
                    pin_battery_charge(0);
                    gSystemInfo.battery_charge = 0xAA;
                }
                else if((gSystemInfo.battery_charge == 0xAA)&&(voltage_battery <= 5200)) 
                { 
                    //如果是在充电状态，则要充到5200
                    pin_battery_charge(0);
                    gSystemInfo.battery_charge = 0xAA;
                }
                else
                {
                    //不需要充电了
                    pin_battery_charge(1);
                    gSystemInfo.battery_charge = 0;
                }
                state ++;
                timer = system_get_tick10ms();
            }
            break;
        case 2:
            if((system_get_tick10ms() - timer) > 300)
            {
                state = 0;
            } 
            else if( (gSystemInfo.lvd > 18000) || dev_power_lost() || (gSystemInfo.dev_temp >= 60) || (gSystemInfo.dev_temp <= -25))
            {
                pin_battery_charge(1);
                gSystemInfo.battery_charge = 0;
                state =0;
            } 
            break;
        case 3:
            if((system_get_tick10ms() - timer) > 100)
            {
                state = 0;
            }
            break;
        default:
            state = 0;
            break;
    }
}







