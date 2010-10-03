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

#define LOG_TAG "SDLEvents-JNI"

#include "SDLRuntime.h"
#include <SDL_androidvideo.h>
#include <SDL.h>
#include <SDL_video.h>
#include <utils/Log.h>

// ----------------------------------------------------------------------------

using namespace android;

// ----------------------------------------------------------------------------
struct fields_t {
    jfieldID    scancode;
	jfieldID    sym;
	jfieldID    mod;
	jfieldID    unicode;
};
static fields_t fields;

static const char* const kClassPathName = "android/sdl/events/SDLEvents";

static
jfieldID checkFieldId(JNIEnv* env, jfieldID fieldId)
{
	if (fieldId == NULL) {
		SDLRuntime::doThrow(env, "java/lang/RuntimeException", "SDLEvents field id is null");
    }
	return fieldId;
}
// ----------------------------------------------------------------------------

static jint
android_sdl_events_SDLEvents_MouseMotion(JNIEnv *env, jobject thiz, 
										 jshort buttonstate, 
										 jint relative, 
										 jint x, 
										 jint y)
{
    return SDL_PrivateMouseMotion((Uint8) buttonstate, relative,
								  (Sint16) x, (Sint16) y);
}

static jint
android_sdl_events_SDLEvents_MouseButton(JNIEnv *env, jobject thiz,
										 jshort state, 
										 jshort button, 
										 jint x, 
										 jint y)
{
    return SDL_PrivateMouseButton((Uint8) state, (Uint8) button,(Sint16) x,(Sint16) y);
}

static jint
android_sdl_events_SDLEvents_Keyboard(JNIEnv *env, jobject thiz,
									  jshort state, 
									  jobject key)
{
	SDL_keysym keysym;
	
    /* Set the keysym information */
    keysym.scancode = env->GetIntField(key, fields.scancode);;
    keysym.sym = (SDLKey)env->GetIntField(key, fields.sym);
    keysym.mod = (SDLMod)env->GetIntField(key, fields.mod);
	
    /* If UNICODE is on, get the UNICODE value for the key */
    keysym.unicode = env->GetIntField(key, fields.unicode);
	
    return SDL_PrivateKeyboard((Uint8)state, &keysym);
}

//(SDL_Color*)env->GetIntField(thiz, fields.mNativePointer);

static void
android_sdl_events_SDLEvents_native_init(JNIEnv *env)
{
    LOGV("native_init");
	
    jclass clazz = env->FindClass("android/sdl/events/SDLKeySym");
    if (clazz == NULL) {
        SDLRuntime::doThrow(env, "java/lang/RuntimeException", 
							"Can't find android/sdl/events/SDLKeySym");
        return;
    }
	
    fields.scancode = checkFieldId(env, env->GetFieldID(clazz, "scancode", "I"));
	fields.sym = checkFieldId(env, env->GetFieldID(clazz, "sym", "I"));
	fields.mod = checkFieldId(env, env->GetFieldID(clazz, "mod", "I"));
	fields.unicode = checkFieldId(env, env->GetFieldID(clazz, "unicode", "I"));
}

static JNINativeMethod gMethods[] = {
	{"native_init",     "()V",                                  (void *)android_sdl_events_SDLEvents_native_init},
    {"MouseMotion",     "(SIII)I",                              (void *)android_sdl_events_SDLEvents_MouseMotion},
    {"MouseButton",     "(SSII)I",                              (void *)android_sdl_events_SDLEvents_MouseButton},
    {"Keyboard",        "(SLandroid/sdl/events/SDLKeySym;)I",   (void *)android_sdl_events_SDLEvents_Keyboard},
};

namespace android {
	
// This function only registers the native methods
int register_android_sdl_events_SDLEvents(JNIEnv *env)
{
    return SDLRuntime::registerNativeMethods(env,
                kClassPathName, gMethods, NELEM(gMethods));
}
	
} // end of android namespace