#include <stm32f4xx_hal_def.h>
#include <stm32f4xx_ll_utils.h>
#include "display.h"
#include "animation.h"
#include "app.h"
#include "app_scene.h"
#include "assets_icons.h"

static bool inverted = false;

static void app_scene_dance_draw_callback(void* context) {
    App* app = context;
    display_clear_framebuffer(app->display);
    animation_draw_current_frame(app->animation, app->display);
    string_animation_draw_string(app->string_animation, app->display);
    display_draw_button_right(app->display, "Next", inverted);
    display_draw_button_left(app->display, "Back", inverted);
}

void app_scene_dance_on_enter(void* context) {
    App* app = context;
    const bool animation_reverse_repeat = false;
    const bool animation_play_once = false;
    animation_set_animation(
        app->animation, &A_Dance_128x64, 0, 0, animation_reverse_repeat, animation_play_once);
    string_animation_set_string(
        app->string_animation, "You were\n   always\n stronger\n     than\n   others", 87, 1, 11);
    display_set_draw_callback(app->display, app_scene_dance_draw_callback, app);
}

void app_scene_dance_on_event(void* context, SceneEvent event) {
    App* app = context;
    if(event == SCENE_EVENT_RIGHT_PRESSED) scene_manager_next_scene(app->scene_manager, SceneLera);
    if(event == SCENE_EVENT_LEFT_PRESSED) scene_manager_previous_scene(app->scene_manager);
    if(event == SCENE_EVENT_CENTER_PRESSED) inverted = !inverted;
}
void app_scene_dance_on_exit(void* context) {
    App* app = context;
    string_animation_reset(app->string_animation);
    animation_reset_animation(app->animation);
    display_reset_draw_callback(app->display);
    display_clear_framebuffer(app->display);
}
