LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_CFLAGS := -DSDL_BUILD_VERSION=$(SDL_BUILD_VERSION) -DIN_NDK=$(IN_NDK)

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

ifeq ($(IN_NDK),true)
LOCAL_SRC_FILES += \
        android/android_nio_utils.cpp
endif

ifeq ($(SDL_BUILD_VERSION), 2)
LOCAL_SRC_FILES += \
	android_sdl_SDLWindow.cpp \
	android_sdl_SDLDisplayMode.cpp
endif

LOCAL_MODULE := libsdl_jni

LOCAL_C_INCLUDES := \
        $(JNI_H_INCLUDE) \
        $(SDL_INCLUDE)

ifeq ($(SDL_BUILD_STATIC),true)
LOCAL_STATIC_LIBRARIES := \
        libsdl_static
else
LOCAL_SHARED_LIBRARIES := \
        libsdl
endif

ifeq ($(IN_NDK),true)
LOCAL_C_INCLUDES += \
        $(LOCAL_PATH)/libnativehelper/include/nativehelper
LOCAL_STATIC_LIBRARIES += \
        libnativehelper_static
LOCAL_LDLIBS := -llog
else
LOCAL_C_INCLUDES += \
        dalvik/libnativehelper/include/nativehelper
LOCAL_SHARED_LIBRARIES += \
        libnativehelper \
        liblog
endif

ifeq ($(SDL_BUILD_VERSION), 1)
LOCAL_C_INCLUDES += \
        $(LOCAL_PATH)/../libs/sdl_1.2.0/src/audio/android \
        $(LOCAL_PATH)/../libs/sdl_1.2.0/src/video/android \

ifeq ($(IN_NDK),false)
LOCAL_C_INCLUDES += \
        external/skia/src/core \
	external/skia/include/core \
	frameworks/base/include
LOCAL_SHARED_LIBRARIES += \
        libskia \
	libsurfaceflinger_client \
	libmedia
endif
else
LOCAL_C_INCLUDES += \
        $(LOCAL_PATH)/../libs/sdl_1.3.0/src/audio/android \
        $(LOCAL_PATH)/../libs/sdl_1.3.0/src/video/android
endif

TARGET_PRELINK_MODULE := false

include $(BUILD_SHARED_LIBRARY)

include $(LOCAL_PATH)/libnativehelper/Android.mk
