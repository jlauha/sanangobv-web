/* $Id: main.c,v 1.65 2005/05/11 12:35:58 tonic Exp $
 * $Revision: 1.65 $
 */

#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#include <windows.h>
#include <float.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include "app.h"
#include "synth.h"
#include "importgl.h"
#include "shapes-opt.h"
#include "cams.h"

#if defined(_DEBUG) || defined(PREVIEW_BUILD)
#include <stdio.h>
#endif

#define INTRO_LENGTH (20 * CAMTRACK_LEN)

//#define USE_GIANT_DISPLAY_LIST    /* deprecated */
#define GROUND_WOBBLE
#define FLYING_SHIPS

#ifdef _DEBUG
#define MANUAL_CAM
#endif

#pragma warning (disable : 4244)
#pragma warning (disable : 4305)


static int startTick;
static int tick = 0; // for blurring this init is needed..


/*
#ifndef NO_SYNTH
static int order, row, oldOrder = -1;
static float orderStartTick;
#endif // !NO_SYNTH
*/

#ifdef _DEBUG
GLdouble eyeX, eyeY, eyeZ, centerX, centerY, centerZ;
GLdouble viewAngle, viewHeightOffs;
GLdouble custX, custY;
#endif // _DEBUG


#ifdef USE_GIANT_DISPLAY_LIST
#define GIANT_DISPLAY_LIST  1
#define SHAPE_LIST_BASE     2
#else
#define SHAPE_LIST_BASE     1
#endif


void updateState()
{
    /*
    float oldtick;
    unsigned int newtick;

    oldtick = tick;
    newtick = GetTickCount() - startTick;
    tick = (tick * 0.75f) + (newtick * 0.25f);
    */
    //tick = GetTickCount() - startTick;
    tick = (tick + (GetTickCount() - startTick)) >> 1;

    /*
#ifndef NO_SYNTH
    row = songPos % PATTERNLENGTH;
    order = songPos / PATTERNLENGTH;

    if (oldOrder != order)
    {
        oldOrder = order;
        orderStartTick = tick;
    }
#endif // !NO_SYNTH
    */
}


//typedef struct {
//    float x, y, z;
//} VECTOR3;
typedef struct {
    union
    {
        float v[3];
        struct {
            float x, y, z;
        };
    };
} VECTOR3;


static __forceinline void vector3Sub(VECTOR3 *dest, VECTOR3 *v1, VECTOR3 *v2)
{
    int a;
    for (a = 0; a < 3; ++a)
        dest->v[a] = v1->v[a] - v2->v[a];
}


static void superShapeMap(VECTOR3 *point, float r1, float r2, float t, float p)
{
    /*
    r1 = 1 / r1; r2 = 1 / r2;
    // sphere
    point->x = r1 * cos(t) * r2 * cos(p);
    point->y = r1 * sin(t) * r2 * cos(p);
    point->z = r2 * sin(p);
    */
    //r1 = 1 / r1; r2 = 1 / r2;
    // sphere
    point->x = cos(t) * cos(p) / r1 / r2;
    point->y = sin(t) * cos(p) / r1 / r2;
    point->z = sin(p) / r2;
}


static float ssFunc(float t, float *p)
{
    return (pow(pow(fabs(cos(p[0] * t / 4)) / p[1], p[4]) + pow(fabs(sin(p[0] * t / 4)) / p[2], p[5]), 1 / p[3]));
}


