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

#include <limits.h>

#include "SDL_androidvideo.h"

#include <ui/DisplayInfo.h>
#include <utils/Log.h>
#include <jni.h>

using namespace android;

static
Surface *gSurface = NULL;

extern "C" {

#define ANDROIDVID_DRIVER_NAME "android"

/* Initialization/Query functions */
static int ANDROID_VideoInit(_THIS, SDL_PixelFormat *vformat);
static SDL_Rect **ANDROID_ListModes(_THIS, SDL_PixelFormat *format, Uint32 flags);
static SDL_Surface *ANDROID_SetVideoMode(_THIS, SDL_Surface *current, int width, int height, int bpp, Uint32 flags);
static int ANDROID_SetColors(_THIS, int firstcolor, int ncolors, SDL_Color *colors);
static void ANDROID_VideoQuit(_THIS);
static void ANDROID_InitOSKeymap(_THIS);	
static void ANDROID_PumpEvents(_THIS);

/* Hardware surface functions */
static int ANDROID_AllocHWSurface(_THIS, SDL_Surface *surface);
static int ANDROID_LockHWSurface(_THIS, SDL_Surface *surface);
static void ANDROID_UnlockHWSurface(_THIS, SDL_Surface *surface);
static void ANDROID_FreeHWSurface(_THIS, SDL_Surface *surface);

/* etc. */
static void ANDROID_UpdateRects(_THIS, int numrects, SDL_Rect *rects);

/* ANDROID driver bootstrap functions */

static 
int ANDROID_Available(void) {
    return 1;
}

static 
void ANDROID_DeleteDevice(SDL_VideoDevice *device)
{
    delete device->hidden->scc;
    delete device->hidden;
    SDL_free(device);
}

static
SDL_VideoDevice *ANDROID_CreateDevice(int devindex)
{
    SDL_VideoDevice *device;

    /* Initialize all variables that we clean on shutdown */
    device = (SDL_VideoDevice *)SDL_malloc(sizeof(SDL_VideoDevice));
    if ( device ) {
        SDL_memset(device, 0, (sizeof *device));
        device->hidden = new SDL_PrivateVideoData;
    }
    if ( (device == NULL) || (device->hidden == NULL) ) {
        SDL_OutOfMemory();
        if ( device ) {
            SDL_free(device);
        }
        return(0);
    }

    device->hidden->scc = new SurfaceComposerClient;

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
    device->SetCaption = NULL;
    device->SetIcon = NULL;
    device->IconifyWindow = NULL;
    device->GrabInput = NULL;
    device->GetWMInfo = NULL;
    device->InitOSKeymap = ANDROID_InitOSKeymap;
    device->PumpEvents = ANDROID_PumpEvents;

    device->free = ANDROID_DeleteDevice;

    return device;
}

VideoBootStrap ANDROID_bootstrap = {
    ANDROIDVID_DRIVER_NAME, "SDL android video driver",
    ANDROID_Available, ANDROID_CreateDevice
};

static
int ANDROID_VideoInit(_THIS, SDL_PixelFormat *vformat)
{
    /* TODO: we only support RGB565 for now */
    vformat->BitsPerPixel = 16;
    vformat->BytesPerPixel = 2;

    /* We're done! */
    return(0);
}

static
SDL_Rect **ANDROID_ListModes(_THIS, SDL_PixelFormat *format, Uint32 flags) {
     return (SDL_Rect **) -1;
}

static
SDL_Surface *ANDROID_SetVideoMode(_THIS, SDL_Surface *current,
                int width, int height, int bpp, Uint32 flags) {
	
	sp<Surface> surface;
    status_t st;

    if(gSurface == NULL) {
        DisplayID dpy_id = 0;
        DisplayInfo dpy_info;
		
        st = SurfaceComposerClient::getDisplayInfo(dpy_id, &dpy_info);
        if (st != 0) {
            SDL_SetError("Couldn't get display info");
            return NULL;
        }

        self->hidden->scc->openTransaction();

        self->hidden->sc = self->hidden->scc->createSurface(
            getpid(), dpy_id, width, height, dpy_info.pixelFormatInfo.format);
        if (self->hidden->sc == NULL) {
            SDL_SetError("Couldn't create display surface");
            return NULL;
		}

        self->hidden->sc->setLayer(INT_MAX);
        surface = self->hidden->sc->getSurface();
	} else {
        surface = gSurface;
    }
	
    Surface::SurfaceInfo surf_info;
    st = surface->lock(&surf_info, true);
    if (st != 0) {
        self->hidden->sc->clear();
        SDL_SetError("Failed to lock surface");
        return NULL;
    }
    self->hidden->buffer = surf_info.bits;

    if(gSurface == NULL) {
        st = self->hidden->sc->show();
        if (st != 0) {
            self->hidden->sc->clear();
            SDL_SetError("Coundn't show display surface");
            return NULL;
	    }

        self->hidden->scc->closeTransaction();
    }

/*  printf("Setting mode %dx%d\n", width, height); */

    /* Allocate the new pixel format for the screen */
    if (!SDL_ReallocFormat(current, bpp, 0, 0, 0, 0)) {
        self->hidden->sc->clear();
        SDL_SetError("Couldn't allocate new pixel format for requested mode");
        return NULL;
    }

    /* Set up the new mode framebuffer */
    current->flags = flags;
    self->hidden->w = current->w = width;
    self->hidden->h = current->h = height;
    current->pitch = current->w * (bpp / 8);
    current->pixels = self->hidden->buffer;

    /* We're done */
    return current;
}

/* We don't actually allow hardware surfaces other than the main one */
static
int ANDROID_AllocHWSurface(_THIS, SDL_Surface *surface) {
    return(-1);
}

static
void ANDROID_FreeHWSurface(_THIS, SDL_Surface *surface) {
    return;
}

/* We need to wait for vertical retrace on page flipped displays */
static 
int ANDROID_LockHWSurface(_THIS, SDL_Surface *surface)
{
    return(0);
}

static 
void ANDROID_UnlockHWSurface(_THIS, SDL_Surface *surface) {
    return;
}

static 
void ANDROID_UpdateRects(_THIS, int numrects, SDL_Rect *rects) {
    Surface::SurfaceInfo surf_info;
    sp<Surface> surface;
    status_t st;

	if(gSurface == NULL) {
        surface = self->hidden->sc->getSurface();
    } else {
        surface = gSurface;
    }
	
    if (surface == NULL) {
        /* why? */
        return;
    }

    st = surface->lock(&surf_info, true);
    if (st != 0) {
        self->hidden->sc->clear();
        SDL_SetError("Failed to lock surface");
        return;
    }
	
    self->screen->pixels = self->hidden->buffer = surf_info.bits;
	
	st = surface->unlockAndPost();
    if (st != 0) {
        SDL_SetError("Failed to unlock surface");
    }
}

static
int ANDROID_SetColors(_THIS, int firstcolor, int ncolors, SDL_Color *colors) {
    /* do nothing of note. */
    return(1);
}

static
void ANDROID_PumpEvents(_THIS) {
}

static
void ANDROID_InitOSKeymap(_THIS) {
}

/* Note:  If we are terminated, this could be called in the middle of
   another SDL video routine -- notably UpdateRects.
*/
static
void ANDROID_VideoQuit(_THIS) {
#if 0
    if (self->screen->pixels != NULL)
    {
        SDL_free(self->screen->pixels);
        self->screen->pixels = NULL;
    }
#endif
}

}

