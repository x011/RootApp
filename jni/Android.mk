LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := sudo
LOCAL_SRC_FILES := sudo.c

include $(BUILD_SHARED_LIBRARY)
