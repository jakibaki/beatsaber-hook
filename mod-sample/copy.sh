#!/bin/bash
set -e
ndk-build NDK_PROJECT_PATH=. APP_BUILD_SCRIPT=./Android.mk NDK_APPLICATION_MK=./Application.mk
adb push libs/armeabi-v7a/libsample.so /sdcard/Android/data/com.beatgames.beatsaber/files/mods/libsample.so
adb shell am force-stop com.beatgames.beatsaber
#adb shell input keyevent KEYCODE_DPAD_RIGHT
#adb shell input keyevent KEYCODE_SPACE

adb shell am start com.beatgames.beatsaber/com.unity3d.player.UnityPlayerActivity