#define PI 3.1415926535897932f
// Renders a supershape.
// Based on Paul Bourke's POV-Ray implementation.
// http://astronomy.swin.edu.au/~pbourke/povray/supershape/
/*
static void superShape(float ss1m, //float ss1a,
                       float ss1b, float ss1n1, float ss1n2, float ss1n3,
                       float ss2m, float ss2a, float ss2b,
                       float ss2n1, float ss2n2, float ss2n3,
                       float resol1, float resol2)
*/
static void superShape(float *params)
{
    float i = 0;
    const float resol1 = params[SUPERSHAPE_PARAMS - 3];
    const float resol2 = params[SUPERSHAPE_PARAMS - 2];

    float col[3];
    int temp;
    for (temp = 0; temp < 3; ++temp)
    {
        col[temp] = ((rand() % 155) + 100) / 255.f;
    }
/*
    float r, g, b;
    r = ((rand() % 155) + 100) / 255.f;
    g = ((rand() % 155) + 100) / 255.f;
    b = ((rand() % 155) + 100) / 255.f;
*/
    glBegin(GL_QUADS);

    // longitude -pi to pi
    while (i < resol1)
    {
        //float j = 0;
        float j = resol2 / 4;   // for no mirrored bottom
        float firstj = j;

        // latitude 0 to pi/2 (org. -pi/2 to pi/2)
        while (j < resol2 / 2)
        {
            const float powFixEpsilon = FLT_EPSILON;
            float t1 = powFixEpsilon + -PI + i * 2 * PI / resol1;
            float t2 = powFixEpsilon + -PI + (i + 1) * 2 * PI / resol1;
            float p1 = /*powFixEpsilon +*/ -PI / 2 + j * 2 * PI / resol2;
            float p2 = /*powFixEpsilon +*/ -PI / 2 + (j + 1) * 2 * PI / resol2;
            //int zeros = 0;
            float r0, r1, r2, r3;
            /*
            r0 = SS1(t1);
            //if (r0 == 0) ++zeros;
            r1 = SS2(p1);
            //if (r1 == 0) ++zeros;
            r2 = SS1(t2);
            //if (r2 == 0) ++zeros;
            r3 = SS2(p2);
            //if (r3 == 0) ++zeros;
            //if (zeros == 0)
            */
            //r0 = ssFunc(t1, ss1m, 1, ss1b, ss1n1, ss1n2, ss1n3);
            //r1 = ssFunc(p1, ss2m, ss2a, ss2b, ss2n1, ss2n2, ss2n3);
            //r2 = ssFunc(t2, ss1m, 1, ss1b, ss1n1, ss1n2, ss1n3);
            //r3 = ssFunc(p2, ss2m, ss2a, ss2b, ss2n1, ss2n2, ss2n3);
            r0 = ssFunc(t1, params);
            r1 = ssFunc(p1, &params[6]);
            r2 = ssFunc(t2, params);
            r3 = ssFunc(p2, &params[6]);
            if (r0 && r1 && r2 && r3)
            {
                //VECTOR3 pa, pb, pc, pd;
                VECTOR3 p[4];
#define pa p[0]
#define pb p[1]
#define pc p[2]
#define pd p[3]

                superShapeMap(&pa, r0, r1, t1, p1);
                superShapeMap(&pb, r2, r1, t2, p1);
                superShapeMap(&pc, r2, r3, t2, p2);
                superShapeMap(&pd, r0, r3, t1, p2);

                // kludge to set lower edge of the object to fixed level
                if (j == firstj + 1)
                    pa.z = pb.z = 0;

                {
                    VECTOR3 v1, v2, n;
                    //float lenSq, invLenSq;
                    float ca;//, cb, cc, cd;
                    /*
                    v1.x = pb.x - pa.x;
                    v1.y = pb.y - pa.y;
                    v1.z = pb.z - pa.z;
                    v2.x = pc.x - pa.x;
                    v2.y = pc.y - pa.y;
                    v2.z = pc.z - pa.z;
                    */

                    vector3Sub(&v1, &pb, &pa);
                    vector3Sub(&v2, &pc, &pa);


                    //   i    j    k      i    j
                    // v1.x v1.y v1.z | v1.x v1.y
                    // v2.x v2.y v2.z | v2.x v2.y

                    n.x = v1.y * v2.z - v1.z * v2.y;
                    n.y = v1.z * v2.x - v1.x * v2.z;
                    n.z = v1.x * v2.y - v1.y * v2.x;
                    // auto normalization is enabled
                    /*
                    lenSq = n.x * n.x + n.y * n.y + n.z * n.z;
                    invLenSq = 1 / sqrt(lenSq);
                    n.x *= invLenSq;
                    n.y *= invLenSq;
                    n.z *= invLenSq;
                    */

                    ca = pa.z + 0.5f;
                    /*
                    cb = pb.z + 0.5f;
                    cc = pc.z + 0.5f;
                    cd = pd.z + 0.5f;
                    */

                    //glNormal3f(n.x, n.y, n.z);
                    glNormal3fv(n.v);

//                    glColor3f(ca * r, ca * g, ca * b);
                    glColor3f(ca * col[0], ca * col[1], ca * col[2]);
                    /*
                    glVertex3f(pa.x, pa.y, pa.z);
                    //glColor3f(cb, cb, cb);
                    glVertex3f(pb.x, pb.y, pb.z);
                    //glColor3f(cc, cc, cc);
                    glVertex3f(pc.x, pc.y, pc.z);
                    //glColor3f(cd, cd, cd);
                    glVertex3f(pd.x, pd.y, pd.z);
                    */
                    glVertex3fv(pa.v);
                    glVertex3fv(pb.v);
                    glVertex3fv(pc.v);
                    glVertex3fv(pd.v);
                }

            } // zeros == 0
            ++j;
        }
        ++i;
    }
//#undef SS1
//#undef SS2
    glEnd();
}