/*********************** JNI implementation ************************/

#define CLASS_PATH "android/sdl/video/SDLVideo"
#define LOG_TAG "SDLVideo"

struct fields_t {
    /* actually in android.view.Surface XXX */
    jfieldID    surface_native;
    jmethodID   post_event;
};
static fields_t fields;

static void
android_sdl_video_SDLVideo_native_setup(JNIEnv *env, jobject thiz, jobject surface) {
    gSurface = (Surface *) env->GetIntField(surface, fields.surface_native);
}

static jboolean
android_sdl_video_SDLVideo_native_init(JNIEnv *env)
{
    LOGV("native_init");
	
    jclass clazz = env->FindClass(CLASS_PATH);
    if (clazz == NULL) {
        return JNI_FALSE;
    }
	
    jclass surface_clazz = env->FindClass("android/view/Surface");
    if (surface_clazz == NULL) {
		return JNI_FALSE;
    }
	
    fields.surface_native = env->GetFieldID(surface_clazz, "mSurface", "I");
    if (fields.surface_native == NULL) {
		return JNI_FALSE;
    }
    return JNI_TRUE;
}

static JNINativeMethod methods[] = {
	{"native_init",     "()V",                              (void*) android_sdl_video_SDLVideo_native_init },
	{"native_setup",    "(Landroid/view/Surface;)V",        (void*) android_sdl_video_SDLVideo_native_setup },
};

/*
 * Register several native methods for one class.
 */
static 
int registerNativeMethods(JNIEnv* env, const char* className,
								 JNINativeMethod* gMethods, int numMethods)
{
    jclass clazz;
	
    clazz = env->FindClass(className);
    if (clazz == NULL) {
        LOGE("Native registration unable to find class '%s'", className);
        return JNI_FALSE;
    }
    if (env->RegisterNatives(clazz, gMethods, numMethods) < 0) {
        LOGE("RegisterNatives failed for '%s'", className);
        return JNI_FALSE;
    }
	
    return JNI_TRUE;
}

/*
 * Register native methods for all classes we know about.
 *
 * returns JNI_TRUE on success.
 */
static 
int registerNatives(JNIEnv* env) {
	if (!registerNativeMethods(env, CLASS_PATH,
							   methods, sizeof(methods) / sizeof(methods[0]))) {
		return JNI_FALSE;
	}
	
	return JNI_TRUE;
}


// ----------------------------------------------------------------------------

/*
 * This is called by the VM when the shared library is first loaded.
 */

typedef union {
    JNIEnv* env;
    void* venv;
} UnionJNIEnvToVoid;

jint JNI_OnLoad(JavaVM* vm, void* reserved) {
    UnionJNIEnvToVoid uenv;
    uenv.venv = NULL;
    jint result = -1;
    JNIEnv* env = NULL;
    
    LOGI("JNI_OnLoad");
	
    if (vm->GetEnv(&uenv.venv, JNI_VERSION_1_4) != JNI_OK) {
        LOGE("ERROR: GetEnv failed");
        goto bail;
    }
    env = uenv.env;
	
    if (registerNatives(env) != JNI_TRUE) {
        LOGE("ERROR: registerNatives failed");
        goto bail;
    }
    
    result = JNI_VERSION_1_4;
    
bail:
    return result;
}