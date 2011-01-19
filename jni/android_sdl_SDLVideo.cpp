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

#define LOG_TAG "SDLImpl-JNI"

#include "SDLRuntime.h"
#include <SDL_androidvideo.h>

#include <android/log.h>
#include <android/bitmap.h>
#include <android/android_nio_utils.h>

// ----------------------------------------------------------------------------

using namespace android;

// ----------------------------------------------------------------------------

struct fields_t {
    /* actually in android.view.Surface XXX */
    jfieldID    surface_native;
    jfieldID    mNativePointer;
    jmethodID   post_event;
};
static fields_t fields;

static const char* const kClassPathName = "android/sdl/SDLVideo";

//-- helper methods for creating java classes representing SDL native structs

extern jobject
android_sdl_SDLRect_create(SDL_Rect* rect);

extern jobject
android_sdl_SDLPixelFormat_create(SDL_PixelFormat* pformat);

extern jobject
android_sdl_SDLVideoDevice_create(SDL_VideoDevice* device);

extern jobject
android_sdl_SDLSurface_create(SDL_Surface* surface);

extern SDL_Surface* 
android_sdl_SDLSurface_getNativeStruct(JNIEnv* env, jobject thiz);

#if SDL_BUILD_VERSION == 2
extern jobject
android_sdl_SDLWindow_create(SDL_Window* win);

extern jobject
android_sdl_SDLDisplayMode_create(SDL_DisplayMode* mode);
#endif

// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// ref-counted object for callbacks
class JNISDLVideoDriverListener: public SDLVideoDriverListener
{
public:
    JNISDLVideoDriverListener(JNIEnv* env, jobject thiz, jobject weak_thiz, jobject surface);
    ~JNISDLVideoDriverListener();
    void                    notify(int what, int arg1, int arg2, void* data);
//    bool                    copyPixelsToJavaBuffer(JNIEnv* env, jobject jbuffer, jint size);
    bool                    copyPixelsToJavaBitmap(JNIEnv* env, jobject jsurface, jobject jbitmap);
private:
    jclass                  mClass;     // Reference to MediaPlayer class
    jobject                 mObject;    // Weak ref to MediaPlayer Java object to call on
};

JNISDLVideoDriverListener::JNISDLVideoDriverListener(JNIEnv* env, jobject thiz, jobject weak_thiz, jobject surface)
{
    // Hold onto the MediaPlayer class for use in calling the static method
    // that posts events to the application thread.
    jclass clazz = env->GetObjectClass(thiz);
    if (clazz == NULL) {
        __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "Can't find android/sdl/SDLVideo");
		SDLRuntime::doThrow(env, "java/lang/Exception", NULL);
        return;
    }
    mClass = (jclass)env->NewGlobalRef(clazz);
	
    // We use a weak reference so the MediaPlayer object can be garbage collected.
    // The reference is only used as a proxy for callbacks.
    mObject = env->NewGlobalRef(weak_thiz);
}

JNISDLVideoDriverListener::~JNISDLVideoDriverListener()
{
    // remove global references
    JNIEnv *env = SDLRuntime::getJNIEnv();
    env->DeleteGlobalRef(mObject);
    env->DeleteGlobalRef(mClass);
}

