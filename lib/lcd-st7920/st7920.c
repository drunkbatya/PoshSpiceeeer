/*
 * ST7920_SERIAL.c
 *
 *  Created on: 07-Jun-2019
 *      Author: poe
 *
 *	Edit By	:		Liyanboy74
 *	Date	:	21-July-2020
 */
#include <stm32f4xx_ll_gpio.h>
#include <stm32f4xx_ll_utils.h>
#include <string.h>
#include "st7920.h"

#define GLCD_SCK_Pin LL_GPIO_PIN_10
#define GLCD_SCK_GPIO_Port GPIOB

#define GLCD_SID_Pin LL_GPIO_PIN_15
#define GLCD_SID_GPIO_Port GPIOB

#define GLCD_RST_Pin LL_GPIO_PIN_13
#define GLCD_RST_GPIO_Port GPIOB

#define RST_PIN GLCD_RST_Pin
#define RST_PORT GLCD_RST_GPIO_Port
#define SID_PIN GLCD_SID_Pin
#define SID_PORT GLCD_SID_GPIO_Port
#define SCLK_PIN GLCD_SCK_Pin
#define SCLK_PORT GLCD_SCK_GPIO_Port

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define FRAME_BUFFER_SIZE ((SCREEN_WIDTH * SCREEN_HEIGHT) / 8)

uint8_t st7920_frame_buffer[FRAME_BUFFER_SIZE];

static void st7920_spi_transmit(uint8_t byte) {
    for(int i = 0; i < 8; i++) {
        if((byte << i) & 0x80) {
            LL_GPIO_SetOutputPin(SID_PORT, SID_PIN);
        } else
            LL_GPIO_ResetOutputPin(SID_PORT, SID_PIN);
        LL_GPIO_ResetOutputPin(SCLK_PORT, SCLK_PIN);
        LL_GPIO_SetOutputPin(SCLK_PORT, SCLK_PIN);
    }
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

static void st7920_draw_bitmap(const unsigned char* graphic) {
    uint8_t x, y;
    uint16_t Index = 0;
    uint8_t Temp, Db;
    for(y = 0; y < 64; y++) {
        for(x = 0; x < 8; x++) {
            if(y < 32) //Up
            {
                st7920_send_cmd(0x80 | y); //Y(0-31)
                st7920_send_cmd(0x80 | x); //X(0-8)
            } else {
                st7920_send_cmd(0x80 | y - 32); //Y(0-31)
                st7920_send_cmd(0x88 | x); //X(0-8)
            }

            Index = ((y / 8) * 128) + (x * 16);
            Db = y % 8;

            Temp = (((graphic[Index + 0] >> Db) & 0x01) << 7) |
                   (((graphic[Index + 1] >> Db) & 0x01) << 6) |
                   (((graphic[Index + 2] >> Db) & 0x01) << 5) |
                   (((graphic[Index + 3] >> Db) & 0x01) << 4) |
                   (((graphic[Index + 4] >> Db) & 0x01) << 3) |
                   (((graphic[Index + 5] >> Db) & 0x01) << 2) |
                   (((graphic[Index + 6] >> Db) & 0x01) << 1) |
                   (((graphic[Index + 7] >> Db) & 0x01) << 0);
            st7920_send_data(Temp);

            Temp = (((graphic[Index + 8] >> Db) & 0x01) << 7) |
                   (((graphic[Index + 9] >> Db) & 0x01) << 6) |
                   (((graphic[Index + 10] >> Db) & 0x01) << 5) |
                   (((graphic[Index + 11] >> Db) & 0x01) << 4) |
                   (((graphic[Index + 12] >> Db) & 0x01) << 3) |
                   (((graphic[Index + 13] >> Db) & 0x01) << 2) |
                   (((graphic[Index + 14] >> Db) & 0x01) << 1) |
                   (((graphic[Index + 15] >> Db) & 0x01) << 0);

            st7920_send_data(Temp);
        }
    }
}
// Update the display with the selected graphics
void st7920_update(void) {
    st7920_draw_bitmap(st7920_frame_buffer);
}

void st7920_clear(void) {
    uint8_t x, y;
    for(y = 0; y < 64; y++) {
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

void st7920_init(void) {
    // LL_SPI_Enable(SPI2);
    LL_GPIO_ResetOutputPin(RST_PORT, RST_PIN);
    LL_mDelay(1); // wait for 10ms
    LL_GPIO_SetOutputPin(RST_PORT, RST_PIN);

    LL_mDelay(1); //wait for >40 ms

    st7920_send_cmd(0x30); // 8bit mode
    LL_mDelay(1);

    st7920_send_cmd(0x30); // 8bit mode
    LL_mDelay(1);

    st7920_send_cmd(0x08); // D=0, C=0, B=0
    LL_mDelay(1);

    st7920_send_cmd(0x01); // clear screen
    LL_mDelay(1); // >10 ms delay

    st7920_send_cmd(0x06); // cursor increment right no shift
    LL_mDelay(1); // 1ms delay

    st7920_send_cmd(0x0C); // D=1, C=0, B=0
    LL_mDelay(1); // 1ms delay

    st7920_send_cmd(0x02); // return to home
    LL_mDelay(1); // 1ms delay
    memset(st7920_frame_buffer, 0x55, 200);
}
