#include "SDL_androidvideo.h"

#define CLASS_PATH "SDL_androidvideo.cpp"

static SDLVideoDriver *thiz = NULL;
static SDLKey keymap[SDLVideoDriver::KEYCODE_LAST+1];

SDLVideoDriver::SDLVideoDriver() {
}

SDLVideoDriver::~SDLVideoDriver() {
	unregisterListener();
}

SDLVideoDriver *SDLVideoDriver::getInstance() {
    if(thiz == NULL) thiz = new SDLVideoDriver();
    return thiz;
}

void SDLVideoDriver::registerListener(SDLVideoDriverListener *listener) {
    mListener = listener;
	__android_log_print(ANDROID_LOG_INFO, CLASS_PATH, "listener registred");
}

void SDLVideoDriver::unregisterListener() {
	if(mListener == NULL) {
		return;
	}
    free(mListener);
}

void SDLVideoDriver::processKey(int key, int action) {
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

    SDL_PrivateKeyboard((action == KEY_EVENT_ACTION_DOWN) ? SDL_PRESSED : SDL_RELEASED, &keysym);
}

void SDLVideoDriver::processMouse(double x, double y, int action) {
    if( action == MOUSE_EVENT_ACTION_DOWN || action == MOUSE_EVENT_ACTION_UP ) {
        SDL_PrivateMouseButton( (action == MOUSE_EVENT_ACTION_DOWN) ? SDL_PRESSED : SDL_RELEASED, 1, x, y);
    }

    if( action == MOUSE_EVENT_ACTION_MOVE ) {
        SDL_PrivateMouseMotion(0, 0, x, y);
    }
}

void SDLVideoDriver::setBitmapConfig(SkBitmap *bitmap, int format, int width, int height) {
     switch (format) {
        case PIXEL_FORMAT_RGBA_8888:
            bitmap->setConfig(SkBitmap::kARGB_8888_Config, width, height);
            break;

        case PIXEL_FORMAT_RGBA_4444:
            bitmap->setConfig(SkBitmap::kARGB_4444_Config, width, height);
            break;

        case PIXEL_FORMAT_RGB_565:
            bitmap->setConfig(SkBitmap::kRGB_565_Config, width, height);
            break;

        case PIXEL_FORMAT_A_8:
            bitmap->setConfig(SkBitmap::kA8_Config, width, height);
            break;

        default:
            bitmap->setConfig(SkBitmap::kNo_Config, width, height);
            break;
    }
}

void SDLVideoDriver::initBitmap(int format, int width, int height) {
    //-- set screen bitmap(sdl) config based on format
    setBitmapConfig(&mBitmap, format, width, height);
    mBitmap.setIsOpaque(true);

    //-- alloc array of pixels
    if(!mBitmap.allocPixels()) {
        SDL_SetError("Failed to alloc bitmap pixels");
    }
}

/* Initialization/Query functions */
SDL_VideoDevice *SDLVideoDriver::onCreateDevice(int devindex) {
    // if class isn't yet initzialized, we will do it
    getInstance();

    /* Initialize all variables that we clean on shutdown */
    thiz->device = (SDL_VideoDevice *)SDL_malloc(sizeof(SDL_VideoDevice));
    if (thiz->device) {
        SDL_memset(thiz->device, 0, (sizeof *thiz->device));
    }
    if (thiz->device == NULL) {
        SDL_OutOfMemory();
        if (thiz->device ) {
            SDL_free(thiz->device);
        }
        return 0;
    }
	
	__android_log_print(ANDROID_LOG_INFO, CLASS_PATH, "device creating");
	
    /* Set the function pointers */
    thiz->device->VideoInit = SDLVideoDriver::onVideoInit;
    thiz->device->ListModes = SDLVideoDriver::onListModes;
    thiz->device->SetVideoMode = SDLVideoDriver::onSetVideoMode;
    thiz->device->CreateYUVOverlay = NULL;
    thiz->device->SetColors = SDLVideoDriver::onSetColors;
    thiz->device->UpdateRects = SDLVideoDriver::onUpdateRects;
    thiz->device->VideoQuit = SDLVideoDriver::onVideoQuit;
    thiz->device->AllocHWSurface = SDLVideoDriver::onAllocHWSurface;
    thiz->device->CheckHWBlit = NULL;
    thiz->device->FillHWRect = NULL;
    thiz->device->SetHWColorKey = NULL;
    thiz->device->SetHWAlpha = NULL;
    thiz->device->LockHWSurface = SDLVideoDriver::onLockHWSurface;
    thiz->device->UnlockHWSurface = SDLVideoDriver::onUnlockHWSurface;
    thiz->device->FlipHWSurface = NULL;
    thiz->device->FreeHWSurface = SDLVideoDriver::onFreeHWSurface;
    thiz->device->SetCaption = SDLVideoDriver::onSetCaption;
    thiz->device->SetIcon = NULL;
    thiz->device->IconifyWindow = NULL;
    thiz->device->GrabInput = NULL;
    thiz->device->GetWMInfo = NULL;
    thiz->device->InitOSKeymap = SDLVideoDriver::onInitOSKeymap;
    thiz->device->PumpEvents = SDLVideoDriver::onPumpEvents;
    thiz->device->free = SDLVideoDriver::onDeleteDevice;
	
	thiz->mListener->notify(SDL_NATIVE_VIDEO_CREATE_DEVICE, 0, 0, (void*) thiz->device);

    __android_log_print(ANDROID_LOG_INFO, CLASS_PATH, "device created");
	
    return thiz->device;
}

