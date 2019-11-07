#include "main.h"

#define SSID "RJCJH"
#define PASSWORD "Apple@123@369"
#define HOST "192.168.0.104"
#define PORT 8080
#define SERIAL_NO 100

uint8_t lightStatus[4] = {0};
char ip[20] = {0};
enum ConnectionStatus connectionStatus = DISCONNECTED;
enum WorkingResultEnum connectResult = FAILED;

void initAllDevices() {
    SystemInit();
    delayInit();
//    initDog();
    initWiFiUSART();
    initLEDGPIO();
}

/**
 * ������������
 * */
void connectionTask() {
    if(isEmpty(ip)) { //����IP�ж��Ƿ�ɹ�����WiFi
        getIPV4Address(ip);
    }

    if(isEmpty(ip)) {//���IP��Ϊ�գ���������WiFi
        initESP8266(SSID, PASSWORD);
    }

    if(!isEmpty(ip) && connectionStatus == DISCONNECTED) { //���ӷ�����
        connectResult = connectServer(HOST, PORT);
        if(connectResult == SUCCEED) {
            connectionStatus = CONNECTED;
        }
    }
}

/**
 * ��������
 * */
void receiveTask() {
    char buff[128] = {0};
    uint8_t status[4] = {0};
    uint16_t size = 0;

    readTCPMsg(buff, &size, sizeof(buff));
    if(contains(buff, "error")) {
        connectionStatus = DISCONNECTED;
    }

    if(size > 0) {
        status[0] = stringToHex(&buff[0]);
        status[1] = stringToHex(&buff[2]);
        status[2] = stringToHex(&buff[4]);
        status[3] = stringToHex(&buff[8]);
    }

    if(status[0] == lightStatus[0] && status[1] == lightStatus[1]) {
        lightStatus[2] = status[2];
        lightStatus[3] = status[3];
        if(lightStatus[3] == 1) {
            lightOn();
        } else {
            lightOff();
        }
    }
}

/**
 * �ϴ��Ƶ�״̬���൱��������
 * */
void uploadingStatus() {
    char message[20] = {0};
    enum WorkingResultEnum workingResultEnum = FAILED;
    lightStatus[0] = SERIAL_NO >> 8;//��Ÿ�λ
    lightStatus[1] = SERIAL_NO & 0xFF;//��ŵ�λ
    hexToString(lightStatus[0], &message[0]);
    hexToString(lightStatus[1], &message[2]);
    hexToString(lightStatus[2], &message[4]);
    hexToString(lightStatus[3], &message[6]);
    workingResultEnum = sendTCPMsg(message);
}


int main() {
    uint16_t uploadCountDown = 0, ledCountDown = 0;
    initAllDevices();
    connectionTask();
    while (1) {
        connectionTask();
        receiveTask();

        uploadCountDown++;
        ledCountDown++;

        if(uploadCountDown == 20) {
            uploadingStatus();
            uploadCountDown = 0;
        }

        if(ledCountDown == 20) {
            ledOn();
        }

        if(ledCountDown == 40) {
            ledOff();
            ledCountDown = 0;
        }
        delayInMilliSeconds(10);
    }
}
