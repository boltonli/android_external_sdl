/*
    SDL - Simple DirectMedia Layer
    Copyright (C) 1997-2006 Sam Lantinga

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

    Sam Lantinga
    slouken@libsdl.org
*/

extern "C" {

#include "SDL_config.h"

/* Dummy SDL video driver implementation; this is just enough to make an
 *  SDL-based application THINK it's got a working video driver, for
 *  applications that call SDL_Init(SDL_INIT_VIDEO) when they don't need it,
 *  and also for use as a collection of stubs when porting SDL to a new
 *  platform for which you haven't yet written a valid video driver.
 *
 * This is also a great way to determine bottlenecks: if you think that SDL
 *  is a performance problem for a given platform, enable this driver, and
 *  then see if your application runs faster without video overhead.
 *
 * Initial work by Ryan C. Gordon (icculus@icculus.org). A good portion
 *  of this was cut-and-pasted from Stephane Peter's work in the AAlib
 *  SDL video driver.  Renamed to "DUMMY" by Sam Lantinga.
 */

#include "SDL_video.h"
#include "SDL_mouse.h"
#include "../SDL_sysvideo.h"
#include "../SDL_pixels_c.h"
#include "../../events/SDL_events_c.h"

}

#include "SDL_androidvideo.h"

#include <ui/DisplayInfo.h>
#include <ui/Surface.h>
#include <ui/SurfaceComposerClient.h>

#include <SkBitmap.h>
#include <SkCanvas.h>

#include <jni.h>

static const char* CLASS_PATH = "SDL_androidvideo.cpp";

JavaVM *mVM = NULL;
JNIEnv *mEnv = NULL;

//surface in my surface view
jfieldID FID_surface;

//android native surface
jfieldID FID_Surface_surface;

jmethodID MID_onWindowTitle;

//-- surface java object
jobject surface;

//-- native surface, which is mirror of java class (android do this for us) 
Surface *nativeSurface;

SkBitmap screenBitmap;

static
void setTitle(const char *title);

static
void *createSDLBitmap(int format, int width, int height);

static
void updateScreen(Surface::SurfaceInfo *surface_info);

using namespace android;

