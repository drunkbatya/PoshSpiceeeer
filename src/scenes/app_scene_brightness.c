#include <stm32f4xx_hal_def.h>
#include "display.h"
#include "animation.h"
#include "app.h"
#include "app_scene.h"
#include "assets_icons.h"

void app_scene_brightness_on_enter(void* context) {
    App* app = context;
    display_draw_icon(app->display, &I_ButtonLeft_4x7, 3, 0);
    display_draw_char(app->display, '!', 20, 0);
    display_draw_char(app->display, '"', 22, 0);
    display_draw_char(app->display, '#', 28, 0);
    display_draw_char(app->display, '$', 35, 0);
    display_draw_char(app->display, '%', 41, 0);
    display_draw_char(app->display, '&', 47, 0);
    display_draw_char(app->display, '\'', 54, 0);

    display_draw_string(app->display, "I'm trying to print string", 0, 11);
    display_draw_string(app->display, "I CAN PRINT ALL LATTERS\nTest!@", 0, 22);
    display_draw_button_left(app->display, "Back");
    // display_draw_string(app->display, "A", 60, 0);
}

void app_scene_brightness_on_event(void* context, InputEvent event) {
    App* app = context;
    if(event == INPUT_EVENT_RIGHT_PRESSED)
        display_set_brightness(app->display, display_get_brightness(app->display) + 5);
    if(event == INPUT_EVENT_LEFT_PRESSED)
        display_set_brightness(app->display, display_get_brightness(app->display) - 5);
    if(event == INPUT_EVENT_CENTER_PRESSED) scene_manager_previous_scene(app->scene_manager);
}
void app_scene_brightness_on_exit(void* context) {
    App* app = context;
    UNUSED(app);
}
