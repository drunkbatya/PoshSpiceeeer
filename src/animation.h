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
    uint8_t x;
    uint8_t y;
    uint8_t current_frame;
    bool reverse_cycle;
    AnimationDirection direction;
    uint32_t timer_compare_value;
} Animation;

Animation* animation_alloc(void);
void animation_free(Animation* animation);
void animation_draw_current_frame(Animation* animation, Display* display);
void animation_set_animation(
    Animation* animation,
    const Icon* icon,
    uint8_t x,
    uint8_t y,
    bool reverse_cycle);
void animation_reset_animation(Animation* animation);
// returns true if frame switched
bool animation_timer_process(Animation* animation);
