/* $Id: importgl.h,v 1.13 2004/06/11 18:37:59 tonic Exp $
 * $Revision: 1.13 $
 */

#ifndef SAFE_BUILD

#ifndef IMPORTGL_H_INCLUDED
#define IMPORTGL_H_INCLUDED


// Note: When updating function array definition, remember
//       to keep ordinal definition array synched in importgl.c.

/*
enum IMPORT_GL_FUNCTION_INDEX
{
    GLFI_GlmfBeginGlsBlock = 0,
    GLFI_GlmfCloseMetaFile,
    GLFI_glBegin,
    GLFI_glBlendFunc,
    GLFI_glCallList,
    GLFI_glClear,
    GLFI_glClearColor,
    GLFI_glColor3f,
    //GLFI_glColor3fv,
    GLFI_glColor3ubv,
    GLFI_glDisable,
    GLFI_glEnable,
    GLFI_glEnd,
    GLFI_glEndList,
    GLFI_glLightfv,
    GLFI_glLoadIdentity,
    GLFI_glMaterialf,
    GLFI_glMaterialfv,
    GLFI_glMatrixMode,
    GLFI_glNewList,
    //GLFI_glNormal3f,
    GLFI_glNormal3fv,
    GLFI_glPopMatrix,
    GLFI_glPushMatrix,
    GLFI_glRotatef,
    GLFI_glScalef,
    GLFI_glShadeModel,
    GLFI_glTranslatef,
    //GLFI_glVertex2f,
    GLFI_glVertex2fv,
    //GLFI_glVertex3f,
    GLFI_glVertex3fv,
    //GLFI_glViewport,
    GLFI_wglCreateContext,
    GLFI_wglMakeCurrent,
    #ifdef _DEBUG
    GLFI_glGetError,
    #endif
    GLFI_COUNT
};
*/
enum IMPORT_GL_FUNCTION_INDEX
{
    GLFI_glBegin = 11,
    GLFI_glBlendFunc = 14,
    GLFI_glCallList = 15,
    GLFI_glClear = 17,
    GLFI_glClearColor = 19,
    GLFI_glColor3f = 28,
    //GLFI_glColor3fv,
    GLFI_glColor3ubv = 35,
    GLFI_glDisable = 71,
    GLFI_glEnable = 80,
    GLFI_glEnd = 82,
    GLFI_glEndList = 83,
    GLFI_glLightfv = 159,
    GLFI_glLoadIdentity = 165,
    GLFI_glMaterialf = 178,
    GLFI_glMaterialfv = 179,
    GLFI_glMatrixMode = 182,
    GLFI_glNewList = 185,
    //GLFI_glNormal3f,
    GLFI_glNormal3fv = 191,
    GLFI_glPopMatrix = 213,
    GLFI_glPushMatrix = 218,
    GLFI_glRotatef = 256,
    GLFI_glScalef = 258,
    GLFI_glShadeModel = 261,
    GLFI_glTranslatef = 317,
    //GLFI_glVertex2f,
    GLFI_glVertex2fv = 321,
    GLFI_glVertex2i = 322,
    GLFI_glVertex2iv = 323,
    //GLFI_glVertex3f,
    GLFI_glVertex3fv = 329,
    //GLFI_glViewport,
    GLFI_wglCreateContext = 346,
    GLFI_wglMakeCurrent = 357,
    #ifdef _DEBUG
    GLFI_glGetError = 110,
    #endif
    //GLFI_COUNT = 368
    GLFI_COUNT = 358
};


/*
enum IMPORT_GLU_FUNCTION_INDEX
{
    GLUFI_gluLookAt = 0,
    GLUFI_gluPerspective,
    //#ifdef _DEBUG
    //GLUFI_gluErrorString,
    //#endif
    GLUFI_COUNT
};
*/
enum IMPORT_GLU_FUNCTION_INDEX
{
    GLUFI_gluLookAt = 22,
    GLUFI_gluPerspective = 33,
    //#ifdef _DEBUG
    //GLUFI_gluErrorString,
    //#endif
    //GLUFI_COUNT = 52
    GLUFI_COUNT = 34
};

