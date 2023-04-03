#pragma once
#include "display.h"

typedef struct {
    Display* display;
} App;

App* app_alloc(void);
void app_run(App* app);
void app_free(App* app);
