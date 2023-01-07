//#include "freertos/FreeRTOS.h"
//#include "freertos/task.h"
//#include "driver/gpio.h"

#include "IO.h"
#include "RotaryEncoder.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"



#define LED_DIM                 3400
#define LED_UNDIM               1800

rotary_encoder_t *encoder = NULL;

void Startup()
{
    IO_Init();

    IO_PWMChannels.Fan0.Soll = 1300;
    IO_PWMChannels.Fan1.Soll = 800;
    IO_PWMChannels.LED.Soll = 1800;
    IO_PWMChannels.HoazLED.Soll = 0;
    gpio_set_level(GPIO_LED_RELAIS, 1);
    gpio_set_level(GPIO_HEIZMATTE_RELAIS, 0);

    uint32_t pcnt_unit = 0;
    rotary_encoder_config_t config = ROTARY_ENCODER_DEFAULT_CONFIG((rotary_encoder_dev_t)pcnt_unit, 34, 35);
    rotary_encoder_new_ec11(&config, &encoder);
    encoder->set_glitch_filter(encoder, 1);    // Filter out glitch (1us)
    encoder->start(encoder);   // Start encoder
 }


void app_main(void)
{
    bool    DimLED = false;
    bool    DimLock = false;
    int32_t Counter;

    Startup();

    while (1)
    {
        if(gpio_get_level(GPIO_GEBER_TASTER) == 0)
        {
            if(!DimLock)
            {
                DimLED = !DimLED;
                DimLock = true;
            }
        }
        else
        {
            DimLock = false;
        }
 
        if(DimLED)
            IO_PWMChannels.LED.Soll = LED_DIM;
        else
            IO_PWMChannels.LED.Soll = LED_UNDIM;

        IO_PWMUpdate();
        Counter = encoder->get_counter_value(encoder);
        vTaskDelay(10);
    }
}
