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

package android.sdl.app;

import android.os.Bundle;
import android.app.Activity;
import android.view.Surface;
import android.view.SurfaceView;

import android.sdl.impl.SDLImpl;
import android.sdl.SDLVideo;
import android.sdl.SDLSurface;
import android.sdl.SDLRect;

public class SDLActivity extends Activity {
	
    // we must call main entry point of java lib for loading jni lib
    // this must be here because user overrides this class when using
    // sdl library
    static {
	    SDLImpl.load();
    }

    private SDLVideo.SDLVideoSetSurfaceClb mSurfaceClb = new SDLVideo.SDLVideoSetSurfaceClb() {
	    public Surface onSetSurface(SDLSurface surface) {
	        //SurfaceView view = new SurfaceView(SDLActivity.this);
	        //SDLActivity.this.setContentView(view);
	        return null;//view.getHolder().getSurface();
	    }
    };

    private SDLVideo.SDLVideoPumpEventsClb mEventsClb = new SDLVideo.SDLVideoPumpEventsClb() {
	    public void onPumpEvents() {
	    }
    };

    private SDLVideo.SDLVideoUpdateRectsClb mUpdateClb = new SDLVideo.SDLVideoUpdateRectsClb() {
	    public void onUpdateRects(SDLRect[] rects) {
	    }
    };
	
    @Override
    protected void onCreate(Bundle bundle) {
	    super.onCreate(bundle);

	    SDLVideo vdriver = SDLImpl.getVideoDriver();
	    vdriver.registerCallback(mSurfaceClb);
	    vdriver.registerCallback(mEventsClb);
	    vdriver.registerCallback(mUpdateClb);
    }
	
}
