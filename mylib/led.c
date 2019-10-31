//
// Created by Steven on 2019/10/29.
//

#include "led.h"


void initLEDGPIO() {
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;//LED
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;//light
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_SetBits(GPIOB, GPIO_Pin_14);//TODO Steven 由外部设置上电后的状态
}

void ledOn() {
    GPIO_ResetBits(GPIOB, GPIO_Pin_12);
}

void ledOff() {
    GPIO_SetBits(GPIOB, GPIO_Pin_12);
}

void lightOn() {
    GPIO_ResetBits(GPIOB, GPIO_Pin_14);
}

void lightOff() {
    GPIO_SetBits(GPIOB, GPIO_Pin_14);
}
