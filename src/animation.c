#include <stdlib.h>
#include "animation.h"

Animation* animation_alloc(void) {
    Animation* animation = malloc(sizeof(Animation));
    animation->frames = NULL;
    return animation;
}

void animation_free(Animation* animation) {
    free(animation);
}

const uint8_t* animation_get_current_frame(Animation* animation) {
    if(animation->frames) return animation->frames[animation->current_frame];
    return NULL;
}

void animation_set_animation(
    Animation* animation,
    const uint8_t** const frames,
    uint16_t frame_size,
    uint16_t total_frames,
    uint8_t frame_rate) {
    animation->frames = frames;
    animation->frame_size = frame_size;
    animation->total_frames = total_frames;
    animation->frame_rate = frame_rate;
    animation->current_frame = 0;
}

uint16_t animation_get_frame_size(Animation* animation) {
    return animation->frame_size;
}

void animation_switch_frame(Animation* animation) {
    animation->current_frame++;
    if(animation->current_frame > animation->total_frames) animation->current_frame = 0;
}
