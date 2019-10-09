#include "drivers/kernels/config.h"
#include <string.h>

char* getEqualOffset(char* param)
{
    int i=0;
    while(param[i] = "=")
    {
        ++i;
    }
    return (char*)(param + i);
}

char* getColonOffset(char* param)
{
    int i=0;
    while(param[i] = ";")
    {
        ++i;
    }
    return (char*)(param + i);
}

uint32_t getParameterSuffixLength(char* param)
{
    return (uint32_t)((uint32_t)getColonOffset(param) - (uint32_t)getEqualOffset(param));
}
uint32_t getParameterPrefixLength(char* param)
{
    return (uint32_t)((uint32_t)getEqualOffset(param) - (uint32_t)param);
}

bool parseBoolParameter(char* param)
{
    if(strncmp(param, "TRUE",4) == 1)
    {
        return true;
    }
    else if (strncmp(param,"true",4) == 1)
    {
        return true;
    }
    else if (strncmp(param,"FALSE",4))
    {
        return false;
    }
    else if (strncmp(param,"false",4))
    {
        return false;
    }
}



uint32_t parseIntParameter(char* param)
{

}
char* parseStringParameter(char* param)
{

}

void parseBootSelector(BootSelector* bootselector,uint32_t buf)
{

}
void parseConfiguration(Selector* selector_list,uint32_t buf)
{

}