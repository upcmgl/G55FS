#ifndef __DRV_FLASH_H__
#define __DRV_FLASH_H__

//һ��������4KB��һ��block64K��16��sector��
//32Mbit=4MByte��������64��block��1024��sector
#define FLASH_SECTOR_SIZE             (4096)
#define FLASH_SECTOR_96BITS_LEAD      (96)
extern INT32U FLASH_SECTOR_COUNT;

#define FLASH_TYPE_MX       0
#define FLASH_TYPE_W25      1

void drv_flash_init(void);
uint8_t nor_flash_detect(void);

uint16_t nor_flash_read_data(uint16_t sector,uint16_t offset,uint8_t *data,uint16_t len);
uint16_t nor_flash_erase_page(uint16_t sector);
uint16_t nor_flash_write_data(uint16_t sector,uint16_t offset,uint8_t *data,uint16_t len);
#endif