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

#define LOG_TAG "SDLSurface-JNI"

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

static const char* const kClassPathName = "android/sdl/SDLSurface";

extern jobject
android_sdl_SDLPixelFormat_create(SDL_PixelFormat* pformat);

extern jobject
android_sdl_SDLRect_create(SDL_Rect* color);

static
jfieldID checkFieldId(JNIEnv* env, jfieldID fieldId)
{
    if (fieldId == NULL) {
	    SDLRuntime::doThrow(env, "java/lang/RuntimeException", "SDLSurface field id is null");
    }
    return fieldId;
}

SDL_Surface* 
android_sdl_SDLSurface_getNativeStruct(JNIEnv* env, jobject thiz)
{
    return (SDL_Surface*)env->GetIntField(thiz, fields.mNativePointer);
}

jobject
android_sdl_SDLSurface_create(SDL_Surface* surface)
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
	
    env->SetIntField(obj, fields.mNativePointer, (jint)surface);
	
    return obj;
}

// This function gets some field IDs, which in turn causes class initialization.
// It is called from a static block, which won't run until the
// first time an instance of this class is used.
static void
android_sdl_SDLSurface_native_init(JNIEnv *env)
{
    LOGV("native_init");
	
    jclass clazz = env->FindClass(kClassPathName);
    if (clazz == NULL) {
        SDLRuntime::doThrow(env, "java/lang/RuntimeException", 
							"Can't find android/sdl/SDLSurface");
        return;
    }
	
    fields.mNativePointer = checkFieldId(env, env->GetFieldID(clazz, "mNativePointer", "I"));
}

static void
android_sdl_SDLSurface_native_finalize(JNIEnv *env, jobject thiz)
{
    LOGV("native_finalize");
    //android_sdl_SDLSurface_release(env, thiz);
}

// ----------------------------------------------------------------------------
static jint
android_sdl_SDLSurface_getFlags(JNIEnv *env, jobject thiz)
{
    SDL_Surface* surface = android_sdl_SDLSurface_getNativeStruct(env, thiz);
    return (jint)surface->flags;
}

static jobject
android_sdl_SDLSurface_getPixelFormat(JNIEnv *env, jobject thiz)
{
    SDL_Surface* surface = android_sdl_SDLSurface_getNativeStruct(env, thiz);
    return android_sdl_SDLPixelFormat_create(surface->format);
}

static jint
android_sdl_SDLSurface_getW(JNIEnv *env, jobject thiz)
{
    SDL_Surface* surface = android_sdl_SDLSurface_getNativeStruct(env, thiz);
    return (jint)surface->w;
}

static jint
android_sdl_SDLSurface_getH(JNIEnv *env, jobject thiz)
{
    SDL_Surface* surface = android_sdl_SDLSurface_getNativeStruct(env, thiz);
    return (jint)surface->h;
}

static jint
android_sdl_SDLSurface_getOffset(JNIEnv *env, jobject thiz)
{
    SDL_Surface* surface = android_sdl_SDLSurface_getNativeStruct(env, thiz);
    return (jint)surface->offset;
}

static jint
android_sdl_SDLSurface_getPitch(JNIEnv *env, jobject thiz)
{
    SDL_Surface* surface = android_sdl_SDLSurface_getNativeStruct(env, thiz);
    return (jint)surface->pitch;
}

static jint
android_sdl_SDLSurface_getRefCount(JNIEnv *env, jobject thiz)
{
    SDL_Surface* surface = android_sdl_SDLSurface_getNativeStruct(env, thiz);
    return (jint)surface->refcount;
}

static jlong
android_sdl_SDLSurface_getFormatVersion(JNIEnv *env, jobject thiz)
{
    SDL_Surface* surface = android_sdl_SDLSurface_getNativeStruct(env, thiz);
    return (jlong)surface->format_version;
}

static jobject
android_sdl_SDLSurface_getClipRect(JNIEnv *env, jobject thiz)
{
    SDL_Surface* surface = android_sdl_SDLSurface_getNativeStruct(env, thiz);
    return android_sdl_SDLRect_create(&surface->clip_rect);
}

static JNINativeMethod gMethods[] = {
    {"native_init",         "()V",                              (void *)android_sdl_SDLSurface_native_init},
    {"native_finalize",     "()V",                              (void *)android_sdl_SDLSurface_native_finalize},
    {"getFlags",            "()I",                              (void *)android_sdl_SDLSurface_getFlags},
    {"getPixelFormat",      "()Landroid/sdl/SDLPixelFormat;",   (void *)android_sdl_SDLSurface_getPixelFormat},
    {"getW",                "()I",                              (void *)android_sdl_SDLSurface_getW},
    {"getH",                "()I",                              (void *)android_sdl_SDLSurface_getH},
    {"getPitch",            "()I",                              (void *)android_sdl_SDLSurface_getPitch},
    {"getOffset",           "()I",                              (void *)android_sdl_SDLSurface_getOffset},
    {"getClipRect",         "()Landroid/sdl/SDLRect;",          (void *)android_sdl_SDLSurface_getClipRect},
    {"getRefCount",         "()I",                              (void *)android_sdl_SDLSurface_getRefCount},
    {"getFormatVersion",    "()J",                              (void *)android_sdl_SDLSurface_getFormatVersion},
};

namespace android {
	
// This function only registers the native methods
int register_android_sdl_SDLSurface(JNIEnv *env)
{
    return SDLRuntime::registerNativeMethods(env,
                kClassPathName, gMethods, NELEM(gMethods));
}
	
} // end of android namespace
