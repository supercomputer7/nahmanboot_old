#include "string.h"
#include <stdmath.h>

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
uint32_t StringToInt(const char *str,uint8_t len)
{
    uint32_t tmp = 0;
    if(str[0] == '0' && (str[1] == 'x' || str[1] == 'X'))
    {
        for(uint8_t i=2; i<len;++i)
        {   
            uint8_t tmp2 = (uint8_t)str[len-i-1]-'0';
            tmp +=  tmp2 * (power(16,i));
        }
    }
    else
    {
        for(uint8_t i=0; i<len;++i)
        {   
            uint8_t tmp2 = ((uint8_t)(str[len-i-1]))-'0';
            tmp +=  tmp2 * (power(10,i));

            *(uint8_t*)(0x60000+i) = tmp2;
        }
    } 
    return tmp;
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
