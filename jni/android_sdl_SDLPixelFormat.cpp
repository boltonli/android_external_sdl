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

static 
SDL_PixelFormat* getNativeStruct(JNIEnv* env, jobject thiz)
{
    return (SDL_PixelFormat*)env->GetIntField(thiz, fields.mNativePointer);
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
}

static void
android_sdl_SDLPixelFormat_native_finalize(JNIEnv *env, jobject thiz)
{
    LOGV("native_finalize");
    //android_sdl_SDLPixelFormat_release(env, thiz);
}

// -----------------------------------------------------------------------
// Getters and setters for native struct
// ------------------------------------------------------------------------
static void
android_sdl_SDLPixelFormat_setBitsPerPixel(JNIEnv *env, jobject thiz, jshort value)
{
    SDL_PixelFormat* pformat = getNativeStruct(env, thiz);
    pformat->BitsPerPixel = value;
}

static jshort
android_sdl_SDLPixelFormat_getBitsPerPixel(JNIEnv *env, jobject thiz)
{
    SDL_PixelFormat* pformat = getNativeStruct(env, thiz);
    return (jshort)pformat->BitsPerPixel;
}

static void
android_sdl_SDLPixelFormat_setBytesPerPixel(JNIEnv *env, jobject thiz, jshort value)
{
    SDL_PixelFormat* pformat = getNativeStruct(env, thiz);
    pformat->BytesPerPixel = value;
}

static jshort
android_sdl_SDLPixelFormat_getBytesPerPixel(JNIEnv *env, jobject thiz)
{
    SDL_PixelFormat* pformat = getNativeStruct(env, thiz);
    return (jshort)pformat->BytesPerPixel;
}

static void
android_sdl_SDLPixelFormat_setRloss(JNIEnv *env, jobject thiz, jshort value)
{
    SDL_PixelFormat* pformat = getNativeStruct(env, thiz);
    pformat->Rloss = value;
}

static jshort
android_sdl_SDLPixelFormat_getRloss(JNIEnv *env, jobject thiz)
{
    SDL_PixelFormat* pformat = getNativeStruct(env, thiz);
    return (jshort)pformat->Rloss;
}

static void
android_sdl_SDLPixelFormat_setGloss(JNIEnv *env, jobject thiz, jshort value)
{
    SDL_PixelFormat* pformat = getNativeStruct(env, thiz);
    pformat->Gloss = value;
}

static jshort
android_sdl_SDLPixelFormat_getGloss(JNIEnv *env, jobject thiz)
{
    SDL_PixelFormat* pformat = getNativeStruct(env, thiz);
    return (jshort)pformat->Gloss;
}

static void
android_sdl_SDLPixelFormat_setBloss(JNIEnv *env, jobject thiz, jshort value)
{
    SDL_PixelFormat* pformat = getNativeStruct(env, thiz);
    pformat->Bloss = value;
}

static jshort
android_sdl_SDLPixelFormat_getBloss(JNIEnv *env, jobject thiz)
{
    SDL_PixelFormat* pformat = getNativeStruct(env, thiz);
    return (jshort)pformat->Bloss;
}

static void
android_sdl_SDLPixelFormat_setAloss(JNIEnv *env, jobject thiz, jshort value)
{
    SDL_PixelFormat* pformat = getNativeStruct(env, thiz);
    pformat->Aloss = value;
}

static jshort
android_sdl_SDLPixelFormat_getAloss(JNIEnv *env, jobject thiz)
{
    SDL_PixelFormat* pformat = getNativeStruct(env, thiz);
    return (jshort)pformat->Aloss;
}

//--------------

static void
android_sdl_SDLPixelFormat_setRshift(JNIEnv *env, jobject thiz, jshort value)
{
    SDL_PixelFormat* pformat = getNativeStruct(env, thiz);
    pformat->Rshift = value;
}

static jshort
android_sdl_SDLPixelFormat_getRshift(JNIEnv *env, jobject thiz)
{
    SDL_PixelFormat* pformat = getNativeStruct(env, thiz);
    return (jshort)pformat->Rshift;
}

