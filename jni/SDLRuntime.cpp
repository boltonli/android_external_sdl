/*
**
** Copyright 2006, Petr Havlena
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

#define LOG_TAG "SDLRuntime"
//#define LOG_NDEBUG 0

#include "SDLRuntime.h"
#include "JNIHelp.h"

#include <utils/Log.h>

#include <stdio.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <signal.h>
#include <dirent.h>
#include <assert.h>

namespace android {

extern int register_android_sdl_LibraryLoader(JNIEnv* env);
extern int register_android_sdl_SDLSurfaceView(JNIEnv* env);

static SDLRuntime* gCurRuntime = NULL;

/*static*/ JavaVM* SDLRuntime::mJavaVM = NULL;


SDLRuntime::SDLRuntime()
{
    assert(gCurRuntime == NULL);        // one per process
    gCurRuntime = this;
}

SDLRuntime::~SDLRuntime()
{
}
	
/*static*/ void SDLRuntime::doThrow(JNIEnv* env, const char* exc, const char* msg)	
{
	if (jniThrowException(env, exc, msg) != 0)
		assert(false);
}

/*
 * Register native methods using JNI.
 */
int SDLRuntime::registerNativeMethods(JNIEnv* env,
    const char* className, const JNINativeMethod* gMethods, int numMethods)
{
    return jniRegisterNativeMethods(env, className, gMethods, numMethods);
}

/*
 * Get the JNIEnv pointer for this thread.
 *
 * Returns NULL if the slot wasn't allocated or populated.
 */
JNIEnv* SDLRuntime::getJNIEnv()
{
    JNIEnv* env;
    JavaVM* vm = SDLRuntime::getJavaVM();
    assert(vm != NULL);

    if (vm->GetEnv((void**) &env, JNI_VERSION_1_4) != JNI_OK)
        return NULL;
    return env;
}

/*
 * Makes the current thread visible to the VM.
 *
 * The JNIEnv pointer returned is only valid for the current thread, and
 * thus must be tucked into thread-local storage.
 */
/*static*/ int SDLRuntime::javaAttachThread(const char* threadName, JNIEnv** pEnv)
{
    JavaVMAttachArgs args;
    JavaVM* vm;
    jint result;

    vm = SDLRuntime::getJavaVM();
    assert(vm != NULL);

    args.version = JNI_VERSION_1_4;
    args.name = (char*) threadName;
    args.group = NULL;

    result = vm->AttachCurrentThread(pEnv, (void*) &args);
    if (result != JNI_OK)
        LOGI("NOTE: attach of thread '%s' failed\n", threadName);

    return result;
}

/*
 * Detach the current thread from the set visible to the VM.
 */
/*static*/ int SDLRuntime::javaDetachThread(void)
{
    JavaVM* vm;
    jint result;

    vm = SDLRuntime::getJavaVM();
    assert(vm != NULL);

    result = vm->DetachCurrentThread();
    if (result != JNI_OK)
        LOGE("ERROR: thread detach failed\n");
    return result;
}

/**
 * Used by LoadClass to register native functions.
 */
extern "C" jint JNI_OnLoad(JavaVM* vm, void* reserved)
{
	SDLRuntime::setJavaVM(vm);
	JNIEnv* env = SDLRuntime::getJNIEnv();
	
	LOG_ASSERT(env, "Could not retrieve the env!");
		
	LOGV("loading . . .");
		
	if(register_android_sdl_LibraryLoader(env) != JNI_OK) {
		LOGE("can't load LibraryLoader");
		return JNI_ERR;
	}
		
	if(register_android_sdl_SDLSurfaceView(env) != JNI_OK) {
		LOGE("can't load SDLSurfaceView");
		return JNI_ERR;
	}
	
	LOGV("loaded");
	return JNI_VERSION_1_4;
}

}   // namespace android
