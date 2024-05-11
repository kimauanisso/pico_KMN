#include "/home/leonardo/pico/pico_project/include/Servo/Servo.hpp"

void Servo(uint gpio_pin){
    gpio_set_function(gpio_pin, GPIO_FUNC_PWM);
    pwm_set_clkdiv(pwm_gpio_to_slice_num(gpio_pin), 125.0f); // Divisor de clock para gerar frequência de 50Hz
    pwm_set_wrap(pwm_gpio_to_slice_num(gpio_pin), 20000); // Definir o período do PWM para gerar 50Hz
    pwm_set_chan_level(pwm_gpio_to_slice_num(gpio_pin), pwm_gpio_to_channel(gpio_pin), 500); // 1ms pulse width (minimum position)
    pwm_set_enabled(pwm_gpio_to_slice_num(gpio_pin), true);
}

