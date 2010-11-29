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

#ifndef _RUNTIME_SDL_RUNTIME_H
#define _RUNTIME_SDL_RUNTIME_H

#include <pthread.h>
#include <jni.h>
#include <JNIHelp.h>

namespace android {
    
class SDLRuntime
{
public:
    SDLRuntime();
    virtual ~SDLRuntime();

    /**
     * Register a set of methods in the specified class.
     */
    static int registerNativeMethods(JNIEnv* env,
    const char* className, const JNINativeMethod* gMethods, int numMethods);

    static void doThrow(JNIEnv* env, const char* exc, const char* msg = NULL);
	
    static jobject createObject(JNIEnv* env, jclass clazz);
	
    static int javaDetachThread(void);

    static int javaAttachThread(const char* threadName, JNIEnv** pEnv);

    /** return a pointer to the VM running in this process */
    static JavaVM* getJavaVM() { return mJavaVM; }
	
    static void setJavaVM(JavaVM* vm) { mJavaVM = vm; }

    /** return a pointer to the JNIEnv pointer for this thread */
    static JNIEnv* getJNIEnv();

private:
    /* JNI JavaVM pointer */
    static JavaVM* mJavaVM;
};

}

#endif
