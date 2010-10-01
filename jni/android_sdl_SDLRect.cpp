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

#define LOG_TAG "SDLRect-JNI"

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

static const char* const kClassPathName = "android/sdl/SDLRect";

static
jfieldID checkFieldId(JNIEnv* env, jfieldID fieldId)
{
	if (fieldId == NULL) {
		SDLRuntime::doThrow(env, "java/lang/RuntimeException", "SDLRect field id is null");
    }
	return fieldId;
}


static 
SDL_Rect* getNativeStruct(JNIEnv* env, jobject thiz)
{
    return (SDL_Rect*)env->GetIntField(thiz, fields.mNativePointer);
}

jobject
android_sdl_SDLRect_create(SDL_Rect* color)
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
	
    env->SetIntField(obj, fields.mNativePointer, (jint)color);
	
    return obj;
}

// This function gets some field IDs, which in turn causes class initialization.
// It is called from a static block, which won't run until the
// first time an instance of this class is used.
static void
android_sdl_SDLRect_native_init(JNIEnv *env)
{
    LOGV("native_init");
	
    jclass clazz = env->FindClass(kClassPathName);
    if (clazz == NULL) {
        SDLRuntime::doThrow(env, "java/lang/RuntimeException", 
							"Can't find android/sdl/SDLRect");
        return;
    }
	
    fields.mNativePointer = checkFieldId(env, env->GetFieldID(clazz, "mNativePointer", "I"));
}

static void
android_sdl_SDLRect_native_finalize(JNIEnv *env, jobject thiz)
{
    LOGV("native_finalize");
    //android_sdl_SDLRect_release(env, thiz);
}

// ----------------------------------------------------------------------------

static jint
android_sdl_SDLRect_getX(JNIEnv *env, jobject thiz)
{
    SDL_Rect* rect = getNativeStruct(env, thiz);
    return (jint)rect->x;
}

static void
android_sdl_SDLRect_setX(JNIEnv *env, jobject thiz, jint value)
{
    SDL_Rect* rect = getNativeStruct(env, thiz);
    rect->x = value;
}

static jint
android_sdl_SDLRect_getY(JNIEnv *env, jobject thiz)
{
    SDL_Rect* rect = getNativeStruct(env, thiz);
    return (jint)rect->y;
}

static void
android_sdl_SDLRect_setY(JNIEnv *env, jobject thiz, jint value)
{
    SDL_Rect* rect = getNativeStruct(env, thiz);
    rect->y = value;
}

static jint
android_sdl_SDLRect_getW(JNIEnv *env, jobject thiz)
{
    SDL_Rect* rect = getNativeStruct(env, thiz);
    return (jint)rect->w;
}

static void
android_sdl_SDLRect_setW(JNIEnv *env, jobject thiz, jint value)
{
    SDL_Rect* rect = getNativeStruct(env, thiz);
    rect->w = value;
}

static jint
android_sdl_SDLRect_getH(JNIEnv *env, jobject thiz)
{
    SDL_Rect* rect = getNativeStruct(env, thiz);
    return (jint)rect->h;
}

static void
android_sdl_SDLRect_setH(JNIEnv *env, jobject thiz, jint value)
{
    SDL_Rect* rect = getNativeStruct(env, thiz);
    rect->h = value;
}

static JNINativeMethod gMethods[] = {
    {"native_init",         "()V",                              (void *)android_sdl_SDLRect_native_init},
    {"native_finalize",     "()V",                              (void *)android_sdl_SDLRect_native_finalize},
    {"getX",                "()I",                              (void *)android_sdl_SDLRect_getX},
    {"getY",                "()I",                              (void *)android_sdl_SDLRect_getY},
    {"getW",                "()I",                              (void *)android_sdl_SDLRect_getW},
    {"getH",                "()I",                              (void *)android_sdl_SDLRect_getH},
    {"setX",                "(I)V",                             (void *)android_sdl_SDLRect_setX},
    {"setY",                "(I)V",                             (void *)android_sdl_SDLRect_setY},
    {"setW",                "(I)V",                             (void *)android_sdl_SDLRect_setW},
    {"setH",                "(I)V",                             (void *)android_sdl_SDLRect_setH},
};

namespace android {
	
// This function only registers the native methods
int register_android_sdl_SDLRect(JNIEnv *env)
{
    return SDLRuntime::registerNativeMethods(env,
                kClassPathName, gMethods, NELEM(gMethods));
}
	
} // end of android namespace

// KTHXBYE
