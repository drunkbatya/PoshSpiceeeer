#include <stm32f4xx_hal_def.h>
#include "display.h"
#include "animation.h"
#include "app.h"
#include "assets_icons.h"

const uint8_t* frames[] = {
    _I_frame_0_0,  _I_frame_1_0,  _I_frame_2_0,  _I_frame_3_0,  _I_frame_4_0,  _I_frame_5_0,
    _I_frame_6_0,  _I_frame_7_0,  _I_frame_8_0,  _I_frame_9_0,  _I_frame_10_0, _I_frame_11_0,
    _I_frame_12_0, _I_frame_13_0, _I_frame_14_0, _I_frame_15_0, _I_frame_16_0, _I_frame_17_0,
    _I_frame_18_0, _I_frame_19_0, _I_frame_20_0, _I_frame_21_0, _I_frame_22_0, _I_frame_23_0,
    _I_frame_24_0, _I_frame_25_0, _I_frame_26_0, _I_frame_27_0, _I_frame_28_0, _I_frame_29_0,
    _I_frame_30_0, _I_frame_31_0, _I_frame_32_0, _I_frame_33_0, _I_frame_34_0, _I_frame_35_0};

static void app_scene_start_draw_callback(void* context) {
    App* app = context;
    display_draw_image(
        app->display,
        animation_get_current_frame(app->animation),
        animation_get_frame_size(app->animation));
    animation_switch_frame(app->animation);
    return;
}

void app_scene_start_on_enter(void* context) {
    App* app = context;
    animation_set_animation(app->animation, frames, 1024, 35, 1);
    display_set_draw_callback(app->display, app_scene_start_draw_callback, app);
}

bool app_scene_start_on_event(void* context, SceneManagerEvent event) {
    UNUSED(context);
    UNUSED(event);
    return false;
}
void app_scene_start_on_exit(void* context) {
    App* app = context;
    display_set_draw_callback(app->display, NULL, NULL);
}
