LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE_TAGS := optional

LOCAL_MODULE := sdltest

LOCAL_C_INCLUDES := \
	external/sdl/include

LOCAL_SHARED_LIBRARIES := libsdl

LOCAL_SRC_FILES := simpletest.c

include $(BUILD_EXECUTABLE)