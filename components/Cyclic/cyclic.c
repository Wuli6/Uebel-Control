/**
* @file cyclic.c
* @brief 
*
* @author Christopher Sommerauer
*
* @date 2022-12-28
*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_timer.h"

#include "driver/gpio.h"
#include "driver/ledc.h"

#define CYCLIC_MAIN_PRIO 24
#define CYCLETIME 500

static SemaphoreHandle_t EnabelMainCyclic;
static esp_timer_handle_t periodic_timer;
static TaskHandle_t CyclicMain;


static void Cyclic(void *arg)
{
    int16_t led_state = 0;
    int16_t led_inc = 2;
    int64_t timelast = 0, timeact, diff, cycles = 0;;
//    gpio_reset_pin(2);
//    gpio_set_direction(2, GPIO_MODE_OUTPUT);
    led_state = 2047;
//    ledc_set_duty_and_update(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, led_state, 0);
    printf("Jitter is only printed if not zero! Difference from cycletime in microseconds.\n");
    printf("Cycletime: %d microseconds\n", CYCLETIME);
    vTaskDelay(20);
    timelast = esp_timer_get_time();

    while (1)
    {
        xSemaphoreTake(EnabelMainCyclic, portMAX_DELAY);
        xSemaphoreGive(EnabelMainCyclic);
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        cycles++;
//        gpio_set_level(2, led_state);
//        led_state = !led_state;
        timeact = esp_timer_get_time();
        diff = timeact-timelast-CYCLETIME;
        timelast = timeact;
        if(diff != 0)
            printf("Cycle: %lld, Diff: %lld;\n",cycles, diff);
        
        if(cycles%20000 == 0)
            printf(".\n");

        led_state += led_inc;
        if(led_state > 2559)
        {
            led_state = 2559;
            led_inc *= -1;
        }
        else if(led_state < 0)
        {
            led_state = 0;
            led_inc *= -1;
        }
        ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0, led_state);
        ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0);
    }
}

void Cyclic_Start()
{
    xSemaphoreGive(EnabelMainCyclic);
}

void Cyclic_Stop()
{
    xSemaphoreTake(EnabelMainCyclic, portMAX_DELAY);
}

static void MainTimerCallback(void* arg)
{
    vTaskNotifyGiveFromISR(CyclicMain, NULL);
    esp_timer_isr_dispatch_need_yield();
}

void Cyclic_Init()
{
    const esp_timer_create_args_t periodic_timer_args = 
    {
            .callback = &MainTimerCallback,
            .dispatch_method = ESP_TIMER_ISR,
            .name = "MainTimer"
    };

    esp_timer_create(&periodic_timer_args, &periodic_timer);
    esp_timer_start_periodic(periodic_timer, CYCLETIME);

    EnabelMainCyclic = xSemaphoreCreateBinary();
    xTaskCreatePinnedToCore(Cyclic, "Cyclic Main", 10000, NULL, CYCLIC_MAIN_PRIO, &CyclicMain, 0);



    ledc_timer_config_t ledc_timer = {
        .speed_mode       = LEDC_HIGH_SPEED_MODE,
        .timer_num        = LEDC_TIMER_0,
        .duty_resolution  = LEDC_TIMER_12_BIT,
        .freq_hz          = 5000,  // Set output frequency at 5 kHz
        .clk_cfg          = LEDC_USE_APB_CLK
    };
    ledc_timer_config(&ledc_timer);

    ledc_channel_config_t ledc_channel = {
        .speed_mode     = LEDC_HIGH_SPEED_MODE,
        .channel        = LEDC_CHANNEL_0,
        .timer_sel      = LEDC_TIMER_0,
        .intr_type      = LEDC_INTR_DISABLE,
        .gpio_num       = 2,
        .duty           = 0, // Set duty to 0%
        .hpoint         = 0
    };
    ledc_channel_config(&ledc_channel);
}