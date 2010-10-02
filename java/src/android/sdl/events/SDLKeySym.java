/*
**
** Copyright 2010; Havlena Petr
**
** Licensed under the Apache License; Version 2.0 (the "License"); 
** you may not use this file except in compliance with the License. 
** You may obtain a copy of the License at 
**
**     http://www.apache.org/licenses/LICENSE-2.0 
**
** Unless required by applicable law or agreed to in writing; software 
** distributed under the License is distributed on an "AS IS" BASIS; 
** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND; either express or implied. 
** See the License for the specific language governing permissions and 
** limitations under the License.
*/

package android.sdl.events;

public class SDLKeySym {
	
	public int scancode;
	public int sym;
	public int mod;
	public int unicode;
	
	/** What we really want is a mapping of every raw key on the keyboard.
	 *  To support international keyboards; we use the range 0xA1 - 0xFF
	 *  as international virtual keycodes.  We'll follow in the footsteps of X11...
	 *  @brief The names of the keys
	 */
	public static class SDLKey {
        /** @name ASCII mapped keysyms
         *  The keyboard syms have been cleverly chosen to map to ASCII
         */
        /*@{*/
		public static int SDLK_UNKNOWN		= 0;
		public static int SDLK_FIRST		= 0;
		public static int SDLK_BACKSPACE		= 8;
		public static int SDLK_TAB		= 9;
		public static int SDLK_CLEAR		= 12;
		public static int SDLK_RETURN		= 13;
		public static int SDLK_PAUSE		= 19;
		public static int SDLK_ESCAPE		= 27;
		public static int SDLK_SPACE		= 32;
		public static int SDLK_EXCLAIM		= 33;
		public static int SDLK_QUOTEDBL		= 34;
		public static int SDLK_HASH		= 35;
		public static int SDLK_DOLLAR		= 36;
		public static int SDLK_AMPERSAND		= 38;
		public static int SDLK_QUOTE		= 39;
		public static int SDLK_LEFTPAREN		= 40;
		public static int SDLK_RIGHTPAREN		= 41;
		public static int SDLK_ASTERISK		= 42;
		public static int SDLK_PLUS		= 43;
		public static int SDLK_COMMA		= 44;
		public static int SDLK_MINUS		= 45;
		public static int SDLK_PERIOD		= 46;
		public static int SDLK_SLASH		= 47;
		public static int SDLK_0			= 48;
		public static int SDLK_1			= 49;
		public static int SDLK_2			= 50;
		public static int SDLK_3			= 51;
		public static int SDLK_4			= 52;
		public static int SDLK_5			= 53;
		public static int SDLK_6			= 54;
		public static int SDLK_7			= 55;
		public static int SDLK_8			= 56;
		public static int SDLK_9			= 57;
		public static int SDLK_COLON		= 58;
		public static int SDLK_SEMICOLON		= 59;
		public static int SDLK_LESS		= 60;
		public static int SDLK_EQUALS		= 61;
		public static int SDLK_GREATER		= 62;
		public static int SDLK_QUESTION		= 63;
		public static int SDLK_AT			= 64;
		/* 
		 Skip uppercase letters
		 */
		public static int SDLK_LEFTBRACKET	= 91;
		public static int SDLK_BACKSLASH		= 92;
		public static int SDLK_RIGHTBRACKET	= 93;
		public static int SDLK_CARET		= 94;
		public static int SDLK_UNDERSCORE		= 95;
		public static int SDLK_BACKQUOTE		= 96;
		public static int SDLK_a			= 97;
		public static int SDLK_b			= 98;
		public static int SDLK_c			= 99;
		public static int SDLK_d			= 100;
		public static int SDLK_e			= 101;
		public static int SDLK_f			= 102;
		public static int SDLK_g			= 103;
		public static int SDLK_h			= 104;
		public static int SDLK_i			= 105;
		public static int SDLK_j			= 106;
		public static int SDLK_k			= 107;
		public static int SDLK_l			= 108;
		public static int SDLK_m			= 109;
		public static int SDLK_n			= 110;
		public static int SDLK_o			= 111;
		public static int SDLK_p			= 112;
		public static int SDLK_q			= 113;
		public static int SDLK_r			= 114;
		public static int SDLK_s			= 115;
		public static int SDLK_t			= 116;
		public static int SDLK_u			= 117;
		public static int SDLK_v			= 118;
		public static int SDLK_w			= 119;
		public static int SDLK_x			= 120;
		public static int SDLK_y			= 121;
		public static int SDLK_z			= 122;
		public static int SDLK_DELETE		= 127;
		/* End of ASCII mapped keysyms */
        /*@}*/
		
