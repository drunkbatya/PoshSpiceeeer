#pragma once
#include <stm32f4xx_ll_gpio.h>
#include <stm32f4xx_ll_spi.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define FRAME_BUFFER_SIZE ((SCREEN_WIDTH * SCREEN_HEIGHT) / 8)

typedef void (*DisplayDrawCallback)(void* context);

typedef struct {
    SPI_TypeDef* SPI;
    GPIO_TypeDef* reset_pin_port;
    uint32_t reset_pin;
    uint8_t* frame_buffer;
    DisplayDrawCallback draw_callback;
    void* context;
} Display;

Display* display_alloc(SPI_TypeDef* SPI, GPIO_TypeDef* reset_pin_port, uint32_t reset_pin);
void display_init(Display* display);
void display_draw_frame(Display* display, const uint8_t* frame, uint16_t frame_size);
void display_sync_framebuffer(Display* display);
void display_free(Display* display);
void display_set_draw_callback(Display* display, DisplayDrawCallback draw_callback, void* context);
void display_reset_draw_callback(Display* display);
void display_execute_draw_callback(Display* display);
void display_clear_framebuffer(Display* display);
void display_clear(Display* display);
void display_draw_pixel(Display* display, uint8_t x, uint8_t y);
