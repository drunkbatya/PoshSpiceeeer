#include <stdlib.h>
#include <string.h>
#include "st7920.h"
#include "display.h"

Display* display_alloc(
    SPI_TypeDef* SPI,
    GPIO_TypeDef* reset_pin_port,
    uint32_t reset_pin,
    TIM_TypeDef* timer,
    GPIO_TypeDef* timer_pin_port,
    uint32_t timer_pin) {
    Display* display = malloc(sizeof(Display));
    display->SPI = SPI;
    display->reset_pin_port = reset_pin_port;
    display->reset_pin = reset_pin;
    display->frame_buffer = malloc(FRAME_BUFFER_SIZE);
    display->draw_callback = NULL;
    display->pwm = pwm_init_alloc(timer, timer_pin_port, timer_pin);
    return display;
}

void display_clear_framebuffer(Display* display) {
    memset(display->frame_buffer, 0, FRAME_BUFFER_SIZE);
}

void display_init(Display* display) {
    LL_SPI_Enable(display->SPI);
    st7920_init();
    st7920_graphic_mode(true);
    display_clear_framebuffer(display);
    display_set_brightness(display, 255);
}

void display_draw_pixel(Display* display, uint8_t x, uint8_t y) {
    uint8_t slash_x = (x / 8);
    uint8_t percent_x = (x % 8);
    display->frame_buffer[slash_x + (y * (SCREEN_WIDTH / 8))] |= (1 << percent_x);
}

void display_draw_icon_animation_x(
    Display* display,
    const Icon* icon,
    uint8_t x,
    uint8_t y,
    uint8_t current_frame) {
    const uint8_t* data = icon_get_data(icon)[current_frame];
    for(uint8_t frame_y = 0; frame_y < icon_get_height(icon); frame_y++) {
        for(uint8_t frame_x = 0; frame_x < icon_get_width(icon); frame_x += 8) {
            uint8_t bit_data = data[(frame_y * icon_get_height(icon) + frame_x)];
            for(uint8_t bit = 0; bit < 8; bit++) {
                if((bit_data << bit) & 0x01) display_draw_pixel(display, x + frame_x + bit, y);
            }
        }
        y++;
    }
}

void display_draw_icon_animation(Display* display, const Icon* icon, uint8_t current_frame) {
    memcpy(
        display->frame_buffer,
        icon_get_data(icon)[current_frame],
        (icon_get_width(icon) * icon_get_height(icon) / 8));
}

void display_draw_icon(Display* display, const Icon* icon) {
    memcpy(
        display->frame_buffer, icon_get_data(icon), icon_get_width(icon) * icon_get_height(icon));
}

void displat_draw_icon(
    Display* display,
    const uint8_t* icon,
    uint8_t x,
    uint8_t y,
    uint8_t width,
    uint8_t height) {
}

void display_sync_framebuffer(Display* display) {
    st7920_draw_bitmap(display->frame_buffer);
}

void display_free(Display* display) {
    LL_SPI_Disable(display->SPI);
    pwm_free(display->pwm);
    free(display->frame_buffer);
    free(display);
}

void display_set_draw_callback(Display* display, DisplayDrawCallback draw_callback, void* context) {
    display->draw_callback = draw_callback;
    display->context = context;
}

void display_reset_draw_callback(Display* display) {
    display->draw_callback = NULL;
    display->context = NULL;
}

void display_execute_draw_callback(Display* display) {
    if(display->draw_callback) display->draw_callback(display->context);
}

void display_clear(Display* display) {
    display_clear_framebuffer(display);
    display_sync_framebuffer(display);
}

void display_set_brightness(Display* display, int16_t bright) {
    if(bright < 0) bright = 0;
    if(bright > 254) bright = 254;
    pwm_set_params(display->pwm, 10000, 255 - bright);
}

uint8_t display_get_brightness(Display* display) {
    return (255 - pwm_get_duty(display->pwm));
}
