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
	jfieldID    mName;
	jfieldID    mGamma;
	jfieldID    mIs32bit;
	jfieldID    mWmTitle;
	jfieldID    mWmIcon;
	jfieldID    mOffsetX;
	jfieldID    mOffsetY;
	jfieldID    mHandlesAnySize;
};
static fields_t fields;

static const char* const kClassPathName = "android/sdl/SDLSurface";

static
jfieldID checkFieldId(JNIEnv* env, jfieldID fieldId)
{
	if (fieldId == NULL) {
		SDLRuntime::doThrow(env, "java/lang/RuntimeException", "SDLSurface field id is null");
    }
	return fieldId;
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

static JNINativeMethod gMethods[] = {
    {"native_init",         "()V",                              (void *)android_sdl_SDLSurface_native_init},
    {"native_finalize",     "()V",                              (void *)android_sdl_SDLSurface_native_finalize},
};

namespace android {
	
// This function only registers the native methods
int register_android_sdl_SDLSurface(JNIEnv *env)
{
    return SDLRuntime::registerNativeMethods(env,
                kClassPathName, gMethods, NELEM(gMethods));
}
	
} // end of android namespace

// KTHXBYE
