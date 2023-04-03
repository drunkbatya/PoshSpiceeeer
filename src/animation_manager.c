#include <stdlib.h>
#include "animation_manager.h"

AnimationManager* animation_manager_alloc() {
    AnimationManager* animation_manager = malloc(sizeof(AnimationManager));
    return animation_manager;
}

void animation_manager_free(AnimationManager* animation_manager) {
    free(animation_manager);
}
