#include <stdio.h>
#include <SDL.h>
#include <jni.h>
#include <utils/Log.h>

#define WIDTH 480
#define HEIGHT 320
#define BPP 4
#define DEPTH 32

void setpixel(SDL_Surface *screen, int x, int y, Uint8 r, Uint8 g, Uint8 b)
{
    Uint32 *pixmem32;
    Uint32 colour;  
	
    colour = SDL_MapRGB( screen->format, r, g, b );
	
    pixmem32 = (Uint32*) screen->pixels  + y + x;
    *pixmem32 = colour;
}


void DrawScreen(SDL_Surface* screen, int h)
{ 
    int x, y, ytimesw;
	
    if(SDL_MUSTLOCK(screen)) 
    {
        if(SDL_LockSurface(screen) < 0) return;
    }
	
    for(y = 0; y < screen->h; y++ ) 
    {
        ytimesw = y*screen->pitch/BPP;
        for( x = 0; x < screen->w; x++ ) 
        {
            setpixel(screen, x, ytimesw, (x*x)/256+3*y+h, (y*y)/256+x+h, h);
        }
    }
	
    if(SDL_MUSTLOCK(screen)) SDL_UnlockSurface(screen);
	
    SDL_Flip(screen); 
}


int main(int argc, char* argv[])
{
    SDL_Surface *screen;
    SDL_Event event;
	
    int keypress = 0;
    int h=0; 
	
    if (SDL_Init(SDL_INIT_VIDEO) < 0 ) return 1;
	
    if (!(screen = SDL_SetVideoMode(WIDTH, HEIGHT, DEPTH, SDL_FULLSCREEN|SDL_HWSURFACE)))
    {
        SDL_Quit();
        return 1;
    }
	
    while(!keypress) 
    {
		DrawScreen(screen,h++);
		while(SDL_PollEvent(&event)) 
		{      
			switch (event.type) 
			{
				case SDL_QUIT:
					keypress = 1;
					break;
				case SDL_KEYDOWN:
					keypress = 1;
					break;
			}
		}
    }
	
    SDL_Quit();
	
    return 0;
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