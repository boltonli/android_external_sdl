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

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Rect;
import android.util.Log;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

import java.lang.ref.WeakReference;

public class SDLVideo extends SurfaceView {
    private static final String TAG = "SDLVideo";
	
    // must equals with libsdl/src/video/android/SDL_androidvideo.h -> sdl_native_events
    private static class SDLNativeEvents {
        private static final int SDL_NATIVE_VIDEO_CREATE_DEVICE = 1;
        private static final int SDL_NATIVE_VIDEO_DELETE_DEVICE = 2;
        private static final int SDL_NATIVE_VIDEO_PUMP_EVENTS = 3;
        private static final int SDL_NATIVE_VIDEO_INIT = 4;
        private static final int SDL_NATIVE_VIDEO_SET_SURFACE = 5;
        private static final int SDL_NATIVE_VIDEO_UPDATE_RECTS = 6;
        private static final int SDL_NATIVE_VIDEO_SET_CAPTION = 7;
        private static final int SDL_NATIVE_VIDEO_INIT_OS_KEYMAP = 8;
        private static final int SDL_NATIVE_VIDEO_ALLOC_HW_SURFACE = 9;
        private static final int SDL_NATIVE_VIDEO_LOCK_HW_SURFACE = 10;
        private static final int SDL_NATIVE_VIDEO_UNLOCK_HW_SURFACE = 11;
        private static final int SDL_NATIVE_VIDEO_FREE_HW_SURFACE = 12;
		
        /**** GL implementation *****/
        private static final int SDL_NATIVE_VIDEO_GL_LOAD_LIBRARY = 13;
        private static final int SDL_NATIVE_VIDEO_GL_GET_PROC_ADDRESS = 14;
        private static final int SDL_NATIVE_VIDEO_GL_UNLOAD_LIBRARY = 15;
        private static final int SDL_NATIVE_VIDEO_GL_CREATE_CONTEXT = 16;
        private static final int SDL_NATIVE_VIDEO_GL_MAKE_CURRENT = 17;
        private static final int SDL_NATIVE_VIDEO_GL_SET_SWAP_INTERVAL = 18;
        private static final int SDL_NATIVE_VIDEO_GL_GET_SWAP_INTERVAL = 19;
        private static final int SDL_NATIVE_VIDEO_GL_SWAP_WINDOW = 20;
        private static final int SDL_NATIVE_VIDEO_GL_DELETE_CONTEXT = 21;
    }
    
    /****** Android surface information *******/
    private SurfaceHolder mSurfaceHolder;
    
    /****** Callbacks variables *******/
    private SDLVideoPreparedClb mPreparedClb;
    private SDLVideoSetSurfaceClb mSurfaceClb;
    private SDLVideoPumpEventsClb mEventsClb;
    //private SDLVideoUpdateRectsClb mUpdateClb;
    private SDLVideoSetCaptionClb mCaptionClb;
    private SDLVideoSurfaceChangedClb mSurfaceChangedClb;
    
    //private ByteBuffer mPixelBuffer;
    private Bitmap mSdlBitmap;
    private SDLSurface mSdlSurface;
	
    // registers fields and methods
    private static native final void native_init(int sdk_version);
    private native final void native_setup(Object mediaplayer_this, Surface surface);
    private native final void native_finalize();
    //private native boolean nativeCopyPixelsToBuffer(int native_listener, Buffer buffer, int buffer_size);
    private native boolean nativeCopyPixelsToBitmap(int native_listener, SDLSurface surface, Bitmap bitmap);

    private int mNativePointer;

    static {
        Log.d(TAG, "loading java SDLVideo");
        Log.d(TAG, "version: " + android.os.Build.VERSION.SDK_INT);
        native_init(android.os.Build.VERSION.SDK_INT);
        Log.d(TAG, "java SDLVideo loaded");
    }
    
    /**
     * Surface holder callback handler
     * when we receive surface created event, than we call sdlPrepared listener 
     * and SDL fun can start :-)
     */
    private SurfaceHolder.Callback mSurfaceHClb = new SurfaceHolder.Callback() {
		
    	// android surface holder implementation
        public void surfaceDestroyed(SurfaceHolder holder) {
            Log.d(TAG, "surafce destroyed");
    	}
    	
    	public void surfaceCreated(SurfaceHolder holder) {
            Log.d(TAG, "surface created");
            /* Native setup requires a weak reference to our object.
    	     * It's easier to create it here than in C++.
    	     */
            Surface surface = holder.getSurface();
    	    native_setup(new WeakReference<SDLVideo>(SDLVideo.this), surface);
    	    if(mPreparedClb != null) {
    	    	mPreparedClb.onPrepared(surface);
    	    }
    	}
    	
    	public void surfaceChanged(SurfaceHolder holder, int format, int width,	int height) {
            Log.d(TAG, "surface changed: format=" + format + ", res=" + width + "x" + height);
            if(mSurfaceChangedClb != null) {
                mSurfaceChangedClb.onSurfaceChanged(format, width, height);
            }
    	}
    };
	
    public SDLVideo(Context context) {
        super(context);
    	SurfaceHolder holder = getHolder();
        holder.addCallback(mSurfaceHClb);
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
        SDLVideo ref = (SDLVideo)((WeakReference)sdlvideo_ref).get();
        if (ref == null) {
            Log.e(TAG, "SDLVideo ref is null");
            return;
        }
        ref.handleNativeMessage(what, obj);
    }