		/** @name International keyboard syms */
        /*@{*/
		public static int SDLK_WORLD_0		= 160;		/* 0xA0 */
		public static int SDLK_WORLD_1		= 161;
		public static int SDLK_WORLD_2		= 162;
		public static int SDLK_WORLD_3		= 163;
		public static int SDLK_WORLD_4		= 164;
		public static int SDLK_WORLD_5		= 165;
		public static int SDLK_WORLD_6		= 166;
		public static int SDLK_WORLD_7		= 167;
		public static int SDLK_WORLD_8		= 168;
		public static int SDLK_WORLD_9		= 169;
		public static int SDLK_WORLD_10		= 170;
		public static int SDLK_WORLD_11		= 171;
		public static int SDLK_WORLD_12		= 172;
		public static int SDLK_WORLD_13		= 173;
		public static int SDLK_WORLD_14		= 174;
		public static int SDLK_WORLD_15		= 175;
		public static int SDLK_WORLD_16		= 176;
		public static int SDLK_WORLD_17		= 177;
		public static int SDLK_WORLD_18		= 178;
		public static int SDLK_WORLD_19		= 179;
		public static int SDLK_WORLD_20		= 180;
		public static int SDLK_WORLD_21		= 181;
		public static int SDLK_WORLD_22		= 182;
		public static int SDLK_WORLD_23		= 183;
		public static int SDLK_WORLD_24		= 184;
		public static int SDLK_WORLD_25		= 185;
		public static int SDLK_WORLD_26		= 186;
		public static int SDLK_WORLD_27		= 187;
		public static int SDLK_WORLD_28		= 188;
		public static int SDLK_WORLD_29		= 189;
		public static int SDLK_WORLD_30		= 190;
		public static int SDLK_WORLD_31		= 191;
		public static int SDLK_WORLD_32		= 192;
		public static int SDLK_WORLD_33		= 193;
		public static int SDLK_WORLD_34		= 194;
		public static int SDLK_WORLD_35		= 195;
		public static int SDLK_WORLD_36		= 196;
		public static int SDLK_WORLD_37		= 197;
		public static int SDLK_WORLD_38		= 198;
		public static int SDLK_WORLD_39		= 199;
		public static int SDLK_WORLD_40		= 200;
		public static int SDLK_WORLD_41		= 201;
		public static int SDLK_WORLD_42		= 202;
		public static int SDLK_WORLD_43		= 203;
		public static int SDLK_WORLD_44		= 204;
		public static int SDLK_WORLD_45		= 205;
		public static int SDLK_WORLD_46		= 206;
		public static int SDLK_WORLD_47		= 207;
		public static int SDLK_WORLD_48		= 208;
		public static int SDLK_WORLD_49		= 209;
		public static int SDLK_WORLD_50		= 210;
		public static int SDLK_WORLD_51		= 211;
		public static int SDLK_WORLD_52		= 212;
		public static int SDLK_WORLD_53		= 213;
		public static int SDLK_WORLD_54		= 214;
		public static int SDLK_WORLD_55		= 215;
		public static int SDLK_WORLD_56		= 216;
		public static int SDLK_WORLD_57		= 217;
		public static int SDLK_WORLD_58		= 218;
		public static int SDLK_WORLD_59		= 219;
		public static int SDLK_WORLD_60		= 220;
		public static int SDLK_WORLD_61		= 221;
		public static int SDLK_WORLD_62		= 222;
		public static int SDLK_WORLD_63		= 223;
		public static int SDLK_WORLD_64		= 224;
		public static int SDLK_WORLD_65		= 225;
		public static int SDLK_WORLD_66		= 226;
		public static int SDLK_WORLD_67		= 227;
		public static int SDLK_WORLD_68		= 228;
		public static int SDLK_WORLD_69		= 229;
		public static int SDLK_WORLD_70		= 230;
		public static int SDLK_WORLD_71		= 231;
		public static int SDLK_WORLD_72		= 232;
		public static int SDLK_WORLD_73		= 233;
		public static int SDLK_WORLD_74		= 234;
		public static int SDLK_WORLD_75		= 235;
		public static int SDLK_WORLD_76		= 236;
		public static int SDLK_WORLD_77		= 237;
		public static int SDLK_WORLD_78		= 238;
		public static int SDLK_WORLD_79		= 239;
		public static int SDLK_WORLD_80		= 240;
		public static int SDLK_WORLD_81		= 241;
		public static int SDLK_WORLD_82		= 242;
		public static int SDLK_WORLD_83		= 243;
		public static int SDLK_WORLD_84		= 244;
		public static int SDLK_WORLD_85		= 245;
		public static int SDLK_WORLD_86		= 246;
		public static int SDLK_WORLD_87		= 247;
		public static int SDLK_WORLD_88		= 248;
		public static int SDLK_WORLD_89		= 249;
		public static int SDLK_WORLD_90		= 250;
		public static int SDLK_WORLD_91		= 251;
		public static int SDLK_WORLD_92		= 252;
		public static int SDLK_WORLD_93		= 253;
		public static int SDLK_WORLD_94		= 254;
		public static int SDLK_WORLD_95		= 255;		/* 0xFF */
        /*@}*/
		
