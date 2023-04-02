#include "my_main.h"
#include <stm32f4xx_ll_utils.h>
#include "st7920.h"
#include "assets_icons.h"

const uint8_t* const frames[] = {
    _I_frame_0_0,  _I_frame_1_0,  _I_frame_2_0,  _I_frame_3_0,  _I_frame_4_0,  _I_frame_5_0,
    _I_frame_6_0,  _I_frame_7_0,  _I_frame_8_0,  _I_frame_9_0,  _I_frame_10_0, _I_frame_11_0,
    _I_frame_12_0, _I_frame_13_0, _I_frame_14_0, _I_frame_15_0, _I_frame_16_0, _I_frame_17_0,
    _I_frame_18_0, _I_frame_19_0, _I_frame_20_0, _I_frame_21_0, _I_frame_22_0, _I_frame_23_0,
    _I_frame_24_0, _I_frame_25_0, _I_frame_26_0, _I_frame_27_0, _I_frame_28_0, _I_frame_29_0,
    _I_frame_30_0, _I_frame_31_0, _I_frame_32_0, _I_frame_33_0, _I_frame_34_0, _I_frame_35_0};

void my_main(void) {
    st7920_init();
    st7920_graphic_mode(true);
    while(true) {
        for(uint8_t cur = 0; cur < 36; cur++) {
            st7920_draw_image(frames[cur], 1024);
            st7920_update();
            LL_mDelay(300);
        }
    }
}
