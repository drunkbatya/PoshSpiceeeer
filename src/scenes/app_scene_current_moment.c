#include <stm32f4xx_hal_def.h>
#include <stm32f4xx_ll_utils.h>
#include "display.h"
#include "animation.h"
#include "app.h"
#include "app_scene.h"
#include "assets_icons.h"

static bool left_inverted = false;
static bool right_inverted = false;
static bool animation_right_blinking = false;

static void app_scene_current_moment_draw_callback(void* context) {
    App* app = context;
    display_clear_framebuffer(app->display);
    animation_draw_current_frame(app->animation, app->display);
    display_draw_button_right(app->display, "Next", right_inverted);
    display_draw_button_left(app->display, "Back", left_inverted);
}

void app_scene_current_moment_on_enter(void* context) {
    App* app = context;
    const bool animation_reverse_repeat = false;
    const bool animation_play_once = true;
    animation_set_animation(
        app->animation,
        &A_CurrentMoment_128x64,
        0,
        0,
        animation_reverse_repeat,
        animation_play_once);
    display_set_draw_callback(app->display, app_scene_current_moment_draw_callback, app);
}

void app_scene_current_moment_on_event(void* context, SceneEvent event) {
    App* app = context;
    static uint8_t animation_event_counter = 0;
    if(event == SCENE_EVENT_RIGHT_PRESSED) {
        scene_manager_next_scene(app->scene_manager, SceneMeet);
    } else if(event == SCENE_EVENT_LEFT_PRESSED) {
        scene_manager_previous_scene(app->scene_manager);
    } else if(event == SCENE_EVENT_CENTER_PRESSED) {
        left_inverted = !left_inverted;
    } else if(event == SCENE_EVENT_ANIMATION_STOPED) {
        animation_right_blinking = true;
    } else if(event == SCENE_EVENT_ANIMATION_TIMER_ACTION && animation_right_blinking) {
        if((animation_event_counter % 3) == 0) right_inverted = !right_inverted;
        animation_event_counter++;
    }
}
void app_scene_current_moment_on_exit(void* context) {
    App* app = context;
    animation_right_blinking = false;
    right_inverted = false;
    animation_reset_animation(app->animation);
    display_reset_draw_callback(app->display);
    display_clear_framebuffer(app->display);
}
