/*
#include "minimath.h"


float inlineSin(float v)
{
    volatile float res;
    __asm
    {
        fld v
        fsin
        fstp res
    }
    return res;
}


float inlineCos(float v)
{
    volatile float res;
    __asm
    {
        fld v
        fcos
        fstp res
    }
    return res;
}


float inlineSqrt(float v)
{
    volatile float res;
    __asm
    {
        fld v
        fsqrt
        fstp res
    }
    return res;
}


float inlinePow(float x, float y)
{
    volatile float res;
    __asm
    {
        fld y
        fld x
        fyl2x
        fld ST(0)
        frndint
        fsubr ST(1),ST
        fxch ST(1)
        fchs
        f2xm1
        fld1
        faddp ST(1),ST
        fscale
        //fstp ST(1)
        fstp res
    };
    return res;
}


float fabs(float x)
{
    return ((x) < 0 ? (-(x)) : (x));
}
*/