extern "C" {

static SDLKey keymap[KEYCODE_LAST+1];

#define ANDROIDVID_DRIVER_NAME "android"

/* Initialization/Query functions */
static int ANDROID_VideoInit(_THIS, SDL_PixelFormat *vformat);
static SDL_Rect **ANDROID_ListModes(_THIS, SDL_PixelFormat *format, Uint32 flags);
static SDL_Surface *ANDROID_SetVideoMode(_THIS, SDL_Surface *current, int width, int height, int bpp, Uint32 flags);
static int ANDROID_SetColors(_THIS, int firstcolor, int ncolors, SDL_Color *colors);
static void ANDROID_VideoQuit(_THIS);

/* Hardware surface functions */
static int ANDROID_AllocHWSurface(_THIS, SDL_Surface *surface);
static int ANDROID_LockHWSurface(_THIS, SDL_Surface *surface);
static void ANDROID_UnlockHWSurface(_THIS, SDL_Surface *surface);
static void ANDROID_FreeHWSurface(_THIS, SDL_Surface *surface);
	
static void ANDROID_SetCaption(_THIS, const char *title, const char *icon);

/* etc. */
static void ANDROID_UpdateRects(_THIS, int numrects, SDL_Rect *rects);

/* ANDROID driver bootstrap functions */
static int ANDROID_Available(void)
{
	//__android_log_print(ANDROID_LOG_INFO, CLASS_PATH, "available");
    return 1;
}

static void ANDROID_DeleteDevice(SDL_VideoDevice *device)
{
	__android_log_print(ANDROID_LOG_INFO, CLASS_PATH, "deleting device");
	free(device->hidden->buffer);
	SDL_free(device->hidden);
    SDL_free(device);
	__android_log_print(ANDROID_LOG_INFO, CLASS_PATH, "device deleted");
}

static SDL_VideoDevice *ANDROID_CreateDevice(int devindex)
{
	//__android_log_print(ANDROID_LOG_INFO, CLASS_PATH, "creating device");
    SDL_VideoDevice *device;

    /* Initialize all variables that we clean on shutdown */
    device = (SDL_VideoDevice *)SDL_malloc(sizeof(SDL_VideoDevice));
    if ( device ) {
        SDL_memset(device, 0, (sizeof *device));
        device->hidden = (struct SDL_PrivateVideoData *) SDL_malloc((sizeof *device->hidden));
    }
    if ( (device == NULL) || (device->hidden == NULL) ) {
        SDL_OutOfMemory();
        if ( device ) {
            SDL_free(device);
        }
        return(0);
    }

    SDL_memset(device->hidden, 0, (sizeof *device->hidden));

    /* Set the function pointers */
    device->VideoInit = ANDROID_VideoInit;
    device->ListModes = ANDROID_ListModes;
    device->SetVideoMode = ANDROID_SetVideoMode;
    device->CreateYUVOverlay = NULL;
    device->SetColors = ANDROID_SetColors;
    device->UpdateRects = ANDROID_UpdateRects;
    device->VideoQuit = ANDROID_VideoQuit;
    device->AllocHWSurface = ANDROID_AllocHWSurface;
    device->CheckHWBlit = NULL;
    device->FillHWRect = NULL;
    device->SetHWColorKey = NULL;
    device->SetHWAlpha = NULL;
    device->LockHWSurface = ANDROID_LockHWSurface;
    device->UnlockHWSurface = ANDROID_UnlockHWSurface;
    device->FlipHWSurface = NULL;
    device->FreeHWSurface = ANDROID_FreeHWSurface;
    device->SetCaption = ANDROID_SetCaption;
    device->SetIcon = NULL;
    device->IconifyWindow = NULL;
    device->GrabInput = NULL;
    device->GetWMInfo = NULL;
    device->InitOSKeymap = ANDROID_InitOSKeymap;
    device->PumpEvents = ANDROID_PumpEvents;

    device->free = ANDROID_DeleteDevice;

	//__android_log_print(ANDROID_LOG_INFO, CLASS_PATH, "device created");
	
    return device;
}

VideoBootStrap ANDROID_bootstrap = {
    ANDROIDVID_DRIVER_NAME, "SDL android video driver",
    ANDROID_Available, ANDROID_CreateDevice
};


int ANDROID_VideoInit(_THIS, SDL_PixelFormat *vformat)
{
	//__android_log_print(ANDROID_LOG_INFO, CLASS_PATH, "initzializing video");

	/* TODO: we only support RGB565 for now */
    vformat->BitsPerPixel = 16;
    vformat->BytesPerPixel = 2;

    self->hidden->bytesPerPixel = vformat->BytesPerPixel;

	//__android_log_print(ANDROID_LOG_INFO, CLASS_PATH, "video initzialized");
    /* We're done! */
    return(0);
}

SDL_Rect **ANDROID_ListModes(_THIS, SDL_PixelFormat *format, Uint32 flags)
{
	__android_log_print(ANDROID_LOG_INFO, CLASS_PATH, "list modes");
     return (SDL_Rect **) -1;
}

SDL_Surface *ANDROID_SetVideoMode(_THIS, SDL_Surface *current, int width, int height, int bpp, Uint32 flags)
{
	//__android_log_print(ANDROID_LOG_INFO, CLASS_PATH, "setting video mode");
	if(self == NULL)
	{
		return NULL;
	}
	
	if(self->hidden->buffer != NULL)
	{
		free(self->hidden->buffer);
	}

    /* Allocate the new pixel format for the screen */
    if (!SDL_ReallocFormat(current, bpp, 0, 0, 0, 0)) {
    	__android_log_print(ANDROID_LOG_ERROR, CLASS_PATH, "couldn't allocate new pixel format for requested mode");
        SDL_SetError("Couldn't allocate new pixel format for requested mode");
        return NULL;
    }
	
	__android_log_print(ANDROID_LOG_INFO, CLASS_PATH, "setting screen size to: %dx%d", width, height);

    /* Set up the new mode framebuffer */
    current->flags = (flags & SDL_FULLSCREEN) | (flags & SDL_DOUBLEBUF);
	self->hidden->flags = flags;
   	self->hidden->w = current->w = width;
   	self->hidden->h = current->h = height;
   	self->hidden->buffer = createSDLBitmap(PIXEL_FORMAT_RGB_565, width, height);
    current->pitch = current->w * (bpp / 8);
    current->pixels = self->hidden->buffer;

    //__android_log_print(ANDROID_LOG_INFO, CLASS_PATH, "video mode setted");
    /* We're done */
    return current;
}

/* We don't actually allow hardware surfaces other than the main one */
static int ANDROID_AllocHWSurface(_THIS, SDL_Surface *surface)
{
	//__android_log_print(ANDROID_LOG_INFO, CLASS_PATH, "ANDROID_AllocHWSurface");
    return(-1);
}
static void ANDROID_FreeHWSurface(_THIS, SDL_Surface *surface)
{
	//__android_log_print(ANDROID_LOG_INFO, CLASS_PATH, "ANDROID_FreeHWSurface");
    return;
}

/* We need to wait for vertical retrace on page flipped displays */
static int ANDROID_LockHWSurface(_THIS, SDL_Surface *surface)
{
	//__android_log_print(ANDROID_LOG_INFO, CLASS_PATH, "ANDROID_LockHWSurface");
    return(0);
}

static void ANDROID_UnlockHWSurface(_THIS, SDL_Surface *surface)
{
	//__android_log_print(ANDROID_LOG_INFO, CLASS_PATH, "ANDROID_UnlockHWSurface");
    return;
}

static
void ANDROID_UpdateRects(_THIS, int numrects, SDL_Rect *rects)
{
	//__android_log_print(ANDROID_LOG_INFO, CLASS_PATH, "updating rects");
	
	if(self->hidden->buffer == NULL)
	{
		return;
	}
	
	Surface::SurfaceInfo surf_info;
	if (!nativeSurface || !nativeSurface->isValid()) {
		__android_log_print(ANDROID_LOG_INFO, CLASS_PATH,
				"nativeSurface wasn't valid");
		return;
	}
	
	if (nativeSurface->lock(&surf_info) < 0)
	{
		__android_log_print(ANDROID_LOG_INFO, CLASS_PATH,
							"Failed to lock surface");
		SDL_SetError("Failed to lock surface");
		return;
	}

	//-- update android Surface pixels
	updateScreen(&surf_info);

	if (nativeSurface->unlockAndPost() < 0) {
		__android_log_print(ANDROID_LOG_INFO, CLASS_PATH,
							"Failed to unlock surface");
		SDL_SetError("Failed to unlock surface");
		return;
	}

	//__android_log_print(ANDROID_LOG_INFO, CLASS_PATH, "recs updated");
}

void ANDROID_PumpEvents(_THIS)
{
	//__android_log_print(ANDROID_LOG_INFO, CLASS_PATH, "ANDROID_PumpEvents");
}

static
void ANDROID_SetCaption(_THIS, const char *title, const char *icon)
{
	setTitle(title);
}

void ANDROID_InitOSKeymap(_THIS)
{
	//__android_log_print(ANDROID_LOG_INFO, CLASS_PATH, "initzializing key map");
	int i;

	/* Initialize the DirectFB key translation table */
	for (i = 0; i < SDL_arraysize(keymap); ++i)
		keymap[i] = SDLK_UNKNOWN;

	keymap[KEYCODE_UNKNOWN] = SDLK_UNKNOWN;

	keymap[KEYCODE_MENU] = SDLK_ESCAPE;

	keymap[KEYCODE_CALL] = SDLK_F1;
	keymap[KEYCODE_ENDCALL] = SDLK_F2;
	keymap[KEYCODE_CAMERA] = SDLK_F3;
	keymap[KEYCODE_POWER] = SDLK_F4;

	keymap[KEYCODE_BACK] = SDLK_ESCAPE; // Note: generates SDL_QUIT
	keymap[KEYCODE_0] = SDLK_0;
	keymap[KEYCODE_1] = SDLK_1;
	keymap[KEYCODE_2] = SDLK_2;
	keymap[KEYCODE_3] = SDLK_3;
	keymap[KEYCODE_4] = SDLK_4;
	keymap[KEYCODE_5] = SDLK_5;
	keymap[KEYCODE_6] = SDLK_6;
	keymap[KEYCODE_7] = SDLK_7;
	keymap[KEYCODE_8] = SDLK_8;
	keymap[KEYCODE_9] = SDLK_9;
	keymap[KEYCODE_STAR] = SDLK_ASTERISK;
	keymap[KEYCODE_POUND] = SDLK_DOLLAR;

	keymap[KEYCODE_DPAD_UP] = SDLK_UP;
	keymap[KEYCODE_DPAD_DOWN] = SDLK_DOWN;
	keymap[KEYCODE_DPAD_LEFT] = SDLK_LEFT;
	keymap[KEYCODE_DPAD_RIGHT] = SDLK_RIGHT;
	keymap[KEYCODE_DPAD_CENTER] = SDLK_RETURN;

	keymap[KEYCODE_SOFT_LEFT] = SDLK_KP4;
	keymap[KEYCODE_SOFT_RIGHT] = SDLK_KP6;
	keymap[KEYCODE_ENTER] = SDLK_KP_ENTER;

	keymap[KEYCODE_VOLUME_UP] = SDLK_PAGEUP;
	keymap[KEYCODE_VOLUME_DOWN] = SDLK_PAGEDOWN;
	keymap[KEYCODE_SEARCH] = SDLK_END;
	keymap[KEYCODE_HOME] = SDLK_HOME;

	keymap[KEYCODE_CLEAR] = SDLK_CLEAR;
	keymap[KEYCODE_A] = SDLK_a;
	keymap[KEYCODE_B] = SDLK_b;
	keymap[KEYCODE_C] = SDLK_c;
	keymap[KEYCODE_D] = SDLK_d;
	keymap[KEYCODE_E] = SDLK_e;
	keymap[KEYCODE_F] = SDLK_f;
	keymap[KEYCODE_G] = SDLK_g;
	keymap[KEYCODE_H] = SDLK_h;
	keymap[KEYCODE_I] = SDLK_i;
	keymap[KEYCODE_J] = SDLK_j;
	keymap[KEYCODE_K] = SDLK_k;
	keymap[KEYCODE_L] = SDLK_l;
	keymap[KEYCODE_M] = SDLK_m;
	keymap[KEYCODE_N] = SDLK_n;
	keymap[KEYCODE_O] = SDLK_o;
	keymap[KEYCODE_P] = SDLK_p;
	keymap[KEYCODE_Q] = SDLK_q;
	keymap[KEYCODE_R] = SDLK_r;
	keymap[KEYCODE_S] = SDLK_s;
	keymap[KEYCODE_T] = SDLK_t;
	keymap[KEYCODE_U] = SDLK_u;
	keymap[KEYCODE_V] = SDLK_v;
	keymap[KEYCODE_W] = SDLK_w;
	keymap[KEYCODE_X] = SDLK_x;
	keymap[KEYCODE_Y] = SDLK_y;
	keymap[KEYCODE_Z] = SDLK_z;
	keymap[KEYCODE_COMMA] = SDLK_COMMA;
	keymap[KEYCODE_PERIOD] = SDLK_PERIOD;
	keymap[KEYCODE_TAB] = SDLK_TAB;
	keymap[KEYCODE_SPACE] = SDLK_SPACE;
	keymap[KEYCODE_DEL] = SDLK_DELETE;
	keymap[KEYCODE_GRAVE] = SDLK_BACKQUOTE;
	keymap[KEYCODE_MINUS] = SDLK_MINUS;
	keymap[KEYCODE_EQUALS] = SDLK_EQUALS;
	keymap[KEYCODE_LEFT_BRACKET] = SDLK_LEFTBRACKET;
	keymap[KEYCODE_RIGHT_BRACKET] = SDLK_RIGHTBRACKET;
	keymap[KEYCODE_BACKSLASH] = SDLK_BACKSLASH;
	keymap[KEYCODE_SEMICOLON] = SDLK_SEMICOLON;
	keymap[KEYCODE_APOSTROPHE] = SDLK_QUOTE;
	keymap[KEYCODE_SLASH] = SDLK_SLASH;
	keymap[KEYCODE_AT] = SDLK_AT;

	keymap[KEYCODE_PLUS] = SDLK_PLUS;

	/*
	 keymap[KEYCODE_SYM] = SDLK_SYM;
	 keymap[KEYCODE_NUM] = SDLK_NUM;

	 keymap[KEYCODE_SOFT_LEFT] = SDLK_SOFT_LEFT;
	 keymap[KEYCODE_SOFT_RIGHT] = SDLK_SOFT_RIGHT;

	 keymap[KEYCODE_ALT_LEFT] = SDLK_ALT_LEFT;
	 keymap[KEYCODE_ALT_RIGHT] = SDLK_ALT_RIGHT;
	 keymap[KEYCODE_SHIFT_LEFT] = SDLK_SHIFT_LEFT;
	 keymap[KEYCODE_SHIFT_RIGHT] = SDLK_SHIFT_RIGHT;

	 keymap[KEYCODE_EXPLORER] = SDLK_EXPLORER;
	 keymap[KEYCODE_ENVELOPE] = SDLK_ENVELOPE;
	 keymap[KEYCODE_HEADSETHOOK] = SDLK_HEADSETHOOK;
	 keymap[KEYCODE_FOCUS] = SDLK_FOCUS;
	 keymap[KEYCODE_NOTIFICATION] = SDLK_NOTIFICATION;
	 keymap[KEYCODE_MEDIA_PLAY_PAUSE=] = SDLK_MEDIA_PLAY_PAUSE=;
	 keymap[KEYCODE_MEDIA_STOP] = SDLK_MEDIA_STOP;
	 keymap[KEYCODE_MEDIA_NEXT] = SDLK_MEDIA_NEXT;
	 keymap[KEYCODE_MEDIA_PREVIOUS] = SDLK_MEDIA_PREVIOUS;
	 keymap[KEYCODE_MEDIA_REWIND] = SDLK_MEDIA_REWIND;
	 keymap[KEYCODE_MEDIA_FAST_FORWARD] = SDLK_MEDIA_FAST_FORWARD;
	 keymap[KEYCODE_MUTE] = SDLK_MUTE;
	 */

	//__android_log_print(ANDROID_LOG_INFO, CLASS_PATH, "key map initzialized");
}

int ANDROID_SetColors(_THIS, int firstcolor, int ncolors, SDL_Color *colors)
{
    /* do nothing of note. */
    return(1);
}

/* Note:  If we are terminated, this could be called in the middle of
   another SDL video routine -- notably UpdateRects.
*/
void ANDROID_VideoQuit(_THIS)
{
#if 0
    if (self->screen->pixels != NULL)
    {
        SDL_free(self->screen->pixels);
        self->screen->pixels = NULL;
    }
#endif
}

}//end extern C

