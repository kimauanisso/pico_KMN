#ifndef PIO_PWM_H
#define PIO_PWM_H

#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "/home/leonardo/pico/pico_project/build/pwm.pio.h"

void PIOpwm(PIO pio, int sm, uint gpio_pin, int frequency);
void pio_pwm_set_period(PIO pio, int sm, uint32_t period);


#endif