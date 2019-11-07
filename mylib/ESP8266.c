//
// Created by Steven on 2019/10/31.
//

#include "ESP8266.h"

/**
 * 查看ESP8266工作模式
 * */
enum ESPWorkingMode getESPWorkingMode() {
    char *command = "AT+CWMODE?\r\n", buff[256] = {0}, results[8][64] = {0};
    enum ESPWorkingMode mode = ESP8266_ERROR;
    uint16_t size = 0, timeout = 20;

    receiveDataFromUSART1(buff, &size, sizeof(buff));//发送前先清空接收缓冲区
    sendDataToUSART1(command);
    delayInMilliSeconds(5000);

    size = 0;
    while (size == 0 && timeout--) {
        delayInMilliSeconds(10);
        receiveDataFromUSART1(buff, &size, sizeof(buff));//等待返回数据，最多200ms
    }
    if(size > 0 && contains(buff, "OK") == TRUE) { //"AT+CWMODE?\r\n+CWMODE:1\r\n\r\nOK\r\n";
        split(buff, "\r\n", results);
        if(startWith(results[1], "+CWMODE") == TRUE) {
            if(contains(results[1], "1")) {
                mode = ESP8266_STATION;
            } else if(contains(results[1], "2")) {
                mode = ESP8266_AP;
            }  else if(contains(results[1], "3")) {
                mode = ESP8266_MIX;
            }
        }
        delayInMilliSeconds(2000);
    }
    if(size > 0 && contains(buff, "busy")) {
        delayInMilliSeconds(5000);
    }
    return mode;
}

/**
 * 设置ESP8266工作模式
 * */
enum WorkingResultEnum setESP8266WorkingMode(enum ESPWorkingMode espWorkingMode) {
    char *commandPattern = "AT+CWMODE=%d\r\n", command[64] = {0}, buff[512] = {0};
    uint16_t size = 0, timeout = 10;
    sprintf(command, commandPattern, espWorkingMode);

    receiveDataFromUSART1(buff, &size, sizeof(buff));//发送前先清空接收缓冲区
    sendDataToUSART1(command);
    delayInMilliSeconds(5000);

    size = 0;
    while (size == 0 && timeout--) { //等待返回数据, 1000ms
        receiveDataFromUSART1(buff, &size, sizeof(buff));
        delayInMilliSeconds(100);
    }
    if(size > 0 && contains(buff, "OK") == TRUE) { //设置成功后重启
        sendDataToUSART1("AT+RST\r\n");

        size = 0;
        timeout = 30;
        while (size == 0 && timeout--) {
            delayInMilliSeconds(100); //等待重启完成，最多3000ms(重启成功后会发初始化数据)
            receiveDataFromUSART1(buff, &size, sizeof(buff));//清空缓冲区
        }
        if(size > 0) {
            delayInMilliSeconds(500);
        }
        delayInMilliSeconds(2000);
        return SUCCEED;
    }
    if(size > 0 && contains(buff, "busy")) {
        delayInMilliSeconds(5000);
    }
    return FAILED;
}


/**
 * 查看当前连接，返回ssid
 * */
WiFiConfig checkConnection() {
    char *command = "AT+CWJAP?\r\n", buff[128] = {0}, results[8][64] = {0}, count;
    uint16_t size = 0, timeout = 20;
    WiFiConfig wiFiConfig;
    clearString(wiFiConfig.ssid, sizeof(wiFiConfig.ssid));
    clearString(wiFiConfig.mac, sizeof(wiFiConfig.mac));
    clearString(wiFiConfig.channel, sizeof(wiFiConfig.channel));
    clearString(wiFiConfig.rssi, sizeof(wiFiConfig.rssi));
    receiveDataFromUSART1(buff, &size, sizeof(buff));//发送前先清空接收缓冲区

    sendDataToUSART1(command);
    delayInMilliSeconds(5000);
    size = 0;
    while (size == 0 && timeout--) {
        delayInMilliSeconds(10);
        receiveDataFromUSART1(buff, &size, sizeof(buff)); //AT+CWJAP?\r\n+CWJAP:"RJCJH","cc:08:fb:0d:bb:7e",6,-49\r\n\r\nOK
    }
    if(size > 0) {
        split(buff, "\r\n", results);
        clearString(buff, sizeof(buff));
        strcpy(results[1], buff);//+CWJAP:"RJCJH","cc:08:fb:0d:bb:7e",6,-49
        for(count = 0; count < 8; count++) {
            clearString(results[count], 128);
        }
        replaceAll(buff, "\"", "", results[0]);//+CWJAP:RJCJH,cc:08:fb:0d:bb:7e,6,-49
        replaceAll(results[0], "+CWJAP:", "", results[1]);//RJCJH,cc:08:fb:0d:bb:7e,6,-49
        split(results[1], ",", &results[2]);
        strcpy(results[2], wiFiConfig.ssid);
        strcpy(results[3], wiFiConfig.mac);
        strcpy(results[4], wiFiConfig.channel);
        strcpy(results[5], wiFiConfig.rssi);
        delayInMilliSeconds(2000);
    }
    if(size > 0 && contains(buff, "busy")) {
        delayInMilliSeconds(5000);
    }
    return wiFiConfig;
}

/**
 * 搜索wifi列表
 * */
