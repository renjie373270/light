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
    //initDog();
    initWiFiUSART();
    initLEDGPIO();
    //initESP8266();
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
 * wifi任务
 * */
static void wifiTask(void *parameter) {
    WiFiConfig wiFiConfig;
    char *name = "RJCJH", *password = "Apple@123@369";
    while (1) {
//        checkConnection(ssid, mac, channel, rssi);
//        if(isEmpty(ssid) == FALSE) {
//            disconnectWiFi();
//        }
//        delayInMilliSeconds(3000);
//
//        checkConnection(ssid, mac, channel, rssi);
//        if(isEmpty(ssid) == TRUE) {
//            connectWiFi(name, password);
//        }
        wiFiConfig = checkConnection();
        delayInMilliSeconds(3000);
    }
}

int main() {
    initAllDevices();
    xTaskCreate(initTask, "initTask", 1024, NULL, 31, NULL);
    xTaskCreate(wifiTask, "wifiTask", 1024, NULL, 30, NULL);
    xTaskCreate(ledTask, "ledTask", 64, NULL, 29, NULL);
    //xTaskCreate(feedDogTask, "feedDogTask", 64, NULL, 28, NULL);
    vTaskStartScheduler();
}
