#ifdef __has_include
    #if __has_include("lvgl.h")
        #ifndef LV_LVGL_H_INCLUDE_SIMPLE
            #define LV_LVGL_H_INCLUDE_SIMPLE
        #endif
    #endif
#endif

#if defined(LV_LVGL_H_INCLUDE_SIMPLE)
    #include "lvgl.h"
#else
    #include "lvgl/lvgl.h"
#endif


#ifndef LV_ATTRIBUTE_MEM_ALIGN
#define LV_ATTRIBUTE_MEM_ALIGN
#endif

#ifndef LV_ATTRIBUTE_IMG_HUM
#define LV_ATTRIBUTE_IMG_HUM
#endif

const LV_ATTRIBUTE_MEM_ALIGN LV_ATTRIBUTE_LARGE_CONST LV_ATTRIBUTE_IMG_HUM uint8_t Hum_map[] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0xef, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0xdf, 0xff, 0xdf, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x90, 0xff, 0xa0, 0xff, 0x90, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0xff, 0xa0, 0x00, 0xa0, 0xff, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0xef, 0xd0, 0x10, 0x00, 0x10, 0xd0, 0xef, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x10, 0xcf, 0xf0, 0x30, 0x00, 0x00, 0x00, 0x30, 0xf0, 0xcf, 0x10, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0xb0, 0xff, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x50, 0xff, 0x9f, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x70, 0xff, 0x90, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x90, 0xff, 0x70, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x30, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0xff, 0x30, 0x00, 0x00, 
  0x00, 0x00, 0xd0, 0xf0, 0x20, 0x10, 0x6f, 0x7f, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0xf0, 0xcf, 0x00, 0x00, 
  0x00, 0x80, 0xff, 0x70, 0x00, 0xd0, 0xff, 0xff, 0xef, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0xff, 0x80, 0x00, 
  0x10, 0xef, 0xe0, 0x00, 0x40, 0xff, 0x80, 0x30, 0xff, 0x80, 0x00, 0x00, 0x60, 0xff, 0x20, 0x00, 0xe0, 0xef, 0x10, 
  0x60, 0xff, 0x60, 0x00, 0x20, 0xff, 0xcf, 0xaf, 0xff, 0x60, 0x00, 0x60, 0xff, 0xb0, 0x00, 0x00, 0x60, 0xff, 0x60, 
  0xb0, 0xf0, 0x00, 0x00, 0x00, 0x60, 0xf0, 0xff, 0x90, 0x00, 0x60, 0xff, 0xd0, 0x10, 0x00, 0x00, 0x00, 0xff, 0xb0, 
  0xe0, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0xff, 0xb0, 0x10, 0x00, 0x00, 0x00, 0x00, 0xc0, 0xc0, 
  0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0xff, 0xa0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0xff, 
  0xff, 0xa0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0xff, 0xa0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0xe0, 
  0xc0, 0xcf, 0x00, 0x00, 0x00, 0x00, 0x60, 0xff, 0xa0, 0x00, 0x40, 0xbf, 0xaf, 0x20, 0x00, 0x00, 0x00, 0xd0, 0xc0, 
  0x90, 0xff, 0x20, 0x00, 0x00, 0x60, 0xff, 0xa0, 0x00, 0x30, 0xff, 0xe0, 0xf0, 0xdf, 0x10, 0x00, 0x30, 0xff, 0x80, 
  0x20, 0xff, 0xbf, 0x00, 0x30, 0xff, 0xa0, 0x00, 0x00, 0x80, 0xff, 0x20, 0x60, 0xff, 0x40, 0x00, 0xc0, 0xff, 0x10, 
  0x00, 0x90, 0xff, 0x60, 0x00, 0x40, 0x00, 0x00, 0x00, 0x40, 0xff, 0xcf, 0xef, 0xf0, 0x10, 0x60, 0xff, 0x90, 0x00, 
  0x00, 0x10, 0xd0, 0xff, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0xc0, 0xc0, 0x30, 0x60, 0xff, 0xd0, 0x00, 0x00, 
  0x00, 0x00, 0x10, 0xd0, 0xff, 0xaf, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0xbf, 0xff, 0xd0, 0x10, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x10, 0x90, 0xff, 0xff, 0xcf, 0xbf, 0x7f, 0x9f, 0xdf, 0xff, 0xff, 0x90, 0x10, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x70, 0xc0, 0xf0, 0xff, 0xff, 0xc0, 0x70, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 
};

const lv_img_dsc_t ImgHum = {
  .header.cf = LV_IMG_CF_ALPHA_8BIT,
  .header.always_zero = 0,
  .header.reserved = 0,
  .header.w = 19,
  .header.h = 25,
  .data_size = 475,
  .data = Hum_map,
};
