LOCAL_PATH:= $(call my-dir)

# testsprite

include $(CLEAR_VARS)

LOCAL_SRC_FILES := testsprite.c

LOCAL_MODULE := testsprite

LOCAL_SYSTEM_SHARED_LIBRARIES := libsdl libc

LOCAL_C_INCLUDES := external/libsdl/include

include $(BUILD_EXECUTABLE)

# checkkeys

include $(CLEAR_VARS)

LOCAL_SRC_FILES := checkkeys.c

LOCAL_MODULE := checkkeys

LOCAL_SYSTEM_SHARED_LIBRARIES := libsdl libc

LOCAL_C_INCLUDES := external/libsdl/include

include $(BUILD_EXECUTABLE)
