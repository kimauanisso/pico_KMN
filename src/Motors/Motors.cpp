#include "/home/leonardo/pico/pico_project/include/Motors/Motors.hpp"

void Motors(uint pwmE1, uint pwmE2, uint pwmD1, uint pwmD2, float frequency){
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