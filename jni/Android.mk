LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

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
	android_sdl_events_SDLEvents.cpp \

ifeq ($(SDL_BUILD_VERSION), 2)
LOCAL_SRC_FILES += \
	android_sdl_SDLWindow.cpp \
	android_sdl_SDLDisplayMode.cpp
endif

LOCAL_MODULE := libsdl_jni

LOCAL_C_INCLUDES := \
        $(JNI_H_INCLUDE) \
        $(SDL_INCLUDE) \
        dalvik/libnativehelper/include/nativehelper

LOCAL_SHARED_LIBRARIES := \
	liblog \
        libsdl \
        libnativehelper \
        libutils

ifeq ($(SDL_BUILD_VERSION), 1)
LOCAL_C_INCLUDES += \
        external/sdl/libs/sdl_1.2.0/src/audio/android \
        external/sdl/libs/sdl_1.2.0/src/video/android \
        external/skia/src/core \
	external/skia/include/core \
	frameworks/base/include

LOCAL_SHARED_LIBRARIES += \
        libskia \
	libsurfaceflinger_client \
	libmedia
else
LOCAL_C_INCLUDES += \
        external/sdl/libs/sdl_1.3.0/src/audio/android \
        external/sdl/libs/sdl_1.3.0/src/video/android
endif

TARGET_PRELINK_MODULE := false

include $(BUILD_SHARED_LIBRARY)
