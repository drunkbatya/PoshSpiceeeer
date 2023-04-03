#include <stdlib.h>
#include <stdbool.h>
#include <stm32f4xx_ll_gpio.h>
#include <stm32f4xx_ll_spi.h>
#include <stm32f4xx_ll_utils.h>
#include "app.h"
#include "assets_icons.h"

const uint8_t* const frames[] = {
    _I_frame_0_0,  _I_frame_1_0,  _I_frame_2_0,  _I_frame_3_0,  _I_frame_4_0,  _I_frame_5_0,
    _I_frame_6_0,  _I_frame_7_0,  _I_frame_8_0,  _I_frame_9_0,  _I_frame_10_0, _I_frame_11_0,
    _I_frame_12_0, _I_frame_13_0, _I_frame_14_0, _I_frame_15_0, _I_frame_16_0, _I_frame_17_0,
    _I_frame_18_0, _I_frame_19_0, _I_frame_20_0, _I_frame_21_0, _I_frame_22_0, _I_frame_23_0,
    _I_frame_24_0, _I_frame_25_0, _I_frame_26_0, _I_frame_27_0, _I_frame_28_0, _I_frame_29_0,
    _I_frame_30_0, _I_frame_31_0, _I_frame_32_0, _I_frame_33_0, _I_frame_34_0, _I_frame_35_0};

App* app_alloc(void) {
    App* app = malloc(sizeof(App));
    app->display = display_alloc(SPI2, GPIOB, LL_GPIO_PIN_13);
    display_init(app->display);
    return app;
}

void app_run(App* app) {
    while(true) {
        for(uint8_t cur = 0; cur < 36; cur++) {
            display_draw_image(app->display, frames[cur], 1024);
            display_sync_framebuffer(app->display);
            LL_mDelay(300);
        }
    }
}

void app_free(App* app) {
    display_free(app->display);
    free(app);
}
