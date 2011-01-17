#include "SDLRuntime.h"

#include <android/log.h>

#define LOG_TAG "SDL_onLoad"

namespace android {
extern int register_android_sdl_SDLVideo(JNIEnv* env);
extern int register_android_sdl_SDLPixelFormat(JNIEnv *env);
extern int register_android_sdl_SDLVideoDevice(JNIEnv *env);
extern int register_android_sdl_SDLSurface(JNIEnv *env);
extern int register_android_sdl_SDLColor(JNIEnv *env);
extern int register_android_sdl_SDLRect(JNIEnv *env);
extern int register_android_sdl_SDLPalette(JNIEnv *env);
extern int register_android_sdl_impl_SDLImpl(JNIEnv *env);
extern int register_android_sdl_events_SDLEvents(JNIEnv *env);
#if SDL_BUILD_VERSION == 2
extern int register_android_sdl_SDLWindow(JNIEnv *env);
extern int register_android_sdl_SDLDisplayMode(JNIEnv *env);
#endif
#if IN_NDK == true
extern int register_android_nio_utils(JNIEnv* env);
#endif
};

using namespace android;

/**
 * Used by LoadClass to register native functions.
 */
jint JNI_OnLoad(JavaVM* vm, void* reserved)
{
	__android_log_print(ANDROID_LOG_INFO, LOG_TAG, "loading . . .");
	
	SDLRuntime::setJavaVM(vm);
	JNIEnv* env = SDLRuntime::getJNIEnv();
	
	if(register_android_sdl_SDLVideo(env) != JNI_OK) {
             __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "can't load android_sdl_SDLVideo");
             return JNI_ERR;
	}
	if(register_android_sdl_SDLPixelFormat(env) != JNI_OK) {
             __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "can't load android_sdl_SDLPixelFormat");
             return JNI_ERR;
	}
	if(register_android_sdl_SDLVideoDevice(env) != JNI_OK) {
             __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "can't load android_sdl_SDLVideoDevice");
             return JNI_ERR;
	}
	if(register_android_sdl_SDLSurface(env) != JNI_OK) {
             __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "can't load android_sdl_SDLSurface");
             return JNI_ERR;
	}
        if(register_android_sdl_SDLColor(env) != JNI_OK) {
             __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "can't load android_sdl_SDLColor");
             return JNI_ERR;
	}
        if(register_android_sdl_SDLRect(env) != JNI_OK) {
             __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "can't load android_sdl_SDLRect");
             return JNI_ERR;
	}
	if(register_android_sdl_SDLPalette(env) != JNI_OK) {
             __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "can't load android_sdl_SDLPalette");
             return JNI_ERR;
	}
        if(register_android_sdl_impl_SDLImpl(env) != JNI_OK) {
             __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "can't load android_sdl_impl_SDLImpl");
             return JNI_ERR;
	}
	if(register_android_sdl_events_SDLEvents(env) != JNI_OK) {
            __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "can't load android_sdl_SDLEvents");
            return JNI_ERR;
	}
#if SDL_BUILD_VERSION == 2
	if(register_android_sdl_SDLWindow(env) != JNI_OK) {
            __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "can't load android_sdl_SDLWindow");
            return JNI_ERR;
	}
	if(register_android_sdl_SDLDisplayMode(env) != JNI_OK) {
            __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "can't load android_sdl_SDLDisplayMode"); 
            return JNI_ERR;
	}
#endif

#if IN_NDK == true
        if(register_android_nio_utils(env) != 0) {
            __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "can't load android_nio_utils");
            return JNI_ERR;
        }
#endif

	__android_log_print(ANDROID_LOG_INFO, LOG_TAG, "loaded");
	
	return JNI_VERSION_1_4;
}
