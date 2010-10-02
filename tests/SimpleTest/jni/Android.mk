LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE_TAGS := optional

LOCAL_MODULE := libsimplesdltest

LOCAL_C_INCLUDES := \
	external/sdl/include \
	$(JNI_H_INCLUDE)

LOCAL_SHARED_LIBRARIES := libsdl libutils

LOCAL_SRC_FILES := native_pixel.cpp

# Don't prelink this library.  For more efficient code, you may want
# to add this library to the prelink map and set this to true. However,
# it's difficult to do this for applications that are not supplied as
# part of a system image.

LOCAL_PRELINK_MODULE := false

include $(BUILD_SHARED_LIBRARY)