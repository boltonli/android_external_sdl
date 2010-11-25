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

package android.sdl.events;

import android.view.KeyEvent;
import android.sdl.events.SDLKeySym.SDLKey;

public class SDLEvents {
	
    //private int mNativePointer;
	public static final int SDL_RELEASED = 0;
	public static final int SDL_PRESSED	= 1;
	
	public static int[] keymap = new int[KeyEvent.getMaxKeyCode() + 1];
	

	/** Event enumerations */
	public static class SDLEventType {
		public static final int SDL_NOEVENT = 0;				/**< Unused (do not remove) */
		public static final int SDL_ACTIVEEVENT = 1;			/**< Application loses/gains visibility */
		public static final int SDL_KEYDOWN = 2;				/**< Keys pressed */
		public static final int SDL_KEYUP = 3;					/**< Keys released */
		public static final int SDL_MOUSEMOTION = 4;			/**< Mouse moved */
		public static final int SDL_MOUSEBUTTONDOWN = 5;		/**< Mouse button pressed */
		public static final int SDL_MOUSEBUTTONUP = 6;			/**< Mouse button released */
		public static final int SDL_JOYAXISMOTION = 7;			/**< Joystick axis motion */
		public static final int SDL_JOYBALLMOTION = 8;			/**< Joystick trackball motion */
		public static final int SDL_JOYHATMOTION = 9;			/**< Joystick hat position change */
		public static final int SDL_JOYBUTTONDOWN = 10;			/**< Joystick button pressed */
		public static final int SDL_JOYBUTTONUP = 11;			/**< Joystick button released */
		public static final int SDL_QUI = 12;					/**< User-requested quit */
		public static final int SDL_SYSWMEVENT = 13;			/**< System specific event */
		public static final int SDL_EVENT_RESERVEDA = 14;		/**< Reserved for future use.. */
		public static final int SDL_EVENT_RESERVEDB = 15;		/**< Reserved for future use.. */
		public static final int SDL_VIDEORESIZE = 16;			/**< User resized video mode */
		public static final int SDL_VIDEOEXPOSE = 17;			/**< Screen needs to be redrawn */
		public static final int SDL_EVENT_RESERVED2 = 18;		/**< Reserved for future use.. */
		public static final int SDL_EVENT_RESERVED3 = 19;		/**< Reserved for future use.. */
		public static final int SDL_EVENT_RESERVED4 = 20;		/**< Reserved for future use.. */
		public static final int SDL_EVENT_RESERVED5 = 21;		/**< Reserved for future use.. */
		public static final int SDL_EVENT_RESERVED6 = 22;		/**< Reserved for future use.. */
		public static final int SDL_EVENT_RESERVED7 = 23;		/**< Reserved for future use.. */
		/** Events SDL_USEREVENT through SDL_MAXEVENTS-1 are for your use */
		public static final int SDL_USEREVENT = 24;
		/** This last event is only for bounding internal arrays
		 *  It is the number of bits in the event mask datatype -- Uint32
		 */
		public static final int SDL_NUMEVENTS = 32;
	}

	/*@{*/
	/** Used as a mask when testing buttons in buttonstate
	 *  Button 1:	Left mouse button
	 *  Button 2:	Middle mouse button
	 *  Button 3:	Right mouse button
	 *  Button 4:	Mouse wheel up	 (may also be a real button)
	 *  Button 5:	Mouse wheel down (may also be a real button)
	 */
	public static class SDLMouseButton {
		public static final short SDL_BUTTON_LEFT =	1;
		public static final short SDL_BUTTON_MIDDLE = 2;
		public static final short SDL_BUTTON_RIGHT = 3;
		public static final short SDL_BUTTON_WHEELUP = 4;
		public static final short SDL_BUTTON_WHEELDOWN = 5;
		public static final short SDL_BUTTON_X1	= 6;
		public static final short SDL_BUTTON_X2	= 7;
	}

	public native static void native_init();
	static {
		native_init();
		initOSKeymap();
	}

    private SDLEvents() {
    }

