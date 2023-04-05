#pragma once
#include <stdint.h>

typedef struct {
    uint8_t frame_rate;
    const uint8_t** frames;
    uint16_t current_frame;
    uint16_t total_frames;
    uint16_t frame_size;
} Animation;

Animation* animation_alloc(void);
void animation_free(Animation* animation);
const uint8_t* animation_get_current_frame(Animation* animation);
void animation_set_animation(
    Animation* animation,
    const uint8_t** const frames,
    uint16_t frame_size,
    uint16_t total_frames,
    uint8_t frame_rate);
void animation_reset_animation(Animation* animation);
void animation_switch_frame(Animation* animation);
uint16_t animation_get_frame_size(Animation* animation);