void app_init()
{
    int a;

    prepareSuperShapeArray();

    glEnable(GL_NORMALIZE);
    glEnable(GL_DEPTH_TEST);    // default is disabled
    //glDisable(GL_CULL_FACE);  // default is disabled
    glShadeModel(GL_FLAT);      // default is smooth


    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHT2);

    //srand(4);
    //srand(11);
    //srand(14);
    srand(15);

    for (a = 0; a < SUPERSHAPE_COUNT; ++a)
    {
        glNewList(a + SHAPE_LIST_BASE, GL_COMPILE);
        superShape(superShapes_opt[a]);
        glEndList();
    }

    startTick = GetTickCount();

#ifdef _DEBUG
    eyeX = 0;
    eyeY = 5;
    eyeZ = 1;
    viewAngle = 0;
    viewHeightOffs = -0.25f;
    custX = 0;
    custY = 1;
#endif // _DEBUG
}


__forceinline void prepareFrame()
{
    glClearColor(0.1f, 0.2f, 0.3f, 1);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    //glViewport(0, 0, WIDTH, HEIGHT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //gluPerspective(45, (float)WIDTH / HEIGHT, 0.01f, 5);
    //gluPerspective(45, (float)WIDTH / HEIGHT, 0.01f, 100);
    gluPerspective(45, (float)WIDTH / HEIGHT, 0.1f, 150);
    //glOrtho(-4, 4, -3, 3, 0.01f, 100);
    glMatrixMode(GL_MODELVIEW);

    glLoadIdentity();
}


