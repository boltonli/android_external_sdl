LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

#include our sdl config
include $(LOCAL_PATH)/../config.mk

LOCAL_CFLAGS := -DSDL_BUILD_VERSION=$(SDL_BUILD_VERSION)

LOCAL_CPP_EXTENSION := .cpp

LOCAL_SRC_FILES := \
	onLoad.cpp \
	SDLRuntime.cpp \
        android_sdl_SDLVideo.cpp \
	android_sdl_SDLPixelFormat.cpp \
	android_sdl_SDLVideoDevice.cpp \
	android_sdl_SDLSurface.cpp \
	android_sdl_SDLColor.cpp \
	android_sdl_SDLRect.cpp \
	android_sdl_SDLPalette.cpp \
        android_sdl_impl_SDLImpl.cpp \
	android_sdl_events_SDLEvents.cpp

LOCAL_MODULE := libsdl_jni

LOCAL_C_INCLUDES := \
        $(JNI_H_INCLUDE) \
	external/skia/src/core \
	external/skia/include/core \
	frameworks/base/include

ifeq ($(SDL_BUILD_VERSION), 1)
LOCAL_C_INCLUDES += \
	external/sdl/include \
        external/sdl/libs/sdl_1.2.0/src/audio/android \
        external/sdl/libs/sdl_1.2.0/src/video/android
else
LOCAL_C_INCLUDES += \
	external/sdl/libs/sdl_1.3.0/include \
        external/sdl/libs/sdl_1.3.0/src/audio/android \
        external/sdl/libs/sdl_1.3.0/src/video/android
endif
        
LOCAL_SHARED_LIBRARIES := \
	libskia \
	libsurfaceflinger_client \
	libmedia \
	liblog \
        libsdl \
        libnativehelper \
        libutils

TARGET_PRELINK_MODULE := false

include $(BUILD_SHARED_LIBRARY)
