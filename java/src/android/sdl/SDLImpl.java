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

public class SDLImpl {
	
	/** @name SDL_Surface Flags
	 *  These are the currently supported flags for the SDL_surface
	 */
	/*@{*/
	
	/** Available for SDL_CreateRGBSurface() or SDL_SetVideoMode() */
	/*@{*/
	protected static int SDL_SWSURFACE = 0x00000000;	/**< Surface is in system memory */
	protected static int SDL_HWSURFACE = 0x00000001;	/**< Surface is in video memory */
	protected static int SDL_ASYNCBLIT = 0x00000004;	/**< Use asynchronous blits if possible */
	/*@}*/
	
	/** Available for SDL_SetVideoMode() */
	/*@{*/
	protected static int SDL_ANYFORMAT = 0x10000000;	/**< Allow any video depth/pixel-format */
	protected static int SDL_HWPALETTE = 0x20000000;	/**< Surface has exclusive palette */
	protected static int SDL_DOUBLEBUF = 0x40000000;	/**< Set up double-buffered video mode */
	protected static int SDL_FULLSCREEN = 0x80000000;	/**< Surface is a full screen display */
	protected static int SDL_OPENGL = 0x00000002;       /**< Create an OpenGL rendering context */
	protected static int SDL_OPENGLBLIT = 0x0000000A;	/**< Create an OpenGL rendering context and use it for blitting */
	protected static int SDL_RESIZABLE = 0x00000010;	/**< This video mode may be resized */
	protected static int SDL_NOFRAME = 0x00000020;  	/**< No window caption or edge frame */
	/*@}*/
	
	/** Used internally (read-only) */
	/*@{*/
	protected static int SDL_HWACCEL = 0x00000100;  	/**< Blit uses hardware acceleration */
	protected static int SDL_SRCCOLORKEY = 0x00001000;	/**< Blit uses a source color key */
	protected static int SDL_RLEACCELOK = 0x00002000;	/**< Private flag */
	protected static int SDL_RLEACCEL = 0x00004000; 	/**< Surface is RLE encoded */
	protected static int SDL_SRCALPHA = 0x00010000; 	/**< Blit uses source alpha blending */
	protected static int SDL_PREALLOC = 0x01000000; 	/**< Surface uses preallocated memory */

    // Private accessors for native code

    native private static void _nativeClassInit();
    static {
		System.loadLibrary("sdl_jni.so");
	    _nativeClassInit();
    }
	
    private SDLImpl() {
    }
	
	
}
