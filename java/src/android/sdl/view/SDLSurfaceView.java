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

package android.sdl.view;

import android.content.Context;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.util.Log;

import android.sdl.impl.SDLImpl;
import android.sdl.app.SDLActivity;
import android.sdl.SDLVideo;
import android.sdl.SDLSurface;
import android.sdl.SDLRect;

public class SDLSurfaceView extends SurfaceView {
	
	private static final String TAG = "SDLSurfaceView";
	
	// we must call main entry point of java lib for loading jni lib
    // this must be here because user overrides this class when using
    // sdl library
    static {
	    SDLImpl.load();
    }
	
	private SDLVideo.SDLVideoSetCaptionClb mCaptionClb = new SDLVideo.SDLVideoSetCaptionClb() {
		public void onSetCaption(String caption) {
		    ((SDLActivity)mContext).onSetCaption(caption);
	    }
    };
	
	private SDLVideo.SDLVideoSetSurfaceClb mSurfaceClb = new SDLVideo.SDLVideoSetSurfaceClb() {
	    public void onSetSurface(SDLSurface surface) {
	    }
    };
	
    private SDLVideo.SDLVideoPumpEventsClb mEventsClb = new SDLVideo.SDLVideoPumpEventsClb() {
	    public void onPumpEvents() {
		    ((SDLActivity)mContext).onProcessEvents();
	    }
    };
	
    private SDLVideo.SDLVideoUpdateRectsClb mUpdateClb = new SDLVideo.SDLVideoUpdateRectsClb() {
	    public void onUpdateRects(SDLRect[] rects) {
	    }
    };

    private SurfaceHolder.Callback mSurfaceViewClb = new SurfaceHolder.Callback() {
        public void surfaceChanged(SurfaceHolder holder, final int format, final int width, final int height) {}

        public void surfaceCreated(SurfaceHolder holder) {
            mVideoDriver = new SDLVideo(mSurfaceHolder.getSurface());
            mVideoDriver.registerCallback(mSurfaceClb);
            mVideoDriver.registerCallback(mEventsClb);
		    mVideoDriver.registerCallback(mUpdateClb);
            ((SDLActivity)mContext).onSDLCreate();
        }

        public void surfaceDestroyed(SurfaceHolder holder) {
            mVideoDriver = null;
        }
    };
	
	private SurfaceHolder mSurfaceHolder;
	private SDLVideo mVideoDriver;
	
	public SDLSurfaceView(Context context) {
		super(context);
		init();
	}
	
	private void init() {
		mSurfaceHolder = getHolder();
		mSurfaceHolder.addCallback(mSurfaceViewClb);
	}

}