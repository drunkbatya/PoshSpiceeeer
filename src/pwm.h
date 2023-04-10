#pragma once
#include "stm32f4xx_ll_tim.h"
#include "stm32f4xx_ll_gpio.h"

typedef struct {
    TIM_TypeDef* timer;
    GPIO_TypeDef* timer_pin_port;
    uint32_t timer_pin;
    uint32_t freq;
    uint8_t duty;
} Pwm;

Pwm* pwm_init_alloc(TIM_TypeDef* timer, GPIO_TypeDef* timer_pin_port, uint32_t timer_pin);
void pwm_free(Pwm* pwm);
void pwm_set_params(Pwm* pwm, uint32_t freq, uint8_t duty);
uint8_t pwm_get_duty(Pwm* pwm);
