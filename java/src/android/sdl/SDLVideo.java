/*
**
** Copyright 2010, Havlena Petr
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

import android.sdl.impl.SDLImpl.InitHandler;
import android.util.Log;

import java.lang.ref.WeakReference;

public class SDLVideo {
	private static final String TAG = "SDLVideo";
	
	// must equals with libsdl/src/video/android/SDL_androidvideo.h -> sdl_native_events
	private static class SDLNativeEvents {
	    private static final int SDL_NATIVE_VIDEO_CREATE_DEVICE = 1;
	    private static final int SDL_NATIVE_VIDEO_DELETE_DEVICE = 2;
	    private static final int SDL_NATIVE_VIDEO_PUMP_EVENTS = 3;
		private static final int SDL_NATIVE_VIDEO_INIT = 4;
		private static final int SDL_NATIVE_VIDEO_SET_SURFACE = 5;
		private static final int SDL_NATIVE_VIDEO_UPDATE_RECTS = 6;
	}
	
	// registers fields and methods
	private static native final void native_init();
    private native final void native_setup(Object mediaplayer_this);
    private native final void native_finalize();
	
	// handle static initzialization of sdl library
	public static InitHandler sInitCallback = new InitHandler() {
	    public void onInit() {
			Log.d(TAG, "loading java SDLVideo");
	
	        native_init();
	
			// create our SDLVideo driver
			sInstance = new SDLVideo();
	
			Log.d(TAG, "java SDLVideo loaded");
		}
	};

    private static SDLVideo sInstance;
	
    private SDLVideo() {
		/* Native setup requires a weak reference to our object.
		 * It's easier to create it here than in C++.
		 */
		native_setup(new WeakReference<SDLVideo>(this));
    }
	
	/**
	 * native callback
	 **/
	private static void postEventFromNative(int arg0, int arg1, Object obj) {
		switch (arg0) {
			case SDLNativeEvents.SDL_NATIVE_VIDEO_CREATE_DEVICE:
                SDLVideoDevice device = (SDLVideoDevice) obj;
                sInstance.handleVideoDeviceCreate(device);
				break;
			case SDLNativeEvents.SDL_NATIVE_VIDEO_DELETE_DEVICE:
                sInstance.handleVideoDeviceDelete();
				break;
			case SDLNativeEvents.SDL_NATIVE_VIDEO_INIT:
				SDLPixelFormat pformat = (SDLPixelFormat) obj;
				sInstance.handleVideoDeviceInit(pformat);
				break;
			case SDLNativeEvents.SDL_NATIVE_VIDEO_SET_SURFACE:
				SDLSurface surface = (SDLSurface) obj;
				sInstance.handleVideoDeviceSetSurface(surface);
				break;
			case SDLNativeEvents.SDL_NATIVE_VIDEO_PUMP_EVENTS:
				sInstance.handleVideoDevicePumpEvents();
				break;
			case SDLNativeEvents.SDL_NATIVE_VIDEO_UPDATE_RECTS:
				SDLRect[] rects = (SDLRect[]) obj; 
				sInstance.handleVideoDeviceUpdateRects(rects);
				break;
		}
	}

    private void handleVideoDeviceCreate(SDLVideoDevice device) {
    }
	
    private void handleVideoDeviceDelete() {
    }

    private void handleVideoDeviceInit(SDLPixelFormat pformat) {
    }

	private void handleVideoDeviceSetSurface(SDLSurface surface) {
		//SDLActivity.onSetSurface(surface);
	}

	private void handleVideoDevicePumpEvents() {
	}

	// this event isn't invoked into java and drawing is written in c++
	private void handleVideoDeviceUpdateRects(SDLRect[] rects) {
	}

	public interface SDLVideoSetSurfaceHandler {
		public void onSetSurface(SDLSurface surface);
	}

}
