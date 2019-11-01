//
// Created by Steven on 2019/10/31.
//

#ifndef LIGHT_ESP8266_H
#define LIGHT_ESP8266_H

#include "main.h"

enum ESPWorkingMode {
    ESP8266_STATION = 1, //站点
    ESP8266_AP = 2,//路由
    ESP8266_MIX = 3,//混合
    ESP8266_ERROR = 4,//获取失败
    ESP8266_NULL = 5
};

enum WorkingResultEnum {
    SUCCEED = 1,
    FAILED = 0
};

typedef struct {
    char ssid[64];
    char mac[32];
    char channel[4];
    char rssi[4];
}WiFiConfig;

enum ESPWorkingMode getESPWorkingMode(void);
enum WorkingResultEnum setESP8266WorkingMode(enum ESPWorkingMode espWorkingMode);
WiFiConfig checkConnection(void);
enum WorkingResultEnum connectWiFi(char *ssid, char *password);
enum WorkingResultEnum disconnectWiFi(void);
void getIPV4Address(char *ipv4);
void initESP8266(void);

#endif //LIGHT_ESP8266_H
