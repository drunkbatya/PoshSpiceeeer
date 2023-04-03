#pragma once
#include <stdbool.h>

void st7920_graphic_mode(bool enable);
void st7920_init(void);
void st7920_draw_bitmap(const unsigned char* graphic);
