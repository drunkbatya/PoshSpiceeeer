#pragma once
#include <stdint.h>
#include <stdbool.h>
#include "display.h"

typedef struct {
    uint8_t x;
    uint8_t y;
    uint8_t current_max_char;
    uint8_t freq;
    const char* str;
    uint32_t timer_compare_value;
} StringAnimation;

StringAnimation* string_animation_alloc(void);
void string_animation_free(StringAnimation* string_animation);
void string_animation_draw_string(StringAnimation* string_animation, Display* display);
void string_animation_set_string(
    StringAnimation* string_animation,
    const char* str,
    uint8_t x,
    uint8_t y,
    uint8_t freq);
void string_animation_reset(StringAnimation* string_animation);
void string_animation_timer_process(StringAnimation* string_animation);
