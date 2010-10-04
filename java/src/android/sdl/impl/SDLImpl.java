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

package android.sdl.impl;

import android.sdl.SDLSurface;
import android.sdl.SDLVideo;
import android.util.Log;
import android.view.SurfaceView;

import java.lang.SecurityException;
import java.lang.UnsatisfiedLinkError;

public final class SDLImpl {
    private static final String TAG = "SDLImpl";

    /** @name SDL_INIT Flags
     *  These are the flags which may be passed to SDL_Init() -- you should
     *  specify the subsystems which you will be using in your application.
     */
    /*@{*/
    public static int SDL_INIT_TIMER = 0x00000001;
    public static int SDL_INIT_AUDIO = 0x00000010;
    public static int SDL_INIT_VIDEO = 0x00000020;
    public static int SDL_INIT_CDROM = 0x00000100;
    public static int SDL_INIT_JOYSTICK	= 0x00000200;
    public static int SDL_INIT_NOPARACHUTE = 0x00100000;	/**< Don't catch fatal signals */
    public static int SDL_INIT_EVENTTHREAD = 0x01000000;	/**< Not supported on all OS's */
    public static int SDL_INIT_EVERYTHING = 0x0000FFFF;
    /*@}*/
	
	/** @name SDL_Surface Flags
	 *  These are the currently supported flags for the SDL_surface
	 */
	/*@{*/
	
	/** Available for SDL_CreateRGBSurface() or SDL_SetVideoMode() */
	/*@{*/
	public static int SDL_SWSURFACE = 0x00000000;	/**< Surface is in system memory */
	public static int SDL_HWSURFACE = 0x00000001;	/**< Surface is in video memory */
	public static int SDL_ASYNCBLIT = 0x00000004;	/**< Use asynchronous blits if possible */
	/*@}*/
	
	/** Available for SDL_SetVideoMode() */
	/*@{*/
	public static int SDL_ANYFORMAT = 0x10000000;	/**< Allow any video depth/pixel-format */
	public static int SDL_HWPALETTE = 0x20000000;	/**< Surface has exclusive palette */
	public static int SDL_DOUBLEBUF = 0x40000000;	/**< Set up double-buffered video mode */
	public static int SDL_FULLSCREEN = 0x80000000;	/**< Surface is a full screen display */
	public static int SDL_OPENGL = 0x00000002;       /**< Create an OpenGL rendering context */
	public static int SDL_OPENGLBLIT = 0x0000000A;	/**< Create an OpenGL rendering context and use it for blitting */
	public static int SDL_RESIZABLE = 0x00000010;	/**< This video mode may be resized */
	public static int SDL_NOFRAME = 0x00000020;  	/**< No window caption or edge frame */
	/*@}*/
	
	/** Used internally (read-only) */
	/*@{*/
	public static int SDL_HWACCEL = 0x00000100;  	/**< Blit uses hardware acceleration */
	public static int SDL_SRCCOLORKEY = 0x00001000;	/**< Blit uses a source color key */
	public static int SDL_RLEACCELOK = 0x00002000;	/**< Private flag */
	public static int SDL_RLEACCEL = 0x00004000; 	/**< Surface is RLE encoded */
	public static int SDL_SRCALPHA = 0x00010000; 	/**< Blit uses source alpha blending */
	public static int SDL_PREALLOC = 0x01000000; 	/**< Surface uses preallocated memory */
	
	private static SDLVideo sVideo;
	
    private SDLImpl() {
    }
	
	/**
	 * Main entry point of java lib
	 */
	public static boolean load() {
		Log.d(TAG, "loading java sdl library");
		
		try {
			System.loadLibrary("sdl_jni");
		}
		catch (SecurityException ex) {
			Log.e(TAG, "couldn't load sdl native lib due to security exception");
			return false;
		}
		catch (UnsatisfiedLinkError ex) {
			Log.e(TAG, "native sdl library doesn't exists");
			return false;
		}
		Log.d(TAG, "java sdl library loaded");
		return true;
	}
	
	public static SDLVideo registerVideoDriver(SurfaceView surface) {
		sVideo = new SDLVideo(surface);
		return sVideo;
	}
	
