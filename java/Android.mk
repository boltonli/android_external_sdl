LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

COMMON := src/common
VERSION_1_2 := src/sdl_1_2_0
VERSION_1_3 := src/sdl_1_3_0

LOCAL_SRC_FILES := \
	$(call find-other-java-files, $(COMMON))

ifeq ($(SDL_BUILD_VERSION), 2)
LOCAL_SRC_FILES += \
 	$(call find-other-java-files, $(VERSION_1_3))
else
LOCAL_SRC_FILES += \
	$(call find-other-java-files, $(VERSION_1_2))
endif

LOCAL_MODULE:= cz.havlena.sdl

#LOCAL_MODULE_TAGS := optional

include $(BUILD_JAVA_LIBRARY)

#--------------------------------------
include $(CLEAR_VARS)

LOCAL_MODULE := cz.havlena.sdl.xml

LOCAL_MODULE_TAGS := user

LOCAL_MODULE_CLASS := ETC

# This will install the file in /system/etc/permissions
#
LOCAL_MODULE_PATH := $(TARGET_OUT_ETC)/permissions

LOCAL_SRC_FILES := $(LOCAL_MODULE)

include $(BUILD_PREBUILT)
