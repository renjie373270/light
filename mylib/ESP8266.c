//
// Created by Steven on 2019/10/31.
//

#include "ESP8266.h"

/**
 * �鿴ESP8266����ģʽ
 * */
enum ESPWorkingMode getESPWorkingMode() {
    char *command = "AT+CWMODE?\r\n", buff[256] = {0}, results[8][64] = {0};
    uint16_t size = 0, timeout = 20;

    receiveDataFromUSART1(buff, &size);//����ǰ����ս��ջ�����
    sendDataToUSART1(command);

    size = 0;
    while (size == 0 && timeout--) {
        delayInMilliSeconds(10);
        receiveDataFromUSART1(buff, &size);//�ȴ��������ݣ����200ms
    }
    if(size > 0 && contains(buff, "OK") == TRUE) { //"AT+CWMODE?\r\n+CWMODE:1\r\n\r\nOK\r\n";
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
enum WorkingResultEnum setESP8266WorkingMode(enum ESPWorkingMode espWorkingMode) {
    char *commandPattern = "AT+CWMODE=%d\r\n", command[64] = {0}, buff[512] = {0};
    uint16_t size = 0, timeout = 20;
    sprintf(command, commandPattern, espWorkingMode);

    receiveDataFromUSART1(buff, &size);//����ǰ����ս��ջ�����
    sendDataToUSART1(command);

    size = 0;
    while (size == 0 && timeout--) { //�ȴ���������, 200ms
        receiveDataFromUSART1(buff, &size);
        delayInMilliSeconds(10);
    }
    if(size > 0 && contains(buff, "OK") == TRUE) { //���óɹ�������
        sendDataToUSART1("AT+RST\r\n");

        size = 0;
        timeout = 20;
        while (size == 0 && timeout--) {
            delayInMilliSeconds(100); //�ȴ�������ɣ����2000ms(�����ɹ���ᷢ��ʼ������)
            receiveDataFromUSART1(buff, &size);//��ջ�����
        }
        return SUCCEED;
    }
    return FAILED;
}


/**
 * �鿴��ǰ���ӣ�����ssid
 * */
WiFiConfig checkConnection() {
    char *command = "AT+CWJAP?\r\n", buff[128] = {0}, temp[128] = {0}, results[8][64] = {0}, count;
    uint16_t size = 0, timeout = 20;
    WiFiConfig wiFiConfig;
    clearString(wiFiConfig.ssid, sizeof(wiFiConfig.ssid));
    clearString(wiFiConfig.mac, sizeof(wiFiConfig.mac));
    clearString(wiFiConfig.channel, sizeof(wiFiConfig.channel));
    clearString(wiFiConfig.rssi, sizeof(wiFiConfig.rssi));
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
        strcpy(results[2], wiFiConfig.ssid);
        strcpy(results[3], wiFiConfig.mac);
        strcpy(results[4], wiFiConfig.channel);
        strcpy(results[5], wiFiConfig.rssi);
    }
    return wiFiConfig;
}

/**
 * ����WiFi
 * */
enum WorkingResultEnum connectWiFi(char *ssid, char *password) {
    char *commandPattern = "AT+CWJAP=\"%s\",\"%s\"\r\n", command[64] = {0}, buff[128] = {0};
    uint16_t size = 0, timeout = 20;
    receiveDataFromUSART1(buff, &size);//����ǰ����ս��ջ�����

    sprintf(command, commandPattern, ssid, password);
    sendDataToUSART1(command);
    size = 0;
    while (size == 0 && timeout--) {
        delayInMilliSeconds(100);
        receiveDataFromUSART1(buff, &size);
    }

    if(size > 0 && contains(buff, "CONNECTED")) {
        return SUCCEED;
    }
    return FAILED;
}

/**
 * �Ͽ�WiFi
 * */
enum WorkingResultEnum disconnectWiFi() {
    char *command = "AT+CWQAP\r\n", buff[128] = {0};
    uint16_t size = 0, timeout = 10;
    sendDataToUSART1(command);

    while (size == 0 && timeout--) {
        delayInMilliSeconds(100);
        receiveDataFromUSART1(buff, &size);
    }

    if(size > 0 && contains(buff, "DISCONNECT")) {
        return SUCCEED;
    }
    return FAILED;
}

/**
 * ��ȡIP��ַ, APģʽ�²��ܵ��ã������������
 * */
void getIPV4Address(char *ipv4) {
    char *command = "AT+CIFSR\r\n", buff[256] = {0}, results[8][64] = {0};
    uint16_t size = 0, timeout = 20, count = 0;
    receiveDataFromUSART1(buff, &size);//����ǰ����ս��ջ�����

    sendDataToUSART1(command);
    size = 0;
    while (size == 0 && timeout--) {
        delayInMilliSeconds(10);
        receiveDataFromUSART1(buff, &size);
    }

    if(size > 0 && contains(buff, "OK")) {//AT+CIFSR\r\n+CIFSR:STAIP,"192.168.0.100"\r\n+CIFSR:STAMAC,"ec:fa:bc:2e:dc:1b"\r\n\r\nOK\r\n
        split(buff, "\r\n", results);
        clearString(buff, sizeof(buff));
        strcpy(results[1], buff);//+CIFSR:STAIP,"192.168.0.100"
        for(count = 0; count < 8; count++) {
            clearString(results[count], 128);
        }
        split(buff, "\"", results);
        strcpy(results[1], ipv4);
    }
}


/**
 * ��ʼ��ΪStationģʽ������WiFi
 * */
enum WorkingResultEnum initStationMode(char *ssid, char *password) {
    uint16_t count = 0, timeout = 100, size = 0;
    char buff[512] = {0};    
    enum WorkingResultEnum workingResultEnum = FAILED;

    //1.�ֶ�����
    sendDataToUSART1("AT+RST\r\n");
    while (contains(buff, "CONNECTED") == FALSE && timeout--) { //һֱ�ȴ�WIFI CONNECTED����
        receiveDataFromUSART1(buff, &size);
        delayInMilliSeconds(100);
    }

    //2. ���ӳɹ���ֱ���˳�
    if(size > 0 && contains(buff, "CONNECTED") == TRUE) {//WIFI CONNECTED\r\nWIFI GOT IP
        return SUCCEED;//���ӳɹ����˳�
    }

    //3. �������ʧ�ܣ����ù���ģʽ������һ��
    workingResultEnum = setESP8266WorkingMode(ESP8266_STATION);
    if(workingResultEnum == SUCCEED) {
        workingResultEnum = connectWiFi(ssid, password);
        if(workingResultEnum == SUCCEED) {
            return SUCCEED;
        }
    }
    return FAILED;
}

/**
 * ��ʼ��ΪAPģʽ������ҳ
 * */
void initAPMode() {
    enum WorkingResultEnum workingResultEnum = setESP8266WorkingMode(ESP8266_AP);

}


/**
 * ESP8266 ��ʼ��
 * */
void initESP8266() {
    char *ssid = "RJCJH", *password = "Apple@123@369";
    enum WorkingResultEnum workingResultEnum = initStationMode(ssid, password);
    if(workingResultEnum == FAILED) {
        initAPMode();
    }
}
