#pragma once
#include <stdbool.h>

void st7920_graphic_mode(bool enable);
void st7920_update(void);
void st7920_clear_buffer(void);
void st7920_init(void);
