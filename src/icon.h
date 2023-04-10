#pragma once
#include <stdint.h>

typedef struct Icon {
    const uint8_t width;
    const uint8_t height;
    const uint8_t frame_count;
    const uint8_t frame_rate;
    const uint8_t* const* frames;
} Icon;

uint8_t icon_get_width(const Icon* icon);
uint8_t icon_get_height(const Icon* icon);
uint8_t icon_get_frame_count(const Icon* icon);
uint8_t icon_get_frame_rate(const Icon* icon);
const uint8_t* const* icon_get_data(const Icon* icon);
