#ifndef STDSTRING_H__
#define STDSTRING_H__

#include "stddef.h"
#include "stdint.h"

int strncmp(const char *str1, const char *str2,uint16_t len);
uint32_t StringToInt(const char *str,uint8_t len);
uint32_t strlen(const char *str);

#endif