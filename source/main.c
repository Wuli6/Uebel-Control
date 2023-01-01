#include "cyclic.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void app_main(void)
{
    unsigned long test = 0;
    test++;
    vTaskDelay(pdMS_TO_TICKS(100));
    test++;
    Cyclic_Init();
    test++;
    Cyclic_Start();
//    vTaskDelay(pdMS_TO_TICKS(4000));
//    Cyclic_Stop();
}
