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
    thiz->device->VideoQuit = SDLVideoDriver::onVideoQuit;
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

int SDLVideoDriver::onVideoInit(_THIS) {
    SDL_DisplayMode mode;

    /* Use a fake 32-bpp desktop mode */
    mode.format = SDL_PIXELFORMAT_RGB888;
    mode.w = iScreenWidth;
    mode.h = iScreenHeight;
    mode.refresh_rate = 0;
    mode.driverdata = NULL;
	
	thiz->mListener->notify(SDL_NATIVE_VIDEO_INIT, 0, 0, (void*) &mode);
	
    if (SDL_AddBasicVideoDisplay(&mode) < 0) {
        return -1;
    }
    SDL_AddRenderDriver(&self->displays[0], &Android_RenderDriver);
    SDL_zero(mode);
    SDL_AddDisplayMode(&self->displays[0], &mode);

    /* We're done! */
    return 0;	
}

void SDLVideoDriver::onVideoQuit(_THIS) {
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

/* Opengl impl */
int SDLVideoDriver::GLLoadLibrary(_THIS, const char *path){
    __android_log_print(ANDROID_LOG_INFO, "SDL", "[STUB] GL_LoadLibrary\n");
	thiz->mListener->notify(SDL_NATIVE_VIDEO_GL_LOAD_LIBRARY, 0, 0, (void*) path);
    return 0;
}

void* SDLVideoDriver::GLGetProcAddress(_THIS, const char *proc){
    __android_log_print(ANDROID_LOG_INFO, "SDL", "[STUB] GL_GetProcAddress\n");
	thiz->mListener->notify(SDL_NATIVE_VIDEO_GL_GET_PROC_ADDRESS, 0, 0, (void*) proc);
    return 0;
}

void SDLVideoDriver::GLUnloadLibrary(_THIS){
    __android_log_print(ANDROID_LOG_INFO, "SDL", "[STUB] GL_UnloadLibrary\n");
	thiz->mListener->notify(SDL_NATIVE_VIDEO_GL_UNLOAD_LIBRARY, 0, 0, (void*) NULL);
}

/*
int *Android_GL_GetVisual(_THIS, Display * display, int screen){
	__android_log_print(ANDROID_LOG_INFO, "SDL","[STUB] GL_GetVisual\n");
	return 0;
}
*/

SDL_GLContext SDLVideoDriver::GLCreateContext(_THIS, SDL_Window * window){
	thiz->mListener->notify(SDL_NATIVE_VIDEO_GL_CREATE_CONTEXT, 0, 0, (void*) window);
    return (void *) NULL;
}

int SDLVideoDriver::GLMakeCurrent(_THIS, SDL_Window * window, SDL_GLContext context){
    __android_log_print(ANDROID_LOG_INFO, "SDL", "[STUB] GL_MakeCurrent\n");
	thiz->mListener->notify(SDL_NATIVE_VIDEO_GL_MAKE_CURRENT, 0, 0, (void*) context);
    return 0;
}

int SDLVideoDriver::GLSetSwapInterval(_THIS, int interval){
    __android_log_print(ANDROID_LOG_INFO, "SDL", "[STUB] GL_SetSwapInterval\n");
	thiz->mListener->notify(SDL_NATIVE_VIDEO_GL_SET_SWAP_INTERVAL, 0, 0, (void*) interval);
    return 0;
}

int SDLVideoDriver::GLGetSwapInterval(_THIS){
    __android_log_print(ANDROID_LOG_INFO, "SDL", "[STUB] GL_GetSwapInterval\n");
	thiz->mListener->notify(SDL_NATIVE_VIDEO_GL_GET_SWAP_INTERVAL, 0, 0, (void*) NULL);
    return 0;
}

void SDLVideoDriver::GLSwapWindow(_THIS, SDL_Window * window){
    //Android_Render();
	thiz->mListener->notify(SDL_NATIVE_VIDEO_GL_SWAP_WINDOW, 0, 0, (void*) window);
}

void SDLVideoDriver::GLDeleteContext(_THIS, SDL_GLContext context){
    __android_log_print(ANDROID_LOG_INFO, "SDL", "[STUB] GL_DeleteContext\n");
	thiz->mListener->notify(SDL_NATIVE_VIDEO_GL_DELETE_CONTEXT, 0, 0, (void*) context);
}

extern "C" {

VideoBootStrap ANDROID_bootstrap = {
    SDLVideoDriver::getDriverName(), SDLVideoDriver::getDriverDescription(),
    SDLVideoDriver::onAvailable, SDLVideoDriver::onCreateDevice
};

}//end extern C