		/** @name Numeric keypad */
        /*@{*/
		public static int SDLK_KP0		= 256;
		public static int SDLK_KP1		= 257;
		public static int SDLK_KP2		= 258;
		public static int SDLK_KP3		= 259;
		public static int SDLK_KP4		= 260;
		public static int SDLK_KP5		= 261;
		public static int SDLK_KP6		= 262;
		public static int SDLK_KP7		= 263;
		public static int SDLK_KP8		= 264;
		public static int SDLK_KP9		= 265;
		public static int SDLK_KP_PERIOD		= 266;
		public static int SDLK_KP_DIVIDE		= 267;
		public static int SDLK_KP_MULTIPLY	= 268;
		public static int SDLK_KP_MINUS		= 269;
		public static int SDLK_KP_PLUS		= 270;
		public static int SDLK_KP_ENTER		= 271;
		public static int SDLK_KP_EQUALS		= 272;
        /*@}*/
		
		/** @name Arrows + Home/End pad */
        /*@{*/
		public static int SDLK_UP			= 273;
		public static int SDLK_DOWN		= 274;
		public static int SDLK_RIGHT		= 275;
		public static int SDLK_LEFT		= 276;
		public static int SDLK_INSERT		= 277;
		public static int SDLK_HOME		= 278;
		public static int SDLK_END		= 279;
		public static int SDLK_PAGEUP		= 280;
		public static int SDLK_PAGEDOWN		= 281;
        /*@}*/
		
		/** @name Function keys */
        /*@{*/
		public static int SDLK_F1			= 282;
		public static int SDLK_F2			= 283;
		public static int SDLK_F3			= 284;
		public static int SDLK_F4			= 285;
		public static int SDLK_F5			= 286;
		public static int SDLK_F6			= 287;
		public static int SDLK_F7			= 288;
		public static int SDLK_F8			= 289;
		public static int SDLK_F9			= 290;
		public static int SDLK_F10		= 291;
		public static int SDLK_F11		= 292;
		public static int SDLK_F12		= 293;
		public static int SDLK_F13		= 294;
		public static int SDLK_F14		= 295;
		public static int SDLK_F15		= 296;
        /*@}*/
		
		/** @name Key state modifier keys */
        /*@{*/
		public static int SDLK_NUMLOCK		= 300;
		public static int SDLK_CAPSLOCK		= 301;
		public static int SDLK_SCROLLOCK		= 302;
		public static int SDLK_RSHIFT		= 303;
		public static int SDLK_LSHIFT		= 304;
		public static int SDLK_RCTRL		= 305;
		public static int SDLK_LCTRL		= 306;
		public static int SDLK_RALT		= 307;
		public static int SDLK_LALT		= 308;
		public static int SDLK_RMETA		= 309;
		public static int SDLK_LMETA		= 310;
		public static int SDLK_LSUPER		= 311;		/**< Left "Windows" key */
		public static int SDLK_RSUPER		= 312;		/**< Right "Windows" key */
		public static int SDLK_MODE		= 313;		/**< "Alt Gr" key */
		public static int SDLK_COMPOSE		= 314;		/**< Multi-key compose key */
        /*@}*/
		
		/** @name Miscellaneous function keys */
        /*@{*/
		public static int SDLK_HELP		= 315;
		public static int SDLK_PRINT		= 316;
		public static int SDLK_SYSREQ		= 317;
		public static int SDLK_BREAK		= 318;
		public static int SDLK_MENU		= 319;
		public static int SDLK_POWER		= 320;	/**< Power Macintosh power key */
		public static int SDLK_EURO		= 321;		/**< Some european keyboards */
		public static int SDLK_UNDO		= 322;		/**< Atari keyboard has Undo */
        /*@}*/
		
		/* Add any other keys here */
        public static int SDLK_LAST = 323;
	};
	
	public static class SDLMod {
		public static int KMOD_NONE  = 0x0000;
		public static int KMOD_LSHIFT= 0x0001;
		public static int KMOD_RSHIFT= 0x0002;
		public static int KMOD_LCTRL = 0x0040;
		public static int KMOD_RCTRL = 0x0080;
		public static int KMOD_LALT  = 0x0100;
		public static int KMOD_RALT  = 0x0200;
		public static int KMOD_LMETA = 0x0400;
		public static int KMOD_RMETA = 0x0800;
		public static int KMOD_NUM   = 0x1000;
		public static int KMOD_CAPS  = 0x2000;
		public static int KMOD_MODE  = 0x4000;
		public static int KMOD_RESERVED = 0x8000;
	};
	
    public SDLKeySym() {
    }
	
}