enum IMPORT_WIN_USER32_INDEX
{
    WUI_ChangeDisplaySettingsA = 33,
    WUI_CreateWindowExA = 97,
    WUI_GetAsyncKeyState = 243,
    WUI_GetDC = 269,
    WUI_ShowCursor = 655,
    //WUI_COUNT = 732,
    WUI_COUNT = 656
};

enum IMPORT_WIN_WINMM_INDEX
{
    WMMI_waveOutGetPosition = 195,
    WMMI_waveOutOpen = 198,
    WMMI_waveOutPrepareHeader = 200,
    WMMI_waveOutWrite = 207,
    //WMMI_COUNT = 211
    WMMI_COUNT = 208
};

extern void *importedGLFunctions[GLFI_COUNT];
extern void *importedGLUFunctions[GLUFI_COUNT];
extern void *importedWinUSER32Functions[WUI_COUNT];
extern void *importedWinWINMMFunctions[WMMI_COUNT];

#define GLFI_FUNCTION(func, proto) ((proto)importedGLFunctions[GLFI_##func])
#define GLUFI_FUNCTION(func, proto) ((proto)importedGLUFunctions[GLUFI_##func])
#define WUI_FUNCTION(func, proto) ((proto)importedWinUSER32Functions[WUI_##func])
#define WMMI_FUNCTION(func, proto) ((proto)importedWinWINMMFunctions[WMMI_##func])

#define glBegin GLFI_FUNCTION(glBegin, void (APIENTRY *)(GLenum mode))
#define glBlendFunc GLFI_FUNCTION(glBlendFunc, void (APIENTRY *)(GLenum sfactor, GLenum dfactor))
#define glCallList GLFI_FUNCTION(glCallList, void (APIENTRY *)(GLuint list))
#define glClear GLFI_FUNCTION(glClear, void (APIENTRY *)(GLbitfield mask))
#define glClearColor GLFI_FUNCTION(glClearColor, void (APIENTRY *)(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha))
#define glColor3f GLFI_FUNCTION(glColor3f, void (APIENTRY *)(GLfloat red, GLfloat green, GLfloat blue))
//#define glColor3fv GLFI_FUNCTION(glColor3fv, void (APIENTRY *)(const GLfloat *v))
#define glColor3ubv GLFI_FUNCTION(glColor3ubv, void (APIENTRY *)(const GLubyte *v))
#define glDisable GLFI_FUNCTION(glDisable, void (APIENTRY *)(GLenum cap))
#define glEnable GLFI_FUNCTION(glEnable, void (APIENTRY *)(GLenum cap))
#define glEnd GLFI_FUNCTION(glEnd, void (APIENTRY *)(void))
#define glEndList GLFI_FUNCTION(glEndList, void (APIENTRY *)(void))
#define glLightfv GLFI_FUNCTION(glLightfv, void (APIENTRY *)(GLenum light, GLenum pname, const GLfloat *params))
#define glLoadIdentity GLFI_FUNCTION(glLoadIdentity, void (APIENTRY *)(void))
#define glMaterialf GLFI_FUNCTION(glMaterialf, void (APIENTRY *)(GLenum face, GLenum pname, GLfloat param))
#define glMaterialfv GLFI_FUNCTION(glMaterialfv, void (APIENTRY *)(GLenum face, GLenum pname, const GLfloat *params))
#define glMatrixMode GLFI_FUNCTION(glMatrixMode, void (APIENTRY *)(GLenum mode))
#define glNewList GLFI_FUNCTION(glNewList, void (APIENTRY *)(GLuint list, GLenum mode))
//#define glNormal3f GLFI_FUNCTION(glNormal3f, void (APIENTRY *)(GLfloat nx, GLfloat ny, GLfloat nz))
#define glNormal3fv GLFI_FUNCTION(glNormal3fv, void (APIENTRY *)(const GLfloat *v))
#define glPopMatrix GLFI_FUNCTION(glPopMatrix, void (APIENTRY *)(void))
#define glPushMatrix GLFI_FUNCTION(glPushMatrix, void (APIENTRY *)(void))
#define glRotatef GLFI_FUNCTION(glRotatef, void (APIENTRY *)(GLfloat angle, GLfloat x, GLfloat y, GLfloat z))
#define glScalef GLFI_FUNCTION(glScalef, void (APIENTRY *)(GLfloat x, GLfloat y, GLfloat z))
#define glShadeModel GLFI_FUNCTION(glShadeModel, void (APIENTRY *)(GLenum mode))
#define glTranslatef GLFI_FUNCTION(glTranslatef, void (APIENTRY *)(GLfloat x, GLfloat y, GLfloat z))
//#define glVertex2f GLFI_FUNCTION(glVertex2f, void (APIENTRY *)(GLfloat x, GLfloat y))
#define glVertex2fv GLFI_FUNCTION(glVertex2fv, void (APIENTRY *)(const GLfloat *v))
#define glVertex2i GLFI_FUNCTION(glVertex2i, void (APIENTRY *)(GLint x, GLint y))
#define glVertex2iv GLFI_FUNCTION(glVertex2iv, void (APIENTRY *)(const GLint *v))
//#define glVertex3f GLFI_FUNCTION(glVertex3f, void (APIENTRY *)(GLfloat x, GLfloat y, GLfloat z))
#define glVertex3fv GLFI_FUNCTION(glVertex3fv, void (APIENTRY *)(const GLfloat *v))
//#define glViewport GLFI_FUNCTION(glViewport, void (APIENTRY *)(GLint x, GLint y, GLsizei width, GLsizei height))
#define wglCreateContext GLFI_FUNCTION(wglCreateContext, HGLRC (WINAPI *)(HDC))
#define wglMakeCurrent GLFI_FUNCTION(wglMakeCurrent, BOOL (WINAPI *)(HDC, HGLRC))
#ifdef _DEBUG
#define glGetError GLFI_FUNCTION(glGetError, GLenum (APIENTRY *)(void))
#endif

