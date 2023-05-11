#include <stdlib.h>
#include <string.h>
#include "st7920.h"
#include "display.h"
#include <stm32f4xx_hal_def.h>
#include "assets_icons.h"

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
    display->color = DisplayDrawColorWhite;
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
    display_set_brightness(display, 240);
}

void display_set_color(Display* display, DisplayDrawColor color) {
    display->color = color;
}

void display_invert_color(Display* display) {
    display->color = !display->color;
}

void display_draw_pixel(Display* display, uint8_t x, uint8_t y) {
    uint8_t slash_x = (x / 8);
    uint8_t percent_x = (x % 8);
    if((display->color) == DisplayDrawColorWhite)
        display->frame_buffer[slash_x + (y * (SCREEN_WIDTH / 8))] |= (1 << percent_x);
    else if((display->color) == DisplayDrawColorBlack)
        display->frame_buffer[slash_x + (y * (SCREEN_WIDTH / 8))] &= ~(1 << percent_x);
}

void display_clear_pixel(Display* display, uint8_t x, uint8_t y) {
    uint8_t slash_x = (x / 8);
    uint8_t percent_x = (x % 8);
    if((display->color) == DisplayDrawColorBlack)
        display->frame_buffer[slash_x + (y * (SCREEN_WIDTH / 8))] |= (1 << percent_x);
    else if((display->color) == DisplayDrawColorWhite)
        display->frame_buffer[slash_x + (y * (SCREEN_WIDTH / 8))] &= ~(1 << percent_x);
}

void display_draw_line_straight(
    Display* display,
    uint8_t x_start,
    uint8_t y_start,
    uint8_t x_end,
    uint8_t y_end) {
    // just vertical or horizontal lines
    if((x_end != x_start) && (y_end != y_start)) return;
    while(x_end > x_start) {
        display_draw_pixel(display, x_start, y_start);
        x_start++;
    }
    while(y_end > y_start) {
        display_draw_pixel(display, x_start, y_start);
        y_start++;
    }
}

uint8_t
    display_draw_line_straight_animated(Display* display, Line line, uint16_t current_max_dots) {
    uint16_t printed_dots = 0;
    // just vertical or horizontal lines
    if((line.x_end != line.x_start) && (line.y_end != line.y_start)) return printed_dots;
    if(line.x_end > line.x_start) {
        while((line.x_end > line.x_start) && (printed_dots < current_max_dots)) {
            display_draw_pixel(display, line.x_start, line.y_start);
            printed_dots++;
            line.x_start++;
        }
    } else if(line.x_start > line.x_end) {
        while((line.x_start > line.x_end) && (printed_dots < current_max_dots)) {
            display_draw_pixel(display, line.x_start, line.y_start);
            printed_dots++;
            line.x_start--;
        }
    } else if(line.y_end > line.y_start) {
        while((line.y_end > line.y_start) && (printed_dots < current_max_dots)) {
            display_draw_pixel(display, line.x_start, line.y_start);
            printed_dots++;
            line.y_start++;
        }
    } else if(line.y_start > line.y_end) {
        while((line.y_start > line.y_end) && (printed_dots < current_max_dots)) {
            display_draw_pixel(display, line.x_start, line.y_start);
            printed_dots++;
            line.y_start--;
        }
    }
    return printed_dots;
}

void display_draw_rectangle(Display* display, uint8_t x, uint8_t y, uint8_t width, uint8_t height) {
    display_draw_line_straight(display, x, y, x + width, y);
    display_draw_line_straight(display, x, y + height, x + width, y + height);
    display_draw_line_straight(display, x, y, x, y + height);
    display_draw_line_straight(display, x + width, y, x + width, y + height);
}

void display_draw_filled_rectangle(
    Display* display,
    uint8_t x,
    uint8_t y,
    uint8_t width,
    uint8_t height) {
    while(height) {
        display_draw_line_straight(display, x, y, x + width, y);
        y++;
        height--;
    }
}

uint8_t display_get_string_width(Display* display, const char* str) {
    UNUSED(display);
    uint8_t string_width = 0;
    while(*str) {
        if(*str != '\n') string_width += icon_get_width(F_Haxrcorp_4089[*str - 32]) + 1;
        str++;
    }
    return string_width;
}

void display_draw_button_right(Display* display, const char* str, bool inverted) {
    const uint8_t button_height = 12;
    const uint8_t vertical_offset = 1;
    const uint8_t horizontal_offset = 3;
    const uint8_t string_width = display_get_string_width(display, str);
    const Icon* icon = &I_ButtonRight_4x7;
    const uint8_t icon_h_offset = 3;
    const uint8_t icon_width_with_offset = icon_get_width(icon) + icon_h_offset;
    const uint8_t icon_v_offset = icon_get_height(icon) + vertical_offset + 2;
    const uint8_t button_width = string_width + horizontal_offset * 2 + icon_width_with_offset;
    const uint8_t x = SCREEN_WIDTH - 1;
    const uint8_t y = SCREEN_HEIGHT - 1;

    if(inverted) display_invert_color(display);
    display_draw_filled_rectangle(
        display, x - button_width, y - button_height, button_width - 1, button_height - 1);
    display_invert_color(display);
    display_draw_rectangle(
        display, x - button_width, y - button_height - 1, button_width - 1, button_height);
    display_draw_string(
        display, str, x - button_width + horizontal_offset, y - vertical_offset - 10);
    display_draw_icon(
        display, icon, x - horizontal_offset - icon_get_width(icon), y - icon_v_offset);
    if(!inverted) display_invert_color(display);
}

