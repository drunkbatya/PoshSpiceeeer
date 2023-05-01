#include <stdlib.h>
#include <string.h>
#include <stm32f4xx_ll_bus.h>
#include "string_animation.h"

static void string_animation_timer_stop(StringAnimation* string_animation) {
    LL_TIM_DeInit(TIM2);
    string_animation->timer_compare_value = 0;
}

static void string_animation_timer_setup(StringAnimation* string_animation) {
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM2);
    LL_TIM_SetCounterMode(TIM2, LL_TIM_COUNTERMODE_UP);
    LL_TIM_SetRepetitionCounter(TIM2, 0);
    LL_TIM_SetClockDivision(TIM2, LL_TIM_CLOCKDIVISION_DIV1);
    LL_TIM_SetClockSource(TIM2, LL_TIM_CLOCKSOURCE_INTERNAL);
    LL_TIM_EnableARRPreload(TIM2);
    uint32_t freq_div = 64000000LU / string_animation->freq * 2;
    uint32_t prescaler = freq_div / 0x10000LU;
    uint32_t period = freq_div / (prescaler + 1);
    string_animation->timer_compare_value = period * 50 / 100;
    LL_TIM_SetPrescaler(TIM2, prescaler);
    LL_TIM_SetAutoReload(TIM2, 65535);
    LL_TIM_EnableCounter(TIM2);
}

StringAnimation* string_animation_alloc(void) {
    StringAnimation* string_animation = malloc(sizeof(StringAnimation));
    string_animation->x = 0;
    string_animation->y = 0;
    string_animation->current_max_char = 1;
    string_animation->freq = 0;
    string_animation->str = NULL;
    string_animation->timer_compare_value = 0;
    return string_animation;
}

void string_animation_free(StringAnimation* string_animation) {
    string_animation_timer_stop(string_animation);
    free(string_animation);
}

void string_animation_draw_string(StringAnimation* string_animation, Display* display) {
    display_draw_string_animation(
        display,
        string_animation->str,
        string_animation->x,
        string_animation->y,
        string_animation->current_max_char);
}

void string_animation_set_string(
    StringAnimation* string_animation,
    const char* str,
    uint8_t x,
    uint8_t y,
    uint8_t freq) {
    string_animation->str = str;
    string_animation->current_max_char = 1;
    string_animation->x = x;
    string_animation->y = y;
    string_animation->freq = freq;
    string_animation_timer_setup(string_animation);
}

void string_animation_reset(StringAnimation* string_animation) {
    string_animation->str = NULL;
    string_animation->current_max_char = 1;
    string_animation->x = 0;
    string_animation->y = 0;
    string_animation->freq = 0;
    string_animation_timer_stop(string_animation);
}

static void string_animation_inc_max_char(StringAnimation* string_animation) {
    if(string_animation->str == NULL) return;
    string_animation->current_max_char++;
    if(string_animation->current_max_char > strlen(string_animation->str))
        string_animation_timer_stop(string_animation);
}

void string_animation_timer_process(StringAnimation* string_animation) {
    if(string_animation->str == NULL) return;
    if(LL_TIM_GetCounter(TIM2) > string_animation->timer_compare_value) {
        string_animation_inc_max_char(string_animation);
        LL_TIM_SetCounter(TIM2, 0);
    }
}
