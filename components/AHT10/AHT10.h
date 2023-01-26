#pragma once
/**
* @file AHT10.h
* @brief 
*
* @author Christopher Sommerauer
*
* @date 2020-01-07
*/

void begin();
float AHT10_GetDewPoint();
void AHT10_readSensor();
uint8_t AHT10_readStatus();
void AHT10_Reset();
void AHT10_Init();

void AHT_Cyclic(void *arg);