/* Called by SDL for setting window title string */
static
void setTitle(const char *title)
{
	/*
	__android_log_print(ANDROID_LOG_INFO, CLASS_PATH, "setting window title");
	
	JNIEnv *env;
	
	if (mVM->GetEnv((void**) &env, JNI_VERSION_1_4) != JNI_OK) 
	{
		__android_log_print(ANDROID_LOG_ERROR, CLASS_PATH, "Bad version");
		return;
	}
	
	//env->CallVoidMethod(surface, MID_onWindowTitle, screenBuffer);
	__android_log_print(ANDROID_LOG_INFO, CLASS_PATH, "window title setted");*/
}

static
void *createSDLBitmap(int format, int width, int height)
{
	__android_log_print(ANDROID_LOG_INFO, CLASS_PATH, "creating sdl bitmap");

	switch (format) 
	{
	case PIXEL_FORMAT_RGBA_8888:
		screenBitmap.setConfig(SkBitmap::kARGB_8888_Config,
			       width, height);
		break;

	case PIXEL_FORMAT_RGBA_4444:
		screenBitmap.setConfig(SkBitmap::kARGB_4444_Config,
			       width, height);
		break;

        case PIXEL_FORMAT_RGB_565:
		screenBitmap.setConfig(SkBitmap::kRGB_565_Config,
			       width, height);
		break;

        case PIXEL_FORMAT_A_8:
		screenBitmap.setConfig(SkBitmap::kA8_Config,
			       width, height);
		break;

        default:
		screenBitmap.setConfig(SkBitmap::kNo_Config,
			       width, height);
		break;
	}

	screenBitmap.setIsOpaque(true);
	if(!screenBitmap.allocPixels())
	{
		__android_log_print(ANDROID_LOG_INFO, CLASS_PATH,
							"Failed to alloc bitmap pixels");
		SDL_SetError("Failed to alloc bitmap pixels");
		return NULL;
	}

	__android_log_print(ANDROID_LOG_INFO, CLASS_PATH, "width: %i, height: %i, bpp: %i", screenBitmap.width(), 
											    screenBitmap.height(), 
											    screenBitmap.bytesPerPixel());

	__android_log_print(ANDROID_LOG_INFO, CLASS_PATH, "sdl bitmap created");
	return screenBitmap.getPixels();
}
 
