#include <stdlib.h>
#include <stm32f4xx_ll_bus.h>
#include "animation.h"

static void animation_timer_stop(Animation* animation) {
    LL_TIM_DeInit(TIM3);
    animation->timer_compare_value = 0;
    animation->running = false;
}

static void animation_timer_setup(Animation* animation) {
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM3);
    LL_TIM_SetCounterMode(TIM3, LL_TIM_COUNTERMODE_UP);
    LL_TIM_SetRepetitionCounter(TIM3, 0);
    LL_TIM_SetClockDivision(TIM3, LL_TIM_CLOCKDIVISION_DIV1);
    LL_TIM_SetClockSource(TIM3, LL_TIM_CLOCKSOURCE_INTERNAL);
    LL_TIM_EnableARRPreload(TIM3);
    uint32_t freq_div = 64000000LU / icon_get_frame_rate(animation->icon) * 2;
    uint32_t prescaler = freq_div / 0x10000LU;
    uint32_t period = freq_div / (prescaler + 1);
    animation->timer_compare_value = period * 50 / 100;
    LL_TIM_SetPrescaler(TIM3, prescaler);
    LL_TIM_SetAutoReload(TIM3, 65535);
    LL_TIM_EnableCounter(TIM3);
}

Animation* animation_alloc(void) {
    Animation* animation = malloc(sizeof(Animation));
    animation->icon = NULL;
    animation->x = 0;
    animation->y = 0;
    animation->current_frame = 0;
    animation->direction = AnimationDirectionForward;
    animation->timer_compare_value = 0;
    animation->one_time = false;
    animation->running = false;
    return animation;
}

void animation_free(Animation* animation) {
    animation_timer_stop(animation);
    free(animation);
}

uint8_t animation_get_current_frame(Animation* animation) {
    return animation->current_frame;
}

void animation_draw_current_frame(Animation* animation, Display* display) {
    display_draw_icon_animation(
        display, animation->icon, animation->x, animation->y, animation->current_frame);
}

void animation_set_animation(
    Animation* animation,
    const Icon* icon,
    uint8_t x,
    uint8_t y,
    bool reverse_cycle,
    bool one_time) {
    animation->icon = icon;
    animation->current_frame = 0;
    animation->reverse_cycle = reverse_cycle;
    animation->x = x;
    animation->y = y;
    animation_timer_setup(animation);
    animation->one_time = one_time;
    animation->running = true;
}

void animation_reset_animation(Animation* animation) {
    animation->icon = NULL;
    animation->current_frame = 0;
    animation->x = 0;
    animation->y = 0;
    animation->direction = AnimationDirectionForward;
    animation_timer_stop(animation);
    animation->running = false;
}

static void animation_toggle_direction(Animation* animation) {
    if(animation->direction == AnimationDirectionForward)
        animation->direction = AnimationDirectionBackward;
    else
        animation->direction = AnimationDirectionForward;
}

static void animation_switch_frame(Animation* animation) {
    if(animation->icon == NULL) return;
    if(animation->current_frame == 0 && animation->direction == AnimationDirectionBackward) {
        animation_toggle_direction(animation);
    } else if(
        animation->current_frame == (icon_get_frame_count(animation->icon) - 1) &&
        animation->direction == AnimationDirectionForward) {
        if(animation->one_time) return animation_timer_stop(animation);
        if(animation->reverse_cycle) {
            animation_toggle_direction(animation);
        } else {
            animation->current_frame = 0;
            return;
        }
    }
    if(animation->direction == AnimationDirectionForward) {
        animation->current_frame++;
    } else if(animation->direction == AnimationDirectionBackward) {
        animation->current_frame--;
    }
}

bool animation_timer_process(Animation* animation) {
    bool success = false;
    do {
        if(animation->icon == NULL) break;
        if(!animation->running) break;
        if(LL_TIM_GetCounter(TIM3) > animation->timer_compare_value) {
            animation_switch_frame(animation);
            LL_TIM_SetCounter(TIM3, 0);
            success = true;
        }
    } while(false);
    return success;
}
