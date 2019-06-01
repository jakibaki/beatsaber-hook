#include <android/log.h>
#include <stdlib.h>
#include <time.h>
#include <stdalign.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>

#include "../shared/inline-hook/inlineHook.h"
#include "../shared/utils/utils.h"

const float left_r = 1.05f;
const float left_g = 0.0f;
const float left_b = 2.188f;

const float right_r = 0.0f;
const float right_g = 1.0f;
const float right_b = 0.0f;

typedef struct __attribute__((__packed__)) {
    float r;
    float g;
    float b;
    float a;
} Color;

MAKE_HOOK(get_colorA, 0x104B6C8, Color, void* self)
{
    Color color = {0};
    color.r = left_r;
    color.g = left_g;
    color.b = left_b;

    return color;
}

MAKE_HOOK(get_colorB, 0x104B820, Color, void* self)
{
    Color color = {0};
    color.r = right_r;
    color.g = right_g;
    color.b = right_b;

    return color;
}

// bg colors
MAKE_HOOK(get_color, 0x10E960C, Color, void* self)
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
    srand(time(NULL));
    INSTALL_HOOK(get_colorA);
    INSTALL_HOOK(get_colorB);
    INSTALL_HOOK(get_color);
    log("custom-colors loaded!");
}
