#include <android/log.h>
#include "utils.h"
#include <stdio.h>

#include "inline-hook/inlineHook.h"

struct cs_string {
    char padding[0x8];
    uint len;
    char str[];
};



MAKE_HOOK(BeatmapDataSO_ctor, 0x109D0D4, void, void* _this)
{
    __android_log_write(ANDROID_LOG_INFO, "QuestHook", "BeatmapDataSO_ctor called!");
    BeatmapDataSO_ctor(_this);
}

MAKE_HOOK(BeatmapDataSO_set_BeatmapData, 0x109D0B0, void, void* _this, void* beatmapdata) {
    BeatmapDataSO_set_BeatmapData(_this, beatmapdata); // Calls original
    __android_log_write(ANDROID_LOG_INFO, "QuestHook", "Set_beatmapdata called!");
}


void hook()
{
    INSTALL_HOOK(BeatmapDataSO_ctor);
    INSTALL_HOOK(BeatmapDataSO_set_BeatmapData);
    
    __android_log_write(ANDROID_LOG_INFO, "QuestHook", "done hoooking!");
}

__attribute__((constructor)) void lib_main()
{
    __android_log_write(ANDROID_LOG_INFO, "QuestHook", "Welcome!");
    hook();
}