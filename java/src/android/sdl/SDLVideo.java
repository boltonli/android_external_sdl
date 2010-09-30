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

import android.util.Log;
import android.view.Surface;

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

    private SDLVideoSetSurfaceClb mSurfaceClb;
    private SDLVideoPumpEventsClb mEventsClb;
    private SDLVideoUpdateRectsClb mUpdateClb;
	
    // registers fields and methods
    private static native final void native_init();
    private native final void native_setup(Object mediaplayer_this);
    private native final void native_finalize();
	
    static {
	    Log.d(TAG, "loading java SDLVideo");
		
	    native_init();
		
	    Log.d(TAG, "java SDLVideo loaded");
    }
	
    public SDLVideo() {
	    /* Native setup requires a weak reference to our object.
	     * It's easier to create it here than in C++.
	     */
	    native_setup(new WeakReference<SDLVideo>(this));
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

		ref.handleNativeMessage(what, obj);
    }

    private Object handleNativeMessage(int what, Object obj) {
	    switch(what) {
	    case SDLNativeEvents.SDL_NATIVE_VIDEO_CREATE_DEVICE:
		    SDLVideoDevice device = (SDLVideoDevice) obj;
		    handleVideoDeviceCreate(device);
		    break;
	    case SDLNativeEvents.SDL_NATIVE_VIDEO_DELETE_DEVICE:
		    handleVideoDeviceDelete();
		    break;
	    case SDLNativeEvents.SDL_NATIVE_VIDEO_INIT:
		    SDLPixelFormat pformat = (SDLPixelFormat) obj;
		    handleVideoDeviceInit(pformat);
		    break;
	    case SDLNativeEvents.SDL_NATIVE_VIDEO_SET_SURFACE:
		    SDLSurface surface = (SDLSurface) obj;
		    handleVideoDeviceSetSurface(surface);
		    break;
	    case SDLNativeEvents.SDL_NATIVE_VIDEO_PUMP_EVENTS:
		    handleVideoDevicePumpEvents();
			break;
	    case SDLNativeEvents.SDL_NATIVE_VIDEO_UPDATE_RECTS:
		    SDLRect[] rects = (SDLRect[]) obj; 
		    handleVideoDeviceUpdateRects(rects);
		    break;
	    default:
		    Log.e(TAG, "undefined event");
        }
        return null;
    }

    private void handleVideoDeviceCreate(SDLVideoDevice device) {
	    Log.d(TAG, "handleVideoDeviceCreate");
    }
	
    private void handleVideoDeviceDelete() {
	    Log.d(TAG, "handleVideoDeviceDelete");
    }

    private void handleVideoDeviceInit(SDLPixelFormat pformat) {
	    Log.d(TAG, "handleVideoDeviceInit");
            Log.d(TAG, "bits per pixel: " + pformat.getBitsPerPixel());
            Log.d(TAG, "bytes per pixel: " + pformat.getBytesPerPixel());
    }

    private Surface handleVideoDeviceSetSurface(SDLSurface surface) {
	    Log.d(TAG, "handleVideoDeviceSetSurface");
            Log.d(TAG, "surface w: " + surface.getW());
            Log.d(TAG, "surface h: " + surface.getH());
            SDLRect rect = surface.getClipRect();
            Log.d(TAG, "surface rect x: " + rect.getX());
            Log.d(TAG, "surface rect y: " + rect.getY());
            Log.d(TAG, "surface rect w: " + rect.getW());
            Log.d(TAG, "surface rect h: " + rect.getH());
            SDLPixelFormat pformat = surface.getPixelFormat();
            Log.d(TAG, "bits per pixel: " + pformat.getBitsPerPixel());
            Log.d(TAG, "bytes per pixel: " + pformat.getBytesPerPixel());
            if(mSurfaceClb != null) {
	        return mSurfaceClb.onSetSurface(surface);
	    }
        return null;
    }

    private void handleVideoDevicePumpEvents() {
	    Log.d(TAG, "handleVideoDevicePumpEvents");
	    if(mEventsClb != null) {
	        mEventsClb.onPumpEvents();
	    }
    }

    // this event isn't invoked into java and drawing is written in c++
    private void handleVideoDeviceUpdateRects(SDLRect[] rects) {
		Log.d(TAG, "handleVideoDeviceUpdateRects");
	    if(mUpdateClb != null) {
	        mUpdateClb.onUpdateRects(rects);
	    }
    }

    // methods for registering callbacks
    //---------------------------------------------------------------
    public void registerCallback(SDLVideoSetSurfaceClb clb) {
 	    mSurfaceClb = clb;
    }
 
    public void registerCallback(SDLVideoPumpEventsClb clb) {
 	    mEventsClb = clb;
    }

    public void registerCallback(SDLVideoUpdateRectsClb clb) {
 	    mUpdateClb = clb;
    }

    // callbacks definitions
    //---------------------------------------------------------------
    public interface SDLVideoSetSurfaceClb {
	    public Surface onSetSurface(SDLSurface surface);
    }

    public interface SDLVideoDeviceCreateClb {
	    public void onDeviceCreate(SDLVideoDevice device);
    }

    public interface SDLVideoDeviceDeleteClb {
	    public void onDeviceDelete();
    }

    public interface SDLVideoDeviceInitClb {
	    public void onDeviceInit(SDLPixelFormat pformat);
    }

    public interface SDLVideoPumpEventsClb {
	    public void onPumpEvents();
    }

    public interface SDLVideoUpdateRectsClb {
	    public void onUpdateRects(SDLRect[] rects);
    }

}
