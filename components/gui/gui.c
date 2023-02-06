

#include "gui.h"
#include "AHT10.h"

static lv_group_t *g;
extern lv_indev_t *my_indev;

GUIObjs_t GUIObjs;
Style_t Style;


void TabTempValueUpdate()
{
    TabTempObjs_t *ActTab = &GUIObjs.TabTemp;
    AHT10_Data_t *Data = &AHT10_Data;

    lv_label_set_text_fmt(ActTab->TempMin, "%.1f", Data->TemperatureMin);
    lv_label_set_text_fmt(ActTab->Temp, "%.1f", Data->Temperature);
    lv_label_set_text_fmt(ActTab->TempMax, "%.1f", Data->TemperatureMax);
    lv_label_set_text_fmt(ActTab->HumMin, "%.1f", Data->HumidityMin);
    lv_label_set_text_fmt(ActTab->Hum, "%.1f", Data->Humidity);
    lv_label_set_text_fmt(ActTab->HumMax, "%.1f", Data->HumidityMax);
}
static void TabTempResetHandler(lv_event_t * e)
{
    AHT10_ResetMinMax();
}
static void TabTemp(lv_obj_t * parent)
{
    TabTempObjs_t *ActTab = &GUIObjs.TabTemp;

    ActTab->ImgTemp = lv_img_create(parent);
    lv_img_set_src(ActTab->ImgTemp, &ImgTemp);
    lv_obj_align(ActTab->ImgTemp, LV_ALIGN_TOP_LEFT, 3, 0);
    ActTab->TempMin =  lv_label_create(parent);
    lv_label_set_text(ActTab->TempMin, "");
    lv_obj_align(ActTab->TempMin, LV_ALIGN_TOP_MID, -20, 0);
    ActTab->Temp =  lv_label_create(parent);
    lv_label_set_text(ActTab->Temp, "");
    lv_obj_align(ActTab->Temp, LV_ALIGN_TOP_MID, 10, 0);
    ActTab->TempMax =  lv_label_create(parent);
    lv_label_set_text(ActTab->TempMax, "");
    lv_obj_align(ActTab->TempMax, LV_ALIGN_TOP_MID, 40, 0);

    ActTab->ImgHum = lv_img_create(parent);
    lv_img_set_src(ActTab->ImgHum, &ImgHum);
    lv_obj_align(ActTab->ImgHum, LV_ALIGN_LEFT_MID, 0, 0);
    ActTab->HumMin =  lv_label_create(parent);
    lv_label_set_text(ActTab->HumMin, "");
    lv_obj_align(ActTab->HumMin, LV_ALIGN_CENTER, -20, 0);
    ActTab->Hum =  lv_label_create(parent);
    lv_label_set_text(ActTab->Hum, "");
    lv_obj_align(ActTab->Hum, LV_ALIGN_CENTER, 10, 0);
    ActTab->HumMax =  lv_label_create(parent);
    lv_label_set_text(ActTab->HumMax, "");
    lv_obj_align(ActTab->HumMax, LV_ALIGN_CENTER, 40, 0);

    ActTab->BtnReset = lv_btn_create(parent);
    lv_obj_add_event_cb(ActTab->BtnReset, TabTempResetHandler, LV_EVENT_CLICKED, NULL);
    lv_obj_align(ActTab->BtnReset, LV_ALIGN_BOTTOM_RIGHT, 0, 0);
    lv_obj_t *label = lv_label_create(ActTab->BtnReset);
    lv_label_set_text(label, "Reset");
    lv_obj_center(label);
}

