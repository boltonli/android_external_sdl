LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE_TAGS := optional

LOCAL_CFLAGS := -DANDROID

LOCAL_MODULE := libsimplesdltest

LOCAL_C_INCLUDES := \
	$(JNI_H_INCLUDE) \
	$(SDL_INCLUDE)

LOCAL_SHARED_LIBRARIES := libsdl

ifeq ($(IN_NDK),true)
LOCAL_LDLIBS := -llog
else
LOCAL_SHARED_LIBRARIES += liblog
endif

ifeq ($(SDL_BUILD_VERSION), 2)
LOCAL_C_INCLUDES += \
	frameworks/base/opengl/include

LOCAL_SRC_FILES := lesson05.cpp

LOCAL_SHARED_LIBRARIES += libGLESv1_CM
else
LOCAL_SRC_FILES := native_pixel.cpp
endif

# Don't prelink this library.  For more efficient code, you may want
# to add this library to the prelink map and set this to true. However,
# it's difficult to do this for applications that are not supplied as
# part of a system image.

LOCAL_PRELINK_MODULE := false

include $(BUILD_SHARED_LIBRARY)
