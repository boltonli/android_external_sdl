#include "android_sdl_LibraryLoader.h"

#include <dlfcn.h>
#include <android/log.h>

#define NULL 0
#define TAG "android_sdl_LibraryLoader.cpp"

static void *library;

int LibraryLoader::load(JNIEnv *env, jobject obj, jstring libName) {
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

void LibraryLoader::close() {
    dlclose(library);
}

typedef int (*main) (int argc, char *argv[]);
int LibraryLoader::callMain() {
    if (library == NULL) {
        return JNI_ERR;
    }

    main m = (int (*)(int, char *argv[]))dlsym(library, "main");
    if (m == NULL) {
        return JNI_ERR;
    }
    return m(0, NULL);
}
