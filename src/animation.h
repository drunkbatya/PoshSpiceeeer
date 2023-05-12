#pragma once
#include <stdint.h>
#include <stdbool.h>
#include "display.h"
#include "icon.h"
#include "scene_manager.h"

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
    bool running;
    bool one_time;
    AnimationDirection direction;
    uint32_t timer_compare_value;
    SceneEvent scene_event;
} Animation;

Animation* animation_alloc(void);
void animation_free(Animation* animation);
void animation_draw_current_frame(Animation* animation, Display* display);
void animation_set_animation(
    Animation* animation,
    const Icon* icon,
    uint8_t x,
    uint8_t y,
    bool reverse_cycle,
    bool one_time);
void animation_reset_animation(Animation* animation);
void animation_timer_process(Animation* animation);
bool animation_has_new_event(Animation* animation);
SceneEvent animation_get_event(Animation* animation);
