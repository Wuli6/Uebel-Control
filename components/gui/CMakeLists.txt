idf_component_register(SRCS "gui.c" "Img/temp.c" "Img/hum.c"
                       INCLUDE_DIRS "."
                       PRIV_INCLUDE_DIRS ""
                       PRIV_REQUIRES "lvgl"
                       REQUIRES "AHT10")