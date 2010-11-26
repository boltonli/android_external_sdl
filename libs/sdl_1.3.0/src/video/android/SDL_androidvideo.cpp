#include "SDL_androidvideo.h"
#include "SDL_androidrender.h"

#define CLASS_PATH "SDL_androidvideo.cpp"

static SDLVideoDriver *thiz = NULL;
//These are filled in with real values in Android_SetScreenResolution on 
//init (before SDL_Main())
static int iScreenWidth = 480;
static int iScreenHeight = 320;

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
/*
    thiz->device->ListModes = SDLVideoDriver::onListModes;
    thiz->device->SetVideoMode = SDLVideoDriver::onSetVideoMode;
    thiz->device->CreateYUVOverlay = NULL;
    thiz->device->SetColors = SDLVideoDriver::onSetColors;
    thiz->device->UpdateRects = SDLVideoDriver::onUpdateRects;
*/
    thiz->device->VideoQuit = SDLVideoDriver::onVideoQuit;
/*
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
*/
    //thiz->device->SetDisplayMode = SDLVideoDriver::SetDisplayMode;
    thiz->device->PumpEvents = SDLVideoDriver::onPumpEvents;

    thiz->device->free = SDLVideoDriver::onDeleteDevice;
    /* GL pointers */
    thiz->device->GL_LoadLibrary = SDLVideoDriver::GLLoadLibrary;
    thiz->device->GL_GetProcAddress = SDLVideoDriver::GLGetProcAddress;
    thiz->device->GL_UnloadLibrary = SDLVideoDriver::GLUnloadLibrary;
    thiz->device->GL_CreateContext = SDLVideoDriver::GLCreateContext;
    thiz->device->GL_MakeCurrent = SDLVideoDriver::GLMakeCurrent;
    thiz->device->GL_SetSwapInterval = SDLVideoDriver::GLSetSwapInterval;
    thiz->device->GL_GetSwapInterval = SDLVideoDriver::GLGetSwapInterval;
    thiz->device->GL_SwapWindow = SDLVideoDriver::GLSwapWindow;
    thiz->device->GL_DeleteContext = SDLVideoDriver::GLDeleteContext;
	
    thiz->mListener->notify(SDL_NATIVE_VIDEO_CREATE_DEVICE, 0, 0, (void*) thiz->device);

    __android_log_print(ANDROID_LOG_INFO, CLASS_PATH, "device created");
	
    return thiz->device;
}

int SDLVideoDriver::onVideoInit(_THIS/*, SDL_PixelFormat *vformat*/) {
    SDL_DisplayMode mode;

    /* Use a fake 32-bpp desktop mode */
    mode.format = SDL_PIXELFORMAT_RGB888;
    mode.w = iScreenWidth;
    mode.h = iScreenHeight;
    mode.refresh_rate = 0;
    mode.driverdata = NULL;
    if (SDL_AddBasicVideoDisplay(&mode) < 0) {
        return -1;
    }
    SDL_AddRenderDriver(&self->displays[0], &Android_RenderDriver);

    SDL_zero(mode);
    SDL_AddDisplayMode(&self->displays[0], &mode);

    /* TODO: we only support RGB565 for now *
    vformat->BitsPerPixel = 16;
    vformat->BytesPerPixel = 2;
	
    thiz->mListener->notify(SDL_NATIVE_VIDEO_INIT, 0, 0, (void*) vformat);
    /* We're done! */
    return 0;	
}

SDL_Rect **SDLVideoDriver::onListModes(_THIS, SDL_PixelFormat *format, Uint32 flags) {
    return (SDL_Rect **) -1;
}

SDL_Surface *SDLVideoDriver::onSetVideoMode(_THIS, SDL_Surface *current, int width, int height, int bpp, Uint32 flags) {
    if(self == NULL) {
        SDL_SetError("onSetVideoMode: self is NULL");
        return NULL;
    }

    // create bitmap where will sdl render pixels for android
    thiz->initBitmap(PIXEL_FORMAT_RGB_565, width, height);
	
    /* Set up the new mode framebuffer */
    current->flags = flags;
    current->w = thiz->mBitmap.width();
    current->h = thiz->mBitmap.height();
    current->pitch = current->w * (bpp / 8);
    current->pixels = thiz->mBitmap.getPixels();
	
    thiz->mListener->notify(SDL_NATIVE_VIDEO_SET_SURFACE, 0, 0, (void*) current);
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
	thiz->mListener->notify(SDL_NATIVE_VIDEO_ALLOC_HW_SURFACE, 0, 0, (void*) surface);
    return(-1);
}

