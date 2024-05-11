#ifndef ENCODER_H
#define ENCODER_H

#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "quadrature.pio.h"

void Encoder(PIO pio, uint sm, uint a_pin, uint b_pin);

#endif
