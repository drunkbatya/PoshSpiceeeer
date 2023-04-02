// Originally written by poe and Liyanboy74

#include <stm32f4xx_ll_gpio.h>
#include <stm32f4xx_ll_spi.h>
#include <stm32f4xx_ll_utils.h>
#include <string.h>
#include "st7920.h"

#define RST_PIN GLCD_RST_Pin
#define RST_PORT GLCD_RST_GPIO_Port

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define FRAME_BUFFER_SIZE ((SCREEN_WIDTH * SCREEN_HEIGHT) / 8)

uint8_t st7920_frame_buffer[FRAME_BUFFER_SIZE];

static void st7920_spi_transmit(uint8_t byte) {
    while(!(SPI2->SR & SPI_SR_TXE))
        ;
    LL_SPI_TransmitData8(SPI2, byte);
    while(SPI2->SR & SPI_SR_BSY)
        ;
}

static void st7920_send_cmd(uint8_t cmd) {
    st7920_spi_transmit((0xF8 + (0 << 1)));
    st7920_spi_transmit((cmd & 0xF0));
    st7920_spi_transmit(((cmd << 4) & 0xF0));
}

static void st7920_send_data(uint8_t data) {
    st7920_spi_transmit((0xF8 + (1 << 1)));
    st7920_spi_transmit((data & 0xF0));
    st7920_spi_transmit(((data << 4) & 0xF0));
}

void st7920_graphic_mode(bool enable) {
    if(enable) {
        st7920_send_cmd(0x30); // 8 bit mode
        LL_mDelay(1);
        st7920_send_cmd(0x34); // switch to Extended instructions
        LL_mDelay(1);
        st7920_send_cmd(0x36); // enable graphics
        LL_mDelay(1);
    } else {
        st7920_send_cmd(0x30); // 8 bit mode
        LL_mDelay(1);
    }
}

void st7920_clear_buffer(void) {
    memset(st7920_frame_buffer, 0, FRAME_BUFFER_SIZE);
}

static uint8_t swap_bits(uint8_t num) {
    num = (num & 0xF0) >> 4 | (num & 0x0F) << 4;
    num = (num & 0xCC) >> 2 | (num & 0x33) << 2;
    num = (num & 0xAA) >> 1 | (num & 0x55) << 1;
    return num;
}

static void st7920_draw_bitmap(const unsigned char* graphic) {
    uint8_t x, y;
    for(y = 0; y < SCREEN_HEIGHT; y++) {
        if(y < 32) {
            for(x = 0; x < 8; x++) {
                st7920_send_cmd(0x80 | y);
                st7920_send_cmd(0x80 | x);
                st7920_send_data(swap_bits(graphic[2 * x + 16 * y]));
                st7920_send_data(swap_bits(graphic[2 * x + 1 + 16 * y]));
            }
        } else {
            for(x = 0; x < 8; x++) {
                st7920_send_cmd(0x80 | (y - 32));
                st7920_send_cmd(0x88 | x);
                st7920_send_data(swap_bits(graphic[2 * x + 16 * y]));
                st7920_send_data(swap_bits(graphic[2 * x + 1 + 16 * y]));
            }
        }
    }
}

void st7920_update(void) {
    st7920_draw_bitmap(st7920_frame_buffer);
}

void st7920_clear(void) {
    uint8_t x, y;
    for(y = 0; y < SCREEN_HEIGHT; y++) {
        if(y < 32) {
            st7920_send_cmd(0x80 | y);
            st7920_send_cmd(0x80);
        } else {
            st7920_send_cmd(0x80 | (y - 32));
            st7920_send_cmd(0x88);
        }
        for(x = 0; x < 8; x++) {
            st7920_send_data(0);
            st7920_send_data(0);
        }
    }
    st7920_clear_buffer();
}

void st7920_draw_image(const uint8_t* image, uint16_t image_size) {
    memcpy(st7920_frame_buffer, image, image_size);
}

void st7920_init(void) {
    LL_SPI_Enable(SPI2);
    LL_GPIO_ResetOutputPin(RST_PORT, RST_PIN);
    LL_mDelay(1);
    LL_GPIO_SetOutputPin(RST_PORT, RST_PIN);

    LL_mDelay(1);

    st7920_send_cmd(0x30); // 8bit mode
    LL_mDelay(1);

    st7920_send_cmd(0x30); // 8bit mode
    LL_mDelay(1);

    st7920_send_cmd(0x08); // D=0, C=0, B=0
    LL_mDelay(1);

    st7920_send_cmd(0x01); // clear screen
    LL_mDelay(1);

    st7920_send_cmd(0x06); // cursor increment right no shift
    LL_mDelay(1);

    st7920_send_cmd(0x0C); // D=1, C=0, B=0
    LL_mDelay(1);

    st7920_send_cmd(0x02); // return to home
    LL_mDelay(1);
    st7920_clear_buffer();
}
