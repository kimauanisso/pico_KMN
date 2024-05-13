#include "Encoder/Encoder.hpp"
#include "hardware/gpio.h"

void Encoder(PIO pio, uint sm, uint a_pin, uint b_pin){
    uint offset = pio_add_program(pio, &quadratureA_program);
    quadratureA_program_init(pio, sm, offset, a_pin, b_pin);
}