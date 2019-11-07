//
// Created by Steven on 2019/10/29.
//

#ifndef LIGHT_USART1_H
#define LIGHT_USART1_H

#include "main.h"

void initWiFiUSART(void);
void sendDataToUSART1(char *buff);
void receiveDataFromUSART1(char *buff, uint16_t *length, uint16_t maxLength);

#endif //LIGHT_USART1_H
