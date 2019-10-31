//
// Created by Steven on 2019/9/27.
//

#include "dog.h"

/**
 * 初始化独立看门狗
 * */
void initDog() {
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
    IWDG_SetPrescaler(IWDG_Prescaler_256); // 256分频，156.25Hz
    IWDG_SetReload(500);//3.2秒喂狗
    IWDG_ReloadCounter();
    IWDG_Enable();
}

/**
 * 喂狗
 * */
void feedDog() {
    IWDG_ReloadCounter();
}
