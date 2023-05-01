#pragma once
#include <stdint.h>
#include <stdbool.h>
#include "display.h"

typedef struct {
    uint8_t freq;
    uint8_t step;
    uint16_t current_max_dot;
    uint16_t lines_count;
    uint16_t dots_count;
    const Line* lines;
    uint32_t timer_compare_value;
} LineAnimation;

LineAnimation* line_animation_alloc(void);
void line_animation_free(LineAnimation* line_animation);
void line_animation_draw_lines(LineAnimation* line_animation, Display* display);
void line_animation_set_lines(
    LineAnimation* line_animation,
    const Line* const lines,
    uint16_t lines_count,
    uint8_t freq);
void line_animation_reset(LineAnimation* line_animation);
void line_animation_timer_process(LineAnimation* line_animation);
