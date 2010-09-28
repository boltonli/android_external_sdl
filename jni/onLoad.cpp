#include "SDLRuntime.h"

#include <utils/Log.h>

#define LOG_TAG "SDL_onLoad"

namespace android {
extern int register_android_sdl_SDLVideo(JNIEnv* env);
};

using namespace android;

/**
 * Used by LoadClass to register native functions.
 */
extern "C" jint JNI_OnLoad(JavaVM* vm, void* reserved)
{
	LOGV("loading . . .");
	
	SDLRuntime::setJavaVM(vm);
	JNIEnv* env = SDLRuntime::getJNIEnv();
	
	LOG_ASSERT(env, "Could not retrieve the env!");
	
	if(register_android_sdl_SDLVideo(env) != JNI_OK) {
		LOGE("can't load android_sdl_SDLVideo");
		return JNI_ERR;
	}
	
	LOGV("loaded");
	
	return JNI_VERSION_1_4;
}
