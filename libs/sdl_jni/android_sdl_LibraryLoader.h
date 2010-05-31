#ifndef ANDROID_SDL_LIBRARYLOADER_H
#define ANDROID_SDL_LIBRARYLOADER_H

#include <jni.h>

class LibraryLoader
{
    public:
        static int load(JNIEnv *env, jobject obj, jstring libName);
        static void close();
        static int callMain();
};

#endif // ANDROID_SDL_LIBRARYLOADER_H
