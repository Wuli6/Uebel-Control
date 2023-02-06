#pragma once
/**
* @file IO.h
* @brief 
*
* @author Christopher Sommerauer
*
* @date 2023-02-03
*/


#ifdef __cplusplus
extern "C" {
#endif

#ifdef LV_LVGL_H_INCLUDE_SIMPLE
#include "lvgl.h"
#else
#include "lvgl/lvgl.h"
#endif

//#include "lvgl_esp32_drivers/lvgl_helpers.h"

typedef struct
{
    lv_obj_t *Tab;
    lv_obj_t *ImgTemp;
    lv_obj_t *TempMin;
    lv_obj_t *Temp;
    lv_obj_t *TempMax;
    lv_obj_t *ImgHum;
    lv_obj_t *HumMin;
    lv_obj_t *Hum;
    lv_obj_t *HumMax;
    lv_obj_t *BtnReset;
}TabTempObjs_t;

typedef struct
{
    lv_obj_t *Tab;
    lv_obj_t *SldrFan1;
    lv_obj_t *LblFan1;
    lv_obj_t *SldrFan2;
    lv_obj_t *LblFan2;
    lv_obj_t *SwAuto;
}TabFanObjs_t;

typedef struct
{
    lv_obj_t *Tab;
    lv_obj_t *SldrLight;
    lv_obj_t *LblLight;
    lv_obj_t *TxtOn;
    lv_obj_t *TxtOff;
    lv_obj_t *SwAuto;
    lv_obj_t *SwOnOff;
}TabLightObjs_t;

typedef struct
{
    lv_obj_t *Tab;
    lv_obj_t *SwMat;
    lv_obj_t *SwLED;
    lv_obj_t *SldrDay;
    lv_obj_t *LblDay;
    lv_obj_t *SldrNight;
    lv_obj_t *LblNight;
    lv_obj_t *SldrHum;
    lv_obj_t *LblHum;
}TabHeatObjs_t;

typedef struct
{
    lv_obj_t *TabView;

    TabTempObjs_t   TabTemp;
    TabFanObjs_t    TabFan;
    TabLightObjs_t  TabLight;
    TabHeatObjs_t   TabHeat;
}GUIObjs_t;

typedef struct
{
    lv_style_t Slider;
    lv_style_t Button;
    lv_style_t Switch;
    lv_style_t Lable;
    lv_style_t TextArea;
    lv_style_t TabView;
}Style_t;

extern const lv_img_dsc_t ImgHum;
extern const lv_img_dsc_t ImgTemp;

extern void CreateGUI();

extern void TabTempValueUpdate();

#ifdef __cplusplus
} /* extern "C" */
#endif