//TODO Steven 不可用，占用栈空间太大
void searchWiFi(WiFiStruct wifiList[10]) {
    char *buff = (char *) malloc(1024 * sizeof(char));
    char *element = (char *) malloc(64 * sizeof(char));
    char *command = "AT+CWLAP\r\n";
    char ssid[32] = {0}, rssi[5] = {0}, results[8][32] = {0}, tempStr[64] = {0};
    uint16_t size = 0, timeout = 100, start, end, count, pointer = 0;


    receiveDataFromUSART1(buff, &size, sizeof(buff));//发送前先清空接收缓冲区
    clearString(buff, sizeof(buff));

    sendDataToUSART1(command);
    delayInMilliSeconds(5000);
    while (size == 0 && timeout--) {
        receiveDataFromUSART1(buff, &size, sizeof(buff));
        delayInMilliSeconds(100);
    }

    if(size > 0) {
        for(count = 0; count < 10; count++) {//+CWLAP:(4,"RJCJH",-47,"cc:08:fb:0d:bb:7e",6,21,0)
            start = indexOf(&buff[pointer], ":(");
            end = indexOf(&buff[pointer], ")");
            subString(&buff[pointer], start, end, element);//(4,"RJCJH",-47,"cc:08:fb:0d:bb:7e",6,21,0)
            replaceAll(element, "\"", "", tempStr);//(4,RJCJH,-47,cc:08:fb:0d:bb:7e,6,21,0)
            split(tempStr, ",", results);
            strcpy(results[1], wifiList[count].ssid);
            sprintf(wifiList[count].rssi, "%d", results[2]);
            pointer += (strlen(element) + strlen("+CWLAP:"));
            clearString(tempStr, 64);
            clearString(element, 64);
        }
    }
}

/**
 * 连接WiFi
 * */
enum WorkingResultEnum connectWiFi(const char *ssid, const char *password) {
    char *commandPattern = "AT+CWJAP=\"%s\",\"%s\"\r\n", command[64] = {0}, buff[128] = {0};
    uint16_t size = 0, timeout = 100;

    receiveDataFromUSART1(buff, &size, sizeof(buff));//发送前先清空接收缓冲区		
    clearString(buff, sizeof(buff));

    sprintf(command, commandPattern, ssid, password);
    sendDataToUSART1(command);
    delayInMilliSeconds(5000);
    size = 0;
    while (size == 0 && timeout--) {
        delayInMilliSeconds(100);
        receiveDataFromUSART1(buff, &size, sizeof(buff));
    }

    if(size > 0 && (contains(buff, "OK") == TRUE || contains(buff, "CONNECTED") == TRUE)) {
        delayInMilliSeconds(2000);
        return SUCCEED;
    }
    if(size > 0 && contains(buff, "busy")) {
        delayInMilliSeconds(5000);
    }
    return FAILED;
}

/**
 * 断开WiFi
 * */
enum WorkingResultEnum disconnectWiFi() {
    char *command = "AT+CWQAP\r\n", buff[128] = {0};
    uint16_t size = 0, timeout = 10;

    receiveDataFromUSART1(buff, &size, sizeof(buff));//发送前先清空接收缓冲区
    clearString(buff, sizeof(buff));

    sendDataToUSART1(command);
    delayInMilliSeconds(5000);

    while (size == 0 && timeout--) {
        delayInMilliSeconds(100);
        receiveDataFromUSART1(buff, &size, sizeof(buff));
    }

    if(size > 0 && contains(buff, "DISCONNECT")) {
        return SUCCEED;
    }
    if(size > 0 && contains(buff, "busy")) {
        delayInMilliSeconds(5000);
    }
    return FAILED;
}

/**
 * 获取IP地址, AP模式下不能调用，否则可能死机
 * */
void getIPV4Address(char *ipv4) {
    char *command = "AT+CIFSR\r\n", buff[256] = {0}, results[8][64] = {0};
    uint16_t size = 0, timeout = 20, count = 0;
    receiveDataFromUSART1(buff, &size, sizeof(buff));//发送前先清空接收缓冲区

    sendDataToUSART1(command);
    delayInMilliSeconds(5000);
    size = 0;
    while (size == 0 && timeout--) {
        delayInMilliSeconds(10);
        receiveDataFromUSART1(buff, &size, sizeof(buff));
    }

    if(size > 0 && contains(buff, "OK")) {//AT+CIFSR\r\n+CIFSR:STAIP,"192.168.0.100"\r\n+CIFSR:STAMAC,"ec:fa:bc:2e:dc:1b"\r\n\r\nOK\r\n
        split(buff, "\r\n", results);
        clearString(buff, sizeof(buff));
        strcpy(results[1], buff);//+CIFSR:STAIP,"192.168.0.100"
        for(count = 0; count < 8; count++) {
            clearString(results[count], 64);
        }
        split(buff, "\"", results);
        strcpy(results[1], ipv4);
        delayInMilliSeconds(2000);
    }
    if(size > 0 && contains(buff, "busy")) {
        delayInMilliSeconds(5000);
    }
}

/**
 * 初始化为Station模式
 * */
void initStation(const char *ssid, const char *password) {
    enum ESPWorkingMode workingMode = getESPWorkingMode();
    if(workingMode != ESP8266_STATION) {
        setESP8266WorkingMode(ESP8266_STATION);
    }
    connectWiFi(ssid, password);
}

/**
 * ESP8266 初始化
 * */
void initESP8266(const char *ssid, const char *password) {
    initStation(ssid, password);
}