static void
android_sdl_SDLPixelFormat_setGshift(JNIEnv *env, jobject thiz, jshort value)
{
    SDL_PixelFormat* pformat = getNativeStruct(env, thiz);
    pformat->Gshift = value;
}

static jshort
android_sdl_SDLPixelFormat_getGshift(JNIEnv *env, jobject thiz)
{
    SDL_PixelFormat* pformat = getNativeStruct(env, thiz);
    return (jshort)pformat->Gshift;
}

static void
android_sdl_SDLPixelFormat_setBshift(JNIEnv *env, jobject thiz, jshort value)
{
    SDL_PixelFormat* pformat = getNativeStruct(env, thiz);
    pformat->Bshift = value;
}

static jshort
android_sdl_SDLPixelFormat_getBshift(JNIEnv *env, jobject thiz)
{
    SDL_PixelFormat* pformat = getNativeStruct(env, thiz);
    return (jshort)pformat->Bshift;
}

static void
android_sdl_SDLPixelFormat_setAshift(JNIEnv *env, jobject thiz, jshort value)
{
    SDL_PixelFormat* pformat = getNativeStruct(env, thiz);
    pformat->Ashift = value;
}

static jshort
android_sdl_SDLPixelFormat_getAshift(JNIEnv *env, jobject thiz)
{
    SDL_PixelFormat* pformat = getNativeStruct(env, thiz);
    return (jshort)pformat->Ashift;
}

//--------------

static void
android_sdl_SDLPixelFormat_setRmask(JNIEnv *env, jobject thiz, jlong value)
{
    SDL_PixelFormat* pformat = getNativeStruct(env, thiz);
    pformat->Rmask = value;
}

static jlong
android_sdl_SDLPixelFormat_getRmask(JNIEnv *env, jobject thiz)
{
    SDL_PixelFormat* pformat = getNativeStruct(env, thiz);
    return (jlong)pformat->Rmask;
}

static void
android_sdl_SDLPixelFormat_setGmask(JNIEnv *env, jobject thiz, jlong value)
{
    SDL_PixelFormat* pformat = getNativeStruct(env, thiz);
    pformat->Gmask = value;
}

static jlong
android_sdl_SDLPixelFormat_getGmask(JNIEnv *env, jobject thiz)
{
    SDL_PixelFormat* pformat = getNativeStruct(env, thiz);
    return (jlong)pformat->Gmask;
}

static void
android_sdl_SDLPixelFormat_setBmask(JNIEnv *env, jobject thiz, jlong value)
{
    SDL_PixelFormat* pformat = getNativeStruct(env, thiz);
    pformat->Bmask = value;
}

static jlong
android_sdl_SDLPixelFormat_getBmask(JNIEnv *env, jobject thiz)
{
    SDL_PixelFormat* pformat = getNativeStruct(env, thiz);
    return (jlong)pformat->Bmask;
}

static void
android_sdl_SDLPixelFormat_setAmask(JNIEnv *env, jobject thiz, jlong value)
{
    SDL_PixelFormat* pformat = getNativeStruct(env, thiz);
    pformat->Amask = value;
}

static jlong
android_sdl_SDLPixelFormat_getAmask(JNIEnv *env, jobject thiz)
{
    SDL_PixelFormat* pformat = getNativeStruct(env, thiz);
    return (jlong)pformat->Amask;
}
//----------------

static void
android_sdl_SDLPixelFormat_setColorKey(JNIEnv *env, jobject thiz, jlong value)
{
    //SDL_PixelFormat* pformat = getNativeStruct(env, thiz);
    //pformat->Rmask = value;
}

static jlong
android_sdl_SDLPixelFormat_getColorKey(JNIEnv *env, jobject thiz)
{
//    SDL_PixelFormat* pformat = getNativeStruct(env, thiz);
    return (jlong)0;
}

static void
android_sdl_SDLPixelFormat_setAlpha(JNIEnv *env, jobject thiz, jshort value)
{
    //SDL_PixelFormat* pformat = getNativeStruct(env, thiz);
    //pformat->Rmask = value;
}