__forceinline void configureLightAndMaterial()
{
    /*
    //static GLfloat lightPosition[] = { -1, 4, 2, 0 };
    //static GLfloat lightPosition[] = { -4, 1, 2, 0 };
    static GLfloat lightPosition[] = { -4, 1, 1, 0 };
    static GLfloat lightPosition2[] = { 1, -2, -1, 0 };
    static GLfloat lightPosition3[] = { -1, 0, -4, 0 };
    //static GLfloat lightAmbient[] = { 0.2f, 0.0f, 0.0f, 1 };
    //static GLfloat lightDiffuse[] = { 0.7f, 0.3f, 0.2f, 1 };
    //static GLfloat lightDiffuse[] = { 1, 0.5f, 0, 1 };
    static GLfloat lightDiffuse[] = { 1, 0.4f, 0, 1 };
    //static GLfloat lightAmbient2[] = { 0.0f, 0.2f, 0.3f, 1 };
    //static GLfloat lightDiffuse2[] = { 0.0f, 0.2f, 0.3f, 1 };
    //static GLfloat lightDiffuse2[] = { 0.05f, 0.1f, 0.2f, 1 };
    static GLfloat lightDiffuse2[] = { 0.05f, 0.1f, 0.25f, 1 };
    static GLfloat lightDiffuse3[] = { 0.05f, 0.12f, 0.1f, 1 };
    static GLfloat zeros[] = { 0, 0, 0, 1 };
    static GLfloat ones[] = { 1, 1, 1, 1 };

    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
    //glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);   // default is 0, 0, 0, 1
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);     // default is 1, 1, 1, 1
    //glLightfv(GL_LIGHT0, GL_SPECULAR, ones);          // default is 1, 1, 1, 1

    glLightfv(GL_LIGHT1, GL_POSITION, lightPosition2);
    //glLightfv(GL_LIGHT1, GL_AMBIENT, lightAmbient2);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, lightDiffuse2);
    //glLightfv(GL_LIGHT1, GL_SPECULAR, zeros);

    glLightfv(GL_LIGHT2, GL_POSITION, lightPosition3);
    //glLightfv(GL_LIGHT1, GL_AMBIENT, lightAmbient2);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, lightDiffuse3);
    //glLightfv(GL_LIGHT1, GL_SPECULAR, zeros);

    //glMaterialfv(GL_FRONT, GL_AMBIENT, zeros);    // default ambient is 0.2, 0.2, 0.2, 1
    //glMaterialfv(GL_FRONT, GL_DIFFUSE, ones);     // default diffuse is 0.8, 0.8, 0.8, 1
    glMaterialfv(GL_FRONT, GL_SPECULAR, ones);      // default specular is 0, 0, 0, 1
    //glMaterialfv(GL_FRONT, GL_EMISSION, zeros);   // default emission is 0, 0, 0, 1
    */

    static GLfloat values[] = { 
        -4, 1, 1, 0, 0.07f,
        0.14f, 0.35f, 1, 1, 1,
        1, -2, -1, 0, -4,
        0, 0.07f, 0.17f, 0.14f, 1,
        0.4f, 0, 1
    };
    glLightfv(GL_LIGHT0, GL_POSITION, &values[0]);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, &values[19]);
    glLightfv(GL_LIGHT1, GL_POSITION, &values[10]);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, &values[4]);
    glLightfv(GL_LIGHT2, GL_POSITION, &values[12]);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, &values[16]);
    glMaterialfv(GL_FRONT, GL_SPECULAR, &values[7]);

    /*
    // buggy test
    static GLfloat values[] = {
        -4, 1, 1, 0,
        1, 0.4f, 0, 1,
        1, -2, -1, 0,
        0.05f, 0.1f, 0.25f, 1,
        -1, 0, -4, 0,
        0.05f, 0.12f, 0.1f, 1
    };
    int a;
    GLfloat *v;
    for (a = 0, v = values; a < 6; ++a, v += 4)
        glLightfv(GL_LIGHT0 + (a >> 1), GL_POSITION - (a & 2), v);
    glMaterialfv(GL_FRONT, GL_SPECULAR, v);      // default specular is 0, 0, 0, 1
    */

    glMaterialf(GL_FRONT, GL_SHININESS, 60); // 32  // default shininess is 0
    //glColorMaterial(GL_FRONT, GL_DIFFUSE);        // default is GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE
    glEnable(GL_COLOR_MATERIAL);
}


//static int curShape = 0;


void drawModels(float zScale)
{
    const int translationScale = 9;
    int x, y;

    srand(9);

    glScalef(1, 1, zScale);

    //glColor3f(1, 1, 1);

    for (y = -5; y <= 5; ++y)
    {
        for (x = -5; x <= 5; ++x)
        {
            float buildingScale;
            //seed *= 16807;
            //seed = seed * 0x343fd + 0x269ec3;

            int curShape = rand() % SUPERSHAPE_COUNT;
            buildingScale = superShapes_opt[curShape][SUPERSHAPE_PARAMS - 1];

            glPushMatrix();
            glTranslatef(x * translationScale, y * translationScale, 0);
            glRotatef(rand(), 0, 0, 1);
            glScalef(buildingScale, buildingScale, buildingScale);

            //superShape(superShapes_opt[curShape]);
            glCallList(curShape + SHAPE_LIST_BASE);
            glPopMatrix();
        }
    }

#ifdef FLYING_SHIPS
    for (x = -2; x <= 2; ++x)
    {
        const int shipScale100 = translationScale * 500;
        const int offs100 = x * shipScale100 + (tick % shipScale100);
        float offs = offs100 * 0.01f;
        glPushMatrix();
        glTranslatef(offs, -4, 2);
        glCallList(SHAPE_LIST_BASE + SUPERSHAPE_COUNT - 1);
        glPopMatrix();
        glPushMatrix();
        glTranslatef(-4, offs, 4);
        glRotatef(90, 0, 0, 1);
        glCallList(SHAPE_LIST_BASE + SUPERSHAPE_COUNT - 1);
        glPopMatrix();
    }
#endif // FLYING_SHIPS
}


