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

import java.nio.ByteBuffer;

/** This structure should be treated as read-only, except for 'pixels',
 *  which, if not NULL, contains the raw pixel data for the surface.
 */
public class SDLSurface {
	
    private int mNativePointer;
	
    // registers fields and methods
    private static native final void native_init();
    static {
        native_init();
    }
	
    private SDLSurface() {
    }
	
	
    private native final void native_finalize();
    @Override
    protected void finalize() {
	    native_finalize(); 
    }

    public native int getFlags();

    public native SDLPixelFormat getPixelFormat();

    public native int getW();

    public native int getH();

    public native int getPitch();

    public native int getOffset();

    public native SDLRect getClipRect();

    /** Reference count -- used when freeing surface */
    public native int getRefCount();

    /** format version, bumped at every change to invalidate blit maps */
    public native long getFormatVersion();

    public native boolean getPixels(ByteBuffer buffer, int size);

}
