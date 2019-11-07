//
// Created by Steven on 2019/7/23.
//

#include "delay.h"

/**
 * ������ʱ
 * */
void delayInMilliSeconds(u32 x) {
    u32 sysTickCountDown = x * 100;
    while (sysTickCountDown != 0) {
        while(SysTick_GetFlagStatus(SysTick_FLAG_COUNT) != RESET)
            sysTickCountDown --;
    }
}

/**
 * ΢����ʱ
 * */
 void delayInMicroSeconds(u32 x) {
    u32 sysTickCountDown = x / 10;
    while (sysTickCountDown != 0) {
        while(SysTick_GetFlagStatus(SysTick_FLAG_COUNT) != RESET)
            sysTickCountDown --;
    }
 }

/**
 * ϵͳ��ʱ����ʼ��
 * ��ʱʱ��0.01ms
 * */
void sysTickInit(void){
    SysTick_ITConfig(DISABLE);
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);
    SysTick_SetReload(720);
    SysTick_CounterCmd(SysTick_Counter_Enable);
}

/**
 * Ӳ����ʱģ���ʼ��
 * */
void delayInit() {
    sysTickInit();
}
