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

#define LOG_TAG "SDLPalette-JNI"

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

static const char* const kClassPathName = "android/sdl/SDLPalette";

extern jobject
android_sdl_SDLColor_create(SDL_Color* color);

static
jfieldID checkFieldId(JNIEnv* env, jfieldID fieldId)
{
	if (fieldId == NULL) {
		SDLRuntime::doThrow(env, "java/lang/RuntimeException", "SDLPalette field id is null");
    }
	return fieldId;
}

static 
SDL_Palette* getNativeStruct(JNIEnv* env, jobject thiz)
{
    return (SDL_Palette*)env->GetIntField(thiz, fields.mNativePointer);
}

jobject
android_sdl_SDLPalette_create(SDL_Palette* palette)
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
	
    env->SetIntField(obj, fields.mNativePointer, (jint)palette);
	
    return obj;
}

// This function gets some field IDs, which in turn causes class initialization.
// It is called from a static block, which won't run until the
// first time an instance of this class is used.
static void
android_sdl_SDLPalette_native_init(JNIEnv *env)
{
    LOGV("native_init");
	
    jclass clazz = env->FindClass(kClassPathName);
    if (clazz == NULL) {
        SDLRuntime::doThrow(env, "java/lang/RuntimeException", 
							"Can't find android/sdl/SDLPalette");
        return;
    }
	
    fields.mNativePointer = checkFieldId(env, env->GetFieldID(clazz, "mNativePointer", "I"));
}

static void
android_sdl_SDLPalette_native_finalize(JNIEnv *env, jobject thiz)
{
    LOGV("native_finalize");
    //android_sdl_SDLPalette_release(env, thiz);
}

// ----------------------------------------------------------------------------

static jobjectArray
android_sdl_SDLPalette_getColors(JNIEnv *env, jobject thiz)
{
    jclass clazz;
    SDL_Palette* palette;
    jobjectArray jcolors;

    clazz = env->FindClass("android/sdl/SDLColor");
    if (clazz == NULL) {
        SDLRuntime::doThrow(env, "java/lang/RuntimeException", 
                            "Can't find android/sdl/SDLColor");
        return NULL;
    }

    palette = getNativeStruct(env, thiz);
    jcolors = env->NewObjectArray(palette->ncolors, clazz, NULL);
    for(int i=0;i<palette->ncolors;i++) {
        jobject jcolor = android_sdl_SDLPalette_create(&palette[i]);
        env->SetObjectArrayElement(jcolors, i, jcolor);
    }
    return jcolors;
}

static JNINativeMethod gMethods[] = {
    {"native_init",         "()V",                              (void *)android_sdl_SDLPalette_native_init},
    {"native_finalize",     "()V",                              (void *)android_sdl_SDLPalette_native_finalize},
    {"getColors",           "()[Landroid/sdl/SDLColor;",        (void *)android_sdl_SDLPalette_getColors},
};

namespace android {
	
// This function only registers the native methods
int register_android_sdl_SDLPalette(JNIEnv *env)
{
    return SDLRuntime::registerNativeMethods(env,
                kClassPathName, gMethods, NELEM(gMethods));
}
	
} // end of android namespace
