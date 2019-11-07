//
// Created by Steven on 2019/11/6.
//

#include "tcp.h"



/**
 * ���õ�ͨ����͸��ģʽ
 * */
void setMuxMode() {
    char *commandMux = "AT+CIPMUX=0\r\n";
    char *commandTransparent = "AT+CIPMODE=1\r\n";
    char buff[512] = {0};
    uint16_t size = 0, timeout = 20;

    receiveDataFromUSART1(buff, &size, sizeof(buff));
    clearString(buff, sizeof(buff));
    sendDataToUSART1(commandMux);

    while (size == 0 && timeout--) {
        receiveDataFromUSART1(buff, &size, sizeof(buff));
        delayInMilliSeconds(100);
    }
    delayInMilliSeconds(2000);

    size = 0;
    timeout = 20;
    sendDataToUSART1(commandTransparent);
    while (size == 0 && timeout--) {
        receiveDataFromUSART1(buff, &size, sizeof(buff));
        delayInMilliSeconds(100);
    }
    delayInMilliSeconds(2000);
}

/**
 * ����TCP����
 * host ֻ����IP��ַ
 * */
enum WorkingResultEnum connectServer(char *host, uint16_t port) {
    char command[64] = {0}, buff[512] = {0};
    char *pattern = "AT+CIPSTART=\"TCP\",\"%s\",%d\r\n";
    uint16_t size = 0, timeout = 20, addr = 0;
    sprintf(command, pattern, host, port);

    setMuxMode();

    receiveDataFromUSART1(buff, &size, sizeof(buff));
    clearString(buff, sizeof(buff));

    sendDataToUSART1(command);
    delayInMilliSeconds(1000);

    size = 0;
    timeout = 100;
    while (contains(buff, "CONNECT") == FALSE && timeout--) {
        delayInMilliSeconds(100);
        clearString(buff, sizeof(buff));
        receiveDataFromUSART1(buff, &size, sizeof(buff));
    }

    if(contains(buff, "CONNECT")) {
        return SUCCEED;
    }

    if(contains(buff, "busy")) {
        delayInMilliSeconds(5000);
    }
    return FAILED;
}

/**
 * �Ͽ�TCP����
 * */
enum WorkingResultEnum disconnectServer() {
    return SUCCEED;
}

/**
 * ������Ϣ
 * */
enum WorkingResultEnum sendTCPMsg(char *msg) {
    sendDataToUSART1(msg);
    return SUCCEED;
}

/**
 * ��ȡ��Ϣ
 * */
void readTCPMsg(char *buff, uint16_t *size, uint16_t maxSize) {
    receiveDataFromUSART1(buff, size, maxSize);
}
