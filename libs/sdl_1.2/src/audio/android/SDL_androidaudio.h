
#ifndef _SDL_androidaudio_h
#define _SDL_androidaudio_h

#include "SDL_config.h"

extern "C" {
#include "SDL_audio.h"
#include "../SDL_sysaudio.h"
}

#include <media/AudioSystem.h>
#include <media/AudioTrack.h>
#include <utils/Errors.h>

using namespace android;

#define _THIS	SDL_AudioDevice *self

class SDLAudioDriver {

public:
    SDLAudioDriver();
    static SDL_AudioDevice *onCreateDevice(int devindex);
    static int onAvailable();

    static const char *getDriverName() {
        return "Android";
    }

    static const char *getDriverDescription() {
        return "SDL android audio driver";
    }

private:
    AudioTrack mAudioTrack;
    Uint8 *mBuffer;
    Uint32 mBufferSize;

    status_t initAudioTrack(int rate, int channels, int encoding, int size);

    static void onDeleteDevice(SDL_AudioDevice *device);

    /* Audio driver functions */
    static int onOpenAudio(_THIS, SDL_AudioSpec *spec);
    static void onWaitAudio(_THIS);
    static void onPlayAudio(_THIS);
    static Uint8 *onGetAudioBuf(_THIS);
    static void onCloseAudio(_THIS);

};

#endif /* _SDL_androidaudio_h */
