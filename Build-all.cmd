@echo off

del /Q /S dist obj libs
rename Application.mk Application.mk.orig

echo APP_ABI := all >Application.mk
echo APP_STL := c++_static >>Application.mk
echo APP_BUILD_SCRIPT := Android.mk >>Application.mk
echo APP_PLATFORM := android-28 >>Application.mk
echo APP_BUILD_VERSION := 9 >>Application.mk
@Cmd.exe /C C:\__BuildSource\__LIB__\android-ndk-r20-beta2\ndk-build.cmd NDK_APPLICATION_MK=./Application.mk NDK_PROJECT_PATH=./ APP_BUILD_VERSION=9 -j 4
move libs\arm64-v8a\ascreencap dist\9\arm64-v8a\ascreencap
move libs\armeabi-v7a\ascreencap dist\9\armeabi-v7a\ascreencap
move libs\x86\ascreencap dist\9\x86\ascreencap
move libs\x86_64\ascreencap dist\9\x86_64\ascreencap
del /Q /S obj libs

echo APP_ABI := all >Application.mk
echo APP_STL := c++_static >>Application.mk
echo APP_BUILD_SCRIPT := Android.mk >>Application.mk
echo APP_PLATFORM := android-27 >>Application.mk
echo APP_BUILD_VERSION := 8 >>Application.mk
@Cmd.exe /C C:\__BuildSource\__LIB__\android-ndk-r20-beta2\ndk-build.cmd NDK_APPLICATION_MK=./Application.mk NDK_PROJECT_PATH=./ APP_BUILD_VERSION=8 -j 4
move libs\arm64-v8a\ascreencap dist\8\arm64-v8a\ascreencap
move libs\armeabi-v7a\ascreencap dist\8\armeabi-v7a\ascreencap
move libs\x86\ascreencap dist\8\x86\ascreencap
move libs\x86_64\ascreencap dist\8\x86_64\ascreencap
del /Q /S obj libs

echo APP_ABI := all >Application.mk
echo APP_STL := c++_static >>Application.mk
echo APP_BUILD_SCRIPT := Android.mk >>Application.mk
echo APP_PLATFORM := android-21 >>Application.mk
echo APP_BUILD_VERSION := 5 >>Application.mk
@Cmd.exe /C C:\__BuildSource\__LIB__\android-ndk-r20-beta2\ndk-build.cmd NDK_APPLICATION_MK=./Application.mk NDK_PROJECT_PATH=./ APP_BUILD_VERSION=5 -j 4
move libs\arm64-v8a\ascreencap dist\5\arm64-v8a\ascreencap
move libs\armeabi-v7a\ascreencap dist\5\armeabi-v7a\ascreencap
move libs\x86\ascreencap dist\5\x86\ascreencap
move libs\x86_64\ascreencap dist\5\x86_64\ascreencap
android-elf-cleaner.exe dist\5\armeabi-v7a\ascreencap

del /Q /S obj libs
move Application.mk.orig Application.mk

