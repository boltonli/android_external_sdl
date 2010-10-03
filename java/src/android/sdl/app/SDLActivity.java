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
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.SurfaceView;
import android.util.Log;

import android.sdl.events.SDLEvents;
import android.sdl.events.SDLKeySym;
import android.sdl.events.SDLKeySym.SDLMod;
import android.sdl.view.SDLSurfaceView;

import java.util.ArrayList;

public abstract class SDLActivity extends Activity {
	
    private SDLSurfaceView mView;
	private ArrayList<Object> mEvents;

	/**
	 * @hide
	 */
    @Override
    protected void onCreate(Bundle bundle) {
	    super.onCreate(bundle);
		
		mEvents = new ArrayList<Object>();
		mView = new SDLSurfaceView(this);
		setContentView(mView);
    }
	
	private void addEvent(Object event) {
		synchronized (mEvents) {
			mEvents.add(event);
		}
	}
	
	private Object[] getEvents() {
		Object[] events = new Object[0];
		synchronized (mEvents) {
			events = mEvents.toArray();
			mEvents.clear();
		}
		return events;
	}
	
	public boolean onKeyUp(int keyCode, KeyEvent event) {
		addEvent(event);
		return true;
	}
	
	public boolean onKeyDown(int keyCode, KeyEvent event) {
		addEvent(event);
		return true;
	}
	
	public boolean onTouchEvent(MotionEvent event) {
		addEvent(event);
		return true;
	}
	
	@Override
	public boolean onTrackballEvent(MotionEvent event) {
		addEvent(event);
		return true;
	}

	/**
	 * called by sdl to set title of window
	 * @hide
	 */
	public void onSetCaption(String caption) {
		setTitle(caption);
	}
	
	public void onProcessEvents() {
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
	
	// called when sdl is prepared
	public abstract void onSDLCreate();
	
}