static jshort
android_sdl_SDLPixelFormat_getAlpha(JNIEnv *env, jobject thiz)
{
//    SDL_PixelFormat* pformat = getNativeStruct(env, thiz);
    return (jshort)0;
}


// ----------------------------------------------------------------------------
/*
Type     Chararacter 
boolean      Z 
byte         B 
char         C 
double       D 
float        F 
int          I 
long         J 
object       L 
short        S 
void         V 
array        [ 
*/
static JNINativeMethod gMethods[] = {
    {"native_init",         "()V",                      (void *)android_sdl_SDLPixelFormat_native_init},
    {"native_finalize",     "()V",                      (void *)android_sdl_SDLPixelFormat_native_finalize},
    {"setBitsPerPixel",     "(S)V",                     (void *)android_sdl_SDLPixelFormat_setBitsPerPixel},
    {"getBitsPerPixel",     "()S",                      (void *)android_sdl_SDLPixelFormat_getBitsPerPixel},
    {"setBytesPerPixel",    "(S)V",                     (void *)android_sdl_SDLPixelFormat_setBytesPerPixel},
    {"getBytesPerPixel",    "()S",                	(void *)android_sdl_SDLPixelFormat_getBytesPerPixel},
    {"setRloss",            "(S)V",                     (void *)android_sdl_SDLPixelFormat_setRloss},
    {"getRloss",            "()S",                      (void *)android_sdl_SDLPixelFormat_getRloss},
    {"setGloss",            "(S)V",                     (void *)android_sdl_SDLPixelFormat_setGloss},
    {"getGloss",            "()S",                      (void *)android_sdl_SDLPixelFormat_getGloss},
    {"setBloss",            "(S)V",                     (void *)android_sdl_SDLPixelFormat_setBloss},
    {"getBloss",            "()S",                      (void *)android_sdl_SDLPixelFormat_getBloss},
    {"setAloss",            "(S)V",                     (void *)android_sdl_SDLPixelFormat_setAloss},
    {"getAloss",            "()S",                      (void *)android_sdl_SDLPixelFormat_getAloss},
    {"setRshift",           "(S)V",                     (void *)android_sdl_SDLPixelFormat_setRshift},
    {"getRshift",           "()S",                      (void *)android_sdl_SDLPixelFormat_getRshift},
    {"setGshift",           "(S)V",                     (void *)android_sdl_SDLPixelFormat_setGshift},
    {"getGshift",           "()S",                      (void *)android_sdl_SDLPixelFormat_getGshift},
    {"setBshift",           "(S)V",                     (void *)android_sdl_SDLPixelFormat_setBshift},
    {"getBshift",           "()S",                      (void *)android_sdl_SDLPixelFormat_getBshift},
    {"setAshift",           "(S)V",                     (void *)android_sdl_SDLPixelFormat_setAshift},
    {"getAshift",           "()S",                      (void *)android_sdl_SDLPixelFormat_getAshift},
    {"setRmask",            "(J)V",                     (void *)android_sdl_SDLPixelFormat_setRmask},
    {"getRmask",            "()J",                      (void *)android_sdl_SDLPixelFormat_getRmask},
    {"setGmask",            "(J)V",                     (void *)android_sdl_SDLPixelFormat_setGmask},
    {"getGmask",            "()J",                      (void *)android_sdl_SDLPixelFormat_getGmask},
    {"setBmask",            "(J)V",                     (void *)android_sdl_SDLPixelFormat_setBmask},
    {"getBmask",            "()J",                      (void *)android_sdl_SDLPixelFormat_getBmask},
    {"setAmask",            "(J)V",                     (void *)android_sdl_SDLPixelFormat_setAmask},
    {"getAmask",            "()J",                      (void *)android_sdl_SDLPixelFormat_getAmask},
    {"setColorKey",         "(J)V",                     (void *)android_sdl_SDLPixelFormat_setColorKey},
    {"getColorKey",         "()J",                      (void *)android_sdl_SDLPixelFormat_getColorKey},
    {"setAlpha",            "(S)V",                     (void *)android_sdl_SDLPixelFormat_setAlpha},
    {"getAlpha",            "()S",                      (void *)android_sdl_SDLPixelFormat_getAlpha},
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
