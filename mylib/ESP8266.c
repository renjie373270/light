//
// Created by Steven on 2019/10/31.
//

#include "ESP8266.h"

/**
 * �鿴ESP8266����ģʽ
 * */
enum ESPWorkingMode getESPWorkingMode() {
    char *command = "AT+CWMODE?\r\n", buff[512] = {0}, results[8][128] = {0};
    uint16_t size = 0, timeout = 20;

    receiveDataFromUSART1(buff, &size);//����ǰ����ս��ջ�����
    sendDataToUSART1(command);

    size = 0;
    while (size == 0 && timeout--) {
        delayInMilliSeconds(10);
        receiveDataFromUSART1(buff, &size);//�ȴ��������ݣ����200ms
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
 * ����ESP8266����ģʽ
 * */
void setESP8266WorkingMode(enum ESPWorkingMode espWorkingMode) {
    char *commandPattern = "AT+CWMODE=%d\r\n", command[64] = {0}, buff[512];
    uint16_t size = 0, timeout = 20;
    sprintf(command, commandPattern, espWorkingMode);

    receiveDataFromUSART1(buff, &size);//����ǰ����ս��ջ�����
    sendDataToUSART1(command);

    size = 0;
    while (size == 0 && timeout--) { //�ȴ���������, 200ms
        receiveDataFromUSART1(buff, &size);
        delayInMilliSeconds(10);
    }
    if(size > 0 && contains(buff, "OK") == COMPARE_TRUE) { //���óɹ�������
        sendDataToUSART1("AT+RST\r\n");

        size = 0;
        timeout = 20;
        while (size == 0 && timeout--) {
            delayInMilliSeconds(100); //�ȴ�������ɣ����2000ms(�����ɹ���ᷢ��ʼ������)
            receiveDataFromUSART1(buff, &size);//��ջ�����
        }
    }
}


/**
 * �鿴��ǰ���ӣ�����ssid
 * */
void checkCurrentConnection(char *ssid, char *mac, char *channel, char *rssi) {
    char *command = "AT+CWJAP?\r\n", buff[128], results[8][128] = {0}, count;
    uint16_t size = 0, timeout = 20;
    receiveDataFromUSART1(buff, &size);//����ǰ����ս��ջ�����

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
 * ����WiFi
 * */
void searchWiFi(char **ssidList) {

}

/**
 * ����WiFi
 * */
void connectWiFi(char *ssid, char *password) {

}

/**
 * ��ȡIP��ַ
 * */
void getIPV4Address(char *ipv4Address) {

}


/**
 * ESP8266 ��ʼ��
 * */
void initESP8266() {
//    receiveDataFromUSART1 TODO Steven ��մ��ڻ�����
}
