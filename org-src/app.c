/* $Id: app.c,v 1.15 2004/06/13 10:31:38 tonic Exp $
 * $Revision: 1.15 $
 */

#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include "app.h"
#include "synth.h"
#include "importgl.h"

#ifndef _DEBUG
unsigned int _fltused = 1;
#endif


int alive = 1;


#ifdef _DEBUG
int WINAPI WinMain(HINSTANCE instance, HINSTANCE prevInstance, LPSTR cmdLine, int cmdShow)
#else
int WinMainCRTStartup()
#endif
{
    PIXELFORMATDESCRIPTOR pfd;
    HWND hwnd;
    HDC hdc;
    int pixelFormat;
#ifdef FULLSCREEN
    DEVMODE deviceMode;
#endif
#ifndef _DEBUG
    HINSTANCE instance = GetModuleHandle(NULL);
#endif

#ifndef SAFE_BUILD
    //grabWinFuncs();
    grabGLFuncs();
#endif

#ifdef FULLSCREEN
    deviceMode.dmSize = sizeof(deviceMode);
    deviceMode.dmPelsWidth = WIDTH;
    deviceMode.dmPelsHeight = HEIGHT;
    deviceMode.dmBitsPerPel = 32;
    deviceMode.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

    //if (ChangeDisplaySettings(&deviceMode, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
    //    return 0;
    // if change doesn't succeed, continue without fullscreen (ignore return value)
    ChangeDisplaySettings(&deviceMode, CDS_FULLSCREEN);
#endif

    hwnd = CreateWindowEx(
#ifdef _DEBUG
        WS_EX_APPWINDOW,
#else
        WS_EX_TOPMOST,
#endif
        "Static" /*"Edit"*/, "",
#ifdef _DEBUG
        WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_VISIBLE | WS_DLGFRAME,
#else
        WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_VISIBLE, //| WS_DLGFRAME,
#endif
        0, 0, WIDTH, HEIGHT, NULL, NULL, instance, NULL); 

    /* init the pixelformat descriptor */ 
    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;
    pfd.cDepthBits = 16;
    // these are probably not necessary to reset, disabled to save some space
    //pfd.cAlphaBits = pfd.cAccumBits = pfd.cStencilBits = pfd.cAuxBuffers = 0;
    pfd.iLayerType = PFD_MAIN_PLANE;

    /* select the pixel format for the current window */ 
    hdc = GetDC(hwnd);
    pixelFormat = ChoosePixelFormat(hdc, &pfd);
    SetPixelFormat(hdc, pixelFormat, &pfd);

    wglMakeCurrent(hdc, wglCreateContext(hdc));

    ShowCursor(0);

#if defined(_DEBUG) || defined(PREVIEW_BUILD)
    {
        //GLYPHMETRICSFLOAT agmf[256]; 
        //wglUseFontOutlines(hdc, 0, 255, 1000, 0, 0.1f, WGL_FONT_POLYGONS, agmf);
        SelectObject(hdc, GetStockObject(OEM_FIXED_FONT));
        wglUseFontBitmaps(hdc, 0, 255, 1000);
    }
#endif // _DEBUG

#ifndef MINIMAL
    {
        int a;
        // clear the display a few times
        glClearColor(0, 0, 0, 1);
        for (a = 0; a < 4; ++a)
        {
            glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
            SwapBuffers(hdc);
        }
    }
#endif


#ifndef NO_SYNTH
    initSoundSystem();
#endif

#ifndef NO_INTRO_START_DELAY
    Sleep(2000);
#endif // !NO_INTRO_START_DELAY

    app_init();

#ifndef NO_SYNTH
    startSound();
#endif

    // main loop
    while (!GetAsyncKeyState(VK_ESCAPE) && alive)
    {
#if defined(_DEBUG)
        MSG message;
        // pump win messages
        while (PeekMessage(&message, hwnd, 0, 0, PM_REMOVE))
        {
            if (message.message == WM_KEYDOWN)
                app_keyDown((int)message.wParam);
            if (message.message == WM_KEYUP)
                app_keyUp((int)message.wParam);
            TranslateMessage(&message);
            DispatchMessage(&message);
        }
#elif !defined(NO_MESSAGE_PUMP)
        MSG message;
        while (PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
            DispatchMessage(&message);
#endif // !_DEBUG && !NO_MESSAGE_PUMP


        app_render();

#ifdef _DEBUG
        // check for GL errors
        {
            GLenum error = glGetError();
            if (error != GL_NO_ERROR)
            {
                MessageBox(NULL, gluErrorString(error), "GL error", MB_OK);
            }
        }
#endif

        SwapBuffers(hdc);
#ifndef NO_SYNTH
        updateSound();
#endif
#if defined(_DEBUG) /*|| !defined(MINIMAL)*/
        Sleep(1);
#endif
    }

    ChangeDisplaySettings(NULL, 0);

    ExitProcess(0);

    return 0;
}
