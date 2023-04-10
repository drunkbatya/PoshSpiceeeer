#include <stdlib.h>
#include <stdbool.h>
#include <stm32f4xx_ll_gpio.h>
#include <stm32f4xx_ll_spi.h>
#include <stm32f4xx_ll_utils.h>
#include "app.h"
#include "scenes/app_scene.h"

App* app_alloc(void) {
    App* app = malloc(sizeof(App));
    app->display = display_alloc(SPI2, GPIOB, LL_GPIO_PIN_13, TIM1, GPIOA, LL_GPIO_PIN_7);
    app->input = input_alloc(GPIOA, GPIOA, GPIOA, LL_GPIO_PIN_0, LL_GPIO_PIN_1, LL_GPIO_PIN_2);
    app->scene_manager = scene_manager_alloc(&scene_handlers, app);
    app->animation = animation_alloc();
    display_init(app->display);
    return app;
}

void app_run(App* app) {
    display_clear(app->display);
    scene_manager_next_scene(app->scene_manager, SceneStart);
    while(true) {
        display_execute_draw_callback(app->display);
        display_sync_framebuffer(app->display);
        LL_mDelay(150);
        if(input_recive_new_event(app->input)) {
            scene_manager_send_event_to_scene(app->scene_manager, input_get_event(app->input));
        }
    }
}

void app_free(App* app) {
    animation_free(app->animation);
    scene_manager_stop(app->scene_manager);
    scene_manager_free(app->scene_manager);
    input_free(app->input);
    display_free(app->display);
    free(app);
}