    private void handleNativeMessage(int what, Object obj) {
	    switch(what) {
	    case SDLNativeEvents.SDL_NATIVE_VIDEO_CREATE_DEVICE:
        {
            SDLVideoDevice device = (SDLVideoDevice) obj;
            handleVideoDeviceCreate(device);
            break;
		}
        case SDLNativeEvents.SDL_NATIVE_VIDEO_DELETE_DEVICE:
        {
		    SDLVideoDevice device = (SDLVideoDevice) obj;
            handleVideoDeviceDelete(device);
		    break;
        }
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
		case SDLNativeEvents.SDL_NATIVE_VIDEO_SET_CAPTION:
			String caption = (String) obj; 
			handleVideoDeviceSetCaption(caption);
			break;
	    default:
		    Log.e(TAG, "undefined event");
        }
    }
	
	private void handleVideoDeviceSetCaption(String caption) {
	    Log.d(TAG, "handleVideoDeviceSetCaption");
		Log.d(TAG, "caption: " + caption);
		if (mCaptionClb != null) {
			mCaptionClb.onSetCaption(caption);
		}
    }

    private void handleVideoDeviceCreate(SDLVideoDevice device) {
	    Log.d(TAG, "handleVideoDeviceCreate");
        Log.d(TAG, "device name: " + device.getName());
        Log.d(TAG, "device title: " + device.getWmTitle());
    }

    private void handleVideoDeviceDelete(SDLVideoDevice device) {
        Log.d(TAG, "handleVideoDeviceDelete");
        Log.d(TAG, "device name: " + device.getName());
        Log.d(TAG, "device title: " + device.getWmTitle());
    }
	
    private void handleVideoDeviceDelete() {
	    Log.d(TAG, "handleVideoDeviceDelete");
    }

    private void handleVideoDeviceInit(SDLPixelFormat pformat) {
	    Log.d(TAG, "handleVideoDeviceInit");
        Log.d(TAG, "bits per pixel: " + pformat.getBitsPerPixel());
        Log.d(TAG, "bytes per pixel: " + pformat.getBytesPerPixel());
    }

    private void handleVideoDeviceSetSurface(SDLSurface surface) {
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

		mSdlSurface = surface;
		mSdlBitmap = Bitmap.createBitmap(surface.getW(), surface.getH(), Bitmap.Config.RGB_565);
		
		if(mSurfaceClb != null) {
	        mSurfaceClb.onSetSurface(surface);
	    }
    }

    private void handleVideoDevicePumpEvents() {
	    if(mEventsClb != null) {
	        mEventsClb.onPumpEvents();
	    }
    }

    private long mBuffTime;
    private byte mFpsCounter;
    private void printFps() {
    	long curTime = System.currentTimeMillis();
    	if(mBuffTime + 1000 < curTime) {
    		Log.d(TAG, "Fps: " + mFpsCounter);
    		mBuffTime = curTime;
    		mFpsCounter = 0;
    	}
    	mFpsCounter++;
    }

    private void handleVideoDeviceUpdateRects(SDLRect[] rects) {
 //   	printFps();
    	
    	if(mSdlBitmap == null || 
    			mSdlSurface == null) {
    		return;
    	}
    	
/*
    	for(int i=0;i<rects.length;i++) {
    		Log.d(TAG, "Rectangle " + i + ": x=" + rects[i].getX() + ", y=" + rects[i].getY());
    	}
 */
    	
    	// copy sdl pixels into our bitmap
    	nativeCopyPixelsToBitmap(mNativePointer, mSdlSurface, mSdlBitmap);
    	
    	// draw our bitmap, which is consists from sdl pixels
    	// into screen
    	SurfaceHolder holder = getHolder();
		Canvas c = holder.lockCanvas();
		
		Rect sdlR = new Rect(0, 0, mSdlSurface.getW(), mSdlSurface.getH());
		Rect androidR = new Rect(0, 0, getWidth(), getHeight());
		c.drawBitmap(mSdlBitmap, sdlR, androidR, null);
		
		holder.unlockCanvasAndPost(c);
	    //if(mUpdateClb != null) {
	    //    mUpdateClb.onUpdateRects(rects);
	    //}
    }

    // methods for registering callbacks
    //---------------------------------------------------------------
    public void registerCallback(SDLVideoSetSurfaceClb clb) {
 	    mSurfaceClb = clb;
    }
 
    public void registerCallback(SDLVideoPumpEventsClb clb) {
 	    mEventsClb = clb;
    }

    //public void registerCallback(SDLVideoUpdateRectsClb clb) {
 	//    mUpdateClb = clb;
    //}
    
    public void registerCallback(SDLVideoPreparedClb clb) {
 	    mPreparedClb = clb;
    }
    
    public void registerCallback(SDLVideoSetCaptionClb clb) {
 	    mCaptionClb = clb;
    }

	public void registerCallback(SDLVideoSurfaceChangedClb clb) {
		mSurfaceChangedClb = clb;
	}

    // callbacks definitions
    //---------------------------------------------------------------
    public interface SDLVideoPreparedClb {
	    public void onPrepared(Surface surface);
    }
    
	public interface SDLVideoSetCaptionClb {
	    public void onSetCaption(String caption);
    }
	
    public interface SDLVideoSetSurfaceClb {
	    public void onSetSurface(SDLSurface surface);
    }

	public interface SDLVideoSurfaceChangedClb {
		public void onSurfaceChanged(int format, int width,	int height);
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

    //public interface SDLVideoUpdateRectsClb {
	//    public void onUpdateRects(SDLRect[] rects);
    //}

}
