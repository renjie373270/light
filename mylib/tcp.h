//
// Created by Steven on 2019/11/6.
//

#ifndef LIGHT_TCP_H
#define LIGHT_TCP_H

#include "main.h"

enum ConnectionStatus {
    CONNECTED = 1,
    DISCONNECTED = 0
};


enum WorkingResultEnum connectServer(char *host, uint16_t port);
enum WorkingResultEnum disconnectServer(void);
enum WorkingResultEnum sendTCPMsg(char *msg);
void readTCPMsg(char *buff, uint16_t *size, uint16_t maxSize);

#endif //LIGHT_TCP_H
