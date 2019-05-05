LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE    := ascreencap
#LOCAL_CPP_EXTENSION := .cpp
LOCAL_CPP_EXTENSION := .cpp
LOCAL_SRC_FILES := ascreencap-ABitmapLite.cpp ascreencap-AScreenCap.cpp ascreencap-AScreenConf.cpp ascreencap.cpp extern/lz4/lz4.c
LOCAL_CFLAGS := -D_DEBUG=1 -Wall -Wno-unknown-pragmas -fopenmp -fexceptions
LOCAL_CPPFLAGS := -std=c++17
LOCAL_LDFLAGS := -L./fakelib/libs/$(TARGET_ARCH_ABI) -lutils -lgui -lbinder -lomp
LOCAL_LDLIBS := -llog
LOCAL_C_INCLUDES := C:/__BuildSource/__LIB__/android-aosp-header/android-5.1.1_r1/system/core/include C:/__BuildSource/__LIB__/android-aosp-header/android-5.1.1_r1/frameworks/base/include C:/__BuildSource/__LIB__/android-aosp-header/android-5.1.1_r1/frameworks/native/include C:/__BuildSource/__LIB__/android-aosp-header/android-5.1.1_r1/hardware/libhardware/include C:/__BuildSource/__LIB__/android-aosp-header/other/Skia ./  extern  extern/lz4
CBP2NDK_CMDLINE := --ratio 2 --rotate 90 -f OutBmp.bmp
#  -lcutils -lutils -lbinder -lskia -lui -lsurfaceflinger -lsurfaceflinger_ddmconnection
#LOCAL_ALLOW_UNDEFINED_SYMBOLS := true
LOCAL_MODULE_TAGS := eng
LIBCXX_FORCE_REBUILD := false
LOCAL_CPP_FEATURES := rtti
# -D_DEBUG=1
include $(BUILD_EXECUTABLE)
