//
// Created by Steven on 2019/10/29.
//

#include "usart1.h"


struct USARTStatus txStatus;
struct USARTStatus rxStatus;

/**
 * 初始化串口1相关IO口
 * */
void initUSART1GPIO() {
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO | RCC_APB2Periph_USART1, ENABLE);

    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //USART1_TX
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //USART1_RX
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

/**
 * 配置串口1
 * */
void initUSART1() {
    USART_InitTypeDef InitStructure;
    InitStructure.USART_BaudRate = 115200;
    InitStructure.USART_WordLength = USART_WordLength_8b;
    InitStructure.USART_StopBits = USART_StopBits_1;
    InitStructure.USART_Parity = USART_Parity_No;
    InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
    USART_Init(USART1, &InitStructure);
    USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    USART_Cmd(USART1, ENABLE);
}

/**
 * 初始化串口相关NVIC
 * */
void initUSART1NVIC(void) {
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/**
 * 初始化串口1
 * */
void initWiFiUSART() {
    initUSART1GPIO();
    initUSART1();
    initUSART1NVIC();

    txStatus.length = 0;
    txStatus.counter = 0;
    txStatus.status = DONE;
    
    rxStatus.length = 0;
    rxStatus.counter = 0;
    rxStatus.status = DONE;
}

/**
 * 获取字符串长度
 * */
uint16_t getStringSize(char *buff) {
    uint16_t count = 0;
    for(count = 0; count < 4096; count++) {
        if(buff[count] == '\0') {					
            break;
        }
        if(buff[count] == '\r') {
						count++;
            if(buff[count] == '\n') {
								count++;
                break;
            }
        }
    }
    return count;
}

/**
 * 发送数据
 * */
void sendDataToUSART1(char *buff) {
    uint16_t count;
    txStatus.status = RUNNING;
    txStatus.counter = 0;
    txStatus.length = getStringSize(buff);
    for(count = 0; count < txStatus.length; count++) {
        txStatus.buffer[count] = buff[count];
    }
//    USART_SendData(USART1, txStatus.buffer[txStatus.counter]);
    USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
}

/**
 * 接收数据
 * */
void receiveDataFromUSART1(char *buff, uint16_t *length) {
    uint16_t count;
    if(rxStatus.status == RUNNING) {
        rxStatus.length = rxStatus.counter;
        delayInMilliSeconds(5);
        if(rxStatus.length == rxStatus.counter) {
            *length = rxStatus.length + 1;
            for(count = 0; count < *length; count++) {
                buff[count] = rxStatus.buffer[count];
                rxStatus.buffer[count] = 0;
            }
            rxStatus.length = 0;
            rxStatus.counter = 0;
            rxStatus.status = DONE;
        }
    }
}

/**
 * 串口1中断处理
 * */
void USART1_IRQHandler() {
    if(USART_GetFlagStatus(USART1 , USART_FLAG_TXE) == SET) {
        USART_ClearFlag(USART1, USART_FLAG_TXE);
        if(txStatus.status == RUNNING) {
            USART_SendData(USART1, txStatus.buffer[txStatus.counter]);
            txStatus.counter ++;
            if(txStatus.counter >= txStatus.length) {
                txStatus.status = DONE;
                USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
            }
        }
    }

    if(USART_GetFlagStatus(USART1 , USART_FLAG_RXNE) == SET) {
        USART_ClearFlag(USART1, USART_FLAG_RXNE);
        if(rxStatus.status == DONE) {
            rxStatus.counter = 0;
            rxStatus.length = 0;
            rxStatus.status = RUNNING;
        }
        rxStatus.buffer[rxStatus.counter] = USART_ReceiveData(USART1);
        rxStatus.counter ++;
    }
}
