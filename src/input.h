#pragma once
#include <stdbool.h>
#include <stm32f4xx_ll_gpio.h>
#include "scene_manager.h"

typedef struct {
    GPIO_TypeDef* button_left_port;
    GPIO_TypeDef* button_right_port;
    GPIO_TypeDef* button_center_port;
    uint32_t button_left_pin;
    uint32_t button_right_pin;
    uint32_t button_center_pin;
    SceneEvent event;
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
SceneEvent input_get_event(Input* input);
