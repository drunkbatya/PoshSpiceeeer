#include <stm32f4xx_hal_def.h>
#include "display.h"
#include "animation.h"
#include "app.h"
#include "app_scene.h"
#include "assets_icons.h"

void app_scene_brightness_on_enter(void* context) {
    App* app = context;
    UNUSED(app);
}

void app_scene_brightness_on_event(void* context, InputEvent event) {
    App* app = context;
    if(event == INPUT_EVENT_RIGHT_PRESSED) display_set_brightness(app->display, display_get_brightness(app->display) + 5);
    if(event == INPUT_EVENT_LEFT_PRESSED) display_set_brightness(app->display, display_get_brightness(app->display) - 5);
    if(event == INPUT_EVENT_CENTER_PRESSED) scene_manager_previous_scene(app->scene_manager);
}
void app_scene_brightness_on_exit(void* context) {
    App* app = context;
    UNUSED(app);
}
