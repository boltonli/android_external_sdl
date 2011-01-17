LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

ifeq ($(SDL_BUILD_VERSION), 1)

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
        src/video/android/*.cpp \
	src/joystick/dummy/*.c \
	src/cdrom/dummy/*.c \
	src/thread/pthread/*.c \
	src/timer/unix/*.c \
	src/loadso/dlopen/*.c \

ifeq ($(IN_NDK),true)
SDL_SRCS += \
	src/audio/dummy/*.cpp
else
SDL_SRCS += \
        src/audio/android/*.cpp
endif
 
LOCAL_CPP_EXTENSION := .cpp
 
# Note this simple makefile var substitution, you can find even simpler examples in different Android projects
LOCAL_SRC_FILES := $(foreach F, $(SDL_SRCS), $(addprefix $(dir $(F)),$(notdir $(wildcard $(LOCAL_PATH)/$(F)))))

LOCAL_CFLAGS := \
	-DNO_MALLINFO=1 -DANDROID=1 -DDEBUG_ERROR=1 -DSDL_LOADSO_DLOPEN

ifeq ($(IN_NDK),true)
LOCAL_C_INCLUDES := \
	$(SDL_INCLUDE)

LOCAL_LDLIBS := -lGLESv1_CM -ldl -llog
else
LOCAL_C_INCLUDES := \
	$(SDL_INCLUDE) \
	external/skia/src/core \
	external/skia/include/core \
	frameworks/base/include \
        frameworks/base/opengl/include

LOCAL_SHARED_LIBRARIES := \
	libskia \
	libmedia \
	libutils \
	libbinder \
	liblog \
	libdl

TARGET_PRELINK_MODULE := false
endif

LOCAL_STATIC_LIBRARIES := libsdlmixer_static

ifeq ($(SDL_BUILD_STATIC),true)
LOCAL_MODULE := libsdl_static
include $(BUILD_STATIC_LIBRARY)
else
LOCAL_MODULE := libsdl
include $(BUILD_SHARED_LIBRARY)
endif

endif
