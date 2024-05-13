#ifndef MOTORS_H
#define MOTORS_H

#include "pico/stdlib.h"
#include "hardware/pwm.h"

void MotorsDRV(uint pwmE1, uint pwmE2, uint pwmD1, uint pwmD2, float frequency);
void MotorsIFX(uint digitalE, uint pwmE, uint digitalD, uint pwmS, float frequency);

#endif
