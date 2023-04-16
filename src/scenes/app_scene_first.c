#include <stm32f4xx_hal_def.h>
#include <stm32f4xx_ll_utils.h>
#include "display.h"
#include "animation.h"
#include "app.h"
#include "app_scene.h"
#include "assets_icons.h"

static void app_scene_first_draw_callback(void* context) {
    App* app = context;
    display_clear_framebuffer(app->display);
    animation_draw_current_frame(app->animation, app->display);
    display_draw_button_right(app->display, "Next");
    animation_switch_frame(app->animation); // TODO: timer
    LL_mDelay(300);
}

void app_scene_first_on_enter(void* context) {
    App* app = context;
    animation_set_animation(app->animation, &A_L1_Second_128x64, 0, 0, true);
    display_set_draw_callback(app->display, app_scene_first_draw_callback, app);
}

void app_scene_first_on_event(void* context, InputEvent event) {
    App* app = context;
    if(event == INPUT_EVENT_LEFT_PRESSED) scene_manager_previous_scene(app->scene_manager);
    if(event == INPUT_EVENT_RIGHT_PRESSED)
        scene_manager_next_scene(app->scene_manager, SceneBrightness);
}
void app_scene_first_on_exit(void* context) {
    App* app = context;
    animation_reset_animation(app->animation);
    display_reset_draw_callback(app->display);
    display_clear_framebuffer(app->display);
}
