LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

# Makefile to build the SDL library
# based on Makefile.minimal
SDL_SRCS := \
	src/*.c \
	src/audio/*.c \
	src/cdrom/*.c \
	src/cpuinfo/*.c \
	src/events/*.c \
	src/file/*.c \
	src/joystick/*.c \
	src/stdlib/*.c \
	src/thread/*.c \
	src/timer/*.c \
	src/video/*.c \
	src/audio/android/*.cpp \
	src/video/android/*.cpp \
	src/joystick/dummy/*.c \
	src/cdrom/dummy/*.c \
	src/thread/pthread/*.c \
	src/timer/unix/*.c \
	src/loadso/dummy/*.c \
 
LOCAL_CPP_EXTENSION := .cpp
 
# Note this simple makefile var substitution, you can find even simpler examples in different Android projects
LOCAL_SRC_FILES := $(foreach F, $(SDL_SRCS), $(addprefix $(dir $(F)),$(notdir $(wildcard $(LOCAL_PATH)/$(F)))))

LOCAL_MODULE := libsdl_jni

LOCAL_CFLAGS := \
	-DNO_MALLINFO=1 -DANDROID=1 -DDEBUG_ERROR=1

LOCAL_C_INCLUDES := \
	$(LOCAL_PATH)/../../include \
	$(JNI_H_INCLUDE) \
	external/skia/src/core \
	external/skia/include/core \
	frameworks/base/include

LOCAL_STATIC_LIBRARIES := libsdlmixer

LOCAL_SHARED_LIBRARIES := \
	libskia \
	libui \
	libmedia \
	libutils \
	liblog

TARGET_PRELINK_MODULE := false

include $(BUILD_SHARED_LIBRARY)
