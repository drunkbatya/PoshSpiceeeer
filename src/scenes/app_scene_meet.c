#include <stm32f4xx_hal_def.h>
#include <stm32f4xx_ll_utils.h>
#include "display.h"
#include "animation.h"
#include "app.h"
#include "app_scene.h"
#include "assets_icons.h"

static void app_scene_meet_draw_callback(void* context) {
    App* app = context;
    display_clear_framebuffer(app->display);
    display_draw_icon(app->display, &I_Meet_128x64, 0, 0);
    // animation_draw_current_frame(app->animation, app->display);
    string_animation_draw_string(app->string_animation, app->display);
}

void app_scene_meet_on_enter(void* context) {
    App* app = context;
    // animation_set_animation(app->animation, &A_Sorry_128x64, 0, 0, false);
    string_animation_set_string(
        app->string_animation, "Scene Meet\n     is under\nconstruction", 37, 9, 10);
    display_set_draw_callback(app->display, app_scene_meet_draw_callback, app);
}

void app_scene_meet_on_event(void* context, InputEvent event) {
    App* app = context;
    UNUSED(app);
}
void app_scene_meet_on_exit(void* context) {
    App* app = context;
    string_animation_reset(app->string_animation);
    animation_reset_animation(app->animation);
    display_reset_draw_callback(app->display);
    display_clear_framebuffer(app->display);
}
