#include "stdmath.h"
double power(double x,double y)
{
    if(y==0)
        return 1;
    if(x==0)
        return 0;
    double res=x;
    for(int i=0;i<y;++i)
        res *= x;
    return res;
}