#pragma once
#include <stdint.h>
#include <stdbool.h>
#include "display.h"
#include "icon.h"

typedef enum {
    AnimationDirectionForward,
    AnimationDirectionBackward,
} AnimationDirection;

typedef struct {
    const Icon* icon;
    uint8_t current_frame;
    bool reverse_cycle;
    AnimationDirection direction;
} Animation;

Animation* animation_alloc(void);
void animation_free(Animation* animation);
void animation_draw_current_frame(Animation* animation, Display* display);
void animation_set_animation(Animation* animation, const Icon* icon, bool reverse_cycle);
void animation_reset_animation(Animation* animation);
void animation_switch_frame(Animation* animation);
uint8_t animation_get_frame_size(Animation* animation);
