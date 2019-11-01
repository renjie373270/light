//
// Created by Steven on 2019/11/1.
//

#include "mystring.h"


uint16_t strlen(const char * source) {
    uint16_t count;
    for(count = 0; count < 4096; count ++) {
        if(source[count] == '\0') {
            return count;
        }
    }
    return 0;
}

enum ResultEnum startWith(char * source, char * str) {
    uint16_t sizeOfSource = strlen(source);
    uint16_t sizeOfStr = strlen(str);
    uint16_t count;
    if((sizeOfSource < sizeOfStr) || (sizeOfStr == 0)) {
        return FALSE;
    }
    for(count = 0; count < sizeOfStr; count++) {
        if(source[count] != str[count]) {
            return FALSE;
        }
    }
    return TRUE;
}

enum ResultEnum endWith(char * source, char * str) {
    uint16_t sizeOfSource = strlen(source);
    uint16_t sizeOfStr = strlen(str);
    uint16_t count;
    if((sizeOfSource < sizeOfStr) || (sizeOfStr == 0)) {
        return FALSE;
    }

    for(count = 1; count <= sizeOfStr; count++) {
        if(source[sizeOfSource - count] != str[sizeOfStr - count]) {
            return FALSE;
        }
    }
    return TRUE;
}

enum ResultEnum contains(char * source, char * str) {
    uint16_t sizeOfSource = strlen(source);
    uint16_t sizeOfStr = strlen(str);
    uint16_t count = 0;
    if(sizeOfSource < sizeOfStr) {
        return FALSE;
    }

    for(count = 0; count <= sizeOfSource - sizeOfStr; count++) {
        if(startWith(&source[count], str) == TRUE) {
            return TRUE;
        }
    }
    return FALSE;
}

void subString(const char * source, uint16_t startInclusive, uint16_t endInclusive, char * result) {
    uint16_t count = 0;
    for(count = 0; count <= endInclusive - startInclusive; count++) {
        result[count] = source[count + startInclusive];
    }
}

int16_t indexOf(char * source, char * str) {
    uint16_t sizeOfSource = strlen(source);
    uint16_t sizeOfStr = strlen(str);
    uint16_t count = 0;
    if(sizeOfSource < sizeOfStr) {
        return -1;
    }

    for(count = 0; count <= sizeOfSource - sizeOfStr; count++) {
        if(startWith(&source[count], str) == TRUE) {
            return count;
        }
    }
    return -1;
}

int16_t lastIndexOf(char * source, char * str) {
    uint16_t sizeOfSource = strlen(source);
    uint16_t sizeOfStr = strlen(str);
    uint16_t count = 0;
    char subOfSource[1024];

    if(sizeOfSource < sizeOfStr) {
        return -1;
    }

    for(count = 0; count <= sizeOfSource - sizeOfStr; count++) {
        subString(source, 0, sizeOfSource - count, subOfSource);
        if(endWith(subOfSource, str) == TRUE) {
            return (sizeOfSource - sizeOfStr - count);
        }
    }
    return -1;
}


void strcpy(char * source, char * destination) {
    uint16_t sizeOfSource = strlen(source);
    uint16_t count = 0;

    for(count = 0; count < sizeOfSource; count++) {
        destination[count] = source[count];
    }
}

void strncpy(char * source, char * destination, uint16_t size) {
    uint16_t sizeOfSource = strlen(source);
    uint16_t minLength = sizeOfSource < size ? sizeOfSource : size;
    uint16_t count = 0;

    for(count = 0; count < minLength; count++) {
        destination[count] = source[count];
    }
}

void replaceFirst(char * source, char * oldStr, char * newStr, char * result) {
    uint16_t sizeOfSource = strlen(source);
    uint16_t sizeOfOldStr = strlen(oldStr);
    uint16_t sizeOfNewStr = strlen(newStr);
    int16_t index = indexOf(source, oldStr);

    if(index == -1) {
        strcpy(source, result);
        return;
    }

    strncpy(source, result, index);
    strcpy(newStr, &result[index]);
    strcpy(&source[index + sizeOfOldStr], &result[index + sizeOfNewStr]);
}

void replaceAll(char * source, char * oldStr, char * newStr, char * result) {
    char tempString1[512] = {0}, tempString2[512] = {0};
    uint16_t lengthOfOld = strlen(oldStr), lengthOfNew = strlen(newStr);
    if(contains(source, oldStr) == FALSE) {
        strcpy(source, result);
        return;
    }

    strcpy(source, tempString1);
    while (contains(tempString1, oldStr) == TRUE) {
        replaceFirst(tempString1, oldStr, newStr, tempString2);
        clearString(tempString1, 512);

        strcpy(tempString2, tempString1);
        clearString(tempString2, 512);
    }
    strcpy(tempString1, result);
}

void split(char * source, char * delimeter, char results[8][128]) {
    uint16_t lengthOfSource = strlen(source);
    uint16_t lengthOfDelimeter = strlen(delimeter);
    uint16_t count = 0, row = 0, column = 0;

    for(count = 0; count < lengthOfSource; count++) {
        if(startWith(&source[count], delimeter) == TRUE) {
            while(startWith(&source[count], delimeter) == TRUE) {
                count += lengthOfDelimeter;
            }
            row ++;
            column = 0;
        }
        results[row][column] = source[count];
        column ++;
    }
}

void clearString(char * source, uint16_t size) {
    uint16_t count = 0;
    for (count = 0; count < size; count++) {
        source[count] = 0;
    }
}
