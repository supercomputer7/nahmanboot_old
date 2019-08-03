#include "stdmath.h"
double power(double x,double y)
{
    double res=x;
    for(int i=0;i<y;++i)
        res *= x;
    return res;
}