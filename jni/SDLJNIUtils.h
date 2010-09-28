/*
 * Copyright (C) Petr Havlena
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

//

#ifndef _SDL_JNI_UTILS_H
#define _SDL_JNI_UTILS_H

#include <jni.h>

#include <SDL_androidvideo.h>

void sdlJniUtils_PixelFormat_ToJava(JNIEnv* env, jclass clazz, jobject obj, SDL_PixelFormat* pformat) {
	env->SetIntField(obj,
					 env->GetFieldID(clazz, "mBitsPerPixel", "I"),
					 (jint)0);
	env->SetIntField(obj,
					 env->GetFieldID(clazz, "mBytesPerPixel", "I"),
					 (jint)0);
	env->SetIntField(obj,
					 env->GetFieldID(clazz, "mRloss", "I"),
					 (jint)0);
	env->SetIntField(obj,
					 env->GetFieldID(clazz, "mGloss", "I"),
					 (jint)0);
	env->SetIntField(obj,
					 env->GetFieldID(clazz, "mBloss", "I"),
					 (jint)0);
	env->SetIntField(obj,
					 env->GetFieldID(clazz, "mAloss", "I"),
					 (jint)0);
	env->SetIntField(obj,
					 env->GetFieldID(clazz, "mRshift", "I"),
					 (jint)0);
	env->SetIntField(obj,
					 env->GetFieldID(clazz, "mGshift", "I"),
					 (jint)0);
	env->SetIntField(obj,
					 env->GetFieldID(clazz, "mBshift", "I"),
					 (jint)0);
	env->SetIntField(obj,
					 env->GetFieldID(clazz, "mAshift", "I"),
					 (jint)0);
	env->SetIntField(obj,
					 env->GetFieldID(clazz, "mRmask", "L"),
					 (jint)0);
	env->SetIntField(obj,
					 env->GetFieldID(clazz, "mGmask", "L"),
					 (jint)0);
	env->SetIntField(obj,
					 env->GetFieldID(clazz, "mBmask", "L"),
					 (jint)0);
	env->SetIntField(obj,
					 env->GetFieldID(clazz, "mAmask", "L"),
					 (jint)0);
};

void sdlJniUtils_PixelFormat_ToNative(JNIEnv* env, jclass clazz, jobject obj, SDL_PixelFormat* pformat) {
}

void sdlJniUtils_VideoDevice_ToNative(JNIEnv* env, jclass clazz, jobject obj, SDL_VideoDevice* device) {
};

void sdlJniUtils_VideoDevice_ToJava(JNIEnv* env, jclass clazz, jobject obj, SDL_VideoDevice* device) {
};

void sdlJniUtils_Surface_ToJava(JNIEnv* env, jclass clazz, jobject obj, SDL_Surface* surface) {
};

void sdlJniUtils_Surface_ToNative(JNIEnv* env, jclass clazz, jobject obj, SDL_Surface* surface) {
};

#endif
