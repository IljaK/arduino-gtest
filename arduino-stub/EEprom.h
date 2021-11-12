#pragma once
#include <stdint.h>
#include <stdio.h>
#include <cstring>

#define PROGMEM
class __FlashStringHelper
{

};

class String {
    const char *pStr;
public:
    String(const char *str):pStr(str) {

    }
    bool equals(const char *str) {
        return strcmp(pStr, str);
    }
};

#define F(string_literal) (string_literal)
#define PSTR(string_literal) (string_literal)