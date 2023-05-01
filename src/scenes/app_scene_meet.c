#include <stm32f4xx_hal_def.h>
#include <stm32f4xx_ll_utils.h>
#include "display.h"
#include "animation.h"
#include "app.h"
#include "app_scene.h"
#include "assets_icons.h"

// x_start, y_start, x_end, y_end, step
const Line lines[] = {
    {0, 58, 5, 58, 1},
    {5, 58, 5, 53, 1},
    {5, 53, 10, 53, 1},
    {10, 53, 10, 48, 1},
    {10, 48, 15, 48, 1},
    {15, 48, 15, 43, 1},
    {15, 43, 128, 43, 4},
    {100, 43, 100, 6, 2},
    {100, 6, 120, 6, 2},
    {120, 6, 120, 43, 2},
    {100, 25, 104, 25, 1}};

static void app_scene_meet_draw_callback(void* context) {
    App* app = context;
    display_clear_framebuffer(app->display);
    line_animation_draw_lines(app->line_animation, app->display);
    string_animation_draw_string(app->string_animation, app->display);
}

void app_scene_meet_on_enter(void* context) {
    App* app = context;
    line_animation_set_lines(app->line_animation, lines, (sizeof(lines) / sizeof(Line)), 17);
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
    line_animation_reset(app->line_animation);
    string_animation_reset(app->string_animation);
    display_reset_draw_callback(app->display);
    display_clear_framebuffer(app->display);
}
