/* $Id: app.h,v 1.20 2004/06/17 16:22:52 tonic Exp $
 * $Revision: 1.20 $
 */

#ifndef APP_H_INCLUDED
#define APP_H_INCLUDED

//#include <math.h>
#include "minimath.h"


//#define PREVIEW_BUILD

//#define NO_SYNTH

#ifndef _DEBUG
#define FULLSCREEN
#endif
#if !defined(_DEBUG) && !defined(SAFE_BUILD)
#define MINIMAL
#define NO_MESSAGE_PUMP
#endif
#define NO_INTRO_START_DELAY

#define WIDTH   640
#define HEIGHT  480


extern void app_init();
extern void app_render();
#ifdef _DEBUG
extern void app_keyDown(int keyCode);
extern void app_keyUp(int keyCode);
#endif // _DEBUG


extern int alive;


#endif // !APP_H_INCLUDED
