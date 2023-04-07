#pragma once

#include <m-array.h>
#include <stdint.h>
#include <input.h>

ARRAY_DEF(SceneManagerIdStack, uint32_t, M_DEFAULT_OPLIST);

typedef void (*AppSceneOnEnterCallback)(void* context);
typedef void (*AppSceneOnEventCallback)(void* context, InputEvent event);
typedef void (*AppSceneOnExitCallback)(void* context);

typedef struct {
    const AppSceneOnEnterCallback* on_enter_handlers;
    const AppSceneOnEventCallback* on_event_handlers;
    const AppSceneOnExitCallback* on_exit_handlers;
    const uint32_t scene_num;
} SceneManagerHandlers;

typedef struct {
    SceneManagerIdStack_t scene_id_stack;
    const SceneManagerHandlers* scene_handlers;
    void* context;
} SceneManager;

SceneManager* scene_manager_alloc(const SceneManagerHandlers* app_scene_handlers, void* context);
void scene_manager_free(SceneManager* scene_manager);
void scene_manager_next_scene(SceneManager* scene_manager, uint32_t next_scene_id);
void scene_manager_previous_scene(SceneManager* scene_manager);
void scene_manager_stop(SceneManager* scene_manager);
void scene_manager_send_event_to_scene(SceneManager* scene_manager, InputEvent event);
