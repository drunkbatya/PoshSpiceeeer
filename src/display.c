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

void display_draw_image(Display* display, const uint8_t* image, uint16_t image_size) {
    memcpy(display->frame_buffer, image, image_size);
}

void display_sync_framebuffer(Display* display) {
    st7920_draw_bitmap(display->frame_buffer);
}

void display_free(Display* display) {
    LL_SPI_Disable(display->SPI);
    free(display->frame_buffer);
    free(display);
}
