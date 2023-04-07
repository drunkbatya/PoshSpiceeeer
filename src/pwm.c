#include <stdlib.h>
#include <stm32f4xx_ll_tim.h>
#include <stm32f4xx_ll_bus.h>
#include "pwm.h"

static void pwm_update(Pwm* pwm) {
    uint32_t freq_div = 64000000LU / pwm->freq;
    uint32_t prescaler = freq_div / 0x10000LU;
    uint32_t period = freq_div / (prescaler + 1);
    uint32_t compare = period * pwm->duty / 100;
    LL_TIM_SetPrescaler(pwm->timer, prescaler);
    LL_TIM_SetAutoReload(pwm->timer, period - 1);
    LL_TIM_OC_SetCompareCH1(pwm->timer, compare);
}

void pwm_set_params(Pwm* pwm, uint32_t freq, uint8_t duty) {
    pwm->freq = freq;
    pwm->duty = duty;
    pwm_update(pwm);
}

static void pwm_init(Pwm* pwm) {
    LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = pwm->timer_pin;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    GPIO_InitStruct.Alternate = LL_GPIO_AF_1;
    LL_GPIO_Init(pwm->timer_pin_port, &GPIO_InitStruct);
    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_TIM1);
    LL_TIM_SetCounterMode(pwm->timer, LL_TIM_COUNTERMODE_UP);
    LL_TIM_SetRepetitionCounter(pwm->timer, 0);
    LL_TIM_SetClockDivision(pwm->timer, LL_TIM_CLOCKDIVISION_DIV1);
    LL_TIM_SetClockSource(pwm->timer, LL_TIM_CLOCKSOURCE_INTERNAL);
    LL_TIM_EnableARRPreload(pwm->timer);
    LL_TIM_OC_EnablePreload(pwm->timer, LL_TIM_CHANNEL_CH1);
    LL_TIM_OC_SetMode(pwm->timer, LL_TIM_CHANNEL_CH1, LL_TIM_OCMODE_PWM1);
    LL_TIM_OC_SetPolarity(pwm->timer, LL_TIM_CHANNEL_CH1N, LL_TIM_OCPOLARITY_HIGH);
    LL_TIM_OC_DisableFast(pwm->timer, LL_TIM_CHANNEL_CH1);
    LL_TIM_CC_EnableChannel(pwm->timer, LL_TIM_CHANNEL_CH1N);
    LL_TIM_EnableAllOutputs(pwm->timer);
    pwm_update(pwm);
    LL_TIM_EnableCounter(pwm->timer);
}

Pwm* pwm_init_alloc(TIM_TypeDef* timer, GPIO_TypeDef* timer_pin_port, uint32_t timer_pin) {
    Pwm* pwm = malloc(sizeof(Pwm));
    pwm->timer = timer;
    pwm->timer_pin_port = timer_pin_port;
    pwm->timer_pin = timer_pin;
    pwm->freq = 1000;
    pwm->duty = 50;
    pwm_init(pwm);
    return pwm;
}

static void pwm_deinit(Pwm* pwm) {
    LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = pwm->timer_pin;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_ANALOG;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    LL_GPIO_Init(pwm->timer_pin_port, &GPIO_InitStruct);
    LL_TIM_DeInit(pwm->timer);
}

void pwm_free(Pwm* pwm) {
    pwm_deinit(pwm);
    free(pwm);
}
