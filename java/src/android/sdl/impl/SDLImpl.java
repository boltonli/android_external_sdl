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

import android.sdl.SDLVideo;
import android.util.Log;

import java.lang.SecurityException;
import java.lang.UnsatisfiedLinkError;

public final class SDLImpl {
	private static final String TAG = "SDLImpl";
	
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
		
		if(!loadDrivers()) {
			Log.e(TAG, "couldn't load sdl java drivers");
			return false;
		}
		
		Log.d(TAG, "java sdl library loaded");
		return true;
	}
	
	private static boolean loadDrivers() {
		sVideo = new SDLVideo();
		return true;
	}
	
    public static SDLVideo getVideoDriver() {
	    return sVideo;
    }
	
}
