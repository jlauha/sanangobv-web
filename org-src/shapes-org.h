/* $Id: shapes-org.h,v 1.1 2004/06/02 18:51:41 tonic Exp $
 * $Revision: 1.1 $
 */

#ifndef SHAPES_H_INCLUDED
#define SHAPES_H_INCLUDED


// 245 changes v1:
// 2 1 0 5 33 7 8 6 23 14 13 26 25 32 30 11 29 28 31 27 16 19 21 9 10 12 22 18 17 24 3 20 15 4


#pragma warning (disable: 4305)


#define SUPERSHAPE_PARAMS 14

static const float superShapes_org[][SUPERSHAPE_PARAMS] =
{
    // m  a     b     n1     n2     n3     m     a  b     n1     n2      n3   res1 res2

    //{  1, 1,   1,     1,     1,     1,    1,    1, 1,     1,     1,      1,  100, 100 },

    // 5-ulokkeinen lumme, väleistä näkee sisään
    { 10, 1,   2,    90,     1,   -45,    4,    1, 1,  -0.8,    90,   -0.4,   80,  60 },
    // korkea, 5 ylöspäin suipponevaa tolppaa notkahduksella
    { 10, 1,   2,    90,     1,   -45,    8,    1, 1,    -1,     1,   -0.4,   40,  60 },
    // 5 "sarvea" joiden kärjet koskee toisiaan
    { 10, 1,   2,    90,     1,   -45,    4,    1, 1,    10,     1,   -0.4,   40,  40 },
    // iso ja korkea ylöspäin levenevä, pyöreä muutamalla lovella ("tehdas")
    { 10, 1,   2,    60,     1,   -10,    4,    1, 1,    -1,    -2,   -0.4,   82,  82 },
    // iso ja korkea suipponeva, pyöreä mutta kulmainen ("tehdas")
    { 16, 1, 0.2,    60,     1,   -10,    4,    1, 1,    -1,    -1,   -0.4,   62,  62 },
    // lummemainen muodostelma, jossa on monta uloketta, väleistä näkee sisään
    { 20, 1,   2,    90,     1,    -8,    3,    1, 1,    -1,    90,   -0.4,   80,  60 },

    // kolmikulmaiset pyöristetyt ulokkeet ja piikki
    {  6, 1,   1,    60,     1,   -70,    8,    1, 1,   0.4,     3,   0.25,   40,  40 },
    // kukkamaiset kartiot
    {  4, 1,   1,    30,     1,    20,   25,    1, 1,   0.4,     3,   0.25,   10, 100 },
    // suihkulähde
    {  4, 1,   1,    30,     1,    20,   12,    1, 1,   0.4,     3,   0.25,   40,  80 },

    // hedelmämäinen, pyöristetyt sektorit
    {  8, 1,   1,    30,     1,    -4,    8,    2, 1,    -1,     5,    0.5,   60,  60 },
    // monimutkainen epäsymmetrinen poimukas hökötys
    { 13, 1,   1,    30,     1,    -4,   13,    1, 1,     1,     5,      1,   60,  60 },

    // pyöristetyt sektorit joissa on poimuja ("parvekkeet")
    { 10, 1, 1.1,  -0.5,   0.1,    70,   60,    1, 1,   -90,     0,  -0.25,   60, 180 }, // scale up

    // hieman epäsäännöllinen isohko muodostelma, jossa on reikä sisään
    {  9, 1,   1,    40,     1,   -45,    4,    1, 1,  -0.8,    20,   -0.4,   30,  60 }, // scale down
    // paksureunainen kulho ja piikki keskellä
    {  1, 1,   1,    80,     1,    72,    8,    1, 1,   0.5,     3,    0.2,   20,  40 },
    // pienenpieni suippo paasi
    {  9, 1,   1,   -70,  -0.2,    80,    2,    1, 1,   0.4,     4,     -1,    4,  16 },
    // epäsymmetriset pyöristetyt sektorit
    {  7, 1,   1,    20,  -0.3,  -3.5,    6,    1, 1,    -1,   4.5,    0.5,   60,  80 },


    // pyöristetty kuutio
    {  4, 1,   1,    10,    10,    10,    4,    1, 1,    10,    10,     10,   20,  40 },
    // pieni kulmikas kartio
    {  4, 1,   1,     1,     1,     1,    4,    1, 1,     1,     1,      1,   10,  10 },
    // pieni kartion mallinen
    {  4, 1,   1,     1,     1,     1,    4,    1, 1,     1,     1,      1,   14,  14 },
    // pyöristetty lieriö
    {  1, 1,   1,    38, -0.25,    19,    4,    1, 1,    10,    10,     10,   20,  40 },
    // pystypropellievä
    {  2, 1,   1,   0.7,   0.3,   0.2,    3,    1, 1,   100,   100,    100,   20,  50 },  // -0.126f
    // tähtimuodostelma
    {  7, 1,   1,   0.2,   1.7,   1.7,    7,    1, 1,   0.2,   1.7,    1.7,   60,  80 },  // -0.072f
    // kulmakartio
    {  2, 1,   1,     1,     1,     1,    4,    1, 1,     1,     1,      1,   14,  40 },
    // sateenvarjo
    {  6, 1,   1,     1,     1,     1,    3,    1, 1,     1,     1,      1,   60,  60 },
    // kolmivarjostin
    {  3, 1,   1,     1,     1,     1,    6,    1, 1,     2,     1,      1,   20,  40 },
    // telttakulmio
    {  6, 1,   1,    30,    10,    10,    6,    1, 1,    25,    10,     10,   60,  70 },
    // kolmipropelli
    {  6, 1,   1,   0.3,    50,    -1,    7,    1, 1,   -80,   0.5,    -60,   80,  60 },
    // terävät tuplakulmat kolmiolla
    {  6, 1,   1,     6,   5.5,   100,    6,    1, 1,    25,    10,     10,   60,  40 },
    // 4x tuplakulmat kuvulla
    {  8, 1,   1,    60,   100,    30,    2,    1, 1,    10,    10,     10,   50,  40 },
    // molluska
    {  3, 1,   1,   0.5,   1.7,   1.7,    2,    1, 1,    10,    10,     10,   40,  40 },
    // meritähti
    {  5, 1,   1,   0.1,   1.7,   1.7,    1,    1, 1,   0.3,   0.5,    0.5,   40,  40 },
    // isohko matala allas + reunat + huippu
    {  2, 1,   1,     6,   5.5,   100,    6,    1, 1,     4,    10,     10,   40,  40 },
    // pyöreä kupoli terävillä ulokkeilla ja kartio katolla
    {  6, 1,   1,    -1,    70,   0.1,    9,    1, 0.5, -98,  0.05,    -45,   60,  91 },
    // kupoli kolmella ulokkeella joista yksi on pitkä
    {  6, 1,   1,    -1,    90,  -0.1,    7,    1, 1,    90,   1.3,     34,   32,  60 },


    // archived
    //{10, 1, 0.1f, 90, 0, -45,  4, 1, 1, -0.8, 88, -0.35,  100, 100},  // five leaves with sharp corners

    // torus mappings:
    //{ 6, 1.99f, 0.64f, -0.68f, 22.77f, 0.75f,  3, 1.05f, 0.28f, 15.24f, 0.35f, 49.59f,  100, 100 },
    //{ 4, 0.1f, 0.24f, 0.17f, -0.61f, -0.66f,  5, 1.14f, 1.83f, 8.83f, -0.81f, 87.44f,  40, 10 },
};
#define SUPERSHAPE_COUNT (sizeof(superShapes_org) / sizeof(superShapes_org[0]))


static float getSuperShapeValue(int row, int col)
{
    return superShapes_org[row][col];
}


#endif // !SHAPES_H_INCLUDED
