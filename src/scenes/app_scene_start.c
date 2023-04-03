#include <stm32f4xx_hal_def.h>
#include <stm32f4xx_ll_utils.h>
#include "app.h"

void app_scene_start_on_enter(void* context) {
    App* app = context;
    UNUSED(app);
    // display_add_animation(app->display, frames);
    // display_draw_image(app->display, frames[0], 1024);
}

bool app_scene_start_on_event(void* context, SceneManagerEvent event) {
    UNUSED(context);
    UNUSED(event);
    return false;
}
void app_scene_start_on_exit(void* context) {
    App* app = context;
    UNUSED(app);
}
