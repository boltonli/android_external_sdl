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

    This file written by Ryan C. Gordon (icculus@icculus.org)
*/
extern "C" 
{
	
#include "SDL_config.h"

/* Output audio to nowhere... */
#include "SDL_rwops.h"
#include "SDL_timer.h"
#include "SDL_audio.h"
#include "../SDL_audiomem.h"
#include "../SDL_audio_c.h"
#include "../SDL_audiodev_c.h"
#include "SDL_mutex.h"
#include "SDL_thread.h"

}

static const char *CLASS_PATH = "SDL_androidaudio.cpp";

#include "SDL_androidaudio.h"

#include <stdio.h>
#include <stdlib.h>
#include <media/AudioSystem.h>
#include <media/AudioTrack.h>
#include <utils/Errors.h>
#include <android/log.h>

using namespace android;

AudioTrack audioTrack;

static int startAudioTrack(int rate, int channels, int encoding, int size);
static void stopAudioTrack();
static void playAudioTrack(void *buffer, int length);

extern "C" 
{

#define ANDROIDAUD_DRIVER_NAME         "android"

/* Audio driver functions */
static int ANDROIDAUD_OpenAudio(_THIS, SDL_AudioSpec *spec);
static void ANDROIDAUD_WaitAudio(_THIS);
static void ANDROIDAUD_PlayAudio(_THIS);
static Uint8 *ANDROIDAUD_GetAudioBuf(_THIS);
static void ANDROIDAUD_CloseAudio(_THIS);

/* Audio driver bootstrap functions */
static 
int ANDROIDAUD_Available(void)
{
	return(1);
}

static 
void ANDROIDAUD_DeleteDevice(SDL_AudioDevice *device)
{
	SDL_free(device->hidden);
	SDL_free(device);
}

static 
SDL_AudioDevice *ANDROIDAUD_CreateDevice(int devindex)
{
	__android_log_print(ANDROID_LOG_ERROR, CLASS_PATH, "creating audio device");
	
	SDL_AudioDevice *device;

	/* Initialize all variables that we clean on shutdown */
	device = (SDL_AudioDevice *)SDL_malloc(sizeof(SDL_AudioDevice));
	if ( device ) {
		SDL_memset(device, 0, (sizeof *device));
		device->hidden = (struct SDL_PrivateAudioData *)
				SDL_malloc((sizeof *device->hidden));
	}
	if ( (device == NULL) || (device->hidden == NULL) ) {
		SDL_OutOfMemory();
		if ( device ) {
			SDL_free(device);
		}
		return(0);
	}
	SDL_memset(device->hidden, 0, (sizeof *device->hidden));

	/* Set the function pointers */
	device->OpenAudio = ANDROIDAUD_OpenAudio;
	device->WaitAudio = ANDROIDAUD_WaitAudio;
	device->PlayAudio = ANDROIDAUD_PlayAudio;
	device->GetAudioBuf = ANDROIDAUD_GetAudioBuf;
	device->CloseAudio = ANDROIDAUD_CloseAudio;

	device->free = ANDROIDAUD_DeleteDevice;

	__android_log_print(ANDROID_LOG_ERROR, CLASS_PATH, "audio device created");

	return device;
}

AudioBootStrap ANDROIDAUD_bootstrap = {
	ANDROIDAUD_DRIVER_NAME, "SDL Android audio driver",
	ANDROIDAUD_Available, ANDROIDAUD_CreateDevice
};

static 
Uint8 *ANDROIDAUD_GetAudioBuf(_THIS)
{
	//__android_log_print(ANDROID_LOG_ERROR, CLASS_PATH, "getting audio buffer");
	return (self->hidden->mixbuf);
}

static 
void ANDROIDAUD_CloseAudio(_THIS)
{
	__android_log_print(ANDROID_LOG_ERROR, CLASS_PATH, "closing audio device");
	stopAudioTrack();
}

static 
int ANDROIDAUD_OpenAudio(_THIS, SDL_AudioSpec *spec)
{
	__android_log_print(ANDROID_LOG_ERROR, CLASS_PATH, "opening audio device");
	
	int result = -1;
	int bytesPerSample = (spec->format & 0xFF) / 8;
	spec->format = ( bytesPerSample == 2 ) ? AUDIO_S16 : AUDIO_S8;
	
	if((result = startAudioTrack(spec->freq, 
							     spec->channels, 
							     ( bytesPerSample == 2 ) ? 1 : 0, 
							     spec->size)) != NO_ERROR)
	{
		__android_log_print(ANDROID_LOG_ERROR, CLASS_PATH, "Error initializing audio (%s)", strerror(result));
		return 1;
	}
	
	self->hidden->mixbuf = (Uint8 *) malloc(sizeof(Uint8 *) * spec->size);
	self->hidden->mixlen = spec->size;
	
	__android_log_print(ANDROID_LOG_ERROR, CLASS_PATH, "audio device opened");

	return(0);
}

/* This function waits until it is possible to write a full sound buffer */
static 
void ANDROIDAUD_WaitAudio(_THIS)
{
	/* We will block in PlayAudio(), do nothing here */
}

static 
void ANDROIDAUD_PlayAudio(_THIS)
{
	//__android_log_print(ANDROID_LOG_ERROR, CLASS_PATH, "playing audio device");
	
	playAudioTrack(self->hidden->mixbuf, self->hidden->mixlen);
	
	//__android_log_print(ANDROID_LOG_ERROR, CLASS_PATH, "audio device played");
}
	
} //end of extern C

static
void stopAudioTrack()
{
	if(!audioTrack.stopped())
	{
		audioTrack.stop();
	}
}
		
static
int startAudioTrack(int rate, int channels, int encoding, int size)
{
	status_t err;
	if((err = audioTrack.set(AudioSystem::MUSIC,
									rate,
									( encoding == 1 ) ? AudioSystem::PCM_16_BIT : AudioSystem::PCM_8_BIT,
									( channels == 1 ) ? AudioSystem::CHANNEL_OUT_MONO : AudioSystem::CHANNEL_OUT_STEREO, // stereo
									size, // frameCount
									0, // flags
									0, //mixCallback,
									0,
									0//1024 // notificationFrames
									)) != NO_ERROR)
	{
		goto end;
	}

	audioTrack.start();
	
end:
	return err;
}

static
void playAudioTrack(void *buffer, int length)
{
	void *temp = buffer;
	while(length > 0)
	{
		int r = audioTrack.write(buffer, length);
		if(r > 0)
		{
			length -= r;
			temp += r;			
		}
		else
		{
			__android_log_print(ANDROID_LOG_ERROR, CLASS_PATH, "Can't play audio buffer from %i", r);
			return;
		}
	}
}
