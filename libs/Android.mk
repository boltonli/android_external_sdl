ROOT_FOLDER := $(call my-dir)
include $(CLEAR_VARS)

SDL_BUILD_VERSION := 1.2
SDL_BUILD_MIXER := true

ifeq ($(SDL_BUILD_VERSION),1.2)
    include $(ROOT_FOLDER)/sdl_1.2/Android.mk
else
    include $(ROOT_FOLDER)/sdl_1.3.0/Android.mk
endif

ifeq ($(SDL_BUILD_MIXER),true)
    include $(ROOT_FOLDER)/sdl_mixer_1.2/Android.mk
endif
