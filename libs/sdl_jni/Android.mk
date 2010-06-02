LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_CPP_EXTENSION := .cpp

LOCAL_SRC_FILES := \
        onload.cpp \
        android_sdl_SDLSurfaceView.cpp \
	android_sdl_LibraryLoader.cpp

LOCAL_MODULE := libsdl_jni

LOCAL_C_INCLUDES := \
        $(JNI_H_INCLUDE) \
        $(LOCAL_PATH)/../../include \
        external/sdl/libs/sdl_1.2/src/audio/android \
        external/sdl/libs/sdl_1.2/src/video/android \
	external/skia/src/core \
	external/skia/include/core \
	frameworks/base/include
        
LOCAL_SHARED_LIBRARIES := \
	libskia \
	libui \
	libmedia \
	liblog \
        libdl \
        libsdl \
        libnativehelper \
        libutils

TARGET_PRELINK_MODULE := false

include $(BUILD_SHARED_LIBRARY)
