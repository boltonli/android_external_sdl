
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

#include <ui/Surface.h>
#include <SkBitmap.h>

using namespace android;

/* Hidden "this" pointer for the video functions */
#define _THIS	SDL_VideoDevice *self

class SDLVideoDriver {
public:
        class SDLVideoDriverListener
        {
		protected:
			~SDLVideoDriverListener();
        public:
			virtual void onUpdateScreen(SkBitmap *bitmap);
        };

        SDLVideoDriver();
        ~SDLVideoDriver();

        void registerListener(SDLVideoDriverListener *listener);
        void unregisterListener(SDLVideoDriverListener *listener);
        void processKey(int key, int action);
        void processMouse(double x, double y, int action);

        static SDLVideoDriver *getInstance();
        static void setBitmapConfig(SkBitmap *bitmap, int format, int width, int height);
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
        SkBitmap mBitmap;
        Vector<SDLVideoDriverListener*> mListeners;

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

public:
        enum KEY_EVENTS_ANDROID {
            KEY_EVENT_ACTION_DOWN = 0,
            KEY_EVENT_ACTION_UP = 1
        };

        enum MOUSE_EVENTS_ANDROID {
            MOUSE_EVENT_ACTION_DOWN = 0,
            MOUSE_EVENT_ACTION_UP = 1,
            MOUSE_EVENT_ACTION_MOVE = 2,
        };

        // Keycodes ripped from Java SDK
        enum KEYCODES_ANDROID {
            KEYCODE_UNKNOWN = 0,
            KEYCODE_SOFT_LEFT = 1,
            KEYCODE_SOFT_RIGHT = 2,
            KEYCODE_HOME = 3,
            KEYCODE_BACK = 4,
            KEYCODE_CALL = 5,
            KEYCODE_ENDCALL = 6,
            KEYCODE_0 = 7,
            KEYCODE_1 = 8,
            KEYCODE_2 = 9,
            KEYCODE_3 = 10,
            KEYCODE_4 = 11,
            KEYCODE_5 = 12,
            KEYCODE_6 = 13,
            KEYCODE_7 = 14,
            KEYCODE_8 = 15,
            KEYCODE_9 = 16,
            KEYCODE_STAR = 17,
            KEYCODE_POUND = 18,
            KEYCODE_DPAD_UP = 19,
            KEYCODE_DPAD_DOWN = 20,
            KEYCODE_DPAD_LEFT = 21,
            KEYCODE_DPAD_RIGHT = 22,
            KEYCODE_DPAD_CENTER = 23,
            KEYCODE_VOLUME_UP = 24,
            KEYCODE_VOLUME_DOWN = 25,
            KEYCODE_POWER = 26,
            KEYCODE_CAMERA = 27,
            KEYCODE_CLEAR = 28,
            KEYCODE_A = 29,
            KEYCODE_B = 30,
            KEYCODE_C = 31,
            KEYCODE_D = 32,
            KEYCODE_E = 33,
            KEYCODE_F = 34,
            KEYCODE_G = 35,
            KEYCODE_H = 36,
            KEYCODE_I = 37,
            KEYCODE_J = 38,
            KEYCODE_K = 39,
            KEYCODE_L = 40,
            KEYCODE_M = 41,
            KEYCODE_N = 42,
            KEYCODE_O = 43,
            KEYCODE_P = 44,
            KEYCODE_Q = 45,
            KEYCODE_R = 46,
            KEYCODE_S = 47,
            KEYCODE_T = 48,
            KEYCODE_U = 49,
            KEYCODE_V = 50,
            KEYCODE_W = 51,
            KEYCODE_X = 52,
            KEYCODE_Y = 53,
            KEYCODE_Z = 54,
            KEYCODE_COMMA = 55,
            KEYCODE_PERIOD = 56,
            KEYCODE_ALT_LEFT = 57,
            KEYCODE_ALT_RIGHT = 58,
            KEYCODE_SHIFT_LEFT = 59,
            KEYCODE_SHIFT_RIGHT = 60,
            KEYCODE_TAB = 61,
            KEYCODE_SPACE = 62,
            KEYCODE_SYM = 63,
            KEYCODE_EXPLORER = 64,
            KEYCODE_ENVELOPE = 65,
            KEYCODE_ENTER = 66,
            KEYCODE_DEL = 67,
            KEYCODE_GRAVE = 68,
            KEYCODE_MINUS = 69,
            KEYCODE_EQUALS = 70,
            KEYCODE_LEFT_BRACKET = 71,
            KEYCODE_RIGHT_BRACKET = 72,
            KEYCODE_BACKSLASH = 73,
            KEYCODE_SEMICOLON = 74,
            KEYCODE_APOSTROPHE = 75,
            KEYCODE_SLASH = 76,
            KEYCODE_AT = 77,
            KEYCODE_NUM = 78,
            KEYCODE_HEADSETHOOK = 79,
            KEYCODE_FOCUS = 80,
            KEYCODE_PLUS = 81,
            KEYCODE_MENU = 82,
            KEYCODE_NOTIFICATION = 83,
            KEYCODE_SEARCH = 84,
            KEYCODE_MEDIA_PLAY_PAUSE= 85,
            KEYCODE_MEDIA_STOP = 86,
            KEYCODE_MEDIA_NEXT = 87,
            KEYCODE_MEDIA_PREVIOUS = 88,
            KEYCODE_MEDIA_REWIND = 89,
            KEYCODE_MEDIA_FAST_FORWARD = 90,
            KEYCODE_MUTE = 91,

            KEYCODE_LAST = KEYCODE_MUTE
        };
};

#endif /* _SDL_androidvideo_h */
