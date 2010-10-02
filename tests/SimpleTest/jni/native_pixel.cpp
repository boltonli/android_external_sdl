#include <stdio.h>
#include <SDL.h>
#include <jni.h>
#include <utils/Log.h>

#define SCREEN_WIDTH 480
#define SCREEN_HEIGHT 320
#define SCREEN_DEPTH 16

int main(int argc, char *argv[]) {
	SDL_Surface *screen;
	Uint8       *p;
	int         x = 10; //x coordinate of our pixel
	int         y = 20; //y coordinate of our pixel
	
	/* Initialize SDL */
	SDL_Init(SDL_INIT_VIDEO);
	
	/* Initialize the screen / window */
	screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_DEPTH, SDL_SWSURFACE);
	
	/* Make p point to the place we want to draw the pixel */
	p = (Uint8 *)screen->pixels + y * screen->pitch + x * screen->format->BytesPerPixel;
	
	/* Draw the pixel! */
	*p=0xff;
	
	/* update the screen (aka double buffering) */
	SDL_Flip(screen);
	while(1);
}

static jint start(JNIEnv *env, jobject thiz) {
    return main(0, NULL);
}

static const char *classPathName = "com/android/sdl/SimpleTestActivity";

static JNINativeMethod methods[] = {
	{"nativeStart", "()I", (void*)start },
};

/*
 * Register several native methods for one class.
 */
static int registerNativeMethods(JNIEnv* env, const char* className,
								 JNINativeMethod* gMethods, int numMethods)
{
    jclass clazz;
	
    clazz = env->FindClass(className);
    if (clazz == NULL) {
        LOGE("Native registration unable to find class '%s'", className);
        return JNI_FALSE;
    }
    if (env->RegisterNatives(clazz, gMethods, numMethods) < 0) {
        LOGE("RegisterNatives failed for '%s'", className);
        return JNI_FALSE;
    }
	
    return JNI_TRUE;
}

/*
 * Register native methods for all classes we know about.
 *
 * returns JNI_TRUE on success.
 */
static int registerNatives(JNIEnv* env)
{
	if (!registerNativeMethods(env, classPathName,
							   methods, sizeof(methods) / sizeof(methods[0]))) {
		return JNI_FALSE;
	}
	
	return JNI_TRUE;
}


// ----------------------------------------------------------------------------

/*
 * This is called by the VM when the shared library is first loaded.
 */

typedef union {
    JNIEnv* env;
    void* venv;
} UnionJNIEnvToVoid;

jint JNI_OnLoad(JavaVM* vm, void* reserved)
{
    UnionJNIEnvToVoid uenv;
    uenv.venv = NULL;
    jint result = -1;
    JNIEnv* env = NULL;
    
    LOGI("JNI_OnLoad");
	
    if (vm->GetEnv(&uenv.venv, JNI_VERSION_1_4) != JNI_OK) {
        LOGE("ERROR: GetEnv failed");
        goto bail;
    }
    env = uenv.env;
	
    if (registerNatives(env) != JNI_TRUE) {
        LOGE("ERROR: registerNatives failed");
        goto bail;
    }
    
    result = JNI_VERSION_1_4;
    
bail:
    return result;
}
