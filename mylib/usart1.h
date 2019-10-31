//
// Created by Steven on 2019/10/29.
//

#ifndef LIGHT_USART1_H
#define LIGHT_USART1_H

#include "main.h"

enum RunningStatus{
    RUNNING,
    DONE
};

struct USARTStatus {
    char buffer[1024];
    uint16_t  counter;
    uint16_t  length;
    enum RunningStatus status;
};

void initWiFiUSART(void);
void sendDataToUSART1(char *buff);
void receiveDataFromUSART1(char *buff, uint16_t *length);

#endif //LIGHT_USART1_H