// callback from libsdl which notify us about situation in sdl video driver
void JNISDLVideoDriverListener::notify(int what, int arg1, int arg2, void* data)
{
    JNIEnv *env = NULL;
    jobject obj = NULL;
	
#if SDL_BUILD_VERSION == 1 && IN_NDK != true
    if (what == SDL_NATIVE_VIDEO_UPDATE_RECTS) {
        updateScreen((SkBitmap*) data);
        return;
    }
#endif

    env = SDLRuntime::getJNIEnv();
	
    // we create java representation of native structs
    switch (what) {
        case SDL_NATIVE_VIDEO_CREATE_DEVICE:
        case SDL_NATIVE_VIDEO_DELETE_DEVICE:
            obj = android_sdl_SDLVideoDevice_create((SDL_VideoDevice*) data);
            break;
        case SDL_NATIVE_VIDEO_INIT:
#if SDL_BUILD_VERSION == 1
            obj = android_sdl_SDLPixelFormat_create((SDL_PixelFormat*) data);
#else
            obj = android_sdl_SDLDisplayMode_create((SDL_DisplayMode*) data);
#endif
            break;
        case SDL_NATIVE_VIDEO_SET_SURFACE:
//        case SDL_NATIVE_VIDEO_UPDATE_RECTS:
            obj = android_sdl_SDLSurface_create((SDL_Surface*) data);
            break;
#if SDL_BUILD_VERSION == 1
        case SDL_NATIVE_VIDEO_SET_CAPTION:
            obj = (jobject)env->NewStringUTF((const char *)data);
            break;
#endif
#if SDL_BUILD_VERSION == 2
//      case SDL_NATIVE_VIDEO_GL_SWAP_WINDOW:
        case SDL_NATIVE_VIDEO_GL_CREATE_CONTEXT:
            obj = android_sdl_SDLWindow_create((SDL_Window*)data);
            break;
#endif
/*
        case SDL_NATIVE_VIDEO_UPDATE_RECTS:
            SDL_RECTS *rectangles = (SDL_RECTS *) data;
            obj = (jobjectArray) env->NewObjectArray(rectangles->numrects, 
                env->FindClass("android/sdl/SDLRect"), NULL);

            for(int i=0;i<rectangles->numrects;i++) {
                env->SetObjectArrayElement((jobjectArray) obj, i, 
                    android_sdl_SDLRect_create(&rectangles->rects[i]));
            }
            break;
*/
    }
    // than call java to process class represents sdl struct
    env->CallStaticVoidMethod(mClass, fields.post_event, mObject, what, arg1, arg2, obj);

/*
    if(what == SDL_NATIVE_VIDEO_UPDATE_RECTS) {
        SDL_RECTS *rectangles = (SDL_RECTS *) data;
        for(int i=0;i<rectangles->numrects;i++) {
            jobject o = (jobject) env->GetObjectArrayElement((jobjectArray)obj, i);
            SDLRuntime::freeObject(env, o);
        }
    }
*/
}

/*
bool JNISDLVideoDriverListener::copyPixelsToJavaBuffer(JNIEnv* env, jobject jbuffer, jint size) {
    if(mSdlSurface == NULL) {
        return false;
    }
    android::AutoBufferPointer abp(env, jbuffer, JNI_TRUE);
    // the java side has already checked that buffer is large enough
    //memcpy(abp.pointer(), mSdlSurface->pixels, 50);
    return true;
}
*/

bool JNISDLVideoDriverListener::copyPixelsToJavaBitmap(JNIEnv* env, jobject jsurface, jobject jbitmap) {
    AndroidBitmapInfo  info;
    int                ret;
    void*              pixels;

    SDL_Surface* surface = android_sdl_SDLSurface_getNativeStruct(env, jsurface);
    if(surface == NULL) {
        return false;
    }
    
    if ((ret = AndroidBitmap_getInfo(env, jbitmap, &info)) < 0) {
        __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "AndroidBitmap_getInfo() failed ! error=%d", ret);
        return false;
    }

    if (info.format != ANDROID_BITMAP_FORMAT_RGB_565) {
        __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "Bitmap format is not RGB_565 !");
        return false;
    }

    if ((ret = AndroidBitmap_lockPixels(env, jbitmap, &pixels)) < 0) {
        __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "AndroidBitmap_lockPixels() failed ! error=%d", ret);
        return false;
    }

//    __android_log_print(ANDROID_LOG_INFO, LOG_TAG, "SdlSurface %ix%i bpp:%i", mSdlSurface->w, mSdlSurface->h, mSdlSurface->pitch);
//    __android_log_print(ANDROID_LOG_INFO, LOG_TAG, "Android bitmap %ix%i bpp:%i", info.width, info.height, info.format);

//    memcpy(pixels, mSdlSurface->pixels, 10000);
    memcpy(pixels, surface->pixels, info.height * info.width * 2);

    AndroidBitmap_unlockPixels(env, jbitmap);
    return true;
}

// ----------------------------------------------------------------------------
/*
static jboolean
android_sdl_SDLVideo_nativeCopyPixelsToBuffer(JNIEnv* env, jobject obj, JNISDLVideoDriverListener* listener, jobject jbuffer, jint size) {
    if (listener == NULL) {
        __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "nativeCopyPixelsToBuffer listener is NULL");
        return JNI_FALSE;
    }
    if(!listener->copyPixelsToJavaBuffer(env, jbuffer, size)) {
        __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "copyPixelsToJavaBuffer couldn't copy pixels to java buffer");
        return JNI_FALSE;
    }
    return JNI_TRUE;
}
*/

