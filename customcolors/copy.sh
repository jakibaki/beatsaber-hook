#!/usr/bin/env bash
set -e
ndk-build NDK_PROJECT_PATH=. APP_BUILD_SCRIPT=./Android.mk NDK_APPLICATION_MK=./Application.mk
adb push libs/armeabi-v7a/libcustomcolors.so /sdcard/Android/data/com.beatgames.beatsaber/files/mods/libcustomcolors.so
adb push customcolors.cfg /sdcard/Android/data/com.beatgames.beatsaber/files/mods/customcolors.cfg
adb shell am force-stop com.beatgames.beatsaber
#adb shell input keyevent KEYCODE_DPAD_RIGHT
#adb shell input keyevent KEYCODE_SPACE

adb shell am start com.beatgames.beatsaber/com.unity3d.player.UnityPlayerActivity
