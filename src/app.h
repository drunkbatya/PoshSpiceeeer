#pragma once
#include "display.h"
#include "input.h"
#include "scene_manager.h"
#include "animation.h"
#include "string_animation.h"
#include "line_animation.h"

typedef struct {
    Display* display;
    Input* input;
    SceneManager* scene_manager;
    Animation* animation;
    StringAnimation* string_animation;
    LineAnimation* line_animation;
} App;

App* app_alloc(void);
void app_run(App* app);
void app_free(App* app);