static jboolean
android_sdl_SDLVideo_nativeCopyPixelsToBitmap(JNIEnv* env, jobject obj, JNISDLVideoDriverListener* listener, jobject jsurface, jobject jbitmap) {
    if (listener == NULL) {
        __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "nativeCopyPixelsToBitmap listener is NULL");
        return JNI_FALSE;
    }
    if(!listener->copyPixelsToJavaBitmap(env, jsurface, jbitmap)) {
        __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "copyPixelsToJavaBitmap couldn't copy pixels to java buffer");
        return JNI_FALSE;
    }
    return JNI_TRUE;
}

// This function gets some field IDs, which in turn causes class initialization.
// It is called from a static block, which won't run until the
// first time an instance of this class is used.
static void
android_sdl_SDLVideo_native_init(JNIEnv *env, jobject thiz, jint sdk_version)
{
    __android_log_print(ANDROID_LOG_INFO, LOG_TAG, "native_init");
	
    jclass clazz = env->FindClass(kClassPathName);
    if (clazz == NULL) {
        SDLRuntime::doThrow(env, "java/lang/RuntimeException", 
							"Can't find android/sdl/SDLVideo");
        return;
    }
	 
    fields.post_event = env->GetStaticMethodID(clazz, "postEventFromNative",
                                               "(Ljava/lang/Object;IIILjava/lang/Object;)V");
    if (fields.post_event == NULL) {
        SDLRuntime::doThrow(env, "java/lang/RuntimeException", 
							"Can't find SDLVideo.postEventFromNative");
        return;
    }
	
    jclass surface_clazz = env->FindClass("android/view/Surface");
    if (surface_clazz == NULL) {
	__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "Can't find android/view/Surface");
	return;
    }

    // on gingerbread we should change name of pointer to native surface
    if(sdk_version >= 9) {
        fields.surface_native = env->GetFieldID(surface_clazz, "mNativeSurface", "I");
    } else {
        fields.surface_native = env->GetFieldID(surface_clazz, "mSurface", "I");
    }
    if (fields.surface_native == NULL) {
	__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "Can't find Surface.mSurface");
	return;
    }

    fields.mNativePointer = env->GetFieldID(clazz, "mNativePointer", "I");
    if (fields.mNativePointer == NULL) {
	__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "Can't find mNativePointer");
	return;
    }
}

// Register us into sdl video driver, so we can handle sdl video driver statuses
static void
android_sdl_SDLVideo_native_setup(JNIEnv *env, jobject thiz, jobject weak_this, jobject surface)
{
    __android_log_print(ANDROID_LOG_INFO, LOG_TAG, "native_setup");

    SDLVideoDriver* vd = SDLVideoDriver::getInstance();
    if (vd == NULL) {
        SDLRuntime::doThrow(env, "java/lang/RuntimeException", "Out of memory");
        return;
    }

    // create new listener and give it to MediaPlayer
    JNISDLVideoDriverListener* listener = new JNISDLVideoDriverListener(env, thiz, weak_this, surface);
    vd->registerListener(listener);

    env->SetIntField(thiz, fields.mNativePointer, (jint)listener);
}

static void
android_sdl_SDLVideo_native_finalize(JNIEnv *env, jobject thiz)
{
    __android_log_print(ANDROID_LOG_INFO, LOG_TAG, "native_finalize");
    //android_sdl_SDLVideo_release(env, thiz);
}

// ----------------------------------------------------------------------------

static JNINativeMethod gMethods[] = {
    {"native_init",                "(I)V",                                            (void *)android_sdl_SDLVideo_native_init},
    {"native_setup",               "(Ljava/lang/Object;Landroid/view/Surface;)V",     (void *)android_sdl_SDLVideo_native_setup},
    {"native_finalize",            "()V",                                             (void *)android_sdl_SDLVideo_native_finalize},
//    {"nativeCopyPixelsToBuffer",   "(ILjava/nio/Buffer;I)Z",                          (void *)android_sdl_SDLVideo_nativeCopyPixelsToBuffer},
    {"nativeCopyPixelsToBitmap",   "(ILandroid/sdl/SDLSurface;Landroid/graphics/Bitmap;)Z", (void *)android_sdl_SDLVideo_nativeCopyPixelsToBitmap}
};

namespace android {
	
// This function only registers the native methods
int register_android_sdl_SDLVideo(JNIEnv *env)
{
    return SDLRuntime::registerNativeMethods(env,
                kClassPathName, gMethods, NELEM(gMethods));
}
	
} // end of android namespace
