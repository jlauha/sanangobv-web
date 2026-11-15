/* $Id: cams.h,v 1.10 2004/06/19 16:59:24 tonic Exp $
 * $Revision: 1.10 $
 */

#ifndef CAMS_H_INCLUDED
#define CAMS_H_INCLUDED


#define CAMTRACK_LEN    (TEMPO * 2000 * 16 / 44100)

typedef struct
{
    // params: eyeX, eyeY, eyeZ, viewAngle, viewHeightOffs
    short src[5], dest[5];
    unsigned char dist;     // if >0, cam rotates around eye xy on dist * 0.1
    unsigned char len;      // length multiplier
} CAMTRACK;

static CAMTRACK camTracks[] =
{
    // goes by the simple obj
    { { 4500, 2700, 100, 70, -30 }, { 50, 50, -90, -100, 0 }, 20, 1 },

    // red building from inside
    //{ { 1330, 4380, 50, -250, 10 }, { 40, -40, 0, -150, 30 }, 0, 1 },
    { { -1448, 4294, 25, 363, 0 }, { -136, 202, 125, -98, 100 }, 0, 1 },

    // backwards looking at red building and trek fountain
    { { 1437, 4930, 200, -275, -20 }, { 1684, 0, 0, 9, 0 }, 0, 1 },

    // camera rotates inside heap building
    { { 1800, 3609, 200, 0, 675 }, { 0, 0, 0, 300, 0 }, 0, 1 },

    // inside starfish
    { { 923, 996, 10, 2336, -80 }, { 0, 0, 0, 0, 170 }, 0, 1 },

    // inside building with cone
    { { -1663, -43, 600, 2170, 0 }, { 20, 0, -600, 0, 100 }, 0, 1 },

    // camera rotates right
    { { 1049, -1420, 175, 2111, -17 }, { 0, 0, 0, -334, 0 }, 0, 2 },

    // rotating camera around big factory
    { { 0, 0, 50, 300, 25 }, { 0, 0, 0, 300, 0 }, 70, 2 },

    // high up to down at tall thin building and ship crossroads
    { { -473, -953, 3500, -353, -350 }, { 0, 0, -2800, 0, 0 }, 0, 2 },

    // forward near the ground, pass by fountain
    { { 191, 1938, 35, 1139, -17 }, { 1205, -2909, 0, 0, 0 }, 0, 2 },

    // strafe, a ship passes by
    { { -1449, -2700, 150, 0, 0 }, { 0, 2000, 0, 0, 0 }, 0, 2},

    // look down from a bit higher and go forward (looking at shadow side of buildings)
    //{ { 5311, 2790, 650, 1581, -50 }, { -7712, -760, 0, 0, 0 }, 0, 2 },
    //{ { 5273, 4992, 650, 373, -50 }, { -6569, -4389, 0, 0, 0 }, 0, 2 },
    { { 5273, 4992, 650, 373, -50 }, { -4598, -3072, 0, 0, 0 }, 0, 2 },

    // ending view
    //{ { 3223, -3282, 875, -393, -25 }, { 2474, -2475, 0, 0, 0 }, 0, 2 },
    { { 3223, -3282, 875, -393, -25 }, { 1649, -1649, 0, 0, 0 }, 0, 2 },

};
#define CAMTRACK_COUNT (sizeof(camTracks) / sizeof(camTracks[0]))


#endif // !CAMS_H_INCLUDED
