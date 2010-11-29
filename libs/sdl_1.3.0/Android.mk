LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

ifeq ($(SDL_BUILD_VERSION), 2)

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
        src/haptic/*.c \
	src/stdlib/*.c \
	src/thread/*.c \
	src/timer/*.c \
	src/video/*.c \
        src/libm/*.c \
	src/audio/android/*.c \
	src/video/android/*.cpp \
        src/video/android/*.c \
	src/joystick/android/*.c \
	src/cdrom/dummy/*.c \
        src/haptic/dummy/*.c \
	src/atomic/dummy/*.c \
	src/thread/pthread/*.c \
	src/timer/unix/*.c \
	src/loadso/dlopen/*.c \
 
# Note this simple makefile var substitution, you can find even simpler examples in different Android projects
LOCAL_SRC_FILES := $(foreach F, $(SDL_SRCS), $(addprefix $(dir $(F)),$(notdir $(wildcard $(LOCAL_PATH)/$(F)))))

LOCAL_MODULE := libsdl

LOCAL_CFLAGS := \
	-DNO_MALLINFO=1 -DANDROID=1 -DDEBUG_ERROR=1 -DSDL_LOADSO_DLOPEN

LOCAL_C_INCLUDES := \
	$(SDL_INCLUDE) \
	external/skia/src/core \
	external/skia/include/core \
	frameworks/base/include \
        frameworks/base/opengl/include

LOCAL_STATIC_LIBRARIES := libsdlmixer

LOCAL_SHARED_LIBRARIES := \
	libutils \
	libbinder \
	liblog \
	libdl \
        libGLESv1_CM

TARGET_PRELINK_MODULE := false

include $(BUILD_SHARED_LIBRARY)

endif
