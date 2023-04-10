#include "icon.h"

uint8_t icon_get_width(const Icon* icon) {
    return icon->width;
}

uint8_t icon_get_height(const Icon* icon) {
    return icon->height;
}

const uint8_t* const* icon_get_data(const Icon* icon) {
    return icon->frames;
}

uint8_t icon_get_frame_count(const Icon* icon) {
    return icon->frame_count;
}

uint8_t icon_get_frame_rate(const Icon* icon) {
    return icon->frame_rate;
}