static
void updateScreen(Surface::SurfaceInfo *surface_info)
{
	//__android_log_print(ANDROID_LOG_INFO, CLASS_PATH, "updating screen");
	SkBitmap bitmap;
	switch (surface_info->format) 
	{
	case PIXEL_FORMAT_RGBA_8888:
		bitmap.setConfig(SkBitmap::kARGB_8888_Config,
			       surface_info->w, surface_info->h);
		break;

	case PIXEL_FORMAT_RGBA_4444:
		bitmap.setConfig(SkBitmap::kARGB_4444_Config,
			       surface_info->w, surface_info->h);
		break;

        case PIXEL_FORMAT_RGB_565:
		bitmap.setConfig(SkBitmap::kRGB_565_Config,
			       surface_info->w, surface_info->h);
		break;

        case PIXEL_FORMAT_A_8:
		bitmap.setConfig(SkBitmap::kA8_Config,
			       surface_info->w, surface_info->h);
		break;

        default:
		bitmap.setConfig(SkBitmap::kNo_Config,
			       surface_info->w, surface_info->h);
		break;
	}
	bitmap.setPixels(surface_info->bits);

	SkCanvas canvas(bitmap);
	SkRect surface_sdl;
	SkRect surface_android;
	SkMatrix matrix;
	surface_android.set(0, 0, surface_info->w, surface_info->h);
	surface_sdl.set(0, 0, screenBitmap.width(), screenBitmap.height());
	matrix.setRectToRect(surface_sdl, surface_android, SkMatrix::kFill_ScaleToFit);

	canvas.drawBitmapMatrix(screenBitmap, matrix);
	//__android_log_print(ANDROID_LOG_INFO, CLASS_PATH, "screen updated");
}

