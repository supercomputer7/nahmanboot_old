#include "string.h"

int strncmp(const char *str1, const char *str2,uint16_t len)
{
    int i=0;
    while(i<len)
    {
        if(str1[i] == '\0' && str2[i] == '\0')
            break;
        if(str1[i] != str2[i])
            return 0;
        i++;
    }
    return 1;
}

uint32_t strlen(const char *str)
{
    char *p = (char*)str;
    uint32_t count = 0;
    while(*p != '\0')
    {
        count++;
        p++;
    }
    return count;
}
