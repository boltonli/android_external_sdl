#SDL version 1.2.0 = 1
#SDL version 1.3.0 = 2
SDL_BUILD_VERSION := 1
SDL_BUILD_MIXER := true
SDL_BUILD_STATIC := true

ifeq ($(IN_NDK),true)
LOCAL_PATH := $(call my-dir)
ifeq ($(SDL_BUILD_VERSION), 2)
SDL_INCLUDE := $(LOCAL_PATH)/libs/sdl_1.3.0/include
else
SDL_INCLUDE := $(LOCAL_PATH)/libs/sdl_1.2.0/include
endif
else
ifeq ($(SDL_BUILD_VERSION), 2)
SDL_INCLUDE := external/sdl/libs/sdl_1.3.0/include
else
SDL_INCLUDE := external/sdl/libs/sdl_1.2.0/include
endif
endif # end of IN_NDK
