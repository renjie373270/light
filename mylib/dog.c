//
// Created by Steven on 2019/9/27.
//

#include "dog.h"

/**
 * ��ʼ���������Ź�
 * */
void initDog() {
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
    IWDG_SetPrescaler(IWDG_Prescaler_256); // 256��Ƶ��156.25Hz
    IWDG_SetReload(500);//3.2��ι��
    IWDG_ReloadCounter();
    IWDG_Enable();
}

/**
 * ι��
 * */
void feedDog() {
    IWDG_ReloadCounter();
}
