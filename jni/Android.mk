LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_CPP_EXTENSION := .cpp

LOCAL_SRC_FILES := \
	onLoad.cpp \
	SDLRuntime.cpp \
        android_sdl_SDLVideo.cpp \
	android_sdl_SDLPixelFormat.cpp \
	android_sdl_SDLVideoDevice.cpp \
	android_sdl_SDLSurface.cpp

LOCAL_MODULE := libsdl_jni

LOCAL_C_INCLUDES := \
        $(JNI_H_INCLUDE) \
        external/sdl/include \
        external/sdl/libs/sdl_1.2/src/audio/android \
        external/sdl/libs/sdl_1.2/src/video/android \
	external/skia/src/core \
	external/skia/include/core \
	frameworks/base/include
        
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
