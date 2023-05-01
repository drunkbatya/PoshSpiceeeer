#include <stdlib.h>
#include <string.h>
#include <stm32f4xx_ll_bus.h>
#include "line_animation.h"

static void line_animation_timer_stop(LineAnimation* line_animation) {
    LL_TIM_DeInit(TIM4);
    line_animation->timer_compare_value = 0;
}

static void line_animation_timer_setup(LineAnimation* line_animation) {
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM4);
    LL_TIM_SetCounterMode(TIM4, LL_TIM_COUNTERMODE_UP);
    LL_TIM_SetRepetitionCounter(TIM4, 0);
    LL_TIM_SetClockDivision(TIM4, LL_TIM_CLOCKDIVISION_DIV1);
    LL_TIM_SetClockSource(TIM4, LL_TIM_CLOCKSOURCE_INTERNAL);
    LL_TIM_EnableARRPreload(TIM4);
    uint32_t freq_div = 64000000LU / line_animation->freq * 2;
    uint32_t prescaler = freq_div / 0x10000LU;
    uint32_t period = freq_div / (prescaler + 1);
    line_animation->timer_compare_value = period * 50 / 100;
    LL_TIM_SetPrescaler(TIM4, prescaler);
    LL_TIM_SetAutoReload(TIM4, 65535);
    LL_TIM_EnableCounter(TIM4);
}

LineAnimation* line_animation_alloc(void) {
    LineAnimation* line_animation = malloc(sizeof(LineAnimation));
    line_animation->freq = 0;
    line_animation->step = 0;
    line_animation->current_max_dot = 1;
    line_animation->lines_count = 0;
    line_animation->dots_count = 0;
    line_animation->lines = NULL;
    line_animation->timer_compare_value = 0;
    return line_animation;
}

void line_animation_free(LineAnimation* line_animation) {
    line_animation_timer_stop(line_animation);
    free(line_animation);
}

void line_animation_draw_lines(LineAnimation* line_animation, Display* display) {
    line_animation->step = display_draw_line_animation(
        display,
        line_animation->lines,
        line_animation->lines_count,
        line_animation->current_max_dot);
}

static uint8_t line_animation_get_line_dots(Line line) {
    if(line.x_end > line.x_start)
        return (line.x_end - line.x_start);
    else if(line.x_start > line.x_end)
        return (line.x_start - line.x_end);
    else if(line.y_end > line.y_start)
        return (line.y_end - line.y_start);
    else if(line.y_start > line.y_end)
        return (line.y_start - line.y_end);
    else
        return 0;
}

static uint8_t
    line_animation_get_line_animated_dot_count(const Line* const lines, uint16_t lines_count) {
    uint16_t total_dots = 0;
    uint16_t printed_lines = 0;
    while(printed_lines < lines_count) {
        total_dots += line_animation_get_line_dots(lines[printed_lines]);
        printed_lines++;
    }
    return total_dots;
}

void line_animation_set_lines(
    LineAnimation* line_animation,
    const Line* const lines,
    uint16_t lines_count,
    uint8_t freq) {
    line_animation->freq = freq;
    line_animation->current_max_dot = 1;
    line_animation->lines_count = lines_count;
    line_animation->dots_count = line_animation_get_line_animated_dot_count(lines, lines_count);
    line_animation->lines = lines;
    line_animation->timer_compare_value = 0;
    line_animation_timer_setup(line_animation);
}

void line_animation_reset(LineAnimation* line_animation) {
    line_animation->freq = 0;
    line_animation->step = 0;
    line_animation->current_max_dot = 1;
    line_animation->lines_count = 0;
    line_animation->dots_count = 0;
    line_animation->lines = NULL;
    line_animation->timer_compare_value = 0;
    line_animation_timer_stop(line_animation);
}

static void line_animation_inc_max_dot(LineAnimation* line_animation) {
    if(line_animation->lines == NULL) return;
    line_animation->current_max_dot += line_animation->step;
    if(line_animation->current_max_dot > line_animation->dots_count)
        line_animation_timer_stop(line_animation);
}

void line_animation_timer_process(LineAnimation* line_animation) {
    if(line_animation->lines == NULL) return;
    if(LL_TIM_GetCounter(TIM4) > line_animation->timer_compare_value) {
        line_animation_inc_max_dot(line_animation);
        LL_TIM_SetCounter(TIM4, 0);
    }
}