#ifdef _DEBUG

void app_keyDown(int keyCode)
{
    /*
    if (keyCode == VK_LEFT)
        curShape = curShape + SUPERSHAPE_COUNT - 1;
    if (keyCode == VK_RIGHT)
        ++curShape;
    */
    const GLdouble step = 0.5;
    switch (keyCode)
    {
        /*
    case VK_LEFT: eyeX += step; break;
    case VK_RIGHT: eyeX -= step; break;
    case VK_UP: eyeY -= step; break;
    case VK_DOWN: eyeY += step; break;
    case VK_PRIOR: eyeZ += step; break;
    case VK_NEXT: eyeZ -= step; break;
    case 'A': centerX += step; break;
    case 'D': centerX -= step; break;
    case 'W': centerY -= step; break;
    case 'S': centerY += step; break;
    case 'R': centerZ += step; break;
    case 'F': centerZ -= step; break;
        */
    case VK_UP: eyeX += cos(viewAngle) * step; eyeY += sin(viewAngle) * step; break;
    case VK_DOWN: eyeX -= cos(viewAngle) * step; eyeY -= sin(viewAngle) * step; break;
    case VK_LEFT: viewAngle += step * (2 * PI) / 32; break;
    case VK_RIGHT: viewAngle -= step * (2 * PI) / 32; break;
    case 'A': eyeX += cos(viewAngle + PI / 2) * step; eyeY += sin(viewAngle + PI / 2) * step; break;
    case 'D': eyeX += cos(viewAngle - PI / 2) * step; eyeY += sin(viewAngle - PI / 2) * step; break;
    case VK_PRIOR: eyeZ += step / 2; break;
    case VK_NEXT: eyeZ -= step / 2; break;
    case 'R': viewHeightOffs += step / 4; break;
    case 'F': viewHeightOffs -= step / 4; break;

    case 'J': custX += step / 5; break;
    case 'L': custX -= step / 5; break;
    case 'I': custY -= step / 5; break;
    case 'K': custY += step / 5; break;
    case 'O': custX = centerX; custY = centerY; break;

    case VK_SPACE:
        {
            char str[1024];
            sprintf(str, "eye: %f %f %f  center: %f %f %f\n",
                    eyeX, eyeY, eyeZ, 
                    centerX, centerY, centerZ);
            OutputDebugString(str);
            sprintf(str, "viewAngle: %f  viewHeightOffs: %f  cust: %f %f\n",
                    viewAngle, viewHeightOffs, custX, custY);
            OutputDebugString(str);
        }
        break;
    }
}

void app_keyUp(int keyCode)
{
}

#endif // _DEBUG


static int currentCamTrack = 0;
static int currentCamTrackStartTick = 0;
static int nextCamTrackStartTick = 0x7fffffff;

__forceinline void camTrack()
{
    float lerp[5];
    //float eX, eY, eZ, cX, cY, cZ;
    float lookAt[6];
#define eX lookAt[0]
#define eY lookAt[1]
#define eZ lookAt[2]
#define cX lookAt[3]
#define cY lookAt[4]
#define cZ lookAt[5]
    float trackPos;
    CAMTRACK *cam;
    int a;

    if (nextCamTrackStartTick <= tick)
    {
        //currentCamTrack = (currentCamTrack + 1) % CAMTRACK_COUNT;
        ++currentCamTrack;
        //currentCamTrackStartTick = tick;
        currentCamTrackStartTick = nextCamTrackStartTick;
    }
    nextCamTrackStartTick = currentCamTrackStartTick + camTracks[currentCamTrack].len * CAMTRACK_LEN;

    cam = &camTracks[currentCamTrack];
    trackPos = (float)(tick - currentCamTrackStartTick) / (CAMTRACK_LEN * cam->len);

    for (a = 0; a < 5; ++a)
        lerp[a] = (cam->src[a] + cam->dest[a] * trackPos) * 0.01f;

    if (cam->dist)
    {
        float dist = cam->dist * 0.1f;
        cX = lerp[0];
        cY = lerp[1];
        cZ = lerp[2];
        eX = cX - cos(lerp[3]) * dist;
        eY = cY - sin(lerp[3]) * dist;
        eZ = cZ - lerp[4];
    }
    else
    {
        eX = lerp[0];
        eY = lerp[1];
        eZ = lerp[2];
        cX = eX + cos(lerp[3]);
        cY = eY + sin(lerp[3]);
        cZ = eZ + lerp[4];
    }
    gluLookAt(eX, eY, eZ, cX, cY, cZ, 0, 0, 1);
#ifdef _DEBUG
    eyeX = eX;
    eyeY = eY;
    eyeZ = eZ;
    centerX = cX;
    centerY = cY;
    centerZ = cZ;
    custX = centerX;
    custY = centerY;
    viewAngle = lerp[3];
    viewHeightOffs = lerp[4];
#endif

#undef eX
#undef eY
#undef eZ
#undef cX
#undef cY
#undef cZ

    /*
    gluLookAt(cos(tick * 0.0001f) * 22, sin(tick * 0.0001f) * 22, 8,
              0, 0, 0,
              0, 0, 1);
    */
}


