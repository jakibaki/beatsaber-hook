#include <android/log.h>
#include "utils.h"
#include <stdio.h>

#include "inline-hook/inlineHook.h"

typedef struct {
    char padding[0x8];
    uint len;
    char str[];
} cs_string;


//public static bool VerifySignature(byte[] data, byte[] signatureData); // RVA: 0x10532A8 Offset: 0x10532A8
MAKE_HOOK(DataEncryper_VerifySignature, 0x010532a8, int, void* data, void* signatureData)
{
    return 1;
}


//	public void .ctor(BeatmapLineData[] beatmapLinesData, BeatmapEventData[] beatmapEventData); // RVA: 0x1097B80 Offset: 0x1097B80
MAKE_HOOK(BeatmapData_ctor, 0x1097B80, void, void* _this, void* beatmapLinesData, void* BeatmapEventData) {
    BeatmapData_ctor(_this, beatmapLinesData, BeatmapEventData);
    __android_log_write(ANDROID_LOG_INFO, "QuestHook", "BeatmapData_ctor called!");
}


void hook()
{
    INSTALL_HOOK(DataEncryper_VerifySignature);
    INSTALL_HOOK(BeatmapData_ctor);

    __android_log_write(ANDROID_LOG_INFO, "QuestHook", "done hoooking!");
}

__attribute__((constructor)) void lib_main()
{
    __android_log_write(ANDROID_LOG_INFO, "QuestHook", "Welcome!");
    hook();
}