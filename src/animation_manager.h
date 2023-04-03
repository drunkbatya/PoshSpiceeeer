#pragma once
#include <stdint.h>

typedef struct {
    uint8_t* const animation;
} AnimationManager;

AnimationManager* animation_manager_alloc();
void animation_manager_free(AnimationManager* animation_manager);
