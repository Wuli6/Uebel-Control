/**
* @file AHT10.c
* @brief 
*
* @author Christopher Sommerauer
*
* @date 2023-01-07
*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/i2c.h"

#define I2C_MASTER_SCL_IO           27      /*!< GPIO number used for I2C master clock */
#define I2C_MASTER_SDA_IO           26      /*!< GPIO number used for I2C master data  */
#define I2C_MASTER_NUM              0                          /*!< I2C master i2c port number, the number of i2c peripheral interfaces available will depend on the chip */
#define I2C_MASTER_FREQ_HZ          100000                     /*!< I2C master clock frequency */
#define I2C_MASTER_TX_BUF_DISABLE   0                          /*!< I2C master doesn't need buffer */
#define I2C_MASTER_RX_BUF_DISABLE   0                          /*!< I2C master doesn't need buffer */
#define I2C_MASTER_TIMEOUT_MS       1000

#define AHT10_SENSOR_ADDR         0x38        /*!< Slave address of the AHT10 sensor */




#include <stdint.h>
#include <math.h>
#include "AHT10.h"

// Specify the constants for water vapor and barometric pressure.
#define WATER_VAPOR 17.62f
#define BAROMETRIC_PRESSURE 243.5f

uint8_t eSensorInitializeCmd[3] = {0xE1, 0x08, 0x00};
uint8_t eSensorNormalCmd[3]    = {0xA8, 0x00, 0x00};
uint8_t eSensorMeasureCmd[3]   = {0xAC, 0x33, 0x00};
uint8_t eSensorResetCmd        = 0xBA;

float AHT10_Temperature, AHT10_Humidity, AHT10_DewPoint;
i2c_config_t conf = {
    .mode = I2C_MODE_MASTER,
    .sda_io_num = I2C_MASTER_SDA_IO,
    .scl_io_num = I2C_MASTER_SCL_IO,
    .sda_pullup_en = GPIO_PULLUP_ENABLE,
    .scl_pullup_en = GPIO_PULLUP_ENABLE,
    .master.clk_speed = I2C_MASTER_FREQ_HZ,
};




void AHT10_begin()
{
    uint8_t temp;
    i2c_master_write_to_device(I2C_MASTER_NUM, AHT10_SENSOR_ADDR, eSensorInitializeCmd, sizeof(eSensorInitializeCmd), I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS);
    vTaskDelay(100);
    i2c_master_read_from_device(I2C_MASTER_NUM, AHT10_SENSOR_ADDR, &temp, 1, I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS);
}


float AHT10_GetDewPoint()
{
  // Calculate the intermediate value 'gamma'
  float gamma = log(AHT10_Humidity / 100) + WATER_VAPOR * AHT10_Temperature / (BAROMETRIC_PRESSURE + AHT10_Temperature);
  // Calculate dew point in Celsius
  float dewPoint = BAROMETRIC_PRESSURE * gamma / (WATER_VAPOR - gamma);

  return dewPoint;
}

void AHT10_readSensor()
{
    uint8_t temp[6];

    i2c_master_write_to_device(I2C_MASTER_NUM, AHT10_SENSOR_ADDR, eSensorMeasureCmd, sizeof(eSensorMeasureCmd), I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS);
    vTaskDelay(200);
    i2c_master_read_from_device(I2C_MASTER_NUM, AHT10_SENSOR_ADDR, &temp[0], 6, I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS);

    AHT10_Humidity = (double)((temp[1] << 12) | (temp[2] << 4) | ((temp[3] >> 4) & 0x0F)) * 0.000095367431640625f;
    AHT10_Temperature = (double)(((temp[3] & 0x0F) << 16) | (temp[4] << 8) | temp[5]) * 0.00019073486328125f - 50;
}

uint8_t AHT10_readStatus()
{
    uint8_t result = 0;

    i2c_master_read_from_device(I2C_MASTER_NUM, AHT10_SENSOR_ADDR, &result, 1, I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS);
    return result;
}

void AHT10_Reset()
{
    i2c_master_write_to_device(I2C_MASTER_NUM, AHT10_SENSOR_ADDR, &eSensorResetCmd, 1, I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS);
}

void AHT10_Init()
{
    i2c_param_config(0, &conf);
    i2c_driver_install(I2C_MASTER_NUM, conf.mode, I2C_MASTER_RX_BUF_DISABLE, I2C_MASTER_TX_BUF_DISABLE, 0);
}

void AHT_Cyclic(void *arg)
{
    AHT10_Reset();
    vTaskDelay(20);
    AHT10_begin();

    while(1)
    {
        AHT10_readSensor();
        AHT10_DewPoint = AHT10_GetDewPoint();
        vTaskDelay(1000);
    }

}