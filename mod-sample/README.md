# Beatsaber quest (il2cpp) function hooking poc

Uses https://github.com/ele7enxxh/Android-Inline-Hook + some macros for magic

Follow https://koz.io/using-frida-on-android-without-root/ to learn how to inject the library into the beatsaber apk (with the so built from this instead of frida)

The function offsets (as well as infos about the structs) can be obtained with il2cppdumper

```
ndk-build NDK_PROJECT_PATH=. APP_BUILD_SCRIPT=./Android.mk NDK_APPLICATION_MK=./Application.mk
```