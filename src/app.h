#pragma once
#include "display.h"
#include "scene_manager.h"
#include "animation_manager.h"

typedef struct {
    Display* display;
    SceneManager* scene_manager;
    AnimationManager* animation_manager;
} App;

App* app_alloc(void);
void app_run(App* app);
void app_free(App* app);