static
void surfaceCreated(JNIEnv *env, jobject thiz)
{
	__android_log_print(ANDROID_LOG_INFO, CLASS_PATH, "creating surface");
	if (surface != NULL)
	{
		__android_log_print(ANDROID_LOG_INFO, CLASS_PATH, "deleting old surface");
		env->DeleteGlobalRef(surface);
		surface = NULL;
	}
	
	surface = env->NewGlobalRef(env->GetObjectField(thiz, FID_surface));
	nativeSurface = (Surface *)env->GetIntField(surface, FID_Surface_surface);

	__android_log_print(ANDROID_LOG_INFO, CLASS_PATH, "surface created");
}

static
void surfaceDestroyed(JNIEnv *env, jobject thiz)
{
	__android_log_print(ANDROID_LOG_INFO, CLASS_PATH, "destroying surface");
	SDL_PrivateQuit();
	__android_log_print(ANDROID_LOG_INFO, CLASS_PATH, "surface destroyed");
}

static
void surfaceChanged(JNIEnv *env, jobject thiz, int format, int width, int height)
{
	__android_log_print(ANDROID_LOG_INFO, CLASS_PATH, "changing surface");
	//SDL_SetVideoMode(width, height, 16, SDL_FULLSCREEN);
	__android_log_print(ANDROID_LOG_INFO, CLASS_PATH, "surface changed");
}

