/* NOTE: This is an automatically generated file!
 * 
 * $Id: shapes-opt.h,v 1.8 2004/06/07 18:35:02 tonic Exp $
 * $Revision
 */

#ifndef SHAPES_OPT_H_INCLUDED
#define SHAPES_OPT_H_INCLUDED


#define SUPERSHAPE_COUNT    21
#define SUPERSHAPE_PARAMS   15

#define QUANTIZED_STEP      0.050000f
#define QUANTIZED_MIN_VALUE -98.050003f
#define QUANTIZED_DISTINCT_VALUE_COUNT 58

static float superShapes_opt[SUPERSHAPE_COUNT][SUPERSHAPE_PARAMS];
static unsigned char superShapes_opt_indices[SUPERSHAPE_COUNT][SUPERSHAPE_PARAMS] =
{
    {   38,  24,  28,  54,  24,   4,  36,  24,  24,   9,  24,  11,  48,  50,  28 },
    {    0,   0,   0,   0,   0,   0,  30,   0,   0,  38,   0,   0,   0,  48,  30 },
    {    0,   0,   0,  50,   0,   5,   0,   0,   0,   9,   8,   0,  53,  53,  24 },
    {   34,   0,  24,   0,   0,   3,  36,   0,   0,  21,  29,  19,  48,  48,   0 },
    {   30,   0,   0,  44,   0,  42,  39,   0,   0,   0,   0,   0,  42,  50,   0 },
    {   36,   0,   0,   0,   0,   6,  36,  28,   0,   9,  32,  22,  50,   0,   0 },
    {   40,   0,   0,   0,   0,   0,  40,  24,   0,  24,   0,  24,   0,   0,  34 },
    {   38,   0,  25,  10,  17,  51,  50,   0,   0,   2,  15,  13,   0,  57,  36 },
    {   35,   0,  24,  42,  12,   7,  34,   0,   0,   9,  31,  22,   0,  52,  30 },
    {   30,   0,   0,  38,  38,  38,  30,   0,   0,  38,  38,  38,  42,  48,  24 },
    {    0,   0,   0,  24,  24,  24,   0,   0,   0,  24,  24,  24,  38,  38,  28 },
    {   24,   0,   0,  47,  13,  41,   0,   0,   0,  38,  38,  38,  42,  48,   0 },
    {   28,   0,   0,  23,  20,  18,  29,   0,   0,  56,  56,  56,   0,  49,   0 },
    {   34,   0,   0,  24,  24,  24,   0,   0,   0,  24,  24,  24,  50,  50,   0 },
    {   29,   0,   0,   0,   0,   0,  34,   0,   0,  28,   0,   0,  42,  48,   0 },
    {   34,   0,   0,  34,  33,  56,   0,   0,   0,  43,  38,  38,  50,   0,   0 },
    {   29,   0,   0,  22,  27,  27,  28,   0,   0,  38,   0,   0,  48,   0,   0 },
    {   32,   0,   0,  17,   0,   0,  24,   0,   0,  20,  22,  22,   0,   0,  30 },
    {   28,   0,   0,  34,  33,  56,  34,   0,   0,  30,  38,  38,   0,   0,  24 },
    {   34,   0,   0,   9,  51,  17,  37,   0,  22,   1,  16,   4,  50,  55,  30 },
    {    0,   0,   0,   0,  54,  14,  35,   0,  24,  54,  26,  46,  45,  50,  24 },
};

static short quantizedDistinctValues[QUANTIZED_DISTINCT_VALUE_COUNT] =
{
    0, 1, 160, 400, 500, 700, 120, 10, 30, 20, 10, 2, 
    2, 1, 3, 2, 1, 1, 2, 1, 1, 2, 2, 4, 
    6, 2, 4, 8, 6, 20, 20, 10, 10, 10, 10, 20, 
    20, 20, 20, 40, 20, 120, 20, 100, 100, 40, 40, 80, 
    40, 200, 200, 200, 200, 40, 160, 20, 180, 1599, 
};

static float getSuperShapeValue(int row, int col)
{
    return superShapes_opt[row][col];
}

static void prepareSuperShapeArray()
{
    int a, b;
    for (a = 1; a < QUANTIZED_DISTINCT_VALUE_COUNT; ++a)
        quantizedDistinctValues[a] += quantizedDistinctValues[a - 1];
    for (a = 0; a < SUPERSHAPE_COUNT; ++a)
    {
        for (b = 0; b < SUPERSHAPE_PARAMS; ++b)
        {
            unsigned char index = superShapes_opt_indices[a][b];
            superShapes_opt[a][b] = quantizedDistinctValues[index] *
                                    QUANTIZED_STEP + QUANTIZED_MIN_VALUE;
            if (a == 0)
                continue;
            if (!index)
                superShapes_opt[a][b] = superShapes_opt[a - 1][b];
        }
    }
}


#endif // !SHAPES_OPT_H_INCLUDED
