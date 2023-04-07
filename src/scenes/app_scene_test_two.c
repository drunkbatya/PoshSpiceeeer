#include <stm32f4xx_hal_def.h>
#include "display.h"
#include "animation.h"
#include "assets_icons.h"
#include "app.h"

void app_scene_test_two_on_enter(void* context) {
    App* app = context;
    display_draw_frame(app->display, _I_frame_30_0, 1024);
}

void app_scene_test_two_on_event(void* context, InputEvent event) {
    App* app = context;
    if(event == INPUT_EVENT_LEFT_PRESSED) scene_manager_previous_scene(app->scene_manager);
    if(event == INPUT_EVENT_CENTER_PRESSED) scene_manager_previous_scene(app->scene_manager);
}
void app_scene_test_two_on_exit(void* context) {
    UNUSED(context);
}
