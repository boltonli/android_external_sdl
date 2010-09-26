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

public class SDLColor {
	
	private short mR;
	private short mG;
	private short mB;
	private short mUnused;

    private SDLColor() {
    }
	
	public short getR() {
		return mR;
	}
	
	public short getG() {
		return mG;
	}
	
	public short getB() {
		return mB;
	}
	
	public short getUnused() {
		return mUnused;
	}
	
}
