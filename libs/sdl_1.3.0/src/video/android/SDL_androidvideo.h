
#ifndef _SDL_androidvideo_h
#define _SDL_androidvideo_h

#include "SDL_config.h"

extern "C" {
#include "SDL_video.h"
#include "SDL_mouse.h"
#include "../SDL_sysvideo.h"
#include "../SDL_pixels_c.h"
#include "../../events/SDL_events_c.h"
}

#include <binder/IMemory.h>

using namespace android;

enum sdl_native_events {
    SDL_NATIVE_VIDEO_CREATE_DEVICE = 1,
    SDL_NATIVE_VIDEO_DELETE_DEVICE = 2,
    SDL_NATIVE_VIDEO_PUMP_EVENTS = 3,
    SDL_NATIVE_VIDEO_INIT = 4,
    SDL_NATIVE_VIDEO_SET_SURFACE = 5,
    SDL_NATIVE_VIDEO_UPDATE_RECTS = 6,
    SDL_NATIVE_VIDEO_SET_CAPTION = 7,
    SDL_NATIVE_VIDEO_INIT_OS_KEYMAP = 8,
    SDL_NATIVE_VIDEO_ALLOC_HW_SURFACE = 9,
    SDL_NATIVE_VIDEO_LOCK_HW_SURFACE = 10,
    SDL_NATIVE_VIDEO_UNLOCK_HW_SURFACE = 11,
    SDL_NATIVE_VIDEO_FREE_HW_SURFACE = 12,

    /**** GL implementation *****/
    SDL_NATIVE_VIDEO_GL_LOAD_LIBRARY = 13,
    SDL_NATIVE_VIDEO_GL_GET_PROC_ADDRESS = 14,
    SDL_NATIVE_VIDEO_GL_UNLOAD_LIBRARY = 15,
    SDL_NATIVE_VIDEO_GL_CREATE_CONTEXT = 16,
    SDL_NATIVE_VIDEO_GL_MAKE_CURRENT = 17,
    SDL_NATIVE_VIDEO_GL_SET_SWAP_INTERVAL = 18,
    SDL_NATIVE_VIDEO_GL_GET_SWAP_INTERVAL = 19,
    SDL_NATIVE_VIDEO_GL_SWAP_WINDOW = 20,
    SDL_NATIVE_VIDEO_GL_DELETE_CONTEXT = 21,
};

/* Hidden "this" pointer for the video functions */
#define _THIS	SDL_VideoDevice *self

// ----------------------------------------------------------------------------
// ref-counted object for callbacks
class SDLVideoDriverListener : virtual public RefBase
{
public:
    virtual void notify(int what, int arg1, int arg2, void* data) = 0;
};

class SDLVideoDriver {
public:
	SDLVideoDriver();
	~SDLVideoDriver();

	void registerListener(SDLVideoDriverListener *listener);
	void unregisterListener();

	static SDLVideoDriver *getInstance();

	/* ANDROID driver bootstrap functions */
	static SDL_VideoDevice *onCreateDevice(int devindex);
	static int onAvailable();

	static const char *getDriverName() {
		return "Android";
	}

	static const char *getDriverDescription() {
		return "SDL android video driver";
	}

private:
	SDL_VideoDevice *device;
	SDLVideoDriverListener* mListener;
	
	/* Initialization/Query functions */
	static int onVideoInit(_THIS);
	static void onVideoQuit(_THIS);
	static void onDeleteDevice(SDL_VideoDevice *device);

	static void onPumpEvents(_THIS);

	static int GLLoadLibrary(_THIS, const char *path);
	static void* GLGetProcAddress(_THIS, const char *proc);
	static void GLUnloadLibrary(_THIS);
	static SDL_GLContext GLCreateContext(_THIS, SDL_Window * window);
	static int GLMakeCurrent(_THIS, SDL_Window * window, SDL_GLContext context);
	static int GLSetSwapInterval(_THIS, int interval);
	static int GLGetSwapInterval(_THIS);
	static void GLSwapWindow(_THIS, SDL_Window * window);
	static void GLDeleteContext(_THIS, SDL_GLContext context);
};

#endif /* _SDL_androidvideo_h */
