/* $Id: importgl.c,v 1.14 2005/01/22 15:19:33 tonic Exp $
 * $Revision: 1.14 $
 */

#ifndef SAFE_BUILD

#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#include <windows.h>

#include <stdio.h>

#include "importgl.h"


void *importedGLFunctions[GLFI_COUNT];
void *importedGLUFunctions[GLUFI_COUNT];
void *importedWinUSER32Functions[WUI_COUNT];
void *importedWinWINMMFunctions[WMMI_COUNT];

/* Strange bug or most probably a coding mistake:
 *   For some reason the second fetched symbol in the arrays doesn't
 *   seem to work, so we fetch two unneeded symbols at top of the
 *   array first..
 */
/*
static unsigned char importGLOrdinalOffsets[] =
{
      1      , // GlmfBeginGlsBlock
      2 -   1, // GlmfCloseMetaFile
     11 -   2, // glBegin
     14 -  11, // glBlendFunc
     15 -  14, // glCallList
     17 -  15, // glClear
     19 -  17, // glClearColor
     28 -  19, // glColor3f
     //29 -  19, // glColor3fv
     35 -  28, // glColor3ubv
     71 -  35, // glDisable
     80 -  71, // glEnable
     82 -  80, // glEnd
     83 -  82, // glEndList
    159 -  83, // glLightfv
    165 - 159, // glLoadIdentity
    178 - 165, // glMaterialf
    179 - 178, // glMaterialfv
    182 - 179, // glMatrixMode
    185 - 182, // glNewList
    //190 - 185, // glNormal3f
    191 - 185, // glNormal3f
    213 - 191, // glPopMatrix
    218 - 213, // glPushMatrix
    256 - 218, // glRotatef
    258 - 256, // glScalef
    261 - 258, // glShadeModel
    317 - 261, // glTranslatef
    //320 - 317, // glVertex2f
    321 - 317, // glVertex2fv
    //328 - 317, // glVertex3f
    329 - 321, // glVertex3fv
    //343 - 328, // glViewport
    346 - 329, // wglCreateContext
    357 - 346, // wglMakeCurrent
    #ifdef _DEBUG // Fetched by name
    0, // GLFI_glGetError
    #endif
};

static unsigned char importGLUOrdinalOffsets[] =
{
    22     , // gluLookAt
    33 - 22, // gluPerspective
    //#ifdef _DEBUG // Fetched by name
    //0, // gluErrorString
    //#endif
};
*/

/*
static HMODULE fetchFunctionsByOrdinals(const char *fileName, int count,
                                        unsigned char *ordinalOffsets,
                                        void **functions)
{
    int a, ordinal;
    HMODULE lib;
    lib = LoadLibrary(fileName);
    ordinal = 0;
    for (a = 0; a < count; ++a)
    {
        ordinal += ordinalOffsets[a];
        functions[a] = GetProcAddress(lib, (LPCSTR)ordinal);
    }
    return lib;
}
*/

static void fetchFunctions(const char *fileName, int count, void **functions)
{
    int a;
    HMODULE lib;
    lib = LoadLibrary(fileName);
    for (a = 0; a < count; ++a)
        functions[a] = GetProcAddress(lib, (LPCSTR)a);
#ifdef _DEBUG
    FreeLibrary(lib);
#endif
}

void grabGLFuncs()
{
    fetchFunctions("opengl32.dll", GLFI_COUNT, importedGLFunctions);
    fetchFunctions("glu32.dll", GLUFI_COUNT, importedGLUFunctions);
    fetchFunctions("user32.dll", WUI_COUNT, importedWinUSER32Functions);
    fetchFunctions("winmm.dll", WMMI_COUNT, importedWinWINMMFunctions);
}


#if 0
void grabGLFuncs()
{
    int a;
    //int ordinal;
    HMODULE lib;

    /*
#ifdef _DEBUG
    const importGLOrdinalOffsetsSize = sizeof(importGLOrdinalOffsets) /
                                       sizeof(importGLOrdinalOffsets[0]);
    const importGLUOrdinalOffsetsSize = sizeof(importGLUOrdinalOffsets) /
                                        sizeof(importGLUOrdinalOffsets[0]);
    if (importGLOrdinalOffsetsSize != GLFI_COUNT)
        MessageBox(NULL, "GL ordinal offset array size doesn't match with GLFI_COUNT!",
                   "grabGLFuncs Debug Error", MB_OK);
    if (importGLUOrdinalOffsetsSize != GLUFI_COUNT)
        MessageBox(NULL, "GLU ordinal offset array size doesn't match with GLUFI_COUNT!",
                   "grabGLFuncs Debug Error", MB_OK);
#endif
    */

    lib = LoadLibrary("OPENGL32.DLL");
    //ordinal = 0;
    for (a = 0; a < GLFI_COUNT; ++a)
    {
        //ordinal += importGLOrdinalOffsets[a];
        //importedGLFunctions[a] = GetProcAddress(lib, (LPCSTR)ordinal);
        importedGLFunctions[a] = GetProcAddress(lib, (LPCSTR)a);
    }
#ifdef _DEBUG
    importedGLFunctions[GLFI_glGetError] = GetProcAddress(lib, "glGetError");
    FreeLibrary(lib);
#endif

    lib = LoadLibrary("GLU32.DLL");
    //ordinal = 0;
    for (a = 0; a < GLUFI_COUNT; ++a)
    {
        //ordinal += importGLUOrdinalOffsets[a];
        //importedGLUFunctions[a] = GetProcAddress(lib, (LPCSTR)ordinal);
        importedGLUFunctions[a] = GetProcAddress(lib, (LPCSTR)a);
    }
#ifdef _DEBUG
//    importedGLFunctions[GLUFI_gluErrorString] = GetProcAddress(lib, "gluErrorString");
    FreeLibrary(lib);
#endif


    lib = LoadLibrary("USER32.DLL");
    for (a = 0; a < WUI_COUNT; ++a)
        importedWinUSER32Functions[a] = GetProcAddress(lib, (LPCSTR)a);
#ifdef _DEBUG
    FreeLibrary(lib);
#endif

    /*
    lib = LoadLibrary("WINMM.DLL");
    for (a = 0; a < WMMI_COUNT; ++a)
        importedWinWINMMFunctions[a] = GetProcAddress(lib, (LPCSTR)a);
#ifdef _DEBUG
    FreeLibrary(lib);
#endif
    */


    /*
    HMODULE lib;

    lib = fetchFunctionsByOrdinals("OPENGL32.DLL", GLFI_COUNT, importGLOrdinalOffsets, importedGLFunctions);
#ifdef _DEBUG
    importedGLFunctions[GLFI_glGetError] = GetProcAddress(lib, "glGetError");
#endif
    //FreeLibrary(lib);

    lib = fetchFunctionsByOrdinals("GLU32.DLL", GLUFI_COUNT, importGLUOrdinalOffsets, importedGLUFunctions);
    //FreeLibrary(lib);
    */
}
#endif // 0


#endif // !SAFE_BUILD
