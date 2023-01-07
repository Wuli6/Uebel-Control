#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/ledc.h"

#define GPIO_LED_RELAIS         22
#define GPIO_HEIZMATTE_RELAIS   21
#define GPIO_LED_PWM            4
#define GPIO_FAN0_PWM           32
#define GPIO_FAN1_PWM           33
#define GPIO_HOAZLED_PWM        16
#define GPIO_GEBER_TASTER       25

#define PWM_CHANNEL_LED         LEDC_CHANNEL_2
#define PWM_CHANNEL_FAN0        LEDC_CHANNEL_0
#define PWM_CHANNEL_FAN1        LEDC_CHANNEL_1
#define PWM_CHANNEL_HOAZLED     LEDC_CHANNEL_3

#define LED_DIM                 3400


void Startup()
{
    ledc_timer_config_t LEDCTimer1KHz = 
    {
        .speed_mode       = LEDC_HIGH_SPEED_MODE,
        .timer_num        = LEDC_TIMER_0,
        .duty_resolution  = LEDC_TIMER_12_BIT,
        .freq_hz          = 1500,  // Set output frequency at 1,5 kHz
        .clk_cfg          = LEDC_USE_APB_CLK
    };
    ledc_timer_config(&LEDCTimer1KHz);

    ledc_timer_config_t LEDCTimer20KHz =
    {
        .speed_mode       = LEDC_HIGH_SPEED_MODE,
        .timer_num        = LEDC_TIMER_1,
        .duty_resolution  = LEDC_TIMER_11_BIT,
        .freq_hz          = 22500,  // Set output frequency at 20 kHz
        .clk_cfg          = LEDC_USE_APB_CLK
    };
    ledc_timer_config(&LEDCTimer20KHz);

    ledc_channel_config_t LEDCChannelFan0 =
    {
        .speed_mode     = LEDC_HIGH_SPEED_MODE,
        .channel        = LEDC_CHANNEL_0,
        .timer_sel      = LEDC_TIMER_1,
        .intr_type      = LEDC_INTR_DISABLE,
        .gpio_num       = GPIO_FAN0_PWM,
        .duty           = 0, // Set duty to 0%
        .hpoint         = 0
    };
    ledc_channel_config(&LEDCChannelFan0);

    ledc_channel_config_t LEDCChannelFan1 =
    {
        .speed_mode     = LEDC_HIGH_SPEED_MODE,
        .channel        = LEDC_CHANNEL_1,
        .timer_sel      = LEDC_TIMER_1,
        .intr_type      = LEDC_INTR_DISABLE,
        .gpio_num       = GPIO_FAN1_PWM,
        .duty           = 0, // Set duty to 0%
        .hpoint         = 0
    };
    ledc_channel_config(&LEDCChannelFan1);

    ledc_channel_config_t LEDCChannelLED =
    {
        .speed_mode     = LEDC_HIGH_SPEED_MODE,
        .channel        = LEDC_CHANNEL_2,
        .timer_sel      = LEDC_TIMER_0,
        .intr_type      = LEDC_INTR_DISABLE,
        .gpio_num       = GPIO_LED_PWM,
        .duty           = 0, // Set duty to 0%
        .hpoint         = 0
    };
    ledc_channel_config(&LEDCChannelLED);

    ledc_channel_config_t LEDCChannelHoazLED =
    {
        .speed_mode     = LEDC_HIGH_SPEED_MODE,
        .channel        = LEDC_CHANNEL_3,
        .timer_sel      = LEDC_TIMER_1,
        .intr_type      = LEDC_INTR_DISABLE,
        .gpio_num       = GPIO_HOAZLED_PWM,
        .duty           = 0, // Set duty to 0%
        .hpoint         = 0
    };
    ledc_channel_config(&LEDCChannelHoazLED);

    gpio_reset_pin(GPIO_LED_RELAIS);
    gpio_reset_pin(GPIO_HEIZMATTE_RELAIS);
    gpio_set_direction(GPIO_LED_RELAIS, GPIO_MODE_OUTPUT);  //LED
    gpio_set_direction(GPIO_HEIZMATTE_RELAIS, GPIO_MODE_OUTPUT);  // Heizmatte
    gpio_set_direction(GPIO_GEBER_TASTER, GPIO_MODE_INPUT);  // Taster Drehgeber
    gpio_set_pull_mode(GPIO_GEBER_TASTER, GPIO_PULLUP_ONLY);
    gpio_set_level(GPIO_LED_RELAIS, 1);
    gpio_set_level(GPIO_HEIZMATTE_RELAIS, 1);
}


void app_main(void)
{
    int32_t PWMFan0Soll=1300;           //Zuluft 0 = Vollgas bis 1600, 1300 min zum starten
    int32_t PWMFan1Soll=800;            //Umluft
    int32_t PWMLEDSoll=1800;            // 0 Vollgas bis 3600 min, 3400 zum starten
    int32_t PWMHoazLEDSoll=0;

    int32_t PWMFan0Ist=0;
    int32_t PWMFan1Ist=0;
    int32_t PWMLEDIst=0;
    int32_t PWMHoazLEDIst=0;
    bool    DimLED = false;
    bool    DimLock = false;

    Startup();

    while (1)
    {
        if(gpio_get_level(GPIO_GEBER_TASTER) == 0)
        {
            if(!DimLock)
            {
                DimLED = ! DimLED;
                DimLock = true;
            }
        }
        else
        {
            DimLock = false;
        }

        if(PWMFan0Soll != PWMFan0Ist)
        {
            ledc_set_duty(LEDC_HIGH_SPEED_MODE, PWM_CHANNEL_FAN0, PWMFan0Soll);
            ledc_update_duty(LEDC_HIGH_SPEED_MODE, PWM_CHANNEL_FAN0);
            PWMFan0Ist = PWMFan0Soll;
        }
        if(PWMFan1Soll != PWMFan1Ist)
        {
            ledc_set_duty(LEDC_HIGH_SPEED_MODE, PWM_CHANNEL_FAN1, PWMFan1Soll);
            ledc_update_duty(LEDC_HIGH_SPEED_MODE, PWM_CHANNEL_FAN1);
            PWMFan1Ist = PWMFan1Soll;
        }
        if(DimLED)
        {
            if(PWMLEDIst != LED_DIM)
            {
                PWMLEDIst = LED_DIM;
                ledc_set_duty(LEDC_HIGH_SPEED_MODE, PWM_CHANNEL_LED, PWMLEDIst);
                ledc_update_duty(LEDC_HIGH_SPEED_MODE, PWM_CHANNEL_LED);
            }
        }
        else if(PWMLEDSoll != PWMLEDIst)
        {
            ledc_set_duty(LEDC_HIGH_SPEED_MODE, PWM_CHANNEL_LED, PWMLEDSoll);
            ledc_update_duty(LEDC_HIGH_SPEED_MODE, PWM_CHANNEL_LED);
            PWMLEDIst = PWMLEDSoll;
        }
        if(PWMHoazLEDSoll != PWMHoazLEDIst)
        {
            ledc_set_duty(LEDC_HIGH_SPEED_MODE, PWM_CHANNEL_HOAZLED, PWMHoazLEDSoll);
            ledc_update_duty(LEDC_HIGH_SPEED_MODE, PWM_CHANNEL_HOAZLED);
            PWMHoazLEDIst = PWMHoazLEDSoll;
        }
    }
}
