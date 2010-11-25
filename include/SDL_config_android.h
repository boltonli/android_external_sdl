/*
    SDL - Simple DirectMedia Layer
    Copyright (C) 1997-2009 Sam Lantinga

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

    Sam Lantinga
    slouken@libsdl.org
*/

#ifndef _SDL_config_minimal_h
#define _SDL_config_minimal_h

#include "SDL_platform.h"

/* This is the minimal configuration that can be used to build SDL */

#include <stdarg.h>
#include <stdint.h>


#define SDL_AUDIO_DRIVER_ANDROID 1

#define SDL_CDROM_DISABLED 1

#define SDL_JOYSTICK_DISABLED 1

#define SDL_LOADSO_DISABLED 0

#define SDL_THREAD_PTHREAD 1
#define SDL_THREAD_PTHREAD_RECURSIVE_MUTEX 1

#define SDL_TIMER_UNIX 1

#define SDL_VIDEO_DRIVER_ANDROID 1

#define HAVE_STDIO_H 1

#undef NO_SDL_GLEXT

/* FireSlash found that SDL native memcpy crashes sometimes, these defines fix it */
#define HAVE_MEMSET 1
#define HAVE_MEMCPY 1
#define HAVE_MEMMOVE 1

/* Enable OpenGL support */
#define SDL_VIDEO_OPENGL 1

#define GL_ALL_ATTRIB_BITS                  0x000fffff
#define GL_UNPACK_ROW_LENGTH                0x0CF2

#ifdef __cplusplus
extern "C" {
#endif

typedef double GLdouble;
typedef double GLclampd;

#ifdef __cplusplus
}
#endif

#endif /* _SDL_config_minimal_h */