static
void processKey(JNIEnv*  env, jobject  thiz, jint key, jint action)
{
	SDL_keysym keysym;

	/* Sanity check */
	if (key >= SDL_arraysize(keymap))
		key = KEYCODE_UNKNOWN;

	/* Set the keysym information */
	keysym.scancode = key;
	keysym.sym = keymap[key];
	keysym.mod = KMOD_NONE;

	/* If UNICODE is on, get the UNICODE value for the key */
	keysym.unicode = 0;
	if ( SDL_TranslateUNICODE ) {
		/* Populate the unicode field with the ASCII value */
		keysym.unicode = key;
	}

	SDL_PrivateKeyboard( (action == KEY_EVENT_ACTION_DOWN) ? SDL_PRESSED : SDL_RELEASED, &keysym);
}

static
void processMouse(JNIEnv*  env, jobject  thiz, jint x, jint y, jint action)
{
	double horizontalMultiplicant = (double) screenBitmap.width() / 480;
	double verticalMultiplicant = (double) screenBitmap.height() / 320;

	if( action == MOUSE_EVENT_ACTION_DOWN || action == MOUSE_EVENT_ACTION_UP )
	{
		SDL_PrivateMouseButton( (action == MOUSE_EVENT_ACTION_DOWN) ? SDL_PRESSED : SDL_RELEASED, 1, 
							x * horizontalMultiplicant, y * verticalMultiplicant );
	}
	
	if( action == MOUSE_EVENT_ACTION_MOVE )
	{
		SDL_PrivateMouseMotion(0, 0, x * horizontalMultiplicant, y *verticalMultiplicant );
	}
}

