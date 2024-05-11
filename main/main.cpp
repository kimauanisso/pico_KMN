#include <stdio.h>
#include "pico/stdlib.h"
#include "Motors/Motors.hpp"
#include "pioPWM/pioPWM.hpp"
#include "Servo/Servo.hpp"
#include "ssd1306/SSD1306_OLED.hpp"
#include "Encoder/Encoder.hpp"
#include "hardware/timer.h"
#include "hardware/gpio.h"


const uint PWME_PIN_1 = 0;       // pino motor esquerdo - pino 1
const uint PWME_PIN_2 = 1;       // pino motor esquerdo - pino 2
const uint PWMD_PIN_1 = 2;       // pino motor direito - pino 1
const uint PWMD_PIN_2 = 3;       // pino motor direito - pino 2
const float frequency = 30000.0; // frequencia do motor
const int invertE = 0;           // caso o motor esquerdo esteja invertido, mude para 1
const int invertD = 0;           // caso o motor direito esteja invertido, mude para 1

const uint SERVO = 6; // pino servo

const uint TURBINA = 7;    // pino turbina
PIO pioT = pio0;           // uso do pio0 para geraçaõ de um sinal PWM, 0 ou 1
int smT = 0;               // maquina de estado 0 a 3
int frequencyTurbina = 10; // frequencia 10->40.5khz

const uint CHE_A = 2;
const uint CHE_B = 3;
const uint CHD_A = 7;
const uint CHD_B = 6;
PIO pioE = pio0;           // uso do pio0 para geraçaõ de um sinal PWM, 0 ou 1
int smE1 = 1;               // maquina de estado 0 a 3
int smE2 = 2;  
uint ENCE = 0;
uint ENCD = 0;


const uint CH1_PIN = 12;
const uint CH2_PIN = 13;
const uint CH3_PIN = 14;
const uint CH4_PIN = 15;

volatile uint32_t tZero_CH1;
volatile uint32_t tZero_CH2;
volatile uint32_t tZero_CH3;
volatile uint32_t tZero_CH4;

volatile uint32_t deltaT_CH1 = 1500;
volatile uint32_t deltaT_CH2 = 1500;
volatile uint32_t deltaT_CH3 = 1500;
volatile uint32_t deltaT_CH4 = 1500;

volatile uint32_t *tZero = NULL;
volatile uint32_t *deltaT = NULL;

#define myOLEDwidth  128
#define myOLEDheight 32
#define myScreenSize (myOLEDwidth * (myOLEDheight/8)) // eg 1024 bytes = 128 * 64/8
uint8_t screenBuffer[myScreenSize]; // Define a buffer to cover whole screen  128 * 64/8

// I2C settings
const uint16_t I2C_Speed = 100;
const uint8_t I2C_Address = 0x3C;

// instantiate  an OLED object
SSD1306 myOLED(myOLEDwidth ,myOLEDheight);

// const uint LED_PIN = 25; // LED BUILT IN

#define TERMS 100000
uint64_t start_time, end_time;

// =============== Function prototype ================
void moveMotor(int, int);
void setServoPosition(int);
void setTurbinaSpeed(int);
void handle_pulse_change(uint, uint32_t);
void init_radio_channels(void);
void OLED(void);
void Test(void);
void EndTest(void);
uint getEncoderValue(PIO, int);

// ======================= Main ===================

void moveMotor(int speedE, int speedD){
    float duty_cycleE = (float)speedE / 100.0f * 125e6 / frequency;
    float duty_cycleD = (float)speedD / 100.0f * 125e6 / frequency;
    if (speedE > 0)
    {
        pwm_set_gpio_level(PWME_PIN_1, !invertE * duty_cycleE);
        pwm_set_gpio_level(PWME_PIN_2, invertE * duty_cycleE);
    }
    else if (speedE < 0)
    {
        pwm_set_gpio_level(PWME_PIN_1, !invertE * duty_cycleE);
        pwm_set_gpio_level(PWME_PIN_2, invertE * duty_cycleE);
    }
    else
    {
        pwm_set_gpio_level(PWME_PIN_1, 1.0f);
        pwm_set_gpio_level(PWME_PIN_2, 1.0f);
    }

    if (speedD > 0)
    {
        pwm_set_gpio_level(PWMD_PIN_1, !invertD * duty_cycleD);
        pwm_set_gpio_level(PWMD_PIN_2, invertD * duty_cycleD);
    }
    else if (speedD < 0)
    {
        pwm_set_gpio_level(PWMD_PIN_1, !invertD * duty_cycleD);
        pwm_set_gpio_level(PWMD_PIN_2, invertD * duty_cycleD);
    }
    else
    {
        pwm_set_gpio_level(PWMD_PIN_1, 1.0f);
        pwm_set_gpio_level(PWMD_PIN_2, 1.0f);
    }
}