    /** 
    *  This function loads the SDL dynamically linked library and initializes 
    *  the subsystems specified by 'flags' (and those satisfying dependencies)
    *  Unless the SDL_INIT_NOPARACHUTE flag is set, it will install cleanup
    *  signal handlers for some commonly ignored fatal signals (like SIGSEGV)
    */
    public native static int Init(long flags);

    /** This function initializes specific SDL subsystems */
    public native static int InitSubSystem(long flags);

    /** This function cleans up specific SDL subsystems */
    public native static void QuitSubSystem(long flags);

    /** 
    *  This function returns mask of the specified subsystems which have
    *  been initialized.
    *  If 'flags' is 0, it returns a mask of all initialized subsystems.
    */
    public native static long WasInit(long flags);

    /** 
    *  This function cleans up all initialized subsystems and unloads the
    *  dynamically linked library.  You should call it upon all exit conditions.
    */
    public native static void Quit();

	/**
	 * Set up a video mode with the specified width, height and bits-per-pixel.
	 * 
	 * If 'bpp' is 0, it is treated as the current display bits per pixel.
	 * 
	 * If SDL_ANYFORMAT is set in 'flags', the SDL library will try to set the
	 * requested bits-per-pixel, but will return whatever video pixel format is
	 * available. The default is to emulate the requested pixel format if it is
	 * not natively available.
	 * 
	 * If SDL_HWSURFACE is set in 'flags', the video surface will be placed in
	 * video memory, if possible, and you may have to call SDL_LockSurface() in
	 * order to access the raw framebuffer. Otherwise, the video surface will be
	 * created in system memory.
	 * 
	 * If SDL_ASYNCBLIT is set in 'flags', SDL will try to perform rectangle
	 * updates asynchronously, but you must always lock before accessing pixels.
	 * SDL will wait for updates to complete before returning from the lock.
	 * 
	 * If SDL_HWPALETTE is set in 'flags', the SDL library will guarantee that
	 * the colors set by SDL_SetColors() will be the colors you get. Otherwise,
	 * in 8-bit mode, SDL_SetColors() may not be able to set all of the colors
	 * exactly the way they are requested, and you should look at the video
	 * surface structure to determine the actual palette. If SDL cannot
	 * guarantee that the colors you request can be set, i.e. if the colormap is
	 * shared, then the video surface may be created under emulation in system
	 * memory, overriding the SDL_HWSURFACE flag.
	 * 
	 * If SDL_FULLSCREEN is set in 'flags', the SDL library will try to set a
	 * fullscreen video mode. The default is to create a windowed mode if the
	 * current graphics system has a window manager. If the SDL library is able
	 * to set a fullscreen video mode, this flag will be set in the surface that
	 * is returned.
	 * 
	 * If SDL_DOUBLEBUF is set in 'flags', the SDL library will try to set up
	 * two surfaces in video memory and swap between them when you call
	 * SDL_Flip(). This is usually slower than the normal single-buffering
	 * scheme, but prevents "tearing" artifacts caused by modifying video memory
	 * while the monitor is refreshing. It should only be used by applications
	 * that redraw the entire screen on every update.
	 * 
	 * If SDL_RESIZABLE is set in 'flags', the SDL library will allow the window
	 * manager, if any, to resize the window at runtime. When this occurs, SDL
	 * will send a SDL_VIDEORESIZE event to you application, and you must
	 * respond to the event by re-calling SDL_SetVideoMode() with the requested
	 * size (or another size that suits the application).
	 * 
	 * If SDL_NOFRAME is set in 'flags', the SDL library will create a window
	 * without any title bar or frame decoration. Fullscreen video modes have
	 * this flag set automatically.
	 * 
	 * This function returns the video framebuffer surface, or NULL if it fails.
	 * 
	 * If you rely on functionality provided by certain video flags, check the
	 * flags of the returned surface to make sure that functionality is
	 * available. SDL will fall back to reduced functionality if the exact flags
	 * you wanted are not available.
	 */
     public native static SDLSurface SetVideoMode(int width, int height, int bpp, long flags);

}
