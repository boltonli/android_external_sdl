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

/*
** This class is only frontend of SDl_PixelFormat native struct
*/
public class SDLPixelFormat {
	
	private int mNativePointer;
	
	//private SDLPalette mPalette;
	public native void setBitsPerPixel(short value);
	public native short getBitsPerPixel();

	public native void setBytesPerPixel(short value);
	public native short getBytesPerPixel();

	public native void setRloss(short value);
	public native short getRloss();
	
	public native void setGloss(short value);
	public native short getGloss();

	public native void setBloss(short value);
	public native short getBloss();

	public native void setAloss(short value);
	public native short getAloss();

	public native void setRshift(short value);
	public native short getRshift();

	public native void setGshift(short value);
	public native short getGshift();

	public native void setBshift(short value);
	public native short getBshift();

	public native void setAshift(short value);
	public native short getAshift();

	public native void setRmask(long value);
	public native long getRmask();

	public native void setGmask(long value);
	public native long getGmask();

	public native void setBmask(long value);
	public native long getBmask();

	public native void setAmask(long value);
	public native long getAmask();

	/** RGB color key information */
	public native void setColorKey(long value);
	public native long getColorKey();
	/** Alpha value information (per-surface alpha) */
	public native void setAlpha(short value);
	public native short getAlpha();
	
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