/* type 0 = ground plane
 * type 1 = fade quad
 */
void drawPlane(char type)
{
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_ZERO, GL_SRC_COLOR);
    glDisable(GL_LIGHTING);

    if (!type)
    {
        // ground plane
        const int scale = 2;
        int x, y;

        glBegin(GL_QUADS);

        //srand(0);
        for (y = -50; y < 50; ++y)
        {
            for (x = -50; x < 50; ++x)
            {
                //const float bl = (rand() & 127) * 0.003f + 0.32f;
                //float bla[3] = { bl, bl, bl };
                //const GLubyte bl = rand() % 97 + 81;
                const GLubyte bl = (rand() & 95) + 81;
                GLubyte bla[3] = { bl, bl, bl };
                //float bl = (x + y) % 2 ? 0.5f : 0.2f;
#ifdef GROUND_WOBBLE
                //#define MAP(x,y) cos(x+tick*0.0005f)*sin(y+tick*0.0005f)
                #define MAP(x,y) cos((x) * 2) * sin((y) * 4) * 0.75f
                int a;
                float f[2];
                //glColor3f(bl, bl, bl);
                //glColor3fv(bla);
                glColor3ubv(bla);
                for (a = 0; a < 4; ++a)
                {
                    int xm = x + ((a & 1) ^ ((a & 2) >> 1));
                    int ym = y + ((a & 2) >> 1);
                    float m = MAP(xm, ym);
                    f[0] = xm * scale + m;
                    f[1] = ym * scale + m;
                    glVertex2fv(f);
                }
#else // or no GROUND_WOBBLE
                glColor3f(bl, bl, bl);
                glVertex2f(x * scale, y * scale);
                glVertex2f((x + 1) * scale, y * scale);
                glVertex2f((x + 1) * scale, (y + 1) * scale);
                glVertex2f(x * scale, (y + 1) * scale);
#endif // !GROUND_WOBBLE
            }
        }
        glEnd();
    }
    else
    {
        // fade quad
        const int beginFade = tick - currentCamTrackStartTick;
        const int endFade = nextCamTrackStartTick - tick;
        const int minFade = beginFade < endFade ? beginFade : endFade;

        if (minFade < 1024)
        {
            const GLubyte fadeIntensity = minFade >> 2;
            const GLubyte fadeCol[3] = { fadeIntensity, fadeIntensity, fadeIntensity };

            // already in modelview
            glLoadIdentity();

            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            //glMatrixMode(GL_MODELVIEW);
            //glLoadIdentity();

            glColor3ubv(fadeCol);
            glBegin(GL_QUADS);
            glVertex2i(-1, -1);
            glVertex2i(1, -1);
            glVertex2i(1, 1);
            glVertex2i(-1, 1);
            glEnd();

            /* this leaves us in projection matrix mode
             * but prepare for next frame will reset the matrix mode
             */
        }
    }

    glEnable(GL_LIGHTING);
    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
}


