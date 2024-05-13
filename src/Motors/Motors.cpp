#include "Motors/Motors.hpp"

void MotorsDRV(uint pwmE1, uint pwmE2, uint pwmD1, uint pwmD2, float frequency){
    pwm_config config_ = pwm_get_default_config();

    pwm_config_set_clkdiv(&config_, 1.0f);
    pwm_init(pwm_gpio_to_slice_num(pwmE1), &config_, true);
    gpio_set_function(pwmE1, GPIO_FUNC_PWM);
    pwm_set_wrap(pwm_gpio_to_slice_num(pwmE1), 125e6/frequency);

    pwm_config_set_clkdiv(&config_, 1.0f);
    pwm_init(pwm_gpio_to_slice_num(pwmE2), &config_, true);
    gpio_set_function(pwmE2, GPIO_FUNC_PWM);
    pwm_set_wrap(pwm_gpio_to_slice_num(pwmE2), 125e6/frequency);

    pwm_config_set_clkdiv(&config_, 1.0f);
    pwm_init(pwm_gpio_to_slice_num(pwmD1), &config_, true);
    gpio_set_function(pwmD1, GPIO_FUNC_PWM);
    pwm_set_wrap(pwm_gpio_to_slice_num(pwmD1), 125e6/frequency);

    pwm_config_set_clkdiv(&config_, 1.0f);
    pwm_init(pwm_gpio_to_slice_num(pwmD2), &config_, true);
    gpio_set_function(pwmD2, GPIO_FUNC_PWM);
    pwm_set_wrap(pwm_gpio_to_slice_num(pwmD2), 125e6/frequency);
}

void MotorsIFX(uint digitalE, uint pwmE, uint digitalD, uint pwmD, float frequency){
    pwm_config config_ = pwm_get_default_config();

    pwm_config_set_clkdiv(&config_, 1.0f);
    pwm_init(pwm_gpio_to_slice_num(pwmE), &config_, true);
    gpio_set_function(pwmE, GPIO_FUNC_PWM);
    pwm_set_wrap(pwm_gpio_to_slice_num(pwmE), 125e6/frequency);

    pwm_config_set_clkdiv(&config_, 1.0f);
    pwm_init(pwm_gpio_to_slice_num(pwmD), &config_, true);
    gpio_set_function(pwmD, GPIO_FUNC_PWM);
    pwm_set_wrap(pwm_gpio_to_slice_num(pwmD), 125e6/frequency);

    gpio_init(digitalE);
    gpio_set_dir(digitalE, GPIO_OUT);

    gpio_init(digitalD);
    gpio_set_dir(digitalD, GPIO_OUT);
}