#include "SDL_androidaudio.h"
#include "SDL_androidvideo.h"

#include <SkCanvas.h>
#include <JNIHelp.h>
#include <jni.h>

#include <utils/Vector.h>

#include "android_sdl_LibraryLoader.h"

#define TAG "android_sdl_SDLSurfaceView.cpp"

#define EVENT_KEY 1
#define EVENT_MOUSE 2

struct fields_t {
    jfieldID    surface;
};
fields_t fields;

typedef struct {
    SDLVideoDriver *videoDriver;
    SDLAudioDriver *audioDriver;
} SDLDrivers;

static void *library;

/**
  * This is implementation of SDL video listener, where is periodically called
  * onUpdateScreen method which receives bitmap created by sdl, so we will take this bitmap
  * and draw her on android surface (if sdl bitmap is too big, we will stretch her via SkMatrix)
  *
 **/
class SDLSurfaceView : public SDLVideoDriver::SDLVideoDriverListener {

protected:
        ~SDLSurfaceView();
	
private:

        // just containers for android events, we will store them mEventBuffer
        class Event {
        public:
            int type;
            int action;
        };

        class KeyEvent : public Event {
        public:
            int key;

                KeyEvent(int key, int action) {
                        this->type = EVENT_KEY;
                        this->key = key;
                        this->action = action;
                }
        };

        class MotionEvent : public Event {
        public:
            int x;
            int y;

                MotionEvent(int x, int y, int action) {
                        this->type = EVENT_MOUSE;
                        this->x = x;
                        this->y = y;
                        this->action = action;
                }
        };

	Vector<Event*> mEventBuffer;
	Mutex mLock;
	SDLDrivers drivers;
        Surface *mSurface;
        Surface::SurfaceInfo mSurfaceInfo;
        SkBitmap mAndroidBitmap;
        SkBitmap *mSdlBitmap;
	
        void processEvents();

public:
    SDLSurfaceView();
    static SDLSurfaceView *getInstance();

    // periodically called by libsdl, which pass rendered sdl bitmap
    void onUpdateScreen(SkBitmap *bitmap);

    // this is jni callbacks from java SDLSurfaceView
    static void release(JNIEnv *env, jobject obj);
    static void init(JNIEnv *env, jobject obj, jobject surface);
    static void onKeyEvent(JNIEnv *env, jobject obj, jint key, jint action);
    static void onMouseEvent(JNIEnv *env, jobject obj, jint x, jint y, jint action);

};

static SDLSurfaceView *thiz = NULL;

SDLSurfaceView::SDLSurfaceView() {
}

SDLSurfaceView *SDLSurfaceView::getInstance() {
    if(thiz == NULL) thiz = new SDLSurfaceView();
    return thiz;
}

void SDLSurfaceView::init(JNIEnv *env, jobject obj, jobject surface) {
    __android_log_print(ANDROID_LOG_INFO, TAG, "creating surface");
    if(thiz == NULL) {
        thiz = getInstance();
    }

    thiz->mSurface = (Surface *) env->GetIntField(surface, fields.surface);
    if(thiz->mSurface == NULL) {
        __android_log_print(ANDROID_LOG_ERROR, TAG, "Native surface is NULL");
        return;
    }

    thiz->drivers.audioDriver = new SDLAudioDriver();
    thiz->drivers.videoDriver = new SDLVideoDriver(thiz);
    __android_log_print(ANDROID_LOG_INFO, TAG, "surface created");
}

void SDLSurfaceView::release(JNIEnv *env, jobject obj) {
    __android_log_print(ANDROID_LOG_INFO, TAG, "destroying surface");
    thiz->mEventBuffer.clear();
    free(thiz->drivers.audioDriver);
    free(thiz->drivers.videoDriver);
    __android_log_print(ANDROID_LOG_INFO, TAG, "surface destroyed");
}

void SDLSurfaceView::onKeyEvent(JNIEnv *env, jobject obj, jint key, jint action) {
    Mutex::Autolock _l(thiz->mLock);
    KeyEvent *e = new KeyEvent(key, action);
    thiz->mEventBuffer.add((Event *) e);
}

void SDLSurfaceView::onMouseEvent(JNIEnv *env, jobject obj, jint x, jint y, jint action) {
    Mutex::Autolock _l(thiz->mLock);
    MotionEvent *e = new MotionEvent(x, y, action);
    thiz->mEventBuffer.add((Event *) e);
}