#ifdef _DEBUG
void world_render()
#else
void app_render()
#endif
{
#ifdef USE_GIANT_DISPLAY_LIST
    static int frame = 0;
#endif
    
    updateState();

    prepareFrame();

    //glTranslatef(0, 0, -5);
    //glRotatef(90, 1, 0, 0);
    //glRotatef(tick * 0.05f, 1, 1, 1);
    /*
    gluLookAt(cos(tick * 0.0001f) * 22, sin(tick * 0.0001f) * 22, 8,
              0, 0, 0,
              0, 0, 1);
    */
#ifndef MANUAL_CAM
    camTrack();
    if (tick >= INTRO_LENGTH)
    {
        alive = 0;
        return;
    }
#else
    centerX = eyeX + cos(viewAngle);
    centerY = eyeY + sin(viewAngle);
    centerZ = eyeZ + viewHeightOffs;
    gluLookAt(eyeX, eyeY, eyeZ,
              centerX, centerY, centerZ,
              0, 0, 1);
#endif // MANUAL_CAM

    configureLightAndMaterial();


#ifdef USE_GIANT_DISPLAY_LIST
    // make one huge display list of the whole world
    if (frame++ == 0)
    { 
        glNewList(GIANT_DISPLAY_LIST, GL_COMPILE);
    } else {
        glCallList(GIANT_DISPLAY_LIST);
        return;
    }
#endif // USE_GIANT_DISPLAY_LIST


    glPushMatrix();
    //glDisable(GL_LIGHT0);
    drawModels(-1);
    //glEnable(GL_LIGHT0);
    glPopMatrix();

    drawPlane(0);

    //glPushMatrix();
    drawModels(1);
    //glPopMatrix();

#ifdef USE_GIANT_DISPLAY_LIST
    glEndList();
#endif

    // fade
    drawPlane(1);


#if defined(PREVIEW_BUILD) && defined(SAFE_BUILD)
    {
        char str[100];
        glColor3f(1, 1, 1);
        glDisable(GL_LIGHTING);
        glDisable(GL_DEPTH_TEST);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glListBase(1000);
        glRasterPos2f(-0.9f, -0.9f);
        sprintf(str, "4k RC3 (safe build)");
        glCallLists(strlen(str), GL_UNSIGNED_BYTE, str);
        glEnable(GL_LIGHTING);
        glListBase(0);
        glEnable(GL_DEPTH_TEST);
    }
#endif
}


#ifdef _DEBUG
void app_render()
{
#ifdef MANUAL_CAM
    int x, y;
#endif
    char str[512];

    world_render();

    glDisable(GL_LIGHTING);

#ifdef MANUAL_CAM
    glColor3f(1, 1, 1);
    glPointSize(2);
    glBegin(GL_POINTS);
    for (y = -50; y < 50; ++y)
    {
        for (x = -50; x < 50; ++x)
            glVertex3f(x, y, 0);
    }
    glEnd();

    glPointSize(2);
    glBegin(GL_POINTS);
    glVertex3d(centerX, centerY, centerZ);
    glEnd();
    glLineWidth(2);
    glBegin(GL_LINES);
    glColor3f(1, 1, 1);
    glVertex3d(custX, custY, 0);
    glVertex3d(custX, custY, 10);
    glEnd();
    glDisable(GL_DEPTH_TEST);
    glBegin(GL_LINES);
    glColor3f(0.5f, 0.5f, 0.5f);
    glVertex3d(custX, custY, -10);
    glVertex3d(custX, custY, 0);
    glEnd();
#endif // MANUAL_CAM

    glColor3f(1, 1, 1);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glListBase(1000);
    glRasterPos2f(-0.98, -0.8);
    sprintf(str, "eye: %f, %f, %f", eyeX, eyeY, eyeZ);
    glCallLists(strlen(str), GL_UNSIGNED_BYTE, str);
    glRasterPos2f(-0.98, -0.85);
    sprintf(str, "center: %f, %f, %f", centerX, centerY, centerZ);
    glCallLists(strlen(str), GL_UNSIGNED_BYTE, str);
    glRasterPos2f(-0.98, -0.9);
    sprintf(str, "viewAngle: %f  viewHeightOffs: %f", viewAngle, viewHeightOffs);
    glCallLists(strlen(str), GL_UNSIGNED_BYTE, str);
    glRasterPos2f(-0.98, -0.95);
    sprintf(str, "cust: %f, %f", custX, custY);
    glCallLists(strlen(str), GL_UNSIGNED_BYTE, str);

    glEnable(GL_LIGHTING);
    glListBase(0);

    glEnable(GL_DEPTH_TEST);
}
#endif // _DEBUG
