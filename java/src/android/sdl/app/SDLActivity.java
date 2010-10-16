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

package android.sdl.app;

import android.os.Bundle;
import android.app.Activity;
import android.view.Surface;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.SurfaceView;

import android.sdl.SDLVideo;
import android.sdl.events.SDLEvents;
import android.sdl.events.SDLKeySym;
import android.sdl.events.SDLKeySym.SDLMod;
import android.sdl.impl.SDLImpl;

import java.util.ArrayList;

public abstract class SDLActivity extends Activity {
	
	// we must call main entry point of java lib for loading jni lib
    // this must be here because user overrides this class when using
    // sdl library
    static {
	    SDLImpl.load();
    }
    
    private SDLVideo.SDLVideoPreparedClb mSDLPreparedClb = new SDLVideo.SDLVideoPreparedClb() {
		
		public void onPrepared(Surface surface) {
			onSDLCreate();
		}
	};
	
	private SDLVideo.SDLVideoPumpEventsClb mSDLPumpEvents = new SDLVideo.SDLVideoPumpEventsClb() {
		
		public void onPumpEvents() {
			processEvents();
		}
	};
	
	private SDLVideo.SDLVideoSetCaptionClb mSDLSetCaptionClb = new SDLVideo.SDLVideoSetCaptionClb() {
		
		public void onSetCaption(String caption) {
			setTitle(caption);
		}
	};
    
	private ArrayList<Object> mEvents;

	/**
	 * @hide
	 */
    @Override
    protected void onCreate(Bundle bundle) {
	    super.onCreate(bundle);
		
		mEvents = new ArrayList<Object>();
		SDLVideo vdriver = SDLImpl.registerVideoDriver(this);
		vdriver.registerCallback(mSDLPreparedClb);
		vdriver.registerCallback(mSDLPumpEvents);
		vdriver.registerCallback(mSDLSetCaptionClb);
		setContentView(vdriver);
    }

    /**
     * @hide
     */
    @Override
    protected void onPause() {
        super.onPause();

        SDLImpl.quit();
    }
	
	private boolean addEvent(Object event) {
		if(event instanceof KeyEvent) {
			int keyCode = ((KeyEvent)event).getKeyCode();
			if(keyCode == KeyEvent.KEYCODE_MENU) {
				return false;
			}
		}
		synchronized (mEvents) {
			mEvents.add(event);
		}
		return true;
	}
	
	private Object[] getEvents() {
		Object[] events = new Object[0];
		synchronized (mEvents) {
			events = mEvents.toArray();
			mEvents.clear();
		}
		return events;
	}
	
	private void processEvents() {
		Object[] events = getEvents();
		for (int i=0; i<events.length; i++) {
			Object event = events[i];
			if(event instanceof MotionEvent) {
				MotionEvent e = (MotionEvent) event;
				switch (e.getAction()) {
					case MotionEvent.ACTION_DOWN:
						SDLEvents.MouseButton((short)SDLEvents.SDL_PRESSED, 
											  (short)0, 
											  (int)e.getX(), 
											  (int)e.getY());
						break;
					case MotionEvent.ACTION_UP:
						SDLEvents.MouseButton((short)SDLEvents.SDL_RELEASED, 
											  (short)0, 
											  (int)e.getX(), 
											  (int)e.getY());
						break;
					case MotionEvent.ACTION_MOVE:
						SDLEvents.MouseMotion((short)0, 
											  0, 
											  (int)e.getX(), 
											  (int)e.getY());
						break;
					default:
						break;
				}
			} else if(event instanceof KeyEvent) {
				KeyEvent e = (KeyEvent) event;
				
				SDLKeySym keysym = new SDLKeySym();
				/* Set the keysym information */
				keysym.scancode = e.getKeyCode();
				keysym.sym = SDLEvents.keymap[e.getKeyCode()];
				keysym.mod = SDLMod.KMOD_NONE;
				
				/* If UNICODE is on, get the UNICODE value for the key */
				keysym.unicode = 0;
				
				SDLEvents.Keyboard((short)((e.getAction() == KeyEvent.ACTION_DOWN) ? SDLEvents.SDL_PRESSED : SDLEvents.SDL_RELEASED), 
								   keysym);
			}
		}
	}
	
	/**
	 * Override android motion and key event callbacks nad pass them to our event buffer
	 */
	public boolean onKeyUp(int keyCode, KeyEvent event) {
		return addEvent(event);
	}
	
	public boolean onKeyDown(int keyCode, KeyEvent event) {
		return addEvent(event);
	}
	
	public boolean onTouchEvent(MotionEvent event) {
		return addEvent(event);
	}
	
	@Override
	public boolean onTrackballEvent(MotionEvent event) {
		return addEvent(event);
	}
	
	// called when sdl is prepared
	public abstract void onSDLCreate();
	
}
