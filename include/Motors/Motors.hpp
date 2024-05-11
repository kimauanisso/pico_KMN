#ifndef MOTORS_H
#define MOTORS_H

#include "pico/stdlib.h"
#include "hardware/pwm.h"

void Motors(uint pwmE1, uint pwmE2, uint pwmD1, uint pwmD2, float frequency);

#endif
