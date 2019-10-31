//
// Created by Steven on 2019/10/14.
//

#include "myflash.h"

#define FLASH_BASE_ADDR 0x0800F7F0

/**
 * 写入数据
 * */
void saveDataToFlash(uint16_t *buff, uint8_t length) {
    uint8_t i;
    FLASH_Status result;
    FLASH_Unlock();
    FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);//清除所有标志
    result = FLASH_ErasePage(FLASH_BASE_ADDR);//扇区擦除
    delayInMilliSeconds(100);
    for(i = 0; i < 8; i++) {
        FLASH_ProgramHalfWord(FLASH_BASE_ADDR + i * 2, buff[i]);
    }
    FLASH_Lock();
}

/**
 * 读取数据
 * */
void readDataFromFlash(uint16_t *buff, uint8_t length) {
    uint8_t i;
    uint32_t addr = FLASH_BASE_ADDR;
    for(i = 0; i < length; i ++) {
        buff[i] = (*(__IO uint16_t *) addr);
        addr += 2;
    }
}
