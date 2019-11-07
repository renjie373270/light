//
// Created by Steven on 2019/7/23.
//

#include "delay.h"

/**
 * 毫秒延时
 * */
void delayInMilliSeconds(u32 x) {
    u32 sysTickCountDown = x * 100;
    while (sysTickCountDown != 0) {
        while(SysTick_GetFlagStatus(SysTick_FLAG_COUNT) != RESET)
            sysTickCountDown --;
    }
}

/**
 * 微秒延时
 * */
 void delayInMicroSeconds(u32 x) {
    u32 sysTickCountDown = x / 10;
    while (sysTickCountDown != 0) {
        while(SysTick_GetFlagStatus(SysTick_FLAG_COUNT) != RESET)
            sysTickCountDown --;
    }
 }

/**
 * 系统定时器初始化
 * 定时时间0.01ms
 * */
void sysTickInit(void){
    SysTick_ITConfig(DISABLE);
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);
    SysTick_SetReload(720);
    SysTick_CounterCmd(SysTick_Counter_Enable);
}

/**
 * 硬件延时模块初始化
 * */
void delayInit() {
    sysTickInit();
}
