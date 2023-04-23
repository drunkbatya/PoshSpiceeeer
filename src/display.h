#pragma once
#include <stm32f4xx_ll_gpio.h>
#include <stm32f4xx_ll_spi.h>
#include <stdbool.h>
#include "icon.h"
#include "pwm.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define FRAME_BUFFER_SIZE ((SCREEN_WIDTH * SCREEN_HEIGHT) / 8)

typedef void (*DisplayDrawCallback)(void* context);

typedef enum {
    DisplayDrawColorWhite = 0x00,
    DisplayDrawColorBlack = 0x01,
} DisplayDrawColor;

typedef struct {
    SPI_TypeDef* SPI;
    GPIO_TypeDef* reset_pin_port;
    uint32_t reset_pin;
    uint8_t* frame_buffer;
    DisplayDrawCallback draw_callback;
    void* context;
    Pwm* pwm;
    DisplayDrawColor color;
} Display;

Display* display_alloc(
    SPI_TypeDef* SPI,
    GPIO_TypeDef* reset_pin_port,
    uint32_t reset_pin,
    TIM_TypeDef* timer,
    GPIO_TypeDef* timer_pin_port,
    uint32_t timer_pin);
void display_init(Display* display);
void display_sync_framebuffer(Display* display);
void display_free(Display* display);
void display_set_draw_callback(Display* display, DisplayDrawCallback draw_callback, void* context);
void display_reset_draw_callback(Display* display);
void display_execute_draw_callback(Display* display);
void display_clear_framebuffer(Display* display);
void display_clear(Display* display);
void display_draw_pixel(Display* display, uint8_t x, uint8_t y);
void display_set_brightness(Display* display, uint8_t bright);
uint8_t display_get_brightness(Display* display);
void display_draw_icon_animation(
    Display* display,
    const Icon* icon,
    uint8_t x,
    uint8_t y,
    uint8_t current_frame);
void display_draw_icon(Display* display, const Icon* icon, uint8_t x, uint8_t y);
// returns char width
uint8_t display_draw_char(Display* display, const char c, uint8_t x, uint8_t y);
void display_draw_string(Display* display, const char* str, uint8_t x, uint8_t y);
void display_draw_string_animation(
    Display* display,
    const char* str,
    uint8_t x,
    uint8_t y,
    uint8_t max_char);
void display_set_color(Display* display, DisplayDrawColor color);
void display_invert_color(Display* display);
void display_draw_button_right(Display* display, const char* str, bool inverted);
void display_draw_button_left(Display* display, const char* str, bool inverted);
