//
// Created by Steven on 2019/11/1.
//

#ifndef LIGHT_MYSTRING_H
#define LIGHT_MYSTRING_H

#include "main.h"

enum ResultEnum {
    TRUE = 1,
    FALSE = 0
};

uint16_t strlen(const char * source);
enum ResultEnum startWith(char * source, char * str);
enum ResultEnum endWith(char * source, char * str);
enum ResultEnum contains(char * source, char * str);
void subString(const char * source, uint16_t startInclusive, uint16_t endInclusive, char * result);
uint16_t indexOf(char * source, char * str);
uint16_t lastIndexOf(char * source, char * str);
void strcpy(char * source, char * destination);
void strncpy(char * source, char * destination, uint16_t size);
void replaceFirst(char * source, char * oldStr, char * newStr, char * result);
void replaceAll(char * source, char * oldStr, char * newStr, char * result);


#endif //LIGHT_MYSTRING_H