	private static void initOSKeymap() {
		/* Initialize the DirectFB key translation table */
		for (int i = 0; i < keymap.length; i++)
			keymap[i] = SDLKey.SDLK_UNKNOWN;

		keymap[KeyEvent.KEYCODE_UNKNOWN] = SDLKey.SDLK_UNKNOWN;

		// keymap[KeyEvent.KEYCODE_MENU] = SDLKey.SDLK_ESCAPE;

		keymap[KeyEvent.KEYCODE_CALL] = SDLKey.SDLK_F1;
		keymap[KeyEvent.KEYCODE_ENDCALL] = SDLKey.SDLK_F2;
		keymap[KeyEvent.KEYCODE_CAMERA] = SDLKey.SDLK_F3;
		keymap[KeyEvent.KEYCODE_POWER] = SDLKey.SDLK_F4;

		keymap[KeyEvent.KEYCODE_BACK] = SDLKey.SDLK_ESCAPE; // Note: generates
															// SDL_QUIT
		keymap[KeyEvent.KEYCODE_0] = SDLKey.SDLK_0;
		keymap[KeyEvent.KEYCODE_1] = SDLKey.SDLK_1;
		keymap[KeyEvent.KEYCODE_2] = SDLKey.SDLK_2;
		keymap[KeyEvent.KEYCODE_3] = SDLKey.SDLK_3;
		keymap[KeyEvent.KEYCODE_4] = SDLKey.SDLK_4;
		keymap[KeyEvent.KEYCODE_5] = SDLKey.SDLK_5;
		keymap[KeyEvent.KEYCODE_6] = SDLKey.SDLK_6;
		keymap[KeyEvent.KEYCODE_7] = SDLKey.SDLK_7;
		keymap[KeyEvent.KEYCODE_8] = SDLKey.SDLK_8;
		keymap[KeyEvent.KEYCODE_9] = SDLKey.SDLK_9;
		keymap[KeyEvent.KEYCODE_STAR] = SDLKey.SDLK_ASTERISK;
		keymap[KeyEvent.KEYCODE_POUND] = SDLKey.SDLK_DOLLAR;

		keymap[KeyEvent.KEYCODE_DPAD_UP] = SDLKey.SDLK_UP;
		keymap[KeyEvent.KEYCODE_DPAD_DOWN] = SDLKey.SDLK_DOWN;
		keymap[KeyEvent.KEYCODE_DPAD_LEFT] = SDLKey.SDLK_LEFT;
		keymap[KeyEvent.KEYCODE_DPAD_RIGHT] = SDLKey.SDLK_RIGHT;
		keymap[KeyEvent.KEYCODE_DPAD_CENTER] = SDLKey.SDLK_RETURN;

		keymap[KeyEvent.KEYCODE_SOFT_LEFT] = SDLKey.SDLK_KP4;
		keymap[KeyEvent.KEYCODE_SOFT_RIGHT] = SDLKey.SDLK_KP6;
		keymap[KeyEvent.KEYCODE_ENTER] = SDLKey.SDLK_KP_ENTER;

		keymap[KeyEvent.KEYCODE_VOLUME_UP] = SDLKey.SDLK_PAGEUP;
		keymap[KeyEvent.KEYCODE_VOLUME_DOWN] = SDLKey.SDLK_PAGEDOWN;
		keymap[KeyEvent.KEYCODE_SEARCH] = SDLKey.SDLK_END;
		keymap[KeyEvent.KEYCODE_HOME] = SDLKey.SDLK_HOME;

		keymap[KeyEvent.KEYCODE_CLEAR] = SDLKey.SDLK_CLEAR;
		keymap[KeyEvent.KEYCODE_A] = SDLKey.SDLK_a;
		keymap[KeyEvent.KEYCODE_B] = SDLKey.SDLK_b;
		keymap[KeyEvent.KEYCODE_C] = SDLKey.SDLK_c;
		keymap[KeyEvent.KEYCODE_D] = SDLKey.SDLK_d;
		keymap[KeyEvent.KEYCODE_E] = SDLKey.SDLK_e;
		keymap[KeyEvent.KEYCODE_F] = SDLKey.SDLK_f;
		keymap[KeyEvent.KEYCODE_G] = SDLKey.SDLK_g;
		keymap[KeyEvent.KEYCODE_H] = SDLKey.SDLK_h;
		keymap[KeyEvent.KEYCODE_I] = SDLKey.SDLK_i;
		keymap[KeyEvent.KEYCODE_J] = SDLKey.SDLK_j;
		keymap[KeyEvent.KEYCODE_K] = SDLKey.SDLK_k;
		keymap[KeyEvent.KEYCODE_L] = SDLKey.SDLK_l;
		keymap[KeyEvent.KEYCODE_M] = SDLKey.SDLK_m;
		keymap[KeyEvent.KEYCODE_N] = SDLKey.SDLK_n;
		keymap[KeyEvent.KEYCODE_O] = SDLKey.SDLK_o;
		keymap[KeyEvent.KEYCODE_P] = SDLKey.SDLK_p;
		keymap[KeyEvent.KEYCODE_Q] = SDLKey.SDLK_q;
		keymap[KeyEvent.KEYCODE_R] = SDLKey.SDLK_r;
		keymap[KeyEvent.KEYCODE_S] = SDLKey.SDLK_s;
		keymap[KeyEvent.KEYCODE_T] = SDLKey.SDLK_t;
		keymap[KeyEvent.KEYCODE_U] = SDLKey.SDLK_u;
		keymap[KeyEvent.KEYCODE_V] = SDLKey.SDLK_v;
		keymap[KeyEvent.KEYCODE_W] = SDLKey.SDLK_w;
		keymap[KeyEvent.KEYCODE_X] = SDLKey.SDLK_x;
		keymap[KeyEvent.KEYCODE_Y] = SDLKey.SDLK_y;
		keymap[KeyEvent.KEYCODE_Z] = SDLKey.SDLK_z;
		keymap[KeyEvent.KEYCODE_COMMA] = SDLKey.SDLK_COMMA;
		keymap[KeyEvent.KEYCODE_PERIOD] = SDLKey.SDLK_PERIOD;
		keymap[KeyEvent.KEYCODE_TAB] = SDLKey.SDLK_TAB;
		keymap[KeyEvent.KEYCODE_SPACE] = SDLKey.SDLK_SPACE;
		keymap[KeyEvent.KEYCODE_DEL] = SDLKey.SDLK_DELETE;
		keymap[KeyEvent.KEYCODE_GRAVE] = SDLKey.SDLK_BACKQUOTE;
		keymap[KeyEvent.KEYCODE_MINUS] = SDLKey.SDLK_MINUS;
		keymap[KeyEvent.KEYCODE_EQUALS] = SDLKey.SDLK_EQUALS;
		keymap[KeyEvent.KEYCODE_LEFT_BRACKET] = SDLKey.SDLK_LEFTBRACKET;
		keymap[KeyEvent.KEYCODE_RIGHT_BRACKET] = SDLKey.SDLK_RIGHTBRACKET;
		keymap[KeyEvent.KEYCODE_BACKSLASH] = SDLKey.SDLK_BACKSLASH;
		keymap[KeyEvent.KEYCODE_SEMICOLON] = SDLKey.SDLK_SEMICOLON;
		keymap[KeyEvent.KEYCODE_APOSTROPHE] = SDLKey.SDLK_QUOTE;
		keymap[KeyEvent.KEYCODE_SLASH] = SDLKey.SDLK_SLASH;
		keymap[KeyEvent.KEYCODE_AT] = SDLKey.SDLK_AT;

		keymap[KeyEvent.KEYCODE_PLUS] = SDLKey.SDLK_PLUS;
	}
	
    public native static int MouseMotion(short buttonstate, int relative, int x, int y);
	
    public native static int MouseButton(short state, short button, int x, int y);
	
	public native static int Keyboard(short state, SDLKeySym key);
	
}