int SDLVideoDriver::onLockHWSurface(_THIS, SDL_Surface *surface) {
	thiz->mListener->notify(SDL_NATIVE_VIDEO_LOCK_HW_SURFACE, 0, 0, (void*) surface);
    return 0;
}

void SDLVideoDriver::onUnlockHWSurface(_THIS, SDL_Surface *surface) {
	thiz->mListener->notify(SDL_NATIVE_VIDEO_UNLOCK_HW_SURFACE, 0, 0, (void*) surface);
}

void SDLVideoDriver::onFreeHWSurface(_THIS, SDL_Surface *surface) {
	thiz->mListener->notify(SDL_NATIVE_VIDEO_FREE_HW_SURFACE, 0, 0, (void*) surface);
}

/* on set window caption */
void SDLVideoDriver::onSetCaption(_THIS, const char *title, const char *icon) {
	thiz->mListener->notify(SDL_NATIVE_VIDEO_SET_CAPTION, 0, 0, (void*) title);
}

/* etc. */
void SDLVideoDriver::onUpdateRects(_THIS, int numrects, SDL_Rect *rects) {
    if(thiz->mBitmap.width() == 0 || thiz->mBitmap.height() == 0) {
        __android_log_print(ANDROID_LOG_ERROR, CLASS_PATH, "Bitmap is too small %ix%i",
                thiz->mBitmap.width(), thiz->mBitmap.height());
        return;
    }

	thiz->mListener->notify(SDL_NATIVE_VIDEO_UPDATE_RECTS, 0, 0, (void*) &thiz->mBitmap);
}

/* ANDROID driver bootstrap functions */
int SDLVideoDriver::onAvailable() {
    return 1;
}

void SDLVideoDriver::onDeleteDevice(SDL_VideoDevice *device) {
    thiz->mListener->notify(SDL_NATIVE_VIDEO_DELETE_DEVICE, 0, 0, (void*) device);
    SDL_free(thiz->device);
}

void SDLVideoDriver::onPumpEvents(_THIS) {
    thiz->mListener->notify(SDL_NATIVE_VIDEO_PUMP_EVENTS, 0, 0, NULL);
}

void SDLVideoDriver::onInitOSKeymap(_THIS) {
    //thiz->mListener->notify(SDL_NATIVE_VIDEO_INIT_OS_KEYMAP, 0, 0, NULL);
}

/* Opengl impl */
int SDLVideoDriver::GLLoadLibrary(_THIS, const char *path){
    __android_log_print(ANDROID_LOG_INFO, "SDL", "[STUB] GL_LoadLibrary\n");
    return 0;
}

void* SDLVideoDriver::GLGetProcAddress(_THIS, const char *proc){
    __android_log_print(ANDROID_LOG_INFO, "SDL", "[STUB] GL_GetProcAddress\n");
    return 0;
}

void SDLVideoDriver::GLUnloadLibrary(_THIS){
    __android_log_print(ANDROID_LOG_INFO, "SDL", "[STUB] GL_UnloadLibrary\n");
}

/*
int *Android_GL_GetVisual(_THIS, Display * display, int screen){
	__android_log_print(ANDROID_LOG_INFO, "SDL","[STUB] GL_GetVisual\n");
	return 0;
}
*/

SDL_GLContext SDLVideoDriver::GLCreateContext(_THIS, SDL_Window * window){
    //Android_CreateContext();
    return (void *) NULL;
}

int SDLVideoDriver::GLMakeCurrent(_THIS, SDL_Window * window, SDL_GLContext context){
    __android_log_print(ANDROID_LOG_INFO, "SDL", "[STUB] GL_MakeCurrent\n");
    return 0;
}

int SDLVideoDriver::GLSetSwapInterval(_THIS, int interval){
    __android_log_print(ANDROID_LOG_INFO, "SDL", "[STUB] GL_SetSwapInterval\n");
    return 0;
}

int SDLVideoDriver::GLGetSwapInterval(_THIS){
    __android_log_print(ANDROID_LOG_INFO, "SDL", "[STUB] GL_GetSwapInterval\n");
    return 0;
}

void SDLVideoDriver::GLSwapWindow(_THIS, SDL_Window * window){
    //Android_Render();
}

void SDLVideoDriver::GLDeleteContext(_THIS, SDL_GLContext context){
    __android_log_print(ANDROID_LOG_INFO, "SDL", "[STUB] GL_DeleteContext\n");
}

extern "C" {

VideoBootStrap ANDROID_bootstrap = {
    SDLVideoDriver::getDriverName(), SDLVideoDriver::getDriverDescription(),
    SDLVideoDriver::onAvailable, SDLVideoDriver::onCreateDevice
};

}//end extern C
