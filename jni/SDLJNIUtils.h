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

/*
 B=byte
 C=char
 D=double
 F=float
 I=int
 J=long
 S=short
 V=void
 Z=boolean
 Lfully-qualified-class=fully qualified class
 [type=array of type>
 (argument types)return type=method type. If no arguments, use empty argument types: (). If return type is void (or constructor) use (argument types)V.
 */

void sdlJniUtils_PixelFormat_ToJava(JNIEnv* env, jclass clazz, jobject obj, SDL_PixelFormat* pformat) {
	env->SetShortField(obj,
					 env->GetFieldID(clazz, "mBitsPerPixel", "S"),
					 (jshort)pformat->BitsPerPixel);
	env->SetShortField(obj,
					 env->GetFieldID(clazz, "mBytesPerPixel", "S"),
					 (jshort)pformat->BytesPerPixel);
	env->SetShortField(obj,
					 env->GetFieldID(clazz, "mRloss", "S"),
					 (jshort)pformat->Rloss);
	env->SetShortField(obj,
					 env->GetFieldID(clazz, "mGloss", "S"),
					 (jshort)pformat->Gloss);
	env->SetShortField(obj,
					 env->GetFieldID(clazz, "mBloss", "S"),
					 (jshort)pformat->Bloss);
	env->SetShortField(obj,
					 env->GetFieldID(clazz, "mAloss", "S"),
					 (jshort)pformat->Aloss);
	env->SetShortField(obj,
					 env->GetFieldID(clazz, "mRshift", "S"),
					 (jshort)pformat->Rshift);
	env->SetShortField(obj,
					 env->GetFieldID(clazz, "mGshift", "S"),
					 (jshort)pformat->Gshift);
	env->SetShortField(obj,
					 env->GetFieldID(clazz, "mBshift", "S"),
					 (jshort)pformat->Bshift);
	env->SetShortField(obj,
					 env->GetFieldID(clazz, "mAshift", "S"),
					 (jshort)pformat->Ashift);
	env->SetLongField(obj,
					 env->GetFieldID(clazz, "mRmask", "J"),
					 (jlong)pformat->Rmask);
	env->SetLongField(obj,
					 env->GetFieldID(clazz, "mGmask", "J"),
					 (jlong)pformat->Gmask);
	env->SetLongField(obj,
					 env->GetFieldID(clazz, "mBmask", "J"),
					 (jlong)pformat->Bmask);
	env->SetLongField(obj,
					 env->GetFieldID(clazz, "mAmask", "J"),
					 (jlong)pformat->Amask);
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
