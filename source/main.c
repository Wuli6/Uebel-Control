//#include "freertos/FreeRTOS.h"
//#include "freertos/task.h"
//#include "driver/gpio.h"

#include "IO.h"
#include "RotaryEncoder.h"
#include "AHT10.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "gui.h"


#include "lvgl.h"

#include "lvgl_helpers.h"

/*********************
 *      DEFINES
 *********************/
#define TAG "demo"
#define LV_TICK_PERIOD_MS 1

#define DISP_RES_HOR CONFIG_LV_X_RES
#define DISP_RES_VER CONFIG_LV_Y_RES
#ifdef CONFIG_LV_TFT_DISPLAY_OFFSETS
#define DISP_OFF_HOR CONFIG_LV_TFT_DISPLAY_X_OFFSET
#define DISP_OFF_VER CONFIG_LV_TFT_DISPLAY_Y_OFFSET
#else
#define DISP_OFF_HOR 0
#define DISP_OFF_VER 0
#endif

#define LED_DIM                 3400

rotary_encoder_t *encoder = NULL;


lv_disp_drv_t disp_drv;
lv_disp_draw_buf_t disp_buf;
lv_indev_t *my_indev;
lv_indev_drv_t indev_drv;

static void lv_tick_task(void *arg) 
{
    lv_tick_inc(LV_TICK_PERIOD_MS);
}

static int32_t CounterOld = 0;
static int32_t IncMax = 0;

static void encoder_read(lv_indev_drv_t * drv, lv_indev_data_t*data)
{
    int32_t Counter = 0;
    int32_t diff, inc;

    Counter = encoder->get_counter_value(encoder);
    diff = Counter - CounterOld;
    if(diff > 3 || diff <= -3)
    {
        inc =((diff * diff) >> 6) + 1;
        if(inc > IncMax)
            IncMax = inc;
        if(diff < 0)
            inc = -inc;
        data->enc_diff = inc;
        CounterOld = Counter;
    }
    if(gpio_get_level(GPIO_GEBER_TASTER)) 
        data->state = LV_INDEV_STATE_RELEASED;
    else
    {
        data->state = LV_INDEV_STATE_PRESSED;
        IncMax = 0;
    }
}

static void DisplayInit()
{

    lv_color_t *buf1, *buf2;
    lv_init();
    lvgl_driver_init();

    buf1 = heap_caps_malloc(DISP_BUF_SIZE * sizeof(lv_color_t), MALLOC_CAP_DMA);
    assert(buf1 != NULL);
    buf2 = heap_caps_malloc(DISP_BUF_SIZE * sizeof(lv_color_t), MALLOC_CAP_DMA);
    assert(buf2 != NULL);
    lv_disp_draw_buf_init(&disp_buf, buf1, buf2, DISP_BUF_SIZE);

    lv_disp_drv_init(&disp_drv);
    disp_drv.flush_cb = disp_driver_flush;
    disp_drv.hor_res = DISP_RES_HOR;
    disp_drv.ver_res = DISP_RES_VER;
    disp_drv.physical_hor_res = -1;
    disp_drv.physical_ver_res = -1;
    disp_drv.offset_x = DISP_OFF_HOR;
    disp_drv.offset_y = DISP_OFF_VER;
    disp_drv.antialiasing = 1;
#if defined CONFIG_DISPLAY_ORIENTATION_PORTRAIT || defined CONFIG_DISPLAY_ORIENTATION_PORTRAIT_INVERTED
    disp_drv.rotated = 1;
#endif
    disp_drv.draw_buf = &disp_buf;
    lv_disp_drv_register(&disp_drv);


    rotary_encoder_config_t config = ROTARY_ENCODER_DEFAULT_CONFIG((rotary_encoder_dev_t)0, 34, 35);
    rotary_encoder_new_ec11(&config, &encoder);
    encoder->set_glitch_filter(encoder, 1);    // Filter out glitch (1us)
    encoder->start(encoder);   // Start encoder
    gpio_set_direction(GPIO_GEBER_TASTER, GPIO_MODE_INPUT);  // Taster Drehgeber
    gpio_set_pull_mode(GPIO_GEBER_TASTER, GPIO_PULLUP_ONLY);
    
    lv_indev_drv_init(&indev_drv);
    indev_drv.read_cb = encoder_read;
    indev_drv.type = LV_INDEV_TYPE_ENCODER;
    my_indev = lv_indev_drv_register(&indev_drv);
}

/* Creates a semaphore to handle concurrent call to lvgl stuff
 * If you wish to call *any* lvgl function from other threads/tasks
 * you should lock on the very same semaphore! */
//SemaphoreHandle_t xGuiSemaphore;
static void guiTask(void *pvParameter) 
{
    /* Create and start a periodic timer interrupt to call lv_tick_inc */
    const esp_timer_create_args_t periodic_timer_args = {
        .callback = &lv_tick_task,
        .name = "periodic_gui"
    };
    esp_timer_handle_t periodic_timer;
    ESP_ERROR_CHECK(esp_timer_create(&periodic_timer_args, &periodic_timer));
    ESP_ERROR_CHECK(esp_timer_start_periodic(periodic_timer, LV_TICK_PERIOD_MS * 1000));

    CreateGUI();

    while (1)
    {
        /* Delay 1 tick (assumes FreeRTOS tick is 10ms */
        vTaskDelay(pdMS_TO_TICKS(10));

        TabTempValueUpdate();
        lv_task_handler();
    }
}

void Startup()
{
    IO_Init();
    AHT10_Init();
    xTaskCreatePinnedToCore(AHT_Cyclic, "AHT10", 4096, NULL, 5, NULL, 1);

    IO_PWMChannels.Fan0.Soll = 1300;
    IO_PWMChannels.Fan1.Soll = 800;
    IO_PWMChannels.LED.Soll = 700;
    IO_PWMChannels.HoazLED.Soll = 0;
    IO_PWMChannels.Display.Soll = 1024;
    gpio_set_level(GPIO_LED_RELAIS, 1);
    gpio_set_level(GPIO_HEIZMATTE_RELAIS, 0);

    // uint32_t pcnt_unit = 0;
    // rotary_encoder_config_t config = ROTARY_ENCODER_DEFAULT_CONFIG((rotary_encoder_dev_t)pcnt_unit, 34, 35);
    // rotary_encoder_new_ec11(&config, &encoder);
    // encoder->set_glitch_filter(encoder, 1);    // Filter out glitch (1us)
    // encoder->start(encoder);   // Start encoder

    DisplayInit();
    xTaskCreatePinnedToCore(guiTask, "gui", 4096*2, NULL, 0, NULL, 1);
 }

void app_main(void)
{
    bool    DimLED = false;
    bool    DimLock = false;
    int32_t Counter;
    uint16_t LED;

    Startup();

    while (1)
    {
        // if(gpio_get_level(GPIO_GEBER_TASTER) == 0)
        // {
        //     if(!DimLock)
        //     {
        //         DimLED = !DimLED;
        //         DimLock = true;

        //         if(DimLED)
        //         {
        //             LED = IO_PWMChannels.LED.Soll;
        //             IO_PWMChannels.LED.Soll = LED_DIM;
        //         }
        //         else
        //             IO_PWMChannels.LED.Soll = LED;
        //     }
        // }
        // else
        // {
        //     DimLock = false;
        // }
 

        IO_PWMUpdate();
        // Counter = encoder->get_counter_value(encoder);
        vTaskDelay(10);
    }
}
