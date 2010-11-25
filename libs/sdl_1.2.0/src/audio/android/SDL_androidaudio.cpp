#include "SDL_androidaudio.h"
#include <android/log.h>

#define CLASS_PATH "SDL_androidaudio.cpp"

static SDLAudioDriver *thiz;

SDLAudioDriver::SDLAudioDriver() {
    thiz = this;
}

SDL_AudioDevice *SDLAudioDriver::onCreateDevice(int devindex) {
    __android_log_print(ANDROID_LOG_ERROR, CLASS_PATH, "creating audio device");

    SDL_AudioDevice *device;

    /* Initialize all variables that we clean on shutdown */
    device = (SDL_AudioDevice *)SDL_malloc(sizeof(SDL_AudioDevice));
    if ( device ) {
        SDL_memset(device, 0, (sizeof *device));
    }

    if (device == NULL) {
        SDL_OutOfMemory();
        if ( device ) {
            SDL_free(device);
        }
        return(0);
    }

    /* Set the function pointers */
    device->OpenAudio = onOpenAudio;
    device->WaitAudio = onWaitAudio;
    device->PlayAudio = onPlayAudio;
    device->GetAudioBuf = onGetAudioBuf;
    device->CloseAudio = onCloseAudio;
    device->free = onDeleteDevice;

    __android_log_print(ANDROID_LOG_ERROR, CLASS_PATH, "audio device created");
    return device;
}

int SDLAudioDriver::onAvailable() {
    return (thiz != NULL) ? 1 : 0;
}

void SDLAudioDriver::onDeleteDevice(SDL_AudioDevice *device) {
}

status_t SDLAudioDriver::initAudioTrack(int rate, int channels, int encoding, int size) {
    status_t err;
    if((err = mAudioTrack.set(AudioSystem::MUSIC,
                             rate,
                             ( encoding == 1 ) ? AudioSystem::PCM_16_BIT : AudioSystem::PCM_8_BIT,
                             ( channels == 1 ) ? AudioSystem::CHANNEL_OUT_MONO : AudioSystem::CHANNEL_OUT_STEREO, // stereo
                             size, // frameCount
                             0, // flags
                             0, //mixCallback,
                             0,
                             0//1024 // notificationFrames
                             )) != NO_ERROR) {
        goto end;
    }

    mAudioTrack.start();

end:
    return err;
}

/* Audio driver functions */
int SDLAudioDriver::onOpenAudio(_THIS, SDL_AudioSpec *spec) {
    __android_log_print(ANDROID_LOG_ERROR, CLASS_PATH, "opening audio device");

    int result = -1;
    int bytesPerSample = (spec->format & 0xFF) / 8;
    spec->format = ( bytesPerSample == 2 ) ? AUDIO_S16 : AUDIO_S8;

    if((result = thiz->initAudioTrack(spec->freq,
                                      spec->channels,
                                      (bytesPerSample == 2) ? 1 : 0,
                                      spec->size)) != NO_ERROR) {
        __android_log_print(ANDROID_LOG_ERROR, CLASS_PATH, "Error initializing audio (%s)", strerror(result));
        return 1;
    }

    thiz->mBuffer = (Uint8 *) malloc(sizeof(Uint8 *) * spec->size);
    thiz->mBufferSize = spec->size;

    __android_log_print(ANDROID_LOG_ERROR, CLASS_PATH, "audio device opened");
    return(0);
}

void SDLAudioDriver::onWaitAudio(_THIS) {
}

void SDLAudioDriver::onPlayAudio(_THIS) {
    void *buff = thiz->mBuffer;
    int length = thiz->mBufferSize;
    while(length > 0) {
        int r = thiz->mAudioTrack.write(buff, length);
        if(r > 0) {
            buff += r;
            length -= r;
        }
        else {
            __android_log_print(ANDROID_LOG_ERROR, CLASS_PATH, "Can't play audio buffer from %i", r);
            return;
        }
    }
}

Uint8 *SDLAudioDriver::onGetAudioBuf(_THIS) {
    return thiz->mBuffer;
}

void SDLAudioDriver::onCloseAudio(_THIS) {
    if(!thiz->mAudioTrack.stopped()) {
        thiz->mAudioTrack.stop();
    }
}


extern "C" {

AudioBootStrap ANDROIDAUD_bootstrap = {
        SDLAudioDriver::getDriverName(), SDLAudioDriver::getDriverDescription(),
        SDLAudioDriver::onAvailable, SDLAudioDriver::onCreateDevice
};

} //end of extern C
