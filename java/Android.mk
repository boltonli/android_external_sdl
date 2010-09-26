LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_SRC_FILES := $(call all-subdir-java-files)

LOCAL_MODULE:= sdl

LOCAL_MODULE_TAGS := optional

include $(BUILD_JAVA_LIBRARY)
