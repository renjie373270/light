//
// Created by Steven on 2019/10/31.
//

#include "ESP8266.h"

/**
 * 查看ESP8266工作模式
 * */
enum ESPWorkingMode getESPWorkingMode() {
    char *command = "AT+CWMODE?\r\n", buff[512] = {0}, results[8][128] = {0};
    uint16_t size = 0, timeout = 20;

    receiveDataFromUSART1(buff, &size);//发送前先清空接收缓冲区
    sendDataToUSART1(command);

    size = 0;
    while (size == 0 && timeout--) {
        delayInMilliSeconds(10);
        receiveDataFromUSART1(buff, &size);//等待返回数据，最多200ms
    }
    if(size > 0 && contains(buff, "OK") == COMPARE_TRUE) { //"AT+CWMODE?\r\n+CWMODE:1\r\n\r\nOK\r\n";
        split(buff, "\r\n", results);
        if(startWith(results[1], "+CWMODE") == TRUE) {
            if(contains(results[1], "1")) {
                return ESP8266_STATION;
            } else if(contains(results[1], "2")) {
                return ESP8266_AP;
            }  else if(contains(results[1], "3")) {
                return ESP8266_MIX;
            }
        }
    }
    return ESP8266_ERROR;
}

/**
 * 设置ESP8266工作模式
 * */
void setESP8266WorkingMode(enum ESPWorkingMode espWorkingMode) {
    char *commandPattern = "AT+CWMODE=%d\r\n", command[64] = {0}, buff[512];
    uint16_t size = 0, timeout = 20;
    sprintf(command, commandPattern, espWorkingMode);

    receiveDataFromUSART1(buff, &size);//发送前先清空接收缓冲区
    sendDataToUSART1(command);

    size = 0;
    while (size == 0 && timeout--) { //等待返回数据, 200ms
        receiveDataFromUSART1(buff, &size);
        delayInMilliSeconds(10);
    }
    if(size > 0 && contains(buff, "OK") == COMPARE_TRUE) { //设置成功后重启
        sendDataToUSART1("AT+RST\r\n");

        size = 0;
        timeout = 20;
        while (size == 0 && timeout--) {
            delayInMilliSeconds(100); //等待重启完成，最多2000ms(重启成功后会发初始化数据)
            receiveDataFromUSART1(buff, &size);//清空缓冲区
        }
    }
}


/**
 * 查看当前连接，返回ssid
 * */
void checkCurrentConnection(char *ssid, char *mac, char *channel, char *rssi) {
    char *command = "AT+CWJAP?\r\n", buff[128], results[8][128] = {0}, count;
    uint16_t size = 0, timeout = 20;
    receiveDataFromUSART1(buff, &size);//发送前先清空接收缓冲区

    sendDataToUSART1(command);
    size = 0;
    while (size == 0 && timeout--) {
        delayInMilliSeconds(10);
        receiveDataFromUSART1(buff, &size); //AT+CWJAP?\r\n+CWJAP:"RJCJH","cc:08:fb:0d:bb:7e",6,-49\r\n\r\nOK
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
        strcpy(results[2], ssid);
        strcpy(results[3], mac);
        strcpy(results[4], channel);
        strcpy(results[5], rssi);
    }
}

/**
 * 搜索WiFi
 * */
void searchWiFi(char **ssidList) {

}

/**
 * 连接WiFi
 * */
void connectWiFi(char *ssid, char *password) {

}

/**
 * 获取IP地址
 * */
void getIPV4Address(char *ipv4Address) {

}


/**
 * ESP8266 初始化
 * */
void initESP8266() {
//    receiveDataFromUSART1 TODO Steven 清空串口缓冲区
}
