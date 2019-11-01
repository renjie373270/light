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
 * wifi任务
 * */
static void wifiTask(void *parameter) {
    static enum ESPWorkingMode espWorkingMode = ESP8266_NULL;
    char ssid[20] = {0}, mac[20] = {0}, channel[5] = {0}, rssi[5] = {0};
    while (1) {
        //espWorkingMode = getESPWorkingMode();
        checkCurrentConnection(ssid, mac, channel, rssi);
        delayInMilliSeconds(2000);
//        setESP8266WorkingMode(ESP8266_STATION);
//        delayInMilliSeconds(2000);
    }
}

int main() {
    initAllDevices();
    xTaskCreate(initTask, "initTask", 64, NULL, 31, NULL);
    xTaskCreate(wifiTask, "wifiTask", 1024, NULL, 30, NULL);
    xTaskCreate(ledTask, "ledTask", 64, NULL, 29, NULL);
    xTaskCreate(feedDogTask, "feedDogTask", 64, NULL, 28, NULL);
    vTaskStartScheduler();
}
