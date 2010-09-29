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

#define LOG_TAG "SDLPixelFormat-JNI"

#include "SDLRuntime.h"
#include <SDL_androidvideo.h>
#include <utils/Log.h>

// ----------------------------------------------------------------------------

using namespace android;

// ----------------------------------------------------------------------------
struct fields_t {
    jfieldID    mNativePointer;
	jfieldID    mBitsPerPixel;
	jfieldID    mBytesPerPixel;
	jfieldID    mRloss;
	jfieldID    mGloss;
	jfieldID    mBloss;
	jfieldID    mAloss;
	jfieldID    mRshift;
	jfieldID    mGshift;
	jfieldID    mBshift;
	jfieldID    mAshift;
	jfieldID    mRmask;
	jfieldID    mGmask;
	jfieldID    mBmask;
	jfieldID    mAmask;
};
static fields_t fields;

static const char* const kClassPathName = "android/sdl/SDLPixelFormat";

static
jfieldID checkFieldId(JNIEnv* env, jfieldID fieldId)
{
	if (fieldId == NULL) {
		SDLRuntime::doThrow(env, "java/lang/RuntimeException", "SDLPixelFormat field id is null");
    }
	return fieldId;
}

jobject
android_sdl_SDLPixelFormat_create(SDL_PixelFormat* pformat)
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
	
	env->SetIntField(obj, fields.mNativePointer, (jint)pformat);
	env->SetShortField(obj, fields.mBitsPerPixel, (jshort)pformat->BitsPerPixel);
	env->SetShortField(obj, fields.mBytesPerPixel, (jshort)pformat->BytesPerPixel);
	env->SetShortField(obj, fields.mRloss, (jshort)pformat->Rloss);
	env->SetShortField(obj, fields.mGloss, (jshort)pformat->Gloss);
	env->SetShortField(obj, fields.mBloss, (jshort)pformat->Bloss);
	env->SetShortField(obj, fields.mAloss, (jshort)pformat->Aloss);
	env->SetShortField(obj, fields.mRshift, (jshort)pformat->Rshift);
	env->SetShortField(obj, fields.mGshift, (jshort)pformat->Gshift);
	env->SetShortField(obj, fields.mBshift, (jshort)pformat->Bshift);
	env->SetShortField(obj, fields.mAshift, (jshort)pformat->Ashift);
	env->SetLongField(obj, fields.mRmask, (jlong)pformat->Rmask);
	env->SetLongField(obj, fields.mGmask, (jlong)pformat->Gmask);
	env->SetLongField(obj, fields.mBmask, (jlong)pformat->Bmask);
	env->SetLongField(obj, fields.mAmask, (jlong)pformat->Amask);
	return obj;
}

// This function gets some field IDs, which in turn causes class initialization.
// It is called from a static block, which won't run until the
// first time an instance of this class is used.
static void
android_sdl_SDLPixelFormat_native_init(JNIEnv *env)
{
	LOGV("native_init");
	
    jclass clazz = env->FindClass(kClassPathName);
    if (clazz == NULL) {
        SDLRuntime::doThrow(env, "java/lang/RuntimeException", 
							"Can't find android/sdl/SDLPixelFormat");
        return;
    }
	
	fields.mNativePointer = checkFieldId(env, env->GetFieldID(clazz, "mNativePointer", "I"));
	fields.mBitsPerPixel = checkFieldId(env, env->GetFieldID(clazz, "mBitsPerPixel", "S"));
	fields.mBytesPerPixel = checkFieldId(env, env->GetFieldID(clazz, "mBytesPerPixel", "S"));
	fields.mRloss = checkFieldId(env, env->GetFieldID(clazz, "mRloss", "S"));
	fields.mGloss = checkFieldId(env, env->GetFieldID(clazz, "mGloss", "S"));
	fields.mBloss = checkFieldId(env, env->GetFieldID(clazz, "mBloss", "S"));
	fields.mAloss = checkFieldId(env, env->GetFieldID(clazz, "mAloss", "S"));
	fields.mRshift = checkFieldId(env, env->GetFieldID(clazz, "mRshift", "S"));
	fields.mGshift = checkFieldId(env, env->GetFieldID(clazz, "mGshift", "S"));
	fields.mBshift = checkFieldId(env, env->GetFieldID(clazz, "mBshift", "S"));
	fields.mAshift = checkFieldId(env, env->GetFieldID(clazz, "mAshift", "S"));
	fields.mRmask = checkFieldId(env, env->GetFieldID(clazz, "mRmask", "J"));
	fields.mGmask = checkFieldId(env, env->GetFieldID(clazz, "mGmask", "J"));
	fields.mBmask = checkFieldId(env, env->GetFieldID(clazz, "mBmask", "J"));
	fields.mAmask = checkFieldId(env, env->GetFieldID(clazz, "mAmask", "J"));
}

static void
android_sdl_SDLPixelFormat_native_finalize(JNIEnv *env, jobject thiz)
{
    LOGV("native_finalize");
    //android_sdl_SDLPixelFormat_release(env, thiz);
}

// ----------------------------------------------------------------------------

static JNINativeMethod gMethods[] = {
    {"native_init",         "()V",                              (void *)android_sdl_SDLPixelFormat_native_init},
    {"native_finalize",     "()V",                              (void *)android_sdl_SDLPixelFormat_native_finalize},
};

namespace android {
	
// This function only registers the native methods
int register_android_sdl_SDLPixelFormat(JNIEnv *env)
{
    return SDLRuntime::registerNativeMethods(env,
                kClassPathName, gMethods, NELEM(gMethods));
}
	
} // end of android namespace

// KTHXBYE
