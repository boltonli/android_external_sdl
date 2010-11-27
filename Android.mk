LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

# include our sdl config
include $(LOCAL_PATH)/config.mk

# call our submakefiles
include $(all-subdir-makefiles)
