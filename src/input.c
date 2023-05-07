#include "input.h"
#include <stdbool.h>
#include <stdlib.h>

static void input_pin_init(GPIO_TypeDef* port, uint32_t pin) {
    LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = pin;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
    LL_GPIO_Init(port, &GPIO_InitStruct);
}

static void input_pin_deinit(GPIO_TypeDef* port, uint32_t pin) {
    LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = pin;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    LL_GPIO_Init(port, &GPIO_InitStruct);
}

Input* input_alloc(
    GPIO_TypeDef* button_left_port,
    GPIO_TypeDef* button_right_port,
    GPIO_TypeDef* button_center_port,
    uint32_t button_left_pin,
    uint32_t button_right_pin,
    uint32_t button_center_pin) {
    Input* input = malloc(sizeof(Input));
    input->button_left_port = button_left_port;
    input->button_right_port = button_right_port;
    input->button_center_port = button_center_port;
    input->button_left_pin = button_left_pin;
    input->button_right_pin = button_right_pin;
    input->button_center_pin = button_center_pin;
    input->event = SCENE_EVENT_NONE;
    input_pin_init(input->button_left_port, input->button_left_pin);
    input_pin_init(input->button_right_port, input->button_right_pin);
    input_pin_init(input->button_center_port, input->button_center_pin);
    return input;
}

static SceneEvent input_get_new_event(Input* input) {
    if(!LL_GPIO_IsInputPinSet(input->button_right_port, input->button_right_pin))
        return SCENE_EVENT_RIGHT_PRESSED;
    if(!LL_GPIO_IsInputPinSet(input->button_left_port, input->button_left_pin))
        return SCENE_EVENT_LEFT_PRESSED;
    if(!LL_GPIO_IsInputPinSet(input->button_center_port, input->button_center_pin))
        return SCENE_EVENT_CENTER_PRESSED;
    return SCENE_EVENT_NONE;
}

bool input_recive_new_event(Input* input) {
    SceneEvent new_event = input_get_new_event(input);
    if(new_event == input->event) return false;
    input->event = new_event;
    return true;
}

SceneEvent input_get_event(Input* input) {
    return input->event;
}

void input_free(Input* input) {
    input_pin_deinit(input->button_left_port, input->button_left_pin);
    input_pin_deinit(input->button_right_port, input->button_right_pin);
    input_pin_deinit(input->button_center_port, input->button_center_pin);
    free(input);
}
