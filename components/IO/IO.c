/**
* @file IO.c
* @brief 
*
* @author Christopher Sommerauer
*
* @date 2023-01-07
*/

#include "IO.h"
#include "driver/gpio.h"
#include "driver/ledc.h"

static uint8_t GPIOAvailable[] = {0, 1, 2, 3, 4, 5, 12, 13, 14, 15, 16, 17, 18, 19, 21, 22, 23, 25, 26, 27, 32, 33, 34, 35, 36, 39};
static uint8_t OnlyGPI[] = {34, 35, 36, 39};

static ledc_timer_config_t      LEDCTimer1_5KHz; 
static ledc_timer_config_t      LEDCTimer20KHz;
static ledc_channel_config_t    LEDCChannelFan0;
static ledc_channel_config_t    LEDCChannelFan1;
static ledc_channel_config_t    LEDCChannelLED;
static ledc_channel_config_t    LEDCChannelHoazLED;

IO_PWMChannels_t IO_PWMChannels;

static void RestetPin(uint8_t Pin)
{
    gpio_reset_pin(Pin);
    gpio_set_pull_mode(Pin, GPIO_FLOATING);
}

void IO_PWMUpdate()
{
    IO_PWMChannel_t *akt = (IO_PWMChannel_t*)&IO_PWMChannels;
    uint8_t i, Anzahl;

    Anzahl = sizeof(IO_PWMChannels_t) / sizeof(IO_PWMChannel_t);
    for(i=0; i < Anzahl; i++)
    {
        akt->Ist = ledc_get_duty(akt->Channel->speed_mode, akt->Channel->channel);
        if(akt->Soll != akt->Ist)
        {
            ledc_set_duty(akt->Channel->speed_mode, akt->Channel->channel, akt->Soll);
            ledc_update_duty(akt->Channel->speed_mode, akt->Channel->channel);
        }
        akt++;
    }
}

void IO_Init()
{
    uint8_t i;

//    for(i=0; i < sizeof(GPIOAvailable); i++)
//        RestetPin(i);

    IO_PWMChannels.Fan0 = (IO_PWMChannel_t){
        .Ist = 0,
        .Soll = 2047,
        .Min = 0,
        .Max = 1600,
        .Start = 1300,
        .Invers = true,
        .Timer = &LEDCTimer20KHz,
        .Channel = &LEDCChannelFan0
    };

    IO_PWMChannels.Fan1 = (IO_PWMChannel_t){
        .Ist = 0,
        .Soll = 2047,
        .Min = 0,
        .Max = 1600,
        .Start = 1300,
        .Invers = true,
        .Timer = &LEDCTimer20KHz,
        .Channel = &LEDCChannelFan1
    };

    IO_PWMChannels.LED = (IO_PWMChannel_t){
        .Ist = 0,
        .Soll = 4095,
        .Min = 0,
        .Max = 3600,
        .Start = 3400,
        .Invers = true,
        .Timer = &LEDCTimer1_5KHz,
        .Channel = &LEDCChannelLED
    };

    IO_PWMChannels.HoazLED = (IO_PWMChannel_t){
        .Ist = 0,
        .Soll = 0,
        .Min = 0,
        .Max = 0,
        .Start = 0,
        .Invers = false,
        .Timer = &LEDCTimer20KHz,
        .Channel = &LEDCChannelHoazLED
    };

    LEDCTimer1_5KHz = (ledc_timer_config_t){
        .speed_mode       = LEDC_HIGH_SPEED_MODE,
        .timer_num        = LEDC_TIMER_0,
        .duty_resolution  = LEDC_TIMER_12_BIT,
        .freq_hz          = 1500,  // Set output frequency at 1,5 kHz
        .clk_cfg          = LEDC_USE_APB_CLK
    };

    LEDCTimer20KHz = (ledc_timer_config_t){
        .speed_mode       = LEDC_HIGH_SPEED_MODE,
        .timer_num        = LEDC_TIMER_1,
        .duty_resolution  = LEDC_TIMER_11_BIT,
        .freq_hz          = 22500,  // Set output frequency at 20 kHz
        .clk_cfg          = LEDC_USE_APB_CLK
    };

    LEDCChannelFan0 = (ledc_channel_config_t){
        .speed_mode     = IO_PWMChannels.Fan1.Timer->speed_mode,
        .channel        = LEDC_CHANNEL_0,
        .timer_sel      = IO_PWMChannels.Fan0.Timer->timer_num,
        .intr_type      = LEDC_INTR_DISABLE,
        .gpio_num       = GPIO_FAN0_PWM,
        .duty           = 0x7FF,        // inverse Logik -> komplett aus 
        .hpoint         = 0
    };

    LEDCChannelFan1 =  (ledc_channel_config_t){
        .speed_mode     = IO_PWMChannels.Fan1.Timer->speed_mode,
        .channel        = LEDC_CHANNEL_1,
        .timer_sel      = IO_PWMChannels.Fan1.Timer->timer_num,
        .intr_type      = LEDC_INTR_DISABLE,
        .gpio_num       = GPIO_FAN1_PWM,
        .duty           = 0x7FF,        // inverse Logik -> komplett aus 
        .hpoint         = 0
    };

    LEDCChannelLED =  (ledc_channel_config_t){
        .speed_mode     = IO_PWMChannels.LED.Timer->speed_mode,
        .channel        = LEDC_CHANNEL_2,
        .timer_sel      = IO_PWMChannels.LED.Timer->timer_num,
        .intr_type      = LEDC_INTR_DISABLE,
        .gpio_num       = GPIO_LED_PWM,
        .duty           = 0xFFF,        // inverse Logik -> komplett aus 
        .hpoint         = 0
    };

    LEDCChannelHoazLED = (ledc_channel_config_t){
        .speed_mode     = IO_PWMChannels.HoazLED.Timer->speed_mode,
        .channel        = LEDC_CHANNEL_3,
        .timer_sel      = IO_PWMChannels.HoazLED.Timer->timer_num,
        .intr_type      = LEDC_INTR_DISABLE,
        .gpio_num       = GPIO_HOAZLED_PWM,
        .duty           = 0,            // Set duty to 0%
        .hpoint         = 0
    };

    ledc_timer_config(&LEDCTimer1_5KHz);
    ledc_timer_config(&LEDCTimer20KHz);
    ledc_channel_config(&LEDCChannelFan0);
    ledc_channel_config(&LEDCChannelFan1);
    ledc_channel_config(&LEDCChannelLED);
    ledc_channel_config(&LEDCChannelHoazLED);

    gpio_set_direction(GPIO_LED_RELAIS, GPIO_MODE_OUTPUT);  //LED
    gpio_set_direction(GPIO_HEIZMATTE_RELAIS, GPIO_MODE_OUTPUT);  // Heizmatte
    gpio_set_direction(GPIO_GEBER_TASTER, GPIO_MODE_INPUT);  // Taster Drehgeber
    gpio_set_pull_mode(GPIO_GEBER_TASTER, GPIO_PULLUP_ONLY);
}