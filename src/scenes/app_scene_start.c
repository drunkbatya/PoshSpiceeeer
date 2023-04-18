#include <stm32f4xx_hal_def.h>
#include <stm32f4xx_ll_utils.h>
#include "display.h"
#include "animation.h"
#include "app.h"
#include "app_scene.h"
#include "assets_icons.h"

static void app_scene_start_draw_callback(void* context) {
    App* app = context;
    display_clear_framebuffer(app->display);
    animation_draw_current_frame(app->animation, app->display);
    display_draw_button_right(app->display, "Next");
}

void app_scene_start_on_enter(void* context) {
    App* app = context;
    // display_draw_image(app->display, _I_ButtonLeft_4x7_0, 8);
    //display_draw_pixel(app->display, 1, 15);
    animation_set_animation(app->animation, &A_Start_128x64, 0, 0, true);
    display_set_draw_callback(app->display, app_scene_start_draw_callback, app);
}

void app_scene_start_on_event(void* context, InputEvent event) {
    App* app = context;
    if(event == INPUT_EVENT_RIGHT_PRESSED)
        scene_manager_next_scene(app->scene_manager, SceneCookie);
}
void app_scene_start_on_exit(void* context) {
    App* app = context;
    animation_reset_animation(app->animation);
    display_reset_draw_callback(app->display);
    display_clear_framebuffer(app->display);
}
