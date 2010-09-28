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
import android.os.Handler;
import android.os.Message;

import java.lang.Thread;
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
	
	static {
		Log.d(TAG, "loading java SDLVideo");
		
		native_init();
		
		Log.d(TAG, "java SDLVideo loaded");
	}
	
	private Handler mEventHandler = new Handler() {
		@Override
		public void handleMessage(Message msg) {
			handleNativeMessage(msg);
		}
	};

	private Thread mOurThread;
	
    public SDLVideo() {
		/* Native setup requires a weak reference to our object.
		 * It's easier to create it here than in C++.
		 */
		native_setup(new WeakReference<SDLVideo>(this));
		mOurThread = Thread.currentThread();
		mOurThread.setName(TAG);
    }

	@Override
	protected void finalize() {
		Log.d(TAG, "finalizing");
		native_finalize(); 
	}
	
	/**
	 * native callback
	 **/
	private static void postEventFromNative(Object sdlvideo_ref, int what, int arg1, int arg2, Object obj) {
		Log.d(TAG, "on native event: " + what);
		SDLVideo ref = (SDLVideo)((WeakReference)sdlvideo_ref).get();
		if (ref == null) {
			Log.e(TAG, "SDLVideo ref is null");
			return;
		}

		String origThread = ref.mOurThread.getName();
		String curThread = Thread.currentThread().getName();

		Message m = ref.mEventHandler.obtainMessage(what, arg1, arg2, obj);
		if(origThread.matches(curThread)) {
			//Log.e(TAG, "Running in current thread");
			ref.handleNativeMessage(m);
		}
		else {
			//Log.e(TAG, "Running in another thread so use event handler");
			ref.mEventHandler.sendMessage(m);
		}
	}

	private void handleNativeMessage(Message msg) {
		switch(msg.what) {
			case SDLNativeEvents.SDL_NATIVE_VIDEO_CREATE_DEVICE:
				SDLVideoDevice device = (SDLVideoDevice) msg.obj;
				handleVideoDeviceCreate(device);
				break;
			case SDLNativeEvents.SDL_NATIVE_VIDEO_DELETE_DEVICE:
				handleVideoDeviceDelete();
				break;
			case SDLNativeEvents.SDL_NATIVE_VIDEO_INIT:
				SDLPixelFormat pformat = (SDLPixelFormat) msg.obj;
				handleVideoDeviceInit(pformat);
				break;
			case SDLNativeEvents.SDL_NATIVE_VIDEO_SET_SURFACE:
				SDLSurface surface = (SDLSurface) msg.obj;
				handleVideoDeviceSetSurface(surface);
				break;
			case SDLNativeEvents.SDL_NATIVE_VIDEO_PUMP_EVENTS:
				handleVideoDevicePumpEvents();
				break;
			case SDLNativeEvents.SDL_NATIVE_VIDEO_UPDATE_RECTS:
				SDLRect[] rects = (SDLRect[]) msg.obj; 
				handleVideoDeviceUpdateRects(rects);
				break;
			default:
				Log.e(TAG, "undefined event");
				break;
		}
	}

    private void handleVideoDeviceCreate(SDLVideoDevice device) {
		Log.d(TAG, "handleVideoDeviceCreate");
    }
	
    private void handleVideoDeviceDelete() {
		Log.d(TAG, "handleVideoDeviceDelete");
    }

    private void handleVideoDeviceInit(SDLPixelFormat pformat) {
		Log.d(TAG, "handleVideoDeviceInit");
    }

	private void handleVideoDeviceSetSurface(SDLSurface surface) {
		Log.d(TAG, "handleVideoDeviceSetSurface");
	}

	private void handleVideoDevicePumpEvents() {
		Log.d(TAG, "handleVideoDevicePumpEvents");
	}

	// this event isn't invoked into java and drawing is written in c++
	private void handleVideoDeviceUpdateRects(SDLRect[] rects) {
		Log.d(TAG, "handleVideoDeviceUpdateRects");
	}

	public interface SDLVideoSetSurfaceHandler {
		public void onSetSurface(SDLSurface surface);
	}

}
