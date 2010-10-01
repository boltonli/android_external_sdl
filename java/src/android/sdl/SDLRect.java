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

public class SDLRect {
	
    private int mNativePointer;

    private SDLRect() {
    }

    // registers fields and methods
    private static native final void native_init();
    static {
        native_init();
    }	

    private native final void native_finalize();
    @Override
    protected void finalize() {
        native_finalize(); 
    }
	
    public native int getX();
	
    public native int getY();
	
    public native int getW();
	
    public native int getH();

    public native void setX(int value);
	
    public native void setY(int value);
	
    public native void setW(int value);
	
    public native void setH(int value);
	
}
