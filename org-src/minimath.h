#ifndef MINIMATH_H_INCLUDED
#define MINIMATH_H_INCLUDED


#undef PI
#define PI 3.1415926535897932f

// we want only pow implementation of functions below
// when intrinsic functions are in use
#ifdef _DEBUG
#include <math.h>
#endif
/*
#define sin inlineSin
#define cos inlineCos
#define sqrt inlineSqrt
*/
#define pow inlinePow
#define rand inlineRand
#define srand inlineSrand
#undef RAND_MAX
#define RAND_MAX 65535

#define randf inlineRandf


static unsigned int randseed = 0;

static void inlineSrand(unsigned int seed)
{
    randseed = seed;
}

static unsigned int inlineRand()
{
    randseed = randseed * 0x343fd + 0x269ec3;
    return randseed >> 16;
}

static float inlineRandf(float scale)
{
    return scale * rand() / RAND_MAX;
}


static /*__forceinline*/ float inlineSin(float v)
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

static /*__forceinline*/ float inlineCos(float v)
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

static /*__forceinline*/ float inlineSqrt(float v)
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


//static /*__forceinline*/ float inlinePow(float x, float y)
/*
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
        fstp ST(1)
        fstp res
    };
    return res;
}
*/

// don't care about FDIV bugs (FPREM below triggers this)
#pragma warning(disable : 4725)

static /*__forceinline*/ float inlinePow(float base,float exp)
{
    float tmpVal;
    _asm
    {
        fld [exp];
        fld [base];
        fyl2x;
        fld1;
        fld st(1);
        fprem;
        f2xm1;
        faddp st(1),st(0);
        fscale;
        fxch;
        fstp st(0);
        fstp [tmpVal];
    }
    return tmpVal;
}


//#define fabs(x) ((x) < 0 ? (-(x)) : (x))
// using intrinsic currently
/*static float fabs(float x)
{
    return ((x) < 0 ? (-(x)) : (x));
}*/


/*
extern float inlineSin(float v);
extern float inlineCos(float v);
extern float inlineSqrt(float v);
extern float inlinePow(float x, float y);
*/


#endif // !MINIMATH_H_INCLUDED