static void TabFanSldr1CB(lv_event_t * e)
{
    lv_label_set_text_fmt(GUIObjs.TabFan.LblFan1, "%d%%", (int)lv_slider_get_value(GUIObjs.TabFan.SldrFan1));
}
static void TabFanSldr2CB(lv_event_t * e)
{
    lv_label_set_text_fmt(GUIObjs.TabFan.LblFan2, "%d%%", (int)lv_slider_get_value(GUIObjs.TabFan.SldrFan2));
}
static void TabFanAutoHandler(lv_event_t * e)
{

}
static void TabFan(lv_obj_t * parent)
{
    lv_obj_t *label;
    TabFanObjs_t *ActTab = &GUIObjs.TabFan;

    ActTab->SldrFan1 = lv_slider_create(parent);
    lv_obj_align(ActTab->SldrFan1, LV_ALIGN_TOP_LEFT, 0, 15);
    lv_obj_add_event_cb(ActTab->SldrFan1, TabFanSldr1CB, LV_EVENT_VALUE_CHANGED, NULL);
    label = lv_label_create(parent);
    lv_label_set_text(label, "Zuluft");
    lv_obj_align_to(label, ActTab->SldrFan1, LV_ALIGN_OUT_TOP_LEFT, 0, -5);
    ActTab->LblFan1 = lv_label_create(parent);
    lv_label_set_text(ActTab->LblFan1, "0%");
    lv_obj_align_to(ActTab->LblFan1, ActTab->SldrFan1, LV_ALIGN_OUT_RIGHT_MID, 0, 0);

    ActTab->SldrFan2 = lv_slider_create(parent);
    lv_obj_align(ActTab->SldrFan2, LV_ALIGN_TOP_LEFT, 0, 50);
    lv_obj_add_event_cb(ActTab->SldrFan2, TabFanSldr2CB, LV_EVENT_VALUE_CHANGED, NULL);
    label = lv_label_create(parent);
    lv_label_set_text(label, "Umluft");
    lv_obj_align_to(label, ActTab->SldrFan2, LV_ALIGN_OUT_TOP_LEFT, 0, -5);
    ActTab->LblFan2 = lv_label_create(parent);
    lv_label_set_text(ActTab->LblFan2, "0%");
    lv_obj_align_to(ActTab->LblFan2, ActTab->SldrFan2, LV_ALIGN_OUT_RIGHT_MID, 0, 0);

    ActTab->SwAuto =  lv_switch_create(parent);
    lv_obj_add_event_cb(ActTab->SwAuto, TabFanAutoHandler, LV_EVENT_ALL, NULL);
    lv_obj_align(ActTab->SwAuto, LV_ALIGN_BOTTOM_LEFT, 0, 0);
    label = lv_label_create(parent);
    lv_label_set_text(label, "Auto");
    lv_obj_align_to(label, ActTab->SwAuto, LV_ALIGN_OUT_RIGHT_MID, 0, 0);
}

static void ta_event_cb(lv_event_t * e)
{
    lv_obj_t * ta = lv_event_get_target(e);
    const char * txt = lv_textarea_get_text(ta);
    if(txt[0] >= '0' && txt[0] <= '9' &&
       txt[1] >= '0' && txt[1] <= '9' &&
       txt[2] != ':') {
        lv_textarea_set_cursor_pos(ta, 2);
        lv_textarea_add_char(ta, ':');
    }
}
static void TabLightAutoHandler(lv_event_t * e)
{

}
static void TabLightManualHandler(lv_event_t * e)
{

}
static void TabLightSldrCB(lv_event_t * e)
{
    lv_label_set_text_fmt(GUIObjs.TabLight.LblLight, "%d%%", (int)lv_slider_get_value(GUIObjs.TabLight.SldrLight));
}
static void TabLight(lv_obj_t * parent)
{
    lv_obj_t *label;
    TabLightObjs_t *ActTab = &GUIObjs.TabLight;

    ActTab->SldrLight = lv_slider_create(parent);
    lv_obj_align(ActTab->SldrLight, LV_ALIGN_TOP_LEFT, 0, 15);
    lv_obj_add_event_cb(ActTab->SldrLight, TabLightSldrCB, LV_EVENT_VALUE_CHANGED, NULL);
    label = lv_label_create(parent);
    lv_label_set_text(label, "Helligkeit");
    lv_obj_align_to(label, ActTab->SldrLight, LV_ALIGN_OUT_TOP_LEFT, 0, -5);
    ActTab->LblLight = lv_label_create(parent);
    lv_label_set_text(ActTab->LblLight, "0%");
    lv_obj_align_to(ActTab->LblLight, ActTab->SldrLight, LV_ALIGN_OUT_RIGHT_MID, 0, 0);

    ActTab->TxtOn = lv_textarea_create(parent);
    lv_obj_add_event_cb(ActTab->TxtOn, ta_event_cb, LV_EVENT_VALUE_CHANGED, NULL);
    lv_textarea_set_accepted_chars(ActTab->TxtOn, "0123456789:");
    lv_textarea_set_max_length(ActTab->TxtOn, 5);
    lv_textarea_set_one_line(ActTab->TxtOn, true);
    lv_textarea_set_text(ActTab->TxtOn, "");
    lv_obj_align(ActTab->TxtOn, LV_ALIGN_LEFT_MID, 0, 0);
    
    ActTab->TxtOff = lv_textarea_create(parent);
    lv_obj_add_event_cb(ActTab->TxtOff, ta_event_cb, LV_EVENT_VALUE_CHANGED, NULL);
    lv_textarea_set_accepted_chars(ActTab->TxtOff, "0123456789:");
    lv_textarea_set_max_length(ActTab->TxtOff, 5);
    lv_textarea_set_one_line(ActTab->TxtOff, true);
    lv_textarea_set_text(ActTab->TxtOff, "");
    lv_obj_align(ActTab->TxtOff, LV_ALIGN_RIGHT_MID, 0, 0);
    
    ActTab->SwAuto =  lv_switch_create(parent);
    lv_obj_add_event_cb(ActTab->SwAuto, TabLightAutoHandler, LV_EVENT_ALL, NULL);
    lv_obj_align(ActTab->SwAuto, LV_ALIGN_BOTTOM_LEFT, 0, 0);
    label = lv_label_create(parent);
    lv_label_set_text(label, "Auto");
    lv_obj_align_to(label, ActTab->SwAuto, LV_ALIGN_OUT_RIGHT_MID, 0, 0);

    ActTab->SwOnOff =  lv_switch_create(parent);
    lv_obj_add_event_cb(ActTab->SwOnOff, TabLightManualHandler, LV_EVENT_ALL, NULL);
    lv_obj_align(ActTab->SwOnOff, LV_ALIGN_BOTTOM_RIGHT, 0, 0);
    label = lv_label_create(parent);
    lv_label_set_text(label, "Manual");
    lv_obj_align_to(label, ActTab->SwOnOff, LV_ALIGN_OUT_LEFT_MID, 0, 0);
}

