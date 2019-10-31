//
// Created by Steven on 2019/7/23.
//

#include "delay.h"

/**
 * 毫秒延时
 * */
void delayInMilliSeconds(u32 x) {
    vTaskDelay(x);
}

/**
 * 系统定时器初始化
 * 定时时间0.01ms
 * */
void sysTickInit(void){
    SysTick_Config(72000);//1ms
}

/**
 * 硬件延时模块初始化
 * */
void delayInit() {
    sysTickInit();
}
