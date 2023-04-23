#include <stm32f4xx_hal_def.h>
#include <stm32f4xx_ll_utils.h>
#include "display.h"
#include "animation.h"
#include "app.h"
#include "app_scene.h"
#include "assets_icons.h"

static bool inverted = false;

static void app_scene_stars_draw_callback(void* context) {
    App* app = context;
    display_clear_framebuffer(app->display);
    animation_draw_current_frame(app->animation, app->display);
    display_draw_icon(app->display, &I_Test_128x64, 0, 0);
    display_draw_button_right(app->display, "Next", inverted);
    display_draw_button_left(app->display, "Back", inverted);
}

void app_scene_stars_on_enter(void* context) {
    App* app = context;
    animation_set_animation(app->animation, &A_Stars_128x64, 0, 0, false);
    display_set_draw_callback(app->display, app_scene_stars_draw_callback, app);
}

void app_scene_stars_on_event(void* context, InputEvent event) {
    App* app = context;
    if(event == INPUT_EVENT_RIGHT_PRESSED)
        scene_manager_next_scene(app->scene_manager, SceneCookie);
    if(event == INPUT_EVENT_LEFT_PRESSED) scene_manager_previous_scene(app->scene_manager);
    if(event == INPUT_EVENT_CENTER_PRESSED) inverted = !inverted;
}
void app_scene_stars_on_exit(void* context) {
    App* app = context;
    animation_reset_animation(app->animation);
    display_reset_draw_callback(app->display);
    display_clear_framebuffer(app->display);
}
