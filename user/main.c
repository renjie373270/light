#include "main.h"


/**
 * 初始化系统外设
 * */
void initAllDevices() {
    SystemInit();
    delayInit();
}

/**
 * 初始化任务
 * */
static void initTask(void *parameter) {
    initAllDevices();
    initDog();
    initWiFiUSART();
    initLEDGPIO();
    vTaskDelete(NULL);
}

/**
 * 看门狗任务
 * */
static void feedDogTask(void *parameter) {
    while (1) {
        delayInMilliSeconds(2000);
        feedDog();
    }
}


/**
 * LED 闪烁
 * */
static void ledTask(void *parameter) {
    while (1) {
        ledOn();
        delayInMilliSeconds(200);
        ledOff();
        delayInMilliSeconds(200);
    }
}

/**
 * 串口任务
 * */
static void usart1Task(void *parameter) {
    char buff[512] = {0};
    uint16_t size = 0;
    while (1) {
        receiveDataFromUSART1(buff, &size);
        if(size > 0) {
            size = 0;
            sendDataToUSART1(buff);
        }
        delayInMilliSeconds(100);
    }
}

int main() {
    initAllDevices();
    xTaskCreate(initTask, "initTask", 64, NULL, 31, NULL);
    xTaskCreate(usart1Task, "usart1Task", 1024, NULL, 30, NULL);
    xTaskCreate(ledTask, "ledTask", 64, NULL, 29, NULL);
    xTaskCreate(feedDogTask, "feedDogTask", 64, NULL, 28, NULL);
    vTaskStartScheduler();
}
