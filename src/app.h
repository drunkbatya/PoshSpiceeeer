#pragma once
#include "display.h"
#include "scene_manager.h"
#include "animation.h"

typedef struct {
    Display* display;
    SceneManager* scene_manager;
    Animation* animation;
} App;

App* app_alloc(void);
void app_run(App* app);
void app_free(App* app);