/*
**
** Copyright 2010, Petr Havlena
**
** Licensed under the Apache License, Version 2.0 (the "License");
** you may not use this file except in compliance with the License.
** You may obtain a copy of the License at
**
**     http://www.apache.org/licenses/LICENSE-2.0
**
** Unless required by applicable law or agreed to in writing, software
** distributed under the License is distributed on an "AS IS" BASIS,
** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
** See the License for the specific language governing permissions and
** limitations under the License.
*/

#define LOG_TAG "SDLVideoDevice-JNI"

#include "SDLRuntime.h"
#include <SDL_androidvideo.h>
#include <utils/Log.h>

// ----------------------------------------------------------------------------

using namespace android;

// ----------------------------------------------------------------------------
struct fields_t {
    jfieldID    mNativePointer;
};
static fields_t fields;

static const char* const kClassPathName = "android/sdl/SDLVideoDevice";

static
jfieldID checkFieldId(JNIEnv* env, jfieldID fieldId)
{
	if (fieldId == NULL) {
		SDLRuntime::doThrow(env, "java/lang/RuntimeException", "SDLVideoDevice field id is null");
    }
	return fieldId;
}

static
SDL_VideoDevice* getNativeStruct(JNIEnv* env, jobject thiz)
{
    return (SDL_VideoDevice*)env->GetIntField(thiz, fields.mNativePointer);
}

jobject
android_sdl_SDLVideoDevice_create(SDL_VideoDevice* device)
{
	jobject obj;
	JNIEnv* env = SDLRuntime::getJNIEnv();
	
	jclass clazz = env->FindClass(kClassPathName);
	if (clazz == NULL) {
		SDLRuntime::doThrow(env, "java/lang/RuntimeException", kClassPathName);
        return NULL;
    }
	
	if((obj = SDLRuntime::createObject(env, clazz)) == NULL) {
		return NULL;
	}
	
	env->SetIntField(obj, fields.mNativePointer, (jint)device);
	//env->SetIntField(obj, fields.mGamma, (jint)device->gamma);
	//env->SetObjectField(obj, fields.mName, (jstring)env->NewStringUTF(env, device->name));
	//env->SetObjectField(obj, fields.mWmTitle, (jstring)env->NewStringUTF(env, device->wm_title));
	//env->SetObjectField(obj, fields.mWmIcon, (jstring)env->NewStringUTF(env, device->wm_icon));
	
	return obj;
}

// This function gets some field IDs, which in turn causes class initialization.
// It is called from a static block, which won't run until the
// first time an instance of this class is used.
static void
android_sdl_SDLVideoDevice_native_init(JNIEnv *env)
{
    LOGV("native_init");
	
    jclass clazz = env->FindClass(kClassPathName);
    if (clazz == NULL) {
        SDLRuntime::doThrow(env, "java/lang/RuntimeException", 
							"Can't find android/sdl/SDLVideoDevice");
        return;
    }
	
    fields.mNativePointer = checkFieldId(env, env->GetFieldID(clazz, "mNativePointer", "I"));
}

static void
android_sdl_SDLVideoDevice_native_finalize(JNIEnv *env, jobject thiz)
{
    LOGV("native_finalize");
    //android_sdl_SDLVideoDevice_release(env, thiz);
}

//--------------------------------------------

static jstring
android_sdl_SDLVideoDevice_getName(JNIEnv *env, jobject thiz)
{
    SDL_VideoDevice* dev = getNativeStruct(env, thiz);
    return env->NewStringUTF(dev->name);
}

static jstring
android_sdl_SDLVideoDevice_getWmTitle(JNIEnv *env, jobject thiz)
{
#if SDL_BUILD_VERSION == 1
    SDL_VideoDevice* dev = getNativeStruct(env, thiz);
    return env->NewStringUTF(dev->wm_title);
#else
	return env->NewStringUTF("none");
#endif
}

static jstring
android_sdl_SDLVideoDevice_getWmIcon(JNIEnv *env, jobject thiz)
{
#if SDL_BUILD_VERSION == 1
    SDL_VideoDevice* dev = getNativeStruct(env, thiz);
    return env->NewStringUTF(dev->wm_icon);
#else
	return env->NewStringUTF("none");
#endif
}

static jint
android_sdl_SDLVideoDevice_getGamma(JNIEnv *env, jobject thiz)
{
#if SDL_BUILD_VERSION == 1
    SDL_VideoDevice* dev = getNativeStruct(env, thiz);
    return (jint)dev->gamma;
#else
	return -1;
#endif
}

static jint
android_sdl_SDLVideoDevice_getIs32bit(JNIEnv *env, jobject thiz)
{
    return 0;
}

static jint
android_sdl_SDLVideoDevice_getOffsetX(JNIEnv *env, jobject thiz)
{
    return 0;
}

static jint
android_sdl_SDLVideoDevice_getOffsetY(JNIEnv *env, jobject thiz)
{
    return 0;
}

static jint
android_sdl_SDLVideoDevice_getHandlesAnySize(JNIEnv *env, jobject thiz)
{
    return 0;
}

// ----------------------------------------------------------------------------


static JNINativeMethod gMethods[] = {
    {"native_init",         "()V",                              (void *)android_sdl_SDLVideoDevice_native_init},
    {"native_finalize",     "()V",                              (void *)android_sdl_SDLVideoDevice_native_finalize},
    {"getName",             "()Ljava/lang/String;",             (void *)android_sdl_SDLVideoDevice_getName},
    {"getWmTitle",          "()Ljava/lang/String;",             (void *)android_sdl_SDLVideoDevice_getWmTitle},
    {"getWmIcon",           "()Ljava/lang/String;",             (void *)android_sdl_SDLVideoDevice_getWmIcon},
    {"getGamma",            "()I",                              (void *)android_sdl_SDLVideoDevice_getGamma},
    {"getIs32bit",          "()I",                              (void *)android_sdl_SDLVideoDevice_getIs32bit},
    {"getOffsetX",          "()I",                              (void *)android_sdl_SDLVideoDevice_getOffsetX},
    {"getOffsetY",          "()I",                              (void *)android_sdl_SDLVideoDevice_getOffsetY},
    {"getHandlesAnySize",   "()I",                              (void *)android_sdl_SDLVideoDevice_getHandlesAnySize},
};

namespace android {
	
// This function only registers the native methods
int register_android_sdl_SDLVideoDevice(JNIEnv *env)
{
    return SDLRuntime::registerNativeMethods(env,
                kClassPathName, gMethods, NELEM(gMethods));
}
	
} // end of android namespace

// KTHXBYE
