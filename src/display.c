#include <stdlib.h>
#include <string.h>
#include "st7920.h"
#include "display.h"

Display* display_alloc(SPI_TypeDef* SPI, GPIO_TypeDef* reset_pin_port, uint32_t reset_pin) {
    Display* display = malloc(sizeof(Display));
    display->SPI = SPI;
    display->reset_pin_port = reset_pin_port;
    display->reset_pin = reset_pin;
    display->frame_buffer = malloc(FRAME_BUFFER_SIZE);
    display->draw_callback = NULL;
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
}

void display_draw_frame(Display* display, const uint8_t* frame, uint16_t frame_size) {
    memcpy(display->frame_buffer, frame, frame_size);
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

void display_draw_pixel(Display* display, uint8_t x, uint8_t y) {
    uint8_t slash_x = (x / 8);
    uint8_t percent_x = (x % 8);
    display->frame_buffer[slash_x + (y * (SCREEN_WIDTH / 8))] |= (1 << percent_x);
}
