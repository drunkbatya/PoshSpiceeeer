#include "scene_manager.h"

SceneManager* scene_manager_alloc(const SceneManagerHandlers* app_scene_handlers, void* context) {
    SceneManager* scene_manager = malloc(sizeof(SceneManager));
    scene_manager->context = context;
    scene_manager->scene_handlers = app_scene_handlers;
    SceneManagerIdStack_init(scene_manager->scene_id_stack);
    return scene_manager;
}

void scene_manager_free(SceneManager* scene_manager) {
    SceneManagerIdStack_clear(scene_manager->scene_id_stack);
    free(scene_manager);
}

void scene_manager_next_scene(SceneManager* scene_manager, uint32_t next_scene_id) {
    if(SceneManagerIdStack_size(scene_manager->scene_id_stack) > 0) {
        uint32_t cur_scene_id = *SceneManagerIdStack_back(scene_manager->scene_id_stack);
        scene_manager->scene_handlers->on_exit_handlers[cur_scene_id](scene_manager->context);
    }
    SceneManagerIdStack_push_back(scene_manager->scene_id_stack, next_scene_id);
    scene_manager->scene_handlers->on_enter_handlers[next_scene_id](scene_manager->context);
}

bool scene_manager_previous_scene(SceneManager* scene_manager) {
    if(SceneManagerIdStack_size(scene_manager->scene_id_stack) > 0) {
        uint32_t cur_scene_id = 0;
        SceneManagerIdStack_pop_back(&cur_scene_id, scene_manager->scene_id_stack);
        if(SceneManagerIdStack_size(scene_manager->scene_id_stack) == 0) {
            scene_manager->scene_handlers->on_exit_handlers[cur_scene_id](scene_manager->context);
            return false;
        }
        uint32_t prev_scene_id = *SceneManagerIdStack_back(scene_manager->scene_id_stack);
        scene_manager->scene_handlers->on_exit_handlers[cur_scene_id](scene_manager->context);
        scene_manager->scene_handlers->on_enter_handlers[prev_scene_id](scene_manager->context);
        return true;
    } else {
        return false;
    }
}

void scene_manager_stop(SceneManager* scene_manager) {
    if(SceneManagerIdStack_size(scene_manager->scene_id_stack) > 0) {
        uint32_t cur_scene_id = *SceneManagerIdStack_back(scene_manager->scene_id_stack);
        scene_manager->scene_handlers->on_exit_handlers[cur_scene_id](scene_manager->context);
    }
}
