#pragma once
#include <stdbool.h>
#include <stm32f4xx_ll_gpio.h>

typedef enum {
    INPUT_EVENT_NONE = 0x00,
    INPUT_EVENT_LEFT_PRESSED = 0x01,
    INPUT_EVENT_RIGHT_PRESSED = 0x02,
    INPUT_EVENT_CENTER_PRESSED = 0x04
} InputEvent;

typedef struct {
    GPIO_TypeDef* button_left_port;
    GPIO_TypeDef* button_right_port;
    GPIO_TypeDef* button_center_port;
    uint32_t button_left_pin;
    uint32_t button_right_pin;
    uint32_t button_center_pin;
    InputEvent event;
} Input;

Input* input_alloc(
    GPIO_TypeDef* button_left_port,
    GPIO_TypeDef* button_right_port,
    GPIO_TypeDef* button_center_port,
    uint32_t button_left_pin,
    uint32_t button_right_pin,
    uint32_t button_center_pin);
void input_free(Input* input);
bool input_recive_new_event(Input* input);
InputEvent input_get_event(Input* input);
