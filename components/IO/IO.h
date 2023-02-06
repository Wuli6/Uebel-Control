#pragma once
/**
* @file IO.h
* @brief 
*
* @author Christopher Sommerauer
*
* @date 2023-01-07
*/

#include "driver/ledc.h"

#define GPIO_LED_RELAIS         22
#define GPIO_HEIZMATTE_RELAIS   21
#define GPIO_LED_PWM            4
#define GPIO_FAN0_PWM           32
#define GPIO_FAN1_PWM           33
#define GPIO_DISPLAY_PWM        17
#define GPIO_HOAZLED_PWM        16
#define GPIO_GEBER_TASTER       25

#define PWM_CHANNEL_LED         LEDC_CHANNEL_2
#define PWM_CHANNEL_FAN0        LEDC_CHANNEL_0
#define PWM_CHANNEL_FAN1        LEDC_CHANNEL_1
#define PWM_CHANNEL_HOAZLED     LEDC_CHANNEL_3

typedef struct
{
    uint16_t                Soll;
    uint16_t                Ist;
    uint16_t                Min;           //Zuluft 0 = Vollgas bis 1600, 1300 min zum starten
    uint16_t                Max;            //Umluft
    uint16_t                Start;            // 0 Vollgas bis 3600 min, 3400 zum starten
    bool                    Invers;
    ledc_timer_config_t    *Timer;
    ledc_channel_config_t  *Channel;
}IO_PWMChannel_t;

typedef struct
{
    IO_PWMChannel_t         Fan0;           //Zuluft 0 = Vollgas bis 1600, 1300 min zum starten
    IO_PWMChannel_t         Fan1;            //Umluft
    IO_PWMChannel_t         LED;            // 0 Vollgas bis 3600 min, 3400 zum starten
    IO_PWMChannel_t         HoazLED;
    IO_PWMChannel_t         Display;
}IO_PWMChannels_t;


// typedef struct IO_PWMValues
// {
//     int16_t Fan0;           //Zuluft 0 = Vollgas bis 1600, 1300 min zum starten
//     int16_t Fan1;            //Umluft
//     int16_t LED;            // 0 Vollgas bis 3600 min, 3400 zum starten
//     int16_t HoazLED;
// }IO_PWMValues;

extern IO_PWMChannels_t IO_PWMChannels;

void IO_PWMUpdate();
void IO_Init();