#define gluLookAt GLUFI_FUNCTION(gluLookAt, void (APIENTRY *)(GLdouble eyex, GLdouble eyey, GLdouble eyez, GLdouble centerx, GLdouble centery, GLdouble centerz, GLdouble upx, GLdouble upy, GLdouble upz))
#define gluPerspective GLUFI_FUNCTION(gluPerspective, void (APIENTRY *)(GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar))
//#ifdef _DEBUG
//#define gluErrorString GLUFI_FUNCTION(gluErrorString, const GLubyte * (APIENTRY *)(GLenum errCode))
//#endif

#define GetAsyncKeyState WUI_FUNCTION(GetAsyncKeyState, SHORT (WINAPI *)(int vKey))
#define ShowCursor WUI_FUNCTION(ShowCursor, int (WINAPI *)(BOOL bShow))
#define GetDC WUI_FUNCTION(GetDC, HDC (WINAPI *)(HWND hWnd))
#define CreateWindowExA WUI_FUNCTION(CreateWindowExA, HWND (WINAPI *)(DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName, DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam))
#define ChangeDisplaySettingsA WUI_FUNCTION(ChangeDisplaySettingsA, LONG (WINAPI *)(LPDEVMODEA  lpDevMode, DWORD dwFlags))

#ifndef _DEBUG
// these doesn't currently work in unsafe debug build for some reason
#define waveOutGetPosition WMMI_FUNCTION(waveOutGetPosition, MMRESULT (WINAPI *)(HWAVEOUT hwo, LPMMTIME pmmt, UINT cbmmt))
#define waveOutWrite WMMI_FUNCTION(waveOutWrite, MMRESULT (WINAPI *)(HWAVEOUT hwo, LPWAVEHDR pwh, UINT cbwh))
#define waveOutPrepareHeader WMMI_FUNCTION(waveOutPrepareHeader, MMRESULT (WINAPI *)(HWAVEOUT hwo, LPWAVEHDR pwh, UINT cbwh))
#define waveOutOpen WMMI_FUNCTION(waveOutOpen, MMRESULT (WINAPI *)(LPHWAVEOUT phwo, UINT uDeviceID, LPCWAVEFORMATEX pwfx, DWORD dwCallback, DWORD dwInstance, DWORD fdwOpen))
#endif // !_DEBUG

extern void grabGLFuncs();


#endif // !IMPORTGL_H_INCLUDED

#endif // !SAFE_BUILD
