APP_PROJECT_PATH 	:= $(call my-dir)
IN_NDK 			:= true
TEST                    := false
STATIC_BUILD            := true
APP_PLATFORM            := android-4
APP_BUILD_SCRIPT        := $(APP_PROJECT_PATH)/Android.mk
APP_MODULES     	:= libnativehelper_static libtremor_static libsdlmixer_static libsdl_jni
ifeq ($(STATIC_BUILD),true)
APP_MODULES     	+= libsdl_static
else
APP_MODULES     	+= libsdl
endif
ifeq ($(TEST),true)
APP_MODULES     	+= libsimplesdltest
endif
