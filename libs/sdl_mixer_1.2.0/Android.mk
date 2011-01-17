LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

ifeq ($(SDL_BUILD_MIXER), true)

LOCAL_CFLAGS := -DANDROID -DWAV_MUSIC -DOGG_USE_TREMOR -DOGG_MUSIC -DIN_NDK

LOCAL_C_INCLUDES := \
	$(LOCAL_PATH)/../sdl_1.2.0/include

ifeq ($(IN_NDK),true)
LOCAL_C_INCLUDES += \
    $(LOCAL_PATH)/../tremor
else
LOCAL_C_INCLUDES += \
    external/tremolo
endif
		
LOCAL_CPP_EXTENSION := .cpp

ifeq ($(IN_NDK),true)
LOCAL_STATIC_LIBRARIES := libtremor_static
else
LOCAL_SHARED_LIBRARIES := libvorbisidec
endif

# Note this simple makefile var substitution, you can find even simpler examples in different Android projects
LOCAL_SRC_FILES := $(notdir $(wildcard $(LOCAL_PATH)/*.c))

LOCAL_MODULE := libsdlmixer_static

include $(BUILD_STATIC_LIBRARY)

endif