void display_draw_button_left(Display* display, const char* str, bool inverted) {
    const uint8_t button_height = 12;
    const uint8_t vertical_offset = 1;
    const uint8_t horizontal_offset = 2;
    const uint8_t string_width = display_get_string_width(display, str);
    const Icon* icon = &I_ButtonLeft_4x7;
    const uint8_t icon_h_offset = 3;
    const uint8_t icon_width_with_offset = icon_get_width(icon) + icon_h_offset;
    const uint8_t icon_v_offset = icon_get_height(icon) + vertical_offset + 2;
    const uint8_t button_width = string_width + horizontal_offset * 2 + icon_width_with_offset;
    const uint8_t x = 1;
    const uint8_t y = SCREEN_HEIGHT - 1;

    if(inverted) display_invert_color(display);
    display_draw_filled_rectangle(display, x, y - button_height, button_width, button_height - 1);
    display_invert_color(display);
    display_draw_rectangle(display, x, y - button_height - 1, button_width, button_height);
    display_draw_string(
        display, str, x + horizontal_offset + icon_width_with_offset + 1, y - vertical_offset - 10);
    display_draw_icon(display, icon, x + horizontal_offset + 1, y - icon_v_offset);
    if(!inverted) display_invert_color(display);
}

void display_draw_xbm(
    Display* display,
    const uint8_t* data,
    uint8_t x,
    uint8_t y,
    uint8_t width,
    uint8_t height) {
    data++;
    if(width % 8 != 0) {
        width = ((width / 8) + 1) * 8;
    }
    for(int i = 0; i < width * height / 8; i++) {
        uint8_t byte_column = *(data + i);
        for(uint8_t bit = 0; bit < 8; bit++) {
            uint8_t target_x = (((8 * i) + bit) % width) + x;
            uint8_t target_y = ((8 * i) / width) + y;
            if((byte_column >> bit) & 0x01) display_draw_pixel(display, target_x, target_y);
        }
    }
}

void display_draw_icon(Display* display, const Icon* icon, uint8_t x, uint8_t y) {
    uint8_t icon_width = icon_get_width(icon);
    uint8_t icon_height = icon_get_height(icon);
    const uint8_t* const* icon_data = icon_get_data(icon);
    display_draw_xbm(display, icon_data[0], x, y, icon_width, icon_height);
}

void display_draw_icon_animation(
    Display* display,
    const Icon* icon,
    uint8_t x,
    uint8_t y,
    uint8_t current_frame) {
    uint8_t icon_width = icon_get_width(icon);
    uint8_t icon_height = icon_get_height(icon);
    const uint8_t* const* icon_data = icon_get_data(icon);
    display_draw_xbm(display, icon_data[current_frame], x, y, icon_width, icon_height);
}

// returns char width
uint8_t display_draw_char(Display* display, const char c, uint8_t x, uint8_t y) {
    const Icon* glyph = F_Haxrcorp_4089[c - 32];
    display_draw_icon(display, glyph, x, y);
    return icon_get_width(glyph);
}

void display_draw_string(Display* display, const char* str, uint8_t x, uint8_t y) {
    uint8_t new_x = x;
    while(*str) {
        if(*str == '\n') {
            new_x = x;
            y += 10;
            str++;
            continue;
        }
        new_x += display_draw_char(display, *str, new_x, y) + 1;
        str++;
    }
}

void display_draw_string_animation(
    Display* display,
    const char* str,
    uint8_t x,
    uint8_t y,
    uint8_t max_char) {
    const char* new_str_ptr = str;
    uint8_t new_x = x;
    while(*new_str_ptr) {
        if(*new_str_ptr == '\n') {
            new_x = x;
            y += 10;
            new_str_ptr++;
            continue;
        }
        if((max_char == 0) || ((new_str_ptr - str) > max_char - 1)) return;
        new_x += display_draw_char(display, *new_str_ptr, new_x, y) + 1;
        new_str_ptr++;
    }
}

uint8_t display_draw_line_animation(
    Display* display,
    const Line* const lines,
    uint16_t lines_count,
    uint16_t current_max_dot) {
    uint16_t printed_dots = 0;
    uint16_t printed_lines = 0;
    uint8_t step = 0;
    while((printed_dots < current_max_dot) && (printed_lines < lines_count)) {
        step = lines[printed_lines].step;
        printed_dots += display_draw_line_straight_animated(
            display, lines[printed_lines], current_max_dot - printed_dots);
        printed_lines++;
    }
    return step;
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

// TODO(drunkbatya): rewrite this fuckin' hardcoded bullshit
void display_set_brightness(Display* display, uint8_t bright) {
    if(bright < 155) bright = 155;
    if(bright > 254) bright = 254;
    pwm_set_params(display->pwm, 10000, 255 - bright);
}

uint8_t display_get_brightness(Display* display) {
    return (255 - pwm_get_duty(display->pwm));
}
