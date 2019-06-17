#include <android/log.h>
#include <stdlib.h>
#include <time.h>
#include <stdalign.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>

#include "../shared/inline-hook/inlineHook.h"
#include "../shared/utils/utils.h"

float left_r = 0.0f;
float left_g = 0.0f;
float left_b = 0.0f;

float right_r = 0.0f;
float right_g = 0.0f;
float right_b = 0.0f;

typedef struct __attribute__((__packed__)) {
    float r;
    float g;
    float b;
    float a;
} Color;

MAKE_HOOK(get_colorA, 0x130C350, Color, void* self)
{
    Color color = {0};
    color.r = left_r;
    color.g = left_g;
    color.b = left_b;

    return color;
}

MAKE_HOOK(get_colorB, 0x130C4A8, Color, void* self)
{
    Color color = {0};
    color.r = right_r;
    color.g = right_g;
    color.b = right_b;

    return color;
}

// bg colors
MAKE_HOOK(get_color, 0x12DC59C, Color, void* self)
{
    Color color = get_color(self);
    if (fabs(color.r - 0.188235f) < 0.001 && fabs(color.g - 0.619608f) < 0.001 && color.b == 1.0f) {
        color.r = right_r;
        color.g = right_g;
        color.b = right_b;
    } else if (fabs(color.r - 0.941176f) < 0.001 && fabs(color.g - 0.188235f) < 0.001 && fabs(color.b - 0.188235) < 0.001) {
        color.r = left_r;
        color.g = left_g;
        color.b = left_b;
    }
    return color;
}

__attribute__((constructor)) void lib_main()
{
    FILE* config = fopen("/sdcard/Android/data/com.beatgames.beatsaber/files/mods/customcolors.cfg", "r");
    fscanf(config, "%f %f %f %f %f %f", &left_r, &left_g, &left_b, &right_r, &right_g, &right_b);
    fclose(config);

    INSTALL_HOOK(get_colorA);
    INSTALL_HOOK(get_colorB);
    INSTALL_HOOK(get_color);
    log("custom-colors loaded!");
}
