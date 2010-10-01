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

#define LOG_TAG "SDLColor-JNI"

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

static const char* const kClassPathName = "android/sdl/SDLColor";

static
jfieldID checkFieldId(JNIEnv* env, jfieldID fieldId)
{
	if (fieldId == NULL) {
		SDLRuntime::doThrow(env, "java/lang/RuntimeException", "SDLColor field id is null");
    }
	return fieldId;
}


static 
SDL_Color* getNativeStruct(JNIEnv* env, jobject thiz)
{
    return (SDL_Color*)env->GetIntField(thiz, fields.mNativePointer);
}

jobject
android_sdl_SDLColor_create(SDL_Color* color)
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
android_sdl_SDLColor_native_init(JNIEnv *env)
{
    LOGV("native_init");
	
    jclass clazz = env->FindClass(kClassPathName);
    if (clazz == NULL) {
        SDLRuntime::doThrow(env, "java/lang/RuntimeException", 
							"Can't find android/sdl/SDLColor");
        return;
    }
	
    fields.mNativePointer = checkFieldId(env, env->GetFieldID(clazz, "mNativePointer", "I"));
}

static void
android_sdl_SDLColor_native_finalize(JNIEnv *env, jobject thiz)
{
    LOGV("native_finalize");
    //android_sdl_SDLColor_release(env, thiz);
}

// ----------------------------------------------------------------------------

static jshort
android_sdl_SDLColor_getR(JNIEnv *env, jobject thiz)
{
    SDL_Color* color = getNativeStruct(env, thiz);
    return (jshort)color->r;
}

static void
android_sdl_SDLColor_setR(JNIEnv *env, jobject thiz, jshort value)
{
    SDL_Color* color = getNativeStruct(env, thiz);
    color->r = value;
}

static jshort
android_sdl_SDLColor_getG(JNIEnv *env, jobject thiz)
{
    SDL_Color* color = getNativeStruct(env, thiz);
    return (jshort)color->g;
}

static void
android_sdl_SDLColor_setG(JNIEnv *env, jobject thiz, jshort value)
{
    SDL_Color* color = getNativeStruct(env, thiz);
    color->g = value;
}

static jshort
android_sdl_SDLColor_getB(JNIEnv *env, jobject thiz)
{
    SDL_Color* color = getNativeStruct(env, thiz);
    return (jshort)color->b;
}

static void
android_sdl_SDLColor_setB(JNIEnv *env, jobject thiz, jshort value)
{
    SDL_Color* color = getNativeStruct(env, thiz);
    color->b = value;
}

static jshort
android_sdl_SDLColor_getUnused(JNIEnv *env, jobject thiz)
{
    SDL_Color* color = getNativeStruct(env, thiz);
    return (jshort)color->unused;
}

static void
android_sdl_SDLColor_setUnused(JNIEnv *env, jobject thiz, jshort value)
{
    SDL_Color* color = getNativeStruct(env, thiz);
    color->unused = value;
}

static JNINativeMethod gMethods[] = {
    {"native_init",         "()V",                              (void *)android_sdl_SDLColor_native_init},
    {"native_finalize",     "()V",                              (void *)android_sdl_SDLColor_native_finalize},
    {"getR",                "()S",                              (void *)android_sdl_SDLColor_getR},
    {"getG",                "()S",                              (void *)android_sdl_SDLColor_getG},
    {"getB",                "()S",                              (void *)android_sdl_SDLColor_getB},
    {"getUnused",           "()S",                              (void *)android_sdl_SDLColor_getUnused},
    {"setR",                "(S)V",                             (void *)android_sdl_SDLColor_setR},
    {"setG",                "(S)V",                             (void *)android_sdl_SDLColor_setG},
    {"setB",                "(S)V",                             (void *)android_sdl_SDLColor_setB},
    {"setUnused",           "(S)V",                             (void *)android_sdl_SDLColor_setUnused},
};

namespace android {
	
// This function only registers the native methods
int register_android_sdl_SDLColor(JNIEnv *env)
{
    return SDLRuntime::registerNativeMethods(env,
                kClassPathName, gMethods, NELEM(gMethods));
}
	
} // end of android namespace