/*
 * JNI registration.
 */
static JNINativeMethod methods[] = {
    	{ "nativeSurfaceCreated", "()V", (void*) surfaceCreated },
    	{ "nativeSurfaceDestroyed", "()V", (void*) surfaceDestroyed },
    	{ "nativeSurfaceChanged", "(III)V", (void*) surfaceChanged } ,
    	{ "nativeProcessKey", "(II)V", (void*) processKey},
    	{ "nativeProcessMouse", "(III)V", (void*) processMouse}
};

// ----------------------------------------------------------------------------

/*
 * Register several native methods for one class.
 */
static int registerNativeMethods(JNIEnv* env, jclass clazz,
    JNINativeMethod* gMethods, int numMethods)
{
    if (clazz == NULL) {
        //LOGE("Native registration unable to find class '%s'", className);
        return JNI_FALSE;
    }

    if (env->RegisterNatives(clazz, gMethods, numMethods) < 0) {
        //LOGE("RegisterNatives failed for '%s'", className);
        return JNI_FALSE;
    }

    return JNI_TRUE;
}

// ----------------------------------------------------------------------------

/*
 * This is called by the VM when the shared library is first loaded.
 */
JNIEXPORT jint JNICALL
JNI_OnLoad(JavaVM* vm, void* reserved)
{
    jint result = JNI_ERR;
    JNIEnv* env = NULL;

	jclass CLS_surface;
	jclass cls;
	
    __android_log_print(ANDROID_LOG_INFO, CLASS_PATH, "loading library");
	
	mVM = vm;

    if (vm->GetEnv((void**) &env, JNI_VERSION_1_4) != JNI_OK) {
    	__android_log_print(ANDROID_LOG_ERROR, CLASS_PATH, "Bad version");
        goto bail;
    }
	
    cls = env->FindClass("android/sdl/SDLSurfaceView");
	if(cls == NULL)
	{
		__android_log_print(ANDROID_LOG_ERROR, CLASS_PATH, "can't load SDL view class");
		goto bail;
	}
	
    if (env->RegisterNatives(cls, methods, sizeof(methods) / sizeof(methods[0])) < 0)
    {
       	__android_log_print(ANDROID_LOG_ERROR, CLASS_PATH, "can't register methods");
        goto bail;
    }

    CLS_surface = env->FindClass("android/view/Surface");
    if(CLS_surface == NULL)
    {
       	__android_log_print(ANDROID_LOG_ERROR, CLASS_PATH, "can't load native surface");
		goto bail;
    }
	
	FID_surface = env->GetFieldID(cls, "mSurface", "Landroid/view/Surface;");
  	if(FID_surface == NULL)
  	{
		__android_log_print(ANDROID_LOG_ERROR, CLASS_PATH, "can't load java mSurface");
  	  	goto bail;
  	}
	
	MID_onWindowTitle = env->GetMethodID(cls, "onWindowTitle", "(Ljava/lang/String;)V");
	if(MID_onWindowTitle == NULL)
  	{
		__android_log_print(ANDROID_LOG_ERROR, CLASS_PATH, "can't load onWindowTitle callback");
  	  	goto bail;
  	}
	
  	FID_Surface_surface = env->GetFieldID(CLS_surface, "mSurface", "I");
  	if(FID_Surface_surface == NULL)
  	{
		__android_log_print(ANDROID_LOG_ERROR, CLASS_PATH, "can't load native mSurface");
  	  	goto bail;
  	}
	
	__android_log_print(ANDROID_LOG_INFO, CLASS_PATH, "library loaded");
    result = JNI_VERSION_1_4;

bail:
    return result;
}
