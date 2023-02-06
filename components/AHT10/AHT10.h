#pragma once
/**
* @file AHT10.h
* @brief 
*
* @author Christopher Sommerauer
*
* @date 2020-01-07
*/

typedef struct
{
    float Temperature;
    float Humidity;
    float DewPoint;
    float TemperatureMin;
    float TemperatureMax;
    float HumidityMin;
    float HumidityMax;
}AHT10_Data_t;

extern AHT10_Data_t AHT10_Data;

void begin();
void AHT10_readSensor();
uint8_t AHT10_readStatus();
void AHT10_Reset();
void AHT10_Init();

extern void AHT_Cyclic(void *arg);
extern void AHT10_ResetMinMax();
