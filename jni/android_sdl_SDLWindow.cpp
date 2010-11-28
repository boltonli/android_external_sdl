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
#include <SDL.h>
#include <utils/Log.h>

// ----------------------------------------------------------------------------

using namespace android;

// ----------------------------------------------------------------------------
struct fields_t {
    jfieldID    mNativePointer;
};
static fields_t fields;

static const char* const kClassPathName = "android/sdl/SDLWindow";

static
jfieldID checkFieldId(JNIEnv* env, jfieldID fieldId)
{
	if (fieldId == NULL) {
		SDLRuntime::doThrow(env, "java/lang/RuntimeException", "SDLColor field id is null");
    }
	return fieldId;
}


static 
SDL_Window* getNativeStruct(JNIEnv* env, jobject thiz)
{
    return (SDL_Window*)env->GetIntField(thiz, fields.mNativePointer);
}

jobject
android_sdl_SDLWindow_create(SDL_Window* win)
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
	
    env->SetIntField(obj, fields.mNativePointer, (jint)win);
	
    return obj;
}

// This function gets some field IDs, which in turn causes class initialization.
// It is called from a static block, which won't run until the
// first time an instance of this class is used.
static void
android_sdl_SDLWindow_native_init(JNIEnv *env)
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
android_sdl_SDLWindow_native_finalize(JNIEnv *env, jobject thiz)
{
    LOGV("native_finalize");
    //android_sdl_SDLWindow_release(env, thiz);
}

// ----------------------------------------------------------------------------

static jlong
android_sdl_SDLWindow_getId(JNIEnv *env, jobject thiz)
{
    SDL_Window* win = getNativeStruct(env, thiz);
    return (jlong)win->id;
}

static jlong
android_sdl_SDLWindow_getFlags(JNIEnv *env, jobject thiz)
{
    SDL_Window* win = getNativeStruct(env, thiz);
    return (jlong)win->flags;
}

static jint
android_sdl_SDLWindow_getX(JNIEnv *env, jobject thiz)
{
    SDL_Window* win = getNativeStruct(env, thiz);
    return (jint)win->x;
}

static jint
android_sdl_SDLWindow_getY(JNIEnv *env, jobject thiz)
{
    SDL_Window* win = getNativeStruct(env, thiz);
    return (jint)win->y;
}

static jint
android_sdl_SDLWindow_getW(JNIEnv *env, jobject thiz)
{
    SDL_Window* win = getNativeStruct(env, thiz);
    return (jint)win->w;
}

static jint
android_sdl_SDLWindow_getH(JNIEnv *env, jobject thiz)
{
    SDL_Window* win = getNativeStruct(env, thiz);
    return (jint)win->h;
}

static jobject
android_sdl_SDLWindow_getPrev(JNIEnv *env, jobject thiz)
{
    SDL_Window* win = getNativeStruct(env, thiz);
    return android_sdl_SDLWindow_create(win->prev);
}

static jobject
android_sdl_SDLWindow_getNext(JNIEnv *env, jobject thiz)
{
	SDL_Window* win = getNativeStruct(env, thiz);
    return android_sdl_SDLWindow_create(win->next);
}

static JNINativeMethod gMethods[] = {
    {"native_init",         "()V",                              (void *)android_sdl_SDLWindow_native_init},
    {"native_finalize",     "()V",                              (void *)android_sdl_SDLWindow_native_finalize},
    {"getId",               "()J",                              (void *)android_sdl_SDLWindow_getId},
	{"getFlags",            "()J",                              (void *)android_sdl_SDLWindow_getFlags},
	{"getX",                "()I",                              (void *)android_sdl_SDLWindow_getX},
	{"getY",                "()I",                              (void *)android_sdl_SDLWindow_getY},
	{"getW",                "()I",                              (void *)android_sdl_SDLWindow_getW},
	{"getH",                "()I",                              (void *)android_sdl_SDLWindow_getH},
	{"getPrev",             "()Landroid/sdl/SDLWindow;",        (void *)android_sdl_SDLWindow_getPrev},
	{"getNext",             "()Landroid/sdl/SDLWindow;",        (void *)android_sdl_SDLWindow_getNext},
};

namespace android {
	
// This function only registers the native methods
int register_android_sdl_SDLWindow(JNIEnv *env)
{
    return SDLRuntime::registerNativeMethods(env,
                kClassPathName, gMethods, NELEM(gMethods));
}
	
} // end of android namespace
