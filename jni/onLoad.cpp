#include "SDLRuntime.h"

#include <utils/Log.h>

#define LOG_TAG "SDL_onLoad"

namespace android {
extern int register_android_sdl_SDLVideo(JNIEnv* env);
extern int register_android_sdl_SDLPixelFormat(JNIEnv *env);
extern int register_android_sdl_SDLVideoDevice(JNIEnv *env);
extern int register_android_sdl_SDLSurface(JNIEnv *env);
};

using namespace android;

/**
 * Used by LoadClass to register native functions.
 */
jint JNI_OnLoad(JavaVM* vm, void* reserved)
{
	LOGV("loading . . .");
	
	SDLRuntime::setJavaVM(vm);
	JNIEnv* env = SDLRuntime::getJNIEnv();
	
	LOG_ASSERT(env, "Could not retrieve the env!");
	
	if(register_android_sdl_SDLVideo(env) != JNI_OK) {
		LOGE("can't load android_sdl_SDLVideo");
		return JNI_ERR;
	}
	if(register_android_sdl_SDLPixelFormat(env) != JNI_OK) {
		LOGE("can't load android_sdl_SDLPixelFormat");
		return JNI_ERR;
	}
	if(register_android_sdl_SDLVideoDevice(env) != JNI_OK) {
		LOGE("can't load android_sdl_SDLVideoDevice");
		return JNI_ERR;
	}
	if(register_android_sdl_SDLSurface(env) != JNI_OK) {
		LOGE("can't load android_sdl_SDLSurface");
		return JNI_ERR;
	}
	
	LOGV("loaded");
	
	return JNI_VERSION_1_4;
}