int SDLVideoDriver::onVideoInit(_THIS, SDL_PixelFormat *vformat) {
    /* TODO: we only support RGB565 for now */
    vformat->BitsPerPixel = 16;
    vformat->BytesPerPixel = 2;
	
	thiz->mListener->notify(SDL_NATIVE_VIDEO_INIT, 0, 0, (void*) vformat);
	
	__android_log_print(ANDROID_LOG_INFO, CLASS_PATH, "video initzialized: bpp=%i, Bpp=%i",
                        vformat->BitsPerPixel,
                        vformat->BytesPerPixel);
    /* We're done! */
    return 0;	
}

SDL_Rect **SDLVideoDriver::onListModes(_THIS, SDL_PixelFormat *format, Uint32 flags) {
    //__android_log_print(ANDROID_LOG_INFO, CLASS_PATH, "list modes");
    return (SDL_Rect **) -1;
}

SDL_Surface *SDLVideoDriver::onSetVideoMode(_THIS, SDL_Surface *current, int width, int height, int bpp, Uint32 flags) {
    if(self == NULL)
        return NULL;
	
    /* Allocate the new pixel format for the screen */
    if (!SDL_ReallocFormat(current, bpp, 0, 0, 0, 0)) {
        SDL_SetError("Couldn't allocate new pixel format for requested mode");
        return NULL;
    }

    // create bitmap where will sdl render pixels for android
    thiz->initBitmap(PIXEL_FORMAT_RGB_565, width, height);
	
    /* Set up the new mode framebuffer */
    current->flags = (flags & SDL_FULLSCREEN) | (flags & SDL_DOUBLEBUF);
    current->w = thiz->mBitmap.width();
    current->h = thiz->mBitmap.height();
    current->pitch = current->w * (bpp / 8);
    current->pixels = thiz->mBitmap.getPixels();
	
	thiz->mListener->notify(SDL_NATIVE_VIDEO_SET_SURFACE, 0, 0, (void*) current);

    __android_log_print(ANDROID_LOG_INFO, CLASS_PATH, "setting sdl bitmap size to: %dx%d", width, height);
    /* We're done */
    return current;
}

int SDLVideoDriver::onSetColors(_THIS, int firstcolor, int ncolors, SDL_Color *colors) {
        return 1;
}

void SDLVideoDriver::onVideoQuit(_THIS) {
}

/* Hardware surface functions */
int SDLVideoDriver::onAllocHWSurface(_THIS, SDL_Surface *surface) {
    return(-1);
}

int SDLVideoDriver::onLockHWSurface(_THIS, SDL_Surface *surface) {
    return 0;
}

void SDLVideoDriver::onUnlockHWSurface(_THIS, SDL_Surface *surface) {
}

void SDLVideoDriver::onFreeHWSurface(_THIS, SDL_Surface *surface) {
}

/* on set window caption */
void SDLVideoDriver::onSetCaption(_THIS, const char *title, const char *icon) {
}

/* etc. */
void SDLVideoDriver::onUpdateRects(_THIS, int numrects, SDL_Rect *rects) {
    if(thiz->mBitmap.width() == 0 || thiz->mBitmap.height() == 0) {
        __android_log_print(ANDROID_LOG_ERROR, CLASS_PATH, "Bitmap is too small %ix%i",
                thiz->mBitmap.width(), thiz->mBitmap.height());
        return;
    }

	thiz->mListener->notify(SDL_NATIVE_VIDEO_UPDATE_RECTS, 0, 0, (void*) &thiz->mBitmap);
	/*
    int size = thiz->mListeners.size();
    for(int i=0;i<size;i++) {
        SDLVideoDriverListener *listener = thiz->mListeners.itemAt(i);
        listener->onUpdateScreen(&thiz->mBitmap);
    }
	*/
}

/* ANDROID driver bootstrap functions */
int SDLVideoDriver::onAvailable() {
    return 1;
}

void SDLVideoDriver::onDeleteDevice(SDL_VideoDevice *device) {
	/*
    int size = thiz->mListeners.size();
    for(int i=0;i<size;i++) {
        SDLVideoDriverListener *listener = thiz->mListeners.itemAt(i);
        listener->onDeleteDevice();
    }
	*/
	thiz->mListener->notify(SDL_NATIVE_VIDEO_DELETE_DEVICE, 0, 0, (void*) device);
	
    SDL_free(thiz->device);
    __android_log_print(ANDROID_LOG_INFO, CLASS_PATH, "device deleted");
}

void SDLVideoDriver::onPumpEvents(_THIS) {
	/*
    int size = thiz->mListeners.size();
    for(int i=0;i<size;i++) {
        SDLVideoDriverListener *listener = thiz->mListeners.itemAt(i);
        listener->onProcessEvents();
    }
	*/
	thiz->mListener->notify(SDL_NATIVE_VIDEO_PUMP_EVENTS, 0, 0, NULL);
}

void SDLVideoDriver::onInitOSKeymap(_THIS) {
    //__android_log_print(ANDROID_LOG_INFO, CLASS_PATH, "initzializing key map");
        unsigned int i;

        /* Initialize the DirectFB key translation table */
        for (i = 0; i < SDL_arraysize(keymap); ++i)
            keymap[i] = SDLK_UNKNOWN;

        keymap[KEYCODE_UNKNOWN] = SDLK_UNKNOWN;

        //keymap[KEYCODE_MENU] = SDLK_ESCAPE;

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

extern "C" {

VideoBootStrap ANDROID_bootstrap = {
    SDLVideoDriver::getDriverName(), SDLVideoDriver::getDriverDescription(),
    SDLVideoDriver::onAvailable, SDLVideoDriver::onCreateDevice
};

}//end extern C
