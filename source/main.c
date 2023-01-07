//#include "freertos/FreeRTOS.h"
//#include "freertos/task.h"
//#include "driver/gpio.h"

#include "IO.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"



#define LED_DIM                 3400
#define LED_UNDIM               1800


void Startup()
{
    IO_Init();

    IO_PWMChannels.Fan0.Soll = 1300;
    IO_PWMChannels.Fan1.Soll = 800;
    IO_PWMChannels.LED.Soll = 1800;
    IO_PWMChannels.HoazLED.Soll = 0;
    gpio_set_level(GPIO_LED_RELAIS, 1);
    gpio_set_level(GPIO_HEIZMATTE_RELAIS, 0);
}


void app_main(void)
{
    bool    DimLED = false;
    bool    DimLock = false;

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
        vTaskDelay(10);
    }
}