static void TabHeatMatHandler(lv_event_t * e)
{

}
static void TabHeatLEDHandler(lv_event_t * e)
{

}
static void TabHeatSldrDayCB(lv_event_t * e)
{
    lv_label_set_text_fmt(GUIObjs.TabHeat.LblDay, "%d%%", (int)lv_slider_get_value(GUIObjs.TabHeat.SldrDay));
}
static void TabHeatSldrNightCB(lv_event_t * e)
{
    lv_label_set_text_fmt(GUIObjs.TabHeat.LblNight, "%d%%", (int)lv_slider_get_value(GUIObjs.TabHeat.SldrNight));
}
static void TabHeatSldrHumCB(lv_event_t * e)
{
    lv_label_set_text_fmt(GUIObjs.TabHeat.LblHum, "%d%%", (int)lv_slider_get_value(GUIObjs.TabHeat.SldrHum));
}
void TabHeat(lv_obj_t * parent)
{
    lv_obj_t *label;
    TabHeatObjs_t *ActTab = &GUIObjs.TabHeat;
 
    ActTab->SwMat =  lv_switch_create(parent);
    lv_obj_add_event_cb(ActTab->SwMat, TabHeatMatHandler, LV_EVENT_ALL, NULL);
    lv_obj_align(ActTab->SwMat, LV_ALIGN_TOP_LEFT, 0, 0);
    label = lv_label_create(parent);
    lv_label_set_text(label, "Matte");
    lv_obj_align_to(label, ActTab->SwMat, LV_ALIGN_OUT_RIGHT_MID, 0, 0);

    ActTab->SwLED =  lv_switch_create(parent);
    lv_obj_add_event_cb(ActTab->SwLED, TabHeatLEDHandler, LV_EVENT_ALL, NULL);
    lv_obj_align(ActTab->SwLED, LV_ALIGN_TOP_RIGHT, 0, 0);
    label = lv_label_create(parent);
    lv_label_set_text(label, "LED");
    lv_obj_align_to(label, ActTab->SwLED, LV_ALIGN_OUT_LEFT_MID, 0, 0);

    ActTab->SldrDay = lv_slider_create(parent);
    lv_obj_align(ActTab->SldrDay, LV_ALIGN_LEFT_MID, 0, -15);
    lv_obj_add_event_cb(ActTab->SldrDay, TabHeatSldrDayCB, LV_EVENT_VALUE_CHANGED, NULL);
    label = lv_label_create(parent);
    lv_label_set_text(label, "Tag");
    lv_obj_align_to(label, ActTab->SldrDay, LV_ALIGN_OUT_TOP_LEFT, 0, -5);
    ActTab->LblDay = lv_label_create(parent);
    lv_label_set_text(ActTab->LblDay, "0%");
    lv_obj_align_to(ActTab->LblDay, ActTab->SldrDay, LV_ALIGN_OUT_RIGHT_MID, 0, 0);

    ActTab->SldrNight = lv_slider_create(parent);
    lv_obj_align(ActTab->SldrNight, LV_ALIGN_LEFT_MID, 0, 0);
    lv_obj_add_event_cb(ActTab->SldrNight, TabHeatSldrNightCB, LV_EVENT_VALUE_CHANGED, NULL);
    label = lv_label_create(parent);
    lv_label_set_text(label, "Nacht");
    lv_obj_align_to(label, ActTab->SldrNight, LV_ALIGN_OUT_TOP_LEFT, 0, -5);
    ActTab->LblNight = lv_label_create(parent);
    lv_label_set_text(ActTab->LblNight, "0%");
    lv_obj_align_to(ActTab->LblNight, ActTab->SldrNight, LV_ALIGN_OUT_RIGHT_MID, 0, 0);

    ActTab->SldrHum = lv_slider_create(parent);
    lv_obj_align(ActTab->SldrHum, LV_ALIGN_LEFT_MID, 0, 15);
    lv_obj_add_event_cb(ActTab->SldrHum, TabHeatSldrHumCB, LV_EVENT_VALUE_CHANGED, NULL);
    label = lv_label_create(parent);
    lv_label_set_text(label, "Luftfeuchtigkeit");
    lv_obj_align_to(label, ActTab->SldrHum, LV_ALIGN_OUT_TOP_LEFT, 0, -5);
    ActTab->LblHum = lv_label_create(parent);
    lv_label_set_text(ActTab->LblHum, "0%");
    lv_obj_align_to(ActTab->LblHum, ActTab->SldrHum, LV_ALIGN_OUT_RIGHT_MID, 0, 0);
}

