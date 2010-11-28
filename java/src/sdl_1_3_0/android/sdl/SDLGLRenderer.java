/*
**
** Copyright 2010, Havlena Petr, havlenapetr@gmail.com
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

package android.sdl;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;
import javax.microedition.khronos.egl.*;

import android.view.SurfaceView;
import android.util.Log;

import java.lang.Exception;

public class SDLGLRenderer {
	private static final String TAG = "SDLGLRenderer";
	
	private static SDLGLRenderer sInstace;
	
	private EGLContext  mEGLContext;
    private EGLSurface  mEGLSurface;
    private EGLDisplay  mEGLDisplay;
	
	private SDLGLRenderer() {}
	
	public static SDLGLRenderer getRenderer() {
		if (sInstace == null) {
			sInstace = new SDLGLRenderer();
		}
		return sInstace;
	}
	
    //EGL functions
    public boolean initEGL(SurfaceView view){
		try{
			EGL10 egl = (EGL10)EGLContext.getEGL();
			EGLDisplay dpy = egl.eglGetDisplay(EGL10.EGL_DEFAULT_DISPLAY);
			int[] version = new int[2];
			egl.eglInitialize(dpy, version);
			
			int[] configSpec = {
				//EGL10.EGL_DEPTH_SIZE,   16,
				EGL10.EGL_NONE
			};
			EGLConfig[] configs = new EGLConfig[1];
			int[] num_config = new int[1];
			egl.eglChooseConfig(dpy, configSpec, configs, 1, num_config);
			EGLConfig config = configs[0];
			
			EGLContext ctx = egl.eglCreateContext(dpy, config, EGL10.EGL_NO_CONTEXT, null);
			EGLSurface surface = egl.eglCreateWindowSurface(dpy, config, view, null);
			
			egl.eglMakeCurrent(dpy, surface, surface, ctx);
			
			mEGLContext = ctx;
			mEGLDisplay = dpy;
			mEGLSurface = surface;
		} catch(Exception e){
			Log.e(TAG, e + "");
			for(StackTraceElement s : e.getStackTrace()){
				Log.e(TAG, s.toString());
			}
			return false;
		}
		return true;
	}
	
	//EGL buffer flip
	public boolean flipEGL(){      
		try{
			EGL10 egl = (EGL10)EGLContext.getEGL();
			GL10 gl = (GL10)mEGLContext.getGL();
			
			egl.eglWaitNative(EGL10.EGL_NATIVE_RENDERABLE, null);
			
			//drawing here
			
			egl.eglWaitGL();
			
			egl.eglSwapBuffers(mEGLDisplay, mEGLSurface);
		} catch(Exception e) {
			Log.e(TAG, "flipEGL(): " + e);
			for(StackTraceElement s : e.getStackTrace()) {
				Log.e(TAG, s.toString());
			}
			return false;
		}
		return true;
	}
	
}
