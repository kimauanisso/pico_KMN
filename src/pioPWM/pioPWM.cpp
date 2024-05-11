#include "/home/leonardo/pico/pico_project/include/pioPWM/pioPWM.hpp"
#include "hardware/gpio.h"

void PIOpwm(PIO pio, int sm, uint gpio_pin, int frequency){
    uint offset = pio_add_program(pio, &pwm_program);
    pwm_program_init(pio, sm, offset, gpio_pin);
    pio_pwm_set_period(pio, sm, (1u << frequency) - 1);
}


void pio_pwm_set_period(PIO pio, int sm, uint32_t period) {
    pio_sm_set_enabled(pio, sm, false);
    pio_sm_put_blocking(pio, sm, period);
    pio_sm_exec(pio, sm, pio_encode_pull(false, false));
    pio_sm_exec(pio, sm, pio_encode_out(pio_isr, 32));
    pio_sm_set_enabled(pio, sm, true);
}