void StyleInit()
{
    lv_style_init(&Style.Slider);
    lv_style_set_width(&Style.Slider, 110);
    lv_style_set_height(&Style.Slider, 5);

    lv_style_init(&Style.Button);
    lv_style_set_width(&Style.Button, 55);
    lv_style_set_height(&Style.Button, 15);

    lv_style_init(&Style.TextArea);
    lv_style_set_width(&Style.TextArea, 55);
    lv_style_set_height(&Style.TextArea, 15);

    lv_style_init(&Style.Switch);
    lv_style_set_width(&Style.Switch, 40);
    lv_style_set_height(&Style.Switch, 15);
}

void theme_cb(struct _lv_theme_t *th , lv_obj_t *obj)
{
    if(lv_obj_check_type(obj, &lv_img_class))
    {
        lv_obj_set_style_img_recolor_opa(obj, 255, 0);
        lv_obj_set_style_img_recolor(obj, lv_color_white(), 0);
    }
    if (lv_obj_check_type(obj, &lv_btn_class))
    {
        lv_obj_add_style(obj, &Style.Button, 0);
    }
    if (lv_obj_check_type(obj, &lv_textarea_class))
    {
        lv_obj_add_style(obj, &Style.TextArea, 0);
    }
    if (lv_obj_check_type(obj, &lv_slider_class))
    {
        lv_obj_add_style(obj, &Style.Slider, 0);
    }
    if (lv_obj_check_type(obj, &lv_switch_class))
    {
        lv_obj_add_style(obj, &Style.Switch, 0);
    }
}

lv_theme_t th_new;
void CreateGUI()
{
    StyleInit();
//    lv_obj_t * scr = lv_scr_act();
    lv_theme_t* th_pr = lv_theme_default_init(lv_disp_get_default(), lv_palette_main(LV_PALETTE_CYAN), lv_palette_main(LV_PALETTE_BLUE), true, &lv_font_montserrat_10);
    th_new = *th_pr;
    lv_theme_set_parent(&th_new, th_pr);

    lv_theme_set_apply_cb(&th_new, theme_cb);

    lv_disp_set_theme(lv_disp_get_default(), &th_new); /*Assign the theme to the display*/

    g = lv_group_create();
    lv_group_set_default(g);

    lv_indev_set_group(my_indev, g);



    GUIObjs.TabView = lv_tabview_create(lv_scr_act(), LV_DIR_TOP, 25);

    GUIObjs.TabTemp.Tab = lv_tabview_add_tab(GUIObjs.TabView, "Temp");
    GUIObjs.TabFan.Tab = lv_tabview_add_tab(GUIObjs.TabView, "Fan");
    GUIObjs.TabLight.Tab = lv_tabview_add_tab(GUIObjs.TabView, "Light");
    GUIObjs.TabHeat.Tab = lv_tabview_add_tab(GUIObjs.TabView, "Heat");

    TabTemp(GUIObjs.TabTemp.Tab);
    TabFan(GUIObjs.TabFan.Tab);
    TabLight(GUIObjs.TabLight.Tab);
    TabHeat(GUIObjs.TabHeat.Tab);

//    lv_group_focus_obj(lv_obj_get_child(t2, 0));
}
