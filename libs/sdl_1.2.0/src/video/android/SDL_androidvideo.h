
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

//#include <binder/IMemory.h>
//#include <surfaceflinger/Surface.h>
//#include <SkBitmap.h>

//using namespace android;

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
};

/* Hidden "this" pointer for the video functions */
#define _THIS	SDL_VideoDevice *self

// ----------------------------------------------------------------------------
// ref-counted object for callbacks
class SDLVideoDriverListener/* : virtual public RefBase*/
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
//	static void setBitmapConfig(SkBitmap *bitmap, int format, int width, int height);
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
//	SkBitmap mBitmap;
        void *mPixelBuffer;
	SDLVideoDriverListener* mListener;

	void initBitmap(int format, int width, int height);
	
	/* Initialization/Query functions */
	static int onVideoInit(_THIS, SDL_PixelFormat *vformat);
	static SDL_Rect **onListModes(_THIS, SDL_PixelFormat *format, Uint32 flags);
	static SDL_Surface *onSetVideoMode(_THIS, SDL_Surface *current, int width, int height, int bpp, Uint32 flags);
	static int onSetColors(_THIS, int firstcolor, int ncolors, SDL_Color *colors);
	static void onVideoQuit(_THIS);
	
	/* Hardware surface functions */
	static int onAllocHWSurface(_THIS, SDL_Surface *surface);
	static int onLockHWSurface(_THIS, SDL_Surface *surface);
	static void onUnlockHWSurface(_THIS, SDL_Surface *surface);
	static void onFreeHWSurface(_THIS, SDL_Surface *surface);
	
	/* on set window caption */
	static void onSetCaption(_THIS, const char *title, const char *icon);
	
	/* etc. */
	static void onUpdateRects(_THIS, int numrects, SDL_Rect *rects);
	
	static void onDeleteDevice(SDL_VideoDevice *device);

	static void onPumpEvents(_THIS);
	static void onInitOSKeymap(_THIS);
};

#endif /* _SDL_androidvideo_h */