void SDLSurfaceView::processEvents() {
    Mutex::Autolock _l(mLock);
    while(!mEventBuffer.isEmpty()) {
        Event *e = mEventBuffer.top();
        switch(e->type) {
            case EVENT_KEY:
            {
            KeyEvent *ke = (KeyEvent *) e;
            drivers.videoDriver->processKey(ke->key, ke->action);
            break;
            }
            case EVENT_MOUSE:
            {
            MotionEvent *me = (MotionEvent *) e;
            double x = (mSdlBitmap->width() * me->x) / mAndroidBitmap.width();
            double y = (mSdlBitmap->height() * me->y) / mAndroidBitmap.height();
            drivers.videoDriver->processMouse(x, y, me->action);
            break;
            }
            default:
            __android_log_print(ANDROID_LOG_ERROR, TAG, "Failed to encode event type: %i", e->type);
        }
		free(e);
        mEventBuffer.pop();
    }
}

void SDLSurfaceView::onUpdateScreen(SkBitmap *bitmap) {
        mSdlBitmap = bitmap;

        if (!mSurface || !mSurface->isValid()) {
            __android_log_print(ANDROID_LOG_INFO, TAG, "nativeSurface wasn't valid");
            return;
        }

        if (mSurface->lock(&mSurfaceInfo) < 0) {
            __android_log_print(ANDROID_LOG_ERROR, TAG, "Failed to lock surface");
            return;
        }

        processEvents();

        //bitmap which is drawed on android surfaceview
        SDLVideoDriver::setBitmapConfig(&mAndroidBitmap, mSurfaceInfo.format, mSurfaceInfo.w, mSurfaceInfo.h);
        mAndroidBitmap.setPixels(mSurfaceInfo.bits);

        SkCanvas canvas(mAndroidBitmap);
        SkRect surface_sdl;
        SkRect surface_android;
        SkMatrix matrix;
        surface_android.set(0, 0, mAndroidBitmap.width(), mAndroidBitmap.height());
        surface_sdl.set(0, 0, mSdlBitmap->width(), mSdlBitmap->height());
        matrix.setRectToRect(surface_sdl, surface_android, SkMatrix::kFill_ScaleToFit);

        canvas.drawBitmapMatrix(*mSdlBitmap, matrix);

        if (mSurface->unlockAndPost() < 0) {
            __android_log_print(ANDROID_LOG_ERROR, TAG, "Failed to unlock surface");
        }
}

/*
 * JNI registration.
 */
static JNINativeMethod methods[] = {
        { "nativeOnSurfaceCreated", "(Landroid/view/Surface;)V", (void*) SDLSurfaceView::init },
        { "nativeOnSurfaceDestroyed", "()V", (void*) SDLSurfaceView::release },
        { "nativeLoadLibrary", "(Ljava/lang/String;)I", (void*) LibraryLoader::load },
        { "nativeCallMain", "()I", (void*) LibraryLoader::callMain },
        { "nativeCloseLibrary", "()V", (void*) LibraryLoader::close },
        { "nativeOnKeyEvent", "(II)V", (void*) SDLSurfaceView::onKeyEvent },
        { "nativeOnMouseEvent", "(III)V", (void*) SDLSurfaceView::onMouseEvent }
};

/*
 * This is called by the VM when the shared library is first loaded.
 */
JNIEXPORT jint JNICALL
JNI_OnLoad(JavaVM* vm, void* reserved)
{
    jint result = JNI_ERR;
    JNIEnv* env = NULL;
    jclass clazz;

    __android_log_print(ANDROID_LOG_INFO, TAG, "loading . . .");

    if (vm->GetEnv((void**) &env, JNI_VERSION_1_4) != JNI_OK) {
        __android_log_print(ANDROID_LOG_ERROR, TAG, "Bad version of jni");
        goto bail;
    }

    clazz = env->FindClass("android/view/Surface");
    if(clazz == NULL) {
        __android_log_print(ANDROID_LOG_ERROR, TAG, "can't load native surface");
        goto bail;
    }

    fields.surface = env->GetFieldID(clazz, "mSurface", "I");
    if(fields.surface == NULL) {
        __android_log_print(ANDROID_LOG_ERROR, TAG, "can't load native mSurface");
        goto bail;
    }

    if(jniRegisterNativeMethods(env, "android/sdl/SDLSurfaceView", methods, sizeof(methods) / sizeof(methods[0])) != JNI_OK) {
        __android_log_print(ANDROID_LOG_ERROR, TAG, "can't register native methods");
        goto bail;
    }

    __android_log_print(ANDROID_LOG_INFO, TAG, "loaded");

    result = JNI_VERSION_1_4;
bail:
    return result;
}
