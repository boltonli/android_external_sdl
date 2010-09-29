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

public class SDLVideoDevice {
	
	private int mNativePointer;
	
	/* The name of this video driver */
	private String mName;
	/* Gamma support */
	private int mGamma;
	private int mIs32bit;
	
	/* Data common to all drivers */
	private String mWmTitle;
	private String mWmIcon;
	private int mOffsetX;
	private int mOffsetY;
	
	/* Driver information flags */
	private int mHandlesAnySize;	/* Driver handles any size video mode */
	
	// registers fields and methods
    private static native final void native_init();
    private native final void native_finalize();
	
	static {
	    native_init();
    }	
	
    private SDLVideoDevice() {
    }
	
	@Override
    protected void finalize() {
	    native_finalize(); 
    }
	
}
