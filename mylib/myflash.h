//
// Created by Steven on 2019/10/14.
//

#ifndef SOLDER_MYFLASH_H
#define SOLDER_MYFLASH_H

#include "main.h"

void saveDataToFlash(uint16_t *buff, uint8_t length);
void readDataFromFlash(uint16_t *buff, uint8_t length);

#endif //SOLDER_MYFLASH_H
