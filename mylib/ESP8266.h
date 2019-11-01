//
// Created by Steven on 2019/10/31.
//

#ifndef LIGHT_ESP8266_H
#define LIGHT_ESP8266_H

#include "main.h"

enum CompareResult {
    COMPARE_FALSE = 0,
    COMPARE_TRUE = 1
};

enum ESPWorkingMode {
    ESP8266_STATION = 1, //վ��
    ESP8266_AP = 2,//·��
    ESP8266_MIX = 3,//���
    ESP8266_ERROR = 4,//��ȡʧ��
    ESP8266_NULL = 5
};

enum ESPWorkingMode getESPWorkingMode(void);
void setESP8266WorkingMode(enum ESPWorkingMode espWorkingMode);
void checkCurrentConnection(char *ssid, char *mac, char *channel, char *rssi);

#endif //LIGHT_ESP8266_H
