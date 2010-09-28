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

import android.sdl.impl.SDLImpl;
//import android.sdl.app.SDLActivity;

public class SDLVideo {
	
	// must equals with libsdl/src/video/android/SDL_androidvideo.h -> sdl_native_events
	private static class SDLNativeEvents {
	    private static final int SDL_NATIVE_VIDEO_CREATE_DEVICE = 1;
	    private static final int SDL_NATIVE_VIDEO_DELETE_DEVICE = 2;
	    private static final int SDL_NATIVE_VIDEO_PROCESS_EVENTS = 3;
		private static final int SDL_NATIVE_VIDEO_INIT = 4;
		private static final int SDL_NATIVE_VIDEO_SET_SURFACE = 5;
		private static final int SDL_NATIVE_VIDEO_PUMP_EVENTS = 6;
	}
	
	// registers fields and methods
	private static native final void native_init();
	
	// handle static initzialization of sdl library
	public static SDLImpl.InitHandler sInitCallback = new SDLImpl.InitHandler() {
	    public void onInit() {
	        native_init();
		}
	};

    private static SDLVideo sInstance;
	
    private SDLVideo() {
        sInstance = this;
    }
	
	/**
	 * native callback
	 **/
	private static void handleNotify(int arg0, int arg1, Object obj) {
		switch (arg0) {
			case SDLNativeEvents.SDL_NATIVE_VIDEO_CREATE_DEVICE:
                SDLVideoDevice device = (SDLVideoDevice) obj;
                sInstance.handleVideoDeviceCreate(device);
				break;
			case SDLNativeEvents.SDL_NATIVE_VIDEO_DELETE_DEVICE:
                sInstance.handleVideoDeviceDelete();
				break;
			case SDLNativeEvents.SDL_NATIVE_VIDEO_PROCESS_EVENTS:
                sInstance.handleVideoDeviceProcessEvents();
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
		}
	}

    private void handleVideoDeviceCreate(SDLVideoDevice device) {
    }
	
    private void handleVideoDeviceDelete() {
    }
 
    private void handleVideoDeviceProcessEvents() {
    }

    private void handleVideoDeviceInit(SDLPixelFormat pformat) {
    }

	private void handleVideoDeviceSetSurface(SDLSurface surface) {
		//SDLActivity.onSetSurface(surface);
	}

	private void handleVideoDevicePumpEvents() {
	}

	public interface SDLVideoSetSurfaceHandler {
		public void onSetSurface(SDLSurface surface);
	}

}
