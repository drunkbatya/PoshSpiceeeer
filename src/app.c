#include <stdlib.h>
#include <stdbool.h>
#include <stm32f4xx_ll_gpio.h>
#include <stm32f4xx_ll_spi.h>
#include <stm32f4xx_ll_utils.h>
#include "app.h"
#include "scenes/app_scene.h"

App* app_alloc(void) {
    App* app = malloc(sizeof(App));
    app->display = display_alloc(SPI2, GPIOB, LL_GPIO_PIN_13);
    app->scene_manager = scene_manager_alloc(&scene_handlers, app);
    app->animation_manager = animation_manager_alloc();
    display_init(app->display);
    return app;
}

void app_run(App* app) {
    display_clear_framebuffer(app->display);
    scene_manager_next_scene(app->scene_manager, SceneStart);
    while(true) {
        display_execute_draw_callback(app->display);
        // for(uint8_t cur = 0; cur < 36; cur++) {
        //    display_draw_image(app->display, frames[cur], 1024);
        display_sync_framebuffer(app->display);
        //    LL_mDelay(300);
        //}
    }
}

void app_free(App* app) {
    animation_manager_free(app->animation_manager);
    scene_manager_stop(app->scene_manager);
    scene_manager_free(app->scene_manager);
    display_free(app->display);
    free(app);
}