void setServoPosition(int pos)
{
    pwm_set_chan_level(pwm_gpio_to_slice_num(SERVO), pwm_gpio_to_channel(SERVO), pos);
}

void setTurbinaSpeed(int speed)
{
    float duty_cycle = (float)speed / 100.0f;
    pio_sm_put_blocking(pioT, smT, ((1u << frequencyTurbina) - 1) * duty_cycle);
}

void handle_pulse_change(uint gpio, uint32_t events)
{
    switch (gpio)
    {
    case CH1_PIN:
        tZero = &tZero_CH1;
        deltaT = &deltaT_CH1;
        break;
    case CH2_PIN:
        tZero = &tZero_CH2;
        deltaT = &deltaT_CH2;
        break;
    case CH3_PIN:
        tZero = &tZero_CH3;
        deltaT = &deltaT_CH3;
        break;
    case CH4_PIN:
        tZero = &tZero_CH4;
        deltaT = &deltaT_CH4;
        break;
    default:
        return;
    }

    *tZero = (events & GPIO_IRQ_EDGE_RISE) ? time_us_32() : *tZero;
    *deltaT = (events & GPIO_IRQ_EDGE_FALL) ? time_us_32() - *tZero : *deltaT;
}

void init_radio_channels()
{
    gpio_set_irq_enabled_with_callback(CH1_PIN, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &handle_pulse_change);
    gpio_set_irq_enabled_with_callback(CH2_PIN, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &handle_pulse_change);
    gpio_set_irq_enabled_with_callback(CH3_PIN, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &handle_pulse_change);
    gpio_set_irq_enabled_with_callback(CH4_PIN, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &handle_pulse_change);
}

void OLED(uint gpio_pin_SDA, uint gpio_pin_SCL){
    myOLED.OLEDbegin(I2C_Address,i2c0,  I2C_Speed, gpio_pin_SDA, gpio_pin_SCL);
    myOLED.OLEDSetBufferPtr(myOLEDwidth, myOLEDheight, screenBuffer, sizeof(screenBuffer)/sizeof(uint8_t));
    // myOLED.OLEDFillScreen(0xF0, 0);
}

void EndOLED(){
	myOLED.OLEDPowerDown(); // Switch off display
	myOLED.OLEDdeI2CInit(); 
}

void Test() {
	myOLED.OLEDclearBuffer(); 
	myOLED.setFont(pFontDefault);
	myOLED.setCursor(0,0);
	myOLED.print("Modo: 0");
	myOLED.OLEDupdate();  
	busy_wait_ms(2000);
}

uint getEncoderValue(PIO pio, int sm){
    pio_sm_exec_wait_blocking(pio, sm, pio_encode_in(pio_x, 32));
    return pio_sm_get_blocking(pio, sm);
}

int main()
{
    // Inicialize o stdio
    stdio_init_all();
    // Motors(PWME_PIN_1, PWME_PIN_2, PWMD_PIN_1, PWMD_PIN_2, frequency);
    // Servo(SERVO);
    // PIOpwm(pioT, smT, TURBINA, frequencyTurbina);
    // init_radio_channels();
    // OLED(4, 5);
    Encoder(pioE, smE1, CHE_A, CHE_B);
    Encoder(pioE, smE2, CHD_A, CHD_B);

    // setTurbinaSpeed(50);
    // setServoPosition(2000);
    // moveMotor(50,50);

    while (1)
    {
        tight_loop_contents();

        start_time = time_us_64();
        for(int i = 0; i<TERMS; i++){
            ENCE = getEncoderValue(pioE, smE1);
            ENCD = getEncoderValue(pioE, smE2);
        }
        end_time = time_us_64();

        printf("%llu\n\n",end_time - start_time);
        printf("%ld\t%ld\n", getEncoderValue(pioE, smE1), getEncoderValue(pioE, smE2));
    }

    return 0;
}
