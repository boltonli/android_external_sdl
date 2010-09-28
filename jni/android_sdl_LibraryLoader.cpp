#include <dlfcn.h>
#include <android/log.h>
#include <utils/misc.h>

#include "jni.h"
#include "JNIHelp.h"

#define NULL 0
#define TAG "android_sdl_LibraryLoader.cpp"

namespace android {

static void *library;

int android_sdl_LibraryLoader_load(JNIEnv *env, jobject obj, jstring libName) {
    const char *lib = env->GetStringUTFChars(libName, NULL);
    if (lib == NULL) {
        __android_log_print(ANDROID_LOG_ERROR, TAG, "Can't get char* from libName string");
        return JNI_FALSE; /* OutOfMemoryError already thrown */
    }

    library = dlopen(lib, RTLD_NOW);
    if (library == NULL) {
        __android_log_print(ANDROID_LOG_ERROR, TAG, "can't dlopen %s", lib);
        __android_log_print(ANDROID_LOG_ERROR, TAG, "dlerror: %s", dlerror());
        return JNI_ERR;
    }
    return JNI_OK;
}

void android_sdl_LibraryLoader_close() {
    dlclose(library);
}

typedef int (*main) (int argc, char *argv[]);
int android_sdl_LibraryLoader_callMain() {
    if (library == NULL) {
        return JNI_ERR;
    }

    main m = (int (*)(int, char *argv[]))dlsym(library, "main");
    if (m == NULL) {
        return JNI_ERR;
    }
    return m(0, NULL);
}

static JNINativeMethod gMethods[] = {
        { "nativeLoadLibrary", "(Ljava/lang/String;)I", (void*) android_sdl_LibraryLoader_load },
        { "nativeCallMain", "()I", (void*) android_sdl_LibraryLoader_callMain },
        { "nativeCloseLibrary", "()V", (void*) android_sdl_LibraryLoader_close },
};

int register_android_sdl_LibraryLoader(JNIEnv* env) {
    return jniRegisterNativeMethods(env, "android/sdl/LibraryLoader", gMethods, NELEM(gMethods));
}

} // end of android namespace
