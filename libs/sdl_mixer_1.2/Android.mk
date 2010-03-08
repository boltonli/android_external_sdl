LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := libsdlmixer

LOCAL_CFLAGS := -DWAV_MUSIC -DOGG_USE_TREMOR -DOGG_MUSIC

LOCAL_C_INCLUDES := $(LOCAL_PATH) $(LOCAL_PATH)/.. $(LOCAL_PATH)/../../include 
		
LOCAL_CPP_EXTENSION := .cpp

LOCAL_STATIC_LIBRARIES := libtremor

# Note this simple makefile var substitution, you can find even simpler examples in different Android projects
LOCAL_SRC_FILES := $(notdir $(wildcard $(LOCAL_PATH)/*.c))

include $(BUILD_STATIC_LIBRARY)

