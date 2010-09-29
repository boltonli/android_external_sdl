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

public class SDLPixelFormat {
	
	private int mNativePointer;
	
	private SDLPalette mPalette;
	private short mBitsPerPixel;
	private short mBytesPerPixel;
	private short mRloss;
	private short mGloss;
	private short mBloss;
	private short mAloss;
	private short mRshift;
	private short mGshift;
	private short mBshift;
	private short mAshift;
	private long mRmask;
	private long mGmask;
	private long mBmask;
	private long mAmask;
	
	/** RGB color key information */
	private long mColorKey;
	/** Alpha value information (per-surface alpha) */
	private short mAlpha;
	
	// registers fields and methods
    private static native final void native_init();
    private native final void native_finalize();
	
	static {
	    native_init();
    }

    private SDLPixelFormat() {
    }
	
	@Override
    protected void finalize() {
	    native_finalize(); 
    }

}
