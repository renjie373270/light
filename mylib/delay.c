//
// Created by Steven on 2019/7/23.
//

#include "delay.h"

/**
 * ������ʱ
 * */
void delayInMilliSeconds(u32 x) {
    vTaskDelay(x);
}

/**
 * ϵͳ��ʱ����ʼ��
 * ��ʱʱ��0.01ms
 * */
void sysTickInit(void){
    SysTick_Config(72000);//1ms
}

/**
 * Ӳ����ʱģ���ʼ��
 * */
void delayInit() {
    sysTickInit();
}
