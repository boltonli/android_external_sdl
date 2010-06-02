#include "JNIHelp.h"
#include "jni.h"
#include "utils/Log.h"
#include "utils/misc.h"

namespace android {
extern int register_android_sdl_LibraryLoader(JNIEnv* env);
extern int register_android_sdl_SDLSurfaceView(JNIEnv* env);
};

#define TAG "SDL_onLoad"

using namespace android;

extern "C" jint JNI_OnLoad(JavaVM* vm, void* reserved)
{
    JNIEnv* env = NULL;
    jint result = -1;

    if (vm->GetEnv((void**) &env, JNI_VERSION_1_4) != JNI_OK) {
        LOGE("GetEnv failed!");
        return result;
    }
    LOG_ASSERT(env, "Could not retrieve the env!");

    __android_log_print(ANDROID_LOG_INFO, TAG, "loading . . .");

    if(register_android_sdl_LibraryLoader(env) != JNI_OK) {
        __android_log_print(ANDROID_LOG_ERROR, TAG, "can't load LibraryLoader");
        return JNI_ERR;
    }

    if(register_android_sdl_SDLSurfaceView(env) != JNI_OK) {
        __android_log_print(ANDROID_LOG_ERROR, TAG, "can't load SDLSurfaceView");
        return JNI_ERR;
    }

    __android_log_print(ANDROID_LOG_INFO, TAG, "loaded");

    return JNI_VERSION_1_4;
}
